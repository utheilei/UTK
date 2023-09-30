#ifndef UMESSAGEDISPATCHER_H
#define UMESSAGEDISPATCHER_H

#include "umessageinterface.h"

class UMessageDispatcherPrivate;
class UTKCORE_EXPORT UMessageDispatcher : public UMessageInterface
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

#endif // UMESSAGEDISPATCHER_H
