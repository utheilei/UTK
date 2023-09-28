#include "message.h"

UMessage::UMessage()
{
}

UMessage::UMessage(int type, int id, const QVariant &data)
{
    msgType = type;
    msgId = id;
    msgData = data;
}

UMessage::~UMessage()
{
}

UMessageResult::UMessageResult()
{
}

UMessageResult::UMessageResult(bool code, const QVariant &result)
{
    msgCode = code;
    msgResult = result;
}

UMessageResult::~UMessageResult()
{
}
