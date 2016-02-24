#ifndef C_XRAVLASTEITEM_H
#define C_XRAVLASTEITEM_H

#include "cw_figurespixra.h"
#include <QVariant>
#include <QDomElement>

class CXravlasteItem
{
public:
    CXravlasteItem(const QString & aWord = QString(), const QString & aSpec = QString(), CAbstractPixra *aPixra = 0);
    ~CXravlasteItem();

    void assign(CXravlasteItem *other);
    void setPixra(CAbstractPixra *aPixra);

    void readXML(const QDomElement & element);
    void writeXML(QXmlStreamWriter & xml);

    QString word;
    QString spec;
    CAbstractPixra *pixra;
};

Q_DECLARE_METATYPE(CXravlasteItem *)
Q_DECLARE_METATYPE(CXravlasteItem)

#endif // C_XRAVLASTEITEM_H
