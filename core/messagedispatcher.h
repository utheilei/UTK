#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include "messageinterface.h"

class MessageDispatcherPrivate;
class UTKCORE_EXPORT MessageDispatcher : public MessageInterface
{
public:
    static MessageDispatcher* instance();

    void registerEvent(int msgId, std::function<std::shared_ptr<MessageResult>(std::shared_ptr<Message> msg)> &&func) override;
    int unregisterEvent(int msgId) override;
    int sendMessage(int msgId, std::shared_ptr<Message> req, std::shared_ptr<MessageResult> res) override;
    int postMessage(int msgId, std::shared_ptr<Message> req) override;
    void handleMessage(std::shared_ptr<Message> req) override;

private:
    MessageDispatcher();
    ~MessageDispatcher();

    Q_DISABLE_COPY(MessageDispatcher);
    QScopedPointer<MessageDispatcherPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MessageDispatcher)
};

#endif // MESSAGEDISPATCHER_H
