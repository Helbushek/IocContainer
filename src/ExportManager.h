// ExportManager.h
#pragma once

#include <QWidget>
#include <QPixmap>

class ExportManager {
  public:
    static bool exportToImage(QWidget *graphWidget,
                              const QString &filePath,
                              const char *format = "PNG")
    {
        if (!graphWidget) return false;

               // Рисуем виджет в pixmap нужного размера
        QPixmap pix(graphWidget->size());
        graphWidget->render(&pix);

               // Сохраняем
        return pix.save(filePath, format);
    }
};
