#include <QPainter>
#include <QtDebug>
#include "cw_colormatrixpixra.h"

CColorMatrixPixra::CColorMatrixPixra(QWidget *parent)
    :CAbstractPixra(parent)
{
    margin = 0;
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
    drawArea = this->rect().adjusted(0, 0, -6, -6);
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

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#272822"));
    painter.drawRect(this->rect());

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

    painter.setBrush(Qt::NoBrush);
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

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
        matrix = cmPixra->colorMatrix();
        updateDrawArea();
    }
}


