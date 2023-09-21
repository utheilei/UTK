#include "hlslider.h"
#include "popupmenustyle.h"

#include <QStyleOptionSlider>
#include <QDebug>
#include <QPaintEvent>

HLSlider::HLSlider(QWidget* parent) : QSlider(parent)
{
    setStyle(new PopupMenuStyle);
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
