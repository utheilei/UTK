#include "uapplicationsettings.h"
#include "uapplication.h"

#include <QSettings>
#include <QDir>
#include <QStandardPaths>

class UApplicationSettingsPrivate
{
public:
    UApplicationSettingsPrivate(UApplicationSettings* q) : q_ptr(q) {}
    ~UApplicationSettingsPrivate() {};

    void init();

    QString mkMutiDir(const QString &path);

private:
    Q_DECLARE_PUBLIC(UApplicationSettings)
    Q_DISABLE_COPY(UApplicationSettingsPrivate)
    UApplicationSettings* const q_ptr = nullptr;
};

void UApplicationSettingsPrivate::init()
{
    QString AppDataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    mkMutiDir(AppDataLocation);
}

QString UApplicationSettingsPrivate::mkMutiDir(const QString &path)
{
    QString tempPath = path;
    tempPath.replace("\\", "/");
    QDir dir(path);
    if (dir.exists(path)) {
        return path;
    }
    QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
    QString dirname = path.mid(path.lastIndexOf('/') + 1);
    QDir parentPath(parentDir);
    if (!dirname.isEmpty())
        parentPath.mkpath(dirname);
    return parentDir + "/" + dirname;
}

UApplicationSettings::UApplicationSettings(QObject* parent) : QObject(parent)
    , d_ptr(new UApplicationSettingsPrivate(this))
{
    Q_D(UApplicationSettings);

    d->init();
    initSettings();
}

UApplicationSettings::~UApplicationSettings()
{
}

UApplicationSettings *UApplicationSettings::instance()
{
    static UApplicationSettings settings;
    return &settings;
}

void UApplicationSettings::initSettings()
{
    QString fileName = QString("%1/%2.ini").
                       arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(uApp->applicationName());

    if (!QFile::exists(fileName))
    {
        QSettings settings(fileName, QSettings::IniFormat);
        settings.setValue("Settings/Theme", UTheme::LightTheme);
        settings.setValue("Settings/Language", QLocale::Chinese);
        settings.sync();
    }
}

void UApplicationSettings::setApplicationLanguage(int language)
{
    QString fileName = QString("%1/%2.ini").
                       arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(uApp->applicationName());

    QSettings settings(fileName, QSettings::IniFormat);
    settings.setValue("Settings/Language", language);
    settings.sync();
}

void UApplicationSettings::setApplicationTheme(int theme)
{
    QString fileName = QString("%1/%2.ini").
                       arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(uApp->applicationName());

    QSettings settings(fileName, QSettings::IniFormat);
    settings.setValue("Settings/Theme", theme);
    settings.sync();
}

int UApplicationSettings::applicationLanguage()
{
    QString fileName = QString("%1/%2.ini").
                       arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(uApp->applicationName());

    QSettings settings(fileName, QSettings::IniFormat);
    return settings.value("Settings/Language", QLocale::Chinese).toInt();
}

int UApplicationSettings::applicationTheme()
{
    QString fileName = QString("%1/%2.ini").
                       arg(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).arg(uApp->applicationName());

    QSettings settings(fileName, QSettings::IniFormat);
    return settings.value("Settings/Theme", UTheme::LightTheme).toInt();
}
