// src/Chart/BarChartGraph.h
#pragma once

#include <src/Chart/IGraphWidget.h>
#include <includes/qcustomplot/qcustomplot.h>
#include <src/Chart/IGraphFactory.h>
#include <src/IOC_Container.h>

class BarChartGraph : public IGraphWidget {
    Q_OBJECT
        public:
                 explicit BarChartGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {
        plot_ = new QCustomPlot(this);
        setAxelsSettings();
        bars_ = new QCPBars(plot_->xAxis, plot_->yAxis);
        bars_->setWidth(0.8 * 3600); // ширина ~0.8 часа (секунды)

        enableInteractions(plot_);

        auto *lay = new QVBoxLayout(this);
        lay->addWidget(plot_);
    }

    void setData(const QList<QStringList> &data) override {
        QVector<double> keys, values;
        for (auto row : data) {
            if (row.size() < 2)
                continue;
            auto dt = parseDate(row.value(0));
            keys.append(dt.toSecsSinceEpoch());
            values.append(row.value(1).toDouble());
        }
        bars_->setData(keys, values);
        plot_->rescaleAxes();
        plot_->replot();
    }

    QCustomPlot* plot() const { return plot_; }

    void applyChartStyle(IChartStyle *style)
    {
        setStyle(style);
        style_->configure(this);
        update();
        plot_->replot();
    }
    QCPBars *bars() const
    {
        return bars_;
    }

  private:
    QCPBars    *bars_;
};

class BarChartGraphFactory : public IGraphFactory {
  public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<BarChartGraph>(parent);
    }
};
