#ifndef UMESSAGEEVENTHANDLER_H
#define UMESSAGEEVENTHANDLER_H

#include "umessageinterface.h"

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
    UMessageObjectEvent(UIMessageHandler* handler, std::shared_ptr<UMessage> &&msg, Type type = Type(QEvent::User + MessageObjectEventType));
    UIMessageHandler* handlerObject;
};

class UMessageEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit UMessageEventHandler(QObject* parent = nullptr);

protected:
    void customEvent(QEvent* event) override;
};

#endif // UMESSAGEEVENTHANDLER_H
