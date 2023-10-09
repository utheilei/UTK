#include "abstractserializer.h"
#include "serializable.h"
#include "serializable_pp.h"

#include <QFile>
#include <QBuffer>
#include <QDebug>
#include <QVariant>
#include <QVariantMap>

UAbstractSerializer::UAbstractSerializer(Attributes attr)
    : m_attribute(attr)
{

}

UAbstractSerializer::~UAbstractSerializer() {}

bool UAbstractSerializer::load(QIODevice* dev, USerializable* object, const QString &name) const
{
    if (!object)
        return false;

    QVariantMap data;
    if (!readData(dev, &data))
        return false;

    if (!testAttribute(Attribute::DoNotUseRootNode))
    {
        QString nickname = Utils::nicknameForSerilizer(object, name);
        if (!data.contains(nickname))
        {
            return false;
        }

        object->deserialize(data[nickname].toMap(), attribute());
    }
    else
    {
        object->deserialize(data, attribute());
    }
    return true;
}

bool UAbstractSerializer::load(const QString &fileName, QVariantMap* data) const
{
    QFile file(fileName);
    return readData(&file, data);
}

bool UAbstractSerializer::load(const QByteArray &text, QVariantMap* data) const
{
    QBuffer buffer;
    buffer.setData(text);
    return readData(&buffer, data);
}

bool UAbstractSerializer::load(const QByteArray &text, USerializable* object, const QString &name) const
{
    QBuffer buffer;
    buffer.setData(text);

    return load(&buffer, object, name);
}


bool UAbstractSerializer::load(const QString &fileName, USerializable* object, const QString &name) const
{
    QFile file(fileName);
    return load(&file, object, name);
}

bool UAbstractSerializer::save(const QString &file, const QVariantMap &data) const
{
    QFile dev(file);
    return writeData(&dev, data);
}

bool UAbstractSerializer::save(const QString &file, const USerializable* object, const QString &name) const
{
    QFile dev(file);
    return save(&dev, object, name);
}

bool UAbstractSerializer::save(QIODevice* dev, const USerializable* object, const QString &name) const
{
    QVariantMap data = object->serialize(attribute());
    if (!testAttribute(Attribute::DoNotUseRootNode))
    {

        QString nickname = Utils::nicknameForSerilizer(object, name);
        return writeData(dev, {{nickname, data}});
    }
    else
    {
        return writeData(dev, data);
    }
}

Attributes UAbstractSerializer::attribute() const
{
    return m_attribute;
}

bool UAbstractSerializer::testAttribute(Attribute attr) const
{
    return static_cast<int>(m_attribute) & static_cast<int>(attr);
}

void UAbstractSerializer::setAttribute(Attributes attr)
{
    m_attribute = attr;
}

void UAbstractSerializer::setAttribute(Attribute attr, bool on)
{
    m_attribute.setFlag(attr, on);
}
