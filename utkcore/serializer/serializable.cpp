#include "serializable.h"

#include <QSize>
#include <QDateTime>
#include <QMetaObject>
#include <QMetaProperty>
#include <QRegExp>
#include <QFile>
#include <QBuffer>
#include <QStringList>
#include <QTextStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QVarLengthArray>

Q_DECLARE_METATYPE(QList<int>)
Q_DECLARE_METATYPE(QList<float>)
Q_DECLARE_METATYPE(QList<double>)
Q_DECLARE_METATYPE(QList<QSize>)

Q_DECLARE_METATYPE(QVector<bool>)
Q_DECLARE_METATYPE(QVector<short>)
Q_DECLARE_METATYPE(QVector<int>)

Q_DECLARE_METATYPE(QVarLengthArray<bool>)
Q_DECLARE_METATYPE(QVarLengthArray<short>)
Q_DECLARE_METATYPE(QVarLengthArray<int>)

static const char* SIZE_PATTERN = "%d*%d";

template<typename N>
static QVariant convertToRawList(const QVariant &source)
{

    const QVariantList &list = source.toList();

    QList<N> rawList;
    foreach (auto ii, list)
    {
        rawList.append(ii.value<N>());
    }

    return QVariant::fromValue(rawList);
}

template<typename N>
static QVariant convertToRawVector(const QVariant &source)
{

    const QVariantList &list = source.toList();

    QVector<N> rawList;
    foreach (auto ii, list)
    {
        rawList.append(ii.value<N>());
    }

    return QVariant::fromValue(rawList);
}

template<typename N>
static QVariant convertToRawVarArray(const QVariant &source)
{

    const QVariantList &list = source.toList();

    QVarLengthArray<N> rawList(list.size());
    for (int i = 0; i < rawList.size() ; i++)
    {
        rawList[i] = list[i].value<N>();
    }

    return QVariant::fromValue(rawList);
}

static QVariant convertToRawListSize(const QVariant &source)
{

    const QVariantList &list = source.toList();

    QList<QSize> rawList;
    foreach (auto ii, list)
    {
        int w = 0;
        int h = 0;
        sscanf(ii.toString().toLatin1().constData(), SIZE_PATTERN, &w, &h);

        rawList.append(QSize(w, h));
    }

    return QVariant::fromValue(rawList);
}

static QVariant convertFromRawQSize(const QVariant &source)
{

    int w = 0;
    int h = 0;
    sscanf(source.toString().toLatin1().constData(), SIZE_PATTERN, &w, &h);

    return QSize(w, h);
}

static QVariant convertToStringList(const QVariant &source)
{
    return source.toStringList();
}

static QVariant convertToDataTime(const QVariant &source)
{
    return QDateTime::fromString(source.toString(), Qt::ISODate);
}

static QVariant convertToByteArray(const QVariant &source)
{
    return QByteArray::fromBase64(source.toString().toLatin1());
}

template<typename T>
static QVariant convertFromList(const QVariant &source)
{

    QVariantList list;

    QList<T> rawList = qvariant_cast< QList<T>>(source);
    foreach (const T &ii, rawList)
    {
        list.append(ii);
    }

    return list;
}

template<typename T>
static QVariant convertFromVector(const QVariant &source)
{

    QVariantList list;

    QVector<T> rawList = qvariant_cast< QVector<T>>(source);
    foreach (const T &ii, rawList)
    {
        list.append(ii);
    }

    return list;
}

template<typename T>
static QVariant convertFromVarArray(const QVariant &source)
{

    QVariantList list;

    QVarLengthArray<T> rawList = qvariant_cast< QVarLengthArray<T>>(source);
    for (int i = 0; i < rawList.size() ; i++)
    {
        list.append(rawList[i]);
    }

    return list;
}

static QVariant convertFromRawListSize(const QVariant &source)
{

    QVariantList list;

    QList<QSize> rawList = qvariant_cast< QList<QSize>>(source);

    foreach (auto ii, rawList)
    {
        list.append(QString::asprintf(SIZE_PATTERN, ii.width(), ii.height()));
    }

    return list;
}

static QVariant convertToRawQSize(const QVariant &source)
{

    QSize size = source.toSize();

    return QString::asprintf(SIZE_PATTERN, size.width(), size.height());
}

static QVariant convertFromStringList(const QVariant &source)
{

    QVariantList list;

    foreach (const QString &ii, source.toStringList())
    {
        list.append(ii);
    }

    return list;
}

static QVariant convertFromDateTime(const QVariant &source)
{
    return source.toDateTime().toString(Qt::ISODate);
}

static QVariant convertFromByteArray(const QVariant &source)
{
    return QString(source.toByteArray().toBase64());
}

static QVariant convertToEnum(const QMetaProperty &metaProperty, const QVariant &source)
{
    QMetaEnum metaEnum = metaProperty.enumerator();
    //    qDebug()<<" propertyName="<<metaProperty.name()<<"QMetaEnum type="<<metaEnum.name();
    QByteArray enumValue = source.toString().toLatin1();

    const char* enumName = metaEnum.name();
    const int enumNameLen = strlen(enumName);

    const int count = metaEnum.keyCount();
    for (int i = 0; i < count; ++i)
    {
        //                        qDebug()<<"QMetaEnum: object="<<object->objectName()<<" from name="<<enumValue<<"to name="<<metaEnum.key(i);
        const char* enumString = metaEnum.key(i);

        if (strstr(enumString, enumName) == enumString)
        {
            enumString = enumString + enumNameLen;
            if (enumString[0] == '_')
                enumString++;
        }

        if (strcmp(enumString, enumValue.data()) == 0)
        {
            int n = metaEnum.value(i);
            return  QVariant(n);
        }
    }

    return QVariant();
}

static QVariant convertFromEnum(const QMetaProperty &metaProperty, const QVariant &source)
{
    QMetaEnum metaEnum = metaProperty.enumerator();
    //                    qDebug()<<"QMetaEnum :object="<<object->objectName()<<" propertyName="<<metaProperty.name()<<"QMetaEnum type="<<metaEnum.name();
    int enumInt = source.toInt();
    const char* enumName = metaEnum.name();
    const int enumNameLen = strlen(enumName);
    const int count = metaEnum.keyCount();

    for (int i = 0; i < count; ++i)
    {
        if (enumInt == metaEnum.value(i))
        {
            const char* enumString = metaEnum.key(i);
            if (strstr(enumString, enumName) == enumString)
            {
                enumString = enumString + enumNameLen;
                if (enumString[0] == '_')
                    enumString++;
            }

            return  QString(enumString);
        }
    }

    return QVariant();
}

static QVariant convertForWrite_SpecialProperty(const QMetaProperty &metaProperty, const QVariant &source)
{
    QVariant target;
    int type = metaProperty.userType();

    if (metaProperty.isEnumType())
    {
        target = convertToEnum(metaProperty, source);
    }
    else if (type == QVariant::ByteArray)
    {
        target = convertToByteArray(source);
    }
    else if (type == QVariant::StringList)
    {
        target = convertToStringList(source);
    }
    else if (type == QVariant::DateTime)
    {
        target = convertToDataTime(source);
    }
    else if (type == QVariant::Size)
    {
        target = convertToRawQSize(source);
    }
    else if (type == qMetaTypeId<QList<int>>())
    {
        target = convertToRawList<int>(source);
    }
    else if (type == qMetaTypeId<QVector<int>>())
    {
        target = convertToRawVector<int>(source);
    }
    else if (type == qMetaTypeId<QVector<short>>())
    {
        target = convertToRawVector<short>(source);
    }
    else if (type == qMetaTypeId<QVector<bool>>())
    {
        target = convertToRawVector<bool>(source);
    }
    else if (type == qMetaTypeId<QList<float>>())
    {
        target = convertToRawList<float>(source);
    }
    else if (type == qMetaTypeId<QList<double>>())
    {
        target = convertToRawList<double>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<int>>())
    {
        target = convertToRawVarArray<int>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<short>>())
    {
        target = convertToRawVarArray<short>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<bool>>())
    {
        target = convertToRawVarArray<bool>(source);
    }
    else if (type == qMetaTypeId<QList<QSize>>())
    {
        target = convertToRawListSize(source);
    }

    return target.isValid() ? target : source;
}

static QVariant convertForRead_SpecialProperty(const QMetaProperty &metaProperty, const QVariant &source, bool enumAsNumber = false)
{
    QVariant target;
    int type = metaProperty.userType();

    if (metaProperty.isEnumType() && !enumAsNumber)
    {
        target = convertFromEnum(metaProperty, source);
    }
    else if (type == QVariant::ByteArray)
    {
        target = convertFromByteArray(source);
    }
    else if (type == QVariant::StringList)
    {
        target = convertFromStringList(source);
    }
    else if (type == QVariant::DateTime)
    {
        target = convertFromDateTime(source);
    }
    else if (type == QVariant::Size)
    {
        target = convertFromRawQSize(source);
    }
    else if (type == qMetaTypeId<QList<int>>())
    {
        target = convertFromList<int>(source);
    }
    else if (type == qMetaTypeId<QVector<int>>())
    {
        target = convertFromVector<int>(source);
    }
    else if (type == qMetaTypeId<QVector<short>>())
    {
        target = convertFromVector<short>(source);
    }
    else if (type == qMetaTypeId<QVector<bool>>())
    {
        target = convertFromVector<bool>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<int>>())
    {
        target = convertFromVarArray<int>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<short>>())
    {
        target = convertFromVarArray<short>(source);
    }
    else if (type == qMetaTypeId<QVarLengthArray<bool>>())
    {
        target = convertFromVarArray<bool>(source);
    }
    // in QVariant float stared as double
    else if (type == qMetaTypeId<QList<float>>() || type == qMetaTypeId<QList<double>>())
    {
        target = convertFromList<double>(source);
    }
    else if (type == qMetaTypeId<QList<QSize>>())
    {
        target = convertFromRawListSize(source);
    }

    return target.isValid() ? target : source;
}

#include "serializable_pp.h"

class AttrbuteDeamon
{
public:
    AttrbuteDeamon(const USerializablePrivate* data, Attributes requiredAttr);
    ~AttrbuteDeamon();

private:
    USerializablePrivate* m_data;
    Attributes m_backupAttr;
};


class USerializablePrivate
{
public:

    USerializablePrivate(const Attributes &attr);
    USerializablePrivate(const USerializablePrivate &other);

    USerializablePrivate &operator = (const USerializablePrivate &other);
    void deleteChildren();
    void setParent_helper(USerializable* thiz, USerializable* o);

    static QVariantMap serializeInternal(const USerializable* object, Attributes m_attr);
    static bool deserializeInternal(const QVariantMap &data, Attributes m_attr, USerializable* object);

public:
    QString name;
    QList<USerializable*> children;
    USerializable* parent = nullptr;
    USerializable* currentChildBeingDeleted = nullptr;
    bool isDeletingChildren = false;
    bool wasDeleted = false;
    Attributes m_attr = Attribute::None;
};

void USerializablePrivate::deleteChildren()
{
    isDeletingChildren = true;

    // delete siblings
    for (int i = 0; i < children.count(); ++i)
    {
        currentChildBeingDeleted = children.at(i);
        children[i] = nullptr;
        delete currentChildBeingDeleted;
    }
    children.clear();

    currentChildBeingDeleted = nullptr;
    isDeletingChildren = false;
}

void USerializablePrivate::setParent_helper(USerializable* q, USerializable* o)
{
    if (o == parent)
        return;
    if (parent)
    {
        USerializablePrivate* parentD = parent->d_func();
        if (parentD->isDeletingChildren && wasDeleted
            && parentD->currentChildBeingDeleted == q)
        {
            // don't do anything since QObjectPrivate::deleteChildren() already
            // cleared our entry in parentD->children.
        }
        else
        {
            const int index = parentD->children.indexOf(q);
            if (parentD->isDeletingChildren)
            {
                parentD->children[index] = nullptr;
            }
            else
            {
                parentD->children.removeAt(index);
            }
        }
    }
    parent = o;
    if (parent)
    {
        // object hierarchies are constrained to a single thread
        parent->d_func()->children.append(q);
    }
}

USerializable::USerializable(USerializable* parent)
    : dPtr(new USerializablePrivate(Attribute::None))
{
    Q_D(USerializable);
    if (parent)
        d->setParent_helper(this, parent);
}

USerializable::USerializable(Attributes attr, USerializable* parent)
    : dPtr(new USerializablePrivate(attr))
{
    Q_D(USerializable);
    if (parent)
        d->setParent_helper(this, parent);
}

USerializable::USerializable(const USerializable &other, USerializable* parent)
    : dPtr(new USerializablePrivate(*other.dPtr))
{
    Q_D(USerializable);
    if (parent)
        d->setParent_helper(this, parent);
}

USerializable &USerializable::operator=(const USerializable &other)
{
    if (this != &other)
    {
        delete dPtr;

        if (other.dPtr)
            dPtr = new USerializablePrivate(*other.dPtr);
        else
            dPtr = new USerializablePrivate(Attribute::None);
    }
    return *this;
}

USerializable* USerializable::parent() const
{
    Q_D(const USerializable);
    return  d->parent;
}

void USerializable::setParent(USerializable* parent)
{
    Q_D(USerializable);
    if (parent)
        d->setParent_helper(this, parent);
}

void USerializable::setObjectName(const QString &name)
{
    Q_D(USerializable);

    d->name = name;
}

QString USerializable::objectName() const
{
    Q_D(const USerializable);
    return d->name;
}

const QMetaObject* USerializable::metaObject() const
{
    return &USerializable::staticMetaObject;
}

USerializable::~USerializable()
{
    Q_D(USerializable);
    d->wasDeleted = true;
    if (!d->children.isEmpty())
        d->deleteChildren();

    if (d->parent)
        d->setParent_helper(this, nullptr);

    delete d;
}

QVariant USerializable::property(const char* name) const
{
    const QMetaObject* meta = metaObject();
    if (!name)
        return QVariant();

    int id = meta->indexOfProperty(name);
    if (id < 0)
    {
        return QVariant();
    }

    QMetaProperty p = meta->property(id);
    return p.readOnGadget(this);
}

bool USerializable::setProperty(const char* name, const QVariant &value)
{
    const QMetaObject* meta = metaObject();
    if (!name)
        return false;

    int id = meta->indexOfProperty(name);
    if (id < 0)
    {
        return false;
    }

    QMetaProperty p = meta->property(id);
    return p.writeOnGadget(this, value);
}

QVariantMap USerializable::serialize(Attributes attr) const
{
    return serializeInternal(attr);
}

void USerializable::deserialize(const QVariantMap &data, Attributes attr)
{
    deserializeInternal(data, attr);
}

QVariantMap USerializable::serializeInternal(Attributes attr) const
{
    Q_D(const USerializable);
    AttrbuteDeamon deamon(d, attr);
    return USerializablePrivate::serializeInternal(this, attr);
}

void USerializable::deserializeInternal(const QVariantMap &data, Attributes attr)
{
    Q_D(USerializable);
    AttrbuteDeamon deamon(d, attr);
    USerializablePrivate::deserializeInternal(data, attr, this);
}

Attributes USerializable::attribute() const
{
    Q_D(const USerializable);
    return d->m_attr;
}

bool USerializable::testAttribute(Attribute attr) const
{
    Q_D(const USerializable);
    return d->m_attr.testFlag(attr);
}

void USerializable::setAttribute(Attributes attr)
{
    Q_D(USerializable);
    d->m_attr = attr;
}

void USerializable::setAttribute(Attribute attr, bool on)
{
    Q_D(USerializable);
    d->m_attr.setFlag(attr, on);
}

QVariant USerializable::packObjByAttribute(const QVariantMap &data) const
{
    if (testAttribute(Attribute::ObjectInArray))
    {
        QVariantList array;
        if (!data.isEmpty())
            array.append(data);
        return array;
    }

    return data;
}

QVariantMap USerializable::unpackObjByAttribute(const QVariant &packedData) const
{
    if (!testAttribute(Attribute::ObjectInArray))
    {
        return packedData.toMap();
    }

    return packedData.toList().value(0).toMap();
}

QVariant USerializable::packListByAttribute(const QVariantList &listData, const char* packedName) const
{
    if (testAttribute(Attribute::ListInMap))
    {
        QString name(Utils::classNameExcludeNamespace(packedName));
        QVariantMap map = {{name, listData}};
        return  map;
    }

    return listData;
}

QVariantList USerializable::unpackListByAttribute(const QVariant &packedData, const char* packedName) const
{
    if (testAttribute(Attribute::ListInMap))
    {
        QVariantMap map = packedData.toMap();
        QString name(Utils::classNameExcludeNamespace(packedName));

        if (map.contains(name))
        {
            const QVariant &data  = map[name];
            const QVariant::Type type = data.type();
            if (type == QVariant::List)
            {
                return data.toList();
            }
            //兼容xml解析中，只有一个对象时，没生成数组的情况
            else if (type == QVariant::Map)
            {
                return {data};
            }
        }
    }
    else
    {
        const QVariant &data  = packedData;
        const QVariant::Type type = data.type();
        if (type == QVariant::List)
        {
            return data.toList();
        }
        //兼容xml解析中，只有一个对象时，没生成数组的情况
        else if (type == QVariant::Map)
        {
            return {data};
        }
    }

    return QVariantList();
}

USerializablePrivate::USerializablePrivate(const Attributes &attr)
    : m_attr(attr)
{

}

USerializablePrivate::USerializablePrivate(const USerializablePrivate &other)
    : m_attr(other.m_attr)
{

}

USerializablePrivate &USerializablePrivate::operator=(const USerializablePrivate &other)
{
    if (this != &other)
    {
        m_attr = other.m_attr;
    }
    return *this;
}
/**
 * @brief SerializablePrivate::serializeInternal
 * @param object
 * @param attr
 * @return
 * @attention 此函数仅供Serializable::serilize使用，其他地方不可以调用
 */
QVariantMap USerializablePrivate::serializeInternal(const USerializable* object, Attributes attr)
{

    QVariantMap data;

    bool enumAsNumber = attr.testFlag(Attribute::EnumAsNumber);
    bool includeReadOnlyProperties =  attr.testFlag(Attribute::IncludeReadonlyProperty);

    const QMetaObject* metaObject = object->metaObject();
    const QLatin1String objectName("objectName");
    for (int i = 0; i < metaObject->propertyCount(); ++i)
    {
        QMetaProperty metaProperty = metaObject->property(i);
        if (objectName != metaProperty.name() && (includeReadOnlyProperties || metaProperty.isWritable()))
        {
            QVariant propertyValue = convertForRead_SpecialProperty(metaProperty, metaProperty.readOnGadget(object), enumAsNumber);
            data[metaProperty.name()] = propertyValue;
        }
    }

    return data;
}

/**
 * @brief SerializablePrivate::deserializeInternal
 * @param data
 * @param attr
 * @param object
 * @return
 * @attention 此函数仅供Serializable::deserilize使用，其他地方不可以调用
 */
bool USerializablePrivate::deserializeInternal(const QVariantMap &data, Attributes attr, USerializable* object)
{
    Q_UNUSED(attr);

    const QMetaObject* metaObject = object->metaObject();

    for (QVariantMap::const_iterator i = data.constBegin(); i != data.constEnd(); ++i)
    {
        const QByteArray &propertyName = i.key().toLatin1();
        int id = metaObject->indexOfProperty(propertyName.constData());
        QMetaProperty metaProperty = metaObject->property(id);
        if (metaProperty.isValid())
        {
            QVariant propertyValue = convertForWrite_SpecialProperty(metaProperty, i.value());
            //            metaProperty.write(object,propertyValue);
            metaProperty.writeOnGadget(object, propertyValue);
        }
    }

    return true;
}

AttrbuteDeamon::AttrbuteDeamon(const USerializablePrivate* data, Attributes requiredAttr):
    m_data(const_cast<USerializablePrivate*>(data)), m_backupAttr(data->m_attr)
{
    if (!m_data->m_attr.testFlag(Attribute::DoNotAcceptOverride))
        m_data->m_attr = requiredAttr;
}

AttrbuteDeamon::~AttrbuteDeamon()
{
    m_data->m_attr = m_backupAttr;
}

namespace Utils
{
    const char* classNameExcludeNamespace(const char* className)
    {
        const char* anchor = className;
        for (const char* begin = anchor; *begin != '\0' ; begin++)
        {
            if (*begin == ':')
                anchor = begin + 1;
        }

        return anchor;
    }

    QString nicknameForSerilizer(const USerializable* object, const QString &recommend)
    {

        QString nickname = recommend;
        if (!nickname.isEmpty())
        {
            return nickname;
        }

        nickname = object->objectName();
        if (!nickname.isEmpty())
        {
            return nickname;
        }

        nickname = classNameExcludeNamespace(object-> metaObject()->className());

        return nickname;
    }
} // Utils
