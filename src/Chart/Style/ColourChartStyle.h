#pragma once

#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QPalette>

// Наши общие стили
#include "src/Chart/Style/IChartStyle.h"

// Подключаем все виды графиков, чтобы dynamic_cast видел типы
class CustomPlotGraph;
class ScatterGraph;
class BarChartGraph;
class PieChartGraph;

// QCustomPlot
#include <includes/qcustomplot/qcustomplot.h>

class ColourChartStyle : public IChartStyle {
  public:
    // цвета для круговой диаграммы
    QColor sliceColor(int index, const QString &, double) const override;

    // обводка/общий пен
    void apply(QPainter &p, IGraphWidget *) const override;

    // настройка виджета сразу после создания/смены
    void configure(IGraphWidget *w) const override;
};
