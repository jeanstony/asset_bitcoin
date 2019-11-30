#include <string>
#include <amount.h>
#include <wallet/walletdb.h>
#include <asset_coin.h>
#include <wallet/wallet.h>

std::shared_ptr<BerkeleyDatabase> GetDatabase(const WalletLocation& location, const int AssetNo);

void CoinAssetManager::GetMainCoinAsset(CoinAsset& MainAsset)
{
    MainAsset.no = 0;
    MainAsset.name = "VC";
    MainAsset.desc = "main asset";
    MainAsset.createAddr = "00000000000000000000000000000000000";
    MainAsset.coin = COIN;
    MainAsset.max = MAX_MONEY; 
    MainAsset.status = 0;
}

void CoinAssetManager::SetLocaltion(const WalletLocation &loc)
{
    m_loc = loc;
}

bool CoinAssetManager::AddCoinAsset(const CoinAsset& ca)
{
    LOCK(cs_Asset);

    std::vector<CoinAsset>::iterator i = std::find(m_CoinAssets.begin(), m_CoinAssets.end(), ca);
    if (i != m_CoinAssets.end()) {
        if (i->compare(ca)) {
            return false;
        } else {
            i->assign(ca);
            return true;
        }
    }

    m_CoinAssets.push_back(ca);
    return true;
}

size_t CoinAssetManager::GetAssetCount()
{
    LOCK(cs_Asset);
    return m_CoinAssets.size();
}

bool CoinAssetManager::RemoveCoinAsset(const int AssetNo)
{
    LOCK(cs_Asset);

    std::vector<CoinAsset>::iterator i = m_CoinAssets.begin();
    for (; i != m_CoinAssets.end(); ++i) {
        if (i->no == AssetNo) {
            m_CoinAssets.erase(i);
            return true;
        }
    }

    return false;
}

bool CoinAssetManager::IsExist(const int no)
{
    LOCK(cs_Asset);
    for (std::vector<CoinAsset>::const_iterator i = m_CoinAssets.begin(); i != m_CoinAssets.end(); i++) {
        if (i->no == no) {
            return true;
        }
    }

    return false;
}

bool CoinAssetManager::GetAsset(const int no, CoinAsset& ca)
{
    LOCK(cs_Asset);
    for (std::vector<CoinAsset>::const_iterator i = m_CoinAssets.begin(); i != m_CoinAssets.end(); i++) {
        if (i->no == no) {
            ca = *i;
            return true;
        }
    }

    return false;
}

int CoinAssetManager::GetAllAssets(std::vector<CoinAsset>& cas)
{
    LOCK(cs_Asset);
    cas = m_CoinAssets;
    return cas.size();
}

bool CoinAssetManager::LockCoinAsset(const int AssetNo)
{
    LOCK(cs_Asset);
    for (CoinAsset &i : m_CoinAssets) {
        if (i.no == AssetNo) {
            i.Lock();
            return true;
		}
	}

	return false;
}

bool CoinAssetManager::UnlockCoinAsset(const int AssetNo)
{
    LOCK(cs_Asset);
    for (CoinAsset &i : m_CoinAssets) {
        if (i.no == AssetNo) {
            i.Unlock();
            return true;
        }
    }

    return false;
}

bool CoinAssetManager::IsLockCoinAsset(const int AssetNo) const
{
    LOCK(cs_Asset);
    for (auto i : m_CoinAssets) {
        if (i.no == AssetNo) {
            return i.IsLock();
        }
    }

    return false;
}

unsigned int CoinAssetManager::UpdateCoinAssetStatus(const int AssetNo, const unsigned int status)
{
    LOCK(cs_Asset);

    for (CoinAsset &ca : m_CoinAssets) {
        if (ca.no == AssetNo) {
            return ca.UpdateStatus(status);
        }
    }

    return -1;
}

bool CoinAssetManager::WriteToDB(bool createflag)
{
    std::string cf;
    if (createflag) {
        cf = "cw";
    } else {
        cf = "w";
    }

    LOCK(cs_Asset);
    std::shared_ptr<BerkeleyDatabase> wd = GetDatabase(m_loc, -1);

    BerkeleyBatch batch(*wd, -1, cf.c_str());
    bool ret = batch.Erase("asset");
    ret = batch.Write("asset", *this);
    if (ret) {
        batch.Flush();
    }
    return ret;
}

bool CoinAssetManager::ReadFromDB(bool createflag)
{
    std::string cf;
    if (createflag) {
        cf = "cr";
    } else {
        cf = "r";
    }

    LOCK(cs_Asset);
    std::shared_ptr<BerkeleyDatabase> wd = GetDatabase(m_loc, -1);
    if (!fs::exists(m_loc.GetPath() / wd->GetFilePath())) {
        return false;
    }

    BerkeleyBatch batch(*wd, -1, cf.c_str());
    bool ret = batch.Read("asset", *this);
    if (!ret) {
        return ret;
    }

    if (m_CoinAssets.size() < 1) {
        return false;
    }

    CoinAsset MainAsset;
    GetMainCoinAsset(MainAsset);
    if (!m_CoinAssets[0].compare(MainAsset)) {
        return false;
    }

    return true;
}

void CoinAssetManager::StatGetAsset(const int AssetNo, CoinAssetStat *&pcas)
{
    for (CoinAssetStat& cas : m_CoinAssetStat) {
        if (cas.AssetNo == AssetNo) {
            pcas = &cas;
            return;
        }
    }

    CoinAssetStat newcas;
    newcas.AssetNo = AssetNo;
    m_CoinAssetStat.push_back(newcas);
    pcas = &m_CoinAssetStat.back();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int GetVInsAndFeeFromTx(const CTransaction& tx, std::vector<CTxIn>& vins, CTxIn *fee)
{
    if (tx.nAssetNo == 0) {
        for (auto in : tx.vin) {
            vins.push_back(in);
        }
    } else {
        for (size_t i = 0; i < tx.vin.size() - 1; i++) {
            vins.push_back(tx.vin[i]);
        }

        *fee = tx.vin[tx.vin.size() - 1];
        return tx.vin.size()-1;
    }

    return -1;
}

int GetVOutsAndFeeFromTx(const CTransaction& tx, std::vector<CTxOut>& vouts, CTxOut* fee)
{
    if (tx.nAssetNo == 0) {
        for (auto out : tx.vout) {
            vouts.push_back(out);
        }
    } else {
        for (size_t i = 0; i < tx.vout.size() - 1; i++) {
            vouts.push_back(tx.vout[i]);
        }

        *fee = tx.vout[tx.vout.size() - 1];
        return tx.vout.size() - 1;
    }

    return -1;
}

CAmount WorkoutFee(const CTransaction* tx)
{
    CAmount amount_in = 0;
    for (auto in : tx->vin) {
        auto cur = ::GetMainWallet()->mapWallet.find(in.prevout.hash);
        assert(cur != GetMainWallet()->mapWallet.end());
        assert(cur->second.tx->vout.size() > in.prevout.n);
        amount_in += cur->second.tx->vout[in.prevout.n].nValue;
    }

    CAmount amount_out = 0;
    for (auto out : tx->vout) {
        amount_out += out.nValue;
    }

    return amount_in - amount_out;
}
