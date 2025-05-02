#pragma once
#include <src/Chart/IChartWidget.h>
#include <includes/qcustomplot/qcustomplot.h>

class CustomPlotGraph : public IGraphWidget {
    Q_OBJECT
        public:

    CustomPlotGraph(QWidget *parent = nullptr) : IGraphWidget(parent)
    {
        plot = new QCustomPlot(this);
        plot->addGraph();

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(plot);

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("dd.MM.yyyy HH:mm"); // формат вывода
        plot->xAxis->setTicker(dateTicker);
               // Конфигурация осей и зума
        plot->xAxis->setLabel("Дата и Время");
        plot->yAxis->setLabel("Значение");
        plot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
        plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);

        connect(plot, &QCustomPlot::mouseWheel, parent, [=](QWheelEvent *event) {
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

    void setData(const QList<QStringList*> &data) override
    {
        QVector<double> x, y;
        for (auto list : data) {
            QDateTime dt = parseDate(list->at(0));
            x.append(dt.toSecsSinceEpoch());
            y.append(list->at(1).toDouble());
        }
        plot->graph(0)->setData(x, y);
        plot->rescaleAxes();
        plot->replot();
    }

  private:
    QCustomPlot *plot;
};
