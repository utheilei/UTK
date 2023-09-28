#include "messagedispatcher.h"
#include "messageeventhandler.h"

#include <QMutex>
#include <QCoreApplication>

class UMessageDispatcherPrivate
{
    Q_DECLARE_PUBLIC(UMessageDispatcher)
public:
    UMessageDispatcherPrivate(UMessageDispatcher* parent)
        : q_ptr(parent), eventHandler(new UMessageEventHandler)
    {
    }

    UMessageDispatcher* q_ptr = nullptr;
private:
    QMutex mutex;
    QMap<int, std::function<std::shared_ptr<UMessageResult>(std::shared_ptr<UMessage> msg)>> handlers;
    QScopedPointer<UMessageEventHandler> eventHandler;
};

UMessageDispatcher* UMessageDispatcher::instance()
{
    static UMessageDispatcher dispatcher;
    return &dispatcher;
}

void UMessageDispatcher::registerEvent(int msgId, std::function<std::shared_ptr<UMessageResult> (std::shared_ptr<UMessage>)> &&func)
{
    Q_D(UMessageDispatcher);
    QMutexLocker locker(&d->mutex);
    d->handlers.insert(msgId, std::move(func));
}

int UMessageDispatcher::unregisterEvent(int msgId)
{
    Q_D(UMessageDispatcher);
    QMutexLocker locker(&d->mutex);
    return d->handlers.remove(msgId);
}

int UMessageDispatcher::sendMessage(int msgId, std::shared_ptr<UMessage> req, std::shared_ptr<UMessageResult> &&res)
{
    Q_D(UMessageDispatcher);

    QMutexLocker locker(&d->mutex);
    if (!d->handlers.contains(msgId))
    {
        return -1;
    }
    if (auto func = d->handlers[msgId])
    {
        res = func(req);
        return 0;
    }
    else
    {
        return -1;
    }
}

int UMessageDispatcher::postMessage(int msgId, std::shared_ptr<UMessage> &&req)
{
    Q_D(UMessageDispatcher);

    QMutexLocker locker(&d->mutex);
    if (!d->handlers.contains(msgId))
    {
        return -1;
    }
    QEvent* event = new UMessageEvent(std::move(req));
    QCoreApplication::postEvent(d->eventHandler.get(), event);
    return 0;
}

void UMessageDispatcher::handleMessage(std::shared_ptr<UMessage> req)
{
    Q_D(UMessageDispatcher);
    QMutexLocker locker(&d->mutex);
    if (!d->handlers.contains(req->msgId))
    {
        return;
    }
    if (auto func = d->handlers[req->msgId])
    {
        func(req);
    }
}

UMessageDispatcher::UMessageDispatcher()
    : MessageInterface(), d_ptr(new UMessageDispatcherPrivate(this))
{
}

UMessageDispatcher::~UMessageDispatcher()
{
}
