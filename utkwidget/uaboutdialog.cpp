#include "uaboutdialog.h"

#include <QIcon>
#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

class UAboutDialogPrivate
{
public:
    UAboutDialogPrivate(UAboutDialog* q) : q_ptr(q) {}
    ~UAboutDialogPrivate();

    void init();

private:
    Q_DECLARE_PUBLIC(UAboutDialog)
    Q_DISABLE_COPY(UAboutDialogPrivate)
    UAboutDialog* const q_ptr = nullptr;
    QLabel* iconLabel = nullptr;
    QLabel* nameLabel = nullptr;
    QLabel* versionLabel = nullptr;
    QLabel* describeLabel = nullptr;
    QLabel* linkLabel = nullptr;
};

UAboutDialogPrivate::~UAboutDialogPrivate()
{
}

void UAboutDialogPrivate::init()
{
    Q_Q(UAboutDialog);
    QWidget* widget = new QWidget(q);
    iconLabel = new QLabel(q);
    iconLabel->setObjectName("AboutDialog_icon");
    nameLabel = new QLabel(q);
    nameLabel->setObjectName("AboutDialog_name");
    QFont font = q->font();
    font.setPixelSize(14);
    font.setWeight(QFont::Bold);
    nameLabel->setFont(font);
    versionLabel = new QLabel(q);
    versionLabel->setObjectName("AboutDialog_version");
    describeLabel = new QLabel(q);
    describeLabel->setObjectName("AboutDialog_describe");
    describeLabel->setWordWrap(true);
    linkLabel = new QLabel(q);
    linkLabel->setObjectName("AboutDialog_link");
    linkLabel->setOpenExternalLinks(true);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setContentsMargins(9, 9, 9, 19);
    layout->addWidget(iconLabel, 0, Qt::AlignHCenter);
    layout->addWidget(nameLabel, 0, Qt::AlignHCenter);
    layout->addWidget(versionLabel, 0, Qt::AlignHCenter);
    layout->addWidget(describeLabel, 0, Qt::AlignHCenter);
    layout->addWidget(linkLabel, 0, Qt::AlignHCenter);
    widget->setLayout(layout);
    q->setContentWidget(widget);
}

UAboutDialog::UAboutDialog(QWidget* parent) : UDialog(parent)
    , d_ptr(new UAboutDialogPrivate(this))
{
    Q_D(UAboutDialog);
    d->init();
}

UAboutDialog::~UAboutDialog()
{
}

void UAboutDialog::setAppIcon(const QIcon &icon)
{
    Q_D(UAboutDialog);

    d->iconLabel->setPixmap(icon.pixmap(68, 68));
}

void UAboutDialog::setAppName(const QString &name)
{
    Q_D(UAboutDialog);

    d->nameLabel->setText(name);
}

QString UAboutDialog::appName()
{
    Q_D(UAboutDialog);

    return d->nameLabel->text();
}

void UAboutDialog::setAppVersion(const QString &version)
{
    Q_D(UAboutDialog);

    d->versionLabel->setText(version);
}

QString UAboutDialog::appVersion()
{
    Q_D(UAboutDialog);

    return d->versionLabel->text();
}

void UAboutDialog::setAppDescribe(const QString &describe)
{
    Q_D(UAboutDialog);

    d->describeLabel->setText(describe);
}

QString UAboutDialog::appDescribe()
{
    Q_D(UAboutDialog);

    return d->describeLabel->text();
}

void UAboutDialog::setAppHomePage(const QString &link)
{
    Q_D(UAboutDialog);

    d->linkLabel->setProperty("link", link);
    d->linkLabel->setText(QString("<a style='color: #2F54EB; text-decoration: none' href = %1>%2").arg(link).arg(link));
}

QString UAboutDialog::appHomePage()
{
    Q_D(UAboutDialog);

    return d->linkLabel->property("link").toString();
}
