#include <QPainter>
#include "cw_teststats.h"

CTestStats::CTestStats(QWidget *parent) :
    QWidget(parent)
{
    m_right = 0;
    m_wrong = 0;
}


void CTestStats::setRightCount(int right)
{
    m_right = right;
    update();
}

void CTestStats::setWrongCount(int wrong)
{
    m_wrong = wrong;
    update();
}

void CTestStats::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(QPen(QBrush(QColor(0x538BC4)), 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(this->rect().adjusted(1, 1, -1, -1));

    QRect rightRect = QRect(0, height()/3, width(), 30);
    QRect wrongRect = rightRect.translated(0, 30);
    QRect boxRect = wrongRect.translated(0, 40).adjusted(50, 5, -50, -5);
    QRect percentRect = boxRect.translated(0, 30).adjusted(0, 0, 0, 20);

    painter.setFont(QFont("Arial", 16));

    painter.setPen(QColor(0x55A92E));
    painter.drawText(rightRect, Qt::AlignCenter, QString("right answers : %1").arg(m_right));

    painter.setPen(QColor(0xD92731));
    painter.drawText(wrongRect, Qt::AlignCenter, QString("wrong answers : %1").arg(m_wrong));

    //painter.drawRect(boxRect);
    int totalCount = m_right + m_wrong;
    if (totalCount > 0) {
        int wrongWidth = boxRect.width() * m_wrong/totalCount;
        int rightWidth = boxRect.width() - wrongWidth;
        painter.setPen(Qt::gray);
        painter.drawRect(boxRect.adjusted(-5, -5, 5, 5));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::darkGreen);
        painter.drawRect(QRect(boxRect.topLeft(), QSize(rightWidth, 20)));
//        painter.setBrush(QColor(0xD92731));
//        painter.drawRect(QRect(boxRect.left() + rightWidth, boxRect.top(), wrongWidth, 20));

        painter.setPen(Qt::white);
        int percent = (100 * m_right)/totalCount;
        painter.drawText(percentRect, Qt::AlignCenter, QString("%1 \%").arg(percent));
    }

    painter.end();
}
