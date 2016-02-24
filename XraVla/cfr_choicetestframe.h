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

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

    void updateRects();

    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QVector <CXravlasteItem *> m_items;
    int m_rightIdx;

    QRect m_rightRect;
    QVector <QRect> m_pixraRects;

    int m_hotIdx;
    int m_clickIdx;

    QFont m_wordFont;
    QFont m_specFont;

    bool m_testMode;
    bool m_showSpec;
};

#endif // CFR_CHOICETESTFRAME_H
