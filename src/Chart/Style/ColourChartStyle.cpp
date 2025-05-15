#pragma once

#include "src/Chart/Style/ColourChartStyle.h"

#include "src/Chart/CustomGraphs/CustomPlotGraph.h"
#include "src/Chart/CustomGraphs/ScatterGraph.h"
#include "src/Chart/CustomGraphs/BarChartGraph.h"
#include "src/Chart/CustomGraphs/PieChartGraph.h"

QColor ColourChartStyle::sliceColor(int index, const QString &, double) const
{
    static const QVector<int> hues = {0, 30, 60, 120, 180, 210, 270, 300};
    int h = hues.at(index % hues.size());
    return QColor::fromHsv(h, 200, 220);
}

// обводка/общий пен
void ColourChartStyle::apply(QPainter &p, IGraphWidget *) const
{
    p.setPen(QPen(Qt::black, 1));
}

// настройка виджета сразу после создания/смены
void ColourChartStyle::configure(IGraphWidget *w) const
{
    if (auto *c = dynamic_cast<CustomPlotGraph *>(w))
    {
        auto *plot = c->plot();
        plot->setBackground(QBrush(Qt::white));
        plot->graph(0)->setPen(QPen(Qt::blue, 1));
        plot->graph(0)->setBrush(Qt::NoBrush);
        plot->xAxis->setBasePen(QPen(Qt::black));
        plot->yAxis->setBasePen(QPen(Qt::black));
        c->enableInteractions(plot);
    }
    else if (auto *s = dynamic_cast<ScatterGraph *>(w))
    {
        auto *plot = s->plot();
        plot->setBackground(QBrush(Qt::white));
        auto *g = plot->graph(0);
        g->setLineStyle(QCPGraph::lsNone);
        QCPScatterStyle style(QCPScatterStyle::ScatterShape::ssCircle, Qt::blue, Qt::blue, 6);
        style.setBrush(Qt::NoBrush);
        style.setPen(QPen(Qt::blue, 1));
        g->setScatterStyle(style);
        plot->xAxis->setBasePen(QPen(Qt::black));
        plot->yAxis->setBasePen(QPen(Qt::black));
        s->enableInteractions(plot);
    }
    else if (auto *b = dynamic_cast<BarChartGraph*>(w)) {
        auto *plot = b->plot();
        plot->setBackground(QBrush(Qt::white));
        auto *bars = b->bars();
        bars->setBrush(QBrush(Qt::blue));
        bars->setPen(QPen(Qt::black));
        plot->xAxis->setBasePen(QPen(Qt::black));
        plot->yAxis->setBasePen(QPen(Qt::black));
        b->enableInteractions(plot);
    }
    else if (auto *p = dynamic_cast<PieChartGraph*>(w)) {
        p->setAutoFillBackground(true);
        QPalette pal = p->palette();
        pal.setColor(QPalette::Window, Qt::white);
        p->setPalette(pal);
        p->setContentsMargins(10, 10, 10, 10);
    }
}
