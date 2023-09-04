#include "mainwindow.h"
#include "singleapplication/qtsingleapplication.h"

#include <QIcon>
#include <QDebug>

int main(int argc, char* argv[])
{
    QtSingleApplication a(argc, argv);

    if (a.isRunning())
    {
        return !a.sendMessage("Application is Running");
    }

    a.setWindowIcon(QIcon(":/images/utkwidget.svg"));
    a.setApplicationName(QObject::tr("UTK"));
    a.setApplicationVersion("1.0");

    MainWindow w;
    w.setMinimumSize(1200, 800);
    a.setActivationWindow(&w);
    w.moveToCenter();
    w.show();

    QObject::connect(&a, &QtSingleApplication::messageReceived, &w, [&a](const QString & message)
    {
        qDebug() << "message:" << message;
        a.activationWindow();
    });

    return a.exec();
}
