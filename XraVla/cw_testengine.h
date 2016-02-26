#ifndef CW_TESTENGINE_H
#define CW_TESTENGINE_H

#include <QWidget>
#include <QLabel>

#include "c_xravlasteitem.h"
#include "c_testresult.h"
#include "cw_pushbutton.h"
#include "cw_teststats.h"
#include "cfr_abstracttestframe.h"
#include "cw_styledcheckbox.h"

class CTestEngine : public QWidget
{
    Q_OBJECT
public:
    explicit CTestEngine(QWidget *parent = 0);
    ~CTestEngine();

    void addItem(CXravlasteItem *item); // owns items (ca'o ponse la items)
    void clearVocabulary();             // !!! deletes items

public slots:
    void startTest();

protected slots:
    void frameDone(STestResult result);
    void nextFrame();
    void stopTest();
    void showItemSpec(bool xu);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *);

    void placeTestFrame();

    void errorFrame(const QString & msg);
    void statsFrame();

    void hideTestButtons();
    void showTestButtons();


private:
    QVector <CXravlasteItem *> m_vocabulary;

    QVector <int> m_restItems;
    QVector <int> m_usedItems;

    int m_usedMax;
    int m_testItemCount;

    QVector <STestResult> m_results;

    CAbstractTestFrame *m_frame;
    QLabel *m_infoLabel;
    CTestStats *m_statsWidget;

    CPushButton *m_forwardBtn;
    CPushButton *m_stopBtn;
    CPushButton *m_retryBtn;

    CStyledCheckBox *m_showSpecCBox;
};

#endif // CW_TESTENGINE_H
