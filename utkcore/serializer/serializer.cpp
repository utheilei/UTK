#include "serializer.h"

#include <QJsonObject>
#include <QTextCodec>
#include <QMetaObject>
#include <QObject>
#include <QDebug>

void USerializer::addMappedData(QVariantMap &data,
                               const QString &name,
                               const QVariant &value)
{
    // 如果已经存在则将现有的转换为list
    if (data.contains(name))
    {
        QVariant &existingData = data[name];
        if (existingData.type() == QVariant::List)
        {
            QVariantList listOfValuesWithTheSameName = existingData.toList();
            listOfValuesWithTheSameName.append(value);
            data[name] = listOfValuesWithTheSameName;
        }
        else
        {
            QVariantList listOfValuesWithTheSameName;
            listOfValuesWithTheSameName.append(existingData);
            listOfValuesWithTheSameName.append(value);
            data[name] = listOfValuesWithTheSameName;
        }
    }
    else
    {
        QVariantList listOfValuesWithTheSameName;
        listOfValuesWithTheSameName.append(value);
        data[name] = listOfValuesWithTheSameName;
    }
}

QVariantMap USerializer::serialize(const QObject* object,
                                  bool includeReadOnlyProperties)
{
    QVariantMap data;

    // Properties.
    const QMetaObject* metaObject = object->metaObject();
    for (int i = 0; i < metaObject->propertyCount(); ++i)
    {
        QMetaProperty metaProperty = metaObject->property(i);
        if (metaProperty.isReadable() && (includeReadOnlyProperties || metaProperty.isWritable()))
        {
            QString propertyName = QString::fromLatin1(metaProperty.name());
            if (propertyName != "objectName")
            {
                QVariant propertyValue = object->property(propertyName.toUtf8().constData());
                data[propertyName] = propertyValue;
            }
        }
    }

    // Children.
    for (QObjectList::const_iterator i = object->children().constBegin(); i != object->children().constEnd(); ++i)
    {
        QObject* child = *i;
        QString childName = (child->objectName().size() ? child->objectName() : child->metaObject()->className());
        addMappedData(data, childName, serialize(child, includeReadOnlyProperties));
    }

    return data;
}

void USerializer::deserialize(QObject* object,
                             const QVariantMap &data,
                             const USerializer::ObjectFactory &factory)
{
    if (!object)
        return;

    for (QVariantMap::const_iterator i = data.constBegin(); i != data.constEnd(); ++i)
    {
        if (i.value().type() == QVariant::Map)
        {
            // Child.
            // 1. 反序列化已有child到对应的map
            // 2. 使用map和工厂创建一个新的child
            const QString &childName = i.key();
            const QVariantMap &childData = i.value().toMap();
            QObject* child = object->findChild<QObject*>(childName);
            if (child)
                deserialize(child, childData, factory);
            else if (factory.hasCreator(childName))
            {
                QObject* child = factory.create(childName);
                child->setParent(object);
                child->setObjectName(childName);
                deserialize(child, childData, factory);
            }
        }
        else if (i.value().type() == QVariant::List)
        {
            // List of children
            const QString &childName = i.key();
            const QVariantList &childDataList = i.value().toList();
            QObjectList existingChildList = object->findChildren<QObject*>(childName);
            int count = 0;
            for (QVariantList::const_iterator j = childDataList.constBegin(); j != childDataList.constEnd(); ++j)
            {
                if (j->type() == QVariant::Map)
                {
                    // Child.
                    // 1. 反序列化已有child到对应的map，
                    // 匹配到多个child
                    // 2. 使用map和工厂创建一个新的child
                    const QVariantMap &childData = j->toMap();
                    if (count < existingChildList.count())
                    {
                        deserialize(existingChildList[count], childData, factory);
                        ++count;
                    }
                    else if (factory.hasCreator(childName))
                    {
                        QObject* child = factory.create(childName);
                        child->setParent(object);
                        child->setObjectName(childName);
                        deserialize(child, childData, factory);
                    }
                }
                else
                {
                    // Property.
                    // 反序列化属性
                    const QString &propertyName = childName;
                    const QVariant &propertyValue = *j;
                    object->setProperty(propertyName.toUtf8().constData(), propertyValue);
                }
            }
        }
        else
        {
            // Property.
            const QMetaObject* metaObject = object->metaObject();
            const QString &propertyName = i.key();

            int id = metaObject->indexOfProperty(propertyName.toUtf8().constData());
            QMetaProperty p = metaObject->property(id);
            if (p.isEnumType())
            {
                object->setProperty(propertyName.toUtf8().constData(),  i.value().toInt());
            }
            else
            {
                const QVariant &propertyValue = i.value();
                object->setProperty(propertyName.toUtf8().constData(), propertyValue);
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////


USerializeToXML::USerializeToXML()
{
    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
}

void USerializeToXML::appendXml(QDomDocument &doc,
                               QDomElement &root,
                               const QVariantMap &data,
                               const QStringList &attributes,
                               bool allPropertiesAreAttributes,
                               bool skipEmptyProperties)
{
    for (QVariantMap::const_iterator i = data.constBegin(); i != data.constEnd(); ++i)
    {
        if (i.value().type() == QVariant::Map)
        {
            // Child node.
            const QString &childName = i.key();
            const QVariantMap &childData = i.value().toMap();
            QDomElement child = doc.createElement(childName);
            root.appendChild(child);
            appendXml(doc, child, childData, attributes, allPropertiesAreAttributes, skipEmptyProperties);
        }
        else if (i.value().type() == QVariant::List)
        {
            // List of child or property nodes.
            const QVariantList &childList = i.value().toList();
            for (QVariantList::const_iterator j = childList.constBegin(); j != childList.constEnd(); ++j)
            {
                if (j->type() == QVariant::Map)
                {
                    // Child node.
                    const QString &childName = i.key();
                    const QVariantMap &childData = j->toMap();
                    QDomElement child = doc.createElement(childName);
                    root.appendChild(child);
                    appendXml(doc, child, childData, attributes, allPropertiesAreAttributes, skipEmptyProperties);
                }
                else
                {
                    // Property node.
                    const QString &propertyName = i.key();
                    const QVariant &propertyValue = *j;
                    if (propertyValue.canConvert(QVariant::String))
                    {
                        QString propertyValueStr = propertyValue.toString();
                        if (propertyValueStr.size() || !skipEmptyProperties)
                        {
                            if (allPropertiesAreAttributes || attributes.contains(propertyName))
                            {
                                root.setAttribute(propertyName, propertyValueStr);
                            }
                            else
                            {
                                QDomElement child = doc.createElement(propertyName);
                                QDomText text = doc.createTextNode(propertyValueStr);
                                child.appendChild(text);
                                root.appendChild(child);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            // Property node.
            const QString &propertyName = i.key();
            const QVariant &propertyValue = i.value();
            if (propertyValue.canConvert(QVariant::String))
            {
                QString propertyValueStr = propertyValue.toString();
                if (propertyValueStr.size() || !skipEmptyProperties)
                {
                    if (allPropertiesAreAttributes || attributes.contains(propertyName))
                    {
                        root.setAttribute(propertyName, propertyValueStr);
                    }
                    else
                    {
                        QDomElement child = doc.createElement(propertyName);
                        QDomText text = doc.createTextNode(propertyValueStr);
                        child.appendChild(text);
                        root.appendChild(child);
                    }
                }
            }
        }
    }
}

void USerializeToXML::appendXml(const QObject* object,
                               const QString &fileName,
                               const QStringList &attributes,
                               bool allPropertiesAreAttributes,
                               bool skipEmptyProperties)
{
    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::WriteOnly))
        return;

    QVariantMap data = USerializer::serialize(object);
    appendXml(document, docroot, data, attributes, allPropertiesAreAttributes, skipEmptyProperties);

    QTextStream out(&file);

    out << document.toString();
    file.close();
}

void USerializeToXML::appendXmlNamed(const QObject* object, const QString &fileName, const QStringList &attributes, bool allPropertiesAreAttributes, bool skipEmptyProperties)
{
    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::WriteOnly))
        return;

    QString name(object->objectName().size() ? object->objectName() : object->metaObject()->className());

    QVariantMap data = {{name, USerializer::serialize(object)}};

    appendXml(document, docroot, data, attributes, allPropertiesAreAttributes, skipEmptyProperties);

    QTextStream out(&file);

    out << document.toString();
    file.close();
}

QVariantMap USerializeToXML::parseXml(const QDomElement &root,
                                     const USerializeToXML::ObjectFactory &factory)
{
    QVariantMap data;

    // Node attributes (properties).
    QDomNamedNodeMap attrNodes = root.attributes();
    for (int i = 0; i < attrNodes.size(); ++i)
    {
        QDomAttr attr = attrNodes.item(i).toAttr();
        data[attr.name()] = QVariant(attr.value());
    }

    // Child nodes.
    const QDomNodeList childNodes = root.childNodes();
    for (int i = 0; i < childNodes.size(); ++i)
    {
        const QDomNode &childNode = childNodes.at(i);
        if (childNode.isElement())
        {
            const QDomElement &child = childNode.toElement();
            if ((child.firstChild() == child.lastChild()) && child.firstChild().isText())
            {
                // Property.
                const QString &propertyName = child.tagName();
                QString propertyValueStr = child.firstChild().toText().data();
                USerializer::addMappedData(data, propertyName, QVariant(propertyValueStr));
            }
            else
            {
                // Child.
                const QString &childName = child.tagName();
                USerializer::addMappedData(data, childName, parseXml(child, factory));
            }
        }
    }

    return data;
}

void USerializeToXML::saveXml(const QObject* object,
                             const QString &fileName,
                             const QStringList &attributes,
                             bool allPropertiesAreAttributes,
                             bool skipEmptyProperties)
{
    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::WriteOnly))
        return;

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    const QMetaObject* metaObject = object->metaObject();
    QString tag = object->objectName().size() ? object->objectName() : metaObject->className();
    QDomElement root = doc.createElement(tag);
    doc.appendChild(root);

    QVariantMap data = USerializer::serialize(object);
    appendXml(doc, root, data, attributes, allPropertiesAreAttributes, skipEmptyProperties);
    QTextStream out(&file);
    out << doc.toString();
    document = doc;
    docroot = root;
    file.close();
}

void USerializeToXML::loadXml(QObject* object,
                             const QString &fileName,
                             const USerializeToXML::ObjectFactory &factory)
{
    if (!object)
        return;

    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::ReadOnly))
        return;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();

    const QDomElement &root = doc.firstChildElement();
    QVariantMap data = parseXml(root, factory);
    USerializer::deserialize(object, data, factory);
}

QObject* USerializeToXML::loadXml(const QString &fileName,
                                 const USerializeToXML::ObjectFactory &factory)
{
    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::ReadOnly))
        return nullptr;

    QDomDocument doc;
    doc.setContent(&file);
    file.close();
    const QDomElement &root = doc.firstChildElement();
    if (!factory.hasCreator(root.tagName()))
        return nullptr;

    QObject* object = factory.create(root.tagName());
    if (!object)
        return nullptr;

    QVariantMap data = parseXml(root, factory);
    USerializer::deserialize(object, data, factory);

    return object;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

void USerializeToJson::saveJson(const QObject* object, const QString &fileName, QJsonDocument::JsonFormat format)
{
    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::WriteOnly))
        return;

    const QMetaObject* metaObject = object->metaObject();
    QString tag = object->objectName().size() ? object->objectName() : metaObject->className();

    QVariantMap data = USerializer::serialize(object);
    QJsonObject jsonObj = QJsonObject::fromVariantMap(data);
    QJsonDocument doc(jsonObj);
    file.write(doc.toJson(format));
    file.close();
}

void USerializeToJson::saveJson(const QObject* object, QByteArray &dataArray, QJsonDocument::JsonFormat format)
{
    if (object == nullptr)
    {
        return;
    }

    const QMetaObject* metaObject = object->metaObject();
    QString tag = object->objectName().size() ? object->objectName() : metaObject->className();

    QVariantMap dataMap = USerializer::serialize(object);
    QJsonObject jsonObj = QJsonObject::fromVariantMap(dataMap);
    QJsonDocument doc(jsonObj);

    dataArray.append(doc.toJson(format));
}

void USerializeToJson::loadJson(QObject* object, const QString &fileName,
                               const ObjectFactory &factory)
{
    if (!object)
        return;

    QFile file(fileName);
    if (!file.open(QFile::Text | QFile::ReadOnly))
        return;

    QByteArray fileData = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(fileData));
    file.close();

    QVariantMap data = doc.object().toVariantMap();
    USerializer::deserialize(object, data, factory);
}

void USerializeToJson::loadJson(QObject* object, const QByteArray data,
                               const ObjectFactory &factory)
{
    if (object == nullptr)
    {
        return;
    }

    QJsonDocument doc(QJsonDocument::fromJson(data));

    QVariantMap dataMap = doc.object().toVariantMap();
    USerializer::deserialize(object, dataMap, factory);
}
