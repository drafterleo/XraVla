#include "c_xravlastemodel.h"
#include "c_pixrafactory.h"
#include <QMimeData>
#include <QByteArray>
#include <QDataStream>
#include <QtDebug>

QString xravlasteItemMimeType("xravla/xravlaste.item-pointer");
CXravlasteItem draggingItem;

CXravlasteModel::CXravlasteModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_dropping = false;
    m_droppingRow = -1;

    CFiguresPixra *figPixra = new CFiguresPixra;
    figPixra->addNormFigure(new CRectangleFigure(QRectF(0.1, 0.1, 0.33, 0.33)));
    figPixra->addNormFigure(new CRectangleFigure(QRectF(0.33, 0.33, 0.33, 0.33)));
    figPixra->addNormFigure(new CRectangleFigure(QRectF(0.56, 0.56, 0.33, 0.33)));
    m_protoPixra = figPixra;
}

CXravlasteModel::~CXravlasteModel()
{
    for (int i = 0; i < m_items.count(); ++i) {
        delete m_items[i];
    }
    m_items.clear();

    delete m_protoPixra;
}

int CXravlasteModel::rowCount(const QModelIndex &parent) const
{
   Q_UNUSED(parent)
   return m_items.count();
}

QVariant CXravlasteModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= m_items.size())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole){
        return QVariant::fromValue(m_items.at(index.row()));
    }else{
        return QVariant();
    }
}

bool CXravlasteModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        CXravlasteItem *citem = m_items.at(index.row());
        CXravlasteItem *nitem = value.value<CXravlasteItem *>();
        if (citem && nitem) {
            citem->assign(nitem);
            emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}

Qt::ItemFlags CXravlasteModel::flags(const QModelIndex &index) const
{
//    if (!index.isValid())
//        return Qt::ItemIsEnabled;

//    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

   Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);

    if (index.isValid())
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
        return Qt::ItemIsDropEnabled | defaultFlags;

   //return defaultFlags;
}

Qt::DropActions CXravlasteModel::supportedDropActions() const
{
    return Qt::MoveAction;
    //return QAbstractListModel::supportedDropActions();
}

QStringList CXravlasteModel::mimeTypes() const
{
    QStringList types;
    types << xravlasteItemMimeType;
    return types;
}

QMimeData *CXravlasteModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    //foreach (QModelIndex index, indexes) {
    for (int i = 0; i < indexes.count(); ++i) {
        if (indexes.at(i).isValid()) {
            CXravlasteItem *xitem = data(indexes.at(i), Qt::DisplayRole).value<CXravlasteItem *>();
            draggingItem.assign(xitem); /* !!! */
            stream << QVariant();
        }
    }

    mimeData->setData(xravlasteItemMimeType, encodedData);

    return mimeData;
}

bool CXravlasteModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
         return true;
     if (!data->hasFormat(xravlasteItemMimeType))
         return false;
     if (column > 0)
         return false;

     int beginRow;
     if (row != -1)
         beginRow = row;
     else if (parent.isValid())
         beginRow = parent.row();
     else
         beginRow = rowCount(QModelIndex());

     /* decoding data */
     QByteArray encodedData = data->data(xravlasteItemMimeType);
     QDataStream stream(&encodedData, QIODevice::ReadOnly);
     QList <QVariant> newItems;
     int rows = 0;

     while (!stream.atEnd()) {
         QVariant data;
         stream >> data;
         newItems << data;
         ++rows;
     }

     //emit dataDropped(this->index(beginRow, 0));

     insertRows(beginRow, rows, QModelIndex());
     foreach (QVariant item, newItems) {
         QModelIndex idx = index(beginRow, 0, QModelIndex());
         QVariant vitem;
         vitem.setValue(&draggingItem); /* !!! */
         setData(idx, vitem);
         beginRow++;
     }

     m_dropping = true;
     m_droppingRow = beginRow;

     return true;
}

bool CXravlasteModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(QModelIndex(), position, position + rows - 1);

//    for (int row = 0; row < rows; ++row) {
//        CFiguresPixra *pixra = new CFiguresPixra;
//        for (int i = 0; i < 3; ++i) {
//            int w = qrand()%80 + 16;
//            int h = qrand()%80 + 16;
//            int x = qrand()%(96 - w) + 2;
//            int y = qrand()%(96 - h) + 2;
//            pixra->addNormFigure(new CRectangleFigure(QRectF(x/100.0, y/100.0, w/100.0, h/100.0)));
//        }
//        m_items.insert(position, new CXravlasteItem("toki ala", "toki ala", pixra));
//    }

    for (int row = 0; row < rows; ++row) {
//        CFiguresPixra *pixra = new CFiguresPixra;
//        pixra->addNormFigure(new CRectangleFigure(QRectF(0.1, 0.1, 0.33, 0.33)));
//        pixra->addNormFigure(new CRectangleFigure(QRectF(0.33, 0.33, 0.33, 0.33)));
//        pixra->addNormFigure(new CRectangleFigure(QRectF(0.56, 0.56, 0.33, 0.33)));
//        m_items.insert(position, new CXravlasteItem("toki", "toki", pixra));

        m_items.insert(position, new CXravlasteItem("name", "description", m_protoPixra));
    }

    endInsertRows();
    return true;
}

bool CXravlasteModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
        if (m_items.count() > 0) {
            delete m_items[position];
            m_items.removeAt(position);
        }
    }

    endRemoveRows();

    if (m_dropping) {
        if (m_droppingRow > position)
            emit dataDropped(this->index(m_droppingRow - 2));
        else
            emit dataDropped(this->index(m_droppingRow - 1));
        m_dropping = false;
    }

    return true;
}

void CXravlasteModel::moveRowToUp(int row)
{
    if (!m_items.isEmpty() && row > 0 && row < m_items.count()) {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
        m_items.swap(row, row - 1);
        endMoveRows();
    }
}

void CXravlasteModel::moveRowToDown(int row)
{
    if (!m_items.isEmpty() && row >= 0 && row < m_items.count() - 1) {
        beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
        m_items.swap(row, row + 1);
        endMoveRows();
    }
}

void CXravlasteModel::setProtoPixra(CAbstractPixra *pixra)
{
    delete m_protoPixra;
    if (pixra) {
        m_protoPixra = pixraFactoryInstance.createPixra(pixra->metaObject()->className());
        if (m_protoPixra)
            m_protoPixra->assign(pixra);
    } else
        m_protoPixra = 0;
}
