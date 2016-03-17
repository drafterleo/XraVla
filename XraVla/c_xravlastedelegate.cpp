#include "c_xravlastedelegate.h"
#include "c_xravlasteitem.h"
#include "cw_figurespixra.h"
#include <QPainter>
#include <QtDebug>

CXravlasteDelegate::CXravlasteDelegate()
{
}

CXravlasteDelegate::~CXravlasteDelegate()
{
}


void CXravlasteDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QString word = "???";
    QString spec = "???";
    CAbstractPixra *pixra = 0;
    CXravlasteItem * xitem = index.data().value<CXravlasteItem *>();
    if (xitem) {
        word = xitem->name;
        spec = xitem->spec;
        pixra = xitem->pixra;
    }

    QPen framePen;
    QPen wordPen;
    QPen specPen;
    QBrush brush;

    if (option.state & QStyle::State_Selected){
        framePen = Qt::NoPen;//QPen(Qt::darkGray, 2);
        brush = QColor(0x3E3E3E);
        wordPen = QColor(Qt::white);
        specPen = QColor(0x91E22C);
    }else{
        framePen = Qt::NoPen;//QPen(Qt::white, 2);
        brush = QColor(0x2A2A2A);
        wordPen = QColor(Qt::gray);
        specPen = QColor(0x9FB89A); //QColor(0xBEDF96);
    }

    //wdg->resize(option.rect.size());


    painter->setBrush(brush);
    painter->setPen(framePen);
    painter->drawRect(option.rect.adjusted(1, 1, -1, -1));

    QPoint pixraPos = option.rect.topLeft() + QPoint(5, 5);

    if (pixra) {
        painter->save();
        painter->translate(pixraPos);
        pixra->resize(70, 70);
        pixra->setColor(wordPen.color());
        pixra->render(painter, QPoint(), QRegion(), 0);
        painter->restore();
    }

    //painter->setRenderHint(QPainter::Antialiasing, true);

    int startX = 78;
    QRect wordRect = QRect(startX, pixraPos.y(),
                           option.rect.width() - startX, 25);
    painter->setFont(QFont("verdana", 11));
    painter->setPen(wordPen);
    painter->drawText(wordRect, Qt::AlignLeft, word);

    QRect specRect = QRect(startX, pixraPos.y() + 20,
                           option.rect.width() - startX, 40);
    painter->setFont(QFont("verdana", 9));
    painter->setPen(specPen);
    painter->drawText(specRect, Qt::AlignLeft | Qt::TextWrapAnywhere, spec);

}

QSize CXravlasteDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 80);
    //return QItemDelegate::sizeHint(option, index);
}

//QWidget *CXravlasteDelegate::createEditor(QWidget *parent,
//                                          const QStyleOptionViewItem &option,
//                                          const QModelIndex &index) const
//{

//    qDebug() << "createEditor " << index.row();

//    CXravlaWidget *xw = new CXravlaWidget(parent);
//    return xw;
//}

//void CXravlasteDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    CXravlaWidget *xw = dynamic_cast <CXravlaWidget *>(editor);
//    if (xw) {
//        CXravlasteItem *xitem = index.data().value<CXravlasteItem *>();
//        xw->assignItem(xitem);
//    }
//}

//void CXravlasteDelegate::setModelData(QWidget *editor,
//                                      QAbstractItemModel *model,
//                                      const QModelIndex &index) const
//{
//    CXravlaWidget *xw = dynamic_cast <CXravlaWidget *>(editor);
//    if (xw) {
//        CXravlasteItem *xitem = index.data().value<CXravlasteItem *>();
//        xitem->word = xw->wordEdit->text();
//        xitem->spec = xw->specEdit->toPlainText();
//    }

//}

//void CXravlasteDelegate::updateEditorGeometry(QWidget *editor,
//                                              const QStyleOptionViewItem &option,
//                                              const QModelIndex &index) const
//{
//   editor->move(option.rect.topLeft() + QPoint(3, 3));
//}

