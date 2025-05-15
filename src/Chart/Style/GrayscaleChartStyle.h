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
        // Вспомогательная ламбда для настройки осей
        auto setupAxes = [](QCustomPlot *plot){
            plot->xAxis->grid()->setVisible(true);
            plot->yAxis->grid()->setVisible(true);
            plot->xAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DashLine));
            plot->yAxis->grid()->setPen(QPen(Qt::gray, 1, Qt::DashLine));
            plot->xAxis->setBasePen(QPen(Qt::darkGray, 1));
            plot->yAxis->setBasePen(QPen(Qt::darkGray, 1));
            plot->xAxis->setTickPen(QPen(Qt::darkGray, 1));
            plot->yAxis->setTickPen(QPen(Qt::darkGray, 1));
            plot->xAxis->setSubTickPen(QPen(Qt::lightGray, 1));
            plot->yAxis->setSubTickPen(QPen(Qt::lightGray, 1));
        };

        if (auto *c = dynamic_cast<CustomPlotGraph*>(w)) {
            auto *plot = c->plot();
            // Нежный серый фон
            plot->setBackground(QBrush(QColor(240,240,240)));
            // Основная линия графика — чуть более темная
            plot->graph(0)->setPen(QPen(Qt::darkGray, 2));
            setupAxes(plot);
            c->enableInteractions(plot);
        }
        else if (auto *s = dynamic_cast<ScatterGraph*>(w)) {
            auto *plot = s->plot();
            plot->setBackground(QBrush(QColor(240,240,240)));
            auto *g = plot->graph(0);
            g->setLineStyle(QCPGraph::lsNone);
            // Точки чуть посветлее, без заливки
            QCPScatterStyle scatter(QCPScatterStyle::ssCircle, 8);
            scatter.setPen(QPen(Qt::darkGray, 2));
            scatter.setBrush(QBrush(Qt::lightGray));
            g->setScatterStyle(scatter);
            setupAxes(plot);
            s->enableInteractions(plot);
        }
        else if (auto *b = dynamic_cast<BarChartGraph*>(w)) {
            auto *plot = b->plot();
            plot->setBackground(QBrush(QColor(240,240,240)));
            auto *bars = b->bars();
            // Заливка баров — светло-серая, рамка — темно-серая
            bars->setBrush(QBrush(QColor(200,200,200)));
            bars->setPen(QPen(Qt::darkGray, 1));
            setupAxes(plot);
            b->enableInteractions(plot);
        }
        else if (auto *p = dynamic_cast<PieChartGraph*>(w)) {
            // Фон под виджетом — очень светлый серый
            p->setAutoFillBackground(true);
            QPalette pal = p->palette();
            pal.setColor(QPalette::Window, QColor(250,250,250));
            p->setPalette(pal);
            // Отступ, чтобы круг не прилипал к краям
            p->setContentsMargins(15,15,15,15);
        }
    }
};
