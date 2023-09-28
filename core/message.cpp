#include "message.h"

class MessagePrivate
{
    Q_DECLARE_PUBLIC(Message)
public:
    MessagePrivate(Message* parent) : q_ptr(parent)
    {
    }

    Message* q_ptr = nullptr;
private:
    int type = -1;
    int id = -1;
    QVariant data;
};

class MessageResultPrivate
{
    Q_DECLARE_PUBLIC(MessageResult)
public:
    MessageResultPrivate(MessageResult* parent) : q_ptr(parent)
    {
    }

    MessageResult* q_ptr = nullptr;
private:
    bool code = false;
    QVariant result;
};

Message::Message(int type, int id, const QVariant &data) : d_ptr(new MessagePrivate(this))
{
    Q_D(Message);
    d->type = type;
    d->id = id;
    d->data = data;
}

int Message::type() const
{
    Q_D(const Message);
    return d->type;
}

int Message::id() const
{
    Q_D(const Message);
    return d->id;
}

QVariant Message::data() const
{
    Q_D(const Message);
    return d->data;
}

MessageResult::MessageResult(bool code, const QVariant &result) : d_ptr(new MessageResultPrivate(this))
{
    Q_D(MessageResult);
    d->code = code;
    d->result = result;
}

bool MessageResult::code() const
{
    Q_D(const MessageResult);
    return d->code;
}

QVariant MessageResult::result() const
{
    Q_D(const MessageResult);
    return d->result;
}
