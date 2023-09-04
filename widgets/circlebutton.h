#ifndef CIRCLEBUTTON_H
#define CIRCLEBUTTON_H

#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QStyle>

class CircleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CircleButton(const QPixmap &pixmap, QWidget *parent = nullptr);
    CircleButton(QStyle::StandardPixmap st, QWidget *parent = nullptr);

    static QIcon getCircleIcon(const QPixmap &pixmap, int diameter = 36);
    static QIcon getCircleIcon(const QIcon &icon, int diameter = 36);

protected:
    void paintEvent(QPaintEvent *) override;
};

#endif // CIRCLEBUTTON_H
