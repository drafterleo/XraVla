#include <QPainter>
#include "cw_colormatrixpixra.h"

CColorMatrixPixra::CColorMatrixPixra()
{

}

void CColorMatrixPixra::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if (matrix.isEmpty()) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(this->rect());
        return;
    }

    int w = width() / matrix.colCount();
    int h = height() / matrix.rowCount();

    painter.setPen(QPen(Qt::black, 2));
    for (int col = 0; col < matrix.colCount(); ++ col) {
        for (int row = 0; row < matrix.rowCount(); ++row) {
            painter.setBrush(matrix.getColor(col, row));
            painter.drawRect(col * w, row * h, w, h);
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
    CColorMatrixPixra *cmPixra = dynamic_cast <CColorMatrixPixra *>(pixra);
    if (cmPixra) {
        matrix = cmPixra->colorMatrix();
        update();
    }
}


