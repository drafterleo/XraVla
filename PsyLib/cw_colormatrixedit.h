#ifndef CW_COLORMATRIXEDIT_H
#define CW_COLORMATRIXEDIT_H

#include "c_colormatrix.h"
#include "cw_abstractedit.h"
#include "cw_colormatrixpixra.h"

class CColorMatrixEdit : public CAbstractEdit
{
    Q_OBJECT
public:
    explicit CColorMatrixEdit(QWidget *parent = 0);

    ~CColorMatrixEdit();

    void setColorMatrix(const CColorMatrix & cm);
    inline const CColorMatrix & colorMatrix() const {return matrix;}

    void fillRandom(void);
    inline void setColor(int col, int row, const QColor & color) {matrix.setColor(col, row, color);}

    virtual void clear();
    virtual void clearHIstory(void) {}
    virtual CAbstractPixra *pixra(void);
    virtual bool assignPixra(CAbstractPixra *pixra);
    virtual void randomize() {fillRandom();}

protected:
    QPoint cellAt(QPoint point);
    void updateDrawArea();

    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *event);

private:
    CColorMatrix matrix;
    QRect drawArea;
    QPoint activeCell;
    int margin;
    CColorMatrixPixra *m_pixra;
};

#endif // CW_COLORMATRIXEDIT_H
