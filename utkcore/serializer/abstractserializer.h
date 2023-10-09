#ifndef UABSTRACTSERIALIZER_H
#define UABSTRACTSERIALIZER_H

#include "../utkcore_global.h"
#include "serializeattribute.h"

#include <QString>
#include <QMetaType>

class QIODevice;
class QByteArray;

class USerializable;
class UTKCORE_EXPORT UAbstractSerializer
{
public:
    UAbstractSerializer(Attributes attr);
    virtual ~UAbstractSerializer();

    /**
     * @brief load 从文件加载数据到 data
     * @param file 源文件
     * @param data 目标
     * @return
     */
    bool load(const QString &file, QVariantMap* data) const;
    /**
     * @brief load 从文本加载数据到 data
     * @param text 文本数据，
     * @param data 目标
     * @return
     */
    bool load(const QByteArray &text, QVariantMap* data) const;

    /**
     * @brief load 从文件加载数据并序列化对象
     * @param file 文件路径
     * @param object 从Serializable派生的对象可以调用成员函数 deserialize（） 执行序列化
     * @param name object对象的名称，如果没有指定则会依次检测objectName,className使用
     * @return
     */
    bool load(const QString &file, USerializable* object, const QString &name = QString()) const;

    /**
     * @brief load 从文本加载数据并序列化对象
     * @param text 文本数据
     * @param object 从Serializable派生的对象可以调用成员函数 deserialize（） 执行序列化
     * @param name object对象的名称，如果没有指定则会依次检测objectName,className使用
     * @return
     */
    bool load(const QByteArray &text, USerializable* object, const QString &name = QString()) const;

    /**
     * @brief load 从IO读取并序列化对象
     * @param dev QIODevice 派生类，可以正常调用打开。内部会调用open(ReadOnly)确保已经打开读取。
     * @param object 从Serializable派生的对象可以调用成员函数 deserialize（） 执行序列化
     * @param name object对象的名称，如果没有指定则会依次检测objectName,className使用
     * @return
     */
    bool load(QIODevice* dev, USerializable* object, const QString &name = QString()) const;

    /**
     * @brief save 序列化map并保存到文件file
     * @param file 文件路径
     * @param data
     * @return
     */

    bool save(const QString &file, const QVariantMap &data) const;

    /**
     * @brief save 序列化对象并保存到文件file
     * @param file 文件路径
     * @param object 从Serializable派生的对象可以调用成员函数 serialize（） 执行序列化
     * @param name object对象的名称，如果没有指定则会依次检测objectName,className使用
     * @return
     */
    bool save(const QString &file, const USerializable* object, const QString &name  = QString()) const;

    /**
     * @brief save 从IO读取并序列化对象
     * @param dev QIODevice 派生类，可以正常调用打开。内部会调用open(WriteOnly)确保已经打开读取。
     * @param object 从Serializable派生的对象可以调用成员函数 serialize（） 执行序列化
     * @param name object对象的名称，如果没有指定则会依次检测objectName,className使用
     * @return
     */
    bool save(QIODevice* dev, const USerializable* object, const QString &name = QString()) const;


    Attributes attribute() const;
    bool testAttribute(Attribute attr) const;
    void setAttribute(Attributes attr);
    void setAttribute(Attribute attr, bool enable);
protected:
    virtual bool readData(QIODevice* dev, QVariantMap* data) const = 0;
    virtual bool writeData(QIODevice* dev, const QVariantMap &data) const = 0;
private:
    Attributes m_attribute;
};

#endif // UABSTRACTSERIALIZER_H
