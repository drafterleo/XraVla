#ifndef CW_FIGURESPIXRA_H
#define CW_FIGURESPIXRA_H

#include <QWidget>
#include <QDomElement>
#include <QXmlStreamWriter>
#include "cw_abstractpixra.h"
#include "c_figuresfactory.h"
#include "c_figure.h"

class CFiguresPixra : public CAbstractPixra
{
    Q_OBJECT
public:
    explicit CFiguresPixra(QWidget *parent = 0);
    ~CFiguresPixra();

    void addNormFigure(CAbstractFigure *figure);
    const QVector <CAbstractFigure *> & normFigures() {return m_normFigures;}

    void clear();
    void assign(CAbstractPixra *pixra);
    void readXML(const QDomElement & element);
    void writeXML(QXmlStreamWriter &xml);

    void setColor(const QColor & color);
    void setFrameColor(const QColor &color);
    void setBackColor(const QColor & color);
    void setFigureColor(const QColor & color);

    void addPolygon(int n, const QRectF boundRect);
    void addRectangle(const QRectF boundRect);
    void addEllipse(const QRectF boundRect);

    inline bool isEmpty(void) {return m_normFigures.count() == 0;}

protected:
    void clearFigures();

    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

private:
    QVector <CAbstractFigure *> m_normFigures;

    QColor m_backColor;
    QColor m_figureColor;
    QColor m_frameColor;

    int m_margin;
    qreal m_figureWidth;
};

#endif // CW_FIGURESPIXRA_H
