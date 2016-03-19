#ifndef CW_PAGEMANAGER_H
#define CW_PAGEMANAGER_H

#include "cp_pagewidget.h"
#include "cw_pagebar.h"
#include <QWidget>
#include <QStackedWidget>

class CPageManager : public QWidget
{
    Q_OBJECT
public:
    explicit CPageManager(QWidget *parent = 0);
    void addPage(const QString &name, const QPixmap &pix, CPageWidget *pageWidget);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *event);

signals:
    
public slots:
    void setPage(int pageIdx);

private:
    QStackedWidget *m_pageStack;
    CPageBar *m_pageBar;

    int m_barWidth;
    int m_pageMargin;

    QColor m_backColor;
};

#endif // CW_PAGEMANAGER_H
