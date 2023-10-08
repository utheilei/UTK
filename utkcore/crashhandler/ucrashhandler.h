#ifndef UCRASHHANDLER_H
#define UCRASHHANDLER_H

#include "../utkcore_global.h"

#include <QScopedPointer>
#include <QString>
#include <functional>

class UCrashHandlerPrivate;
class UTKCORE_EXPORT UCrashHandler
{
public:
    UCrashHandler(const QString &dumpFilePath,
                       std::function<void()> callback = nullptr,
                       const QString &dumpReporterPath = QString(),
                       bool bIsReport = false,
                       const QString &appVersion = "1.0.0",
                       const QString &companyShortName = "UTK");
    ~UCrashHandler();

    static QString getDumpDirDefault(const QString &appName,
                                     const QString &strCompanyShortName = "UTK");
private:
    QScopedPointer<UCrashHandlerPrivate> d_ptr;
    Q_DECLARE_PRIVATE_D(d_ptr, UCrashHandler)
};

#endif // UCRASHHANDLER_H



