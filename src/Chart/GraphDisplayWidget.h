#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include <src/Chart/IChartWidget.h>

class GraphDisplayArea : public QWidget {
    Q_OBJECT
        public:
                 GraphDisplayArea(QWidget *parent = nullptr) : QWidget(parent) {
        layout = new QVBoxLayout(this);
    }

    void setGraph(IGraphWidget *graph) {
        if (currentGraph) {
            layout->removeWidget(currentGraph);
            currentGraph->deleteLater();
        }
        currentGraph = graph;
        layout->addWidget(currentGraph);
    }

    void setData(const QList<QStringList*> &data) {
        if (currentGraph)
            currentGraph->setData(data);
    }

  private:
    QVBoxLayout *layout;
    IGraphWidget *currentGraph = nullptr;
};
