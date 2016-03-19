#ifndef CFR_CHOICETESTFRAME_H
#define CFR_CHOICETESTFRAME_H

#include "cfr_abstracttestframe.h"
#include "c_xravlasteitem.h"
#include "c_testresult.h"
#include "cw_pushbutton.h"

class CChoiceTestFrame : public CAbstractTestFrame
{
    Q_OBJECT
public:
    explicit CChoiceTestFrame(QWidget *parent = 0);
    ~CChoiceTestFrame();

    void addItem(CXravlasteItem *item, bool right = false);
    void setRight(int itemIdx = 0);

    void shuffleItems(int n);
    void showSpec(bool xu);

signals:
    void needEditItem(int idx);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);

    void updateRects();

    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

private:
    QVector <CXravlasteItem *> m_items;
    int m_rightIdx;

    QRect m_descriptRect;
    QVector <QRect> m_pixraRects;

    int m_hotIdx;
    int m_clickIdx;

    QFont m_wordFont;
    QFont m_specFont;

    bool m_testMode;
    bool m_showSpec;
};

#endif // CFR_CHOICETESTFRAME_H
