#ifndef HLMENU_H
#define HLMENU_H

#include <QMenu>

class PopupMenuStyle;
class HLMenu : public QMenu
{
    Q_OBJECT
public:
    explicit HLMenu(QWidget* parent = nullptr);
    ~HLMenu() override = default;

protected:
    bool event(QEvent* event) override;

private:
    QScopedPointer<PopupMenuStyle> popupMenuStyle;
};

#endif // HLMENU_H
