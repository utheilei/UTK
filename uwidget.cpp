#include "uwidget.h"

#include <QPainter>
#include <QPainterPath>

UWidget::UWidget(QWidget* parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}

void UWidget::addDropShadowEffect(bool isOpen)
{
    if (m_isOpen != isOpen)
    {
        m_isOpen = isOpen;
        update();
    }
}

int UWidget::margins() const
{
    return m_margins;
}

void UWidget::setMargins(int margins)
{
    if (m_margins != margins)
    {
        m_margins = margins;
        update();
    }
}

void UWidget::setRadius(int radius)
{
    if (m_radius != radius)
    {
        m_radius = radius;
        update();
    }
}

void UWidget::setBackgroundColor(const QBrush &brush)
{
    auto palette = this->palette();
    palette.setBrush(QPalette::Background, brush);
    setPalette(palette);
}

void UWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    QRect rect = QRect(m_margins, m_margins, width() - m_margins * 2, height() - m_margins * 2);
    if (m_radius > 0)
        rectPath.addRoundedRect(rect, m_radius, m_radius);
    else
        rectPath.addRect(rect);
    painter.fillPath(rectPath, palette().color(QPalette::Background));

    if (m_isOpen)
    {
        QColor color(92, 93, 95, 50);
        int arr[10] = {150, 120, 80, 50, 40, 30, 20, 10, 5, 5};
        for (int i = 0; i < 10; i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(rect.topLeft().x() - i - 1, rect.topLeft().y() - i - 1,
                                ((i + 1) * 2 + rect.width()), ((i + 1) * 2 + rect.height()), 15, 15);

            color.setAlpha(arr[i]);
            painter.setPen(color);
            painter.drawPath(path);
        }
    }
}
