#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "../utkcore_global.h"

#include <QMetaProperty>
#include <QObject>
#include <QVariantList>
#include <QVariantMap>

#include <QtXml/QDomDocument>
#include <QTextStream>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

class USerializer
{
public:
    /* ----------------------------------------------------------------------------
     * 在反序列化用于在运行时动态创建新的对象.目前,对象需要一个默认的无参构造函数。
     *
     * like:
     *   class MyObject : public QObject { ... }
     *
     * Register method 1:
     *   factoryInstance.registerCreator<MyObject>(ID);
     * Register method 2:
     *   QObject* myObjectCreator() { return new MyObject(); }
     *   factoryInstance.registerCreator(ID,myObjectCreator);
     * ---------------------------------------------------------------------------- */
    class ObjectFactory
    {
    public:
        typedef QObject* (*ObjectCreatorFuncPtr)();
        typedef QMap<QString, ObjectCreatorFuncPtr> ObjectCreatorMap;

    public:
        inline bool hasCreator(const QString &id) const
        {
            return _objectCreatorMap.contains(id);
        }

        QT_DEPRECATED_X("Recommend to use template instead")
        inline void registerCreator(const QString &id, ObjectCreatorFuncPtr creator)
        {
            _objectCreatorMap[id] = creator;
        }

        template<typename T>
        inline void registerCreator(const QString &id)
        {

            auto creator = []()
            {
                return static_cast<QObject*>(new T());
            };

            _objectCreatorMap[id] = creator;
        }

        QObject* create(const QString &id) const
        {
            if (_objectCreatorMap.contains(id))
                return (*_objectCreatorMap[id])();
            return nullptr;
        }

    private:
        ObjectCreatorMap _objectCreatorMap;
    };

    ///
    /// \brief addMappedData
    /// \param data
    /// \param name 属性
    /// \param value
    /// 添加一个“键值对” 到map中
    static void addMappedData(QVariantMap &data, const QString &name, const QVariant &value);
    ///
    /// \brief serialize
    /// \param object 待序列化的对象
    /// \param includeReadOnlyProperties
    /// 必须使用 Q_PROPERTY
    /// 为false时，序列化可读可写的属性，为ture时只序列化可读属性
    /// \return
    ///
    static QVariantMap serialize(const QObject* object, bool includeReadOnlyProperties = false);
    ///
    /// \brief deserialize
    /// \param object
    /// \param data
    /// \param factory
    ///
    static void deserialize(QObject* object, const QVariantMap &data,
                            const ObjectFactory &factory = ObjectFactory());
};

/* ----------------------------------------------------------------------------
 * 要被序列化的对象必须继承与QObject
 * ---------------------------------------------------------------------------- */
class UTKCORE_EXPORT USerializeToXML
{
public:
    typedef USerializer::ObjectFactory ObjectFactory;

    USerializeToXML();

    QDomDocument document;
    QDomElement docroot;
    // 构造xml文件
    void appendXml(QDomDocument &doc, QDomElement &root, const QVariantMap &data,
                   const QStringList &attributes = QStringList(),
                   bool allPropertiesAreAttributes = false,
                   bool skipEmptyProperties = true);

    ///
    /// \brief appendXml 添加对象到现有的xml中.
    /// \attention 请使用appendXmlNamed输出带对象名称的xml。使用new QObject再包一层以达到含有对象名称做法是有问题的，因为Qt中父对象会管理子对象，会使此关系混乱且不能简单的执行delete object。
    /// \example 这是一个危险的例子，因为不删除pObj会内存泄漏，而删除pObj同时会删除子对象data，要小心处理。倒不如用appendXmlNamed来得稳妥
    ///    foreach(CabinetData *data, lsCabinets)
    ///    {
    ///        QObject* pObj = new QObject;
    ///        data->setParent(pObj);
    ///        serializer.appendXml(pObj, filename);
    ///         ////may be safe do this
    ///         //data->setParent(oldParent);
    ///         //delete pObj;
    ///    }
    /// \param object
    /// \param fileName
    /// \param attributes
    /// \param allPropertiesAreAttributes
    /// \param skipEmptyProperties
    ///
    void appendXml(const QObject* object,
                   const QString &fileName,
                   const QStringList &attributes = QStringList(),
                   bool allPropertiesAreAttributes = false,
                   bool skipEmptyProperties = true);

    ///
    /// \brief appendXmlNamed 与appendXml不同的是，它会把object的名称包涵进来。
    /// \param object
    /// \param fileName
    /// \param attributes
    /// \param allPropertiesAreAttributes
    /// \param skipEmptyProperties
    /// \see appendXml
    ///
    void appendXmlNamed(const QObject* object,
                        const QString &fileName,
                        const QStringList &attributes = QStringList(),
                        bool allPropertiesAreAttributes = false,
                        bool skipEmptyProperties = true);

    // 解析XML
    QVariantMap parseXml(const QDomElement &root,
                         const ObjectFactory &factory = ObjectFactory());

    // 保存文件
    ///
    /// \brief saveXml
    /// \param object
    /// \param fileName
    /// \param attributes 节点属性列表
    /// \param allPropertiesAreAttributes
    /// \param skipEmptyProperties
    ///
    void saveXml(const QObject* object, const QString &fileName,
                 const QStringList &attributes = QStringList(),
                 bool allPropertiesAreAttributes = false,
                 bool skipEmptyProperties = true);

    ///
    /// \brief loadXml
    /// \param object
    /// \param fileName
    /// \param factory
    ///
    void loadXml(QObject* object, const QString &fileName,
                 const ObjectFactory &factory = ObjectFactory());

    // void loadXml的变形
    QObject* loadXml(const QString &fileName,
                     const ObjectFactory &factory = ObjectFactory());
};

class UTKCORE_EXPORT USerializeToJson
{
public:
    typedef USerializer::ObjectFactory ObjectFactory;

    static void saveJson(const QObject* object, const QString &fileName,
                         QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Compact);
    static void saveJson(const QObject* object, QByteArray &dataArray,
                         QJsonDocument::JsonFormat format = QJsonDocument::JsonFormat::Compact);

    static void loadJson(QObject* object, const QString &fileName,
                         const ObjectFactory &factory = ObjectFactory());
    static void loadJson(QObject* object, const QByteArray data,
                         const ObjectFactory &factory = ObjectFactory());
};

#endif // SERIALIZER_H
