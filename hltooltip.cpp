#include "hltooltip.h"
#include "effect/ugraphicsgloweffect.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QDebug>
#include <qmath.h>
#include <QPainter>
#include <QPainterPath>

HLToolTip::HLToolTip(QWidget* parent) : QWidget(parent)
{
    QWidget* widget = new QWidget(this);
    iconLabel = new QLabel(this);
    iconLabel->setAlignment(Qt::AlignVCenter);
    textLabel = new QLabel(this);
    textLabel->setAlignment(Qt::AlignVCenter);
    layout = new QHBoxLayout;
    widget->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(iconLabel);
    layout->addSpacing(5);
    layout->addWidget(textLabel);

    QVBoxLayout* vlayout = new QVBoxLayout;
    vlayout->addWidget(widget);
    this->setLayout(vlayout);

    addGraphicsEffect();
}

void HLToolTip::setIcon(const QIcon &icon)
{
    iconLabel->setPixmap(icon.pixmap(30, 30));
}

void HLToolTip::setText(const QString &text)
{
    textLabel->setText(text);
}

void HLToolTip::resizeEvent(QResizeEvent* event)
{
    iconLabel->adjustSize();
    textLabel->adjustSize();

    int width = layout->sizeHint().width() + 20;

    resize(QSize(width, 50));
}

void HLToolTip::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath rectPath;
    if (m_radius > 0)
        rectPath.addRoundedRect(this->rect(), m_radius, m_radius);
    else
        rectPath.addRect(this->rect());

    painter.fillPath(rectPath, palette().color(QPalette::Base));
}

void HLToolTip::addGraphicsEffect()
{
    if (graphicsClipEffect == Q_NULLPTR)
    {
        graphicsClipEffect = new UGraphicsGlowEffect(this);
        this->setGraphicsEffect(graphicsClipEffect);
    }
}
