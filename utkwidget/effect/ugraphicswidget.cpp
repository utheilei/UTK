#include "ugraphicswidget.h"

#include <QGraphicsBlurEffect>
#include <QPainter>

QT_BEGIN_NAMESPACE
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter* p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

UGraphicsBlurWidget::UGraphicsBlurWidget(QWidget* parent) : QWidget(parent)
{
}

void UGraphicsBlurWidget::setDistance(int distance)
{
    m_distance = distance;
}

int UGraphicsBlurWidget::distance()
{
    return m_distance;
}

void UGraphicsBlurWidget::setOpacity(qreal opacity)
{
    m_opacity = opacity;
}

qreal UGraphicsBlurWidget::opacity()
{
    return m_opacity;
}

qreal UGraphicsBlurWidget::blurRadius()
{
    return m_blurRadius;
}

void UGraphicsBlurWidget::setBlurRadius(qreal blurRadius)
{
    m_blurRadius = blurRadius;
}

void UGraphicsBlurWidget::setPixmap(const QPixmap &pixmap)
{
    m_pixmap = pixmap;
    update();
}

QPixmap UGraphicsBlurWidget::pixmap() const
{
    return m_pixmap;
}

void UGraphicsBlurWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    if ((blurRadius() + distance()) <= 0)
    {
        QWidget::paintEvent(event);
        return;
    }

    if (m_pixmap.isNull())
    {
        QWidget::paintEvent(event);
        return;
    }

    painter.setOpacity(opacity());

    QTransform restoreTransform = painter.worldTransform();
    painter.setWorldTransform(QTransform());

    QSize scaleSize(width() + 2 * distance(), height() + 2 * distance());

    QImage tmpImg(scaleSize, QImage::Format_ARGB32_Premultiplied);
    QPixmap scaled = m_pixmap.scaled(scaleSize);
    tmpImg.fill(0);
    QPainter tmpPainter(&tmpImg);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPointF(-distance(), -distance()), scaled);
    tmpPainter.end();

    QImage blurred(tmpImg.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmpImg, blurRadius(), true, false);
    blurPainter.end();

    tmpImg = blurred;

    painter.drawImage(QPoint(0, 0), tmpImg);

    painter.setWorldTransform(restoreTransform);
    painter.setOpacity(1);
}

QSize UGraphicsBlurWidget::sizeHint() const
{
    return m_pixmap.isNull() ? QWidget::sizeHint() : m_pixmap.size();
}
