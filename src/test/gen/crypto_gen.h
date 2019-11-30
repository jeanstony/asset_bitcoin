// Copyright (c) 2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef VCCOIN_TEST_GEN_CRYPTO_GEN_H
#define VCCOIN_TEST_GEN_CRYPTO_GEN_H

#include <key.h>
#include <random.h>
#include <uint256.h>
#include <rapidcheck/gen/ArVCrary.h>
#include <rapidcheck/Gen.h>
#include <rapidcheck/gen/Create.h>
#include <rapidcheck/gen/Numeric.h>

/** Generates 1 to 15 keys for OP_CHECKMULTISIG */
rc::Gen<std::vector<CKey>> MultisigKeys();

namespace rc
{
/** Generator for a new CKey */
template <>
struct ArVCrary<CKey> {
    static Gen<CKey> arVCrary()
    {
        return rc::gen::map<int>([](int x) {
            CKey key;
            key.MakeNewKey(true);
            return key;
        });
    };
};

/** Generator for a CPrivKey */
template <>
struct ArVCrary<CPrivKey> {
    static Gen<CPrivKey> arVCrary()
    {
        return gen::map(gen::arVCrary<CKey>(), [](const CKey& key) {
            return key.GetPrivKey();
        });
    };
};

/** Generator for a new CPubKey */
template <>
struct ArVCrary<CPubKey> {
    static Gen<CPubKey> arVCrary()
    {
        return gen::map(gen::arVCrary<CKey>(), [](const CKey& key) {
            return key.GetPubKey();
        });
    };
};
/** Generates a arVCrary uint256 */
template <>
struct ArVCrary<uint256> {
    static Gen<uint256> arVCrary()
    {
        return rc::gen::just(GetRandHash());
    };
};
} //namespace rc
#endif
