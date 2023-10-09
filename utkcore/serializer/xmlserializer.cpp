#include "xmlserializer.h"

#include <QIODevice>
#include <QDebug>
#include <QDomDocument>
#include <QRegExp>
#include <QVariant>

static const QRegExp INTEGER_NUMBER("^-?\\d+$");
static const QRegExp FLOAT_NUMBER("^(-?\\d+)(\\.\\d+)?$");
static const QRegExp BOOL_FLASE("false", Qt::CaseInsensitive);
static const QRegExp BOOL_TRUE("true", Qt::CaseInsensitive);

static void xmlAddMappedData(QVariantMap &data,
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

        if (value.type() == QVariant::Map)
        {
            QVariantMap map = value.toMap();
            if (map.count() == 1)
            {
                const QByteArray &key = map.firstKey().toLatin1();
                if (key == "int"
                    || key == "bool"
                    || key == "float"
                    || key == "double"
                    || key == "string")
                {
                    if (map.first().type() == QVariant::List)
                    {
                        data[name] = map.first();
                    }
                    else
                    {
                        data[name] = QVariantList() << map.first();
                    }
                    return;
                }

            }
        }

        data[name] = value;
    }
}

static QVariantMap xmlParse(const QDomElement &root)
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

                if (BOOL_TRUE.exactMatch(propertyValueStr))
                {
                    xmlAddMappedData(data, propertyName, QVariant(true));
                }
                else if (BOOL_FLASE.exactMatch(propertyValueStr))
                {
                    xmlAddMappedData(data, propertyName, QVariant(false));
                }
                //Test:这个代码经测试是可以去除的，副作用待验证
                //                else if(INTEGER_NUMBER.exactMatch(propertyValueStr)) {
                //                    xmlAddMappedData(data, propertyName, QVariant(propertyValueStr.toInt()));
                //                }
                //                else if(FLOAT_NUMBER.exactMatch(propertyValueStr)) {
                //                    xmlAddMappedData(data, propertyName, QVariant(propertyValueStr.toDouble()));
                //                }
                else
                {
                    xmlAddMappedData(data, propertyName, QVariant(propertyValueStr));
                }

            }
            else
            {
                // Child.
                const QString &childName = child.tagName();
                xmlAddMappedData(data, childName, xmlParse(child));
            }
        }
    }

    return data;
}

static void xmlAppend(QDomDocument &doc,
                      QDomNode* root,
                      const QVariantMap &data,
                      const QStringList &attributes,
                      bool allPropertiesAreAttributes,
                      bool skipEmptyProperties)
{
    for (QVariantMap::const_iterator i = data.constBegin(); i != data.constEnd(); ++i)
    {
        QVariant::Type type = i.value().type();
        if (type == QVariant::Map)
        {
            // Child node.
            const QString &childName = i.key();
            const QVariantMap &childData = i.value().toMap();
            QDomElement child = doc.createElement(childName);
            root->appendChild(child);
            xmlAppend(doc, &child, childData, attributes, allPropertiesAreAttributes, skipEmptyProperties);
        }
        else if (type == QVariant::List)
        {
            // List of child or property nodes.
            const QVariantList &childList = i.value().toList();
            const QString &childName = i.key();
            // Child node.
            if (childList.count() > 0)
            {
                QVariant::Type type = childList.first().type();
                if (type == QVariant::Map)
                {
                    for (QVariantList::const_iterator j = childList.constBegin(); j != childList.constEnd(); ++j)
                    {
                        QDomElement child = doc.createElement(childName);
                        const QVariantMap &childData = j->toMap();
                        xmlAppend(doc, &child, childData, attributes, allPropertiesAreAttributes, skipEmptyProperties);
                        root->appendChild(child);
                    }
                }
                else if (type == QVariant::List)
                {
                    //这是个什么鬼
                    Q_ASSERT_X(false, __FUNCTION__, "wrong");
                }
                else
                {
                    QDomElement child = doc.createElement(childName);
                    root->appendChild(child);

                    for (QVariantList::const_iterator j = childList.constBegin(); j != childList.constEnd(); ++j)
                    {
                        QDomNode item;
                        if (type == QVariant::Bool)
                        {
                            item = doc.createElement(QStringLiteral("bool"));
                        }
                        else if (type == QVariant::Int || type == QVariant::LongLong)
                        {
                            item = doc.createElement(QStringLiteral("int"));
                        }
                        else if (type == QVariant::Double)
                        {
                            item = doc.createElement(QStringLiteral("float"));
                        }
                        else if (j->canConvert(QVariant::String))
                        {
                            item = doc.createElement(QStringLiteral("string"));
                        }

                        if (!item.isNull())
                        {
                            QString string = j->toString();
                            QDomText text = doc.createTextNode(string);
                            item.appendChild(text);
                            child.appendChild(item);
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
                const QString &propertyValueStr = propertyValue.toString();

                if (propertyValueStr.size() || !skipEmptyProperties)
                {
                    if (root->nodeType() == QDomNode::ElementNode && (allPropertiesAreAttributes || attributes.contains(propertyName)))
                    {
                        static_cast<QDomElement*>(root)->setAttribute(propertyName, propertyValueStr);
                    }
                    else
                    {
                        QDomElement child = doc.createElement(propertyName);
                        QDomText text = doc.createTextNode(propertyValueStr);
                        child.appendChild(text);
                        root->appendChild(child);
                    }
                }
            }
        }
    }
}

bool UXmlSerializer::writeData(QIODevice* dev, const QVariantMap &data) const
{
    if (!dev->open(QIODevice::WriteOnly) && !dev->isWritable())
        return false;

    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    xmlAppend(doc, &doc, data, m_attributes, m_allPropertiesAreAttributes, m_skipEmptyProperties);
    dev->write(doc.toByteArray());

    return true;
}

UXmlSerializer::UXmlSerializer()
    : UAbstractSerializer(Attribute::ListInMap)
{

}

bool UXmlSerializer::readData(QIODevice* dev, QVariantMap* data) const
{
    if (!dev->open(QIODevice::ReadOnly) && !dev->isReadable())
        return false;

    QDomDocument doc;
    QString errorString;
    int errorLine;
    bool successed = doc.setContent(dev, &errorString, &errorLine);
    if (!successed)
    {
        qDebug() << "setContent error:" << errorString << "on line:" << errorLine;
        return  false;
    }

    QDomElement root = doc.firstChildElement();
    data->insert(root.nodeName(), xmlParse(root));
    return true;
}


