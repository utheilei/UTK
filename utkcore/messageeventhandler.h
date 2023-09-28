#ifndef MESSAGEEVENTHANDLER_H
#define MESSAGEEVENTHANDLER_H

#include "messageinterface.h"

#include <QEvent>
#include <QObject>

#define MessageEventType 1
#define MessageObjectEventType 2

class UMessageEvent: public QEvent
{
public:
    explicit UMessageEvent(std::shared_ptr<UMessage> &&msg, Type type = Type(QEvent::User + MessageEventType));
    std::shared_ptr<UMessage> eventMsg;
};

class UMessageObjectEvent: public UMessageEvent
{
public:
    UMessageObjectEvent(IMessageHandler* handler, std::shared_ptr<UMessage> &&msg, Type type = Type(QEvent::User + MessageObjectEventType));
    IMessageHandler* handlerObject;
};

class UMessageEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit UMessageEventHandler(QObject* parent = nullptr);

protected:
    void customEvent(QEvent* event) override;
};

#endif // MESSAGEEVENTHANDLER_H
