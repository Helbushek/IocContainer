#pragma once

#include <QMainWindow>
#include <QTextCodec>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <src/Chart/GraphDisplayWidget.h>
#include <src/DataReaders/DataReader.h>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT
        protected:
    QWidget *centralWidget;

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void UpdatePlot(QList<QStringList *> *data);

    QDateTime parseDate(const QString &str);

  private:
    DataReader *dataReader;
    Ui::MainWindow *ui;
    GraphDisplayArea *customPlot;
};
