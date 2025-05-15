#pragma once
#include <src/Chart/CustomGraphs/CustomPlotGraph.h>

QCustomPlot* CustomPlotGraph::plot() const { return this->plot_; }
CustomPlotGraph::CustomPlotGraph(QWidget *parent) : IGraphWidget(parent)
{
    plot_ = new QCustomPlot(this);
    plot_->addGraph();
    setAxelsSettings();
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(plot_);

    enableInteractions(plot_);
}

void CustomPlotGraph::setData(const QList<QStringList> &data)
{
    QVector<double> x, y;
    for (auto row : data) {
        if (row.size() < 2)
            continue;
        QDateTime dt = parseDate(row.value(0));
        x.append(dt.toSecsSinceEpoch());
        y.append(row.value(1).toDouble());
    }
    plot_->graph(0)->setData(x, y);
    plot_->rescaleAxes();
    plot_->replot();
}

REGISTER_GRAPH_FACTORY("Line", CustomPlotGraph)
