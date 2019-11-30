// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <wallet/load.h>
#include <interfaces/chain.h>
#include <scheduler.h>
#include <util/system.h>
#include <wallet/wallet.h>

bool VerifyWallets(interfaces::Chain& chain, const std::vector<std::string>& wallet_files)
{
    assert(wallet_files.size() == 1);

    if (gArgs.IsArgSet("-walletdir")) {
        fs::path wallet_dir = gArgs.GetArg("-walletdir", "");
        boost::system::error_code error;
        // The canonical path cleans the path, preventing >1 Berkeley environment instances for the same directory
        fs::path canonical_wallet_dir = fs::canonical(wallet_dir, error);
        if (error || !fs::exists(wallet_dir)) {
            chain.initError(strprintf(_("Specified -walletdir \"%s\" does not exist"), wallet_dir.string()));
            return false;
        } else if (!fs::is_directory(wallet_dir)) {
            chain.initError(strprintf(_("Specified -walletdir \"%s\" is not a directory"), wallet_dir.string()));
            return false;
            // The canonical path transforms relative paths into absolute ones, so we check the non-canonical version
        } else if (!wallet_dir.is_absolute()) {
            chain.initError(strprintf(_("Specified -walletdir \"%s\" is a relative path"), wallet_dir.string()));
            return false;
        }
        gArgs.ForceSetArg("-walletdir", canonical_wallet_dir.string());
    }

    LogPrintf("Using wallet directory %s\n", GetWalletDir().string());

    chain.initMessage(_("Verifying wallet(s)..."));

    // Parameter interaction code should have thrown an error if -salvagewallet
    // was enabled with more than wallet file, so the wallet_files size check
    // here should have no effect.
    bool salvage_wallet = gArgs.GetBoolArg("-salvagewallet", false) && wallet_files.size() <= 1;

    // Keep track of each wallet absolute path to detect duplicates.
    std::set<fs::path> wallet_paths;

    for (const auto& wallet_file : wallet_files) {
        WalletLocation location(wallet_file);

        // Load all assets for verifing the trans-blocks
        CoinAssetManager::Instance().SetLocaltion(location);
        CoinAssetManager::Instance().ReadFromDB();

        if (!wallet_paths.insert(location.GetPath()).second) {
            chain.initError(strprintf(_("Error loading wallet %s. Duplicate -wallet filename specified."), wallet_file));
            return false;
        }

            std::string error_string;
            std::string warning_string;
            bool verify_success = CWallet::Verify(chain, location, salvage_wallet, error_string, warning_string);
            if (!error_string.empty()) chain.initError(error_string);
            if (!warning_string.empty()) chain.initWarning(warning_string);
            if (!verify_success) return false;
    }

    return true;
}

// fixed by luolin, the feature will be disabled.
bool LoadWallets(interfaces::Chain& chain, const std::vector<std::string>& wallet_files)
{
    assert(wallet_files.size() == 1);
     
    bool iscreate = false;
    WalletLocation loc = WalletLocation(*wallet_files.begin());
    std::shared_ptr<BerkeleyDatabase> wd = GetDatabase(loc, -1);
    if (!fs::exists(loc.GetPath() / wd->GetFilePath())) {
        iscreate = true;
    }

    CoinAssetManager::Instance().SetLocaltion(loc);
    if (!CoinAssetManager::Instance().ReadFromDB(iscreate)) {
        // old version? We will write the main asset to this wallet db
        CoinAssetManager::Instance().WriteToDB(iscreate);
    }
    std::vector<CoinAsset> asserts;
    CoinAssetManager::Instance().GetAllAssets(asserts);
    for (std::vector<CoinAsset>::const_iterator i = asserts.begin(); i != asserts.end(); i++) {
        std::shared_ptr<CWallet> pwallet = CWallet::CreateWalletFromFile(chain, loc, i->no, 0);
        if (!pwallet) {
            return false;
        }
        //AddWallet(pwallet);
        AddAssetWallet(pwallet);
    }
    /*
    for (const std::string& walletFile : wallet_files) {
        std::shared_ptr<CWallet> pwallet = CWallet::CreateWalletFromFile(chain, WalletLocation(walletFile));
        if (!pwallet) {
            return false;
        }
        AddWallet(pwallet);
    }
	*/
    return true;
}

void StartWallets(CScheduler& scheduler)
{
    for (const std::shared_ptr<CWallet>& pwallet : GetAssetWallets()) {
        pwallet->postInitProcess();
    }

    // Schedule periodic wallet flushes and tx rebroadcasts
    scheduler.scheduleEvery(MaybeCompactWalletDB, 500);
    scheduler.scheduleEvery(MaybeResendWalletTxs, 1000);
}

void FlushWallets()
{
    for (const std::shared_ptr<CWallet>& pwallet : GetAssetWallets()) {
        pwallet->Flush(false);
    }
}

void StopWallets()
{
    for (const std::shared_ptr<CWallet>& pwallet : GetAssetWallets()) {
        pwallet->Flush(true);
    }
}

void UnloadWallets()
{
    auto wallets = GetAssetWallets();
    while (!wallets.empty()) {
        auto wallet = wallets.back();
        wallets.pop_back();
        RemoveWallet(wallet);
        UnloadWallet(std::move(wallet));
    }
}
