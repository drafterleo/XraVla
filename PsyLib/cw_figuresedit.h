#ifndef CW_FIGURESEDIT_H
#define CW_FIGURESEDIT_H

#include <QWidget>
#include <QStack>
#include "c_figure.h"
#include "cw_abstractedit.h"
#include "cw_figurespixra.h"

class CFiguresEdit: public CAbstractEdit
{
    Q_OBJECT
public:
    CFiguresEdit(QWidget * parent = 0);
    ~CFiguresEdit();

    void addFigure(CAbstractFigure *figure);
    void addRandomPolygon(int n, int r = 0);

    void clear();
    void clearHIstory();
    CAbstractPixra *pixra(void);
    bool assignPixra(CAbstractPixra *pixra);
    void randomize();

protected:
    bool writeToPixra(CAbstractPixra *pixra);
    bool readFromPixra(CAbstractPixra *pixra);

    bool event(QEvent *event);

    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);

    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

    void clearFigures();
    void translateFigure(int figIdx, int dx, int dy);
    void translateSelection(int dx, int dy, bool saveHistory = true);
    void scaleFigure(int figIdx, qreal sx, qreal sy);
    void rotateFigure(int figIdx, qreal angle);

    void addSelectCluster(int idx);
    void deleteFigure(int idx);

    bool isFigureSelected(int idx);

    void raiseFigure(int idx);

    void contentChanged();

    void addHistory();
    void undo();

public slots:
    void addRectangle();
    void addPolygon(int anchors);
    void addEllipse();

private:
    QVector <CAbstractFigure *> figures;

    QStack <CFiguresPixra *> m_history;

    CFiguresPixra *m_pixra;
    CFiguresPixra *m_clipboardPixra;

    int selectedFigure;
    QVector <int> selCluster;

    int hotFigure;
    int catchedFigure;
    int catchedAnchor;
    int hotAnchor;

    bool isMousePressed;
    QPoint pressPoint;

    int anchorRadius;
};

#endif // CW_FIGURESEDIT_H
