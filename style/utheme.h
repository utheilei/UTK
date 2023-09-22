#ifndef UTHEME_H
#define UTHEME_H

#include <QColor>
#include <QObject>

class UPalette;
class UThemePrivate;
class UTheme
{
public:
    UTheme();
    ~UTheme();

    enum ThemeType
    {
        LightTheme,
        DarkTheme
    };

    ThemeType theme() const;
    void setTheme(const ThemeType &type);

    UPalette *palette() const;

private:
    QScopedPointer<UThemePrivate> d_ptr;
    Q_DECLARE_PRIVATE(UTheme)
};

#endif // UTHEME_H
