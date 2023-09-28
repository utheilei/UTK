#include "messagedispatcher.h"
#include "messageeventhandler.h"

#include <QMutex>
#include <QCoreApplication>

class MessageDispatcherPrivate
{
    Q_DECLARE_PUBLIC(MessageDispatcher)
public:
    MessageDispatcherPrivate(MessageDispatcher* parent)
        : q_ptr(parent), eventHandler(new MessageEventHandler)
    {
    }

    MessageDispatcher* q_ptr = nullptr;
private:
    QMutex mutex;
    QMap<int, std::function<std::shared_ptr<MessageResult>(std::shared_ptr<Message> msg)>> handlers;
    QScopedPointer<MessageEventHandler> eventHandler;
};

MessageDispatcher* MessageDispatcher::instance()
{
    static MessageDispatcher dispatcher;
    return &dispatcher;
}

void MessageDispatcher::registerEvent(int msgId, std::function<std::shared_ptr<MessageResult> (std::shared_ptr<Message>)> &&func)
{
    Q_D(MessageDispatcher);
    QMutexLocker locker(&d->mutex);
    d->handlers.insert(msgId, std::move(func));
}

int MessageDispatcher::unregisterEvent(int msgId)
{
    Q_D(MessageDispatcher);
    QMutexLocker locker(&d->mutex);
    return d->handlers.remove(msgId);
}

int MessageDispatcher::sendMessage(int msgId, std::shared_ptr<Message> req, std::shared_ptr<MessageResult> res)
{
    Q_D(MessageDispatcher);

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

int MessageDispatcher::postMessage(int msgId, std::shared_ptr<Message> req)
{
    Q_D(MessageDispatcher);

    QMutexLocker locker(&d->mutex);
    if (!d->handlers.contains(msgId))
    {
        return -1;
    }
    QEvent* event = new MessageEvent(req);
    QCoreApplication::postEvent(d->eventHandler.get(), event);
    return 0;
}

void MessageDispatcher::handleMessage(std::shared_ptr<Message> req)
{
    Q_D(MessageDispatcher);
    QMutexLocker locker(&d->mutex);
    if (!d->handlers.contains(req->id()))
    {
        return;
    }
    if (auto func = d->handlers[req->id()])
    {
        func(req);
    }
}

MessageDispatcher::MessageDispatcher()
    : MessageInterface(), d_ptr(new MessageDispatcherPrivate(this))
{
}

MessageDispatcher::~MessageDispatcher()
{
}
