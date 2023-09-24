#include "hlmenu.h"
#include "style/uproxystyle.h"

#include <QHelpEvent>
#include <QToolTip>

HLMenu::HLMenu(QWidget* parent) : QMenu(parent)
{
    setMinimumWidth(160);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    if (nullptr == qobject_cast<UProxyStyle*>(this->style()))
    {
        m_style.reset(new UProxyStyle());
        setStyle(m_style.get());
    }
}

bool HLMenu::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ToolTip:
        {
            QHelpEvent* helpEvent = dynamic_cast<QHelpEvent*>(event);
            QAction* action = actionAt(helpEvent->pos());
            if (action && !action->toolTip().isEmpty())
            {
                QToolTip::showText(helpEvent->globalPos(), action->toolTip(), this);
            }
            else
            {
                QToolTip::hideText();
            }
            return true;
        }
        default:
            break;
    }

    return QMenu::event(event);
}
