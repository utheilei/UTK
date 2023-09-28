#include "messageeventhandler.h"
#include "messagedispatcher.h"

MessageEvent::MessageEvent(std::shared_ptr<Message> msg, Type type)
    : QEvent(type), eventMsg(std::move(msg))
{
}

MessageEventHandler::MessageEventHandler(QObject* parent) : QObject(parent)
{
}

MessageObjectEvent::MessageObjectEvent(IMessageHandler* handler, std::shared_ptr<Message> msg, Type type)
    : MessageEvent(msg, type), handlerObject(handler)
{
}

void MessageEventHandler::customEvent(QEvent* event)
{
    if (event->type() == (QEvent::Type)(QEvent::User + MessageEventType))
    {
        if (auto messageEvent = static_cast<MessageEvent*>(event))
        {
            MessageDispatcher::instance()->handleMessage(messageEvent->eventMsg);
        }
        event->accept();
    }
    else if (event->type() == (QEvent::Type)(QEvent::User + MessageObjectEventType))
    {
        if (auto objectEvent = static_cast<MessageObjectEvent*>(event))
        {
            objectEvent->handlerObject->handleMessage(objectEvent->eventMsg);
        }
        event->accept();
    }
    QObject::customEvent(event);
}
