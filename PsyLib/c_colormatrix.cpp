#include "c_colormatrix.h"
#include "QtDebug"

CColorMatrix::CColorMatrix(int w, int h)
{
    setSize(w, h);
}

CColorMatrix::CColorMatrix()
{
    setSize(0, 0);
}

void CColorMatrix::setSize(int w, int h)
{
    colors.clear();
    if (w > 0 && h > 0) {
        colors.fill(QColor(0xA0A0A0), w * h);
        cols = w;
        rows = h;
    } else {
        cols = 0;
        rows = 0;
    }
}

QColor CColorMatrix::getColor(int col, int row) const
{
    int idx = col + row * cols;
    if (idx >= 0 && idx < colors.size()) {
        return colors.at(idx);
    }
    return Qt::black;
}

QColor CColorMatrix::getColor(const QPoint &cell) const
{
    return getColor(cell.x(), cell.y());
}

void CColorMatrix::setColor(int idx, const QColor & color)
{
    if (idx >= 0 && idx < colors.size()) {
        colors[idx] = color;
    }
}

void CColorMatrix::setColor(int col, int row, const QColor & color)
{
    setColor(col + row * cols, color);
}

void CColorMatrix::setColor(const QPoint & cell, const QColor & color)
{
    setColor(cell.x(), cell.y(), color);
}
