// GraphTypeManager.h
#pragma once

#include <QObject>
#include <vector>
#include <functional>
#include <memory>

class QToolBar;
class QComboBox;
class GraphDisplayArea;
class IGraphWidget;
class QStringList;
class MainWindow;

class GraphTypeManager : public QObject {
    Q_OBJECT
        public:
                 // mainWindow нужен, чтобы получить .currentRows()
                 GraphTypeManager(QToolBar *toolbar,
                                  GraphDisplayArea *display,
                                  MainWindow *mainWindow,
                                  QObject *parent = nullptr);

           // при смене данных или без — рендерит в display через текущую фабрику
    void renderCurrentType(GraphDisplayArea *display,
                           const QList<QStringList> *rows);

    void setType(const QString &name);

  private slots:
    void onTypeChanged(int index);

  private:
    GraphDisplayArea             *display_   = nullptr;
    MainWindow                   *mainWindow_= nullptr;
    QComboBox                    *combo_     = nullptr;

           // каждая фабрика даёт новый IGraphWidget
    std::vector<std::function<std::unique_ptr<IGraphWidget>(QWidget*)>> factories_;
};
