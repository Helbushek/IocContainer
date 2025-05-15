#pragma once

#include "PrintManager.h"
#include "qpdfwriter.h"
#include "qwidget.h"
#include <QPrinter>
#include <QPainter>
#include <QPageLayout>
#include <QPageSize>
#include <QFileInfo>
#include <QDir>

class PrintManager {
  public:
    static bool exportToPdf(QWidget *widget,
                            const QString &filePath,
                            bool grayScale = false,
                            int dpi       = 300)
    {
        if (!widget || filePath.isEmpty())
            return false;

               // 1) Убедимся, что папка для файла существует
        QFileInfo fi(filePath);
        QDir().mkpath(fi.path());

               // 2) Настроим QPdfWriter на A4 и нужное разрешение
        QPdfWriter writer(filePath);
        writer.setResolution(dpi);
        writer.setPageSize(QPagedPaintDevice::A4);

               // 3) Поменяем размер страницы на альбомный, поменяв местами мм
        {
            QSizeF pageMM = writer.pageSizeMM();                // e.g. 210×297
            writer.setPageSizeMM(QSizeF(pageMM.height(), pageMM.width()));
        }

               // 4) Узнаём фактические размеры «бумаги» в пикселях при этом dpi
        int pageW = writer.width();
        int pageH = writer.height();
        if (pageW <= 0 || pageH <= 0)
            return false;

               // 5) Подготовим рисовальщик
        QPainter painter(&writer);

               // 6) Вычислим масштаб, чтобы виджет уместился целиком
        QSize wsize = widget->size();
        if (wsize.isEmpty()) {
            painter.end();
            return false;
        }
        qreal xScale = qreal(pageW)  / wsize.width();
        qreal yScale = qreal(pageH) / wsize.height();
        qreal scale  = qMin(xScale, yScale);

               // 7) Центрируем результат
        qreal dx = (pageW - wsize.width()  * scale) / 2.0;
        qreal dy = (pageH - wsize.height() * scale) / 2.0;
        painter.translate(dx, dy);
        painter.scale(scale, scale);

               // 8) Чёрно-белый режим — опционально
        if (grayScale) {
            painter.setPen(Qt::black);
            painter.setBrush(Qt::NoBrush);
        }

               // 9) Рендерим сам виджет в PDF
        widget->render(&painter);
        painter.end();

               // 10) Проверяем, что файл создался и не пуст
        QFileInfo out(filePath);
        return out.exists() && out.size() > 0;
    }
};
