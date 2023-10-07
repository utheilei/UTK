#include "uwaterprogress.h"

#include <QtMath>
#include <QTimer>
#include <QPainterPath>
#include <QPainter>
#include <QRandomGenerator>
#include <QLinearGradient>

class UWaterProgressPrivate
{
public:
    UWaterProgressPrivate(UWaterProgress* q) : q_ptr(q) {}
    ~UWaterProgressPrivate() {};

    void drawWater(QPainter* painter, int radius);

    void drawText(QPainter* painter, int radius);

    void drawBg(QPainter* painter, int radius);

private:
    Q_DECLARE_PUBLIC(UWaterProgress)
    Q_DISABLE_COPY(UWaterProgressPrivate)
    UWaterProgress* const q_ptr = nullptr;

    double value = 30.0;
    QColor beginColor = QColor(0, 129, 255);
    QColor endColor = QColor(35, 215, 255);
    QTimer timer;
    double waveWidth = 0.05;
    double waveHeight = 4.0;
    double offsetx = 0.5;
    double speed = 0.5;
};

void UWaterProgressPrivate::drawWater(QPainter* painter, int radius)
{
    double sinAlph = 0.1;
    QRect rect(-radius, -radius, 2 * radius, 2 * radius);
    int startX = -radius;
    int endX = radius;
    int endY = radius;
    double height = radius - ((value * radius) / 50);

    QPainterPath waterPath1;
    waterPath1.moveTo(startX, endY);
    QPainterPath waterPath2;
    waterPath2.moveTo(startX, endY);

    for (int i = startX; i <= endX; i++)
    {
        float y1, y2;
        y1 = -waveHeight * qSin(i * waveWidth + offsetx + 2.5) + height;
        y2 = -waveHeight * qSin(i * waveWidth + offsetx) + height;

        if (value == 0)
        {
            y1 = y2 = endY;
        }
        if (value == 100)
        {
            y1 = y2 = -endY;
        }
        waterPath1.lineTo(i, y1);
        waterPath2.lineTo(i, y2);
    }
    waterPath1.lineTo(endX, endY);
    waterPath2.lineTo(endX, endY);

    QPainterPath bigPath;
    bigPath.addEllipse(rect);
    waterPath1 = bigPath.intersected(waterPath1);
    waterPath2 = bigPath.intersected(waterPath2);
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(Qt::NoPen);
    QLinearGradient linearGradient(rect.topLeft(), rect.bottomRight());
    linearGradient.setColorAt(0, beginColor);
    linearGradient.setColorAt(1, endColor);
    painter->setBrush(linearGradient);
    painter->drawPath(waterPath2);
    linearGradient.setColorAt(0, QColor(beginColor.red(), beginColor.green(), beginColor.blue(), 100));
    linearGradient.setColorAt(1, QColor(endColor.red(), endColor.green(), endColor.blue(), 100));
    painter->setBrush(linearGradient);
    painter->drawPath(waterPath1);
    painter->restore();
}

void UWaterProgressPrivate::drawText(QPainter* painter, int radius)
{
    Q_Q(UWaterProgress);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    QString text = QString("%1%").arg(QString::number(value, 'f', 1));
    QFont font = q->font();
    double size = (32.0 / 236.0) * radius * 2;
    if (size < 12)
    {
        size = 12;
    }
    font.setPixelSize(size);
    painter->setFont(font);
    QPen pen = painter->pen();
    pen.setColor(q->palette().color(QPalette::ToolTipText));
    pen.setWidth(4);
    painter->setPen(pen);
    QRect rect(-radius, -radius, radius * 2, radius * 2);
    painter->drawText(rect, Qt::AlignCenter, text);
    painter->restore();
}

void UWaterProgressPrivate::drawBg(QPainter* painter, int radius)
{
    Q_Q(UWaterProgress);

    radius = radius - 0.5;
    int innerRadius = radius - 6;
    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter->setPen(QColor(endColor.red(), endColor.green(), endColor.blue(), 100));
    painter->setBrush(Qt::NoBrush);
    QRect rect(-radius, -radius, radius * 2, radius * 2);
    QRect innerRect(-innerRadius, -innerRadius, innerRadius * 2, innerRadius * 2);
    painter->drawEllipse(rect);
    painter->setPen(q->palette().color(QPalette::Button));
    painter->drawEllipse(innerRect);
    painter->restore();
}

UWaterProgress::UWaterProgress(QWidget* parent) : QWidget(parent)
    , d_ptr(new UWaterProgressPrivate(this))
{
    Q_D(UWaterProgress);

    d->timer.setInterval(100);
    connect(&d->timer, &QTimer::timeout, this, [ = ]()
    {
        d->offsetx = d->offsetx + d->speed;
        d->waveHeight = QRandomGenerator::global()->bounded(1.0) + 4.0;
        update();
    });
}

UWaterProgress::~UWaterProgress()
{
    stop();
}

double UWaterProgress::getValue() const
{
    Q_D(const UWaterProgress);

    return d->value;
}

void UWaterProgress::start()
{
    Q_D(UWaterProgress);

    d->timer.start();
}

void UWaterProgress::stop()
{
    Q_D(UWaterProgress);

    d->timer.stop();
}

void UWaterProgress::setValue(double value)
{
    Q_D(UWaterProgress);

    d->value = value;
    update();
}

void UWaterProgress::setBeginColor(const QColor &beginColor)
{
    Q_D(UWaterProgress);

    d->beginColor = beginColor;
    update();
}

void UWaterProgress::setEndColor(const QColor &endColor)
{
    Q_D(UWaterProgress);

    d->endColor = endColor;
    update();
}

void UWaterProgress::paintEvent(QPaintEvent* event)
{
    Q_D(UWaterProgress);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(rect().center());

    int radius = qMin(this->width(), this->height()) / 2;
    d->drawBg(&painter, radius);
    int innerRadius = radius - 6;
    QRect innerRect(-innerRadius, -innerRadius, innerRadius * 2, innerRadius * 2);
    QPainterPath bigPath;
    bigPath.addEllipse(innerRect);
    painter.setClipPath(bigPath);
    d->drawWater(&painter, radius);
    d->drawText(&painter, radius);
    QWidget::paintEvent(event);
}
