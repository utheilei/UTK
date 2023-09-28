#ifndef UWIDGETUTILS_H
#define UWIDGETUTILS_H

#include "utkwidget_export.h"

#include <QImage>
#include <QIcon>
#include <QTextOption>

class UTKWIDGET_EXPORT UWidgetUtils
{
public:
    UWidgetUtils();
    ~UWidgetUtils() = default;

    static QIcon getNumCircleIcon(int num, const QFont &font, int size = 30);

    static QIcon getCircleCloseIcon(int size = 20);

    static QIcon getPassWordIcon(const QPixmap &pixmap, int size = 30);

    static QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color = Qt::black);

    static QIcon getCircleIcon(const QPixmap &pixmap, int diameter = 36);

    static QIcon getCircleIcon(const QIcon &icon, int diameter = 36);

    static void moveToCenter(QWidget* w);
};

#endif // UWIDGETUTILS_H
