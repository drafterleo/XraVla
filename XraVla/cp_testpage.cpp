#include "cp_testpage.h"
#include <QtDebug>
#include <QResizeEvent>

CTestPage::CTestPage(QWidget *parent) :
    CPageWidget(parent)
{
    m_vocPage = 0;
    m_testEngine = new CTestEngine(this);
}

void CTestPage::resizeEvent(QResizeEvent *event)
{
    m_testEngine->move(0, 0);
    m_testEngine->resize(event->size());
}

void CTestPage::setVocabularyPage(CVocabularyPage *vocPage)
{
    m_vocPage = vocPage;
}

void CTestPage::enterPage()
{
    m_testEngine->clearVocabulary();

    /* copy vocabulary */
    if (m_vocPage && m_vocPage->model()) {
        for (int i = 0; i < m_vocPage->model()->rowCount(); ++i) {
            CXravlasteItem *xitem = new CXravlasteItem;
            xitem->assign(m_vocPage->model()->data(m_vocPage->model()->index(i, 0)).value<CXravlasteItem *>());
            m_testEngine->addItem(xitem);
        }

        m_testEngine->startTest();
    }
}
