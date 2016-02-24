#ifndef C_FIGURE_H
#define C_FIGURE_H

#include <QPoint>
#include <QHash>
#include <QPainterPath>
#include <QTransform>
#include <QDomElement>
#include <QXmlStreamWriter>

class CAbstractFigure
{
public:
    CAbstractFigure();

    virtual void moveAnchor(int idx, const QPointF & pos);
    virtual void translate(int dx, int dy);
    virtual void transform(const QTransform & transform);
    virtual void randomize(const QRectF & bounds, qreal minDist) = 0;
    virtual void readXML(const QDomElement & element) = 0;
    virtual void writeXML(QXmlStreamWriter & xml) = 0;

    virtual CAbstractFigure * clone() = 0;

    int anchorAt(QPointF hit);
    inline QPointF getAnchor(int idx) const {return anchors.value(idx, QPointF());}
    inline const QVector <QPointF> & getAnchors() const {return anchors;}

    inline const QPainterPath & getPath() const {return path;}
    QPainterPath scaledPath(qreal sx, qreal sy);
    QPainterPath transformedPath(const QTransform & transform);

    inline int  getAnchorRadius() const {return anchorRadius;}
    inline void setAnchorRadius(int radius) {anchorRadius = radius;}


protected:
    virtual void updatePath() = 0;
    void readAnchorsXML(const QDomElement & root);
    void writeAnchorsXML(QXmlStreamWriter & xml);

    QPainterPath path;
    QVector <QPointF> anchors;
    int anchorRadius;
};


class CPolygonFigure : public CAbstractFigure
{
public:
    CPolygonFigure();

    void addPoint(const QPointF & point);

    void readXML(const QDomElement & element);
    void writeXML(QXmlStreamWriter & xml);
    CAbstractFigure * clone();
    void randomize(const QRectF &bounds, qreal minDist);

protected:
    virtual void updatePath();
};


class CRectangleFigure : public CAbstractFigure
{
public:
    CRectangleFigure(const QRectF & rect = QRectF());

    void setRect(const QRectF & rect);

    void readXML(const QDomElement & element);
    void writeXML(QXmlStreamWriter & xml);
    CAbstractFigure * clone();
    void randomize(const QRectF &bounds, qreal minDist);
protected:
    virtual void updatePath();
};

class CEllipseFigure : public CAbstractFigure
{
public:
    CEllipseFigure(const QRectF & rect = QRectF());

    void setRect(const QRectF & rect);

    void moveAnchor(int idx, const QPointF & pos);
    void readXML(const QDomElement & element);
    void writeXML(QXmlStreamWriter & xml);
    CAbstractFigure * clone();
    void randomize(const QRectF &bounds, qreal minDist);
protected:
    virtual void updatePath();
};


#endif // C_FIGURE_H
