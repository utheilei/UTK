#ifndef XMLSERIALIZER_H
#define XMLSERIALIZER_H

#include "abstractserializer.h"

#include <QStringList>

class UTKCORE_EXPORT UXmlSerializer: public UAbstractSerializer
{
public:
    UXmlSerializer();
protected:
    virtual bool readData(QIODevice* dev, QVariantMap* data) const;
    virtual bool writeData(QIODevice* dev, const QVariantMap &data) const;
private:
    QStringList m_attributes;
    bool m_allPropertiesAreAttributes = false;
    bool m_skipEmptyProperties = true;
};

#endif // XMLSERIALIZER_H
