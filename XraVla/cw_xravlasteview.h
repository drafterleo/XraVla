#ifndef CW_XRAVLASTEVIEW_H
#define CW_XRAVLASTEVIEW_H

#include <QListView>

class CXravlasteView : public QListView
{
    Q_OBJECT
public:
    explicit CXravlasteView(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);
    
signals:
    void changed(const QModelIndex & current, const QModelIndex & previous);
    
public slots:

protected slots:
    void currentChanged (const QModelIndex & current, const QModelIndex & previous);
    void itemDropped(const QModelIndex & index);

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QRect m_dragRect;
    
};

#endif // CW_XRAVLASTEVIEW_H
