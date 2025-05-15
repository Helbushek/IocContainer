#pragma once

#include "src/Chart/GraphTypeManager.h"
#include <QMainWindow>
#include <QTextCodec>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <src/Chart/GraphDisplayArea.h>
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
    const QList<QStringList>* currentRows() const { return currentRows_.get(); }

   private slots:
    void onPrint();
    void onSavePng();
    void onSaveJpg();
    void onEditGraphProperties();
    void onBatchExport();

  private:
    void batchExport(const QString &inDir,
                     const QString &outDir,
                     const QString &format,
                     bool grayScale);

    DataReader *dataReader;
    GraphDisplayArea *customPlot;
    QFileSystemModel      *model_;
    std::unique_ptr<QList<QStringList>> currentRows_;
    std::unique_ptr<GraphTypeManager> typeManager_;
};
