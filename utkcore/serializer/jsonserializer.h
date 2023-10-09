#ifndef UJSONSERIALIZER_H
#define UJSONSERIALIZER_H

#include "abstractserializer.h"

class UTKCORE_EXPORT UJsonSerializer : public UAbstractSerializer
{
public:
    UJsonSerializer();
    void setIndented(bool indent);
protected:
    virtual bool readData(QIODevice* dev, QVariantMap* data) const;
    virtual bool writeData(QIODevice* dev, const QVariantMap &data) const;
private:
    bool m_indented = false;
};

#endif // UJSONSERIALIZER_H
