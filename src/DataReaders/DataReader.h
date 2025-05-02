#pragma once
#include <QStringList>
#include <QDateTime>

class DataReader
{
  public:
    virtual QList<QStringList *> *read(const QString &dbPath) = 0;
};
