#ifndef NQEXCEPTIONHANDLER_H
#define NQEXCEPTIONHANDLER_H

namespace google_breakpad
{
    class ExceptionHandler;
}

#include "../utkcore_global.h"

#include <QString>
#include <functional>

class UTKCORE_EXPORT NQExceptionHandler
{
public:
    NQExceptionHandler(const QString &dumpFilePath,
                       std::function<void()> callback = nullptr,
                       const QString &dumpReporterPath = QString(),
                       bool bIsReport = false,
                       const QString &appVersion = "1.0.0",
                       const QString &companyShortName = "NovaStar");

    ~NQExceptionHandler();

    static QString getDumpDirDefault(const QString &appName,
                                     const QString &strCompanyShortName = "NovaStar");
private:
    void init(const QString &dumpFilePath,
              const QString &appVersion,
              const QString &dumpReporterPath,
              bool bIsReport,
              const QString &companyShortName,
              std::function<void()> callback);
private:
    google_breakpad::ExceptionHandler* m_exceptionHandler = nullptr;

};
#endif // NQEXCEPTIONHANDLER_H



