#ifndef _asset_coin_h_
#define _asset_coin_h_

#include <amount.h>
#include <sync.h>
#include <wallet/walletutil.h>

#define ASSET_DISABLED (1)
#define ASSET_NOT_CREATE_TRANSACTION (1 << 1)
#define ASSET_NOT_SEND_TRANSACTION (1 << 2)

class WalletLocation;

// desc asset coin
struct CoinAsset {
    int no;                 // asset No.
    std::string name;
    std::string desc;       // desc
    std::string createAddr; //
    CAmount coin;           // percesion. such as: 100000 => 5
    CAmount max;            // max_money. 
    unsigned int status;    // 

	  void Lock()
	  {
          status = status | ASSET_DISABLED;
	  }

    unsigned int UpdateStatus(const unsigned int newStatus)
    {
        unsigned int oldStatus = status;
        status = newStatus;
        return oldStatus;
    }

	  void Unlock()
    {
        status = status & (~ASSET_DISABLED);
    }

    bool IsLock() const
    {
        return status & ASSET_DISABLED;
    }

    bool compare(const CoinAsset& ca) const
    {
        //assert(no == ca.no);
        return no == ca.no && name == ca.name && desc == ca.desc && coin == ca.coin && max == ca.max && status == ca.status && createAddr == ca.createAddr;
    }

    void assign(const CoinAsset& ca)
    {
        assert(no == ca.no);
        name = ca.name;
        desc = ca.desc;
        coin = ca.coin;
        max = ca.max;
        status = ca.status;
        createAddr = ca.createAddr;
    }

    bool operator==(const CoinAsset& ca) const
    {
        return no == ca.no;
    }

    bool operator<(const CoinAsset& ca) const
    {
        return no < ca.no;
    }

    template <typename Stream>
    inline void Serialize(Stream& s) const
    {
        s << no;
        s << name;
        s << desc;
        s << createAddr;
        s << coin;
        s << max;
        s << status;
    }

    template <typename Stream>
    inline void Unserialize(Stream& s)
    {
        s >> no;
        s >> name;
        s >> desc;
        s >> createAddr;
        s >> coin;
        s >> max;
        s >> status;
    }

    bool MoneyRange(const CAmount& nValue) { return (nValue >= 0 && nValue <= max); }
};

int GetAllAssetNo(std::vector<int>& assets, const std::string dbfilepath);

// mining stat
struct CoinAssetStat {
    CoinAssetStat()
        : AssetNo(0), Mininged(0), MiningedInToday(0), Destroy(0) {
        time(&Today);
    }

    int AssetNo;
    CAmount Mininged;
    CAmount Destroy;

    void AddAmountInToday(const CAmount amount, const time_t blocktime) {
        time_t cur;
        time(&cur);

        if (Today / (24 * 3600) != cur / (24 * 3600)) {
            Today = cur;
            MiningedInToday = 0;
        }

        if (cur / (24 * 3600) == blocktime / (24 * 3600)) {
            MiningedInToday += amount;
        }
    }
    CAmount MiningedInToday;
    time_t Today;
};

class CoinAssetManager
{
    friend int GetAllAssetNo(std::vector<int>& assets, const std::string dbfilepath);

public:
    CoinAssetManager()
    {
        CoinAsset MainAsset;
        GetMainCoinAsset(MainAsset);
        m_CoinAssets.push_back(MainAsset);
    }

    void GetMainCoinAsset(CoinAsset& MainAsset);

    static CoinAssetManager& Instance()
    {
        static CoinAssetManager me;
        return me;
    }

    void SetLocaltion(const WalletLocation &loc);

    bool AddCoinAsset(const CoinAsset& ca);
    bool RemoveCoinAsset(const int AssetNo);
    bool LockCoinAsset(const int AssetNo);
    bool UnlockCoinAsset(const int AssetNo);
    bool IsLockCoinAsset(const int AssetNo) const;
    unsigned int UpdateCoinAssetStatus(const int AssetNo, const unsigned int status);
    size_t GetAssetCount();

    template <typename Stream>
    inline void Serialize(Stream& s) const
    {
        s << m_CoinAssets.size();
        for (std::vector<CoinAsset>::const_iterator i = m_CoinAssets.begin(); i != m_CoinAssets.end(); i++) {
            s << (*i);
        }
    }

    template <typename Stream>
    inline void Unserialize(Stream& s)
    {
        m_CoinAssets.clear();

        size_t size;
        s >> size;

        CoinAsset tmp;
        for (size_t i = 0; i < size; i++) {
            s >> tmp;
            m_CoinAssets.push_back(tmp);
        }
    }

    bool WriteToDB(bool createflag = false);
    bool ReadFromDB(bool createflag = false);
    bool IsExist(const int no);
    bool GetAsset(const int no, CoinAsset& ca);
    int GetAllAssets(std::vector<CoinAsset>& cas);

    void StatAddMininged(const int AssetNo, const CAmount amount)
    {
        LOCK(cs_Asset);
        CoinAssetStat* pcas = nullptr;
        StatGetAsset(AssetNo, pcas);
        pcas->Mininged += amount;
    }
    void StatAddMiningedInToday(const int AssetNo, const CAmount amount, const time_t blocktime)
    {
        LOCK(cs_Asset);
        CoinAssetStat* pcas = nullptr;
        StatGetAsset(AssetNo, pcas);
        pcas->AddAmountInToday(amount, blocktime);
    }
    void StatAddDestroy(const int AssetNo, const CAmount amount)
    {
        LOCK(cs_Asset);
        CoinAssetStat* pcas = nullptr;
        StatGetAsset(AssetNo, pcas);
        pcas->Destroy += amount;
    }
    void StatGetAll(std::vector<CoinAssetStat> &cass)
    {
        LOCK(cs_Asset);
        cass = m_CoinAssetStat;
    }
    bool StatGet(const int AssetNo, CoinAssetStat& cas) {
        LOCK(cs_Asset);

        for (CoinAssetStat& i : m_CoinAssetStat) {
            if (i.AssetNo == AssetNo) {
                cas = i;
                return true;
            }
        }

        return false;
    }

private:
    mutable CCriticalSection cs_Asset;
    std::vector<CoinAsset> m_CoinAssets;
    WalletLocation m_loc;

    void StatGetAsset(const int AssetNo, CoinAssetStat*& pcas);
    std::vector<CoinAssetStat> m_CoinAssetStat;
};


#endif
