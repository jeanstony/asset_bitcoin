/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "assetmodel.h"
#include "amount.h"
#include "asset_coin.h"
#include <qt/guiutil.h>

AssetModel::AssetModel(QObject* parent) : QAbstractListModel(parent)
{
    /*
    m_assets.append({ "Angel Hogan", "Chapel St. 368 ", "Clearwater" , "0311 1823993" });
    */
}

int AssetModel::rowCount(const QModelIndex&) const
{
    return m_assets.count();
}

QVariant AssetModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case no:
            return m_assets.at(index.row()).no;
        case Name:
            return m_assets.at(index.row()).name;
        case Balance:
            return m_assets.at(index.row()).balance;
        case Unconfirmed_balance:
            return m_assets.at(index.row()).unconfirmed_balance;
        case Total_balance:
            return m_assets.at(index.row()).total_balance;
        case IsLock:
            return m_assets.at(index.row()).IsLock;
        case IsMainWallet:
            return m_assets.at(index.row()).IsMainWallet;
        default:
            return QVariant();
        }
    return QVariant();
}

QHash<int, QByteArray> AssetModel::roleNames() const
{
    static const QHash<int, QByteArray> roles{
        {no, "AssetNo"},
        {Name, "name"},
        {Balance, "balance"},
        {Unconfirmed_balance, "unconfirmed_balance"},
        {Total_balance, "total_balance"},
        {IsLock, "locked"},
        {IsMainWallet, "IsMainWallet"}
    };
    return roles;
}

QVariantMap AssetModel::get(int row) const
{
    const Asset asset = m_assets.value(row);
    return {
        {"AssetNo", asset.no},
        {"name", asset.name},
        {"balance", asset.balance},
        {"unconfirmed_balance", asset.unconfirmed_balance},
        {"total_balance", asset.total_balance},
        {"locked", asset.IsLock},
        {"IsMainWallet", IsMainWallet}
    };
}

void AssetModel::append(const int AssetNo, const QString& name, const QString& balance, const QString& unconfirmed_balance, const QString& total_balance, const bool IsLocked, const bool IsMainWallet)
{
    int row = 0;
    if (m_assets.size() >= 1) {
        row = 1;
    }
    
    while (row < m_assets.count() && name > m_assets.at(row).name)
        ++row;
    beginInsertRows(QModelIndex(), row, row);
    m_assets.insert(row, {AssetNo, name, balance, unconfirmed_balance, total_balance, IsLocked, IsMainWallet});
    endInsertRows();
}

bool CreateNewAddressHelper(std::string& newAddress);
int CreateAssetHelper(const CoinAsset& ca);
int AssetModel::create_asset(const QString& name, const QString& desc, const QString& total, const QString& coin)
{
    if (name.isEmpty() || total.isEmpty() || coin.isEmpty()) {
        return -1; //"请正确填写代币的创建信息！";
    }

    std::string tmp;
    CoinAsset ca;
    ca.no = -1;
    tmp = GUIUtil::toStdString(name);
    ca.name = tmp.substr(0, 6);
    tmp = GUIUtil::toStdString(desc);
    ca.desc = tmp.substr(0, 32);
    ca.coin = coin.toLongLong();
    ca.max = total.toLongLong() * ca.coin;
    ca.status = 0;
    if (ca.coin <= 0 || ca.max <= 0) {
        return -2; //"代币金额设置错误！";
    }

    std::vector<CoinAsset> cas;
    CoinAssetManager::Instance().GetAllAssets(cas);
    for (auto i : cas) {
        if (i.name == ca.name) {
            return -3; //"代币已存在，请重新设置代币名！";
        }
    }

    std::string addr;
    if (!CreateNewAddressHelper(addr)) {
        return -4; //"创建代币拥有者失败！";
    }
    ca.createAddr = addr;

    try {
        CreateAssetHelper(ca);
    } catch (...) {
        return -5; //"创建代币失败（内部错误）！";
    }

    return 0;
}

#include <init.h>
int AssetModel::lock_asset(const int AssetNo, const bool IsLock)
{
    bool IsSuccess = false;
    if(IsLock){
        if(CoinAssetManager::Instance().IsLockCoinAsset(AssetNo))
            return 1;

        IsSuccess = CoinAssetManager::Instance().LockCoinAsset(AssetNo);
    }else{
        if(!CoinAssetManager::Instance().IsLockCoinAsset(AssetNo))
            return 1;

        IsSuccess = CoinAssetManager::Instance().UnlockCoinAsset(AssetNo);
    }

    if(IsSuccess){
        CoinAssetManager::Instance().WriteToDB();

        CoinAsset ca;
        if( CoinAssetManager::Instance().GetAsset(AssetNo, ca)){
            PushSerCoinAssetStatus(ca.no, ca.status);
        }

        this->UpdateAssetStatus(AssetNo);

        return 1;
    }

    return 0;
}

void AssetModel::set(int row, const int AssetNo, const QString& name, const QString& balance, const QString& unconfirmed_balance, const QString& total_balance, const bool IsLocked, const bool IsMainWallet)
{
    if (row < 0 || row >= m_assets.count())
        return;

    m_assets.replace(row, {AssetNo, name, balance, unconfirmed_balance, total_balance, IsLocked, IsMainWallet});
    dataChanged(index(row, 0), index(row, 0), {no, Name, Balance, Unconfirmed_balance, Total_balance, IsLock, IsMainWallet});
}

void AssetModel::UpdateMainWalletStatus(const bool IsMainWallet)
{
    for(int i = 0; i < m_assets.size(); i++){
        m_assets[i].IsMainWallet = IsMainWallet;
        set(i, m_assets[i].no, m_assets[i].name, m_assets[i].balance, m_assets[i].unconfirmed_balance, m_assets[i].total_balance, m_assets[i].IsLock, m_assets[i].IsMainWallet);
    }
}

void AssetModel::UpdateAssetStatus(const int AssetNo)
{
    CoinAsset ca;
    if( !CoinAssetManager::Instance().GetAsset(AssetNo, ca)){
        return;
    }

    for(int i = 0; i < m_assets.size(); i++){
        if(m_assets[i].no == ca.no){
            m_assets[i].IsLock = ca.IsLock();
            set(i, m_assets[i].no, m_assets[i].name, m_assets[i].balance, m_assets[i].unconfirmed_balance,
                        m_assets[i].total_balance, m_assets[i].IsLock, m_assets[i].IsMainWallet);
            return;
        }
    }
}

void AssetModel::remove(int row)
{
    if (row < 0 || row >= m_assets.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_assets.removeAt(row);
    endRemoveRows();
}
