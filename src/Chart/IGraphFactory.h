#pragma once
#include <src/Chart/IGraphWidget.h>
#include <QWidget>

class IGraphFactory
{
  public:
    virtual std::unique_ptr<IGraphWidget> create(QWidget *parent = nullptr) = 0;
    virtual ~IGraphFactory() = default;
};
