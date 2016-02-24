#ifndef CW_TESTSTATS_H
#define CW_TESTSTATS_H

#include <QWidget>
#include "cw_pushbutton.h"

class CTestStats : public QWidget
{
    Q_OBJECT
public:
    explicit CTestStats(QWidget *parent = 0);

    void setRightCount(int right);
    void setWrongCount(int wrong);

signals:
    void retryTest();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_right;
    int m_wrong;

};

#endif // CW_TESTSTATS_H
