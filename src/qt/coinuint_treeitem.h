#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>

//! [0]
class CoinUintTreeItem
{
public:
    explicit CoinUintTreeItem(const QVector<QVariant> &data, const int ANo, const int CNo = 0, CoinUintTreeItem *parent = 0);
    ~CoinUintTreeItem();

    CoinUintTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns, const int ANo, const int CNo = 0);
    CoinUintTreeItem* insertChild(int position, int columns, const int ANo, const int CNo = 0);
    bool insertColumns(int position, int columns);
    CoinUintTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);
    int getTreeItemIndex(void);
    void setTooltip( QString tip ){ StrTooltip = tip; }
    QString getTooltip(void){ return StrTooltip; }

private:
    const int AssetNo;
    const int CoinNo;       // The value that is equal -1 is invalidate
    QString StrTooltip;

    QList<CoinUintTreeItem*> childItems;
    QVector<QVariant> itemData;
    CoinUintTreeItem *parentItem;
};
//! [0]

#endif // TREEITEM_H
