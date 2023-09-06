#ifndef UABOUTDIALOG_H
#define UABOUTDIALOG_H

#include "udialog.h"

class UAboutDialogPrivate;
class UAboutDialog : public UDialog
{
    Q_OBJECT

public:
    UAboutDialog(QWidget* parent = nullptr);
    ~UAboutDialog();

    void setAppIcon(const QIcon &icon);

    void setAppName(const QString &name);
    QString appName();

    void setAppVersion(const QString &version);
    QString appVersion();

    void setAppDescribe(const QString &describe);
    QString appDescribe();

    void setAppHomePage(const QString &link);
    QString appHomePage();

private:
    QScopedPointer<UAboutDialogPrivate> d_ptr;
    Q_DISABLE_COPY(UAboutDialog)
    Q_DECLARE_PRIVATE_D(d_ptr, UAboutDialog)
};

#endif // UABOUTDIALOG_H
