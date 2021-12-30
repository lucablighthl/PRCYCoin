// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2016-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifndef BITCOIN_MINER_H
#define BITCOIN_MINER_H

<<<<<<< HEAD
#include <stdint.h>
#include "enum.h"
#include "key.h"
=======
#include "primitives/block.h"

#include <stdint.h>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CBlock;
class CBlockHeader;
class CBlockIndex;
<<<<<<< HEAD
=======
class CStakeableOutput;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class CReserveKey;
class CScript;
class CWallet;

struct CBlockTemplate;

<<<<<<< HEAD
/** Run the miner threads */
void GeneratePrcycoins(bool fGenerate, CWallet* pwallet, int nThreads);
/** Run the PoA miner threads */
void GeneratePoAPrcycoin(CWallet* pwallet, int period);
/** Generate a new block, without valid proof-of-work */
CBlockTemplate* CreateNewBlock(const CScript& scriptPubKeyIn, const CPubKey& txPub, const CKey& txPriv, CWallet* pwallet, bool fProofOfStake);
CBlockTemplate* CreateNewBlockWithKey(CReserveKey& reservekey, CWallet* pwallet, bool fProofOfStake);
CBlockTemplate* CreateNewPoABlock(const CScript& scriptPubKeyIn, const CPubKey& txPub, const CKey& txPriv, CWallet* pwallet);
CBlockTemplate* CreateNewPoABlockWithKey(CReserveKey& reservekey, CWallet* pwallet);

/** Modify the extranonce in a block */
void IncrementExtraNonce(CBlock* pblock, CBlockIndex* pindexPrev, unsigned int& nExtraNonce);
/** Check mined block */
void UpdateTime(CBlockHeader* block, const CBlockIndex* pindexPrev);

void BitcoinMiner(CWallet* pwallet, bool fProofOfStake, MineType mineType=MineType::MINETYPE_POW);
void ThreadStakeMinter();
=======
static const bool DEFAULT_PRINTPRIORITY = false;

#ifdef ENABLE_WALLET
    /** Run the miner threads */
    void GenerateBitcoins(bool fGenerate, CWallet* pwallet, int nThreads);
    /** Generate a new PoW block, without valid proof-of-work */
    std::unique_ptr<CBlockTemplate> CreateNewBlockWithKey(std::unique_ptr<CReserveKey>& reservekey, CWallet* pwallet);
    std::unique_ptr<CBlockTemplate> CreateNewBlockWithScript(const CScript& coinbaseScript, CWallet* pwallet);

    void BitcoinMiner(CWallet* pwallet, bool fProofOfStake);
    void ThreadStakeMinter();
#endif // ENABLE_WALLET
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

extern double dHashesPerSec;
extern int64_t nHPSTimerStart;

#endif // BITCOIN_MINER_H
