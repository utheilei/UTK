#include "utheme.h"
#include "upalette.h"

class UThemePrivate
{
    Q_DECLARE_PUBLIC(UTheme)
public:
    UThemePrivate(UTheme* parent)
        : q_ptr(parent), palette(new UPalette())
    {
        themeMap.insert(UTheme::ThemeType::LightTheme, "light");
        themeMap.insert(UTheme::ThemeType::DarkTheme, "dark");
    }

    UTheme* q_ptr = nullptr;
private:
    QScopedPointer<UPalette> palette;
    UTheme::ThemeType type = UTheme::ThemeType::LightTheme;
    QMap<UTheme::ThemeType, QString> themeMap;
};

UTheme::UTheme() : d_ptr(new UThemePrivate(this))
{
}

UTheme::~UTheme()
{
}

UTheme::ThemeType UTheme::theme() const
{
    Q_D(const UTheme);
    return d->type;
}

void UTheme::setTheme(const ThemeType &type)
{
    Q_D(UTheme);
    if (d->type == type)
    {
        return;
    }

    qInfo() << "setTheme" << type;
    d->type = type;
    switch (d->type)
    {
        case ThemeType::LightTheme:
            d->palette->setBrush(QPalette::WindowText, QColor("#414D68"));
            d->palette->setBrush(QPalette::Button, QColor("#E5E5E5"));
            d->palette->setBrush(QPalette::Light, QColor("#E6E6E6"));
            d->palette->setBrush(QPalette::Midlight, QColor("#E5E5E5"));
            d->palette->setBrush(QPalette::Dark, QColor("#E3E3E3"));
            d->palette->setBrush(QPalette::Mid, QColor("#E4E4E4"));
            d->palette->setBrush(QPalette::Text, QColor("#414D68"));
            d->palette->setBrush(QPalette::BrightText, QColor("#414D68"));
            d->palette->setBrush(QPalette::ButtonText, QColor("#414D68"));
            d->palette->setBrush(QPalette::Base, QColor("#FFFFFF"));
            d->palette->setBrush(QPalette::Window, QColor("#F8F8F8"));
            d->palette->setBrush(QPalette::Shadow, QColor(92, 93, 95, 50));
            d->palette->setBrush(QPalette::Highlight, QColor("#0081FF"));
            d->palette->setBrush(QPalette::HighlightedText, QColor("#FFFFFF"));
            d->palette->setBrush(QPalette::Link, QColor("#0082FA"));
            d->palette->setBrush(QPalette::LinkVisited, QColor("#AD4579"));
            d->palette->setBrush(QPalette::AlternateBase, QColor(0, 0, 0, 0.03 * 255));
            d->palette->setBrush(QPalette::ToolTipBase, QColor(255, 255, 255, 0.8 * 255));
            d->palette->setBrush(QPalette::ToolTipText, QColor("#000000"));
            d->palette->setBrush(QPalette::PlaceholderText, QColor(85, 85, 85, 0.4 * 255));

            d->palette->setBrush(UPalette::ItemBackground, QColor(0, 0, 0, 0.03 * 255));
            d->palette->setBrush(UPalette::NormalBackground, QColor("#19DA2F"));
            d->palette->setBrush(UPalette::WarningBackground, QColor("#D89614"));
            d->palette->setBrush(UPalette::ErrorBackground, QColor("#FF5736"));
            d->palette->setBrush(UPalette::OrangeBackground, QColor("#C45E00"));
            d->palette->setBrush(UPalette::OfflineBackground, QColor("#2F54EB"));
            d->palette->setBrush(UPalette::InformationBackground, QColor("#177DDC"));
            d->palette->setBrush(UPalette::MenuBackground, QColor("#FFFFFF"));
            d->palette->setBrush(UPalette::ModuleBackground, QColor("#17171A"));
            d->palette->setBrush(UPalette::SplitLineBackground, QColor("#50505C"));
            break;
        case ThemeType::DarkTheme:
            d->palette->setBrush(QPalette::WindowText, QColor("#C0C6D4"));
            d->palette->setBrush(QPalette::Button, QColor("#444444"));
            d->palette->setBrush(QPalette::Light, QColor("#484848"));
            d->palette->setBrush(QPalette::Midlight, QColor("#474747"));
            d->palette->setBrush(QPalette::Dark, QColor("#414141"));
            d->palette->setBrush(QPalette::Mid, QColor("#434343"));
            d->palette->setBrush(QPalette::Text, QColor("#C0C6D4"));
            d->palette->setBrush(QPalette::BrightText, QColor("#C0C6D4"));
            d->palette->setBrush(QPalette::ButtonText, QColor("#C0C6D4"));
            d->palette->setBrush(QPalette::Base, QColor("#282828"));
            d->palette->setBrush(QPalette::Window, QColor("#252525"));
            d->palette->setBrush(QPalette::Shadow, QColor(19, 17, 17));
            d->palette->setBrush(QPalette::Highlight, QColor("#0081FF"));
            d->palette->setBrush(QPalette::HighlightedText, QColor("#F1F6FF"));
            d->palette->setBrush(QPalette::Link, QColor("#0082FA"));
            d->palette->setBrush(QPalette::LinkVisited, QColor("#AD4579"));
            d->palette->setBrush(QPalette::AlternateBase, QColor(0, 0, 0, 0.2 * 255));
            d->palette->setBrush(QPalette::ToolTipBase, QColor(45, 45, 45, 0.8 * 255));
            d->palette->setBrush(QPalette::ToolTipText, QColor("#C0C6D4"));
            d->palette->setBrush(QPalette::PlaceholderText, QColor(192, 198, 212, 0.4 * 255));

            d->palette->setBrush(UPalette::ItemBackground, QColor(255, 255, 255, 0.05 * 255));
            d->palette->setBrush(UPalette::NormalBackground, QColor("#19DA2F"));
            d->palette->setBrush(UPalette::WarningBackground, QColor("#D89614"));
            d->palette->setBrush(UPalette::ErrorBackground, QColor("#9A2F2F"));
            d->palette->setBrush(UPalette::OrangeBackground, QColor("#C45E00"));
            d->palette->setBrush(UPalette::OfflineBackground, QColor("#2F54EB"));
            d->palette->setBrush(UPalette::InformationBackground, QColor("#177DDC"));
            d->palette->setBrush(UPalette::MenuBackground, QColor("#0F0F12"));
            d->palette->setBrush(UPalette::ModuleBackground, QColor("#17171A"));
            d->palette->setBrush(UPalette::SplitLineBackground, QColor("#50505C"));
            break;
        default:
            break;
    }

}

UPalette* UTheme::palette() const
{
    Q_D(const UTheme);

    return d->palette.get();
}

QString UTheme::themeName() const
{
    Q_D(const UTheme);

    return d->themeMap[theme()];
}

