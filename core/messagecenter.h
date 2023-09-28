#ifndef MESSAGECENTER_H
#define MESSAGECENTER_H

#include "messageinterface.h"

class MessageCenterPrivate;
class UTKCORE_EXPORT MessageCenter
{
public:
    static MessageCenter* instance();

    void addMessageHandler(const QString &name, IMessageHandler* handler);

    void removeMessageHandler(const QString &name);

    IMessageHandler* handler(const QString &name);

    int sendMessage(const QString &name, std::shared_ptr<Message> req, std::shared_ptr<MessageResult> res);

    int postMessage(const QString &name, std::shared_ptr<Message> req);

private:
    MessageCenter();
    ~MessageCenter();

    Q_DISABLE_COPY(MessageCenter);
    QScopedPointer<MessageCenterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MessageCenter)
};

#endif // MESSAGECENTER_H
