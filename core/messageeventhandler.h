#ifndef MESSAGEEVENTHANDLER_H
#define MESSAGEEVENTHANDLER_H

#include "message.h"
#include "messageinterface.h"

#include <memory>

#include <QEvent>
#include <QObject>

#define MessageEventType 1
#define MessageObjectEventType 2

class MessageEvent: public QEvent
{
public:
    MessageEvent(std::shared_ptr<Message> msg, Type type = Type(QEvent::User + MessageEventType));
    std::shared_ptr<Message> eventMsg;
};

class MessageObjectEvent: public MessageEvent
{
public:
    MessageObjectEvent(IMessageHandler* handler, std::shared_ptr<Message> msg, Type type = Type(QEvent::User + MessageObjectEventType));
    IMessageHandler* handlerObject;
};

class MessageEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit MessageEventHandler(QObject* parent = nullptr);

protected:
    void customEvent(QEvent* event) override;
};

#endif // MESSAGEEVENTHANDLER_H
