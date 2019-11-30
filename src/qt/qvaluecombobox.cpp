// Copyright (c) 2011-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <qt/qvaluecombobox.h>
#include <qt/coinuint_treeitem.h>

QValueComboBox::QValueComboBox(QWidget *parent) :
        QComboBox(parent), role(Qt::UserRole)
{
    connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QValueComboBox::handleSelectionChanged);
}

QValueComboBox::~QValueComboBox()
{
}

QVariant QValueComboBox::value() const
{
    return itemData(currentIndex(), role);
}

void QValueComboBox::presetIndex(QModelIndex index)
{
    setRootModelIndex(index.parent());
    setModelColumn(index.column());
    setCurrentIndex(index.row());
    setRootModelIndex(QModelIndex());
    view()->setCurrentIndex(index);
}

void QValueComboBox::setValue(const QVariant &value)
{
    setCurrentIndex(findData(value, role));
}

void QValueComboBox::setRole(int _role)
{
    this->role = _role;
}

QVariant QValueComboBox::itemData(int index, int role) const
{
    QTreeView *pTreeView = (QTreeView*)this->view();
    QModelIndex Index = pTreeView->currentIndex();
    if(!Index.isValid()){
        return QVariant();
    }

    CoinUintTreeItem *item = static_cast<CoinUintTreeItem*>(Index.internalPointer());
    if(item == nullptr){
        return QVariant();
    }
    if(role == Qt::ToolTipRole){
        return item->getTooltip();
    }else{
        return QVariant();
    }
}

int QValueComboBox::MyCurrentIndex() const
{
    QTreeView *pTreeView = (QTreeView*)this->view();
    QModelIndex Index = pTreeView->currentIndex();
    if(!Index.isValid()){
        return -1;
    }
    CoinUintTreeItem *item = static_cast<CoinUintTreeItem*>(Index.internalPointer());
    return item->getTreeItemIndex();
}

void QValueComboBox::handleSelectionChanged(int idx)
{
    Q_EMIT valueChanged();
}
