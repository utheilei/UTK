#ifndef MESSAGE_H
#define MESSAGE_H

#include "utkcore_global.h"

#include <QVariant>

class UTKCORE_EXPORT UMessage
{
public:
    UMessage();
    UMessage(int type, int id, const QVariant &data);
    ~UMessage();

    int msgType = -1;
    int msgId = -1;
    QVariant msgData;
};

class UTKCORE_EXPORT UMessageResult
{
public:
    UMessageResult();
    UMessageResult(bool code, const QVariant &result);
    ~UMessageResult();

    bool msgCode = false;
    QVariant msgResult;
};

#endif // MESSAGE_H
