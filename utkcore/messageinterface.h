#ifndef MESSAGEINTERFACE_H
#define MESSAGEINTERFACE_H

#include "message.h"

#include <functional>
#include <memory>

class UTKCORE_EXPORT MessageInterface
{
public:
    MessageInterface() {};
    virtual ~MessageInterface() {};

    virtual void registerEvent(int msgId, std::function<std::shared_ptr<UMessageResult>(std::shared_ptr<UMessage> msg)> &&func) = 0;
    virtual int unregisterEvent(int msgId) = 0;
    virtual int sendMessage(int msgId, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> &&res) = 0;
    virtual int postMessage(int msgId, std::shared_ptr<UMessage> &&req) = 0;
    virtual void handleMessage(std::shared_ptr<UMessage> req) = 0;
};

class UTKCORE_EXPORT IMessageHandler
{
public:
    IMessageHandler() {}
    virtual ~IMessageHandler() {}

    virtual std::shared_ptr<UMessageResult> handleMessage(std::shared_ptr<UMessage> msg) = 0;
};

#endif // MESSAGEINTERFACE_H
