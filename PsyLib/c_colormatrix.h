#ifndef C_COLORMATRIX_H
#define C_COLORMATRIX_H

#include <QColor>
#include <QVector>
#include <QSize>
#include <QPoint>

class CColorMatrix
{
public:
    CColorMatrix(int w, int h);
    CColorMatrix();

    void setSize(int w, int h);
    QSize size() {return QSize(cols, rows);}
    inline int colCount() const {return cols;}
    inline int rowCount() const {return rows;}

    inline bool isEmpty(void) {return cols == 0 || rows == 0;}

    QColor getColor(int idx) const;
    QColor getColor(int col, int row) const;
    QColor getColor(const QPoint &cell) const;
    void   setColor(int idx, const QColor & color);
    void   setColor(int col, int row, const QColor & color);
    void   setColor(const QPoint & cell, const QColor & color);

private:
    QVector <QColor> colors;

    int cols;
    int rows;
};

#endif // C_COLORMATRIX_H
