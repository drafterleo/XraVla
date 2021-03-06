#ifndef CW_COLORMATRIXEDIT_H
#define CW_COLORMATRIXEDIT_H

#include "c_colormatrix.h"
#include "cw_abstractedit.h"
#include "cw_colormatrixpixra.h"
#include "color_wheel.hpp"
#include <QColorDialog>
#include <QList>

using namespace color_widgets;

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

protected slots:
    void colorWheelChanged(const QColor & color);

protected:
    QPoint cellAt(QPoint point);
    bool isCellValid(const QPoint &cell);
    void setCurrentCell(int col, int row);
    void setMatrixSize(int cols, int rows);
    void updateMatrixArea();
    void updateColorWheel();

    void commitModification();
    void undoModification();
    void rgbSums(int & r, int & g, int & b);

    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void leaveEvent(QEvent *);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual bool eventFilter(QObject *target, QEvent *event);

private:
    CColorMatrix matrix;
    QRect matrixArea;
    QPoint activeCell;
    QPoint currentCell;
    QPoint pressedCell;
    int margin;
    CColorMatrixPixra *m_pixra;
    ColorWheel        *colorWheel;

    QColorDialog colorDialog;
    QPoint colorDialogPos;

    QList <CColorMatrix> history;
};

#endif // CW_COLORMATRIXEDIT_H
