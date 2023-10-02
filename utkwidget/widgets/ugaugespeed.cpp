#include "ugaugespeed.h"
#include "uapplication.h"
#include "style/upalette.h"

#include <QtMath>
#include <QPaintEvent>
#include <QPainter>

class UGaugeSpeedPrivate
{
public:
    UGaugeSpeedPrivate(UGaugeSpeed* q) : q_ptr(q) {}
    ~UGaugeSpeedPrivate() {};

    void drawRing(QPainter *painter);   //绘制表盘圆环
    void drawScale(QPainter *painter);  //绘制表盘刻度
    void drawScaleNum(QPainter *painter);  //绘制表盘刻度值
    void drawPointer(QPainter *painter);   //绘制表盘指针
    void drawValue(QPainter *painter);     //绘制表盘显示值

private:
    Q_DECLARE_PUBLIC(UGaugeSpeed)
    Q_DISABLE_COPY(UGaugeSpeedPrivate)
    UGaugeSpeed* const q_ptr = nullptr;

    QColor ringColor;
    int rotateAngle = 30;
    int radius = 100;
    double value = 30;
};

void UGaugeSpeedPrivate::drawRing(QPainter *painter)
{
    painter->save();
    QPen pen;
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidthF(20);
    QRect rect(-radius, -radius, 2*radius, 2*radius);

    //绘制第一圆环
    pen.setColor(uApp->applicationPalette()->color(UPalette::NormalBackground));
    painter->setPen(pen);
    painter->drawArc(rect, 150 * 16, 60 * 16);

    //绘制第二圆环
    pen.setColor(uApp->applicationPalette()->color(UPalette::Highlight));
    painter->setPen(pen);
    painter->drawArc(rect, 30 * 16, 120 * 16);

    //绘制第三圆环
    pen.setColor(uApp->applicationPalette()->color(UPalette::ErrorBackground));
    painter->setPen(pen);
    painter->drawArc(rect, -30 * 16, 60 * 16);
    painter->restore();
}

void UGaugeSpeedPrivate::drawScale(QPainter *painter)
{
    painter->save();
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(1.5);
    painter->rotate(30);
    for (int i=0; i<9; i++) {
        if(i<3)
        {
            ringColor = uApp->applicationPalette()->color(UPalette::NormalBackground);
        }
        else if (i>5) {
            ringColor = uApp->applicationPalette()->color(UPalette::ErrorBackground);
        }
        else {
            ringColor = uApp->applicationPalette()->color(UPalette::Highlight);
        }
        painter->rotate(rotateAngle);
        pen.setColor(ringColor);
        painter->setPen(pen);
        painter->drawLine(0, radius-25, 0, radius-1);
    }
    painter->restore();
}

void UGaugeSpeedPrivate::drawScaleNum(QPainter *painter)
{
    Q_Q(UGaugeSpeed);

    int starAngle = -30;
    painter->save();
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(1.5);
    pen.setColor(uApp->applicationPalette()->color(UPalette::ToolTipText));
    painter->setPen(pen);
    for (int i=0;i<9;i++) {
        starAngle -= 30;
        qreal x = (radius*2/3)*qSin(M_PI*starAngle/180);
        qreal y = (radius*2/3)*qCos(M_PI*starAngle/180);
        QString strValue = QString("%1M").arg(i*12.5);
        double textWidth = q->fontMetrics().width(strValue);
        double textHeight = q->fontMetrics().height();
        int pointX = x - textWidth / 2;
        int pointY = y + textHeight / 4;
        painter->drawText(pointX,pointY,strValue);
    }
    painter->restore();
}

void UGaugeSpeedPrivate::drawPointer(QPainter *painter)
{
    painter->save();
    painter->setBrush(Qt::darkBlue);

    QPolygon polygon;
    polygon.setPoints(4, -5, 0, 0, -8, 5, 0, 0, radius/2);

    painter->rotate(60);
    double angle = (value/100)*(300-60);
    painter->rotate(angle);
    painter->drawConvexPolygon(polygon);
    painter->restore();
}

void UGaugeSpeedPrivate::drawValue(QPainter *painter)
{
    Q_Q(UGaugeSpeed);

    painter->save();
    QPen pen;
    pen.setColor(uApp->applicationPalette()->color(UPalette::ToolTipText));
    painter->setPen(pen);

    QFont font = q->font();
    font.setBold(true);
    font.setPointSize(30);
    painter->setFont(font);

    int textWidth = q->fontMetrics().width(QString::number(value));
    painter->drawText(-(textWidth/2+15), radius*2/5, QString::number(value));
    painter->restore();
}

UGaugeSpeed::UGaugeSpeed(QWidget *parent) : QWidget(parent)
    , d_ptr(new UGaugeSpeedPrivate(this))
{
}

UGaugeSpeed::~UGaugeSpeed()
{
}

int UGaugeSpeed::getRadius()
{
    Q_D(UGaugeSpeed);

    return d->radius;
}

double UGaugeSpeed::getValue()
{
    Q_D(UGaugeSpeed);

    return d->value;
}

void UGaugeSpeed::paintEvent(QPaintEvent *e)
{
    Q_D(UGaugeSpeed);

    int width = this->width();
    int height = this->height();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);

    d->drawRing(&painter);
    d->drawScale(&painter);
    d->drawScaleNum(&painter);
    d->drawPointer(&painter);
    d->drawValue(&painter);

    QWidget::paintEvent(e);
}

void UGaugeSpeed::setRadius(int radius)
{
    Q_D(UGaugeSpeed);

    if (d->radius != radius) {
        d->radius = radius;
        update();
        emit radiusChanged();
    }
}

void UGaugeSpeed::setValue(int value)
{
    Q_D(UGaugeSpeed);

    if (d->value != value) {
        d->value = value;
        update();
        emit valueChanged();
    }
}
