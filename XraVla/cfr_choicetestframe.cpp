#include "cfr_choicetestframe.h"
#include <QPainter>
#include <QResizeEvent>
#include <QtDebug>

CChoiceTestFrame::CChoiceTestFrame(QWidget *parent) :
    CAbstractTestFrame(parent)
{
    m_rightIdx = 0;
    m_hotIdx = -1;
    m_clickIdx = -1;

    m_wordFont = QFont("Arial", 22, QFont::Bold);
    m_specFont = QFont("Arial", 12);

    m_testMode = true;

    setMouseTracking(true);
}

CChoiceTestFrame::~CChoiceTestFrame()
{
    for (int i = 0; i < m_items.count(); ++i) {
        delete m_items[i];
    }
}

void CChoiceTestFrame::addItem(CXravlasteItem *item, bool right)
{
    CXravlasteItem *newItem = new CXravlasteItem;
    newItem->assign(item);
    m_items.append(newItem);
    if (right)
        m_rightIdx = m_items.count() - 1;
    m_pixraRects.append(QRect());
    updateRects();
    update();
}

void CChoiceTestFrame::setRight(int itemIdx)
{
    if (itemIdx >= 0 && itemIdx < m_items.count()) {
        m_rightIdx = itemIdx;
    }
}

void CChoiceTestFrame::shuffleItems(int n)
{
    if(m_items.count() > 1) {
        for (int i = 0; i < n; ++i) {
            int aIdx = qrand() % m_items.count();
            int bIdx = qrand() % m_items.count();
            if (aIdx != bIdx) {
                if (m_rightIdx == aIdx) m_rightIdx = bIdx;
                else
                if (m_rightIdx == bIdx) m_rightIdx = aIdx;

                CXravlasteItem *temp = m_items[aIdx];
                m_items.replace(aIdx, m_items[bIdx]);
                m_items.replace(bIdx, temp);
            }
        }
        update();
    }
}

void CChoiceTestFrame::resizeEvent(QResizeEvent *event)
{
    updateRects();
}

void CChoiceTestFrame::updateRects()
{
    m_rightRect = QRect(0, 0, width(), height() * 0.3);

    if (m_pixraRects.count() > 0) {
        int rwidth = width() / m_pixraRects.count();
        int xshift = (width() % m_pixraRects.count())/2;
        for (int i = 0; i < m_pixraRects.count(); ++i) {
            QRect itemRect = QRect(i*rwidth + xshift, m_rightRect.bottom(), rwidth, height() - m_rightRect.height());
            int pixWidth = itemRect.width() - 12;
            int pixHeight = pixWidth;
            m_pixraRects[i] = QRect(itemRect.left() + 6, itemRect.top() + 50, pixWidth, pixHeight);
            if (m_items[i]->pixra){
                m_items[i]->pixra->resize(pixWidth, pixHeight);
            }
            //qDebug() << m_itemRects[i];
        }
    }
}

void CChoiceTestFrame::mouseMoveEvent(QMouseEvent *event)
{
    if (m_testMode) {
        int oldHotIdx = m_hotIdx;
        m_hotIdx = -1;
        for (int i = 0; i < m_pixraRects.count(); ++i) {
            if(m_pixraRects[i].contains(event->pos())) {
                m_hotIdx = i;
                break;
            }
        }

        if (m_hotIdx != oldHotIdx) {
            update();
        }
    }
}

void CChoiceTestFrame::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_hotIdx = -1;
    update();
}

void CChoiceTestFrame::mousePressEvent(QMouseEvent *event)
{
    if (m_testMode) {
        for (int i = 0; i < m_pixraRects.count(); ++i) {
            if(m_pixraRects[i].contains(event->pos())) {
                m_clickIdx = i;
                m_testMode = false;
                STestResult result;
                result.isCorrect = (m_clickIdx == m_rightIdx);
                emit testDone(result);
                update();
                break;
            }
        }
    }
}


void CChoiceTestFrame::showSpec(bool xu)
{
    if (m_showSpec != xu) {
        m_showSpec = xu;
        update();
    }
}

void CChoiceTestFrame::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
//    painter.setPen(Qt::gray);
//    painter.drawRect(this->rect().adjusted(0, 0, -1, -1));
//    painter.setPen(Qt::green);
//    painter.drawRect(m_wordRect);
//    painter.setPen(Qt::blue);
//    painter.drawRects(m_itemRects);

    /* draw word */
    if (m_rightIdx >= 0 && m_rightIdx < m_items.count()) {
        QRect wordRect = m_rightRect.adjusted(0, 0, 0, -m_rightRect.height()/2);
        painter.setPen(Qt::white);
        painter.setFont(m_wordFont);
        painter.drawText(wordRect, Qt::AlignCenter, m_items[m_rightIdx]->word);
        if (m_showSpec || !m_testMode) {
            QRect specRect = wordRect.translated(0, wordRect.height() - 15);
            painter.setPen(QColor(0x9FB89A));
            painter.setFont(m_specFont);
            painter.drawText(specRect, Qt::AlignCenter | Qt::AlignTop, m_items[m_rightIdx]->spec);
        }
    }

    /* draw items */
    for (int i = 0; i < m_items.count(); ++i) {
        CAbstractPixra *pixra = m_items[i]->pixra;
        if (pixra) {
            painter.setOpacity(1.0);
            if (m_testMode) {
                if (i == m_hotIdx) {
                    pixra->setColor(Qt::white);
                    pixra->setFrameColor(QColor(0x538BC4));
                } else {
                    pixra->setColor(Qt::lightGray);
                    pixra->setFrameColor(QColor(0x466A7E));
                }
            } else {
                pixra->setFrameColor(QColor(0x466A7E));
                if (i == m_clickIdx && i == m_rightIdx) {
                    pixra->setColor(Qt::white);
                    pixra->setBackColor(QColor(0x55A92E));
                } else
                if (i == m_clickIdx && i != m_rightIdx) {
                    pixra->setColor(Qt::white);
                    pixra->setBackColor(QColor(0xD92731));
                } else
                if (i == m_rightIdx) {
                    pixra->setColor(Qt::white);
                    pixra->setBackColor(QColor(0x55A92E));
                } else {
                    painter.setOpacity(0.3);
                    pixra->setColor(Qt::lightGray);
                    pixra->setFrameColor(QColor(0x466A7E));
                }
            }

            painter.save();
            painter.translate(m_pixraRects[i].topLeft());
            pixra->render(&painter, QPoint(), QRegion(), 0);
            painter.restore();
            //qDebug() << m_itemRects[i] << offset << pixra->size();
        }
    }
    painter.end();
}
