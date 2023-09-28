#include "stylebutton.h"

#include <QPainterPath>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

StyleButton::StyleButton(const QString text, QWidget *parent)
    : QPushButton(text, parent)
{
}

void StyleButton::setRadius(int radius)
{
    if (m_radius != radius) {
        m_radius = radius;
        update();
    }
}

void StyleButton::setHoverColor(QColor color)
{
    m_color = color;
}

void StyleButton::setRadiuPosition(StyleButton::RadiuPosition position)
{
    if (m_position != position) {
        m_position = position;
        update();
    }
}

void StyleButton::enterEvent(QEvent *e)
{
    m_isEnter = true;
    QPushButton::enterEvent(e);
}
void StyleButton::leaveEvent(QEvent *e)
{
    m_isEnter = false;
    QPushButton::leaveEvent(e);
}

void StyleButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    int padding = 2;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QRectF rect = this->rect();
    QRectF paddingRect = rect.adjusted(padding, padding, -padding, -padding);
    QPalette pa = this->palette();

    QPainterPath painterPath;
    QPainterPath framePath;
    painterPath.setFillRule(Qt::WindingFill);
    if (m_radius > 0) {
        framePath.addRoundedRect(paddingRect, m_radius, m_radius);
        painterPath.addRoundedRect(rect, m_radius, m_radius);
        if (m_position == RadiuPosition::TopRight) {
            painterPath.addRect(QRect(0, 0, width()/2, height()));
            painterPath.addRect(QRect(0, height()/2, width(), height()/2));
        }
    } else {
        painterPath.addRect(rect);
        framePath.addRect(paddingRect);
    }

    if (m_isEnter) {
        painter.setBrush(m_color.isValid() ? m_color : QBrush(pa.color(QPalette::Button)));
        painter.setPen(Qt::NoPen);
        painter.drawPath(painterPath);
    }

    if (isDown()) {
        QColor color = pa.color(QPalette::Button);
        color = color.darker(120);
        painter.fillPath(painterPath, color);
    }

    QStyleOptionButton btn;
    initStyleOption(&btn);
    QStylePainter stylePainter(this);
    stylePainter.drawControl(QStyle::CE_PushButtonLabel, btn);

    if (!isFlat()) {
        painter.save();
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(isDown() ? pa.color(QPalette::Highlight) : Qt::black, padding));
        painter.drawPath(framePath);
        painter.restore();
    }
}
