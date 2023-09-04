#include "hlstylebutton.h"

#include <QPainterPath>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>

constexpr int margins = 10;
constexpr int radius = 8;

HLStyleButton::HLStyleButton(const QString text, QWidget *parent)
    : QPushButton(text, parent)
{
    QFont f = font();
    f.setPixelSize(16);
    setFlat(true);
    setCheckable(true);
    setContentsMargins(margins, margins, contentsMargins().top(), contentsMargins().bottom());
}

void HLStyleButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QStyleOptionButton btn;
    initStyleOption(&btn);
    QStylePainter stylePainter(this);
    stylePainter.setRenderHint(QPainter::Antialiasing);
    QRectF rect = this->rect();

    QColor brush = palette().color(QPalette::Button);
    stylePainter.setBrush(brush.dark(150));
    stylePainter.setPen(Qt::NoPen);
    if (btn.state & QStyle::State_Enabled) {
        if (btn.state & QStyle::State_MouseOver)
            stylePainter.setBrush(brush.dark(200));
    }

    if (isChecked())
        stylePainter.setBrush(palette().color(QPalette::Highlight));

    QPainterPath painterPath;
    painterPath.addRoundedRect(rect, radius, radius);
    stylePainter.drawPath(painterPath);

//    stylePainter.drawControl(QStyle::CE_PushButtonBevel, btn);
    stylePainter.drawControl(QStyle::CE_PushButtonLabel, btn);
}
