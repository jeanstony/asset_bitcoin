#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <amount.h>

class CoinUintTreeItem;

//! [0]
class CoinUintTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    CoinUintTreeModel(const QStringList &headers, QObject *parent = 0);
    ~CoinUintTreeModel();
//! [0] //! [1]

    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    Q_INVOKABLE virtual QModelIndexList match(const QModelIndex &start, int role,
                                              const QVariant &value, int hits = 1,
                                              Qt::MatchFlags flags =
                                              Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
//! [1]

//! [2]
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    void ReadAllBalanceData(void);
    void UpdateAssetBalance(QMap<int, CAmount> &amounts);

private:
    CoinUintTreeItem *getItem(const QModelIndex &index) const;

    CoinUintTreeItem *rootItem;
};
//! [2]

#endif // TREEMODEL_H
