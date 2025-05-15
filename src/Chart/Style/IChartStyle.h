#pragma once

#include <QPainter>
class IGraphWidget;

class IChartStyle {
  public:
    virtual ~IChartStyle() = default;

           // вызывaется из paintEvent перед рисованием
    virtual void apply(QPainter &p, IGraphWidget *widget) const = 0;

           // вызывается при создании/смене графика,
           // здесь можно перенастроить сам виджет (цвет фона, параметры линий)
    virtual void configure(IGraphWidget *widget) const = 0;

    virtual QColor sliceColor(int index, const QString &key, double value) const {
        // по-умолчанию — разноцветие, как было у вас:
        return QColor::fromHsv((index*45) % 360, 255, 200);
    }
};
