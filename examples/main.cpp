#include "mainwindow.h"
#include "uapplication.h"

#include <QIcon>
#include <QDebug>

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

    app.setApplicationTheme(UTheme::LightTheme);
    app.setWindowIcon(QIcon::fromTheme("utkwidget"));
    app.setApplicationName(QObject::tr("UTK"));
    app.setApplicationVersion("1.0");

    qInfo() << "========== Application is start ==========" << Qt::endl;

    MainWindow w;
    w.setMinimumSize(1200, 800);
    app.setActivationWindow(&w);
    w.moveToCenter();
    w.show();

    return app.exec();
}