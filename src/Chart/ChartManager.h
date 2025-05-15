#pragma once

#include <QString>
#include <memory>
#include "src/DataReaders/DataReader.h"
#include "src/Chart/IGraphWidget.h"
#include <QWidget>

class ChartManager {
  public:
    // Возвращает DataReader по расширению файла (.sqlite или .json)
    static std::shared_ptr<DataReader> createReader(const QString &path);

           // Возвращает виджет графика по расширению файла (линейный или круговой)
    static std::unique_ptr<IGraphWidget> createGraphWidget(const QString &path,
                                                           QWidget *parent = nullptr);
};
