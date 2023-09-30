#ifndef UMESSAGECENTER_H
#define UMESSAGECENTER_H

#include "umessageinterface.h"

class UMessageCenterPrivate;
class UTKCORE_EXPORT UMessageCenter
{
public:
    static UMessageCenter* instance();

    void addMessageHandler(const QString &name, UIMessageHandler* handler);

    void removeMessageHandler(const QString &name);

    UIMessageHandler* handler(const QString &name);

    int sendMessage(const QString &name, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> res);

    int postMessage(const QString &name, std::shared_ptr<UMessage> req);

private:
    UMessageCenter();
    ~UMessageCenter();

    Q_DISABLE_COPY(UMessageCenter);
    QScopedPointer<UMessageCenterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UMessageCenter)
};

#endif // UMESSAGECENTER_H
