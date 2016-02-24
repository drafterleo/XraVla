#ifndef C_COLORMATRIX_H
#define C_COLORMATRIX_H

#include <QColor>
#include <QVector>

class CColorMatrix
{
public:
    CColorMatrix(int w, int h);
    CColorMatrix();

    void setSize(int w, int h);
    inline int colCount() const {return cols;}
    inline int rowCount() const {return rows;}

    inline bool isEmpty(void) {return cols == 0 || rows == 0;}

    QColor getColor(int col, int row) const;
    void   setColor(int col, int row, const QColor & color);

private:
    QVector <QColor> colors;

    int cols;
    int rows;
};

#endif // C_COLORMATRIX_H
