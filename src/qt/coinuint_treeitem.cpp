#include "coinuint_treeitem.h"

#include <QStringList>

//! [0]
CoinUintTreeItem::CoinUintTreeItem(const QVector<QVariant> &data, const int ANo, const int CNo, CoinUintTreeItem *parent)
    :AssetNo(ANo), CoinNo(CNo)
{
    parentItem = parent;
    itemData = data;
}
//! [0]

//! [1]
CoinUintTreeItem::~CoinUintTreeItem()
{
    qDeleteAll(childItems);
}
//! [1]

//! [2]
CoinUintTreeItem *CoinUintTreeItem::child(int number)
{
    return childItems.value(number);
}
//! [2]

//! [3]
int CoinUintTreeItem::childCount() const
{
    return childItems.count();
}
//! [3]

//! [4]
int CoinUintTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CoinUintTreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int CoinUintTreeItem::columnCount() const
{
    return itemData.count();
}
//! [5]

//! [6]
QVariant CoinUintTreeItem::data(int column) const
{
    return itemData.value(column);
}
//! [6]

//! [7]
bool CoinUintTreeItem::insertChildren(int position, int count, int columns, const int ANo, const int CNo)
{
    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        CoinUintTreeItem *item = new CoinUintTreeItem(data, ANo, CNo, this);
        childItems.insert(position, item);
    }

    return true;
}

CoinUintTreeItem* CoinUintTreeItem::insertChild(int position, int columns, const int ANo, const int CNo)
{
    if (position < 0 || position > childItems.size())
        return nullptr;

    QVector<QVariant> data(columns);
    CoinUintTreeItem *item = new CoinUintTreeItem(data, ANo, CNo, this);
    childItems.insert(position, item);

    return item;
}
//! [7]

//! [8]
bool CoinUintTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.insert(position, QVariant());

    for(int i = 0; i < childItems.size(); i++){
        CoinUintTreeItem *child = childItems[i];
        child->insertColumns(position, columns);
    }

    return true;
}
//! [8]

//! [9]
CoinUintTreeItem *CoinUintTreeItem::parent()
{
    return parentItem;
}
//! [9]

//! [10]
bool CoinUintTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}
//! [10]

bool CoinUintTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData.remove(position);

    for(int i = 0; i < childItems.size(); i++){
        CoinUintTreeItem *child = childItems[i];
        child->removeColumns(position, columns);
    }

    return true;
}

int CoinUintTreeItem::getTreeItemIndex(void)
{
    int tmp = AssetNo << 16;
    return tmp | CoinNo;
}

//! [11]
bool CoinUintTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData.size())
        return false;

    itemData[column] = value;
    return true;
}
//! [11]
