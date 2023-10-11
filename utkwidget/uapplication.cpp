#include "uapplication.h"
#include "logger/simplelog.h"
#include "uaboutdialog.h"
#include "style/uproxystyle.h"
#include "uapplicationsettings.h"

#include <QTextCodec>
#include <QTranslator>
#include <QMainWindow>
#include <QFile>
#include <QDebug>
#include <qevent.h>

LOGGER_USE_NAMESPACE

class UApplicationPrivate
{
public:
    UApplicationPrivate(UApplication* q) : q_ptr(q) {}
    ~UApplicationPrivate();

    void init();

    void setApplicationStyle(QStyle* style);

private:
    Q_DECLARE_PUBLIC(UApplication)
    Q_DISABLE_COPY(UApplicationPrivate)
    UApplication* const q_ptr = nullptr;
    QMainWindow* m_mainWindow = nullptr;
    QTranslator translator;
    QString translationPath;
    QLocale::Language languageType = QLocale::Language::Chinese;
    UAboutDialog* dialog = nullptr;
};

UApplicationPrivate::~UApplicationPrivate()
{
}

void UApplicationPrivate::init()
{
    Q_Q(UApplication);

    q->setOrganizationName("utkwidget");
    q->setOrganizationDomain("https://gitee.com/uthelei/utkwidget.git");
    q->setApplicationName("UTK");
}

void UApplicationPrivate::setApplicationStyle(QStyle* style)
{
    Q_Q(UApplication);

    q->setStyle(style);
    q->setPalette(style->standardPalette());
}

UApplication::UApplication(const QString &appId, int &argc, char** argv)
    : QtSingleApplication(appId, argc, argv)
    , d_ptr(new UApplicationPrivate(this))
{
    Q_D(UApplication);
    d->init();
}

UApplication::~UApplication()
{
}

UTheme::ThemeType UApplication::applicationTheme()
{
    auto ustyle = qobject_cast<UProxyStyle*>(style());
    return ustyle ? ustyle->theme() : UTheme::ThemeType::LightTheme;
}

void UApplication::setApplicationTheme(const UTheme::ThemeType &type)
{
    Q_D(UApplication);

    QString searchPath = QLatin1String(":/icons");
    QIcon::setThemeSearchPaths(QStringList() << searchPath);
    QIcon::setFallbackSearchPaths(QStringList() << searchPath);
    UProxyStyle *style = qobject_cast<UProxyStyle*>(uApp->style());
    if (nullptr == style) {
        style = new UProxyStyle();
        connect(style, &UProxyStyle::themeChanged, this, [](const UTheme::ThemeType &type) {
            UApplicationSettings::instance()->setApplicationTheme(type);
        });
    }
    style->setTheme(type);
    d->setApplicationStyle(style);
}

void UApplication::initApplicationLog(const QString &fileName)
{
    LogManager::setlogFilePath(fileName);
    LogManager::setLogFormat("%{time}{yyyy-MM-dd, HH:mm:ss.zzz} "
                             "[%{type:-7}] [%{file:-20} %{line}] %{message}\n");
    LogManager::registerFileAppender();
}

void UApplication::setLogLevel(simplelog::Logger::LogLevel level)
{
    LogManager::setLogLevel(level);
}

void UApplication::loadFont(const QFont &font)
{
    setFont(font);
}

QLocale::Language UApplication::currentLanguage() const
{
    Q_D(const UApplication);
    return d->languageType;
}

void UApplication::loadTranslator(const QString &path, const QLocale::Language &language)
{
    Q_D(UApplication);

    d->translationPath = path;
    d->languageType = language;
    qInfo() << "translation filePath:" << d->translationPath << "Language:" << d->languageType;
    QString translatorFileName = "";

    if (QLocale::Language::English == language)
    {
        translatorFileName = QString("%1_en.qm").arg(path);
    }
    else if (QLocale::Language::Chinese == language)
    {
        translatorFileName = QString("%1_zh_CN.qm").arg(path);
    }

    QFile file(translatorFileName);

    if (!file.exists())
    {
        qWarning() << "fileName not exists!!!";
        return;
    }

    if (d->translator.load(translatorFileName))
    {
        QApplication::installTranslator(&d->translator);
    }
    else
    {
        qWarning() << "translator load failed!!!";
    }
}

void UApplication::changeTranslator(const QLocale::Language &language)
{
    Q_D(UApplication);

    loadTranslator(d->translationPath, language);
    UApplicationSettings::instance()->setApplicationLanguage(language);
}

UPalette* UApplication::applicationPalette()
{
    auto ustyle = qobject_cast<UProxyStyle*>(style());
    return ustyle ? ustyle->palette() : nullptr;
}

void UApplication::setAboutDialog(UAboutDialog* dialog)
{
    Q_D(UApplication);

    if (d->dialog && d->dialog != dialog)
    {
        d->dialog->deleteLater();
    }

    d->dialog = dialog;
}

UAboutDialog* UApplication::aboutDialog()
{
    Q_D(UApplication);

    return d->dialog;
}

bool UApplication::notify(QObject* receiver, QEvent* event)
{
    auto objMeta = receiver->metaObject();
    switch (event->type())
    {
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::MouseMove:
        {
            auto mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent && ("QWidgetWindow" == QString(objMeta->className())))
                emit global_mouseEvent(mouseEvent);
            break;
        }
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
        {
            auto keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent && ("QWidgetWindow" == QString(objMeta->className())))
                emit global_keyEvent(keyEvent);
            break;
        }
        default:
            break;
    }

    return QtSingleApplication::notify(receiver, event);
}
