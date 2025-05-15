#pragma once

#include <QPainter>
#include <QPaintEvent>
#include <QMap>
#include <QString>
#include <src/Chart/IGraphWidget.h>
#include <src/Chart/IGraphFactory.h>
#include <src/IOC_Container.h>
#include <src/Chart/GraphFactoryRegistry.h>

class PieChartGraph : public IGraphWidget {
    Q_OBJECT
        public:
                 explicit PieChartGraph(QWidget *parent = nullptr)
        : IGraphWidget(parent)
    {}

    void setData(const QList<QStringList> &data) override {
        pieData.clear();
        for (const auto &row : data) {
            if (row.size() < 2)
                continue;
            const QString &key = row.value(0);
            bool ok = false;
            double v = row.value(1).toDouble(&ok);
            if (ok) pieData[key] += v;
        }
        update();
    }

  protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

               // сначала настроим общий painter
        if (style_) style_->apply(painter, this);

               // рисуем секторы
        QRectF rect = this->rect().adjusted(10,10,-10,-10);
        double total = 0; for (double v : pieData.values()) total += v;
        if (total <= 0) {
            QWidget::paintEvent(event);
            return;
        }

        double angle = 0;
        int i = 0;
        for (auto it = pieData.constBegin(); it != pieData.constEnd(); ++it, ++i) {
            double span = 360.0 * it.value() / total;
            // берём цвет из стиля
            QColor c = style_
                           ? style_->sliceColor(i, it.key(), it.value())
                           : QColor::fromHsv((i*45)%360,255,200);
            painter.setBrush(c);
            painter.drawPie(rect, int(angle*16), int(span*16));
            angle += span;
        }

        QWidget::paintEvent(event);
    }

  private:
    QMap<QString,double> pieData;
};

class PieChartGraphFactory : public IGraphFactory {
  public:
    std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) override {
        return std::make_unique<PieChartGraph>(parent);
    }
};
