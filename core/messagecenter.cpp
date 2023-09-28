#include "messagecenter.h"
#include "messageeventhandler.h"

#include <QWriteLocker>
#include <QCoreApplication>

class MessageCenterPrivate
{
    Q_DECLARE_PUBLIC(MessageCenter)
public:
    MessageCenterPrivate(MessageCenter* parent)
        : q_ptr(parent), eventHandler(new MessageEventHandler)
    {
    }

    MessageCenter* q_ptr = nullptr;
private:
    QReadWriteLock lock;
    QMap<QString, IMessageHandler*> handlers;
    QScopedPointer<MessageEventHandler> eventHandler;
};

MessageCenter* MessageCenter::instance()
{
    static MessageCenter center;
    return &center;
}

void MessageCenter::addMessageHandler(const QString &name, IMessageHandler* handler)
{
    Q_D(MessageCenter);

    QWriteLocker lock(&d->lock);
    d->handlers.insert(name, handler);
}

void MessageCenter::removeMessageHandler(const QString &name)
{
    Q_D(MessageCenter);

    QWriteLocker lock(&d->lock);
    d->handlers.remove(name);
}

IMessageHandler* MessageCenter::handler(const QString &name)
{
    Q_D(MessageCenter);

    QReadLocker lock(&d->lock);
    return d->handlers[name];
}

int MessageCenter::sendMessage(const QString &name, std::shared_ptr<Message> req, std::shared_ptr<MessageResult> res)
{
    Q_D(MessageCenter);

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

int MessageCenter::postMessage(const QString &name, std::shared_ptr<Message> req)
{
    Q_D(MessageCenter);

    QReadLocker lock(&d->lock);
    if (!d->handlers.contains(name))
    {
        return -1;
    }
    QEvent* event = new MessageObjectEvent(d->handlers[name], req);
    QCoreApplication::postEvent(d->eventHandler.get(), event);
    return 0;
}

MessageCenter::MessageCenter() : d_ptr(new MessageCenterPrivate(this))
{
}

MessageCenter::~MessageCenter()
{
}
