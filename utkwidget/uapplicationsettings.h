#ifndef UAPPLICATIONSETTINGS_H
#define UAPPLICATIONSETTINGS_H

#include "utkwidget_export.h"

#include <QObject>

class UApplicationSettingsPrivate;
class UTKWIDGET_EXPORT UApplicationSettings : public QObject
{
    Q_OBJECT
public:
    static UApplicationSettings* instance();

    void initSettings();

    void setApplicationLanguage(int language);

    void setApplicationTheme(int theme);

    int applicationLanguage();

    int applicationTheme();

private:
    explicit UApplicationSettings(QObject* parent = nullptr);
    ~UApplicationSettings();

    QScopedPointer<UApplicationSettingsPrivate> d_ptr;
    Q_DISABLE_COPY(UApplicationSettings)
    Q_DECLARE_PRIVATE_D(d_ptr, UApplicationSettings)
};

#endif // UAPPLICATIONSETTINGS_H
