#include "MainWindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    qDebug() << " main start";
    QApplication a(argc, argv);
    qDebug() << "   after QApplication ctor";
    MainWindow w;
    qDebug() << "   after MainWindow ctor";
    w.show();
    qDebug() << "   after w.show()";
    return a.exec();
}
