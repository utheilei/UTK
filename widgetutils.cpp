#include "widgetutils.h"

#include <QPainter>
#include <QPainterPath>
#include <QApplication>
#include <QPalette>

WidgetUtils::WidgetUtils()
{

}

QIcon WidgetUtils::getNumCircleIcon(int num, const QFont &font, int size)
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

QIcon WidgetUtils::getCircleCloseIcon(int size)
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

QIcon WidgetUtils::getPassWordIcon(const QPixmap &pixmap, int size)
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
