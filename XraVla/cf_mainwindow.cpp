#include "cf_mainwindow.h"
#include <QGridLayout>
#include <QCloseEvent>
#include <cw_styledmessagebox.h>

CMainWindow::CMainWindow(QWidget *parent) :
    QWidget(parent)
{

    setMinimumSize(800, 600);

    m_vocabularyPage = new CVocabularyPage;
    m_testPage = new CTestPage;
    m_testPage->setVocabularyPage(m_vocabularyPage);

    m_pageManager = new CPageManager(this);
    m_pageManager->addPage("Vocabulary", QPixmap(":/images/iconVocabulary.png"), m_vocabularyPage);
    m_pageManager->addPage("Test", QPixmap(":/images/iconTest.png"), m_testPage);

    QGridLayout *layout = new QGridLayout;
    layout->setMargin(0);
    layout->addWidget(m_pageManager);
    this->setLayout(layout);
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    if (m_vocabularyPage->isModified()) {
//        QMessageBox::StandardButton reply;
//        reply = QMessageBox::critical(this, tr("XraVla"),
//                                      "Vocabulary has not been saved. Save it?",
//                                      QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Close);

        CStyledMessageBox msgBox;
        msgBox.setText("Vocabulary has not been saved.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int reply = msgBox.exec();

        if (reply == QMessageBox::Save) {
            m_vocabularyPage->saveItems();
            event->accept();
        } else
        if (reply == QMessageBox::Cancel) {
            event->ignore();
        } else {
            event->accept();
        }
    } else
        event->accept();

}
