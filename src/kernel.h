<<<<<<< HEAD
// Copyright (c) 2012-2013 The PPCoin developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_KERNEL_H
#define BITCOIN_KERNEL_H

#include "main.h"


// MODIFIER_INTERVAL: time to elapse before new modifier is computed
static const unsigned int MODIFIER_INTERVAL = 60;
static const unsigned int MODIFIER_INTERVAL_TESTNET = 60;
extern unsigned int nModifierInterval;
extern unsigned int getIntervalVersion(bool fTestNet);

// MODIFIER_INTERVAL_RATIO:
// ratio of group interval length between the last group and the first group
static const int MODIFIER_INTERVAL_RATIO = 3;

// Compute the hash modifier for proof-of-stake
bool ComputeNextStakeModifier(const CBlockIndex* pindexPrev, uint64_t& nStakeModifier, bool& fGeneratedStakeModifier);

// Check whether stake kernel meets hash target
// Sets hashProofOfStake on success return
uint256 stakeHash(unsigned int nTimeTx, CDataStream ss, unsigned int prevoutIndex, uint256 prevoutHash, unsigned int nTimeBlockFrom);
bool stakeTargetHit(uint256 hashProofOfStake, int64_t nValueIn, uint256 bnTargetPerCoinDay);
bool CheckStakeKernelHash(unsigned int nBits, const CBlockHeader blockFrom, const CTransaction txPrev, const COutPoint prevout, const unsigned char* encryptionKey, unsigned int& nTimeTx, unsigned int nHashDrift, bool fCheck, uint256& hashProofOfStake, bool fPrintProofOfStake = false);

// Check kernel hash target and coinstake signature
// Sets hashProofOfStake on success return
bool CheckProofOfStake(const CBlock block, uint256& hashProofOfStake);

// Get stake modifier checksum
unsigned int GetStakeModifierChecksum(const CBlockIndex* pindex);

// Check stake modifier hard checkpoints
bool CheckStakeModifierCheckpoints(int nHeight, unsigned int nStakeModifierChecksum);

// Get time weight using supplied timestamps
int64_t GetWeight(int64_t nIntervalBeginning, int64_t nIntervalEnd);

#endif // BITCOIN_KERNEL_H
=======
// Copyright (c) 2011-2013 The PPCoin developers
// Copyright (c) 2013-2014 The NovaCoin Developers
// Copyright (c) 2014-2018 The BlackCoin Developers
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_KERNEL_H
#define PIVX_KERNEL_H

#include "stakeinput.h"

class CStakeKernel {
public:
    /**
     * CStakeKernel Constructor
     *
     * @param[in]   pindexPrev      index of the parent of the kernel block
     * @param[in]   stakeInput      input for the coinstake of the kernel block
     * @param[in]   nBits           target difficulty bits of the kernel block
     * @param[in]   nTimeTx         time of the kernel block
     */
    CStakeKernel(const CBlockIndex* const pindexPrev, CStakeInput* stakeInput, unsigned int nBits, int nTimeTx);

    // Return stake kernel hash
    uint256 GetHash() const;

    // Check that the kernel hash meets the target required
    bool CheckKernelHash(bool fSkipLog = false) const;

private:
    // kernel message hashed
    CDataStream stakeModifier{CDataStream(SER_GETHASH, 0)};
    int nTimeBlockFrom{0};
    CDataStream stakeUniqueness{CDataStream(SER_GETHASH, 0)};
    int nTime{0};
    // hash target
    unsigned int nBits{0};     // difficulty for the target
    CAmount stakeValue{0};     // target multiplier
};

/* PoS Validation */

/*
 * Stake                Check if stakeInput can stake a block on top of pindexPrev
 *
 * @param[in]   pindexPrev      index of the parent block of the block being staked
 * @param[in]   stakeInput      input for the coinstake
 * @param[in]   nBits           target difficulty bits
 * @param[in]   nTimeTx         new blocktime
 * @return      bool            true if stake kernel hash meets target protocol
 */
bool Stake(const CBlockIndex* pindexPrev, CStakeInput* stakeInput, unsigned int nBits, int64_t& nTimeTx);

/*
 * CheckProofOfStake    Check if block has valid proof of stake
 *
 * @param[in]   block           block with the proof being verified
 * @param[out]  strError        string returning error message (if any, else empty)
 * @param[in]   pindexPrev      index of the parent block
 *                              (if nullptr, it will be searched in mapBlockIndex)
 * @return      bool            true if the block has a valid proof of stake
 */
bool CheckProofOfStake(const CBlock& block, std::string& strError, const CBlockIndex* pindexPrev = nullptr);

/*
 * GetStakeKernelHash   Return stake kernel of a block
 *
 * @param[out]  hashRet         hash of the kernel (set by this function)
 * @param[in]   block           block with the kernel to return
 * @param[in]   pindexPrev      index of the parent block
 *                              (if nullptr, it will be searched in mapBlockIndex)
 * @return      bool            false if kernel cannot be initialized, true otherwise
 */
bool GetStakeKernelHash(uint256& hashRet, const CBlock& block, const CBlockIndex* pindexPrev = nullptr);

#endif // PIVX_KERNEL_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
