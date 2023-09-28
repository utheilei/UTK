#ifndef UPALETTE_H
#define UPALETTE_H

#include "utkwidget_export.h"

#include <QDebug>
#include <QPalette>

class UPalettePrivate;
class UTKWIDGET_EXPORT UPalette : public QPalette
{
    Q_GADGET
public:
    enum ColorType
    {
        NoType,
        ItemBackground,                //列表项的背景色
        NormalBackground,              //成功色
        WarningBackground,             //告警色
        ErrorBackground,               //错误色
        OrangeBackground,              //橙色
        OfflineBackground,             //离线状态颜色（灰色）
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
