#ifndef MESSAGE_H
#define MESSAGE_H

#include "utkcore_export.h"

#include <QVariant>

class MessagePrivate;
class UTKCORE_EXPORT Message
{
public:
    Message(int type, int id, const QVariant &data);

    int type() const;

    int id() const;

    QVariant data() const;

private:
    QScopedPointer<MessagePrivate> d_ptr;
    Q_DECLARE_PRIVATE(Message)
};

class MessageResultPrivate;
class UTKCORE_EXPORT MessageResult
{
public:
    MessageResult(bool code, const QVariant &result);

    bool code() const;

    QVariant result() const;

private:
    QScopedPointer<MessageResultPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MessageResult)
};

#endif // MESSAGE_H
