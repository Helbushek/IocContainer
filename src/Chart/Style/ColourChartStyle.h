#pragma once

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <src/Chart/Style/IChartStyle.h>
#include <src/Chart/CustomGraphs/PieChartGraph.h>
#include <src/Chart/CustomGraphs/CustomPlotGraph.h>
#include <src/Chart/CustomGraphs/ScatterGraph.h>
#include <src/Chart/CustomGraphs/BarChartGraph.h>
#include <includes/qcustomplot/qcustomplot.h>

class ColourChartStyle : public IChartStyle {
  public:
    // Выбираем яркие, но не кричащие цвета
    QColor sliceColor(int index, const QString&, double) const override {
        static const QVector<int> hues = { 0,  30,  60,  120, 180, 210, 270, 300 };
        int h = hues.at(index % hues.size());
        return QColor::fromHsv(h, 200, 220);
    }

    void apply(QPainter &p, IGraphWidget*) const override {
        // общая обводка для секторов; заливка берётся из sliceColor
        p.setPen(QPen(Qt::black, 1));
    }

    void configure(IGraphWidget *w) const override {
        if (auto *c = dynamic_cast<CustomPlotGraph*>(w)) {
            auto *plot = c->plot();
            plot->setBackground(QBrush(Qt::white));
            plot->graph(0)->setPen(QPen(Qt::blue, 2));
            plot->xAxis->setBasePen(QPen(Qt::black));
            plot->yAxis->setBasePen(QPen(Qt::black));
        }
        else if (auto *s = dynamic_cast<ScatterGraph*>(w)) {
            auto *plot = s->plot();
            plot->setBackground(QBrush(Qt::white));
            auto *g = plot->graph(0);
            g->setLineStyle(QCPGraph::lsNone);
            QCPScatterStyle scatter(QCPScatterStyle::ssCircle, Qt::blue, Qt::blue, 6);
            g->setScatterStyle(scatter);
            plot->xAxis->setBasePen(QPen(Qt::black));
            plot->yAxis->setBasePen(QPen(Qt::black));
        }
        else if (auto *b = dynamic_cast<BarChartGraph*>(w)) {
            auto *plot = b->plot();
            plot->setBackground(QBrush(Qt::white));
            auto *bars = b->bars();
            bars->setBrush(QBrush(Qt::blue));
            bars->setPen(QPen(Qt::black));
            plot->xAxis->setBasePen(QPen(Qt::black));
            plot->yAxis->setBasePen(QPen(Qt::black));
        }
        else if (auto *p = dynamic_cast<PieChartGraph*>(w)) {
            // ставим белый фон
            p->setAutoFillBackground(true);
            QPalette pal = p->palette();
            pal.setColor(QPalette::Window, Qt::white);
            p->setPalette(pal);
            // отступы, чтобы круг не прилипал к краю
            p->setContentsMargins(10,10,10,10);
        }
    }
};
