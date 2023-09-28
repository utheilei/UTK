#include "messagecenter.h"
#include "messageeventhandler.h"

#include <QWriteLocker>
#include <QCoreApplication>

class UMessageCenterPrivate
{
    Q_DECLARE_PUBLIC(UMessageCenter)
public:
    UMessageCenterPrivate(UMessageCenter* parent)
        : q_ptr(parent), eventHandler(new UMessageEventHandler)
    {
    }

    UMessageCenter* q_ptr = nullptr;
private:
    QReadWriteLock lock;
    QMap<QString, IMessageHandler*> handlers;
    QScopedPointer<UMessageEventHandler> eventHandler;
};

UMessageCenter* UMessageCenter::instance()
{
    static UMessageCenter center;
    return &center;
}

void UMessageCenter::addMessageHandler(const QString &name, IMessageHandler* handler)
{
    Q_D(UMessageCenter);

    QWriteLocker lock(&d->lock);
    d->handlers.insert(name, handler);
}

void UMessageCenter::removeMessageHandler(const QString &name)
{
    Q_D(UMessageCenter);

    QWriteLocker lock(&d->lock);
    d->handlers.remove(name);
}

IMessageHandler* UMessageCenter::handler(const QString &name)
{
    Q_D(UMessageCenter);

    QReadLocker lock(&d->lock);
    return d->handlers[name];
}

int UMessageCenter::sendMessage(const QString &name, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> res)
{
    Q_D(UMessageCenter);

    QReadLocker lock(&d->lock);
    if (!d->handlers.contains(name))
    {
        return -1;
    }
    if (auto object = d->handlers[name])
    {
        res = object->handleMessage(req);
        return 0;
    }
    else
    {
        return -1;
    }
}

int UMessageCenter::postMessage(const QString &name, std::shared_ptr<UMessage> req)
{
    Q_D(UMessageCenter);

    QReadLocker lock(&d->lock);
    if (!d->handlers.contains(name))
    {
        return -1;
    }
    QEvent* event = new UMessageObjectEvent(d->handlers[name], std::move(req));
    QCoreApplication::postEvent(d->eventHandler.get(), event);
    return 0;
}

UMessageCenter::UMessageCenter() : d_ptr(new UMessageCenterPrivate(this))
{
}

UMessageCenter::~UMessageCenter()
{
}
