#ifndef CFR_ABSTRACTTESTFRAME_H
#define CFR_ABSTRACTTESTFRAME_H

#include <QWidget>
#include "c_testresult.h"

class CAbstractTestFrame : public QWidget
{
    Q_OBJECT
public:
    explicit CAbstractTestFrame(QWidget *parent = 0);

    virtual void showSpec(bool xu) = 0;
    
signals:
    void testDone(STestResult result);
    
public slots:
    
};

#endif // CFR_ABSTRACTTESTFRAME_H
