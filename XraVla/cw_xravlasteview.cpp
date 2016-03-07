#include "cw_xravlasteview.h"
#include "c_xravlastemodel.h"
#include "c_xravlastedelegate.h"
#include <QtDebug>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPainter>

CXravlasteView::CXravlasteView(QWidget *parent) :
    QListView(parent)
{
    CXravlasteModel *xmodel = new CXravlasteModel(this);
    //xmodel->insertRows(0, 1);

    CXravlasteDelegate *xdelegate = new CXravlasteDelegate;

    setModel(xmodel);
    setItemDelegate(xdelegate);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    setResizeMode(QListView::Adjust);
    //setSelectionMode(QAbstractItemView::ExtendedSelection);
//    setMovement(QListView::Free);
//    setDragEnabled(true);
//    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(false);
    setDragDropMode(QAbstractItemView::InternalMove);
    //setDragDropMode(QAbstractItemView::DragDrop);

    setCurrentIndex(xmodel->index(0));

    QPalette pal = palette();
    pal.setColor(QPalette::Base, Qt::transparent);
    setPalette(pal);
    setFrameStyle(QFrame::Plain);
    setLineWidth(0);
}

void CXravlasteView::setModel(QAbstractItemModel *model)
{
    CXravlasteModel *xmodel = dynamic_cast <CXravlasteModel *>(model);
    if (xmodel) {
        QListView::setModel(xmodel);
        connect(xmodel, SIGNAL(dataDropped(QModelIndex)), this, SLOT(itemDropped(QModelIndex)));
    }
}

void CXravlasteView::currentChanged (const QModelIndex & current, const QModelIndex & previous)
{
    QListView::currentChanged(current, previous);
    emit changed(current, previous);
}

void CXravlasteView::dragMoveEvent(QDragMoveEvent *event)
{
    QListView::dragMoveEvent(event);
    m_dragRect = event->answerRect();
    //qDebug() << m_dropRect;
    update();
}

void CXravlasteView::itemDropped(const QModelIndex & index)
{
    scrollTo(index);
    setCurrentIndex(index);
}


void CXravlasteView::paintEvent(QPaintEvent *event)
{
    QListView::paintEvent(event);

    if (state() & DraggingState) {
        QModelIndex dragIdx = indexAt(m_dragRect.topLeft());
        QRect itemRect = visualRect(dragIdx);

        if (dragIdx.row() != currentIndex().row() &&
            dragIdx.row() != currentIndex().row() + 1)
        {
            QPainter painter(viewport());
            painter.setPen(QPen(QColor(0x3EA8ED), 2));
            painter.setBrush(Qt::NoBrush);

            if (!dragIdx.isValid()) {
                itemRect = visualRect(model()->index(model()->rowCount() - 1, 0));
                painter.drawLine(0, itemRect.bottom() + 1, width(), itemRect.bottom() + 1);
            } else
                painter.drawLine(0, itemRect.top() + 1, width(), itemRect.top() + 1);
            painter.end();
        }
    }

    //qDebug() << "paint";
}
