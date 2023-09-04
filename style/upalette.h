#ifndef UPALETTE_H
#define UPALETTE_H

#include <QDebug>
#include <QPalette>

class UPalettePrivate;
class UPalette : public QPalette
{
    Q_GADGET
public:
    enum ColorType
    {
        NoType,
        HighlightBackground,           //高亮背景色（蓝色）
        HighlightHoverBackground,      //高亮背Hover景色（蓝色）
        HighlightPressBackground,      //高亮背Press景色（蓝色）
        ItemBackground,                //列表项的背景色
        AlternatingRow,                //交替列表项的背景色
        LabelText,                     //Label文本的颜色（0.86透明度白色）
        HighlightTextForeground,       //高亮文字（蓝色）
        NormalBackground,              //成功色
        WarningBackground,             //告警色
        ErrorBackground,               //错误色
        OrangeBackground,              //橙色
        OfflineBackground,             //离线状态颜色（灰色）
        MixedBackground,               //Mix态颜色
        InformationBackground,         //信息色
        MenuBackground,                //菜单栏色
        ModuleBackground,              //模块背景色
        SplitLineBackground,           //分割线颜色
        NColorTypes
    };
    Q_ENUM(ColorType)

    UPalette();
    UPalette(const QPalette &palette);
    UPalette(const UPalette &palette);
    ~UPalette();

    UPalette &operator=(const UPalette &palette);

    inline const QColor &color(ColorGroup cg, ColorType ct) const
    {
        return brush(cg, ct).color();
    }
    const QBrush &brush(ColorGroup cg, ColorType ct) const;
    inline void setColor(ColorGroup cg, ColorType ct, const QColor &color)
    {
        setBrush(cg, ct, color);
    }
    inline void setColor(ColorType ct, const QColor &color)
    {
        setColor(All, ct, color);
    }
    inline void setBrush(ColorType ct, const QBrush &brush)
    {
        setBrush(All, ct, brush);
    }
    void setBrush(ColorGroup cg, ColorType ct, const QBrush &brush);

    inline const QColor &color(ColorType ct) const
    {
        return color(Current, ct);
    }
    using QPalette::color;
    using QPalette::brush;
    using QPalette::setBrush;
    using QPalette::setColor;

private:
    void initPaletteData();

    QScopedPointer<UPalettePrivate> d_ptr;
    Q_DECLARE_PRIVATE(UPalette)
};

QDataStream &operator<<(QDataStream &s, const UPalette &p);
QDataStream &operator>>(QDataStream &ds, UPalette &p);
QDebug operator<<(QDebug, const UPalette &);

#endif // UPALETTE_H
