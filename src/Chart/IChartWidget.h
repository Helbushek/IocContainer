#pragma once
#include <QWidget>
#include <QDateTime>

class IGraphWidget : public QWidget
{
    Q_OBJECT
  public:
    using QWidget::QWidget; // наследуем конструктор

    virtual void setData(const QList<QStringList *> &data) = 0;

    virtual QDateTime parseDate(const QString &str) {
        // Попробовать обычный формат с временем
        QDateTime dt = QDateTime::fromString(str, "dd.MM.yyyy HH:mm");
        dt.setTimeSpec(Qt::UTC);
        if (dt.isValid())
            return dt;

               // Разделить строку: первая часть — дата, вторая — возможно минуты
        QStringList parts = str.trimmed().split(QRegExp("\\s+"));
        if (parts.size() >= 2) {
            QString datePart = parts.at(0);
            bool ok = false;
            int minutes = parts.at(1).toInt(&ok);
            if (ok) {
                QDate date = QDate::fromString(datePart, "dd.MM.yyyy");
                if (date.isValid()) {
                    QTime time = QTime(0, 0).addSecs(minutes * 60);
                    QDateTime combined(date, time, Qt::UTC);
                    return combined;
                }
            }
        }

               // Попробовать просто дату без времени
        QDate dateOnly = QDate::fromString(str.left(10), "dd.MM.yyyy");
        if (dateOnly.isValid())
            return QDateTime(dateOnly, QTime(0, 0), Qt::UTC);

               // Всё неудачно
        return QDateTime();
    }
};
