#pragma once
#include <includes/qcustomplot/qcustomplot.h>
#include <src/Chart/Style/IChartStyle.h>
#include <QWidget>
#include <QDateTime>

class IGraphWidget : public QWidget
{
    Q_OBJECT
  public:
    using QWidget::QWidget; // наследуем конструктор

    virtual void setData(const QList<QStringList> &data) = 0;

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

    void enableInteractions(QCustomPlot *plot) {
        plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

        connect(plot, &QCustomPlot::mouseWheel, this, [=](QWheelEvent *event) {
            if (event->modifiers() & Qt::ShiftModifier)
            {
                // Зум только по X
                plot->axisRect()->setRangeZoomAxes(plot->xAxis, nullptr);
            }
            else if (event->modifiers() & Qt::ControlModifier)
            {
                // Зум только по Y
                plot->axisRect()->setRangeZoomAxes(nullptr, plot->yAxis);
            }
            else
            {
                // Зум по обеим осям
                plot->axisRect()->setRangeZoomAxes(plot->xAxis, plot->yAxis);
            }
        });
    }

    void setTitle(const QString &title) {
        if (!titleElement_) {
            // Вставляем новую строку для заголовка
            plot_->plotLayout()->insertRow(0);
            titleElement_ = new QCPTextElement(plot_, title, QFont("sans", 12, QFont::Bold));
            plot_->plotLayout()->addElement(0, 0, titleElement_);
        } else {
            // Просто обновляем текст
            titleElement_->setText(title);
        }
        plot_->replot();
    }

    void setAxisLabels(const QString &x, const QString &y) {
        plot_->xAxis->setLabel(x);
        plot_->yAxis->setLabel(y);
    }

    void setLegendVisible(bool visible) {
        plot_->legend->setVisible(visible);
        if (visible) {
            plot_->axisRect()->insetLayout()->setInsetAlignment(
                0, Qt::AlignTop|Qt::AlignRight);
        }
    }

    void setStyle(IChartStyle* st) {
        style_ = st;
    }

    void applyChartStyle(IChartStyle *style)
    {
        setStyle(style);
        style_->configure(this);
        update();
        plot_->replot();
    }

    QCustomPlot* plot() const { return plot_; }
  protected:
    QCustomPlot *plot_;
    QCPTextElement   *titleElement_ = nullptr;
    IChartStyle* style_;
};
