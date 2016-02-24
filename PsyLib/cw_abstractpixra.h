#ifndef CW_ABSTRACTPIXRA_H
#define CW_ABSTRACTPIXRA_H

#include <QWidget>
#include <QDomElement>
#include <QXmlStreamWriter>

class CAbstractPixra : public QWidget
{
    Q_OBJECT
public:
    explicit CAbstractPixra(QWidget *parent = 0);

    virtual void clear() = 0;
    virtual void assign(CAbstractPixra *pixra) = 0;

    virtual void readXML(const QDomElement & element) = 0;
    virtual void writeXML(QXmlStreamWriter & xml) = 0;

    virtual void setColor(const QColor & color) = 0;
    virtual void setFrameColor(const QColor & color) = 0;
    virtual void setBackColor(const QColor & color) = 0;
};

#endif // CW_ABSTRACTPIXRA_H
