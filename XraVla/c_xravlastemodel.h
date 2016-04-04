#ifndef C_XRAVLASTEMODEL_H
#define C_XRAVLASTEMODEL_H

#include <QAbstractListModel>
#include "c_xravlasteitem.h"

class CXravlasteModel: public QAbstractListModel
{
    Q_OBJECT
public:
    CXravlasteModel(QObject *parent = 0);
    ~CXravlasteModel();

    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant        data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool            setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags   flags(const QModelIndex &index) const;

    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);


    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    void moveRowToUp(int row);
    void moveRowToDown(int row);
    inline CAbstractPixra *protoPixra(void) {return m_protoPixra;}
    void setProtoPixra(CAbstractPixra *pixra);

    void sortItemsByName();

signals:
    void dataDropped(const QModelIndex &index);

private:
    QList <CXravlasteItem *> m_items;
    bool m_dropping;
    int  m_droppingRow;

    CAbstractPixra *m_protoPixra;

};

#endif // C_XRAVLASTEMODEL_H
