#include "jsonserializer.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QIODevice>
#include <QVariant>

UJsonSerializer::UJsonSerializer() : UAbstractSerializer(Attribute::EnumAsNumber)
{
}

void UJsonSerializer::setIndented(bool indent)
{
    m_indented = indent;
}

bool UJsonSerializer::readData(QIODevice* dev, QVariantMap* data) const
{
    if (!dev->open(QIODevice::ReadOnly) && !dev->isReadable())
        return false;

    QByteArray fileData = dev->readAll();

    *data = QJsonDocument::fromJson(fileData).object().toVariantMap();

    return  true;
}

bool UJsonSerializer::writeData(QIODevice* dev, const QVariantMap &data) const
{
    if (!dev->open(QIODevice::WriteOnly) && !dev->isWritable())
        return false;
    QJsonObject jsonObj = QJsonObject::fromVariantMap(data);
    QJsonDocument doc(jsonObj);
    return dev->write(doc.toJson(m_indented ? QJsonDocument::Indented : QJsonDocument::Compact)) > 0;
}
