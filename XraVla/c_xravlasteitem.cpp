#include "c_xravlasteitem.h"
#include "c_pixrafactory.h"
#include <QtDebug>

CXravlasteItem::CXravlasteItem(const QString & aWord, const QString & aSpec, CAbstractPixra *aPixra)
    : name(aWord), spec(aSpec)
{
    if (aPixra) {
        pixra = pixraFactoryInstance.createPixra(aPixra->metaObject()->className());
        pixra->assign(aPixra);
    } else
        pixra = 0;
}

CXravlasteItem::~CXravlasteItem()
{
    delete pixra;
}

void CXravlasteItem::assign(CXravlasteItem *other)
{
    if (other) {
        name = other->name;
        spec = other->spec;

        delete pixra;
        if (other->pixra) {
            pixra = pixraFactoryInstance.createPixra(other->pixra->metaObject()->className());
            pixra->assign(other->pixra);
        } else
            pixra = 0;
    }
}

void CXravlasteItem::setPixra(CAbstractPixra *aPixra)
{
    delete pixra;
    if (aPixra) {
        pixra = pixraFactoryInstance.createPixra(aPixra->metaObject()->className());
        pixra->assign(aPixra);
    } else
        pixra = 0;
}

void CXravlasteItem::readXML(const QDomElement & element)
{
    if (element.tagName() == "item") {
        name = element.firstChildElement("word").text();
        spec = element.firstChildElement("spec").text();
        delete pixra;
        pixra = 0;
        QDomElement pixraElement = element.firstChildElement("pixra");
        if (!pixraElement.isNull()) {
            pixra = pixraFactoryInstance.createPixra(pixraElement.attribute("type")); //new CFiguresPixra;
            if (pixra)
                pixra->readXML(pixraElement);
        }
    }
}

void CXravlasteItem::writeXML(QXmlStreamWriter & xml)
{
    xml.writeStartElement("item");
        xml.writeStartElement("word");
        xml.writeCharacters(name);
        xml.writeEndElement();

        xml.writeStartElement("spec");
        xml.writeCharacters(spec);
        xml.writeEndElement();

        if (pixra)
            pixra->writeXML(xml);
    xml.writeEndElement();
}
