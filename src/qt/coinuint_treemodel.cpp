#include <QtWidgets>
#include <amount.h>
#include "asset_coin.h"
#include <qt/VCcoinunits.h>
#include "interfaces/wallet.h"

#include "coinuint_treeitem.h"
#include "coinuint_treemodel.h"

//! [0]
CoinUintTreeModel::CoinUintTreeModel(const QStringList &headers, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for(int i = 0; i < headers.size(); i++){
        rootData <<  headers[i];
        //rootData << header;
    }

    rootItem = new CoinUintTreeItem(rootData, -1, -1);
}

//! [1]
CoinUintTreeModel::~CoinUintTreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int CoinUintTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem->columnCount();
}
//! [2]

QModelIndexList CoinUintTreeModel::match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const
{
    assert(role == Qt::UserRole);

    QModelIndexList RetList;
    if (!start.isValid()) {
        return RetList;
    }

    CoinUintTreeItem *first = static_cast<CoinUintTreeItem*>(start.internalPointer());
    int childcount = first->parent()->childCount();
    for(int i = 0; i < childcount; i++){
        CoinUintTreeItem *TreeItem = first->parent()->child(i);
        QModelIndex item = createIndex(i, 0, TreeItem);
        RetList.push_back(item);
/*
        for(int j = 0; j < TreeItem->childCount(); j++){
            CoinUintTreeItem *ChildTreeItem = TreeItem->child(j);
            QModelIndex item = createIndex(ChildTreeItem->getOnlyIndex(), 0, ChildTreeItem);
            RetList.push_back(item);
        }
*/
    }

    return RetList;
}

QVariant CoinUintTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole && role != Qt::TextColorRole)
        return QVariant();

    CoinUintTreeItem *item = getItem(index);
    if(role == Qt::TextColorRole){
        QVariant bal = item->data(1);
        if(!bal.isValid() || bal.isNull()){
            return QColor(0, 0, 0);
        }

        QString strbal = bal.toString();
        if(strbal[0] == '-'){
            return QColor(255, 0, 0);
        }else{
            return QColor(0, 0, 0);
        }
    }

    return item->data(index.column());
}

//! [3]
Qt::ItemFlags CoinUintTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [3]

//! [4]
CoinUintTreeItem *CoinUintTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        CoinUintTreeItem *item = static_cast<CoinUintTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}
//! [4]

QVariant CoinUintTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex CoinUintTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
//! [5]

//! [6]
    CoinUintTreeItem *parentItem = getItem(parent);

    CoinUintTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

bool CoinUintTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

//! [7]
QModelIndex CoinUintTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CoinUintTreeItem *childItem = getItem(index);
    CoinUintTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}
//! [7]

bool CoinUintTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool CoinUintTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    CoinUintTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

//! [8]
int CoinUintTreeModel::rowCount(const QModelIndex &parent) const
{
    CoinUintTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}
//! [8]

bool CoinUintTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    CoinUintTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        Q_EMIT dataChanged(index, index, {role});

    return result;
}

bool CoinUintTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        Q_EMIT headerDataChanged(orientation, section, section);

    return result;
}

#include <sstream>
#include <string>
std::shared_ptr<CWallet> GetAssetWallet(const int AssetNo);
void CoinUintTreeModel::ReadAllBalanceData()
{
    CoinUintTreeItem *parent = rootItem;

    if(this->rowCount() > 0){
        this->removeRows(0, this->rowCount());
    }

    VccoinUnits::AutoAssetStack AssetStack;
    VccoinUnits::Reset();

    std::vector<CoinAsset> cas;
    CoinAssetManager::Instance().GetAllAssets(cas);
    
	if (cas.size() > 2) {
        std::sort(++cas.begin(), cas.end(), [](const CoinAsset& b1, const CoinAsset& b2) { return b1.name < b2.name; });
    }	
	
    for (auto ca : cas) {
        std::shared_ptr<CWallet> pWallet = ::GetAssetWallet(ca.no);
        if (pWallet == nullptr) {
            continue;
        }
        std::unique_ptr<interfaces::Wallet> pInfWallet = interfaces::MakeWallet(pWallet);
        interfaces::WalletBalances balances = pInfWallet->getBalances();
        if (ca.no != 0) {
            if (balances.balance == 0 && balances.unconfirmed_balance == 0) {
                continue;
            }
        }

        VccoinUnits::SetActiveCoinAsset(ca.no);
        std::stringstream strs;
        strs << ca.name.c_str();    // << "(" << ca.no << ")";
        QString name = strs.str().c_str();

        VccoinUnits::SetActiveCoinAsset(ca.no);

        parent->insertChild(parent->childCount(), rootItem->columnCount(), ca.no, 0)->setTooltip(VccoinUnits::description(0));
        parent->child(parent->childCount() - 1)->setData(0, name);

        {
            QList<VccoinUnits::Unit> uints = VccoinUnits::availableUnits();

            if(uints.size() == 1 ){
                parent->child(parent->childCount() - 1)->setData(1, VccoinUnits::formatWithUnit(uints[0], balances.balance));
            }else{
                QVector<std::pair<VccoinUnits::Unit, QStringList>> strColumnData;
                for(auto uint : uints){
                    QStringList tmp;
                    tmp.push_back(VccoinUnits::longName(uint));
                    tmp.push_back(VccoinUnits::formatWithUnit(uint, balances.balance));

                    strColumnData.push_back(std::make_pair(uint, tmp));
                }

                CoinUintTreeItem *cur_parent = parent->child(parent->childCount() - 1);
                for(auto vec : strColumnData){
                    cur_parent->insertChild(cur_parent->childCount(), rootItem->columnCount(), ca.no, vec.first)->setTooltip(VccoinUnits::description(vec.first));
                    cur_parent->child(cur_parent->childCount() - 1)->setData(0, vec.second[0]);
                    cur_parent->child(cur_parent->childCount() - 1)->setData(1, vec.second[1]);
                }
            }
        }
    }
}

void CoinUintTreeModel::UpdateAssetBalance(QMap<int, CAmount> &amounts)
{
    CoinUintTreeItem *parent = rootItem;

    if(this->rowCount() > 0){
        this->removeRows(0, this->rowCount());
    }

    VccoinUnits::AutoAssetStack AssetStack;
    VccoinUnits::Reset();
     
    std::vector<CoinAsset> cas;
    CoinAssetManager::Instance().GetAllAssets(cas);
	
    if (cas.size() > 2) {
        std::sort(++cas.begin(), cas.end(), [](const CoinAsset& b1, const CoinAsset& b2) { return b1.name < b2.name; });
    }	
	
    for (auto ca : cas) {
        std::shared_ptr<CWallet> pWallet = ::GetAssetWallet(ca.no);
        if (pWallet == nullptr) {
            continue;
        }
        std::unique_ptr<interfaces::Wallet> pInfWallet = interfaces::MakeWallet(pWallet);
        interfaces::WalletBalances balances = pInfWallet->getBalances();
        if (ca.no != 0) {
            if (balances.balance == 0 && balances.unconfirmed_balance == 0) {
                continue;
            }
        }

        VccoinUnits::SetActiveCoinAsset(ca.no);
        std::stringstream strs;
        strs << ca.name.c_str();        // << "(" << ca.no << ")";
        QString name = strs.str().c_str();

        VccoinUnits::SetActiveCoinAsset(ca.no);

        parent->insertChild(parent->childCount(), rootItem->columnCount(), ca.no, 0)->setTooltip(VccoinUnits::description(0));
        parent->child(parent->childCount() - 1)->setData(0, name);

        {
            if(amounts.find(ca.no) != amounts.end()){
                balances.balance = balances.balance - amounts[ca.no];
            }
            parent->child(parent->childCount() - 1)->setData(1, VccoinUnits::formatWithUnit(0, balances.balance));
        }
    }
}
