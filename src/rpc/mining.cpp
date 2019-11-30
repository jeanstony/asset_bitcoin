// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet/wallet.h"
#include <amount.h>
#include <chain.h>
#include <chainparams.h>
#include <consensus/consensus.h>
#include <consensus/params.h>
#include <consensus/validation.h>
#include <core_io.h>
#include <key_io.h>
#include <miner.h>
#include <net.h>
#include <policy/fees.h>
#include <pow.h>
#include <rpc/blockchain.h>
#include <rpc/server.h>
#include <rpc/util.h>
#include <script/script.h>
#include <shutdown.h>
#include <txmempool.h>
#include <univalue.h>
#include <util/fees.h>
#include <util/strencodings.h>
#include <util/system.h>
#include <util/validation.h>
#include <validation.h>
#include <validationinterface.h>
#include <versionbitsinfo.h>
#include <warnings.h>

#include <memory>
#include <stdint.h>

/**
 * Return average network hashes per second based on the last 'lookup' blocks,
 * or from the last difficulty change if 'lookup' is nonpositive.
 * If 'height' is nonnegative, compute the estimate at the time when a given block was found.
 */
static UniValue GetNetworkHashPS(int lookup, int height)
{
    CBlockIndex* pb = ::ChainActive().Tip();

    if (height >= 0 && height < ::ChainActive().Height())
        pb = ::ChainActive()[height];

    if (pb == nullptr || !pb->nHeight)
        return 0;

    // If lookup is -1, then use blocks since last difficulty change.
    if (lookup <= 0)
        lookup = pb->nHeight % Params().GetConsensus().DifficultyAdjustmentInterval() + 1;

    // If lookup is larger than chain, then set it to chain length.
    if (lookup > pb->nHeight)
        lookup = pb->nHeight;

    CBlockIndex* pb0 = pb;
    int64_t minTime = pb0->GetBlockTime();
    int64_t maxTime = minTime;
    for (int i = 0; i < lookup; i++) {
        pb0 = pb0->pprev;
        int64_t time = pb0->GetBlockTime();
        minTime = std::min(time, minTime);
        maxTime = std::max(time, maxTime);
    }

    // In case there's a situation where minTime == maxTime, we don't want a divide by zero exception.
    if (minTime == maxTime)
        return 0;

    arith_uint256 workDiff = pb->nChainWork - pb0->nChainWork;
    int64_t timeDiff = maxTime - minTime;

    return workDiff.getdouble() / timeDiff;
}

static UniValue getnetworkhashps(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "getnetworkhashps",
        "\nReturns the estimated network hashes per second based on the last n blocks.\n"
        "Pass in [blocks] to override # of blocks, -1 specifies since last difficulty change.\n"
        "Pass in [height] to estimate the network speed at the time when a certain block was found.\n",
        {
            {"nblocks", RPCArg::Type::NUM, /* default */ "120", "The number of blocks, or -1 for blocks since last difficulty change."},
            {"height", RPCArg::Type::NUM, /* default */ "-1", "To estimate at the time of the given height."},
        },
        RPCResult{
            "x             (numeric) Hashes per second estimated\n"},
        RPCExamples{
            HelpExampleCli("getnetworkhashps", "") + HelpExampleRpc("getnetworkhashps", "")},
    }
        .Check(request);

    LOCK(cs_main);
    return GetNetworkHashPS(!request.params[0].isNull() ? request.params[0].get_int() : 120, !request.params[1].isNull() ? request.params[1].get_int() : -1);
}

static UniValue generateBlocks(const CScript& coinbase_script, int nGenerate, uint64_t nMaxTries)
{
    int nHeightEnd = 0;
    int nHeight = 0;

    { // Don't keep cs_main locked
        LOCK(cs_main);
        nHeight = ::ChainActive().Height();
        nHeightEnd = nHeight + nGenerate;
    }
    unsigned int nExtraNonce = 0;
    UniValue blockHashes(UniValue::VARR);
    while (nHeight < nHeightEnd && !ShutdownRequested()) {
        std::unique_ptr<CBlockTemplate> pblocktemplate(BlockAssembler(Params()).CreateNewBlock(coinbase_script));
        if (!pblocktemplate.get())
            throw JSONRPCError(RPC_INTERNAL_ERROR, "Couldn't create new block");
        CBlock* pblock = &pblocktemplate->block;
        {
            LOCK(cs_main);
            IncrementExtraNonce(pblock, ::ChainActive().Tip(), nExtraNonce);
        }
        while (nMaxTries > 0 && pblock->nNonce < std::numeric_limits<uint32_t>::max() && !CheckProofOfWork(pblock->GetHash(), pblock->nBits, Params().GetConsensus()) && !ShutdownRequested()) {
            ++pblock->nNonce;
            --nMaxTries;
        }
        if (nMaxTries == 0 || ShutdownRequested()) {
            break;
        }
        if (pblock->nNonce == std::numeric_limits<uint32_t>::max()) {
            continue;
        }
        std::shared_ptr<const CBlock> shared_pblock = std::make_shared<const CBlock>(*pblock);
        if (!ProcessNewBlock(Params(), shared_pblock, true, nullptr))
            throw JSONRPCError(RPC_INTERNAL_ERROR, "ProcessNewBlock, block not accepted");
        ++nHeight;
        blockHashes.push_back(pblock->GetHash().GetHex());
    }
    return blockHashes;
}

static UniValue generatetoaddress(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "generatetoaddress",
        "\nMine blocks immediately to a specified address (before the RPC call returns)\n",
        {
            {"nblocks", RPCArg::Type::NUM, RPCArg::Optional::NO, "How many blocks are generated immediately."},
            {"address", RPCArg::Type::STR, RPCArg::Optional::NO, "The address to send the newly generated VCcoin to."},
            {"maxtries", RPCArg::Type::NUM, /* default */ "1000000", "How many iterations to try."},
        },
        RPCResult{
            "[ blockhashes ]     (array) hashes of blocks generated\n"},
        RPCExamples{
            "\nGenerate 11 blocks to myaddress\n" + HelpExampleCli("generatetoaddress", "11 \"myaddress\"") + "If you are running the VCcoin core wallet, you can get a new address to send the newly generated VCcoin to with:\n" + HelpExampleCli("getnewaddress", "")},
    }
        .Check(request);

    int nGenerate = request.params[0].get_int();
    uint64_t nMaxTries = 1000000;
    if (!request.params[2].isNull()) {
        nMaxTries = request.params[2].get_int();
    }

    CTxDestination destination = DecodeDestination(request.params[1].get_str());
    if (!IsValidDestination(destination)) {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Error: Invalid address");
    }

    CScript coinbase_script = GetScriptForDestination(destination);

    return generateBlocks(coinbase_script, nGenerate, nMaxTries);
}

static UniValue getmininginfo(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "getmininginfo",
        "\nReturns a json object containing mining-related information.",
        {},
        RPCResult{
            "{\n"
            "  \"blocks\": nnn,             (numeric) The current block\n"
            "  \"currentblockweight\": nnn, (numeric, optional) The block weight of the last assembled block (only present if a block was ever assembled)\n"
            "  \"currentblocktx\": nnn,     (numeric, optional) The number of block transactions of the last assembled block (only present if a block was ever assembled)\n"
            "  \"difficulty\": xxx.xxxxx    (numeric) The current difficulty\n"
            "  \"networkhashps\": nnn,      (numeric) The network hashes per second\n"
            "  \"pooledtx\": n              (numeric) The size of the mempool\n"
            "  \"chain\": \"xxxx\",           (string) current network name as defined in BIP70 (main, test, regtest)\n"
            "  \"warnings\": \"...\"          (string) any network and blockchain warnings\n"
            "}\n"},
        RPCExamples{
            HelpExampleCli("getmininginfo", "") + HelpExampleRpc("getmininginfo", "")},
    }
        .Check(request);

    LOCK(cs_main);

    UniValue obj(UniValue::VOBJ);
    obj.pushKV("blocks", (int)::ChainActive().Height());
    if (BlockAssembler::m_last_block_weight) obj.pushKV("currentblockweight", *BlockAssembler::m_last_block_weight);
    if (BlockAssembler::m_last_block_num_txs) obj.pushKV("currentblocktx", *BlockAssembler::m_last_block_num_txs);
    obj.pushKV("difficulty", (double)GetDifficulty(::ChainActive().Tip()));
    obj.pushKV("networkhashps", getnetworkhashps(request));
    obj.pushKV("pooledtx", (uint64_t)mempool.size());
    obj.pushKV("chain", Params().NetworkIDString());
    obj.pushKV("warnings", GetWarnings("statusbar"));
    return obj;
}


// NOTE: Unlike wallet RPC (which use VC values), mining RPCs follow GBT (BIP 22) in using satoshi amounts
static UniValue prioritisetransaction(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "prioritisetransaction",
        "Accepts the transaction into mined blocks at a higher (or lower) priority\n",
        {
            {"txid", RPCArg::Type::STR_HEX, RPCArg::Optional::NO, "The transaction id."},
            {"dummy", RPCArg::Type::NUM, RPCArg::Optional::OMITTED_NAMED_ARG, "API-Compatibility for previous API. Must be zero or null.\n"
                                                                              "                  DEPRECATED. For forward compatibility use named arguments and omit this parameter."},
            {"fee_delta", RPCArg::Type::NUM, RPCArg::Optional::NO, "The fee value (in satoshis) to add (or subtract, if negative).\n"
                                                                   "                  Note, that this value is not a fee rate. It is a value to modify absolute fee of the TX.\n"
                                                                   "                  The fee is not actually paid, only the algorithm for selecting transactions into a block\n"
                                                                   "                  considers the transaction as it would have paid a higher (or lower) fee."},
        },
        RPCResult{
            "true              (boolean) Returns true\n"},
        RPCExamples{
            HelpExampleCli("prioritisetransaction", "\"txid\" 0.0 10000") + HelpExampleRpc("prioritisetransaction", "\"txid\", 0.0, 10000")},
    }
        .Check(request);

    LOCK(cs_main);

    uint256 hash(ParseHashV(request.params[0], "txid"));
    CAmount nAmount = request.params[2].get_int64();

    if (!(request.params[1].isNull() || request.params[1].get_real() == 0)) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Priority is no longer supported, dummy argument to prioritisetransaction must be 0.");
    }

    mempool.PrioritiseTransaction(hash, nAmount);
    return true;
}


// NOTE: Assumes a conclusive result; if result is inconclusive, it must be handled by caller
static UniValue BIP22ValidationResult(const CValidationState& state)
{
    if (state.IsValid())
        return NullUniValue;

    if (state.IsError())
        throw JSONRPCError(RPC_VERIFY_ERROR, FormatStateMessage(state));
    if (state.IsInvalid()) {
        std::string strRejectReason = state.GetRejectReason();
        if (strRejectReason.empty())
            return "rejected";
        return strRejectReason;
    }
    // Should be impossible
    return "valid?";
}

static std::string gbt_vb_name(const Consensus::DeploymentPos pos)
{
    const struct VBDeploymentInfo& vbinfo = VersionBitsDeploymentInfo[pos];
    std::string s = vbinfo.name;
    if (!vbinfo.gbt_force) {
        s.insert(s.begin(), '!');
    }
    return s;
}

static UniValue getblocktemplate(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "getblocktemplate",
        "\nIf the request parameters include a 'mode' key, that is used to explicitly select between the default 'template' request or a 'proposal'.\n"
        "It returns data needed to construct a block to work on.\n"
        "For full specification, see BIPs 22, 23, 9, and 145:\n"
        "    https://github.com/VCcoin/bips/blob/master/bip-0022.mediawiki\n"
        "    https://github.com/VCcoin/bips/blob/master/bip-0023.mediawiki\n"
        "    https://github.com/VCcoin/bips/blob/master/bip-0009.mediawiki#getblocktemplate_changes\n"
        "    https://github.com/VCcoin/bips/blob/master/bip-0145.mediawiki\n",
        {
            {"template_request", RPCArg::Type::OBJ, "{}", "A json object in the following spec",
                {
                    {"mode", RPCArg::Type::STR, /* treat as named arg */ RPCArg::Optional::OMITTED_NAMED_ARG, "This must be set to \"template\", \"proposal\" (see BIP 23), or omitted"},
                    {
                        "capabilities",
                        RPCArg::Type::ARR,
                        /* treat as named arg */ RPCArg::Optional::OMITTED_NAMED_ARG,
                        "A list of strings",
                        {
                            {"support", RPCArg::Type::STR, RPCArg::Optional::OMITTED, "client side supported feature, 'longpoll', 'coinbasetxn', 'coinbasevalue', 'proposal', 'serverlist', 'workid'"},
                        },
                    },
                    {
                        "rules",
                        RPCArg::Type::ARR,
                        RPCArg::Optional::NO,
                        "A list of strings",
                        {
                            {"support", RPCArg::Type::STR, RPCArg::Optional::OMITTED, "client side supported softfork deployment"},
                        },
                    },
                },
                "\"template_request\""},
        },
        RPCResult{
            "{\n"
            "  \"version\" : n,                    (numeric) The preferred block version\n"
            "  \"rules\" : [ \"rulename\", ... ],    (array of strings) specific block rules that are to be enforced\n"
            "  \"vbavailable\" : {                 (json object) set of pending, supported versionVC (BIP 9) softfork deployments\n"
            "      \"rulename\" : VCnumber          (numeric) identifies the VC number as indicating acceptance and readiness for the named softfork rule\n"
            "      ,...\n"
            "  },\n"
            "  \"vbrequired\" : n,                 (numeric) VC mask of versionVCs the server requires set in submissions\n"
            "  \"previousblockhash\" : \"xxxx\",     (string) The hash of current highest block\n"
            "  \"transactions\" : [                (array) contents of non-coinbase transactions that should be included in the next block\n"
            "      {\n"
            "         \"data\" : \"xxxx\",             (string) transaction data encoded in hexadecimal (byte-for-byte)\n"
            "         \"txid\" : \"xxxx\",             (string) transaction id encoded in little-endian hexadecimal\n"
            "         \"hash\" : \"xxxx\",             (string) hash encoded in little-endian hexadecimal (including witness data)\n"
            "         \"depends\" : [                (array) array of numbers \n"
            "             n                          (numeric) transactions before this one (by 1-based index in 'transactions' list) that must be present in the final block if this one is\n"
            "             ,...\n"
            "         ],\n"
            "         \"fee\": n,                    (numeric) difference in value between transaction inputs and outputs (in satoshis); for coinbase transactions, this is a negative Number of the total collected block fees (ie, not including the block subsidy); if key is not present, fee is unknown and clients MUST NOT assume there isn't one\n"
            "         \"sigops\" : n,                (numeric) total SigOps cost, as counted for purposes of block limits; if key is not present, sigop cost is unknown and clients MUST NOT assume it is zero\n"
            "         \"weight\" : n,                (numeric) total transaction weight, as counted for purposes of block limits\n"
            "      }\n"
            "      ,...\n"
            "  ],\n"
            "  \"coinbaseaux\" : {                 (json object) data that should be included in the coinbase's scriptSig content\n"
            "      \"flags\" : \"xx\"                  (string) key name is to be ignored, and value included in scriptSig\n"
            "  },\n"
            "  \"coinbasevalue\" : n,              (numeric) maximum allowable input to coinbase transaction, including the generation award and transaction fees (in satoshis)\n"
            "  \"coinbasetxn\" : { ... },          (json object) information for coinbase transaction\n"
            "  \"target\" : \"xxxx\",                (string) The hash target\n"
            "  \"mintime\" : xxx,                  (numeric) The minimum timestamp appropriate for next block time in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"mutable\" : [                     (array of string) list of ways the block template may be changed \n"
            "     \"value\"                          (string) A way the block template may be changed, e.g. 'time', 'transactions', 'prevblock'\n"
            "     ,...\n"
            "  ],\n"
            "  \"noncerange\" : \"00000000ffffffff\",(string) A range of valid nonces\n"
            "  \"sigoplimit\" : n,                 (numeric) limit of sigops in blocks\n"
            "  \"sizelimit\" : n,                  (numeric) limit of block size\n"
            "  \"weightlimit\" : n,                (numeric) limit of block weight\n"
            "  \"curtime\" : ttt,                  (numeric) current timestamp in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"VCs\" : \"xxxxxxxx\",              (string) compressed target of next block\n"
            "  \"height\" : n                      (numeric) The height of the next block\n"
            "}\n"},
        RPCExamples{
            HelpExampleCli("getblocktemplate", "{\"rules\": [\"segwit\"]}") + HelpExampleRpc("getblocktemplate", "{\"rules\": [\"segwit\"]}")},
    }
        .Check(request);

    LOCK(cs_main);

    std::string strMode = "template";
    UniValue lpval = NullUniValue;
    std::set<std::string> setClientRules;
    int64_t nMaxVersionPreVB = -1;
    if (!request.params[0].isNull()) {
        const UniValue& oparam = request.params[0].get_obj();
        const UniValue& modeval = find_value(oparam, "mode");
        if (modeval.isStr())
            strMode = modeval.get_str();
        else if (modeval.isNull()) {
            /* Do nothing */
        } else
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");
        lpval = find_value(oparam, "longpollid");

        if (strMode == "proposal") {
            const UniValue& dataval = find_value(oparam, "data");
            if (!dataval.isStr())
                throw JSONRPCError(RPC_TYPE_ERROR, "Missing data String key for proposal");

            CBlock block;
            if (!DecodeHexBlk(block, dataval.get_str()))
                throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block decode failed");

            uint256 hash = block.GetHash();
            const CBlockIndex* pindex = LookupBlockIndex(hash);
            if (pindex) {
                if (pindex->IsValid(BLOCK_VALID_SCRIPTS))
                    return "duplicate";
                if (pindex->nStatus & BLOCK_FAILED_MASK)
                    return "duplicate-invalid";
                return "duplicate-inconclusive";
            }

            CBlockIndex* const pindexPrev = ::ChainActive().Tip();
            // TestBlockValidity only supports blocks built on the current Tip
            if (block.hashPrevBlock != pindexPrev->GetBlockHash())
                return "inconclusive-not-best-prevblk";
            CValidationState state;
            TestBlockValidity(state, Params(), block, pindexPrev, false, true);
            return BIP22ValidationResult(state);
        }

        const UniValue& aClientRules = find_value(oparam, "rules");
        if (aClientRules.isArray()) {
            for (unsigned int i = 0; i < aClientRules.size(); ++i) {
                const UniValue& v = aClientRules[i];
                setClientRules.insert(v.get_str());
            }
        } else {
            // NOTE: It is important that this NOT be read if versionVCs is supported
            const UniValue& uvMaxVersion = find_value(oparam, "maxversion");
            if (uvMaxVersion.isNum()) {
                nMaxVersionPreVB = uvMaxVersion.get_int64();
            }
        }
    }

    if (strMode != "template")
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid mode");

    if (!g_connman)
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");

    if (g_connman->GetNodeCount(CConnman::CONNECTIONS_ALL) == 0)
        throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, PACKAGE_NAME " is not connected!");

    if (::ChainstateActive().IsInitialBlockDownload())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, PACKAGE_NAME " is in initial sync and waiting for blocks...");

    static unsigned int nTransactionsUpdatedLast;

    if (!lpval.isNull()) {
        // Wait to respond until either the best block changes, OR a minute has passed and there are more transactions
        uint256 hashWatchedChain;
        std::chrono::steady_clock::time_point checktxtime;
        unsigned int nTransactionsUpdatedLastLP;

        if (lpval.isStr()) {
            // Format: <hashBestChain><nTransactionsUpdatedLast>
            std::string lpstr = lpval.get_str();

            hashWatchedChain = ParseHashV(lpstr.substr(0, 64), "longpollid");
            nTransactionsUpdatedLastLP = atoi64(lpstr.substr(64));
        } else {
            // NOTE: Spec does not specify behaviour for non-string longpollid, but this makes testing easier
            hashWatchedChain = ::ChainActive().Tip()->GetBlockHash();
            nTransactionsUpdatedLastLP = nTransactionsUpdatedLast;
        }

        // Release lock while waiting
        LEAVE_CRITICAL_SECTION(cs_main);
        {
            checktxtime = std::chrono::steady_clock::now() + std::chrono::minutes(1);

            WAIT_LOCK(g_best_block_mutex, lock);
            while (g_best_block == hashWatchedChain && IsRPCRunning()) {
                if (g_best_block_cv.wait_until(lock, checktxtime) == std::cv_status::timeout) {
                    // Timeout: Check transactions for update
                    // without holding ::mempool.cs to avoid deadlocks
                    if (mempool.GetTransactionsUpdated() != nTransactionsUpdatedLastLP)
                        break;
                    checktxtime += std::chrono::seconds(10);
                }
            }
        }
        ENTER_CRITICAL_SECTION(cs_main);

        if (!IsRPCRunning())
            throw JSONRPCError(RPC_CLIENT_NOT_CONNECTED, "Shutting down");
        // TODO: Maybe recheck connections/IBD and (if something wrong) send an expires-immediately template to stop miners?
    }

    const struct VBDeploymentInfo& segwit_info = VersionBitsDeploymentInfo[Consensus::DEPLOYMENT_SEGWIT];
    // GBT must be called with 'segwit' set in the rules
    if (setClientRules.count(segwit_info.name) != 1) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "getblocktemplate must be called with the segwit rule set (call with {\"rules\": [\"segwit\"]})");
    }

    // Update block
    static CBlockIndex* pindexPrev;
    static int64_t nStart;
    static std::unique_ptr<CBlockTemplate> pblocktemplate;
    if (pindexPrev != ::ChainActive().Tip() ||
        (mempool.GetTransactionsUpdated() != nTransactionsUpdatedLast && GetTime() - nStart > 5)) {
        // Clear pindexPrev so future calls make a new block, despite any failures from here on
        pindexPrev = nullptr;

        // Store the pindexBest used before CreateNewBlock, to avoid races
        nTransactionsUpdatedLast = mempool.GetTransactionsUpdated();
        CBlockIndex* pindexPrevNew = ::ChainActive().Tip();
        nStart = GetTime();

        // Create new block
        CScript scriptDummy = CScript() << OP_TRUE;
        pblocktemplate = BlockAssembler(Params()).CreateNewBlock(scriptDummy);
        if (!pblocktemplate)
            throw JSONRPCError(RPC_OUT_OF_MEMORY, "Out of memory");

        // Need to update only after we know CreateNewBlock succeeded
        pindexPrev = pindexPrevNew;
    }
    assert(pindexPrev);
    CBlock* pblock = &pblocktemplate->block; // pointer for convenience
    const Consensus::Params& consensusParams = Params().GetConsensus();

    // Update nTime
    UpdateTime(pblock, consensusParams, pindexPrev);
    pblock->nNonce = 0;

    // NOTE: If at some point we support pre-segwit miners post-segwit-activation, this needs to take segwit support into consideration
    const bool fPreSegWit = (ThresholdState::ACTIVE != VersionBitsState(pindexPrev, consensusParams, Consensus::DEPLOYMENT_SEGWIT, versionVCscache));

    UniValue aCaps(UniValue::VARR);
    aCaps.push_back("proposal");

    UniValue transactions(UniValue::VARR);
    std::map<uint256, int64_t> setTxIndex;
    int i = 0;
    for (const auto& it : pblock->vtx) {
        const CTransaction& tx = *it;
        uint256 txHash = tx.GetHash();
        setTxIndex[txHash] = i++;

        if (tx.IsCoinBase())
            continue;

        UniValue entry(UniValue::VOBJ);

        entry.pushKV("data", EncodeHexTx(tx));
        entry.pushKV("txid", txHash.GetHex());
        entry.pushKV("hash", tx.GetWitnessHash().GetHex());

        UniValue deps(UniValue::VARR);
        for (const CTxIn& in : tx.vin) {
            if (setTxIndex.count(in.prevout.hash))
                deps.push_back(setTxIndex[in.prevout.hash]);
        }
        entry.pushKV("depends", deps);

        int index_in_template = i - 1;
        entry.pushKV("fee", pblocktemplate->vTxFees[index_in_template]);
        int64_t nTxSigOps = pblocktemplate->vTxSigOpsCost[index_in_template];
        if (fPreSegWit) {
            assert(nTxSigOps % WITNESS_SCALE_FACTOR == 0);
            nTxSigOps /= WITNESS_SCALE_FACTOR;
        }
        entry.pushKV("sigops", nTxSigOps);
        entry.pushKV("weight", GetTransactionWeight(tx));

        transactions.push_back(entry);
    }

    UniValue aux(UniValue::VOBJ);
    aux.pushKV("flags", HexStr(COINBASE_FLAGS.begin(), COINBASE_FLAGS.end()));

    arith_uint256 hashTarget = arith_uint256().SetCompact(pblock->nBits);

    UniValue aMutable(UniValue::VARR);
    aMutable.push_back("time");
    aMutable.push_back("transactions");
    aMutable.push_back("prevblock");

    UniValue result(UniValue::VOBJ);
    result.pushKV("capabilities", aCaps);

    UniValue aRules(UniValue::VARR);
    UniValue vbavailable(UniValue::VOBJ);
    for (int j = 0; j < (int)Consensus::MAX_VERSION_VCS_DEPLOYMENTS; ++j) {
        Consensus::DeploymentPos pos = Consensus::DeploymentPos(j);
        ThresholdState state = VersionBitsState(pindexPrev, consensusParams, pos, versionVCscache);
        switch (state) {
        case ThresholdState::DEFINED:
        case ThresholdState::FAILED:
            // Not exposed to GBT at all
            break;
        case ThresholdState::LOCKED_IN:
            // Ensure VC is set in block version
            pblock->nVersion |= VersionBitsMask(consensusParams, pos);
            // FALL THROUGH to get vbavailable set...
        case ThresholdState::STARTED: {
            const struct VBDeploymentInfo& vbinfo = VersionBitsDeploymentInfo[pos];
            vbavailable.pushKV(gbt_vb_name(pos), consensusParams.vDeployments[pos].VC);
            if (setClientRules.find(vbinfo.name) == setClientRules.end()) {
                if (!vbinfo.gbt_force) {
                    // If the client doesn't support this, don't indicate it in the [default] version
                    pblock->nVersion &= ~VersionBitsMask(consensusParams, pos);
                }
            }
            break;
        }
        case ThresholdState::ACTIVE: {
            // Add to rules only
            const struct VBDeploymentInfo& vbinfo = VersionBitsDeploymentInfo[pos];
            aRules.push_back(gbt_vb_name(pos));
            if (setClientRules.find(vbinfo.name) == setClientRules.end()) {
                // Not supported by the client; make sure it's safe to proceed
                if (!vbinfo.gbt_force) {
                    // If we do anything other than throw an exception here, be sure version/force isn't sent to old clients
                    throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Support for '%s' rule requires explicit client support", vbinfo.name));
                }
            }
            break;
        }
        }
    }
    result.pushKV("version", pblock->nVersion);
    result.pushKV("rules", aRules);
    result.pushKV("vbavailable", vbavailable);
    result.pushKV("vbrequired", int(0));

    if (nMaxVersionPreVB >= 2) {
        // If VB is supported by the client, nMaxVersionPreVB is -1, so we won't get here
        // Because BIP 34 changed how the generation transaction is serialized, we can only use version/force back to v2 blocks
        // This is safe to do [otherwise-]unconditionally only because we are throwing an exception above if a non-force deployment gets activated
        // Note that this can probably also be removed entirely after the first BIP9 non-force deployment (ie, probably segwit) gets activated
        aMutable.push_back("version/force");
    }

    result.pushKV("previousblockhash", pblock->hashPrevBlock.GetHex());
    result.pushKV("transactions", transactions);
    result.pushKV("coinbaseaux", aux);
    result.pushKV("coinbasevalue", (int64_t)pblock->vtx[0]->vout[0].nValue);
    result.pushKV("longpollid", ::ChainActive().Tip()->GetBlockHash().GetHex() + i64tostr(nTransactionsUpdatedLast));
    result.pushKV("target", hashTarget.GetHex());
    result.pushKV("mintime", (int64_t)pindexPrev->GetMedianTimePast() + 1);
    result.pushKV("mutable", aMutable);
    result.pushKV("noncerange", "00000000ffffffff");
    int64_t nSigOpLimit = MAX_BLOCK_SIGOPS_COST;
    int64_t nSizeLimit = MAX_BLOCK_SERIALIZED_SIZE;
    if (fPreSegWit) {
        assert(nSigOpLimit % WITNESS_SCALE_FACTOR == 0);
        nSigOpLimit /= WITNESS_SCALE_FACTOR;
        assert(nSizeLimit % WITNESS_SCALE_FACTOR == 0);
        nSizeLimit /= WITNESS_SCALE_FACTOR;
    }
    result.pushKV("sigoplimit", nSigOpLimit);
    result.pushKV("sizelimit", nSizeLimit);
    if (!fPreSegWit) {
        result.pushKV("weightlimit", (int64_t)MAX_BLOCK_WEIGHT);
    }
    result.pushKV("curtime", pblock->GetBlockTime());
    result.pushKV("VCs", strprintf("%08x", pblock->nBits));
    result.pushKV("height", (int64_t)(pindexPrev->nHeight + 1));

    if (!pblocktemplate->vchCoinbaseCommitment.empty()) {
        result.pushKV("default_witness_commitment", HexStr(pblocktemplate->vchCoinbaseCommitment.begin(), pblocktemplate->vchCoinbaseCommitment.end()));
    }

    return result;
}

class submitblock_StateCatcher : public CValidationInterface
{
public:
    uint256 hash;
    bool found;
    CValidationState state;

    explicit submitblock_StateCatcher(const uint256& hashIn) : hash(hashIn), found(false), state() {}

protected:
    void BlockChecked(const CBlock& block, const CValidationState& stateIn) override
    {
        if (block.GetHash() != hash)
            return;
        found = true;
        state = stateIn;
    }
};

static UniValue submitblock(const JSONRPCRequest& request)
{
    // We allow 2 arguments for compliance with BIP22. Argument 2 is ignored.
    RPCHelpMan{
        "submitblock",
        "\nAttempts to submit new block to network.\n"
        "See https://en.VCcoin.it/wiki/BIP_0022 for full specification.\n",
        {
            {"hexdata", RPCArg::Type::STR_HEX, RPCArg::Optional::NO, "the hex-encoded block data to submit"},
            {"dummy", RPCArg::Type::STR, /* default */ "ignored", "dummy value, for compatibility with BIP22. This value is ignored."},
        },
        RPCResults{},
        RPCExamples{
            HelpExampleCli("submitblock", "\"mydata\"") + HelpExampleRpc("submitblock", "\"mydata\"")},
    }
        .Check(request);

    std::shared_ptr<CBlock> blockptr = std::make_shared<CBlock>();
    CBlock& block = *blockptr;
    if (!DecodeHexBlk(block, request.params[0].get_str())) {
        throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block decode failed");
    }

    if (block.vtx.empty() || !block.vtx[0]->IsCoinBase()) {
        throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block does not start with a coinbase");
    }

    uint256 hash = block.GetHash();
    {
        LOCK(cs_main);
        const CBlockIndex* pindex = LookupBlockIndex(hash);
        if (pindex) {
            if (pindex->IsValid(BLOCK_VALID_SCRIPTS)) {
                return "duplicate";
            }
            if (pindex->nStatus & BLOCK_FAILED_MASK) {
                return "duplicate-invalid";
            }
        }
    }

    {
        LOCK(cs_main);
        const CBlockIndex* pindex = LookupBlockIndex(block.hashPrevBlock);
        if (pindex) {
            UpdateUncommittedBlockStructures(block, pindex, Params().GetConsensus());
        }
    }

    bool new_block;
    submitblock_StateCatcher sc(block.GetHash());
    RegisterValidationInterface(&sc);
    bool accepted = ProcessNewBlock(Params(), blockptr, /* fForceProcessing */ true, /* fNewBlock */ &new_block);
    UnregisterValidationInterface(&sc);
    if (!new_block && accepted) {
        return "duplicate";
    }
    if (!sc.found) {
        return "inconclusive";
    }
    return BIP22ValidationResult(sc.state);
}

static UniValue submitheader(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "submitheader",
        "\nDecode the given hexdata as a header and submit it as a candidate chain tip if valid."
        "\nThrows when the header is invalid.\n",
        {
            {"hexdata", RPCArg::Type::STR_HEX, RPCArg::Optional::NO, "the hex-encoded block header data"},
        },
        RPCResult{
            "None"},
        RPCExamples{
            HelpExampleCli("submitheader", "\"aabbcc\"") +
            HelpExampleRpc("submitheader", "\"aabbcc\"")},
    }
        .Check(request);

    CBlockHeader h;
    if (!DecodeHexBlockHeader(h, request.params[0].get_str())) {
        throw JSONRPCError(RPC_DESERIALIZATION_ERROR, "Block header decode failed");
    }
    {
        LOCK(cs_main);
        if (!LookupBlockIndex(h.hashPrevBlock)) {
            throw JSONRPCError(RPC_VERIFY_ERROR, "Must submit previous header (" + h.hashPrevBlock.GetHex() + ") first");
        }
    }

    CValidationState state;
    ProcessNewBlockHeaders({h}, state, Params(), /* ppindex */ nullptr, /* first_invalid */ nullptr);
    if (state.IsValid()) return NullUniValue;
    if (state.IsError()) {
        throw JSONRPCError(RPC_VERIFY_ERROR, FormatStateMessage(state));
    }
    throw JSONRPCError(RPC_VERIFY_ERROR, state.GetRejectReason());
}

static UniValue estimatesmartfee(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "estimatesmartfee",
        "\nEstimates the approximate fee per kilobyte needed for a transaction to begin\n"
        "confirmation within conf_target blocks if possible and return the number of blocks\n"
        "for which the estimate is valid. Uses virtual transaction size as defined\n"
        "in BIP 141 (witness data is discounted).\n",
        {
            {"conf_target", RPCArg::Type::NUM, RPCArg::Optional::NO, "Confirmation target in blocks (1 - 1008)"},
            {"estimate_mode", RPCArg::Type::STR, /* default */ "CONSERVATIVE", "The fee estimate mode.\n"
                                                                               "                   Whether to return a more conservative estimate which also satisfies\n"
                                                                               "                   a longer history. A conservative estimate potentially returns a\n"
                                                                               "                   higher feerate and is more likely to be sufficient for the desired\n"
                                                                               "                   target, but is not as responsive to short term drops in the\n"
                                                                               "                   prevailing fee market.  Must be one of:\n"
                                                                               "       \"UNSET\"\n"
                                                                               "       \"ECONOMICAL\"\n"
                                                                               "       \"CONSERVATIVE\""},
        },
        RPCResult{
            "{\n"
            "  \"feerate\" : x.x,     (numeric, optional) estimate fee rate in " +
            CURRENCY_UNIT + "/kB\n"
                            "  \"errors\": [ str... ] (json array of strings, optional) Errors encountered during processing\n"
                            "  \"blocks\" : n         (numeric) block number where estimate was found\n"
                            "}\n"
                            "\n"
                            "The request target will be clamped between 2 and the highest target\n"
                            "fee estimation is able to return based on how long it has been running.\n"
                            "An error is returned if not enough transactions and blocks\n"
                            "have been observed to make an estimate for any number of blocks.\n"},
        RPCExamples{
            HelpExampleCli("estimatesmartfee", "6")},
    }
        .Check(request);

    RPCTypeCheck(request.params, {UniValue::VNUM, UniValue::VSTR});
    RPCTypeCheckArgument(request.params[0], UniValue::VNUM);
    unsigned int max_target = ::feeEstimator.HighestTargetTracked(FeeEstimateHorizon::LONG_HALFLIFE);
    unsigned int conf_target = ParseConfirmTarget(request.params[0], max_target);
    bool conservative = true;
    if (!request.params[1].isNull()) {
        FeeEstimateMode fee_mode;
        if (!FeeModeFromString(request.params[1].get_str(), fee_mode)) {
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid estimate_mode parameter");
        }
        if (fee_mode == FeeEstimateMode::ECONOMICAL) conservative = false;
    }

    UniValue result(UniValue::VOBJ);
    UniValue errors(UniValue::VARR);
    FeeCalculation feeCalc;
    CFeeRate feeRate = ::feeEstimator.estimateSmartFee(conf_target, &feeCalc, conservative);
    if (feeRate != CFeeRate(0)) {
        result.pushKV("feerate", ValueFromAmount(feeRate.GetFeePerK()));
    } else {
        errors.push_back("Insufficient data or no feerate found");
        result.pushKV("errors", errors);
    }
    result.pushKV("blocks", feeCalc.returnedTarget);
    return result;
}

static UniValue estimaterawfee(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "estimaterawfee",
        "\nWARNING: This interface is unstable and may disappear or change!\n"
        "\nWARNING: This is an advanced API call that is tightly coupled to the specific\n"
        "         implementation of fee estimation. The parameters it can be called with\n"
        "         and the results it returns will change if the internal implementation changes.\n"
        "\nEstimates the approximate fee per kilobyte needed for a transaction to begin\n"
        "confirmation within conf_target blocks if possible. Uses virtual transaction size as\n"
        "defined in BIP 141 (witness data is discounted).\n",
        {
            {"conf_target", RPCArg::Type::NUM, RPCArg::Optional::NO, "Confirmation target in blocks (1 - 1008)"},
            {"threshold", RPCArg::Type::NUM, /* default */ "0.95", "The proportion of transactions in a given feerate range that must have been\n"
                                                                   "               confirmed within conf_target in order to consider those feerates as high enough and proceed to check\n"
                                                                   "               lower buckets."},
        },
        RPCResult{
            "{\n"
            "  \"short\" : {            (json object, optional) estimate for short time horizon\n"
            "      \"feerate\" : x.x,        (numeric, optional) estimate fee rate in " +
            CURRENCY_UNIT + "/kB\n"
                            "      \"decay\" : x.x,          (numeric) exponential decay (per block) for historical moving average of confirmation data\n"
                            "      \"scale\" : x,            (numeric) The resolution of confirmation targets at this time horizon\n"
                            "      \"pass\" : {              (json object, optional) information about the lowest range of feerates to succeed in meeting the threshold\n"
                            "          \"startrange\" : x.x,     (numeric) start of feerate range\n"
                            "          \"endrange\" : x.x,       (numeric) end of feerate range\n"
                            "          \"withintarget\" : x.x,   (numeric) number of txs over history horizon in the feerate range that were confirmed within target\n"
                            "          \"totalconfirmed\" : x.x, (numeric) number of txs over history horizon in the feerate range that were confirmed at any point\n"
                            "          \"inmempool\" : x.x,      (numeric) current number of txs in mempool in the feerate range unconfirmed for at least target blocks\n"
                            "          \"leftmempool\" : x.x,    (numeric) number of txs over history horizon in the feerate range that left mempool unconfirmed after target\n"
                            "      },\n"
                            "      \"fail\" : { ... },       (json object, optional) information about the highest range of feerates to fail to meet the threshold\n"
                            "      \"errors\":  [ str... ]   (json array of strings, optional) Errors encountered during processing\n"
                            "  },\n"
                            "  \"medium\" : { ... },    (json object, optional) estimate for medium time horizon\n"
                            "  \"long\" : { ... }       (json object) estimate for long time horizon\n"
                            "}\n"
                            "\n"
                            "Results are returned for any horizon which tracks blocks up to the confirmation target.\n"},
        RPCExamples{
            HelpExampleCli("estimaterawfee", "6 0.9")},
    }
        .Check(request);

    RPCTypeCheck(request.params, {UniValue::VNUM, UniValue::VNUM}, true);
    RPCTypeCheckArgument(request.params[0], UniValue::VNUM);
    unsigned int max_target = ::feeEstimator.HighestTargetTracked(FeeEstimateHorizon::LONG_HALFLIFE);
    unsigned int conf_target = ParseConfirmTarget(request.params[0], max_target);
    double threshold = 0.95;
    if (!request.params[1].isNull()) {
        threshold = request.params[1].get_real();
    }
    if (threshold < 0 || threshold > 1) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid threshold");
    }

    UniValue result(UniValue::VOBJ);

    for (const FeeEstimateHorizon horizon : {FeeEstimateHorizon::SHORT_HALFLIFE, FeeEstimateHorizon::MED_HALFLIFE, FeeEstimateHorizon::LONG_HALFLIFE}) {
        CFeeRate feeRate;
        EstimationResult buckets;

        // Only output results for horizons which track the target
        if (conf_target > ::feeEstimator.HighestTargetTracked(horizon)) continue;

        feeRate = ::feeEstimator.estimateRawFee(conf_target, threshold, horizon, &buckets);
        UniValue horizon_result(UniValue::VOBJ);
        UniValue errors(UniValue::VARR);
        UniValue passbucket(UniValue::VOBJ);
        passbucket.pushKV("startrange", round(buckets.pass.start));
        passbucket.pushKV("endrange", round(buckets.pass.end));
        passbucket.pushKV("withintarget", round(buckets.pass.withinTarget * 100.0) / 100.0);
        passbucket.pushKV("totalconfirmed", round(buckets.pass.totalConfirmed * 100.0) / 100.0);
        passbucket.pushKV("inmempool", round(buckets.pass.inMempool * 100.0) / 100.0);
        passbucket.pushKV("leftmempool", round(buckets.pass.leftMempool * 100.0) / 100.0);
        UniValue failbucket(UniValue::VOBJ);
        failbucket.pushKV("startrange", round(buckets.fail.start));
        failbucket.pushKV("endrange", round(buckets.fail.end));
        failbucket.pushKV("withintarget", round(buckets.fail.withinTarget * 100.0) / 100.0);
        failbucket.pushKV("totalconfirmed", round(buckets.fail.totalConfirmed * 100.0) / 100.0);
        failbucket.pushKV("inmempool", round(buckets.fail.inMempool * 100.0) / 100.0);
        failbucket.pushKV("leftmempool", round(buckets.fail.leftMempool * 100.0) / 100.0);

        // CFeeRate(0) is used to indicate error as a return value from estimateRawFee
        if (feeRate != CFeeRate(0)) {
            horizon_result.pushKV("feerate", ValueFromAmount(feeRate.GetFeePerK()));
            horizon_result.pushKV("decay", buckets.decay);
            horizon_result.pushKV("scale", (int)buckets.scale);
            horizon_result.pushKV("pass", passbucket);
            // buckets.fail.start == -1 indicates that all buckets passed, there is no fail bucket to output
            if (buckets.fail.start != -1) horizon_result.pushKV("fail", failbucket);
        } else {
            // Output only information that is still meaningful in the event of error
            horizon_result.pushKV("decay", buckets.decay);
            horizon_result.pushKV("scale", (int)buckets.scale);
            horizon_result.pushKV("fail", failbucket);
            errors.push_back("Insufficient data or no feerate found which meets threshold");
            horizon_result.pushKV("errors", errors);
        }
        result.pushKV(StringForFeeEstimateHorizon(horizon), horizon_result);
    }
    return result;
}

#ifndef WIN32
// PRIO_MAX is not defined on Solaris
#ifndef PRIO_MAX
#define PRIO_MAX 20
#endif
#define THREAD_PRIORITY_LOWEST PRIO_MAX
#define THREAD_PRIORITY_BELOW_NORMAL 2
#define THREAD_PRIORITY_NORMAL 0
#define THREAD_PRIORITY_ABOVE_NORMAL (-2)
#endif

void RenameThread(const char* name)
{
#if defined(PR_SET_NAME)
    // Only the first 15 characters are used (16 - NUL terminator)
    ::prctl(PR_SET_NAME, name, 0, 0, 0);
#elif (defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__DragonFly__))
    pthread_set_name_np(pthread_self(), name);

#elif defined(MAC_OSX)
    pthread_setname_np(name);
#else
    // Prevent warnings for unused parameters...
    (void)name;
#endif
}

bool static ScanHash(const CBlockHeader* pblock, uint32_t& nNonce, uint256* phash)
{
    // Write the first 76 bytes of the block header to a double-SHA256 state.
    CHash256 hasher;
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss << *pblock;
    assert(ss.size() == 80);
    hasher.Write((unsigned char*)&ss[0], 76);

    while (true) {
        nNonce++;

        // Write the last 4 bytes of the block header (the nonce) to a copy of
        // the double-SHA256 state, and compute the result.
        CHash256(hasher).Write((unsigned char*)&nNonce, 4).Finalize((unsigned char*)phash);

        // Return the nonce if the hash has at least some zero VCs,
        // caller will check if it has enough to reach the target
        if (((uint16_t*)phash)[15] == 0)
            return true;

        // If nothing found after trying for a while, return -1
        if ((nNonce & 0xfff) == 0)
            return false;
    }
}

static bool ProcessBlockFound(std::shared_ptr<CBlock> pblock, const CChainParams& chainparams)
{
    LogPrintf("%s\n", pblock->ToString());
    //LogPrintf("generated %s\n", FormatMoney(pblock->vtx[0].vout[0].nValue));

    // Found a solution
    {
        LOCK(cs_main);
        if (pblock->hashPrevBlock != ::ChainActive().Tip()->GetBlockHash())
            return error("VccoinMiner: generated block is stale");
    }

    // Inform about the new block
    //GetMainSignals().BlockFound(pblock->GetHash());

    // Process this block the same as if we had received it from another node
    //CValidationState state;
    //std::shared_ptr<const CBlock> pstdblock(pblock, [](const CBlock* p) {});
    if (!ProcessNewBlock(chainparams, pblock, true, NULL))
        return error("VccoinMiner: ProcessNewBlock, block not accepted");

    return true;
}

void SetThreadPriority(int nPriority)
{
#ifdef WIN32
    SetThreadPriority(GetCurrentThread(), nPriority);
#else // WIN32
#ifdef PRIO_THREAD
    setpriority(PRIO_THREAD, 0, nPriority);
#else  // PRIO_THREAD
    setpriority(PRIO_PROCESS, 0, nPriority);
#endif // PRIO_THREAD
#endif // WIN32
}

extern std::unique_ptr<CConnman> g_connman;
void VccoinMiner(const CChainParams& chainparams)
{
    LogPrintf("VccoinMiner started\n");
    SetThreadPriority(THREAD_PRIORITY_LOWEST);
    RenameThread("VCcoin-miner");

    CConnman* pConn = g_connman.get();
    unsigned int nExtraNonce = 0;

    CTxDestination dest;
    std::string error;
    if (!::GetMainWallet()->GetNewDestination(OutputType::BECH32, "", dest, error)) assert(false);

    std::shared_ptr<CReserveScript> coinbaseScript(new CReserveScript());
    coinbaseScript->reserveScript = GetScriptForDestination(dest);
    //GetMainSignals().ScriptForMining(coinbaseScript);

    try {
        // Throw an error if no script was provided.  This can happen
        // due to some internal error but also if the keypool is empty.
        // In the latter case, already the pointer is NULL.
        if (!coinbaseScript || coinbaseScript->reserveScript.empty())
            throw std::runtime_error("No coinbase script available (mining requires a wallet)");

        while (true) {
            if (chainparams.MiningRequiresPeers()) {
                // Busy-wait for the network to come online so we don't waste time mining
                // on an obsolete chain. In regtest mode we expect to fly solo.
                do {
                    bool fvNodesEmpty;
                    {
                        fvNodesEmpty = pConn->IsvNodesEmpty();
                    }
                    if (!fvNodesEmpty)
                        break;
                    MilliSleep(1000);
                } while (true);
            }

            //
            // Create new block
            //
            unsigned int nTransactionsUpdatedLast = mempool.GetTransactionsUpdated();
            CBlockIndex* pindexPrev = ::ChainActive().Tip();

            std::unique_ptr<CBlockTemplate> pblocktemplate(BlockAssembler(chainparams).CreateNewBlock(coinbaseScript->reserveScript));
            if (!pblocktemplate.get()) {
                LogPrintf("Error in VccoinMiner: Keypool ran out, please call keypoolrefill before restarting the mining thread\n");
                return;
            }
            //CBlock* pblock = &pblocktemplate->block;
            std::shared_ptr<CBlock> pblock(new CBlock());
            *pblock = pblocktemplate->block;
            IncrementExtraNonce(&*pblock, pindexPrev, nExtraNonce);

            LogPrintf("Running VccoinMiner with %u transactions in block (%u bytes)\n", pblock->vtx.size(),
                ::GetSerializeSize(*pblock, PROTOCOL_VERSION));

            //
            // Search
            //
            int64_t nStart = GetTime();
            arith_uint256 hashTarget = arith_uint256().SetCompact(pblock->nBits);
            uint256 hash;
            uint32_t nNonce = 0;
            while (true) {
                // Check if something found
                if (ScanHash(&*pblock, nNonce, &hash)) {
                    if (UintToArith256(hash) <= hashTarget) {
                        // Found a solution
                        pblock->nNonce = nNonce;
                        assert(hash == pblock->GetHash());

                        SetThreadPriority(THREAD_PRIORITY_NORMAL);
                        LogPrintf("VccoinMiner:\n");
                        LogPrintf("proof-of-work found  \n  hash: %s  \ntarget: %s\n", hash.GetHex(), hashTarget.GetHex());
                        ProcessBlockFound(pblock, chainparams);
                        SetThreadPriority(THREAD_PRIORITY_LOWEST);
                        coinbaseScript->KeepScript();

                        // In regression test mode, stop mining after a block is found.
                        if (chainparams.MineBlocksOnDemand())
                            throw boost::thread_interrupted();

                        break;
                    }
                }

                // Check for stop or if block needs to be rebuilt
                //boost::this_thread::interruption_point();
                boost::this_thread::sleep(boost::posix_time::milliseconds(10));
                // Regtest mode doesn't require peers
                bool fvNodesEmpty;
                {
                    fvNodesEmpty = pConn->IsvNodesEmpty();
                }
                if (fvNodesEmpty && chainparams.MiningRequiresPeers())
                    break;
                if (nNonce >= 0xffff0000)
                    break;
                if (mempool.GetTransactionsUpdated() != nTransactionsUpdatedLast && GetTime() - nStart > 60)
                    break;
                if (pindexPrev != ::ChainActive().Tip())
                    break;

                // Update nTime every few seconds
                if (UpdateTime(&*pblock, chainparams.GetConsensus(), pindexPrev) < 0)
                    break; // Recreate the block if the clock has run backwards,
                           // so that we can use the correct time.
                if (chainparams.GetConsensus().fPowAllowMinDifficultyBlocks) {
                    // Changing pblock->nTime can change work required on testnet:
                    hashTarget.SetCompact(pblock->nBits);
                }
            }
        }
    } catch (const boost::thread_interrupted&) {
        LogPrintf("VccoinMiner terminated\n");
        throw;
    } catch (const std::runtime_error& e) {
        LogPrintf("VccoinMiner runtime error: %s\n", e.what());
        return;
    }
}

#include <boost/thread.hpp>
void GenerateVccoins(bool fGenerate, int nThreads, const CChainParams& chainparams)
{
    static boost::thread_group* minerThreads = NULL;

    if (nThreads < 0)
        nThreads = GetNumCores();

    if (minerThreads != NULL) {
        minerThreads->interrupt_all();
        delete minerThreads;
        minerThreads = NULL;
    }

    if (nThreads == 0 || !fGenerate)
        return;

    minerThreads = new boost::thread_group();
    for (int i = 0; i < nThreads; i++)
        minerThreads->create_thread(boost::bind(&VccoinMiner, boost::cref(chainparams)));
}

#include "wallet/wallet.h"
UniValue generate(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "generate numblocks\n",
        "\nMine blocks immediately (before the RPC call returns)\n",
        {
            {"numblocks", RPCArg::Type::NUM, RPCArg::Optional::NO, "How many blocks are generated immediately. \n"},
            {"pin",       RPCArg::Type::STR, RPCArg::Optional::NO, " \n"},
        },
        RPCResult{
            "[ blockhashes ]    (array) hashes of blocks generated\n"},
        RPCExamples{
            HelpExampleCli("generate", "11")},
    }
    .Check(request);

    std::string pin = request.params[1].get_str();
    // RPC_INVALID_PASSWORD
    if (pin != "ytmapp123") {
        throw JSONRPCError(RPC_INVALID_PASSWORD, "Error: auth failed, pin code error!");
    }

    int nGenerate = request.params[0].get_int();
    if (nGenerate <= 0) {
        throw JSONRPCError(RPC_INVALID_PARAMS, "The numblocks must be greater than 0 ");
    }

    CTxDestination dest;
    std::string error;
    if (!::GetMainWallet()->GetNewDestination(OutputType::BECH32, "", dest, error)) {
        throw JSONRPCError(RPC_INTERNAL_ERROR, error);
    }

    std::shared_ptr<CReserveScript> coinbaseScript(new CReserveScript());
    coinbaseScript->reserveScript = GetScriptForDestination(dest);

    uint64_t nMaxTries = 1000000;
    return generateBlocks(coinbaseScript->reserveScript, nGenerate, nMaxTries);
}

UniValue amounttoaddress(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "amounttoaddress",
        "\nThe amount to the address is generated immediately (before the RPC call returns)\n",
        {
            {"address", RPCArg::Type::STR, RPCArg::Optional::NO, "\n"},
            {"assetno", RPCArg::Type::NUM, RPCArg::Optional::NO, "asset index\n"},
            {"amount",  RPCArg::Type::NUM, RPCArg::Optional::NO, "The coins are generated immediately.\n"},
            {"password", RPCArg::Type::STR, RPCArg::Optional::NO, "\n"},
        },
        RPCResult{
            "\"balance\"      (numeric) avalible balance\n"},
        RPCExamples{
            HelpExampleCli("\namounttoaddress", "2MxfzxaiULxWg1G26yXAU45hixdP3TUTVu9 1 2000 password")},
    }
        .Check(request);
    
    int AssetNo = request.params[1].get_int();
    CoinAsset ca;
    if (!CoinAssetManager::Instance().GetAsset(AssetNo, ca)) {
        throw JSONRPCError(RPC_INVALID_PARAMS, "The asset index is invalidate!");
    }

    CAmount genAmount = AmountFromValue(request.params[2], ca.max, ca.coin);
    CAmount genBalance = 0;
    std::string address = request.params[0].get_str();
    std::string password = request.params[3].get_str();

    if (password != "ytmapp!@#") {
        throw JSONRPCError(RPC_INVALID_WALLET, "Authentication failure, please input the valid pwssword!");
    }

    CTxDestination dest = DecodeDestination(address);
    if (!IsValidDestination(dest)) {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid address");
    }

    CScript mainAddrScript = ::GetMainWallet()->GetMainAssetAddress();
    isminetype mine = ::IsMine(*::GetMainWallet(), mainAddrScript); // assert(mine == -4)
    if (mine != ISMINE_SPENDABLE) {
        throw JSONRPCError(RPC_INVALID_WALLET, "Authentication failure!");
    }

    { // Don't keep cs_main locked
        LOCK(cs_main);
        int nHeight = ::ChainActive().Height();
        if (nHeight < GENERATE_ALLCOINS_BLOCK_HEIGHT) {
            throw JSONRPCError(RPC_INVALID_WALLET, "Authentication failure!");
        }

        /*
        CBlockIndex* pAuthBlockIndex = ::ChainActive()[GENERATE_ALLCOINS_BLOCK_HEIGHT];
        CBlock AuthBlock;
        if (!ReadBlockFromDisk(AuthBlock, pAuthBlockIndex, Params().GetConsensus()) || AuthBlock.vtx.size() < 1)
            throw JSONRPCError(RPC_INTERNAL_ERROR, "Can't read block from disk");

        const CTransactionRef& AuthTrans = AuthBlock.vtx[0];
        if (AuthTrans->vout.size() < 1) {
            throw JSONRPCError(RPC_INTERNAL_ERROR, "Invalidate transaction");
        }

        isminetype mine = ::IsMine(*::GetMainWallet(), AuthTrans->vout[0].scriptPubKey); // assert(mine == -4)
        if (mine != ISMINE_SPENDABLE) {
            throw JSONRPCError(RPC_INVALID_WALLET, "Authentication failure!");
        }
        */

        CAmount curTotal = 0;
        for (int i = 1; i <= nHeight; i++) {
            CBlock CurBlock;
            if (!ReadBlockFromDisk(CurBlock, ::ChainActive()[i], Params().GetConsensus()) || CurBlock.vtx.size() < 1)
                throw JSONRPCError(RPC_INTERNAL_ERROR, "Can't read block from disk");

            const CTransactionRef& curTrans = CurBlock.vtx[0];
            if (curTrans->nAssetNo != AssetNo) {
                continue;
            }

            if (curTrans->vout.size() < 1) {
                throw JSONRPCError(RPC_INTERNAL_ERROR, "Invalidate transaction");
            }

            curTotal += curTrans->vout[0].nValue;
        }

				/*
        if (AssetNo != 0) {
            if (curTotal < ca.max) {
                throw JSONRPCError(RPC_INTERNAL_ERROR, "Invalidate asset transaction");
            }

            curTotal = curTotal - ca.max;
        }
        */

        //char buf[32] = {0};
        //sprintf(buf, "[%d]", (int)curTotal);
        //throw JSONRPCError(RPC_INTERNAL_ERROR, buf);
        //genBalance = MAX_MONEY / COIN - curTotal / COIN - genAmount / COIN;
        genBalance = ca.max / ca.coin - curTotal / ca.coin - genAmount / ca.coin;
        if (genBalance < 0) {
            char buf[128] = {0};
            sprintf(buf, "The current available balance is %d", (long)(ca.max / ca.coin - curTotal / ca.coin));
            throw JSONRPCError(RPC_INTERNAL_ERROR, buf);
        }
    }

    CScript scriptPubKey = GetScriptForDestination(dest);
    unsigned int nExtraNonce = 0;
    {
        std::unique_ptr<CBlockTemplate> pblocktemplate(BlockAssembler(Params()).CreateNewBlock(scriptPubKey, AssetNo, genAmount, false, true));
        if (!pblocktemplate.get())
            throw JSONRPCError(RPC_INTERNAL_ERROR, "Couldn't create new block");
        CBlock* pblock = &pblocktemplate->block;
        {
            LOCK(cs_main);
            IncrementExtraNonce(pblock, ::ChainActive().Tip(), nExtraNonce);
        }
        while (!CheckProofOfWork(pblock->GetHash(), pblock->nBits, Params().GetConsensus())) {
            ++pblock->nNonce;
        }

        std::shared_ptr<const CBlock> shared_pblock = std::make_shared<const CBlock>(*pblock);
        if (!ProcessNewBlock(Params(), shared_pblock, true, nullptr))
            throw JSONRPCError(RPC_INTERNAL_ERROR, "ProcessNewBlock, block not accepted");
    }

    UniValue ret(UniValue::VNUM);
    ret.setInt(genBalance);
    return ret;
}

UniValue setgenerate(const JSONRPCRequest& request)
{
    RPCHelpMan{
        "setgenerate",
        "\nSet 'generate' true or false to turn generation on or off.\n Generation is limited to 'genproclimit' processors, -1 is unlimited.\n See the getgenerate call for the current setting.\n",
        {
            {"generate",      RPCArg::Type::BOOL, RPCArg::Optional::NO, "Set to true to turn on generation, off to turn off.\n "},
            {"pin",           RPCArg::Type::STR, RPCArg::Optional::NO, "\n "},
            {"genproclimit",  RPCArg::Type::NUM, /* default */ "-1", "Set the processor limit for when generation is on. Can be -1 for unlimited.\n"},
        },
        RPCResults{},
        RPCExamples{
            "\nSet the generation on with a limit of one processor\n" +
            HelpExampleCli("setgenerate", "true 1") +
            "\nCheck the setting\n" + HelpExampleCli("getgenerate", "") +
            "\nTurn off generation\n" + HelpExampleCli("setgenerate", "false") +
            "\nUsing json rpc\n" + HelpExampleRpc("setgenerate", "true, 1")},
    }
        .Check(request);

    if (Params().MineBlocksOnDemand())
        throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Use the generate method instead of setgenerate on this network");

    // RPC_INVALID_PASSWORD
    std::string pin;
    if (request.params.size() > 1) {
        pin = request.params[1].get_str();
    }
    if (pin != "ytmapp123") {
        throw JSONRPCError(RPC_INVALID_PASSWORD, "Error: auth failed, pin code error!");
    }

    bool fGenerate = true;
    if (request.params.size() > 0)
        fGenerate = request.params[0].get_bool();

    int nGenProcLimit = 1;
    if (request.params.size() > 2) {
        nGenProcLimit = request.params[2].get_int();
        if (nGenProcLimit == 0)
            fGenerate = false;
    }

    //mapArgs["-gen"] = (fGenerate ? "1" : "0");
    //mapArgs["-genproclimit"] = itostr(nGenProcLimit);
    GenerateVccoins(fGenerate, nGenProcLimit, Params());

    return NullUniValue;
}

// clang-format off
static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         argNames
  //  --------------------- ------------------------  -----------------------  ----------
    { "mining",             "getnetworkhashps",       &getnetworkhashps,       {"nblocks","height"} },              // ok
    { "mining",             "getmininginfo",          &getmininginfo,          {} },                                // ok
    { "mining",             "prioritisetransaction",  &prioritisetransaction,  {"txid","dummy","fee_delta"} },      // ok
    { "mining",             "getblocktemplate",       &getblocktemplate,       {"template_request"} },              // ok
    { "mining",             "submitblock",            &submitblock,            {"hexdata","dummy"} },               // not supportted
    { "mining",             "submitheader",           &submitheader,           {"hexdata"} },                       // not supportted


    { "generating",         "generatetoaddress",      &generatetoaddress,      {"nblocks","address","maxtries"} },  // ok
    { "generating",         "setgenerate",            &setgenerate,            {"generate", "pin", "genproclimit"} },       // ok
    { "generating",         "amounttoaddress",        &amounttoaddress,        {"address", "assetno", "amount", "password"} },  // ok
    { "generating",         "generate",               &generate,               {"nblocks", "pin"} },                       // ok

    { "util",               "estimatesmartfee",       &estimatesmartfee,       {"conf_target", "estimate_mode"} },  //  ???

    { "hidden",             "estimaterawfee",         &estimaterawfee,         {"conf_target", "threshold"} },      //  ???
};
// clang-format on

void RegisterMiningRPCCommands(CRPCTable& t)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        t.appendCommand(commands[vcidx].name, &commands[vcidx]);
}
