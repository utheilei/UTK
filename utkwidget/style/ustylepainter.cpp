#include "ustylepainter.h"

void UStylePainter::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption &opt)
{
    wstyle->drawPrimitive(pe, &opt, this, widget);
}

void UStylePainter::drawPrimitive(UProxyStyle::UPrimitiveElement pe, const QStyleOption &opt)
{
    if (uStyle)
    {
        uStyle->drawPrimitive(pe, &opt, this, widget);
    }
}

void UStylePainter::drawControl(QStyle::ControlElement ce, const QStyleOption &opt)
{
    wstyle->drawControl(ce, &opt, this, widget);
}

void UStylePainter::drawControl(UProxyStyle::UControlElement ce, const QStyleOption &opt)
{
    if (uStyle)
    {
        uStyle->drawControl(ce, &opt, this, widget);
    }
}

void UStylePainter::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex &opt)
{
    wstyle->drawComplexControl(cc, &opt, this, widget);
}

void UStylePainter::drawComplexControl(UProxyStyle::UComplexControl cc, const QStyleOptionComplex &opt)
{
    if (uStyle)
    {
        uStyle->drawComplexControl(cc, &opt, this, widget);
    }
}

void UStylePainter::drawItemText(const QRect &r, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole)
{
    wstyle->drawItemText(this, r, flags, pal, enabled, text, textRole);
}

void UStylePainter::drawItemPixmap(const QRect &r, int flags, const QPixmap &pixmap)
{
    wstyle->drawItemPixmap(this, r, flags, pixmap);
}
