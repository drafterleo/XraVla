#ifndef CW_COLORMATRIXEDIT_H
#define CW_COLORMATRIXEDIT_H

#include "c_colormatrix.h"
#include "cw_abstractedit.h"

class CColorMatrixEdit : public CAbstractEdit
{
    Q_OBJECT
public:
    explicit CColorMatrixEdit(QWidget *parent = 0);

    inline void setMatrix(const CColorMatrix & cm) {matrix = cm;}
    inline const CColorMatrix & getMatrix() const {return matrix;}

    void fillRandom(void);
    inline void setColor(int col, int row, const QColor & color) {matrix.setColor(col, row, color);}

    virtual void clear() {}
    virtual void clearHIstory(void) {}
    virtual CAbstractPixra *pixra(void) {}
    virtual bool assignPixra(CAbstractPixra *pixra) {return false;}
    virtual void randomize() {}

protected:
    QPoint cellAt(QPoint point);

    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    CColorMatrix matrix;
    QPoint activeCell;
};

#endif // CW_COLORMATRIXEDIT_H
