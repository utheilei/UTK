#include "messageeventhandler.h"
#include "messagedispatcher.h"

UMessageEvent::UMessageEvent(std::shared_ptr<UMessage> &&msg, Type type)
    : QEvent(type), eventMsg(std::move(msg))
{
}

UMessageEventHandler::UMessageEventHandler(QObject* parent) : QObject(parent)
{
}

UMessageObjectEvent::UMessageObjectEvent(IMessageHandler* handler, std::shared_ptr<UMessage> &&msg, Type type)
    : UMessageEvent(std::move(msg), type), handlerObject(handler)
{
}

void UMessageEventHandler::customEvent(QEvent* event)
{
    if (event->type() == (QEvent::Type)(QEvent::User + MessageEventType))
    {
        if (auto messageEvent = static_cast<UMessageEvent*>(event))
        {
            UMessageDispatcher::instance()->handleMessage(messageEvent->eventMsg);
        }
        event->accept();
    }
    else if (event->type() == (QEvent::Type)(QEvent::User + MessageObjectEventType))
    {
        if (auto objectEvent = static_cast<UMessageObjectEvent*>(event))
        {
            objectEvent->handlerObject->handleMessage(objectEvent->eventMsg);
        }
        event->accept();
    }
    QObject::customEvent(event);
}
