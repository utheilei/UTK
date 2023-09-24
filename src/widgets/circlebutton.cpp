#include "circlebutton.h"
#include <QBitmap>
#include <QDebug>
#include <QPainterPath>
#include <QStyleOption>

CircleButton::CircleButton(const QPixmap &pixmap, QWidget *parent) : QPushButton(parent)
{
    setFixedSize(60, 60);
    setFocusPolicy(Qt::StrongFocus);
    setIcon(QIcon(pixmap));
    setCheckable(true);
}

CircleButton::CircleButton(QStyle::StandardPixmap st, QWidget *parent) : QPushButton(parent)
{
    setFixedSize(60, 60);
    setFocusPolicy(Qt::StrongFocus);
    QIcon icon = style()->standardIcon(st, nullptr, this);
    setIcon(icon);
    setCheckable(true);
}

QIcon CircleButton::getCircleIcon(const QPixmap &pixmap, int diameter)
{
    QPixmap output(QSize(diameter, diameter));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(0, 0, diameter, diameter);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, diameter, diameter, pixmap);
    return QIcon(output);
}

QIcon CircleButton::getCircleIcon(const QIcon &icon, int diameter)
{
    QPixmap pixmap = icon.pixmap(QSize(diameter, diameter));
    return getCircleIcon(pixmap, diameter);
}

void CircleButton::paintEvent(QPaintEvent *e)
{
    auto style = this->style();
    QStyleOptionButton option;
    initStyleOption(&option);

    QPainterPath painterPath;
    painterPath.addEllipse(QRect(0, 0, width(), height()));

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setClipPath(painterPath);

    painter.drawPixmap(e->rect(), icon().pixmap(size()));

    QPalette pal(palette());
    QColor color =  pal.color(QPalette::Highlight);

    if (isChecked() && isCheckable()) {
        QPen pen(Qt::transparent);
        pen.setWidth(4);
        pen.setColor(color);
        painter.setPen(pen);
        painter.drawEllipse(rect());

        int radius = 8;
        int cx = this->rect().center().x();
        int cy = this->rect().center().y();
        QRect crect(QPoint(cx - radius, cy - radius), QPoint(cx + radius, cy + radius));
        option.rect = crect;
        option.state |= QStyle::State_On;

        style->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &option, &painter, this);
    }
}
