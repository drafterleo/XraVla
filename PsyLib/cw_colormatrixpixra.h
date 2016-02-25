#ifndef CCOLORMATRIXPIXRA_H
#define CCOLORMATRIXPIXRA_H

#include <QWidget>
#include <QDomElement>
#include <QXmlStreamWriter>
#include "cw_abstractpixra.h"
#include "c_colormatrix.h"

class CColorMatrixPixra : public CAbstractPixra
{
    Q_OBJECT
public:
    CColorMatrixPixra(QWidget *parent = 0);

    virtual void clear();
    virtual void assign(CAbstractPixra *pixra);

    virtual void readXML(const QDomElement & element) {}
    virtual void writeXML(QXmlStreamWriter & xml) {}

    virtual void setColor(const QColor &) {}
    virtual void setFrameColor(const QColor & color);
    virtual void setBackColor(const QColor & color);

    inline CColorMatrix getMatrix() {return matrix;}
    void setMatrix(const CColorMatrix & cmx);

protected:
    void updateDrawArea();

    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);

private:
    CColorMatrix matrix;
    QRect drawArea;
    int margin;

    QColor m_backColor;
    QColor m_frameColor;
};

#endif // CCOLORMATRIXPIXRA_H
