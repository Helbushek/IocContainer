#pragma once

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
#include <QStringList>
#include "DataReader.h"
#include <src/DataReaders/DataReaderManager.h>

class JsonReader : public DataReader
{
  public:
    // Возвращает либо заполненный список <date, value>, либо nullptr при ошибке
    QList<QStringList>* read(const QString &filePath)
    {
        QFile f(filePath);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "[JsonReader] Не удалось открыть" << filePath;
            return nullptr;
        }
        const auto raw = f.readAll();
        f.close();

        QJsonParseError err;
        auto doc = QJsonDocument::fromJson(raw, &err);
        if (err.error != QJsonParseError::NoError) {
            qWarning() << "[JsonReader] Ошибка разбора JSON:" << err.errorString();
                                                                              return nullptr;
        }

               // Ищем корневой массив
        QJsonArray arr;
        if (doc.isArray()) {
            arr = doc.array();
        } else if (doc.isObject()) {
            auto obj = doc.object();
            // если есть поле "data" → берём его
            if (obj.contains("data") && obj["data"].isArray()) {
                arr = obj["data"].toArray();
            } else {
                // иначе — ищем первое поле-массив
                for (auto it = obj.begin(); it != obj.end(); ++it) {
                    if (it.value().isArray()) {
                        arr = it.value().toArray();
                        break;
                    }
                }
            }
        }
        if (arr.isEmpty()) {
            qWarning() << "[JsonReader] Ожидался JSON-массив в корне или в «data»";
            return nullptr;
        }

        auto *result = new QList<QStringList>();
        result->reserve(arr.size());

               // Проходим по всем элементам массива
        for (const auto &val : arr) {
            QString date;
            double  v = std::numeric_limits<double>::quiet_NaN();

                   // 1) Если элемент — массив [time, value]
            if (val.isArray()) {
                auto sub = val.toArray();
                if (sub.size() >= 2 && sub[0].isString() && sub[1].isDouble()) {
                    date = sub[0].toString();
                    v    = sub[1].toDouble();
                }
            }
            // 2) Если элемент — объект { "Time":…, "Value":… } (ключи не строго регистрозависимые)
            else if (val.isObject()) {
                auto objElem = val.toObject();
                // ищем ключы по привычным именам
                for (auto key : QStringList{ "Time","time","Date","date" }) {
                    if (objElem.contains(key) && objElem[key].isString()) {
                        date = objElem[key].toString();
                        break;
                    }
                }
                for (auto key : QStringList{ "Value","value","Val","val" }) {
                    if (objElem.contains(key) && objElem[key].isDouble()) {
                        v = objElem[key].toDouble();
                        break;
                    }
                }
            }

                   // Если оба поля валидны — добавляем в результат
            if (!date.isEmpty() && !qIsNaN(v)) {
                result->append(QStringList{ date, QString::number(v) });
            }
        }

        if (result->isEmpty()) {
            qWarning() << "[JsonReader] Не найдено ни одной валидной записи в" << filePath;
        }
        return result;
    }
};


