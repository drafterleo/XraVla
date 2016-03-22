#include <QPainter>
#include <QtDebug>
#include "cw_colormatrixpixra.h"

CColorMatrixPixra::CColorMatrixPixra(QWidget *parent)
    :CAbstractPixra(parent)
{
    margin = 2;
    m_backColor = QColor(0x272822);
    m_frameColor = QColor(0x466A7E);
}

void CColorMatrixPixra::setMatrix(const CColorMatrix & cmx)
{
    matrix = cmx;
    updateDrawArea();
}

void CColorMatrixPixra::resizeEvent(QResizeEvent *)
{
    updateDrawArea();
}

void CColorMatrixPixra::updateDrawArea()
{
    drawArea = this->rect().adjusted(0, 0, -10, -10);
    if (drawArea.width() > drawArea.height()) {
        drawArea.setWidth(drawArea.height());
    } else {
        drawArea.setHeight(drawArea.width());
    }
    if (drawArea.width() < 100) {
        margin = 1;
    } else {
        margin = 2;
    }
    int dx = this->rect().width() - drawArea.width();
    int dy = this->rect().height() - drawArea.height();

   int ddx = 0;
   int ddy = 0;
   if (!matrix.isEmpty()) {
    if (drawArea.width() % matrix.colCount())
        ddx = 1;
    if (drawArea.height() % matrix.rowCount())
        ddy = 1;
   }
    drawArea.translate(dx/2 + ddx, dy/2 + ddy);

    update();
}

void CColorMatrixPixra::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(QPen(QBrush(QColor(m_frameColor)), 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
    painter.setBrush(m_backColor);
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    int w = drawArea.width() / matrix.colCount();
    int h = drawArea.height() / matrix.rowCount();
    int dx = drawArea.left();
    int dy = drawArea.top();

    painter.setPen(QPen(Qt::black, 2));
    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            painter.setBrush(matrix.getColor(col, row));
            QRect cellRect = QRect(col * w + dx, row * h + dy, w, h).adjusted(margin, margin, -margin, -margin);
            painter.drawRect(cellRect);
        }
    }

    painter.end();
}

void CColorMatrixPixra::clear()
{
    for (int col = 0; col < matrix.colCount(); ++ col)
        for (int row = 0; row < matrix.rowCount(); ++row) {
            matrix.setColor(col, row, Qt::white);
        }
    this->update();
}

void CColorMatrixPixra::assign(CAbstractPixra *pixra)
{
    //qDebug() << "CColorMatrixPixra::assign";
    CColorMatrixPixra *cmPixra = dynamic_cast <CColorMatrixPixra *>(pixra);
    if (cmPixra) {
        matrix = cmPixra->getMatrix();
        updateDrawArea();
    }
}

void CColorMatrixPixra::setFrameColor(const QColor & color)
{
    m_frameColor = color;
    update();
}

void CColorMatrixPixra::setBackColor(const QColor & color)
{
    m_backColor = color;
    update();
}

void CColorMatrixPixra::readXML(const QDomElement & element)
{
    if (element.tagName() == "pixra" &&
        element.attribute("type") == metaObject()->className())
    {
        QDomNode node = element.firstChild();
        if (node.isElement()) {
            QDomElement matrixElement = node.toElement();
            if (matrixElement.tagName() == "matrix") {
                int cols = matrixElement.attribute("cols", "1").toInt();
                int rows = matrixElement.attribute("rows", "1").toInt();
                matrix = CColorMatrix(cols, rows);
                int idx = 0;
                QDomNode colorNode = matrixElement.firstChild();
                while(!colorNode.isNull()) {
                    if (colorNode.isElement()) {
                        QDomElement colorElement = colorNode.toElement();
                        if (colorElement.tagName() == "color") {
                            QColor color = QColor(colorElement.attribute("rgb", "#FFFFFF"));
                            matrix.setColor(idx, color);
                            idx++;
                        }
                    }
                    colorNode = colorNode.nextSibling();
                }
            }
        }
    }
}

void CColorMatrixPixra::writeXML(QXmlStreamWriter & xml)
{
    xml.writeStartElement("pixra");
    xml.writeAttribute("type", metaObject()->className());
        xml.writeStartElement("matrix");
        xml.writeAttribute("cols", QString::number(matrix.colCount()));
        xml.writeAttribute("rows", QString::number(matrix.colCount()));
        for (int r = 0; r < matrix.rowCount(); ++r)
            for (int c = 0; c < matrix.colCount(); ++c) {
                xml.writeStartElement("color");
                xml.writeAttribute("rgb", matrix.getColor(c, r).name());
                xml.writeEndElement();
            }
        xml.writeEndElement();
    xml.writeEndElement();
}
