#include "uscrollbar.h"
#include "style/ustylepainter.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QStyleOptionSlider>
#include <private/qscrollbar_p.h>

UScrollBar::UScrollBar(QWidget* parent) : QScrollBar(parent)
{
    setStyle(new UProxyStyle);
    initAnimation();
}

UScrollBar::UScrollBar(Qt::Orientation orientation, QWidget* parent) : QScrollBar(orientation, parent)
{
    setStyle(new UProxyStyle);
    initAnimation();
}

void UScrollBar::initAnimation()
{
    m_propertyAnimation.setPropertyName("opacity");
    m_propertyAnimation.setTargetObject(this);
    m_propertyAnimation.setStartValue(1.0);
    m_propertyAnimation.setEndValue(0.0);
    m_propertyAnimation.setDuration(500);
    connect(&m_propertyAnimation, &QPropertyAnimation::valueChanged, this, [ = ](const QVariant & value)
    {
        setOpacity(value.toDouble());
    });
}

void UScrollBar::handleAnimation(qreal startValue, qreal endValue)
{
    if (m_propertyAnimation.state() != QPropertyAnimation::Stopped)
    {
        m_propertyAnimation.stop();
    }
    m_propertyAnimation.setStartValue(startValue);
    m_propertyAnimation.setEndValue(endValue);
    m_propertyAnimation.start();
}

qreal UScrollBar::opacity() const
{
    return m_opacity;
}

void UScrollBar::setOpacity(qreal opacity)
{
    if (m_opacity != opacity)
    {
        m_opacity = opacity;
        update();
        emit opacityChanged();
    }
}

void UScrollBar::paintEvent(QPaintEvent* event)
{
    QStyleOptionSlider option;
    initStyleOption(&option);
    QScrollBarPrivate* bp = static_cast<QScrollBarPrivate*>(QScrollBar::d_ptr.data());
    option.subControls = QStyle::SC_All;
    if (bp->pressedControl)
    {
        option.activeSubControls = (QStyle::SubControl)bp->pressedControl;
        if (!bp->pointerOutsidePressedControl)
            option.state |= QStyle::State_Sunken;
    }
    else
    {
        option.activeSubControls = (QStyle::SubControl)bp->hoverControl;
    }

    UStylePainter painter(this);
    painter.setOpacity(m_opacity);
    painter.drawComplexControl(UProxyStyle::UComplexControl::CC_UScrollBar, option);
}

void UScrollBar::enterEvent(QEvent* event)
{
    handleAnimation(opacity(), 1.0);
    QScrollBar::enterEvent(event);
}

void UScrollBar::leaveEvent(QEvent* event)
{
    handleAnimation(opacity(), 0.0);
    QScrollBar::leaveEvent(event);
}
