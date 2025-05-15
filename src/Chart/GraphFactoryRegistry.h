#pragma once

#include <QString>
#include <vector>
#include <functional>
#include <memory>
#include "IGraphWidget.h"
#include <QWidget>

class GraphFactoryRegistry {
  public:
    using FactoryFn = std::function<std::unique_ptr<IGraphWidget>(QWidget*)>;

    struct Entry {
        QString    name;
        FactoryFn  fn;
    };

    static GraphFactoryRegistry& instance() {
        static GraphFactoryRegistry inst;
        return inst;
    }

    void registerFactory(const QString& name, FactoryFn fn) {
        entries_.push_back({name, std::move(fn)});
    }

    QStringList availableTypes() const {
        QStringList list;
        list.reserve(entries_.size());
        for (const auto &e : entries_) {
            list << e.name;
        }
        std::sort(list.begin(), list.end(), [](const QString &a, const QString &b){
            return a < b;
        });
        return list;
    }

    std::unique_ptr<IGraphWidget> create(const QString &name,
                                         QWidget *parent = nullptr) const
    {
        QString key = name.toLower();
        for (const auto &e : entries_) {
            if (e.name == key) {
                return e.fn(parent);
            }
        }
        return nullptr;
    }

    const std::vector<Entry>& entries() const { return entries_; }

  private:
    std::vector<Entry> entries_;
};


#define REGISTER_GRAPH_FACTORY(NAME, FACTORY_CLASS)                     \
namespace {                                                             \
    const bool _reg_##FACTORY_CLASS = [](){                             \
            GraphFactoryRegistry::instance().registerFactory(           \
            NAME,                                                       \
            [](QWidget* parent){                                        \
                    return std::make_unique<FACTORY_CLASS>(parent);     \
            });                                                         \
            return true;                                                \
    }();                                                                \
}
