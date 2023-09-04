#ifndef UPROXYSTYLE_H
#define UPROXYSTYLE_H

#include <QProxyStyle>

class QStyleOptionMenuItem;
class UProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    UProxyStyle();
    ~UProxyStyle() override;

    QPalette standardPalette() const override;

    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
                  const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
                    const QWidget* widget = nullptr) const override;

    void drawControl(ControlElement element, const QStyleOption* option,
                     QPainter* painter, const QWidget* widget) const override;

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                       QPainter* painter, const QWidget* widget = nullptr) const override;

    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
                            QPainter* painter, const QWidget* widget = nullptr) const override;

    QSize sizeFromContents(ContentsType type, const QStyleOption* option,
                           const QSize &contentsSize, const QWidget* widget = nullptr) const override;
private:
    void drawScroller(const QStyleOption* option, QPainter* painter) const;

    void drawMenuItem(const QStyleOptionMenuItem* menuItem,
                      QPainter* painter) const;

    void drawSeparator(const QStyleOptionMenuItem* menuItem,
                       QPainter* painter) const;

    void drawMenuItemIcon(const QStyleOptionMenuItem* menuItem,
                          QPainter* painter) const;

    void drawSubMenuItem(const QStyleOptionMenuItem* menuItem, QPainter* painter) const;

    void drawMenuItemCheckedIcon(const QStyleOptionMenuItem* menuItem,
                                 QPainter* painter) const;

    QPixmap getSubMenuPixmap(const QStyle::State state) const;

    QPixmap getIconPixmap(const QStyle::State state, const QIcon &icon, const QSize &size) const;

    QPalette::ColorGroup colorGroup(const QStyle::State state) const;
};

#endif // UPROXYSTYLE_H
