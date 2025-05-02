#pragma once

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

class SqliteReader
{
  protected:
  public:
    static QList<QStringList *> *read(const QString &dbPath)
    {
        // Создаем соединение с базой
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbPath);

        if (!db.open())
        {
            qCritical() << "Ошибка открытия базы данных:" << db.lastError().text();
            return nullptr;
        }

        QStringList tables = db.tables();
        for (const QString &table : tables)
        {
            qDebug() << "Таблица:" << table;
        }

        // Пример запроса: читаем все из таблицы example_table
        QSqlQuery query("SELECT * FROM " + tables.at(0));
        if (!query.exec())
        {
            qCritical() << "Ошибка выполнения запроса:" << query.lastError().text();
            return nullptr;
        }

        QList<QStringList *> *result = new QList<QStringList *>();
        // Читаем и выводим строки
        while (query.next())
        {
            QStringList *rowData = new QStringList();
            for (int i = 0; i < query.record().count(); ++i)
            {
                (*rowData) << query.value(i).toString();
                result->append(rowData);
            }
        }

        db.close();
        return result;
    }
};
