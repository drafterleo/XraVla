#ifndef CF_MAINWINDOW_H
#define CF_MAINWINDOW_H

#include <QWidget>
#include "cw_pagemanager.h"
#include "cp_testpage.h"
#include "cp_vocabularypage.h"

class CMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CMainWindow(QWidget *parent = 0);
    
signals:
    
public slots:

protected:
    void closeEvent(QCloseEvent *event);

    void readSettings();
    void writeSettings();

private:
    CPageManager *m_pageManager;

    CVocabularyPage *m_vocabularyPage;
    CTestPage *m_testPage;
};

#endif // CF_MAINWINDOW_H
