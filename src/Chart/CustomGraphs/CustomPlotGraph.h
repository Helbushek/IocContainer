#pragma once
#include "includes/qcustomplot/qcustomplot.h"
#include <src/Chart/IGraphWidget.h>
#include <src/Chart/IGraphFactory.h>
#include <src/IOC_Container.h>
#include <src/Chart/GraphFactoryRegistry.h>

class CustomPlotGraph : public IGraphWidget {
    Q_OBJECT
        public:
          QCustomPlot *plot() const;
    CustomPlotGraph(QWidget *parent = nullptr);

    void setData(const QList<QStringList> &data) override;

    void applyChartStyle(IChartStyle *style)
    {
        setStyle(style);
        style_->configure(this);
        update();
        plot_->replot();
    }
};

class CustomGraphWidgetFactory : public IGraphFactory {
  public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<CustomPlotGraph>(parent);
    }
};


