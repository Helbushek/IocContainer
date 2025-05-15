// src/Chart/GraphTypeManager.cpp
#include "src/Chart/GraphTypeManager.h"

#include <QComboBox>
#include <QToolBar>
#include <QDebug>

#include "src/Chart/GraphFactoryRegistry.h"
#include "src/Chart/GraphDisplayArea.h"
#include "src/MainWindow.h"

GraphTypeManager::GraphTypeManager(QToolBar *toolbar,
                                   GraphDisplayArea *display,
                                   MainWindow *mainWindow,
                                   QObject *parent)
    : QObject(parent)
      , display_(display)
      , mainWindow_(mainWindow)
{
    combo_ = new QComboBox;
    toolbar->addWidget(combo_);

           // Заполняем выпадашку из реестра фабрик
    const auto &entries = GraphFactoryRegistry::instance().entries();
    for (const auto &e : entries) {
        combo_->addItem(e.name);
        factories_.push_back(e.fn);
    }

    connect(combo_, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &GraphTypeManager::onTypeChanged);
}

void GraphTypeManager::setType(const QString &name)
{
    int idx = combo_->findText(name, Qt::MatchFixedString);
    if (idx >= 0) {
        combo_->blockSignals(true);
        combo_->setCurrentIndex(idx);
        combo_->blockSignals(false);
        renderCurrentType(display_, mainWindow_->currentRows());
    }
}

void GraphTypeManager::onTypeChanged(int /*index*/)
{
    renderCurrentType(display_, mainWindow_->currentRows());
}

void GraphTypeManager::renderCurrentType(GraphDisplayArea *display,
                                         const QList<QStringList> *rows)
{
    // 1) Создаём новый график
    auto widget = factories_[ combo_->currentIndex() ](display);
    if (!widget) {
        qWarning() << "[GraphTypeManager] Нет фабрики для индекса"
                   << combo_->currentIndex();
        return;
    }

    // 2) Если есть данные — передаём их
    if (rows && !rows->isEmpty()) {
        widget->setData(*rows);
    }

    // 3) Меняем график в области
    display->setGraph(std::move(widget));
}
