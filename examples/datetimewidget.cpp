#include "datetimewidget.h"
#include "widgets/uclockwidget.h"
#include "widgets/ugaugespeed.h"
#include "widgets/hlslider.h"

#include <QBoxLayout>

DateTimeWidget::DateTimeWidget(QWidget *parent)
    : UWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setRadius(8);
    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;

    UClockWidget *clockWidget = new UClockWidget(this);
    UGaugeSpeed *gauge = new UGaugeSpeed(this);
    gauge->setRadius(150);

    hLayout->addWidget(clockWidget);
    hLayout->addWidget(gauge);

    auto slider = new HLSlider(Qt::Horizontal, this);
    slider->setMaximumWidth(300);
    slider->setMaximumHeight(40);

    vLayout->addLayout(hLayout);
    vLayout->addWidget(slider);

    setLayout(vLayout);

    connect(slider, &QSlider::valueChanged, this, [gauge](int value){
        gauge->setValue(value);
    });

    slider->setValue(60);
}

DateTimeWidget::~DateTimeWidget()
{

}
