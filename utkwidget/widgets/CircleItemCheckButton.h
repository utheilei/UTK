// 圆形图片按钮
#ifndef CIRCLEITEMCHECKBUTTON_H
#define CIRCLEITEMCHECKBUTTON_H

#include "utkwidget_export.h"

#include <QPushButton>

class UTKWIDGET_EXPORT CircleItemCheckButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CircleItemCheckButton(QWidget* parent = Q_NULLPTR);

    QPixmap pixmapToRound(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;
};

#endif // CIRCLEITEMCHECKBUTTON_H
