#include "hlslider.h"
#include "style/uproxystyle.h"

#include <QStyleOptionSlider>
#include <QDebug>
#include <QPaintEvent>

HLSlider::HLSlider(QWidget* parent) : QSlider(parent)
{
    if (nullptr == qobject_cast<UProxyStyle*>(this->style()))
    {
        m_style.reset(new UProxyStyle());
        setStyle(m_style.get());
    }
    setRange(0, 100);
}

HLSlider::HLSlider(Qt::Orientation orientation, QWidget* parent) : QSlider(orientation, parent)
{
    if (nullptr == qobject_cast<UProxyStyle*>(this->style()))
    {
        m_style.reset(new UProxyStyle());
        setStyle(m_style.get());
    }
    setRange(0, 100);
}

void HLSlider::mousePressEvent(QMouseEvent* ev)
{
    QSlider::mousePressEvent(ev);
    //    QStyleOptionSlider opt;
    //    initStyleOption(&opt);
    //    int available = style()->pixelMetric(QStyle::PM_SliderSpaceAvailable, &opt, this);
    int value = QStyle::sliderValueFromPosition(minimum(), maximum(), ev->pos().x(), width());
    setValue(value);
}
