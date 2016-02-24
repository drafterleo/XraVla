#ifndef C_XRAVLASTEDELEGATE_H
#define C_XRAVLASTEDELEGATE_H

#include <QItemDelegate>
//#include "cw_xravlawidget.h"

class CXravlasteDelegate: public QItemDelegate
{
public:
    CXravlasteDelegate();
    ~CXravlasteDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    //CXravlaWidget *xwidget;
};

#endif // C_XRAVLASTEDELEGATE_H
