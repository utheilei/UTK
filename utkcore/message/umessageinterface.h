#ifndef UMESSAGEINTERFACE_H
#define UMESSAGEINTERFACE_H

#include "umessage.h"

#include <functional>
#include <memory>

class UTKCORE_EXPORT UMessageInterface
{
public:
    UMessageInterface() {};
    virtual ~UMessageInterface() {};

    virtual void registerEvent(int msgId, std::function<std::shared_ptr<UMessageResult>(std::shared_ptr<UMessage> msg)> &&func) = 0;
    virtual int unregisterEvent(int msgId) = 0;
    virtual int sendMessage(int msgId, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> &&res) = 0;
    virtual int postMessage(int msgId, std::shared_ptr<UMessage> &&req) = 0;
    virtual void handleMessage(std::shared_ptr<UMessage> req) = 0;
};

class UTKCORE_EXPORT UIMessageHandler
{
public:
    UIMessageHandler() {}
    virtual ~UIMessageHandler() {}

    virtual std::shared_ptr<UMessageResult> handleMessage(std::shared_ptr<UMessage> msg) = 0;
};

#endif // UMESSAGEINTERFACE_H
