#include "hllabel.h"

#include <QPainter>
#include <QPainterPath>

ToolTip::ToolTip(QWidget* parent) :
    QLabel(parent)
{
    setContentsMargins(5, 5, 5, 5);
//    setWindowFlags(windowFlags() | Qt::ToolTip | Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);
}

ToolTip::~ToolTip()
{
}

void ToolTip::setRadius(int radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;
        update();
    }
}

void ToolTip::setToolTipFont(const QFont &font)
{
    setFont(font);
}

void ToolTip::setBackgroundColor(const QBrush &brush)
{
    auto palette = this->palette();
    palette.setBrush(QPalette::Background, brush);
    setPalette(palette);
}

void ToolTip::setTextColor(const QColor &color)
{
    auto palette = this->palette();
    palette.setBrush(QPalette::Foreground, color);
    setPalette(palette);
}

void ToolTip::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    QRect rect = this->rect();
    if (m_radius > 0)
        rectPath.addRoundedRect(rect, m_radius, m_radius);
    else
        rectPath.addRect(rect);

    painter.fillPath(rectPath, palette().color(QPalette::Background));

    QLabel::paintEvent(event);
}
