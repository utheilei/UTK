#ifndef UGRAPHICSWIDGET_H
#define UGRAPHICSWIDGET_H

#include "utkwidget_export.h"

#include <QWidget>

class UTKWIDGET_EXPORT UGraphicsBlurWidget : public QWidget
{
public:
    explicit UGraphicsBlurWidget(QWidget* parent = nullptr);

    void setDistance(int distance);

    int distance();

    void setOpacity(qreal opacity);

    qreal opacity();

    qreal blurRadius();

    void setBlurRadius(qreal blurRadius);

    void setPixmap(const QPixmap &pixmap);

    QPixmap pixmap() const;

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize sizeHint() const override;

private:
    int m_distance = 5;
    qreal m_opacity = 0.6;
    qreal m_blurRadius = 50;
    QPixmap m_pixmap;
};

#endif // UGRAPHICSWIDGET_H
