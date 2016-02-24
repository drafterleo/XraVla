#include "cw_testengine.h"
#include "cfr_choicetestframe.h"
#include <QPainter>
#include <QResizeEvent>
#include <QtDebug>

CTestEngine::CTestEngine(QWidget *parent) :
    QWidget(parent)
{
    m_frame = 0;
    m_usedMax = 10;
    m_testItemCount = 4;

    m_infoLabel = new QLabel(this);
    m_infoLabel->hide();

    m_statsWidget = new CTestStats(this);
    m_statsWidget->hide();

    m_forwardBtn = new CPushButton(QPixmap(":/images/iconForward.png"), this);
    m_forwardBtn->resize(50, 50);
    m_forwardBtn->setToolTip("Next");
    connect(m_forwardBtn, SIGNAL(clicked()), this, SLOT(nextFrame()));

    m_stopBtn = new CPushButton(QPixmap(":/images/iconStop.png"), this);
    m_stopBtn->resize(50, 50);
    m_stopBtn->setToolTip("Stop");
    connect(m_stopBtn, SIGNAL(clicked()), this, SLOT(stopTest()));

    m_retryBtn = new CPushButton(QPixmap(":/images/iconRetry.png"), this);
    connect(m_retryBtn, SIGNAL(clicked()), this, SLOT(startTest()));
    m_retryBtn->resize(50, 50);
    m_retryBtn->hide();

    m_showSpecCBox = new CStyledCheckBox("Show Description", this);
    m_showSpecCBox->setChecked(true);
    connect(m_showSpecCBox, SIGNAL(clicked(bool)), SLOT(showItemSpec(bool)));
    //m_showSpecCBox->resize(200, 50);
}

CTestEngine::~CTestEngine()
{
    clearVocabulary();
    m_restItems.clear();
    m_usedItems.clear();
}

void CTestEngine::addItem(CXravlasteItem *item)
{
    if (item) {
        m_vocabulary.append(item);
    }
}

void CTestEngine::clearVocabulary()
{
    for (int i = 0; i < m_vocabulary.count(); ++i) {
        delete m_vocabulary[i];
    }
    m_vocabulary.clear();
}

void CTestEngine::placeTestFrame()
{
    int w = 640;
    int h = 400;

    m_infoLabel->setGeometry((width() - w)/2, (height() - h)/2, w, h);

    m_statsWidget->setGeometry((width() - w)/2, (height() - h)/2, w, h);
    m_retryBtn->move(m_statsWidget->geometry().bottomRight() + QPoint(-48, 5));
    m_showSpecCBox->move(width() - 20 - m_showSpecCBox->width(), 10);

    if (m_frame) {
        m_frame->setGeometry((width() - w)/2, (height() - h)/2, w, h);

        m_stopBtn->move(m_frame->geometry().right() - 50,
                        m_frame->geometry().bottom());
        m_forwardBtn->move(m_frame->geometry().right() - 105,
                           m_frame->geometry().bottom());
    }
}


void CTestEngine::resizeEvent(QResizeEvent *event)
{
    placeTestFrame();
}

void CTestEngine::hideTestButtons()
{
    m_forwardBtn->hide();
    m_stopBtn->hide();
    m_showSpecCBox->hide();
}

void CTestEngine::showTestButtons()
{
    m_forwardBtn->show();
    m_stopBtn->show();
    m_showSpecCBox->show();
}

void CTestEngine::startTest()
{
    m_retryBtn->hide();
    m_infoLabel->hide();
    m_statsWidget->hide();

    m_restItems.clear();
    m_usedItems.clear();
    m_results.clear();

    for (int i = 0; i < m_vocabulary.count(); ++i) {
        m_restItems.append(i);
    }

    if (m_vocabulary.count() > 3) {
//        if (m_usedMax > m_vocabulary.count()) {
//            m_usedMax = m_vocabulary.count() / 2;
//        }

        if (m_vocabulary.count() > 8) {
            m_usedMax = m_vocabulary.count() / 2;
        } else {
            m_usedMax = 0;
        }

        nextFrame();
        showTestButtons();
    } else {
        hideTestButtons();
        errorFrame("too few items");
    }
}

void CTestEngine::nextFrame()
{
    delete m_frame;
    CChoiceTestFrame *choiceFrame = new CChoiceTestFrame(this);
    connect(choiceFrame, SIGNAL(testDone(STestResult)), this, SLOT(frameDone(STestResult)));

    QVector <int> tempItems;
    for (int i = 0; i < m_testItemCount; ++i) {
        int attempts = 100;
        while (attempts > 0) {
            int rndIdx = qrand() % m_vocabulary.count();
            if (m_usedItems.indexOf(rndIdx) < 0) {
                if (tempItems.indexOf(rndIdx) < 0) {
                    choiceFrame->addItem(m_vocabulary.at(rndIdx));
                    tempItems.append(rndIdx);
                    break;
                }
            }
            attempts --;
        }
    }
    choiceFrame->setRight(0);
    m_usedItems.append(tempItems.at(0));
    if (m_usedItems.count() > m_usedMax)
        m_usedItems.remove(0);

    choiceFrame->shuffleItems(5);
    m_frame = choiceFrame;

    placeTestFrame();
    m_frame->showSpec(m_showSpecCBox->isChecked());
    m_frame->show();
}

void CTestEngine::frameDone(STestResult result)
{
    m_results.append(result);
    //nextFrame();
}

void CTestEngine::stopTest()
{
    hideTestButtons();
    statsFrame();
}


void CTestEngine::errorFrame(const QString &msg)
{
    delete m_frame;
    m_frame = 0;

    m_infoLabel->setText(msg);
    m_infoLabel->setFrameShape(QFrame::Box);
    m_infoLabel->setStyleSheet("color: #D92731;");
    m_infoLabel->setAlignment(Qt::AlignCenter);
    m_infoLabel->setFont(QFont("Arial", 16));
    m_infoLabel->show();
}

void CTestEngine::statsFrame()
{
    delete m_frame;
    m_frame = 0;

    int right = 0;

    for (int i = 0; i < m_results.count(); ++i) {
        if (m_results.at(i).isCorrect)
            right ++;
    }

    m_statsWidget->setRightCount(right);
    m_statsWidget->setWrongCount(m_results.count() - right);

    m_statsWidget->show();
    m_retryBtn->show();
}

void CTestEngine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.end();
}

void CTestEngine::showItemSpec(bool xu)
{
    if (m_frame) {
        m_frame->showSpec(xu);
    }
}

