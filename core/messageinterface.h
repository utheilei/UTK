#ifndef MESSAGEINTERFACE_H
#define MESSAGEINTERFACE_H

#include "utkcore_export.h"
#include "message.h"

#include <functional>
#include <memory>

class UTKCORE_EXPORT MessageInterface
{
public:
    MessageInterface() {};
    virtual ~MessageInterface() {};

    virtual void registerEvent(int msgId, std::function<std::shared_ptr<MessageResult>(std::shared_ptr<Message> msg)> &&func) = 0;
    virtual int unregisterEvent(int msgId) = 0;
    virtual int sendMessage(int msgId, std::shared_ptr<Message> req, std::shared_ptr<MessageResult> res) = 0;
    virtual int postMessage(int msgId, std::shared_ptr<Message> req) = 0;
    virtual void handleMessage(std::shared_ptr<Message> req) = 0;
};

class UTKCORE_EXPORT IMessageHandler
{
public:
    IMessageHandler() {}
    virtual ~IMessageHandler() {}

    virtual std::shared_ptr<MessageResult> handleMessage(std::shared_ptr<Message> msg) = 0;
};

#endif // MESSAGEINTERFACE_H
