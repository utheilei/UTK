#ifndef UTHEME_H
#define UTHEME_H

#include "utkwidget_export.h"

#include <QColor>
#include <QObject>

class UPalette;
class UThemePrivate;
class UTKWIDGET_EXPORT UTheme
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

    UPalette* palette() const;

    QString themeName() const;

private:
    QScopedPointer<UThemePrivate> d_ptr;
    Q_DECLARE_PRIVATE(UTheme)
};

#endif // UTHEME_H
