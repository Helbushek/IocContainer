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

class GrayScaleChartStyle : public IChartStyle {
  public:
    // Этот метод вызывается из PieChartGraph::paintEvent перед рисованием всех секторов
    QColor sliceColor(int index, const QString&, double) const override {
        // Очень светло-серые → к тёмно-серым
        int gray = 220 - (index * 10) % 170;
        return QColor(gray, gray, gray);
    }

           // Настройка рисователя перед paintEvent
    void apply(QPainter &p, IGraphWidget*) const override {
        // для секторов фон не нужен, всё в sliceColor
        p.setPen(QPen(Qt::black, 1));
    }

           // Дополнительная конфигурация самого виджета
    void configure(IGraphWidget *w) const override {
        if (auto *c = dynamic_cast<CustomPlotGraph*>(w)) {
            auto *plot = c->plot();
            plot->setBackground(QBrush(Qt::white));
            plot->graph(0)->setPen(QPen(Qt::darkGray, 2, Qt::DashLine));
            plot->xAxis->setBasePen(QPen(Qt::darkGray));
            plot->yAxis->setBasePen(QPen(Qt::darkGray));
        }
        else if (auto *s = dynamic_cast<ScatterGraph*>(w)) {
            auto *plot = s->plot();
            plot->setBackground(QBrush(Qt::white));
            auto *g = plot->graph(0);
            g->setLineStyle(QCPGraph::lsNone);
            QCPScatterStyle scatter(QCPScatterStyle::ssCircle, 6);
            scatter.setPen(QPen(Qt::darkGray));
            scatter.setBrush(QBrush(Qt::darkGray));
            g->setScatterStyle(scatter);
            plot->xAxis->setBasePen(QPen(Qt::darkGray));
            plot->yAxis->setBasePen(QPen(Qt::darkGray));
        }
        else if (auto *b = dynamic_cast<BarChartGraph*>(w)) {
            auto *plot = b->plot();
            plot->setBackground(QBrush(Qt::white));
            auto *bars = b->bars();
            bars->setBrush(QBrush(Qt::lightGray));
            bars->setPen(QPen(Qt::darkGray));
            plot->xAxis->setBasePen(QPen(Qt::darkGray));
            plot->yAxis->setBasePen(QPen(Qt::darkGray));
        }
        else if (auto *p = dynamic_cast<PieChartGraph*>(w)) {
            // фон под диаграмму
            p->setAutoFillBackground(true);
            QPalette pal = p->palette();
            pal.setColor(QPalette::Window, Qt::white);
            p->setPalette(pal);
            // можно добавить рамку вокруг круга:
            p->setContentsMargins(10,10,10,10);
        }
    }
};
