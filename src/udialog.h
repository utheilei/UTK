#ifndef UDIALOG_H
#define UDIALOG_H

#include "utkwidget_export.h"

#include <QDialog>

class TitleBar;
class QVBoxLayout;
class UTKWIDGET_EXPORT UDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    void setTitleIcon(const QIcon &icon);

    void setMessageBoxTitle(const QString &title);

    void setContentWidget(QWidget* widget);

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE;

private:
    TitleBar* titleBar = Q_NULLPTR;
    QVBoxLayout* m_mainLayout = Q_NULLPTR;
    int margin = 8;
};

#endif // UDIALOG_H
