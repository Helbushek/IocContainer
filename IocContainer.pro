QT = core gui sql charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0   # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/MainWindow.cpp \
        src/main.cpp \
        includes/qcustomplot/qcustomplot.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/Chart/CustomGraphWidget.h \
    src/Chart/GraphDisplayWidget.h \
    src/Chart/IChartWidget.h \
    src/Chart/PieChartGraph.h \
    src/DataReaders/DataReader.h \
    src/IOC_Contaner.h \
    src/JsonReader.h \
    src/MainWindow.h \
    src/SqliteReader.h \
    includes/qcustomplot/qcustomplot.h
