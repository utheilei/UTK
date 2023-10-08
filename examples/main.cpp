#include "mainwindow.h"
#include "uapplication.h"
#include "uapplicationsettings.h"
#include "crashhandler/ucrashhandler.h"

#include <QIcon>
#include <QDebug>
#include <QStandardPaths>

int main(int argc, char* argv[])
{
    UApplication app("UTK", argc, argv);

    if (app.isRunning())
    {
        return !app.sendMessage("Application is Running");
    }

#ifndef BUILD_TYPE_DEBUG
    QString name = app.applicationDirPath() + QString("/%1.log").arg(app.applicationName());
    app.initApplicationLog(name);
    app.setLogLevel(simplelog::Logger::LogLevel::Info);
#endif

    QFont font = app.font();
    font.setPixelSize(12);
    font.setWeight(QFont::Normal);
    font.setFamily("Microsoft YaHei");
    app.loadFont(font);

    app.setApplicationTheme(UTheme::ThemeType(UApplicationSettings::instance()->applicationTheme()));
    app.setWindowIcon(QIcon::fromTheme("utkwidget"));
    app.setApplicationName(QObject::tr("UTK"));
    app.setApplicationVersion("1.0");

    app.loadTranslator(QString("%1/translations/%2").arg(app.applicationDirPath()).arg("example"),
                       QLocale::Language(UApplicationSettings::instance()->applicationLanguage()));

    qInfo() << "========== Application is start ==========" << Qt::endl;

    const QString crashReporterPath = QString("%1/dump").arg(app.applicationDirPath());
    UCrashHandler hander(crashReporterPath, []()
    {
        qCritical() << "========== Application is crash ==========";
    });

    MainWindow w;
    w.setMinimumSize(1200, 800);
    app.setActivationWindow(&w);
    w.moveToCenter();
    w.show();

    return app.exec();
}
