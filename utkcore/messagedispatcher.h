#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "messageinterface.h"

class UMessageDispatcherPrivate;
class UTKCORE_EXPORT UMessageDispatcher : public MessageInterface
{
public:
    static UMessageDispatcher* instance();

    void registerEvent(int msgId, std::function<std::shared_ptr<UMessageResult>(std::shared_ptr<UMessage> msg)> &&func) override;
    int unregisterEvent(int msgId) override;
    int sendMessage(int msgId, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> &&res) override;
    int postMessage(int msgId, std::shared_ptr<UMessage> &&req) override;
    void handleMessage(std::shared_ptr<UMessage> req) override;

private:
    UMessageDispatcher();
    ~UMessageDispatcher();

    Q_DISABLE_COPY(UMessageDispatcher);
    QScopedPointer<UMessageDispatcherPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UMessageDispatcher)
};

#endif // MESSAGEDISPATCHER_H
