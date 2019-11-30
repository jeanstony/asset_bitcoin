// Copyright (c) 2016-2018 The Vccoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <versionVCsinfo.h>

#include <consensus/params.h>

const struct VBDeploymentInfo VersionBitsDeploymentInfo[Consensus::MAX_VERSION_VCS_DEPLOYMENTS] = {
    {
        /*.name =*/ "testdummy",
        /*.gbt_force =*/ true,
    },
    {
        /*.name =*/ "csv",
        /*.gbt_force =*/ true,
    },
    {
        /*.name =*/ "segwit",
        /*.gbt_force =*/ true,
    }
};