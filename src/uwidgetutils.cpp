#include "uwidgetutils.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QDesktopWidget>
#include <QPalette>

QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter* p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE

UWidgetUtils::UWidgetUtils()
{
}

QIcon UWidgetUtils::getNumCircleIcon(int num, const QFont &font, int size)
{
    QPixmap output(QSize(size, size));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.fillPath(path, Qt::gray);
    painter.setFont(font);
    painter.setPen(qApp->palette().color(QPalette::Text));
    painter.drawText(QRect(0, 0, size, size), Qt::TextSingleLine | Qt::AlignCenter, QString::number(num));
    return QIcon(output);
}

QIcon UWidgetUtils::getCircleCloseIcon(int size)
{
    QPixmap output(QSize(size, size));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    QRect rect(0, 0, size, size);
    path.addEllipse(rect);
    painter.setClipPath(path);
    painter.fillPath(path, qApp->palette().color(QPalette::Base));
    painter.drawPixmap(rect.adjusted(6, 6, -6, -6), QPixmap(":/images/close.svg"));
    return QIcon(output);
}

QIcon UWidgetUtils::getPassWordIcon(const QPixmap &pixmap, int size)
{
    QPixmap output(QSize(size, size));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    QRect rect(0, 0, size, size);
    path.addRoundedRect(rect, 8, 8);
    painter.setClipPath(path);
    painter.fillPath(path, qApp->palette().color(QPalette::Highlight));
    painter.drawPixmap(rect.adjusted(6, 6, -6, -6), pixmap);
    return QIcon(output);
}

QImage UWidgetUtils::dropShadow(const QPixmap &px, qreal radius, const QColor &color)
{
    if (px.isNull())
    {
        return QImage();
    }

    QSize size = px.size();

    QImage tmp(size + QSize(radius * 2, radius * 2), QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPoint(radius, radius), px);
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, radius, false, true);
    blurPainter.end();

    if (color == QColor(Qt::black))
    {
        return blurred;
    }

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

void UWidgetUtils::moveToCenter(QWidget* w)
{
    Q_ASSERT(w != nullptr);

    QDesktopWidget* dw = QApplication::desktop();
    QRect parentRect = dw->availableGeometry(dw->primaryScreen());

    if (w->parentWidget())
    {
        parentRect = w->parentWidget()->geometry();
    }
    w->move(parentRect.center() - w->rect().center());
}

QIcon UWidgetUtils::getCircleIcon(const QPixmap &pixmap, int diameter)
{
    QPixmap output(QSize(diameter, diameter));
    output.fill(Qt::transparent);
    QPainter painter(&output);
    // 抗锯齿 + 平滑边缘处理
    painter.setRenderHints(QPainter::Antialiasing, true);
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    painter.setPen(Qt::NoPen);

    QPainterPath path;
    path.addEllipse(0, 0, diameter, diameter);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, diameter, diameter, pixmap);
    return QIcon(output);
}

QIcon UWidgetUtils::getCircleIcon(const QIcon &icon, int diameter)
{
    QPixmap pixmap = icon.pixmap(QSize(diameter, diameter));
    return getCircleIcon(pixmap, diameter);
}
