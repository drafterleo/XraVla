#include "cf_mainwindow.h"
#include <QGridLayout>
#include <QCloseEvent>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <cw_styledmessagebox.h>

CMainWindow::CMainWindow(QWidget *parent) :
    QWidget(parent)
{

    setMinimumSize(800, 600);
    setWindowIcon(QPixmap(":/images/XraVla.png"));

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

    readSettings();
}

void CMainWindow::closeEvent(QCloseEvent *event)
{
    writeSettings();

    if (m_vocabularyPage->isModified()) {
        CStyledMessageBox msgBox(this);
        msgBox.setText("Vocabulary has not been saved.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setButtonText(QMessageBox::Cancel, tr("Back"));
        QPixmap msgIcon(":/images/iconWarning.png");
        msgBox.setIconPixmap(msgIcon);

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
    } else {
        event->accept();
    }
}

void CMainWindow::readSettings()
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "XraVla");
    QString fileName = settings.value("File Name", "").toString();
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists()) {
        m_vocabularyPage->loadVocabularyFromFile(fileName, true, 0);
        setWindowTitle(fileInfo.fileName());
        qDebug() << QDir::setCurrent(fileInfo.dir().path());
    } else {
        setWindowTitle("XraVla");
    }
}

void CMainWindow::writeSettings()
{
    QSettings settings(QSettings::UserScope, "DrafterSoft", "XraVla");
    settings.setValue("File Name", m_vocabularyPage->currentFileName());
}
