#pragma once

#include <QtGlobal>

enum Attribute
{
    None,
    EnumAsNumber = 0x2, /*兼容，枚举可以按数值方式序列化，通常情况下反序列化可以自动识别数值和字符串*/
    ListInMap = 0x1, /*兼容, json中数组明确无须再包一层，推荐仅xml中用*/
    ObjectInArray = 0x4, /* 兼容，json中所有对象都在数组中的情况*/
    IncludeReadonlyProperty = 0x10,
    DoNotUseRootNode = 0x20,
    DoNotAcceptOverride = 0x80
};

Q_DECLARE_FLAGS(Attributes, Attribute)

