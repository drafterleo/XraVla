#include <QPainter>
#include <QtDebug>
#include "cw_colormatrixpixra.h"

CColorMatrixPixra::CColorMatrixPixra(QWidget *parent)
    :CAbstractPixra(parent)
{
    margin = 1;
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
