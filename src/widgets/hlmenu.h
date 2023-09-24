#ifndef HLMENU_H
#define HLMENU_H

#include "utkwidget_export.h"

#include <QMenu>

class UProxyStyle;
class UTKWIDGET_EXPORT HLMenu : public QMenu
{
    Q_OBJECT
public:
    explicit HLMenu(QWidget* parent = nullptr);
    ~HLMenu() override = default;

protected:
    bool event(QEvent* event) override;

private:
    QScopedPointer<UProxyStyle> m_style;
};

#endif // HLMENU_H
