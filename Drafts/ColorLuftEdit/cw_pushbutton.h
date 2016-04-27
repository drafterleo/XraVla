#pragma once

#include <QWidget>
#include <QPixmap>

class CPushButton : public QWidget
{
    Q_OBJECT
public:
    explicit CPushButton(const QPixmap & pix, QWidget *parent = 0);
    void setPixmap(const QPixmap & pix);
    
signals:
    void clicked();
    
protected:
    inline bool isMouseIn() {return m_mouseIn;}
    inline bool isPressed() {return m_pressed;}

    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);

    void paintEvent(QPaintEvent *);

private:
    bool m_mouseIn;
    bool m_pressed;

    QPixmap m_pix;
};

