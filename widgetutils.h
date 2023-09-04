#ifndef WIDGETUTILS_H
#define WIDGETUTILS_H

#include <QIcon>

class WidgetUtils
{
public:
    WidgetUtils();

    static QIcon getNumCircleIcon(int num, const QFont &font, int size = 30);

    static QIcon getCircleCloseIcon(int size = 20);

    static QIcon getPassWordIcon(const QPixmap &pixmap, int size = 30);
};

#endif // WIDGETUTILS_H
