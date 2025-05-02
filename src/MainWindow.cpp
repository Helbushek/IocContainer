#include <QSplitter>

#include "MainWindow.h"
#include <src/DataReaders/SqliteReader.h>
#include <iostream>
#include <QtCharts>
#include <includes/qcustomplot/qcustomplot.h>
#include <QLayout>
#include <src/Chart/CustomGraphWidget.h>
#include <algorithm>
#include <string>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->showMaximized();
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle(QString("Graph viewer"));
    QSplitter *splitter = new QSplitter(centralWidget);

    QToolBar *toolbar = addToolBar("Тулбар");
    toolbar->setContextMenuPolicy(Qt::NoContextMenu);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    QAction *openAction = toolbar->addAction(QIcon::fromTheme("go-up"), " ");

    QAction *refreshAction = toolbar->addAction(QIcon(":/icons/refresh.png"), "Обновить");

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    customPlot = new GraphDisplayArea(centralWidget);
    CustomPlotGraph *plot = new CustomPlotGraph(centralWidget);
    customPlot->setGraph(plot);

    layout->addWidget(splitter);

    customPlot->setMinimumSize(800, 600);

    QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(QDir::homePath());

    QTreeView *tree = new QTreeView;
    tree->setModel(model);
    tree->setRootIndex(model->index(QDir::homePath()));
    tree->setColumnWidth(0, 250);

    QLineEdit *pathEdit = new QLineEdit(QDir::homePath());
    QPushButton *backPath = new QPushButton(centralWidget);
    backPath->setIcon(QIcon("includes/icons/revert.png"));
    backPath->setIconSize(QSize(20, 20));
    backPath->update();

    QObject::connect(pathEdit, &QLineEdit::returnPressed, [=]() {
        QString newPath = pathEdit->text();
        QModelIndex index = model->index(newPath);
        if (index.isValid())
        {
            tree->setRootIndex(index);
        }
    });

    QObject::connect(backPath, &QPushButton::pressed, [=]() {
        try
        {
            std::string path = pathEdit->text().toStdString();
            std::reverse(path.begin(), path.end());
            if (path[0] == '/')
                path = path.substr(1);
            path = path.substr(path.find('/'), path.length());
            std::reverse(path.begin(), path.end());
            QString newPath = QString::fromStdString(path);
            pathEdit->setText(newPath);
            QModelIndex index = model->index(newPath);
            tree->setRootIndex(index);
        }
        catch (...)
        {
            return;
        }
    });

    QObject::connect(tree, &QTreeView::clicked, [=](const QModelIndex &index) {
        QString path = model->filePath(index);
        if (QFileInfo(path).isDir())
        {
            tree->setRootIndex(index);
            pathEdit->setText(path);
        }
    });

    QWidget *fileBrowserWidget = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout(fileBrowserWidget);
    QHBoxLayout *layout2 = new QHBoxLayout(fileBrowserWidget);
    layout2->addWidget(backPath);
    layout2->addWidget(pathEdit);
    layout1->addLayout(layout2);
    layout1->addWidget(tree);
    fileBrowserWidget->setLayout(layout1);

    splitter->addWidget(fileBrowserWidget);
    splitter->addWidget(customPlot);

    connect(tree, &QTreeView::doubleClicked, this, [=](const QModelIndex &index) {
        QString path = model->filePath(index);
        if (QFileInfo(path).isFile() && path.endsWith(".sqlite"))
        {
            qDebug() << "Открываем SQLite файл:" << path;
            QProgressDialog *dialog = new QProgressDialog("Обновление графика...", "", 0, 0, this);
            dialog->setCancelButton(nullptr);
            dialog->setWindowModality(Qt::ApplicationModal);
            dialog->show();

            QApplication::processEvents(); // показать окно
            // Загрузка данных и построение графика
            QList<QStringList *> * res = SqliteReader::read(path);
            customPlot->setData(*res); // твоя функция перерисовки графика

            dialog->close();
        }
    });

    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
}

MainWindow::~MainWindow()
{
}
