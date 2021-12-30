// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2016-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparamsbase.h"

<<<<<<< HEAD
#include "util.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace boost::assign;

/**
 * Main network
 */
class CBaseMainParams : public CBaseChainParams
{
public:
    CBaseMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        nRPCPort = 59683;
    }
};
static CBaseMainParams mainParams;

/**
 * Testnet (v3)
 */
class CBaseTestNetParams : public CBaseMainParams
{
public:
    CBaseTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        nRPCPort = 59685;
        strDataDir = "testnet4";
    }
};
static CBaseTestNetParams testNetParams;

/*
 * Regression test
 */
class CBaseRegTestParams : public CBaseTestNetParams
{
public:
    CBaseRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strDataDir = "regtest";
    }
};
static CBaseRegTestParams regTestParams;

static CBaseChainParams* pCurrentBaseParams = 0;

const CBaseChainParams& BaseParams()
{
    assert(pCurrentBaseParams);
    return *pCurrentBaseParams;
}

void SelectBaseParams(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        pCurrentBaseParams = &mainParams;
        break;
    case CBaseChainParams::TESTNET:
        pCurrentBaseParams = &testNetParams;
        break;
    case CBaseChainParams::REGTEST:
        pCurrentBaseParams = &regTestParams;
        break;
    default:
        assert(false && "Unimplemented network");
        return;
    }
}

CBaseChainParams::Network NetworkIdFromCommandLine()
{
    bool fRegTest = GetBoolArg("-regtest", false);
    bool fTestNet = GetBoolArg("-testnet", false);

    if (fTestNet && fRegTest)
        return CBaseChainParams::MAX_NETWORK_TYPES;
    if (fRegTest)
        return CBaseChainParams::REGTEST;
    if (fTestNet)
        return CBaseChainParams::TESTNET;
    return CBaseChainParams::MAIN;
}

bool SelectBaseParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectBaseParams(network);
    return true;
}

bool AreBaseParamsConfigured()
{
    return pCurrentBaseParams != NULL;
=======
#include "tinyformat.h"
#include "util/system.h"

#include <assert.h>

const std::string CBaseChainParams::MAIN = "main";
const std::string CBaseChainParams::TESTNET = "test";
const std::string CBaseChainParams::REGTEST = "regtest";

void AppendParamsHelpMessages(std::string& strUsage, bool debugHelp)
{
    strUsage += HelpMessageGroup("Chain selection options:");
    strUsage += HelpMessageOpt("-testnet", "Use the test chain");
    if (debugHelp) {
        strUsage += HelpMessageOpt("-regtest", "Enter regression test mode, which uses a special chain in which blocks can be solved instantly. "
                                               "This is intended for regression testing tools and app development.");
    }
}

static std::unique_ptr<CBaseChainParams> globalChainBaseParams;

const CBaseChainParams& BaseParams()
{
    assert(globalChainBaseParams);
    return *globalChainBaseParams;
}

std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::make_unique<CBaseChainParams>("", 51473);
    else if (chain == CBaseChainParams::TESTNET)
        return std::make_unique<CBaseChainParams>("testnet5", 51475);
    else if (chain == CBaseChainParams::REGTEST)
        return std::make_unique<CBaseChainParams>("regtest", 51477);
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectBaseParams(const std::string& chain)
{
    globalChainBaseParams = CreateBaseChainParams(chain);
    gArgs.SelectConfigNetwork(chain);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
