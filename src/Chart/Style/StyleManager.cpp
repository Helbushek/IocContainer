// src/Chart/Style/StyleManager.cpp
#include "StyleManager.h"

#include <QToolBar>
#include <QAction>
#include <QDebug>

#include "src/Chart/GraphDisplayArea.h"
#include "ColourChartStyle.h"
#include "GrayScaleChartStyle.h"

StyleManager::StyleManager(QAction *actColor, QAction *actGray,
                           GraphDisplayArea *display,
                           QObject *parent)
  : QObject(parent)
{

    connect(actColor, &QAction::triggered, this, [display]() {
        auto widget = display->graph();
        if (!widget) {
            qDebug() << "[StyleManager] Нет графика для стилизации";
            return;
        }
        // Берём нужный стиль
        auto style = new ColourChartStyle;
        if (!style)
            return;
        // Единственный вызов:
        widget->applyChartStyle(style);
    });

    connect(actGray, &QAction::triggered, this, [display]() {
        auto widget = display->graph();
        if (!widget) {
            qDebug() << "[StyleManager] Нет графика для стилизации";
            return;
        }
        auto style = new GrayScaleChartStyle;
        if (!style)
            return;
        widget->applyChartStyle(style);
    });
}
