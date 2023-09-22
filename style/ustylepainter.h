#ifndef USTYLEPAINTER_H
#define USTYLEPAINTER_H

#include "uproxystyle.h"

#include <QPainter>
#include <QStyle>
#include <QWidget>

class UStylePainter : public QPainter
{
public:
    inline UStylePainter() : QPainter(), widget(nullptr), wstyle(nullptr) {}
    inline explicit UStylePainter(QWidget* w)
    {
        begin(w, w);
    }
    inline UStylePainter(QPaintDevice* pd, QWidget* w)
    {
        begin(pd, w);
    }
    inline bool begin(QWidget* w)
    {
        return begin(w, w);
    }
    inline bool begin(QPaintDevice* pd, QWidget* w)
    {
        Q_ASSERT_X(w, "UStylePainter::UStylePainter", "Widget must be non-zero");
        widget = w;
        wstyle = w->style();
        uStyle = qobject_cast<UProxyStyle*>(wstyle);
        return QPainter::begin(pd);
    };
    inline QStyle* style() const
    {
        return wstyle;
    }
    void drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption &opt);
    void drawPrimitive(UProxyStyle::UPrimitiveElement pe, const QStyleOption &opt);
    void drawControl(QStyle::ControlElement ce, const QStyleOption &opt);
    void drawControl(UProxyStyle::UControlElement ce, const QStyleOption &opt);
    void drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex &opt);
    void drawComplexControl(UProxyStyle::UComplexControl cc, const QStyleOptionComplex &opt);
    void drawItemText(const QRect &r, int flags, const QPalette &pal, bool enabled,
                      const QString &text, QPalette::ColorRole textRole = QPalette::NoRole);
    void drawItemPixmap(const QRect &r, int flags, const QPixmap &pixmap);

private:
    QWidget* widget = nullptr;
    QStyle* wstyle = nullptr;
    UProxyStyle* uStyle = nullptr;
    Q_DISABLE_COPY(UStylePainter)
};

#endif // USTYLEPAINTER_H
