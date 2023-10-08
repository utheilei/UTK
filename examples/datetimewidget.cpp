#include "datetimewidget.h"
#include "widgets/uclockwidget.h"
#include "widgets/ugaugespeed.h"
#include "widgets/hlslider.h"
#include "widgets/uwaterprogress.h"

#include <QBoxLayout>

DateTimeWidget::DateTimeWidget(QWidget* parent)
    : UWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setRadius(8);

    QGridLayout* mainLayout = new QGridLayout;

    UClockWidget* clockWidget = new UClockWidget(this);
    UGaugeSpeed* gauge = new UGaugeSpeed(this);
    gauge->setRadius(150);

    auto slider = new HLSlider(Qt::Horizontal, this);
    slider->setMaximumWidth(300);
    slider->setMaximumHeight(40);

    UWaterProgress* progress = new UWaterProgress(this);
    UWaterProgress* progress1 = progress;
    progress1->setMinimumSize(200, 200);
    progress1->start();

    mainLayout->addWidget(clockWidget, 0, 0);
    mainLayout->addWidget(gauge, 0, 1);
    mainLayout->addWidget(slider, 1, 0);
    mainLayout->addWidget(progress1, 1, 1);

    setLayout(mainLayout);

    connect(slider, &QSlider::valueChanged, this, [gauge, progress](int value)
    {
        gauge->setValue(value);
        progress->setValue(value);
    });

    slider->setValue(60);
}

DateTimeWidget::~DateTimeWidget()
{

}
