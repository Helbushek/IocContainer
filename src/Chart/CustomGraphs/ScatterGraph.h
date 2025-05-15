#pragma once

#include <src/Chart/IGraphWidget.h>
#include <includes/qcustomplot/qcustomplot.h>
#include <src/Chart/IGraphFactory.h>
#include <src/IOC_Container.h>
#include <src/Chart/GraphFactoryRegistry.h>

class ScatterGraph : public IGraphWidget {
    Q_OBJECT
        public:
                 explicit ScatterGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {
        plot_ = new QCustomPlot(this);
        plot_->addGraph();
        // отключаем линии, оставляем только точки
        plot_->graph(0)->setLineStyle(QCPGraph::lsNone);
        plot_->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);

        enableInteractions(plot_);

        auto *lay = new QVBoxLayout(this);
        lay->addWidget(plot_);
    }

    void setData(const QList<QStringList> &data) override {
        QVector<double> x, y;
        for (auto row : data) {
            if (row.size() < 2)
                continue;
            auto dt = parseDate(row.value(0));
            x.append(dt.toSecsSinceEpoch());
            y.append(row.value(1).toDouble());
        }
        plot_->graph(0)->setData(x, y);
        plot_->rescaleAxes();
        plot_->replot();
    }

    QCustomPlot* plot() const { return plot_; }
};

class ScatterGraphFactory : public IGraphFactory {
  public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<ScatterGraph>(parent);
    }
};
