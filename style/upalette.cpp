#include "upalette.h"

struct DPaletteData : public QSharedData
{
    QBrush br[UPalette::NColorGroups][UPalette::NColorTypes];
};

class UPalettePrivate
{
    Q_DECLARE_PUBLIC(UPalette)
public:
    UPalettePrivate(UPalette* parent) : q_ptr(parent)
    {
    }

    QSharedDataPointer<DPaletteData> data;
    UPalette* q_ptr = nullptr;
};

UPalette::UPalette() : QPalette(), d_ptr(new UPalettePrivate(this))
{
    initPaletteData();
}

UPalette::UPalette(const QPalette &palette)
    : QPalette(palette), d_ptr(new UPalettePrivate(this))
{
    initPaletteData();
}

UPalette::UPalette(const UPalette &palette)
    : QPalette(palette), d_ptr(new UPalettePrivate(this))
{
    initPaletteData();
}

UPalette::~UPalette()
{
}

void UPalette::initPaletteData()
{
    Q_D(UPalette);
    d->data = QSharedDataPointer<DPaletteData>(new DPaletteData());
    setBrush(UPalette::ItemBackground, QColor(0, 0, 0, 0.03 * 255));
    setBrush(UPalette::NormalBackground, QColor("#19DA2F"));
    setBrush(UPalette::WarningBackground, QColor("#D89614"));
    setBrush(UPalette::ErrorBackground, QColor("#FF5736"));
    setBrush(UPalette::OrangeBackground, QColor("#C45E00"));
    setBrush(UPalette::OfflineBackground, QColor("#2F54EB"));
    setBrush(UPalette::InformationBackground, QColor("#177DDC"));
    setBrush(UPalette::MenuBackground, QColor("#FFFFFF"));
    setBrush(UPalette::ModuleBackground, QColor("#17171A"));
    setBrush(UPalette::SplitLineBackground, QColor("#50505C"));
}

UPalette &UPalette::operator=(const UPalette &palette)
{
    Q_D(UPalette);
    QPalette::operator=(palette);
    d->data = palette.d_func()->data;

    return *this;
}

const QBrush &UPalette::brush(QPalette::ColorGroup cg, UPalette::ColorType cr) const
{
    Q_D(const UPalette);
    if (cr >= NColorTypes)
    {
        return QPalette::brush(cg, QPalette::NoRole);
    }

    if (cg == Current)
    {
        cg = currentColorGroup();
    }
    else if (cg >= NColorGroups)
    {
        cg = Active;
    }

    return d->data->br[cg][cr];
}

void UPalette::setBrush(QPalette::ColorGroup cg, UPalette::ColorType cr, const QBrush &brush)
{
    Q_D(UPalette);
    if (cg == All)
    {
        for (uint i = 0; i < NColorGroups; i++)
        {
            setBrush(ColorGroup(i), cr, brush);
        }
        return;
    }

    if (cr >= NColorTypes)
    {
        return QPalette::setBrush(cg, QPalette::NoRole, brush);
    }

    if (cg == Current)
    {
        cg = currentColorGroup();
    }
    else if (cg >= NColorGroups)
    {
        cg = Active;
    }

    QBrush bru = brush;
    if (ColorGroup::Disabled == cg)
    {
        auto color = brush.color();
        color.setAlphaF(0.3);
        bru = color;
    }

    d->data->br[cg][cr] = bru;
}

QDataStream &operator<<(QDataStream &ds, const UPalette &p)
{
    ds << static_cast<const QPalette &>(p);

    for (int i = 0; i < UPalette::NColorGroups; ++i)
    {
        for (int j = 0; j < UPalette::NColorTypes; ++j)
        {
            ds << p.brush(UPalette::ColorGroup(i), UPalette::ColorType(j));
        }
    }

    return ds;
}

QDataStream &operator>>(QDataStream &ds, UPalette &p)
{
    ds >> static_cast<QPalette &>(p);

    for (int i = 0; i < UPalette::NColorGroups; ++i)
    {
        for (int j = 0; j < UPalette::NColorTypes; ++j)
        {
            QBrush brush;
            ds >> brush;

            p.setBrush(UPalette::ColorGroup(i), UPalette::ColorType(j), brush);
        }
    }

    return ds;
}

QDebug operator<<(QDebug dbg, const UPalette &p)
{
    const char* colorGroupNames[] = {"Active", "Disabled", "Inactive", "NColorGroups", "Current", "All", "Normal"};
    const char* colorTypeNames[] =
    {
        "NoType",
        "ItemBackground",
        "AlternatingRow",
        "NormalBackground",
        "WarningBackground",
        "ErrorBackground",
        "OrangeBackground",
        "OfflineBackground",
        "InformationBackground",
        "MenuBackground",
        "ModuleBackground",
        "SplitLineBackground",
        "NColorTypes"
    };

    QDebugStateSaver saver(dbg);
    dbg << "\r\nDPalette: \r\n";
    for (int i = 0; i < UPalette::NColorGroups; ++i)
    {
        for (int j = UPalette::NoType + 1; j < UPalette::NColorTypes; ++j)
        {
            dbg << colorGroupNames[i] << colorTypeNames[j];
            dbg << p.brush(UPalette::ColorGroup(i), UPalette::ColorType(j)) << "\r\n";
        }
    }
    return dbg;
}
