#ifndef UAPPLICATION_H
#define UAPPLICATION_H

#include "singleapplication/qtsingleapplication.h"
#include "logger/logger.h"

#define uApp (static_cast<UApplication *>(QApplication::instance()))

class UPalette;
class UApplicationPrivate;
class QMouseEvent;
class QKeyEvent;
class UAboutDialog;
class UApplication : public QtSingleApplication
{
    Q_OBJECT

public:
    UApplication(const QString &appId, int &argc, char** argv);
    ~UApplication() override;

    void initApplicationLog(const QString &fileName);

    void setLogLevel(simplelog::Logger::LogLevel level);

    void loadFont(const QFont &font);

    void loadTranslator(const QString &path, const QLocale::Language &language = QLocale::system().language());

    void changeTranslator(const QLocale::Language &language);

    UPalette* appPalette();

    void setAboutDialog(UAboutDialog* dialog);
    UAboutDialog* aboutDialog();

signals:
    void global_mouseEvent(QMouseEvent* event);
    void global_keyEvent(QKeyEvent* event);

protected:
    bool notify(QObject*, QEvent*) Q_DECL_OVERRIDE;

private:
    QScopedPointer<UApplicationPrivate> d_ptr;
    Q_DISABLE_COPY(UApplication)
    Q_DECLARE_PRIVATE_D(d_ptr, UApplication)
};

#endif // UAPPLICATION_H
