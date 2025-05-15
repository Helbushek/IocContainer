#pragma once

#include <QUuid>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
#include "DataReader.h"

class SqliteReader : public DataReader
{
  public:
    // Возвращает либо заполненный список <date, value>, либо nullptr при ошибке
    QList<QStringList>* read(const QString &dbPath) override
    {
        // 1) Создаём уникальное имя подключения
        const QString connName = QUuid::createUuid().toString();

               // 2) Открываем соединение
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbPath);
        if (!db.open()) {
            qCritical() << "[SqliteReader] Ошибка открытия базы данных:" << db.lastError().text();
                QSqlDatabase::removeDatabase(connName);
            return nullptr;
        }

               // 3) Получаем список таблиц
        const QStringList tables = db.tables();
        if (tables.isEmpty()) {
            qWarning() << "[SqliteReader] В базе нет ни одной таблицы:" << dbPath;
            db.close();
            QSqlDatabase::removeDatabase(connName);
            return nullptr;
        }

               // 4) Выполняем запрос по первой таблице
        const QString table = tables.first();
        QSqlQuery query(db);
        if (!query.exec(QStringLiteral("SELECT * FROM \"%1\"").arg(table))) {
            qCritical() << "[SqliteReader] Ошибка выполнения запроса:"
                        << query.lastError().text();
            db.close();
            QSqlDatabase::removeDatabase(connName);
            return nullptr;
        }

               // 5) Собираем результат
        auto *result = new QList<QStringList>();
        const int columnCount = query.record().count();
        while (query.next()) {
            QStringList row;
            row.reserve(columnCount);
            for (int i = 0; i < columnCount; ++i) {
                row << query.value(i).toString();
            }
            result->append(row);
        }

               // 6) Закрываем соединение и удаляем
        db.close();
        QSqlDatabase::removeDatabase(connName);
        return result;
    }
};
