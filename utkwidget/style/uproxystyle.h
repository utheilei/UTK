#ifndef UPROXYSTYLE_H
#define UPROXYSTYLE_H

#include "utkwidget_export.h"
#include "utheme.h"

#include <QProxyStyle>

class QStyleOptionMenuItem;
class QStyleOptionViewItem;
class UTKWIDGET_EXPORT UProxyStyle : public QProxyStyle
{
    Q_OBJECT

public:
    enum UPrimitiveElement
    {
        PE_ItemBackground = QStyle::PE_CustomBase + 100
    };
    Q_ENUM(UPrimitiveElement)

    enum UControlElement
    {
        CE_UIconButton = QStyle::CE_CustomBase + 100
    };
    Q_ENUM(UControlElement)

    enum UPixelMetric
    {
        PM_FocusBorderWidth = QStyle::PM_CustomBase + 100,        //控件焦点状态的边框宽度
        PM_FocusBorderSpacing,                                  //控件内容和border之间的间隔
        PM_FrameRadius,                                         //控件的圆角大小
        PM_ShadowRadius,                                        //控件阴影效果的半径
        PM_FrameMargins                                         //控件的margins区域，控件内容 = 控件大小 - FrameMargins
    };
    Q_ENUM(UPixelMetric)

    enum UComplexControl
    {
        CC_UScrollBar = QStyle::CC_CustomBase + 100
    };
    Q_ENUM(UComplexControl)


    UProxyStyle();
    ~UProxyStyle() override;

    UTheme::ThemeType theme() const;
    void setTheme(const UTheme::ThemeType &type);

    QPalette standardPalette() const override;

    UPalette* palette() const;

    int styleHint(StyleHint hint, const QStyleOption* option = nullptr,
                  const QWidget* widget = nullptr, QStyleHintReturn* returnData = nullptr) const override;

    int pixelMetric(PixelMetric metric, const QStyleOption* option = nullptr,
                    const QWidget* widget = nullptr) const override;

    int pixelMetric(UPixelMetric metric, const QStyleOption* option = nullptr,
                    const QWidget* widget = nullptr) const;

    void drawControl(ControlElement element, const QStyleOption* option,
                     QPainter* painter, const QWidget* widget) const override;

    void drawControl(UControlElement element, const QStyleOption* option,
                     QPainter* painter, const QWidget* widget) const;

    void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
                       QPainter* painter, const QWidget* widget = nullptr) const override;

    void drawPrimitive(UPrimitiveElement element, const QStyleOption* option,
                       QPainter* painter, const QWidget* widget = nullptr) const;

    void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option,
                            QPainter* painter, const QWidget* widget = nullptr) const override;

    void drawComplexControl(UComplexControl control, const QStyleOptionComplex* option,
                            QPainter* painter, const QWidget* widget = nullptr) const;

    static QPalette::ColorGroup colorGroup(const QStyleOption* option, const QWidget* widget = nullptr);

    static QPainterPath backgroundPath(const QStyleOptionViewItem &option, int radius);

signals:
    void themeChanged(const UTheme::ThemeType &type);

private:
    void drawMenuItem(const QStyleOptionMenuItem* menuItem,
                      QPainter* painter) const;

    void drawSeparator(const QStyleOptionMenuItem* menuItem,
                       QPainter* painter) const;

    void drawMenuItemIcon(const QStyleOptionMenuItem* menuItem,
                          QPainter* painter) const;

    void drawSubMenuItem(const QStyleOptionMenuItem* menuItem, QPainter* painter) const;

    QPixmap getSubMenuPixmap(const QStyle::State state) const;

    QPixmap getIconPixmap(const QStyle::State state, const QIcon &icon, const QSize &size) const;

    QPalette::ColorGroup colorGroup(const QStyle::State state) const;

    UTheme m_theme;
};

#endif // UPROXYSTYLE_H
