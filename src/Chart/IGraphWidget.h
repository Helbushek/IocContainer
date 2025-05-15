#pragma once

#include "includes/qcustomplot/qcustomplot.h"
#include "src/Chart/Style/IChartStyle.h"
#include <QWidget>
#include <QDateTime>

class IGraphWidget : public QWidget
{
    Q_OBJECT
  public:
    using QWidget::QWidget; // наследуем конструктор

    virtual void setData(const QList<QStringList> &data) = 0;

    virtual ~IGraphWidget();

    virtual QDateTime parseDate(const QString &str);

    virtual void enableInteractions(QCustomPlot *plot);

    virtual void setTitle(const QString &title);

    virtual void setAxisLabels(const QString &x, const QString &y);

    virtual void setLegendVisible(bool visible);

    virtual void setStyle(IChartStyle *st);

    virtual void applyChartStyle(IChartStyle *style)
    {
    }

    virtual QCustomPlot *plot() const;

    virtual void setAxelsSettings();
  protected:
    QCustomPlot *plot_;
    QCPTextElement   *titleElement_ = nullptr;
    IChartStyle* style_ = nullptr;
};
