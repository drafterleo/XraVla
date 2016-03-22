#include "cw_figurespixra.h"
#include <QPainter>
#include <QtDebug>
#include <qmath.h>

CFiguresPixra::CFiguresPixra(QWidget *parent) :
    CAbstractPixra(parent)
{
    m_backColor = QColor(0x272822);
    m_frameColor = QColor(0x466A7E);
    m_figureColor = Qt::white;
    m_margin = 3;
    m_figureWidth = 2.0;
}

CFiguresPixra::~CFiguresPixra()
{
    clearFigures();
}

void CFiguresPixra::clearFigures()
{
    for (int i = 0; i < m_normFigures.count(); ++i) {
        delete m_normFigures[i];
    }
    m_normFigures.clear();
}

void CFiguresPixra::clear()
{
    clearFigures();
}

void CFiguresPixra::assign(CAbstractPixra *pixra)
{
    CFiguresPixra *figPixra = dynamic_cast <CFiguresPixra *>(pixra);
    if (figPixra) {
        clearFigures();
        for (int i = 0; i < figPixra->normFigures().count(); ++i) {
            CAbstractFigure *fig = figPixra->normFigures().at(i)->clone();
            m_normFigures.append(fig);
        }
        update();
    }
}

void CFiguresPixra::addNormFigure(CAbstractFigure *figure)
{
    if (figure) {
        m_normFigures.append(figure);
        update();
    }
}


void CFiguresPixra::resizeEvent(QResizeEvent *)
{
    if(this->width() < 150) {
        m_figureWidth = 2.0;
    } else {
        m_figureWidth = 3.0;
    }

}


void CFiguresPixra::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QPen(QBrush(QColor(m_frameColor)), 2.0, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    painter.setBrush(m_backColor);
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    for (int i = 0; i < m_normFigures.count(); ++i) {
        /* draw figure */
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(QBrush(m_figureColor), m_figureWidth));

        QTransform transform = QTransform().translate(m_margin, m_margin)
                                           .scale(width() - 2*m_margin, height() - 2*m_margin);
        painter.drawPath(m_normFigures.at(i)->transformedPath(transform));
    }

    painter.end();
}

void CFiguresPixra::setFigureColor(const QColor & color)
{
    m_figureColor = color;
    this->update();
}

void CFiguresPixra::setColor(const QColor & color)
{
    setFigureColor(color);
}

void CFiguresPixra::setFrameColor(const QColor &color)
{
    m_frameColor = color;
    this->update();
}

void CFiguresPixra::setBackColor(const QColor & color)
{
    m_backColor = color;
    this->update();
}

void CFiguresPixra::readXML(const QDomElement &element)
{
    if (element.tagName() == "pixra" &&
        element.attribute("type") == metaObject()->className())
    {
        m_normFigures.clear();
        QDomNode node = element.firstChild();
        while(!node.isNull()) {
            if (node.isElement()) {
                QDomElement figElement = node.toElement();
                if (figElement.tagName() == "figure") {
                    CAbstractFigure *figure = figuresFactoryInstance.createFigure(figElement.attribute("type"));
                    if (figure) {
                        figure->readXML(figElement);
                        m_normFigures.append(figure);
                    }
                }
            }
            node = node.nextSibling();
        }
    }
}

void CFiguresPixra::writeXML(QXmlStreamWriter &xml)
{
    xml.writeStartElement("pixra");
    xml.writeAttribute("type", metaObject()->className());
    for (int i = 0; i < m_normFigures.count(); ++i) {
        m_normFigures.at(i)->writeXML(xml);
    }
    xml.writeEndElement();
}

void CFiguresPixra::addRectangle(const QRectF boundRect)
{
    addNormFigure(new CRectangleFigure(boundRect));
}

void CFiguresPixra::addEllipse(const QRectF boundRect)
{
    addNormFigure(new CEllipseFigure(boundRect));
}

void CFiguresPixra::addPolygon(int n, const QRectF boundRect)
{
    if (n > 2) {
        CPolygonFigure *polygon = new CPolygonFigure;
        qreal pi = 3.14159265358979323;
        qreal hr = boundRect.width()/2;
        qreal vr = boundRect.height()/2;
        qreal angle = 3 * pi/2;
        qreal angleStep = 2 * pi/n;
        for (int i = 0; i < n; ++i) {
            polygon->addPoint(QPointF(boundRect.center()) +
                              QPointF(hr * qCos(angle), vr * qSin(angle)));
            angle += angleStep;
        }
        this->addNormFigure(polygon);
    }
}
