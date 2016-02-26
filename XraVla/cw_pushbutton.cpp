#include "cw_pushbutton.h"
#include <QMouseEvent>
#include <QPainter>

CPushButton::CPushButton(const QPixmap & pix, QWidget *parent)
    : QWidget(parent)
{
    m_pix = pix;
    m_mouseIn = false;
    m_pressed = false;
}

void CPushButton::setPixmap(const QPixmap & pix)
{
    m_pix = pix;
    update();
}

void CPushButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QRect btnRect = this->rect().adjusted(1, 1, -3, -3);
    QColor frameColor = QColor(0x4E4E4E);
    QColor backColor = Qt::NoBrush; // QColor(0x1E1E1E);

    if (this->isEnabled()) {
        if (m_pressed) {
            //backColor = QColor(0x3E3E3E);
            frameColor = QColor(0x8E8E8E);
            btnRect.translate(1, 1);
        } else {
            if (m_mouseIn) {
                frameColor = QColor(0x8E8E8E);
                //backColor = QColor(0x3E3E3E);
            }
        }
    } else
        painter.setOpacity(0.3);

    painter.setPen(QPen(frameColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(backColor);
    painter.drawRect(btnRect);

    painter.drawPixmap((btnRect.width() - m_pix.width())/2 + btnRect.x(),
                       (btnRect.height() - m_pix.height())/2 + btnRect.y(),
                       m_pix);

    painter.end();
}

void CPushButton::mouseMoveEvent(QMouseEvent *)
{
    if(!m_mouseIn){
        m_mouseIn = true;
        update();
    }
}

void CPushButton::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pressed = true;
        update();
    }
}

void CPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_pressed = false;
        update();
        if(rect().contains(event->pos()))
            emit clicked();
    }
}

void CPushButton::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_mouseIn = false;
    update();
}

void CPushButton::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_mouseIn = true;
    update();
}

