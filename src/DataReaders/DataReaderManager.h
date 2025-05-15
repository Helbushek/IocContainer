    // src/DataReaders/DataReaderRegistry.h
    #pragma once

    #include <QString>
    #include <vector>
    #include <functional>
    #include <memory>
    #include "DataReader.h"
    #include <src/DataReaders/JsonReader.h>
    #include <src/DataReaders/SqliteReader.h>

    class DataReaderRegistry {
      public:
        using FactoryFn = std::function<std::shared_ptr<DataReader>()>;

        struct Entry {
            QString        ext;     // без точки, lower-case, например "sqlite", "json"
            FactoryFn      fn;
        };

        static DataReaderRegistry& instance() {
            static DataReaderRegistry inst;
            return inst;
        }

        void registerReader(const QString& extension, FactoryFn fn) {
            entries_.push_back({ extension.toLower(), std::move(fn) });
        }

        const std::vector<Entry>& entries() const { return entries_; }

      private:
        std::vector<Entry> entries_;
    };

    #define REGISTER_DATA_READER(EXT,       READER_CLASS)                    \
    namespace {                                                              \
        const bool _reg_##READER_CLASS = [](){                               \
                DataReaderRegistry::instance()                               \
                    .registerReader(EXT,                                     \
                        [](){ return std::make_shared<READER_CLASS>(); }     \
                        );                                                   \
                return true;                                                 \
        }();                                                                 \
    }
