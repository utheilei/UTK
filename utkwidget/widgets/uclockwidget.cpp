#include "uclockwidget.h"
#include "uapplication.h"
#include "style/upalette.h"

#include <QPainter>
#include <QPainterPath>
#include <QTime>
#include <QTimer>
#include <QtMath>

class UClockWidgetPrivate
{
public:
    UClockWidgetPrivate(UClockWidget* q) : q_ptr(q) {}
    ~UClockWidgetPrivate() {};

    void drawScale(QPainter *painter);  //绘制表盘刻度

    void drawScaleNum(QPainter *painter);  //绘制表盘刻度值

    void drawPointer(QPainter *painter);   //绘制表盘指针
private:
    Q_DECLARE_PUBLIC(UClockWidget)
    Q_DISABLE_COPY(UClockWidgetPrivate)
    UClockWidget* const q_ptr = nullptr;
    int radius = 100;
    QTimer time;
};

void UClockWidgetPrivate::drawScale(QPainter *painter)
{
    painter->save();
    for (int i=1; i<13; i++) {
        QPen pen(uApp->applicationPalette()->color(QPalette::ToolTipText), 2);
        painter->setPen(pen);
        painter->drawLine(QPoint(0, -(radius-10)), QPoint(0, -(radius-20)));
        painter->rotate(30);
    }
    painter->restore();

    painter->save();
    for (int i=1; i<61; i++) {
        painter->drawLine(QPoint(0, -(radius-10)), QPoint(0, -(radius-16)));
        painter->rotate(6);
    }
    painter->restore();
}

void UClockWidgetPrivate::drawScaleNum(QPainter *painter)
{
    Q_Q(UClockWidget);

    int starAngle = 30;
    painter->save();
    for (int i=1;i<13;i++) {
        qreal x = (radius-30)*qSin(M_PI*starAngle/180);
        qreal y = -(radius-30)*qCos(M_PI*starAngle/180);
        qreal textWidth = q->fontMetrics().width(QString::number(i));
        qreal textHeight = q->fontMetrics().height();
        int pointX = int(x - textWidth/2);
        int pointY = int(y + textHeight/4);
        painter->drawText(pointX,pointY,QString::number(i));
        starAngle += 30;
    }
    painter->restore();
}

void UClockWidgetPrivate::drawPointer(QPainter *painter)
{
    QTime time = QTime::currentTime();
    int hour = time.hour();
    int minute = time.minute();
    int second = time.second();
    if (hour > 12)
        hour = hour - 12;

    painter->save();
    painter->setBrush(uApp->applicationPalette()->brush(QPalette::ToolTipText));
    QPolygon hPolygon;
    hPolygon << QPoint(0,0) << QPoint(5,-8) << QPoint(0,-(radius/3*2)) << QPoint(-5,-8);
    int angle = hour*30;
    int pAngle = minute/2;
    painter->rotate(angle + pAngle);
    painter->drawConvexPolygon(hPolygon);
    painter->restore();

    painter->save();
    painter->setBrush(uApp->applicationPalette()->brush(QPalette::ToolTipText));
    QPolygon mPolygon;
    mPolygon << QPoint(0,0) << QPoint(5,-8) << QPoint(0,-(radius/4*3)) << QPoint(-5,-8);
    int mAngle = minute*6;
    painter->rotate(mAngle);
    painter->drawConvexPolygon(mPolygon);
    painter->restore();

    painter->save();
    painter->setBrush(uApp->applicationPalette()->brush(UPalette::ErrorBackground));
    QPolygon sPolygon;
    sPolygon << QPoint(0,0) << QPoint(5,-8) << QPoint(0,-(radius-10)) << QPoint(-5,-8);
    int sAngle = second*6;
    painter->rotate(sAngle);
    painter->drawConvexPolygon(sPolygon);
    painter->restore();
}

UClockWidget::UClockWidget(QWidget *parent) : QWidget(parent)
    , d_ptr(new UClockWidgetPrivate(this))
{
    Q_D(UClockWidget);

    connect(&d->time, &QTimer::timeout, this, [this](){
        this->update();
    });

    d->time.start(1000);
}

UClockWidget::~UClockWidget()
{
    stopTime();
}

void UClockWidget::setRadius(int radius)
{
    Q_D(UClockWidget);

    if (d->radius != radius) {
        d->radius = radius;
        update();
    }
}

void UClockWidget::startTime()
{
    Q_D(UClockWidget);

    if (!d->time.isActive())
        d->time.start(1000);
}

void UClockWidget::stopTime()
{
    Q_D(UClockWidget);

    if (d->time.isActive())
        d->time.stop();
}

void UClockWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    Q_D(UClockWidget);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPointF pos = QPointF(width() / 2.0, height() / 2.0);
    painter.translate(pos);

    QPainterPath path;
    path.addEllipse(QPoint(0,0), d->radius, d->radius);
    painter.fillPath(path, palette().brush(QPalette::Button));

    painter.save();
    painter.drawEllipse(QPoint(0,0), d->radius, d->radius);
    painter.drawEllipse(QPoint(0,0), d->radius-5, d->radius-5);
    painter.restore();

    d->drawScale(&painter);
    d->drawScaleNum(&painter);
    d->drawPointer(&painter);
}
