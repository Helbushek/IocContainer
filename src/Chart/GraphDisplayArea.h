#pragma once

#include <QWidget>
#include <QVBoxLayout>

#include <src/Chart/IGraphWidget.h>

class GraphDisplayArea : public QWidget {
    Q_OBJECT
        public:
    explicit GraphDisplayArea(QWidget *parent = nullptr)
        : QWidget(parent), layout_(new QVBoxLayout(this)) {}

    void setGraph(std::unique_ptr<IGraphWidget> graph) {
        if (graph_) {
            layout_->removeWidget(graph_.get());
            graph_.reset();
        }
        graph_       = std::move(graph);
        layout_->addWidget(graph_.get());
    }

    void setData(const QList<QStringList> &data) {
        if (graph_) graph_->setData(data);
    }

    IGraphWidget *graph()
    {
        return graph_.get();
    }

  private:
    QVBoxLayout                     *layout_;
    std::unique_ptr<IGraphWidget>   graph_;
};
