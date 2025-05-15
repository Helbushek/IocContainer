#pragma once

#include "src/DataReaders/DataReader.h"
#include <QString>
#include <QList>
#include <QStringList>

class CsvReader : public DataReader {
  public:
    // Разделитель полей — ',' или ';'
    explicit CsvReader(QChar delimiter = ',')
        : delim_(delimiter) {}

    QList<QStringList>* read(const QString &filePath) override;

  private:
    QChar delim_;
};
