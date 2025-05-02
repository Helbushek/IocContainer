#pragma once
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonArray>
#include <QDebug>
#include <string>

#include <src/DataReaders/DataReader.h>

class JsonReader : public DataReader
{
  public:
    QList<QStringList *> *read(const QString &filePath)
    {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Не удалось открыть файл:" << filePath;
            return nullptr;
        }

        QByteArray jsonData = file.readAll();
        file.close();

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "Ошибка разбора JSON:" << parseError.errorString();
            return nullptr;
        }

        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            qDebug() << "JSON-объект:" << obj;
                // Пример: доступ к значению по ключу
                if (obj.contains("name"))
                    qDebug() << "name:" << obj["name"].toString();
        } else if (doc.isArray()) {
            QJsonArray arr = doc.array();
            qDebug() << "JSON-массив:" << arr;
        }
    }
};
