#include "cw_pagebar.h"

#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>

CPageBar::CPageBar(QWidget *parent) :
    QWidget(parent)
{
    m_itemHeight = 90;
    m_currPageIdx = -1;
    m_hotPageIdx = -1;

    setMouseTracking(true);
}

void CPageBar::setCurrPage(int newPageIdx)
{
    if (newPageIdx != m_currPageIdx &&
        newPageIdx >= 0 &&
        newPageIdx < m_pages.count())
    {
        m_currPageIdx = newPageIdx;
        emit pageChanged(m_pages.at(m_currPageIdx).idx);
        update();
    }
}

void CPageBar::addPage(int pageIdx, const QString &name, const QPixmap &pix)
{
    SPageBarItem page;
    page.idx = pageIdx;
    page.name = name;
    page.pix = pix;

    m_pages.append(page);
    setCurrPage(0);
}


void CPageBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor borderColor = QColor(0x4E4E4E);//QColor(200, 200, 200, 100); //Qt::black;


    if (m_currPageIdx >= 0) {

        QPainterPath path;
        int top = 1;
        int left = 1;
        int right = width() - 1;
        int bottom = height() - 1;
        int flagHeight = m_itemHeight / 3;
        int flagUp = m_itemHeight * m_currPageIdx + (m_itemHeight - flagHeight) / 2;
        path.moveTo(left, top);
        path.lineTo(right, top);
        path.lineTo(right, flagUp);
        path.lineTo(right - 10, flagUp + flagHeight / 2);
        path.lineTo(right, flagUp + flagHeight);
        path.lineTo(right, bottom);
        path.lineTo(left, bottom);
        path.closeSubpath();

        painter.setPen(QPen(borderColor, 2));
        painter.setBrush(QColor(0x3E3E3E));
        painter.drawPath(path);

        painter.setBrush(QColor(0x1E1E1E));
        painter.setPen(Qt::NoPen);
        if (m_currPageIdx > 0)
            painter.drawRect(2, 2, width() - 4, m_currPageIdx * m_itemHeight - 2);
        painter.drawRect(2,
                         (m_currPageIdx + 1) * m_itemHeight + 2,
                         width() - 4,
                         height() - (m_currPageIdx + 1) * m_itemHeight - 4);
    } else {
        painter.setBrush(Qt::darkGray);
        painter.setPen(QPen(borderColor, 2));
        painter.drawRect(this->rect().adjusted(1, 1, -1, -1));
    }


    for (int i = 0; i < m_pages.count(); ++i) {
        QRect iRect = QRect(1, i * m_itemHeight + 1, width(), m_itemHeight - 1);

//        if (i == m_hotPageIdx && i != m_currPageIdx) {
//            painter.setBrush(QColor(200, 200, 200, 20));
//            painter.setPen(Qt::NoPen);
//            painter.drawRect(iRect.adjusted(2, 0, -2, 0));
//        }

        if (i > 0) {
            QRect bRect = iRect.adjusted(2, 0, -2, 2);
            painter.setPen(QPen(borderColor, 2));
            painter.drawLine(bRect.topLeft(), bRect.topRight());
            painter.drawLine(bRect.bottomLeft(), bRect.bottomRight());
        }

        if (i == m_currPageIdx || i == m_hotPageIdx)
            painter.setPen(Qt::white);
        else
            painter.setPen(QColor(0xC0C0C0));
        painter.drawText(iRect.adjusted(0, 0, -10, -2),
                         Qt::AlignCenter | Qt::AlignBottom,
                         m_pages.at(i).name);

        painter.drawPixmap(iRect.left() + (iRect.width() - m_pages.at(i).pix.width() - 10) / 2,
                           iRect.top() + 8,
                           m_pages.at(i).pix);
    }

    painter.end();
}

void CPageBar::mouseMoveEvent(QMouseEvent *event)
{
    m_hotPageIdx = -1;
    for (int i = 0; i < m_pages.count(); ++i) {
        QRect iRect = QRect(0, i * m_itemHeight, width(), m_itemHeight);
        if (iRect.contains(event->pos())) {
            m_hotPageIdx = i;
            break;
        }
    }
    update();
}

void CPageBar::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < m_pages.count(); ++i) {
        QRect iRect = QRect(0, i * m_itemHeight, width(), m_itemHeight);
        if (iRect.contains(event->pos())) {
            setCurrPage(i);
            break;
        }
    }
}

void CPageBar::leaveEvent(QEvent *)
{
    m_hotPageIdx = -1;
    update();
}


