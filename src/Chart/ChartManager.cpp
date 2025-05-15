#include "ChartManager.h"
#include <QFileInfo>
#include <src/IOC_Container.h>
#include <src/DataReaders/SqliteReader.h>
#include <src/DataReaders/JsonReader.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/GraphFactoryRegistry.h>

#ifdef GetObject
#  undef GetObject
#endif

std::shared_ptr<DataReader> ChartManager::createReader(const QString &path) {
    QString ext = QFileInfo(path).suffix().toLower();
    for (auto &e : DataReaderRegistry::instance().entries()) {
        if (e.ext == ext) {
            return e.fn();
        }
    }
    return nullptr;  // не найден ридер для данного расширения
}

std::unique_ptr<IGraphWidget>
ChartManager::createGraphWidget(const QString &path, QWidget *parent) {
    QString ext = QFileInfo(path).suffix().toLower();
    // графики мы по-прежнему берём из GraphFactoryRegistry
    for (auto &e : GraphFactoryRegistry::instance().entries()) {
        // предположим, что имя фабрики совпадает с расширением
        if (e.name.toLower() == ext) {
            return e.fn(parent);
        }
    }
    // либо возвращаем default graph
    auto def = GraphFactoryRegistry::instance().entries().front().fn(parent);
    return def;
}
