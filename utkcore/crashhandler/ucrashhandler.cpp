#include "ucrashhandler.h"

#include <QDateTime>
#include <QFile>
#include <QProcess>
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDebug>

#if defined(Q_OS_LINUX)
#  include "client/linux/handler/exception_handler.h"
#elif defined(Q_OS_WIN)
#include "client/windows/handler/exception_handler.h"
#elif defined(Q_OS_MACOS)
#  include "client/mac/handler/exception_handler.h"
#endif

static QString   s_companyName;
static QString   s_appVerion;
static QString   s_dumpReporterPath;
static QDateTime s_startTime;
static bool s_bIsReport = false;
static std::function<void()> s_callback = nullptr;

class UCrashHandlerPrivate
{
    Q_DECLARE_PUBLIC(UCrashHandler)
public:
    UCrashHandlerPrivate(UCrashHandler* parent) : q_ptr(parent){};
    ~UCrashHandlerPrivate(){};

    UCrashHandler* q_ptr = nullptr;

    void init(const QString &dumpFilePath,
              const QString &appVersion,
              const QString &dumpReporterPath,
              bool bIsReport,
              const QString &companyShortName,
              std::function<void()> callback);
private:
    google_breakpad::ExceptionHandler* exceptionHandler = nullptr;
};

/**
 * @brief 获取默认的Dump输出目录
 *
 * @param const QString & 应用程序名称
 * @param const QString & 公司名称（简称）
 * @return 返回默认的Dump输出目录
 */
QString UCrashHandler::getDumpDirDefault(const QString &appName,
                                         const QString &strCompanyShortName)
{
    QString fullDumpPath = QDir::cleanPath(QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)));
    QStringList dirNames;
    dirNames = fullDumpPath.split('/');
    if (dirNames.count() <= 0)
        dirNames = fullDumpPath.split('\\');
    Q_ASSERT(dirNames.count() > 0);
    if (!(QCoreApplication::applicationName().isNull() || QCoreApplication::applicationName().isEmpty()))
    {
        dirNames.removeLast();
    }
    if (!(QCoreApplication::organizationName().isNull() || QCoreApplication::organizationName().isEmpty()))
    {
        dirNames.removeLast();
    }
    fullDumpPath = QDir::cleanPath(dirNames.join(QDir::separator()));
    if (!fullDumpPath.endsWith(QDir::separator()))
        fullDumpPath.append(QDir::separator());
    fullDumpPath.append(strCompanyShortName);
    fullDumpPath.append(QDir::separator());
    fullDumpPath.append(appName);
    fullDumpPath.append(QDir::separator());
    fullDumpPath.append("dump");
    QDir dir(fullDumpPath);
    if (!dir.exists(fullDumpPath))
        dir.mkpath(fullDumpPath);
    return fullDumpPath;
}

/**
 * @brief 判断路径是否存在，不存在则创建文件夹
 * @param const QString & 完整路径
 * @return 返回是否存在
 */
bool isDirExist(const QString &fullPath)
{
    if (fullPath.isEmpty())
        return false;

    QDir dir(fullPath);
    if (dir.exists())
    {
        return true;
    }
    else
    {
        bool bOk = dir.mkdir(fullPath);
        return bOk;
    }
}

/**
 * @brief 写入信息到指定文件
 * @param const QString & 文件完整路径
 * @param const QStringList & 写入的内容
 * @return void
 */
void writeApplicationInfo(const QString &savefullpath,
                          const QStringList &arguments)
{
    if (savefullpath.isEmpty())
        return;

    QFile writeFile(savefullpath);
    if (writeFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        for (QString info : arguments)
        {
            writeFile.write(info.toStdString().c_str());
            writeFile.write("\n");
        }
        writeFile.write(QString("StartTime:%1").arg(s_startTime.toString("yyyy-MM-dd HH:mm:ss")).toStdString().c_str());
        writeFile.write("\n");
        writeFile.write(QString("CurrentTime:%1").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")).toStdString().c_str());
        writeFile.write("\n");
        writeFile.close();

        return;
    }
    return;
}

/**
 * @brief google_breakpad回调函数
 * @param
 * @return 返回
 */
#if defined(Q_OS_LINUX)
static bool exceptionHandlerCallback(const google_breakpad::MinidumpDescriptor &descriptor,
                                     void* /*context*/,
                                     bool succeeded)
{
    if (!succeeded)
        return succeeded;

    const QStringList argumentList =
        {
            QString::fromLocal8Bit(descriptor.path()),
            s_companyName,
            QCoreApplication::applicationName(),
            s_appVerion,
            QCoreApplication::applicationFilePath()
        };

    //软件版本相关信息写入文件
    QString appInfoPath = QString::fromLocal8Bit(descriptor.path()).replace(".dmp", ".txt");

    writeApplicationInfo(appInfoPath, argumentList);

    //回调函数关闭log
    if (s_callback)
        s_callback();

    if (s_bIsReport)
    {
        return !QProcess::execute(s_dumpReporterPath, argumentList);
    }

    return succeeded;
}
#elif defined(Q_OS_MACOS)
static bool exceptionHandlerCallback(const char* dump_dir,
                                     const char* minidump_id,
                                     void* context,
                                     bool succeeded)
{
    Q_UNUSED(context);

    if (!succeeded)
        return succeeded;

    const QString dumpPath = QString::fromLocal8Bit(dump_dir) + '/'
                             + QString::fromLocal8Bit(minidump_id) + ".dmp";

    const QStringList argumentList =
        {
            dumpPath,
            s_companyName,
            QCoreApplication::applicationName(),
            s_appVerion,
            QCoreApplication::applicationFilePath()
        };

    //软件版本相关信息写入文件
    QString appInfoPath = QString::fromLocal8Bit(dump_dir) + '/'
                          + QString::fromLocal8Bit(minidump_id) + ".txt";

    writeApplicationInfo(appInfoPath, argumentList);

    //回调函数关闭log
    if (s_callback)
        s_callback();

    if (s_bIsReport)
    {
        return !QProcess::execute(s_dumpReporterPath, argumentList);
    }

    return succeeded;
}
#elif defined(Q_OS_WIN)
static bool exceptionHandlerCallback(const wchar_t* dump_path,
                                     const wchar_t* minidump_id,
                                     void* context,
                                     EXCEPTION_POINTERS* exinfo,
                                     MDRawAssertionInfo* assertion,
                                     bool succeeded)
{
    Q_UNUSED(assertion);
    Q_UNUSED(exinfo);
    Q_UNUSED(context);

    if (!succeeded)
        return succeeded;

    const QString dumpPath = QString::fromWCharArray(dump_path, int(wcslen(dump_path))) + '/'
                             + QString::fromWCharArray(minidump_id, int(wcslen(minidump_id))) + ".dmp";
    const QStringList argumentList =
        {
            dumpPath,
            s_companyName,
            QCoreApplication::applicationName(),
            s_appVerion,
            QCoreApplication::applicationFilePath()
        };

    //软件版本相关信息写入文件
    QString appInfoPath = QString::fromWCharArray(dump_path, int(wcslen(dump_path))) + '/'
                          + QString::fromWCharArray(minidump_id, int(wcslen(minidump_id))) + ".txt";

    writeApplicationInfo(appInfoPath, argumentList);

    //    qDebug()<<argumentList;

    //回调函数关闭log
    if (s_callback)
        s_callback();

    if (s_bIsReport)
    {
        return !QProcess::execute(s_dumpReporterPath, argumentList);
    }

    return succeeded;
}
#endif

/**
 * @brief 构造函数，启动捕获日志进程
 * @param const QString & 需要保存的Dump文件路径
 * @param const QString & 公司名称（简称）
 * @param const QString & 软件版本信息
 * @param const QString & Dump上传exe路径
 * @param bool 是否上传Dump
 * @param NQExceptionHandler::CrashCallback 回调函数（软件崩溃时关闭Log日志）
 * @return 返回默认的Dump输出目录
 */
void UCrashHandlerPrivate::init(const QString &dumpFilePath,
                                const QString &appVersion,
                                const QString &dumpReporterPath,
                                bool bIsReport,
                                const QString &companyShortName,
                                std::function<void()> callback)
{
    s_startTime = QDateTime::currentDateTime();
    s_companyName = companyShortName;
    s_appVerion = appVersion;
    s_dumpReporterPath = dumpReporterPath;
    s_bIsReport = bIsReport;
    s_callback = callback;

    if (!isDirExist(dumpFilePath) || exceptionHandler != nullptr)
    {
        qDebug() << "NQExceptionHandler initialization failed!";
        return;
    }

#if defined(Q_OS_LINUX)
    m_exceptionHandler = new google_breakpad::ExceptionHandler(
        google_breakpad::MinidumpDescriptor(dumpFilePath.toStdString()),
        NULL,
        exceptionHandlerCallback,
        NULL,
        true,
        -1);
#elif defined(Q_OS_MACOS)
    m_exceptionHandler = new google_breakpad::ExceptionHandler(
        dumpFilePath.toStdString(),
        NULL,
        exceptionHandlerCallback,
        NULL,
        true,
        NULL);
#elif defined(Q_OS_WIN)
    exceptionHandler = new google_breakpad::ExceptionHandler(
        dumpFilePath.toStdWString(),
        NULL,
        exceptionHandlerCallback,
        NULL,
        google_breakpad::ExceptionHandler::HANDLER_ALL);
#endif
}

/**
 * @brief 构造函数，启动捕获日志进程
 * @param const QString & 需要保存的Dump文件路径
 * @param const QString & 公司名称（简称）
 * @param const QString & 软件版本信息
 * @param const QString & Dump上传exe路径
 * @param bool 是否上传Dump
 * @param NQExceptionHandler::CrashCallback 回调函数（软件崩溃时关闭Log日志）
 * @return 返回默认的Dump输出目录
 */
UCrashHandler::UCrashHandler(const QString &dumpFilePath,
                             std::function<void()> callback,
                             const QString &dumpReporterPath,
                             bool bIsReport,
                             const QString &appVersion,
                             const QString &companyShortName)
    : d_ptr(new UCrashHandlerPrivate(this))
{
    Q_D(UCrashHandler);

    d->init(dumpFilePath, appVersion, dumpReporterPath, bIsReport, companyShortName, callback);
}

UCrashHandler::~UCrashHandler()
{
    Q_D(UCrashHandler);

    if (d->exceptionHandler)
    {
        delete d->exceptionHandler;
        d->exceptionHandler = nullptr;
    }
}
