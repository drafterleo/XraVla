#include <qmath.h>
#include <QtDebug>
#include "c_figure.h"

CAbstractFigure::CAbstractFigure()
{
    anchorRadius = 5;
}

int CAbstractFigure::anchorAt(QPointF hit)
{
    for (int i = 0; i < anchors.count(); ++i) {
        qreal dx = hit.x() - anchors[i].x();
        qreal dy = hit.y() - anchors[i].y();
        int dist =  qRound(qSqrt(dx*dx + dy*dy));
        if (dist <= anchorRadius) {
            return i;
        }
    }
    return -1;
}

void CAbstractFigure::moveAnchor(int idx, const QPointF & pos)
{
    if (idx >=0 && idx < anchors.count()) {
        anchors[idx] = pos;
        updatePath();
    }
}

void CAbstractFigure::translate(int dx, int dy)
{
    for (int i = 0; i < anchors.count(); ++i) {
        anchors[i].setX(anchors[i].x() + dx);
        anchors[i].setY(anchors[i].y() + dy);
    }
    updatePath();
}

void CAbstractFigure::transform(const QTransform  & transform)
{
    for (int i = 0; i < anchors.count(); ++i) {
        anchors[i] = transform.map(anchors[i]);
    }
    updatePath();
}

QPainterPath CAbstractFigure::scaledPath(qreal sx, qreal sy)
{
    return QTransform().scale(sx, sy).map(path);
}

QPainterPath CAbstractFigure::transformedPath(const QTransform & transform)
{
    return transform.map(path);
}

void CAbstractFigure::readAnchorsXML(const QDomElement & root)
{
    if (root.tagName() == "figure") {
        QDomNode node = root.firstChild();
        anchors.clear();
        while(!node.isNull()) {
            if(node.isElement()) {
                QDomElement anchorElement = node.toElement();
                if (anchorElement.tagName() == "anchor") {
                    QPointF point;
                    point.setX(anchorElement.attribute("x", "0.0").toFloat());
                    point.setY(anchorElement.attribute("y", "0.0").toFloat());
                    anchors.append(point);
                }
            }
            node = node.nextSibling();
        }
    }
}

void CAbstractFigure::writeAnchorsXML(QXmlStreamWriter & xml)
{
    for (int i = 0; i < anchors.count(); ++i) {
        xml.writeStartElement("anchor");
        xml.writeAttribute("x", QString::number(anchors.at(i).x()));
        xml.writeAttribute("y", QString::number(anchors.at(i).y()));
        xml.writeEndElement();
    }
}


/*********************************************************************************/

CPolygonFigure::CPolygonFigure()
    : CAbstractFigure()
{

}

void CPolygonFigure::addPoint(const QPointF & point)
{
    anchors.append(point);
    updatePath();
}

void CPolygonFigure::updatePath(void)
{
    QPainterPath newPath;
    newPath.addPolygon(QPolygonF(anchors));
    newPath.closeSubpath();

    this->path = newPath;
}

void CPolygonFigure::readXML(const QDomElement &element)
{
    if (element.tagName() == "figure" &&
        element.attribute("type") == "CPolygonFigure")
    {
        readAnchorsXML(element);
        updatePath();
    }
}

void CPolygonFigure::writeXML(QXmlStreamWriter &xml)
{
    xml.writeStartElement("figure");
    xml.writeAttribute("type", "CPolygonFigure");
    writeAnchorsXML(xml);
    xml.writeEndElement();
}

CAbstractFigure * CPolygonFigure::clone()
{
    CPolygonFigure *fig = new CPolygonFigure();
    *fig = *this;
    return fig;
}

void CPolygonFigure::randomize(const QRectF &bounds, qreal minDist)
{
    for (int i = 0; i < anchors.count(); ++i) {
        int tryCharge = 100;
        while (tryCharge > 0) {
            int wk = static_cast <int> (bounds.width());
            int hk = static_cast <int> (bounds.height());
            QPointF point = QPoint((qrand() % wk) + bounds.left(),
                                   (qrand() % hk) + bounds.top());
            bool pass = true;
            for (int j = 0; j < i; ++j) {
                qreal dx = anchors[j].x() - point.x();
                qreal dy = anchors[j].y() - point.y();
                qreal dist = qSqrt(dx*dx + dy*dy);
                if (dist < minDist) {
                    tryCharge --;
                    pass = false;
                    break;
                }
            }
            if (pass) {
                anchors[i] = point;
                break;
            }
        }

    }
    updatePath();
}

/*********************************************************************************/


CRectangleFigure::CRectangleFigure(const QRectF & rect)
    : CAbstractFigure()
{
    setRect(rect);
}

void CRectangleFigure::setRect(const QRectF & rect)
{
    anchors.clear();
    anchors.append(rect.topLeft());
    anchors.append(rect.bottomRight());
    updatePath();
}

void CRectangleFigure::updatePath(void)
{
    if (anchors.count() == 2){
        QPainterPath newPath;
        newPath.addRect(QRectF(anchors.at(0), anchors.at(1)));
        newPath.closeSubpath();

        this->path = newPath;
    }
}

void CRectangleFigure::readXML(const QDomElement &element)
{
    if (element.tagName() == "figure" &&
        element.attribute("type") == "CRectangleFigure")
    {
        readAnchorsXML(element);
        updatePath();
    }
}

void CRectangleFigure::writeXML(QXmlStreamWriter &xml)
{
    xml.writeStartElement("figure");
    xml.writeAttribute("type", "CRectangleFigure");
    writeAnchorsXML(xml);
    xml.writeEndElement();
}


CAbstractFigure * CRectangleFigure::clone()
{
    CRectangleFigure *fig = new CRectangleFigure();
    *fig = *this;
    return fig;
}

void CRectangleFigure::randomize(const QRectF &bounds, qreal minDist)
{
    int wk = static_cast <int> (bounds.width());
    int hk = static_cast <int> (bounds.height());

    int tryCharge = 100;
    while (tryCharge > 0) {
        QPointF point1 = QPoint((qrand() % wk) + bounds.left(),
                                (qrand() % hk) + bounds.top());
        QPointF point2 = QPoint((qrand() % wk) + bounds.left(),
                                (qrand() % hk) + bounds.top());

        if (qAbs(point1.x() - point2.x()) > minDist && qAbs(point1.y() - point2.y()) > minDist) {
            setRect(QRectF(point1, point2));
            break;
        } else {
            tryCharge --;
        }
    }
}

/*********************************************************************************/

CEllipseFigure::CEllipseFigure(const QRectF & rect)
    : CAbstractFigure()
{
    setRect(rect);
}

void CEllipseFigure::setRect(const QRectF & rect)
{
    anchors.clear();
    anchors.append(QPointF(rect.center().x(), rect.top()));
    anchors.append(QPointF(rect.right(), rect.center().y()));
    anchors.append(QPointF(rect.center().x(), rect.bottom()));
    anchors.append(QPointF(rect.left(), rect.center().y()));
    updatePath();
}

void CEllipseFigure::moveAnchor(int idx, const QPointF & pos)
{
    if (idx >=0 && idx < anchors.count()) {
        qreal dx = pos.x() - anchors[idx].x();
        qreal dy = pos.y() - anchors[idx].y();
        if (idx == 0) {
            anchors[1] += QPointF(0, dy/2);
            anchors[3] += QPointF(0, dy/2);
            anchors[0] += QPointF(0, dy);
        } else
        if (idx == 2) {
            anchors[1] += QPointF(0, dy/2);
            anchors[3] += QPointF(0, dy/2);
            anchors[2] += QPointF(0, dy);
        } else
        if (idx == 1) {
            anchors[0] += QPointF(dx/2, 0);
            anchors[2] += QPointF(dx/2, 0);
            anchors[1] += QPointF(dx, 0);
        } else
        if (idx == 3) {
            anchors[0] += QPointF(dx/2, 0);
            anchors[2] += QPointF(dx/2, 0);
            anchors[3] += QPointF(dx, 0);
        }

        //anchors[idx] = pos;
        updatePath();
    }
}

void CEllipseFigure::updatePath(void)
{
    if (anchors.count() == 4){
        QPainterPath newPath;
        newPath.addEllipse(QRectF(QPointF(anchors[3].x(), anchors[0].y()),
                                  QPointF(anchors[1].x(), anchors[2].y())));
        newPath.closeSubpath();

        this->path = newPath;
    }
}

void CEllipseFigure::readXML(const QDomElement &element)
{
    if (element.tagName() == "figure" &&
        element.attribute("type") == "CEllipseFigure")
    {
        readAnchorsXML(element);
        updatePath();
    }
}

void CEllipseFigure::writeXML(QXmlStreamWriter &xml)
{
    xml.writeStartElement("figure");
    xml.writeAttribute("type", "CEllipseFigure");
    writeAnchorsXML(xml);
    xml.writeEndElement();
}


CAbstractFigure *CEllipseFigure::clone()
{
    CEllipseFigure *fig = new CEllipseFigure();
    *fig = *this;
    return fig;
}

void CEllipseFigure::randomize(const QRectF &bounds, qreal minDist)
{
    int wk = static_cast <int> (bounds.width());
    int hk = static_cast <int> (bounds.height());

    int tryCharge = 100;
    while (tryCharge > 0) {
        QPointF point1 = QPoint((qrand() % wk) + bounds.left(),
                                (qrand() % hk) + bounds.top());
        QPointF point2 = QPoint((qrand() % wk) + bounds.left(),
                                (qrand() % hk) + bounds.top());

        if (qAbs(point1.x() - point2.x()) > minDist && qAbs(point1.y() - point2.y()) > minDist) {
            setRect(QRectF(point1, point2));
            break;
        } else {
            tryCharge --;
        }
    }
}
