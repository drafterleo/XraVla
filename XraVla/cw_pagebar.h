#ifndef CW_PAGEBAR_H
#define CW_PAGEBAR_H

#include <QWidget>
#include <QStackedWidget>

struct SPageBarItem
{
    QString name;
    QPixmap pix;
    int     idx;
};

class CPageBar : public QWidget
{
    Q_OBJECT
public:
    explicit CPageBar(QWidget *parent = 0);
    void addPage(int pageIdx, const QString &name, const QPixmap &pix);

protected:
    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);

    void setCurrPage(int newPageIdx);

signals:
    void pageChanged(int newPageIdx);

private:
    QVector <SPageBarItem> m_pages;

    int m_itemHeight;
    int m_currPageIdx;
    int m_hotPageIdx;
};

#endif // CW_PAGEBAR_H
