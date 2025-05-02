//#pragma once

//#include <QPainter>

//#include <src/Chart/IChartWidget.h>

//class PieChartGraph : public IGraphWidget
//{
//    Q_OBJECT
//  public:
//    PieChartGraph(QWidget *parent = nullptr) : IGraphWidget(parent)
//    {
//    }

//    void setData(const QList<QStringList *> &data) override
//    {
//        pieData.clear();
//        for (auto list : data)
//        {
//            pieData[list->at(0)] = list->at(1).toDouble();
//        }
//    }

//  protected:
//    void paintEvent(QPaintEvent *) override {
//        QPainter painter(this);
//        QRectF rect = this->rect().adjusted(10, 10, -10, -10);
//        double total = 0;
//        for (auto val : pieData.values()) total += val;

//        double angle = 0;
//        int colorIndex = 0;
//        for (auto it = pieData.begin(); it != pieData.end(); ++it) {
//            double span = 360.0 * it.value() / total;
//            painter.setBrush(QColor::fromHsv((colorIndex++ * 45) % 360, 255, 200));
//            painter.drawPie(rect, int(angle * 16), int(span * 16));
//            angle += span;
//        }
//    }

//  private:
//    QMap<QString, double> pieData;
//};
