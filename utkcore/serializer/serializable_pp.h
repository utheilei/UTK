#ifndef SERIALIZABLE_P_H
#define SERIALIZABLE_P_H

#include <QString>

class USerializable;
namespace Utils
{
    const char* classNameExcludeNamespace(const char* className);
    QString nicknameForSerilizer(const USerializable* object, const QString &recommend);
}

#endif // SERIALIZABLE_P_H
