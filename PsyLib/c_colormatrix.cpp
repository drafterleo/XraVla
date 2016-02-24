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
        colors.fill(QColor(Qt::white), w * h);
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

void CColorMatrix::setColor(int col, int row, const QColor & color)
{
    int idx = col + row * cols;
    if (idx >= 0 && idx < colors.size()) {
        colors[idx] = color;
    }
}
