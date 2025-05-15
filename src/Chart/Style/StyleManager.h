#pragma once

#include <QObject>
#include <QToolBar>
#include <QAction>
#include "src/Chart/GraphDisplayArea.h"

class StyleManager : public QObject {
    Q_OBJECT
        public:
                 StyleManager(QAction *actColor, QAction *actGray,
                                       GraphDisplayArea *display,
                                       QObject *parent = nullptr);
};
