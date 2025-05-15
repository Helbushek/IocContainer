#include <QSplitter>

#include "MainWindow.h"
#include "src/Chart/Style/StyleManager.h"
#include <iostream>
#include <QtCharts>
#include <includes/qcustomplot/qcustomplot.h>
#include <QLayout>
#include <src/IOC_Container.h>
#include <algorithm>
#include <string>
#include <src/DataReaders/DataReader.h>
#include <src/Chart/IGraphFactory.h>
#include <src/Chart/ChartManager.h>
#include <src/Chart/Style/IChartStyle.h>
#include <src/PrintManager.h>
#include <src/ExportManager.h>
#include <QtConcurrent/QtConcurrent>
#include <src/Chart/GraphFactoryRegistry.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->showMaximized();
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setWindowTitle(QString("Graph viewer"));
    QSplitter *splitter = new QSplitter(centralWidget);

    QToolBar *toolbar = addToolBar("Тулбар");
    toolbar->setContextMenuPolicy(Qt::NoContextMenu);
    QAction *printAction = toolbar->addAction(QIcon(":/icons/print.png"), "Печать");
    connect(printAction, &QAction::triggered, this, &MainWindow::onPrint);

    QAction *pngAction = toolbar->addAction(QIcon(":/icons/image.png"), "");
    pngAction->setToolTip("Сохранить график как PNG");
    connect(pngAction, &QAction::triggered, this, &MainWindow::onSavePng);

    QAction *jpgAction = toolbar->addAction(QIcon(":/icons/image-jpg.png"), "");
    jpgAction->setToolTip("Сохранить график как JPEG");
    connect(jpgAction, &QAction::triggered, this, &MainWindow::onSaveJpg);

    QAction *props = toolbar->addAction(QIcon(":/icons/settings.png"), "");
    props->setToolTip("Настройки заголовка и осей");
    connect(props, &QAction::triggered, this, &MainWindow::onEditGraphProperties);

    QAction *batchAction = toolbar->addAction(QIcon(":/icons/batch.png"), "");
    batchAction->setToolTip("Пакетный экспорт");
    connect(batchAction, &QAction::triggered,
            this, &MainWindow::onBatchExport);

    QAction *actColor = toolbar->addAction("Цветной");
    QAction *actGray = toolbar->addAction("Ч/Б");

    actColor->setEnabled(false);
    actGray->setEnabled(false);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(splitter);

    customPlot = new GraphDisplayArea(centralWidget);
    new StyleManager(std::move(actColor), std::move(actGray), std::move(customPlot), this);
    typeManager_ = std::make_unique<GraphTypeManager>(toolbar, customPlot, this);
    typeManager_->renderCurrentType(customPlot, nullptr);

    customPlot->setMinimumSize(800, 600);

    model_ = new QFileSystemModel;
    model_->setRootPath(QDir::homePath());

    QTreeView *tree = new QTreeView;
    tree->setModel(model_);
    tree->setRootIndex(model_->index(QDir::homePath()));
    tree->setColumnWidth(0, 250);

    QLineEdit *pathEdit = new QLineEdit(QDir::homePath());
    QPushButton *backPath = new QPushButton(centralWidget);
    backPath->setIcon(QIcon("includes/icons/revert.png"));
    backPath->setIconSize(QSize(20, 20));
    backPath->update();

    QObject::connect(pathEdit, &QLineEdit::returnPressed,
                     [tree, pathEdit, this](){
                         QString newPath = pathEdit->text();
                         QModelIndex idx = model_->index(newPath);
                         if (idx.isValid()) tree->setRootIndex(idx);
                     });

    QObject::connect(backPath, &QPushButton::pressed,
                     [tree, pathEdit, this](){
                         QDir dir(pathEdit->text());
                         if (dir.cdUp()) {
                             QString np = dir.absolutePath();
                             pathEdit->setText(np);
                             QModelIndex idx = model_->index(np);
                             if (idx.isValid()) tree->setRootIndex(idx);
                         }
                     });

    QObject::connect(tree, &QTreeView::clicked,
                     [tree, pathEdit, this](const QModelIndex &idx){
                         QString path = model_->filePath(idx);
                         if (QFileInfo(path).isDir()) {
                             tree->setRootIndex(idx);
                             pathEdit->setText(path);
                         }
                     });

    QWidget *fileBrowserWidget = new QWidget;
    QVBoxLayout *layout1 = new QVBoxLayout();
    QHBoxLayout *layout2 = new QHBoxLayout();
    layout2->addWidget(backPath);
    layout2->addWidget(pathEdit);
    layout1->addLayout(layout2);
    layout1->addWidget(tree);
    fileBrowserWidget->setLayout(layout1);

    splitter->addWidget(fileBrowserWidget);
    splitter->addWidget(customPlot);

    connect(tree, &QTreeView::doubleClicked, this, [this, actColor, actGray](const QModelIndex &idx){
        QString path = model_->filePath(idx);
        if (!QFileInfo(path).isFile()) return;

        auto reader = ChartManager::createReader(path);
        if (!reader) {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось подобрать DataReader для\n" + path);
            return;
        }

        // Создаём прогресс-диалог на куче
        auto *dialog = new QProgressDialog("Загрузка данных...", QString(), 0, 0, this);
        dialog->setWindowModality(Qt::WindowModal);
        dialog->setCancelButton(nullptr);
        dialog->show();
        QApplication::processEvents();

        // Запускаем чтение асинхронно
        QFuture<QList<QStringList>*> future =
            QtConcurrent::run([reader, path]() {
                return reader->read(path);
            });

        // Наблюдатель
        auto *watcher = new QFutureWatcher<QList<QStringList>*>(this);
        connect(watcher, &QFutureWatcher<QList<QStringList>*>::finished, this,
                [this, dialog, watcher]() {
            // Закрываем диалог
            dialog->close();
            dialog->deleteLater();

            // Достаём результат
            QList<QStringList>* rows = watcher->result();
            watcher->deleteLater();

            if (!rows) {
                QMessageBox::critical(this, "Ошибка", "Не удалось прочитать данные");
                return;
            }

            // Сохраняем и рендерим
            currentRows_.reset(rows);
            typeManager_->renderCurrentType(customPlot, rows);
        });
        watcher->setFuture(future);

        actColor->setEnabled(true);
        actGray->setEnabled(true);
    });

    splitter->setCollapsible(0, false);
    splitter->setCollapsible(1, false);
}


void MainWindow::onPrint()
{
    QWidget *gw = customPlot->graph();
    if (!gw) {
        QMessageBox::warning(this, "Печать", "Нет графика для печати");
        return;
    }

    // 1) Открываем диалог «Сохранить как…»
    QString filter = "PDF файлы (*.pdf)";
    QString defaultPath = QDir::homePath() + "/chart.pdf";
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить график в PDF",
        defaultPath,
        filter
    );

    // Если пользователь нажал «Отмена» — выходим
    if (fileName.isEmpty())
        return;

    // Подстраховка: добавим расширение, если забыли
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    // 2) Опционально спросим, печатать ли в ч/б
    bool grayScale = (QMessageBox::question(
        this, "Режим печати",
        "Печатать в чёрно-белом режиме?",
        QMessageBox::Yes|QMessageBox::No,
        QMessageBox::No
    ) == QMessageBox::Yes);

    // 3) Вызываем ваш менеджер печати
    if (!PrintManager::exportToPdf(gw, fileName, grayScale)) {
        QMessageBox::critical(this, "Ошибка",
            "Не удалось сохранить PDF:\n" + fileName);
    } else {
        statusBar()->showMessage("Сохранено: " + fileName, 3000);
    }
}

void MainWindow::onSavePng()
{
    if (!customPlot->graph()) {
        QMessageBox::warning(this, "Экспорт PNG", "Нет графика для сохранения");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить как PNG",
        QDir::homePath() + "/chart.png",
        "PNG файл (*.png)"
    );
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".png", Qt::CaseInsensitive))
        fileName += ".png";

    bool ok = ExportManager::exportToImage(
        customPlot->graph(), fileName, "PNG"
    );
    if (!ok) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить PNG:\n" + fileName);
    } else {
        statusBar()->showMessage("PNG сохранён в " + fileName, 3000);
    }
}

void MainWindow::onSaveJpg()
{
    if (!customPlot->graph()) {
        QMessageBox::warning(this, "Экспорт JPEG", "Нет графика для сохранения");
        return;
    }
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить как JPEG",
        QDir::homePath() + "/chart.jpg",
        "JPEG файл (*.jpg *.jpeg)"
    );
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".jpg", Qt::CaseInsensitive) &&
        !fileName.endsWith(".jpeg", Qt::CaseInsensitive))
    {
        fileName += ".jpg";
    }

    bool ok = ExportManager::exportToImage(
        customPlot->graph(), fileName, "JPEG"
    );
    if (!ok) {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить JPEG:\n" + fileName);
    } else {
        statusBar()->showMessage("JPEG сохранён в " + fileName, 3000);
    }
}

void MainWindow::onEditGraphProperties() {
    auto *g = customPlot->graph();
    if (!g) return;

    bool ok;
    QString title = QInputDialog::getText(
        this, "Заголовок графика",
        "Введите заголовок:", QLineEdit::Normal,
        /*current=*/"", &ok);
    if (ok) g->setTitle(title);

    QString xlabel = QInputDialog::getText(this, "Ось X",
        "Подпись для оси X:", QLineEdit::Normal, "", &ok);
    QString ylabel = QInputDialog::getText(this, "Ось Y",
        "Подпись для оси Y:", QLineEdit::Normal, "", &ok);
    if (ok) g->setAxisLabels(xlabel, ylabel);

    // Легенда
    auto legendBtn = QMessageBox::question(
        this, "Легенда", "Показать легенду?",
        QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    g->setLegendVisible(legendBtn == QMessageBox::Yes);

    // Перерисуем область
    customPlot->update();
}

void MainWindow::onBatchExport()
{
    // 1) Выбор входной папки
    QString inDir = QFileDialog::getExistingDirectory(
        this, "Выберите папку с данными", QDir::homePath());
    if (inDir.isEmpty()) return;

    // 2) Выбор выходной папки
    QString outDir = QFileDialog::getExistingDirectory(
        this, "Папка для сохранения результатов", QDir::homePath());
    if (outDir.isEmpty()) return;

    // 3) Формат
    QStringList formats = {"PDF", "PNG", "JPEG"};
    bool ok;
    QString fmt = QInputDialog::getItem(
        this, "Формат экспорта",
        "Выберите формат файлов:", formats, 0, false, &ok);
    if (!ok) return;

    // 3.1) Тип графика
    auto gtypes = GraphFactoryRegistry::instance().availableTypes();
    gtypes.prepend("Auto");
    QString gtype = QInputDialog::getItem(
        this, "Тип графика",
        "Принудительно выбрать тип графика (Auto = по данным):",
        gtypes, 0, false, &ok);
    if (!ok) return;

    // 3.2) Ч/Б режим
    bool grayScale = (QMessageBox::question(
        this, "Режим экспорта",
        "Экспортировать в чёрно-белом режиме?",
        QMessageBox::Yes|QMessageBox::No,
        QMessageBox::No
    ) == QMessageBox::Yes);

    if (gtype.compare("Auto", Qt::CaseInsensitive) != 0) {
        typeManager_->setType(gtype);
    }

    // 4) Запускаем пакетное сохранение с новыми параметрами
    batchExport(inDir, outDir, fmt, grayScale);
}

void MainWindow::batchExport(const QString &inDir,
                             const QString &outDir,
                             const QString &format,
                             bool grayScale)
{
    // 1) Собираем список всех файлов
    QStringList files;
    QDirIterator it(inDir, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) files << it.next();
    if (files.isEmpty()) {
        QMessageBox::information(this, "Batch Export", "Файлы не найдены.");
        return;
    }

    // 2) Прогресс-диалог в GUI
    QProgressDialog dlg("Экспорт файлов...", "Отмена", 0, files.size(), this);
    dlg.setWindowModality(Qt::WindowModal);
    dlg.show();

    // 3) Размер канвы (берём из основного графика)
    QSize exportSize = customPlot->size();

    // 4) Цикл по всем файлам — **в GUI-потоке**
    for (int i = 0; i < files.size(); ++i) {
        if (dlg.wasCanceled()) break;

        const QString &filePath = files.at(i);

        // a) Читаем данные
        auto reader = ChartManager::createReader(filePath);
        if (!reader) {
            dlg.setValue(i+1);
            QApplication::processEvents();
            continue;
        }
        auto rows = reader->read(filePath);
        if (!rows) {
            delete rows;
            dlg.setValue(i+1);
            QApplication::processEvents();
            continue;
        }

        // b) Строим временный график в оперативной области
        GraphDisplayArea tmpArea;
        tmpArea.resize(exportSize);
        // перепользуем GraphTypeManager, который уже настроен на нужный тип
        typeManager_->renderCurrentType(&tmpArea, rows);

        // c) Экспорт
        QString base    = QFileInfo(filePath).completeBaseName();
        QString outFile = outDir + "/" + base + "." + format.toLower();
        if (format == "PDF") {
            PrintManager::exportToPdf(&tmpArea, outFile, grayScale);
        } else {
            ExportManager::exportToImage(&tmpArea,
                                        outFile.toUtf8(),
                                        format.toUtf8());
        }

        delete rows;

        // d) Обновляем прогресс и даём управление UI
        dlg.setValue(i+1);
        QApplication::processEvents();
    }

    dlg.close();
    QMessageBox::information(this, "Batch Export", "Экспорт завершён.");
}

MainWindow::~MainWindow()
{
}
