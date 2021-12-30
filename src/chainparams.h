// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Copyright (c) 2020-2021 The PRCY developers
=======
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include "chainparamsbase.h"
<<<<<<< HEAD
#include "checkpoints.h"
=======
#include "consensus/params.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "primitives/block.h"
#include "protocol.h"
#include "uint256.h"

<<<<<<< HEAD
#include <vector>

typedef unsigned char MessageStartChars[MESSAGE_START_SIZE];

class CDNSSeedData {
public:
    std::string name, host;
    bool supportsServiceBitsFiltering;
    std::string getHost(uint64_t requiredServiceBits) const;
    CDNSSeedData(const std::string& strName, const std::string& strHost, bool supportsServiceBitsFilteringIn = false) : name(strName), host(strHost), supportsServiceBitsFiltering(supportsServiceBitsFilteringIn) {}
};

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
=======
#include <memory>
#include <vector>

struct CDNSSeedData {
    std::string host;
    bool supportsServiceBitsFiltering;
    CDNSSeedData(const std::string& strHost, bool supportsServiceBitsFilteringIn = false) : host(strHost), supportsServiceBitsFiltering(supportsServiceBitsFilteringIn) {}
};

typedef std::map<int, uint256> MapCheckpoints;

struct CCheckpointData {
    const MapCheckpoints* mapCheckpoints;
    int64_t nTimeLastCheckpoint;
    int64_t nTransactionsLastCheckpoint;
    double fTransactionsPerDay;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

/**
 * CChainParams defines various tweakable parameters of a given instance of the
<<<<<<< HEAD
 * PRCY system. There are three: the main network on which people trade goods
=======
 * PIVX system. There are three: the main network on which people trade goods
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 * and services, the public test network which gets reset from time to time and
 * a regression test mode which is intended for private networks only. It has
 * minimal difficulty to ensure that blocks can be found instantly.
 */
class CChainParams
{
public:
<<<<<<< HEAD
=======
    virtual ~CChainParams() {}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    enum Base58Type {
        PUBKEY_ADDRESS,
        SCRIPT_ADDRESS,
        SECRET_KEY,     // BIP16
        EXT_PUBLIC_KEY, // BIP32
        EXT_SECRET_KEY, // BIP32
<<<<<<< HEAD
=======
        EXT_COIN_TYPE,  // BIP44
        STAKING_ADDRESS,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        MAX_BASE58_TYPES
    };

<<<<<<< HEAD
    const uint256& HashGenesisBlock() const { return hashGenesisBlock; }
    const MessageStartChars& MessageStart() const { return pchMessageStart; }
    int GetDefaultPort() const { return nDefaultPort; }
    const uint256& ProofOfWorkLimit() const { return bnProofOfWorkLimit; }
    int SubsidyHalvingInterval() const { return nSubsidyHalvingInterval; }
    /** Used to check majorities for block version upgrade */
    int EnforceBlockUpgradeMajority() const { return nEnforceBlockUpgradeMajority; }
    int RejectBlockOutdatedMajority() const { return nRejectBlockOutdatedMajority; }
    int ToCheckBlockUpgradeMajority() const { return nToCheckBlockUpgradeMajority; }
    int MaxReorganizationDepth() const { return nMaxReorganizationDepth; }

    /** Used if GeneratePrcycoins is called with a negative number of threads */
    int DefaultMinerThreads() const { return nMinerThreads; }
    const CBlock& GenesisBlock() const { return genesis; }
    bool RequireRPCPassword() const { return fRequireRPCPassword; }
    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return fMiningRequiresPeers; }
    /** Headers first syncing is disabled */
    bool HeadersFirstSyncingActive() const { return fHeadersFirstSyncingActive; };
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return fDefaultConsistencyChecks; }
    /** Allow mining of a min-difficulty block */
    bool AllowMinDifficultyBlocks() const { return fAllowMinDifficultyBlocks; }
    /** Skip proof-of-work check: allow mining of any difficulty block */
    bool SkipProofOfWorkCheck() const { return fSkipProofOfWorkCheck; }
    /** Make standard checks */
    bool RequireStandard() const { return fRequireStandard; }
    int64_t TargetTimespan() const { return nTargetTimespan; }
    int64_t TargetSpacing() const { return nTargetSpacing; }
    int64_t Interval() const { return nTargetTimespan / nTargetSpacing; }
    int COINBASE_MATURITY() const { return nMaturity; }
    CAmount MaxMoneyOut() const { return nMaxMoneyOut; }
    CAmount MNCollateralAmt() const { return nMNCollateralAmt; }
    CAmount MinimumStakeAmount() const { return nMinimumStakeAmount; }
    /** The masternode count that we will allow the see-saw reward payments to be off by */
    int MasternodeCountDrift() const { return nMasternodeCountDrift; }
    /** Make miner stop after a block is found. In RPC, don't return until nGenProcLimit blocks are generated */
    bool MineBlocksOnDemand() const { return fMineBlocksOnDemand; }
    /** In the future use NetworkIDString() for RPC fields */
    bool TestnetToBeDeprecatedFieldRPC() const { return fTestnetToBeDeprecatedFieldRPC; }
    /** Return the BIP70 network string (main, test or regtest) */
    std::string NetworkIDString() const { return strNetworkID; }
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::vector<SeedSpec6>& FixedSeeds() const { return vFixedSeeds; }
    virtual const Checkpoints::CCheckpointData& Checkpoints() const = 0;
    int PoolMaxTransactions() const { return nPoolMaxTransactions; }
    std::string ObfuscationPoolDummyAddress() const { return strObfuscationPoolDummyAddress; }
    int64_t StartMasternodePayments() const { return nStartMasternodePayments; }
    int64_t Budget_Fee_Confirmations() const { return nBudget_Fee_Confirmations; }
    CBaseChainParams::Network NetworkID() const { return networkID; }
    int ExtCoinType() const { return nExtCoinType; }

    /** Height or Time Based Activations **/
    int ModifierUpgradeBlock() const { return nModifierUpdateBlock; }
    int LAST_POW_BLOCK() const { return nLastPOWBlock; }
    int START_POA_BLOCK() const { return nStartPOABlock; }
    int SoftFork() const { return nSoftForkBlock;}
    int PoANewDiff() const { return nPoANewDiff;}
    int PoAFixTime() const { return nPoAFixTime;}
    int PoAPaddingBlock() const { return nPoAPaddingBlock;}
    int PoAPadding() const { return nPoAPadding;}
    int BIP65ActivationHeight() const { return nBIP65ActivationHeight; }
    int HardFork() const { return nHardForkBlock;}

    //For PoA block time
    int POA_BLOCK_TIME() const { return nPoABlockTime; }
    int MIN_NUM_POS_BLOCKS_AUDITED() const {return nMinNumPoSBlocks;}
    int MAX_NUM_POS_BLOCKS_AUDITED() const {return nMaxNumPoSBlocks;}
    int nLastPOWBlock;
    double MAX_MONEY;
    CAmount TOTAL_SUPPLY = 70000000 * COIN; //70M PRCY

protected:
    CChainParams() {}

    uint256 hashGenesisBlock;
    MessageStartChars pchMessageStart;
    //! Raw pub key bytes for the broadcast alert signing key.
    int nDefaultPort;
    int nExtCoinType;
    uint256 bnProofOfWorkLimit;
    mutable int nMaxReorganizationDepth;
    int nSubsidyHalvingInterval;
    int nEnforceBlockUpgradeMajority;
    int nRejectBlockOutdatedMajority;
    int nToCheckBlockUpgradeMajority;
    int64_t nTargetTimespan;
    int64_t nTargetSpacing;
    int nStartPOABlock;
    int nSoftForkBlock;
    int nHardForkBlock;
    int nPoANewDiff;
    int nPoAFixTime;
    int nPoAPaddingBlock;
    int nPoAPadding;
    int nMasternodeCountDrift;
    int nMaturity;
    int nModifierUpdateBlock;
    CAmount nMaxMoneyOut;
    CAmount nMNCollateralAmt;
    CAmount nMinimumStakeAmount;
    int nMinerThreads;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    CBaseChainParams::Network networkID;
    std::string strNetworkID;
    CBlock genesis;
    std::vector<SeedSpec6> vFixedSeeds;
    bool fRequireRPCPassword;
    bool fMiningRequiresPeers;
    bool fAllowMinDifficultyBlocks;
    bool fDefaultConsistencyChecks;
    bool fRequireStandard;
    bool fMineBlocksOnDemand;
    bool fSkipProofOfWorkCheck;
    bool fTestnetToBeDeprecatedFieldRPC;
    bool fHeadersFirstSyncingActive;
    int nPoolMaxTransactions;
    std::string strObfuscationPoolDummyAddress;
    int64_t nStartMasternodePayments;
    int64_t nBudget_Fee_Confirmations;
    int nBIP65ActivationHeight;

    //For PoA blocks
    int nPoABlockTime;
    int nMinNumPoSBlocks;
    int nMaxNumPoSBlocks;
public:
    void ChangeMaxReorg(int num) const {
        nMaxReorganizationDepth = num;
    }
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CChainParams& Params();

/** Return parameters for the given network. */
CChainParams& Params(CBaseChainParams::Network network);


/** Sets the params returned by Params() to those for the given network. */
void SelectParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectParamsFromCommandLine();
=======
    enum Bech32Type {
        SAPLING_PAYMENT_ADDRESS,
        SAPLING_FULL_VIEWING_KEY,
        SAPLING_INCOMING_VIEWING_KEY,
        SAPLING_EXTENDED_SPEND_KEY,
        SAPLING_EXTENDED_FVK,

        MAX_BECH32_TYPES
    };

    const Consensus::Params& GetConsensus() const { return consensus; }
    const CMessageHeader::MessageStartChars& MessageStart() const { return pchMessageStart; }
    int GetDefaultPort() const { return nDefaultPort; }

    const CBlock& GenesisBlock() const { return genesis; }
    /** Policy: Filter transactions that do not match well-defined patterns */
    bool RequireStandard() const { return fRequireStandard; }
    /** If this chain is exclusively used for testing */
    bool IsTestChain() const { return IsTestnet() || IsRegTestNet(); }
    /** Make miner wait to have peers to avoid wasting work */
    bool MiningRequiresPeers() const { return !IsRegTestNet(); }
    /** Headers first syncing is disabled */
    bool HeadersFirstSyncingActive() const { return false; };
    /** Default value for -checkmempool and -checkblockindex argument */
    bool DefaultConsistencyChecks() const { return IsRegTestNet(); }

    /** Return the network string */
    std::string NetworkIDString() const { return strNetworkID; }
    const std::vector<CDNSSeedData>& DNSSeeds() const { return vSeeds; }
    const std::vector<unsigned char>& Base58Prefix(Base58Type type) const { return base58Prefixes[type]; }
    const std::string& Bech32HRP(Bech32Type type) const { return bech32HRPs[type]; }
    const std::vector<uint8_t>& FixedSeeds() const { return vFixedSeeds; }
    virtual const CCheckpointData& Checkpoints() const = 0;

    bool IsRegTestNet() const { return NetworkIDString() == CBaseChainParams::REGTEST; }
    bool IsTestnet() const { return NetworkIDString() == CBaseChainParams::TESTNET; }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight);
protected:
    CChainParams() {}

    std::string strNetworkID;
    CBlock genesis;
    Consensus::Params consensus;
    CMessageHeader::MessageStartChars pchMessageStart;
    int nDefaultPort;
    std::vector<CDNSSeedData> vSeeds;
    std::vector<unsigned char> base58Prefixes[MAX_BASE58_TYPES];
    std::string bech32HRPs[MAX_BECH32_TYPES];
    std::vector<uint8_t> vFixedSeeds;
    bool fRequireStandard;
};

/**
 * Creates and returns a std::unique_ptr<CChainParams> of the chosen chain.
 * @returns a CChainParams* of the chosen chain.
 * @throws a std::runtime_error if the chain is not supported.
 */
std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain);

/**
 * Return the currently selected parameters. This won't change after app
 * startup, except for unit tests.
 */
const CChainParams& Params();

/**
 * Sets the params returned by Params() to those for the given chain name.
 * @throws std::runtime_error when the chain is not supported.
 */
void SelectParams(const std::string& chain);

/**
 * Allows modifying the network upgrade regtest parameters.
 */
void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#endif // BITCOIN_CHAINPARAMS_H
