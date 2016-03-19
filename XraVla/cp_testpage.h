#ifndef CP_TESTPAGE_H
#define CP_TESTPAGE_H

#include <QWidget>
#include "cp_pagewidget.h"
#include "cp_vocabularypage.h"
#include "cw_testengine.h"

class CTestPage : public CPageWidget
{
    Q_OBJECT
public:
    explicit CTestPage(QWidget *parent = 0);
    void enterPage();
    void setVocabularyPage(CVocabularyPage *vocPage);
    
signals:
    void needEditItem(int idx);
    
public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private:
    CVocabularyPage *m_vocPage;
    CTestEngine     *m_testEngine;
};

#endif // CP_TESTPAGE_H
