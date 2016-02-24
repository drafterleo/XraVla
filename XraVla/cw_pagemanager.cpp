#include "cw_pagemanager.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>

CPageManager::CPageManager(QWidget *parent) :
    QWidget(parent)
{
    m_barWidth = 80;
    m_pageMargin = 0;

    m_backColor = Qt::black;

    m_pageStack = new QStackedWidget(this);
    m_pageBar = new CPageBar(this);
    connect(m_pageBar, SIGNAL(pageChanged(int)), this, SLOT(setPage(int)));

    setMouseTracking(true);
}

void CPageManager::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    m_pageBar->setGeometry(0, 0, m_barWidth, height());

    m_pageStack->setGeometry(m_pageBar->width() + m_pageMargin,
                             m_pageMargin,
                             width() - (m_pageBar->width() + m_pageMargin * 2),
                             height() - m_pageMargin * 2);
}

void CPageManager::addPage(const QString &name, const QPixmap &pix, CPageWidget *pageWidget)
{
    if (pageWidget) {
        int pageIdx = m_pageStack->addWidget(pageWidget);
        m_pageBar->addPage(pageIdx, name, pix);
    }
}

void CPageManager::setPage(int pageIdx)
{
    if (pageIdx >= 0 && pageIdx < m_pageStack->count()) {
        CPageWidget *currentPage = dynamic_cast<CPageWidget *>(m_pageStack->currentWidget());
        CPageWidget *nextPage = dynamic_cast<CPageWidget *>(m_pageStack->widget(pageIdx));

        if (currentPage)
            currentPage->exitPage();
        if (nextPage)
            nextPage->enterPage();

        m_pageStack->setCurrentIndex(pageIdx);
    }
}


void CPageManager::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(m_backColor);
    painter.setPen(Qt::NoPen);
    painter.drawRect(this->rect());

    painter.end();
}
