#ifndef HLMENU_H
#define HLMENU_H

#include <QMenu>

class UProxyStyle;
class HLMenu : public QMenu
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
