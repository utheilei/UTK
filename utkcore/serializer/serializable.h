#ifndef USERIALIZABLE_H
#define USERIALIZABLE_H

#include "../utkcore_global.h"
#include "serializeattribute.h"

#include <QObject>
#include <QVector>
#include <QVariant>

/** ----------------------------------------------------------------------------
 * @brief
 *  序列化用于在运行时动态创建新的对象.
 *  容器类型可以支持的: (1)QStringList QByteArray
 *                   (2)QList: int float double                 //适合大对象、数量不定的，特别是带有隐式共享、写时复制的qt对象
 *                   (3)QVector: bool short int float double    //适合各种对象、数量不定的
 *  自定义的类型需要作为成员对象，参考 Q_SERIALIZE_(V/FF/DD) 来处理
 * @attention
 *      (1) QByteArray 在序列化之后是按base64存储的QString
 *      (2) enum 必须在定义之后加 Q_ENUMS(your_enum_name) 否则不能识别
 * @example
 *   class MyObject : public Serializable {
 *      Q_GADGET
 *      Q_SERIALIZABLE
 *
 *      Q_PROPERTY(QList<int>  ColorMatrix  MEMBER ColorMatrix)
 *      Q_PROPERTY(QStringList NameMatrix   MEMBER NameMatrix)
 *      Q_PROPERTY(QByteArray  RamAData     MEMBER RamAData)
 *      Q_PROPERTY(Types       Type         MEMBER Type)
 *      Q_SERIALIZE(LSData, lsData)
 *
 *      enum Types{
 *         Type_0,
 *         Type_1,
 *         Type_2
 *      };
 *      Q_ENUMS(Types)
 *      ...
 *
 *      LSData *lsData;
 *   }
 *
 *   MyObject obj;
 *   XmlSerializer serializer;
 *   serializer.load("c:/file.xml",&obj);
 *
 * ---------------------------------------------------------------------------- **/
#define Q_SERIALIZABLE \
public: \
    virtual const QMetaObject *metaObject() const override {return &staticMetaObject;} \
    virtual Serializable* clone() const override { return  cloneImpl(this);} \
private:

/**
 * @brief 成员是（1）【值】对象或（2）【值】列表 的序列化声明
 */
#define Q_SERIALIZE_V(Name,member) \
    Q_PROPERTY(QVariant Name READ _get##Name##member WRITE _set##Name##member) \
    void _set##Name##member(const QVariant& data) { deserializeMemberV(member,data);} \
    QVariant _get##Name##member() const { return serializeMemberV(member);}
/**
 * @brief 前置声明方式，.h 中声明复杂成员的序列化，当成员对象的类使用前置而没有inlcude头文件
 * @attention 成员是必须【指针】对象或【指针】列表，需要配合Q_SERIALIZE_DD使用
 * @see Q_SERIALIZE_DD
 */
#define Q_SERIALIZE_FF(Name,member) \
    Q_PROPERTY(QVariant Name READ _get##Name##member WRITE _set##Name##member) \
    void _set##Name##member(const QVariant& data); \
    QVariant _get##Name##member() const;
/**
 * @brief 前置声明方式，.cpp中加入此宏实现序列化的方法，当成员对象的类使用前置而没有inlcude头文件
 * @attention 成员必须是【指针】对象或【指针】列表
 */
#define Q_SERIALIZE_DD(Class,Name,member) \
    void Class::_set##Name##member(const QVariant& data) { deserializeMember(&member,data);} \
    QVariant Class::_get##Name##member() const { return serializeMember(member);}

#define Q_SERIALIZE_DDW(Class,Name,member) \
    QVariant Class::_get##Name##member() const { return serializeMember(member);} \
    void Class::_set##Name##member(const QVariant& data)

/**
 * @brief 成员是（1）【指针】对象或（2）【指针】列表 的序列化声明
 */
#define Q_SERIALIZE(Name,member) \
    Q_PROPERTY(QVariant Name READ _get##Name##member WRITE _set##Name##member) \
    void _set##Name##member(const QVariant& data) { deserializeMember(&member,data);} \
    QVariant _get##Name##member() const { return serializeMember(member);}


class USerializablePrivate;
class UTKCORE_EXPORT USerializable
{
    Q_GADGET
public:

    explicit USerializable(USerializable* parent = nullptr);
    USerializable(const USerializable &other, USerializable* parent = nullptr);

    USerializable &operator=(const USerializable &other);

    USerializable* parent() const;
    void setParent(USerializable* parent);
    void setObjectName(const QString &name);
    QString objectName() const;
    virtual const QMetaObject* metaObject() const;
    virtual ~USerializable() ;

    QVariant property(const char* name) const;
    bool setProperty(const char* name, const QVariant &value);


    /**
     * @brief clone
     * @return 新建并复制此对象的数据，这个函数还没被用到，但是请实现它。
     */
    virtual USerializable* clone() const = 0;

    /**
     * @brief serialize 序列化的接口，如果该对象没有复杂成员。无须覆盖（override）
     * @return 序列化后的map数据，它只有属性值，并不包涵对象名称
     * 1.如该对象有name、content属性，那么生成的map数据若以xml表现形式如下
     *      <name>小强</name>
     *      <content>111</content>
     */
    QVariantMap serialize(Attributes attr = Attribute::None) const;

    /**
     * @brief deserialize 反序列化的接口，如果该对象没有复杂成员。无须覆盖（override）
     * @param data 它只是含有属性值，并不包涵对象名称
     * @see serialize
     * @attention
     *  1.如果有动态解析的复杂对象可以覆盖（override）deserializeInternal ，比如ScanBoardProperty中的chipPropey
     *
     */

    void deserialize(const QVariantMap &data, Attributes attr = Attribute::None);


    Attributes attribute() const;
    bool testAttribute(Attribute attr) const;

protected:

    USerializable(Attributes attr, USerializable* parent = nullptr);

    void setAttribute(Attributes attr);
    void setAttribute(Attribute attr, bool enable);
    /**
     * @brief serialize 序列化的接口，如果该对象没有复杂成员。无须覆盖（override）
     * @return 序列化后的map数据，它只有属性值，并不包涵对象名称
     * 1.如该对象有name、content属性，那么生成的map数据若以xml表现形式如下
     *      <name>小强</name>
     *      <content>111</content>
     * 2.如果有动态解析的复杂对象可以覆盖（override）它，比如ScanBoardProperty中的chipPropey
     */
    virtual QVariantMap serializeInternal(Attributes attr) const;

    /**
     * @brief deserialize 反序列化的接口，如果该对象没有复杂成员。无须覆盖（override）
     * @param data 它只是含有属性值，并不包涵对象名称
     * @see serialize
     */

    virtual void deserializeInternal(const QVariantMap &data, Attributes attr);
    /* 【指针】 方式 */
    template<typename T>
    QVariant serializeMember(const T* serializable) const;
    template<typename T>
    void deserializeMember(T** const serializable, const QVariant &data);

    template<typename T>
    QVariant serializeMember(const QList<T*> &plist) const;
    template<typename T>
    void deserializeMember(QList<T*>* plist, const QVariant &data);

    /* 【值】 方式 */
    template<typename T>
    QVariant serializeMemberV(const T &serializable) const;
    template<typename T>
    void deserializeMemberV(T &serializable, const QVariant &data);

    template<typename T>
    QVariant serializeMemberV(const QVector<T> &plist) const;
    template<typename T>
    void deserializeMemberV(QVector<T> &plist, const QVariant &data);

    template<typename T>
    QVariant serializeMemberV(const QList<T> &plist) const;
    template<typename T>
    void deserializeMemberV(QList<T> &plist, const QVariant &data);

    /**
     * @brief serializeListTo
     * @param variantlist
     * @param serializableList
     * @return
     */
    template<typename T>
    static int serializeListTo(QVariantList* mapList, const QList<T*> &list);

    /**
     * @brief deserializeListFrom
     * @param plist
     * @param data
     * @param parent
     * @return
     */
    template<typename T>
    static bool deserializeListFrom(QList<T*>* pList, const QVariantList &mapList, USerializable* parent = nullptr);

    template<typename T>
    static USerializable* cloneImpl(const T* thiz);
private:

    QVariant packObjByAttribute(const QVariantMap &data) const;
    QVariantMap unpackObjByAttribute(const QVariant &packedData) const;

    QVariant packListByAttribute(const QVariantList &listData, const char* packedName) const;
    QVariantList unpackListByAttribute(const QVariant &packedData, const char* packedName) const;

    USerializablePrivate* dPtr;

    Q_DECLARE_PRIVATE_D(dPtr, USerializable)
};

template<typename T>
USerializable* USerializable::cloneImpl(const T* thiz)
{
    return new T(*thiz);
}


template<typename T>
int USerializable::serializeListTo(QVariantList* mapList, const QList<T*> &list)
{
    if (mapList == nullptr)
        return 0;

    foreach (const auto* o, list)
    {
        mapList->append(o->serialize());
    }

    return mapList->count();
}

template<typename T>
bool USerializable::deserializeListFrom(QList<T*>* pList, const QVariantList &mapList, USerializable* parent)
{
    int i = 0;
    foreach (const auto &item, mapList)
    {
        T* t;
        if (pList->count() > i)
        {
            t = pList->at(i);
        }
        else
        {
            t = new T(parent);
            pList->append(t);
        }

        t->deserialize(item.toMap(), parent->attribute());
        ++i;
    }

    while (i < pList->count())
    {
        T* t = pList->takeLast();
        delete t;
    }

    return true;
}

template<typename T>
QVariant USerializable::serializeMember(const T* serializable) const
{
    return packObjByAttribute(serializable ? serializable->serialize(attribute()) : QVariantMap());
}

template<typename T>
void USerializable::deserializeMember(T** const serializable, const QVariant &data)
{
    T* t = *serializable;
    if (t == nullptr)
    {
        t = new T(this);
        *serializable = t;
    }

    QVariantMap map = unpackObjByAttribute(data);
    t->deserialize(map, attribute());
}


template<typename T>
QVariant USerializable::serializeMember(const QList<T*> &list) const
{
    QVariantList mapList;
    serializeListTo(&mapList, list);
    return packListByAttribute(mapList, T::staticMetaObject.className());
}


template<typename T>
void USerializable::deserializeMember(QList<T*>* plist, const QVariant &data)
{
    QVariantList mapList = unpackListByAttribute(data, T::staticMetaObject.className());
    deserializeListFrom(plist, mapList, this);
}

template<typename T>
void USerializable::deserializeMemberV(T &serializable, const QVariant &data)
{
    QVariantMap map = unpackObjByAttribute(data);
    serializable.deserialize(map, attribute());
}

template<typename T>
QVariant USerializable::serializeMemberV(const T &serializable) const
{
    return  packObjByAttribute(serializable.serialize(attribute()));
}

template<typename T>
void USerializable::deserializeMemberV(QVector<T> &plist, const QVariant &data)
{
    QVariantList mapList = unpackListByAttribute(data, T::staticMetaObject.className());
    plist.resize(mapList.size());
    int index = 0;
    foreach (auto &ii, mapList)
    {
        plist[index].deserialize(ii.toMap(), attribute());
        index++;
    }
}

template<typename T>
QVariant USerializable::serializeMemberV(const QVector<T> &list) const
{
    QVariantList mapList;
    for (int i = 0; i < list.size(); i++)
    {
        mapList.append(list[i].serialize(attribute()));
    }

    return packListByAttribute(mapList, T::staticMetaObject.className());
}

template<typename T>
void USerializable::deserializeMemberV(QList<T> &plist, const QVariant &data)
{
    QVariantList mapList = unpackListByAttribute(data, T::staticMetaObject.className());
    plist.clear();
    int index = 0;
    foreach (auto &ii, mapList)
    {
        T t;
        t.deserialize(ii.toMap(), attribute());

        plist.append(t);
        index++;
    }
}

template<typename T>
QVariant USerializable::serializeMemberV(const QList<T> &list) const
{
    QVariantList mapList;
    for (int i = 0; i < list.size(); i++)
    {
        mapList.append(list[i].serialize(attribute()));
    }

    return packListByAttribute(mapList, T::staticMetaObject.className());
}

#define USE_SERIALIZABLE
typedef USerializable SerializableObject;

#endif // USERIALIZABLE_H
