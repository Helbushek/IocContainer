#pragma once
#include "CsvReader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <src/DataReaders/JsonReader.h>


QList<QStringList>* CsvReader::read(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[CsvReader] Не удалось открыть" << filePath;
        return nullptr;
    }

    auto * result = new QList<QStringList>();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.trimmed().isEmpty())
            continue;
        // Разбиваем по delim_ и убираем кавычки
        QStringList fields = line.split(delim_);
        for (auto &f : fields)
            f = f.trimmed().remove('\"');
        // Ожидаем минимум два поля: дата и значение
        if (fields.size() >= 2) {
            bool ok = false;
            fields[1].toDouble(&ok);
            if (ok) {
                result->append(QStringList{ fields[0], fields[1] });
            }
        }
    }
    file.close();

    if (result->isEmpty())
        qWarning() << "[CsvReader] Ни одной валидной записи в" << filePath;
    return result;
}

REGISTER_DATA_READER("csv", CsvReader)
