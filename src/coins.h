// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2016-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COINS_H
#define BITCOIN_COINS_H

#include "compressor.h"
<<<<<<< HEAD
#include "script/standard.h"
#include "serialize.h"
#include "uint256.h"
#include "undo.h"
=======
#include "consensus/consensus.h" // can be removed once policy/ established
#include "crypto/siphash.h"
#include "memusage.h"
#include "sapling/incrementalmerkletree.h"
#include "script/standard.h"
#include "serialize.h"
#include "uint256.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <assert.h>
#include <stdint.h>

<<<<<<< HEAD
#include <boost/unordered_map.hpp>

extern bool GetTransaction(const uint256& hash, CTransaction& tx, uint256& hashBlock, bool fAllowSlow);

/** 

    ****Note - for PRCY we added fCoinStake to the 2nd bit. Keep in mind when reading the following and adjust as needed.
 * Pruned version of CTransaction: only retains metadata and unspent transaction outputs
 *
 * Serialized format:
 * - VARINT(nVersion)
 * - VARINT(nCode)
 * - unspentness bitvector, for vout[2] and further; least significant byte first
 * - the non-spent CTxOuts (via CTxOutCompressor)
 * - VARINT(nHeight)
 *
 * The nCode value consists of:
 * - bit 1: IsCoinBase()
 * - bit 2: vout[0] is not spent
 * - bit 4: vout[1] is not spent
 * - The higher bits encode N, the number of non-zero bytes in the following bitvector.
 *   - In case both bit 2 and bit 4 are unset, they encode N-1, as there must be at
 *     least one non-spent output).
 *
 * Example: 0104835800816115944e077fe7c803cfa57f29b36bf87c1d358bb85e
 *          <><><--------------------------------------------><---->
 *          |  \                  |                             /
 *    version   code             vout[1]                  height
 *
 *    - version = 1
 *    - code = 4 (vout[1] is not spent, and 0 non-zero bytes of bitvector follow)
 *    - unspentness bitvector: as 0 non-zero bytes follow, it has length 0
 *    - vout[1]: 835800816115944e077fe7c803cfa57f29b36bf87c1d35
 *               * 8358: compact amount representation for 60000000000 (600 BTC)
 *               * 00: special txout type pay-to-pubkey-hash
 *               * 816115944e077fe7c803cfa57f29b36bf87c1d35: address uint160
 *    - height = 203998
 *
 *
 * Example: 0109044086ef97d5790061b01caab50f1b8e9c50a5057eb43c2d9563a4eebbd123008c988f1a4a4de2161e0f50aac7f17e7f9555caa486af3b
 *          <><><--><--------------------------------------------------><----------------------------------------------><---->
 *         /  \   \                     |                                                           |                     /
 *  version  code  unspentness       vout[4]                                                     vout[16]           height
 *
 *  - version = 1
 *  - code = 9 (coinbase, neither vout[0] or vout[1] are unspent, 2 (1, +1 because both bit 2 and bit 4 are unset) non-zero bitvector bytes follow)
 *  - unspentness bitvector: bits 2 (0x04) and 14 (0x4000) are set, so vout[2+2] and vout[14+2] are unspent
 *  - vout[4]: 86ef97d5790061b01caab50f1b8e9c50a5057eb43c2d9563a4ee
 *             * 86ef97d579: compact amount representation for 234925952 (2.35 BTC)
 *             * 00: special txout type pay-to-pubkey-hash
 *             * 61b01caab50f1b8e9c50a5057eb43c2d9563a4ee: address uint160
 *  - vout[16]: bbd123008c988f1a4a4de2161e0f50aac7f17e7f9555caa4
 *              * bbd123: compact amount representation for 110397 (0.001 BTC)
 *              * 00: special txout type pay-to-pubkey-hash
 *              * 8c988f1a4a4de2161e0f50aac7f17e7f9555caa4: address uint160
 *  - height = 120891
 */
class CCoins
{
public:
    //! whether transaction is a coinbase
    bool fCoinBase;
    bool fCoinStake;

    //! unspent transaction outputs; spent outputs are .IsNull(); spent outputs at the end of the array are dropped
    std::vector<CTxOut> vout;

    //! at which height this transaction was included in the active block chain
    int nHeight;

    //! version of the CTransaction; accesses to this value should probably check for nHeight as well,
    //! as new tx version will probably only be introduced at certain heights
    int nVersion;

    void FromTx(const CTransaction& tx, int nHeightIn)
    {
        fCoinBase = tx.IsCoinBase();
        fCoinStake = tx.IsCoinStake();
        vout = tx.vout;
        nHeight = nHeightIn;
        nVersion = tx.nVersion;
        ClearUnspendable();
    }

    //! construct a CCoins from a CTransaction, at a given height
    CCoins(const CTransaction& tx, int nHeightIn)
    {
        FromTx(tx, nHeightIn);
    }

    void Clear()
    {
        fCoinBase = false;
        fCoinStake = false;
        std::vector<CTxOut>().swap(vout);
        nHeight = 0;
        nVersion = 0;
    }

    //! empty constructor
    CCoins() : fCoinBase(false), fCoinStake(false), vout(0), nHeight(0), nVersion(0) {}

    //!remove spent outputs at the end of vout
    void Cleanup()
    {
        while (vout.size() > 0 && vout.back().IsNull())
            vout.pop_back();
        if (vout.empty())
            std::vector<CTxOut>().swap(vout);
    }

    void ClearUnspendable()
    {
        for (CTxOut& txout : vout) {
            if (txout.scriptPubKey.IsUnspendable())
                txout.SetNull();
        }
        Cleanup();
    }

    void swap(CCoins& to)
    {
        std::swap(to.fCoinBase, fCoinBase);
        std::swap(to.fCoinStake, fCoinStake);
        to.vout.swap(vout);
        std::swap(to.nHeight, nHeight);
        std::swap(to.nVersion, nVersion);
    }

    //! equality test
    friend bool operator==(const CCoins& a, const CCoins& b)
    {
        // Empty CCoins objects are always equal.
        if (a.IsPruned() && b.IsPruned())
            return true;
        return a.fCoinBase == b.fCoinBase &&
               a.fCoinStake == b.fCoinStake &&
               a.nHeight == b.nHeight &&
               a.nVersion == b.nVersion &&
               a.vout == b.vout;
    }
    friend bool operator!=(const CCoins& a, const CCoins& b)
    {
        return !(a == b);
    }

    void CalcMaskSize(unsigned int& nBytes, unsigned int& nNonzeroBytes) const;

    bool IsCoinBase() const
    {
        return fCoinBase;
    }

    bool IsCoinStake() const
    {
        return fCoinStake;
    }

    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        unsigned int nSize = 0;
        unsigned int nMaskSize = 0, nMaskCode = 0;
        CalcMaskSize(nMaskSize, nMaskCode);
        bool fFirst = vout.size() > 0 && !vout[0].IsNull();
        bool fSecond = vout.size() > 1 && !vout[1].IsNull();
        assert(fFirst || fSecond || nMaskCode);
        unsigned int nCode = 8 * (nMaskCode - (fFirst || fSecond ? 0 : 1)) + (fCoinBase ? 1 : 0) + (fCoinStake ? 2 : 0) + (fFirst ? 4 : 0) + (fSecond ? 8 : 0);
        // version
        nSize += ::GetSerializeSize(VARINT(this->nVersion), nType, nVersion);
        // size of header code
        nSize += ::GetSerializeSize(VARINT(nCode), nType, nVersion);
        // spentness bitmask
        nSize += nMaskSize;
        // txouts themself
        for (unsigned int i = 0; i < vout.size(); i++)
            if (!vout[i].IsNull())
                nSize += ::GetSerializeSize(CTxOutCompressor(REF(vout[i])), nType, nVersion);
        // height
        nSize += ::GetSerializeSize(VARINT(nHeight), nType, nVersion);
        return nSize;
    }

    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
    {
        unsigned int nMaskSize = 0, nMaskCode = 0;
        CalcMaskSize(nMaskSize, nMaskCode);
        bool fFirst = vout.size() > 0 && !vout[0].IsNull();
        bool fSecond = vout.size() > 1 && !vout[1].IsNull();
        assert(fFirst || fSecond || nMaskCode);
        unsigned int nCode = 16 * (nMaskCode - (fFirst || fSecond ? 0 : 1)) + (fCoinBase ? 1 : 0) + (fCoinStake ? 2 : 0) + (fFirst ? 4 : 0) + (fSecond ? 8 : 0);
        // version
        ::Serialize(s, VARINT(this->nVersion), nType, nVersion);
        // header code
        ::Serialize(s, VARINT(nCode), nType, nVersion);
        // spentness bitmask
        for (unsigned int b = 0; b < nMaskSize; b++) {
            unsigned char chAvail = 0;
            for (unsigned int i = 0; i < 8 && 2 + b * 8 + i < vout.size(); i++)
                if (!vout[2 + b * 8 + i].IsNull())
                    chAvail |= (1 << i);
            ::Serialize(s, chAvail, nType, nVersion);
        }
        // txouts themself
        for (unsigned int i = 0; i < vout.size(); i++) {
            if (!vout[i].IsNull())
                ::Serialize(s, CTxOutCompressor(REF(vout[i])), nType, nVersion);
        }
        // coinbase height
        ::Serialize(s, VARINT(nHeight), nType, nVersion);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType, int nVersion)
    {
        unsigned int nCode = 0;
        // version
        ::Unserialize(s, VARINT(this->nVersion), nType, nVersion);
        // header code
        ::Unserialize(s, VARINT(nCode), nType, nVersion);
        fCoinBase = nCode & 1;         //0001 - means coinbase
        fCoinStake = (nCode & 2) != 0; //0010 coinstake
        std::vector<bool> vAvail(2, false);
        vAvail[0] = (nCode & 4) != 0; // 0100
        vAvail[1] = (nCode & 8) != 0; // 1000
        unsigned int nMaskCode = (nCode / 16) + ((nCode & 12) != 0 ? 0 : 1);
        // spentness bitmask
        while (nMaskCode > 0) {
            unsigned char chAvail = 0;
            ::Unserialize(s, chAvail, nType, nVersion);
            for (unsigned int p = 0; p < 8; p++) {
                bool f = (chAvail & (1 << p)) != 0;
                vAvail.push_back(f);
            }
            if (chAvail != 0)
                nMaskCode--;
        }
        // txouts themself
        vout.assign(vAvail.size(), CTxOut());
        for (unsigned int i = 0; i < vAvail.size(); i++) {
            if (vAvail[i])
                ::Unserialize(s, REF(CTxOutCompressor(vout[i])), nType, nVersion);
        }
        // coinbase height
        ::Unserialize(s, VARINT(nHeight), nType, nVersion);
        Cleanup();
    }

    //! mark an outpoint spent, and construct undo information
    bool Spend(const COutPoint& out, CTxInUndo& undo);

    //! mark a vout spent
    bool Spend(int nPos);

    //! check whether a particular output is still available
    bool IsAvailable(unsigned int nPos) const
    {
        return (nPos < vout.size() && !vout[nPos].IsNull());
    }

    //! check whether the entire CCoins is spent
    //! note that only !IsPruned() CCoins can be serialized
    bool IsPruned() const
    {
        for (const CTxOut& out : vout)
            if (!out.IsNull())
                return false;
        return true;
    }
};

class CCoinsKeyHasher
{
private:
    uint256 salt;

public:
    CCoinsKeyHasher();
=======
#include <unordered_map>

/**
 * A UTXO entry.
 *
 * Serialized format:
 * - VARINT((coinbase ? 2 : 0) | (coinstake ? 1 : 0) | (height << 2))
 * - the non-spent CTxOut (via CTxOutCompressor)
 */
class Coin
{
public:
    //! whether the containing transaction was a coinbase
    bool fCoinBase;

    //! whether the containing transaction was a coinstake
    bool fCoinStake;

    //! unspent transaction output
    CTxOut out;

    //! at which height the containing transaction was included in the active block chain
    uint32_t nHeight;

    //! construct a Coin from a CTxOut and height/coinbase properties.
    Coin(CTxOut&& outIn, int nHeightIn, bool fCoinBaseIn, bool fCoinStakeIn) : fCoinBase(fCoinBaseIn), fCoinStake(fCoinStakeIn), out(std::move(outIn)), nHeight(nHeightIn) {}
    Coin(const CTxOut& outIn, int nHeightIn, bool fCoinBaseIn, bool fCoinStakeIn) : fCoinBase(fCoinBaseIn), fCoinStake(fCoinStakeIn), out(outIn), nHeight(nHeightIn) {}

    void Clear() {
        out.SetNull();
        fCoinBase = false;
        fCoinStake = false;
        nHeight = 0;
    }

    //! empty constructor
    Coin() : fCoinBase(false), fCoinStake(false), nHeight(0) { }

    bool IsCoinBase() const {
        return fCoinBase;
    }

    bool IsCoinStake() const {
        return fCoinStake;
    }

    template<typename Stream>
    void Serialize(Stream &s) const {
        assert(!IsSpent());
        uint32_t code = nHeight * 4 + (fCoinBase ? 2 : 0) + (fCoinStake ? 1 : 0);
        ::Serialize(s, VARINT(code));
        ::Serialize(s, Using<TxOutCompression>(out));
    }

    template<typename Stream>
    void Unserialize(Stream &s) {
        uint32_t code = 0;
        ::Unserialize(s, VARINT(code));
        nHeight = code >> 2;
        fCoinBase = code & 2;
        fCoinStake = code & 1;
        ::Unserialize(s, Using<TxOutCompression>(out));
    }

    bool IsSpent() const {
        return out.IsNull();
    }

    size_t DynamicMemoryUsage() const {
        return memusage::DynamicUsage(out.scriptPubKey);
    }
};

class SaltedOutpointHasher
{
private:
    /** Salt */
    const uint64_t k0, k1;

public:
    SaltedOutpointHasher();

    /**
     * This *must* return size_t. With Boost 1.46 on 32-bit systems the
     * unordered_map will behave unpredictably if the custom hasher returns a
     * uint64_t, resulting in failures when syncing the chain (#4634).
     */
    size_t operator()(const COutPoint& id) const {
        return SipHashUint256Extra(k0, k1, id.hash, id.n);
    }
};

// Used on Sapling nullifiers, anchor maps and txmempool::mapTx: sorted by txid
class SaltedIdHasher
{
private:
    /** Salt */
    const uint64_t k0, k1;

public:
    SaltedIdHasher();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /**
     * This *must* return size_t. With Boost 1.46 on 32-bit systems the
     * unordered_map will behave unpredictably if the custom hasher returns a
     * uint64_t, resulting in failures when syncing the chain (#4634).
     */
<<<<<<< HEAD
    size_t operator()(const uint256& key) const
    {
        return key.GetHash(salt);
=======
    size_t operator()(const uint256& txid) const {
        return SipHashUint256(k0, k1, txid);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
};

struct CCoinsCacheEntry {
<<<<<<< HEAD
    CCoins coins; // The actual cached data.
=======
    Coin coin; // The actual cached data.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned char flags;

    enum Flags {
        DIRTY = (1 << 0), // This cache entry is potentially different from the version in the parent view.
        FRESH = (1 << 1), // The parent view does not have this entry (or it is pruned).
    };

<<<<<<< HEAD
    CCoinsCacheEntry() : coins(), flags(0) {}
};

typedef boost::unordered_map<uint256, CCoinsCacheEntry, CCoinsKeyHasher> CCoinsMap;

struct CCoinsStats {
    int nHeight;
    uint256 hashBlock;
    uint64_t nTransactions;
    uint64_t nTransactionOutputs;
    uint64_t nSerializedSize;
    uint256 hashSerialized;
    CAmount nTotalAmount;

    CCoinsStats() : nHeight(0), hashBlock(0), nTransactions(0), nTransactionOutputs(0), nSerializedSize(0), hashSerialized(0), nTotalAmount(0) {}
};

=======
    CCoinsCacheEntry() : flags(0) {}
    explicit CCoinsCacheEntry(Coin&& coin_) : coin(std::move(coin_)), flags(0) {}
};

// Sapling

struct CAnchorsSaplingCacheEntry
{
    bool entered; // This will be false if the anchor is removed from the cache
    SaplingMerkleTree tree; // The tree itself
    unsigned char flags;

    enum Flags {
        DIRTY = (1 << 0), // This cache entry is potentially different from the version in the parent view.
    };

    CAnchorsSaplingCacheEntry() : entered(false), flags(0) {}
};

struct CNullifiersCacheEntry
{
    bool entered; // If the nullifier is spent or not
    unsigned char flags;

    enum Flags {
        DIRTY = (1 << 0), // This cache entry is potentially different from the version in the parent view.
    };

    CNullifiersCacheEntry() : entered(false), flags(0) {}
};

typedef std::unordered_map<uint256, CAnchorsSaplingCacheEntry, SaltedIdHasher> CAnchorsSaplingMap;
typedef std::unordered_map<uint256, CNullifiersCacheEntry, SaltedIdHasher> CNullifiersMap;

typedef std::unordered_map<COutPoint, CCoinsCacheEntry, SaltedOutpointHasher> CCoinsMap;

/** Cursor for iterating over CoinsView state */
class CCoinsViewCursor
{
public:
    CCoinsViewCursor(const uint256 &hashBlockIn): hashBlock(hashBlockIn) {}
    virtual ~CCoinsViewCursor() {}

    virtual bool GetKey(COutPoint& key) const = 0;
    virtual bool GetValue(Coin& coin) const = 0;
    virtual unsigned int GetValueSize() const = 0;

    virtual bool Valid() const = 0;
    virtual void Next() = 0;

    //! Get best block at the time this cursor was created
    const uint256 &GetBestBlock() const { return hashBlock; }
private:
    uint256 hashBlock;
};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Abstract view on the open txout dataset. */
class CCoinsView
{
public:
<<<<<<< HEAD
    //! Retrieve the CCoins (unspent transaction outputs) for a given txid
    virtual bool GetCoins(const uint256& txid, CCoins& coins) const;

    //! Just check whether we have data for a given txid.
    //! This may (but cannot always) return true for fully spent transactions
    virtual bool HaveCoins(const uint256& txid) const;
=======
    //! Retrieve the Coin (unspent transaction output) for a given outpoint.
    virtual bool GetCoin(const COutPoint& outpoint, Coin& coin) const;

    //! Just check whether we have data for a given outpoint.
    //! This may (but cannot always) return true for spent outputs.
    virtual bool HaveCoin(const COutPoint& outpoint) const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //! Retrieve the block hash whose state this CCoinsView currently represents
    virtual uint256 GetBestBlock() const;

<<<<<<< HEAD
    //! Do a bulk modification (multiple CCoins changes + BestBlock change).
    //! The passed mapCoins can be modified.
    virtual bool BatchWrite(CCoinsMap& mapCoins, const uint256& hashBlock);

    //! Calculate statistics about the unspent transaction output set
    virtual bool GetStats(CCoinsStats& stats) const;

    //! As we use CCoinsViews polymorphically, have a virtual destructor
    virtual ~CCoinsView() {}
=======
    //! Retrieve the range of blocks that may have been only partially written.
    //! If the database is in a consistent state, the result is the empty vector.
    //! Otherwise, a two-element vector is returned consisting of the new and
    //! the old block hash, in that order.
    virtual std::vector<uint256> GetHeadBlocks() const;

    //! Do a bulk modification (multiple Coin changes + BestBlock change).
    //! The passed mapCoins can be modified.
    virtual bool BatchWrite(CCoinsMap& mapCoins,
                            const uint256& hashBlock,
                            const uint256& hashSaplingAnchor,
                            CAnchorsSaplingMap& mapSaplingAnchors,
                            CNullifiersMap& mapSaplingNullifiers);

    //! Get a cursor to iterate over the whole state
    virtual CCoinsViewCursor* Cursor() const;

    //! As we use CCoinsViews polymorphically, have a virtual destructor
    virtual ~CCoinsView() {}

    //! Estimate database size (0 if not implemented)
    virtual size_t EstimateSize() const { return 0; }

    // Sapling
    //! Retrieve the tree (Sapling) at a particular anchored root in the chain
    virtual bool GetSaplingAnchorAt(const uint256 &rt, SaplingMerkleTree &tree) const;

    //! Determine whether a nullifier is spent or not
    virtual bool GetNullifier(const uint256 &nullifier) const;

    //! Get the current "tip" or the latest anchored tree root in the chain
    virtual uint256 GetBestAnchor() const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};


/** CCoinsView backed by another CCoinsView */
class CCoinsViewBacked : public CCoinsView
{
protected:
    CCoinsView* base;

public:
    CCoinsViewBacked(CCoinsView* viewIn);
<<<<<<< HEAD
    bool GetCoins(const uint256& txid, CCoins& coins) const;
    bool HaveCoins(const uint256& txid) const;
    uint256 GetBestBlock() const;
    void SetBackend(CCoinsView& viewIn);
    bool BatchWrite(CCoinsMap& mapCoins, const uint256& hashBlock);
    bool GetStats(CCoinsStats& stats) const;
};

class CCoinsViewCache;

/** Flags for nSequence and nLockTime locks */
enum {
    /* Interpret sequence numbers as relative lock-time constraints. */
    LOCKTIME_VERIFY_SEQUENCE = (1 << 0),

    /* Use GetMedianTimePast() instead of nTime for end point timestamp. */
    LOCKTIME_MEDIAN_TIME_PAST = (1 << 1),
};
=======
    bool GetCoin(const COutPoint& outpoint, Coin& coin) const override;
    bool HaveCoin(const COutPoint& outpoint) const override;
    uint256 GetBestBlock() const override;
    std::vector<uint256> GetHeadBlocks() const override;
    void SetBackend(CCoinsView& viewIn);
    CCoinsViewCursor* Cursor() const override;
    size_t EstimateSize() const override;

    bool BatchWrite(CCoinsMap& mapCoins,
                    const uint256& hashBlock,
                    const uint256& hashSaplingAnchor,
                    CAnchorsSaplingMap& mapSaplingAnchors,
                    CNullifiersMap& mapSaplingNullifiers) override;

    // Sapling
    bool GetSaplingAnchorAt(const uint256 &rt, SaplingMerkleTree &tree) const override;
    bool GetNullifier(const uint256 &nullifier) const override;
    uint256 GetBestAnchor() const override;
};

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Used as the flags parameter to sequence and nLocktime checks in non-consensus code. */
static const unsigned int STANDARD_LOCKTIME_VERIFY_FLAGS = LOCKTIME_VERIFY_SEQUENCE |
                                                           LOCKTIME_MEDIAN_TIME_PAST;

<<<<<<< HEAD
/** 
 * A reference to a mutable cache entry. Encapsulating it allows us to run
 *  cleanup code after the modification is finished, and keeping track of
 *  concurrent modifications. 
 */
class CCoinsModifier
{
private:
    CCoinsViewCache& cache;
    CCoinsMap::iterator it;
    CCoinsModifier(CCoinsViewCache& cache_, CCoinsMap::iterator it_);

public:
    CCoins* operator->() { return &it->second.coins; }
    CCoins& operator*() { return it->second.coins; }
    ~CCoinsModifier();
    friend class CCoinsViewCache;
};

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/** CCoinsView that adds a memory cache for transactions to another CCoinsView */
class CCoinsViewCache : public CCoinsViewBacked
{
protected:
<<<<<<< HEAD
    /* Whether this cache has an active modifier. */
    bool hasModifier;

    /**
     * Make mutable so that we can "fill the cache" even from Get-methods
     * declared as "const".  
=======
    /**
     * Make mutable so that we can "fill the cache" even from Get-methods
     * declared as "const".
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     */
    mutable uint256 hashBlock;
    mutable CCoinsMap cacheCoins;

<<<<<<< HEAD
public:
    CCoinsViewCache(CCoinsView* baseIn);
    ~CCoinsViewCache();

    // Standard CCoinsView methods
    bool GetCoins(const uint256& txid, CCoins& coins) const;
    bool HaveCoins(const uint256& txid) const;
    uint256 GetBestBlock() const;
    void SetBestBlock(const uint256& hashBlock);
    bool BatchWrite(CCoinsMap& mapCoins, const uint256& hashBlock);

    /**
     * Return a pointer to CCoins in the cache, or NULL if not found. This is
     * more efficient than GetCoins. Modifications to other cache entries are
     * allowed while accessing the returned pointer.
     */
    const CCoins* AccessCoins(const uint256& txid) const;

    /**
     * Return a modifiable reference to a CCoins. If no entry with the given
     * txid exists, a new one is created. Simultaneous modifications are not
     * allowed.
     */
    CCoinsModifier ModifyCoins(const uint256& txid);
=======
    // Sapling
    mutable uint256 hashSaplingAnchor;
    mutable CAnchorsSaplingMap cacheSaplingAnchors;
    mutable CNullifiersMap cacheSaplingNullifiers;

    /* Cached dynamic memory usage for the inner Coin objects. */
    mutable size_t cachedCoinsUsage;

public:
    CCoinsViewCache(CCoinsView *baseIn);

    /**
     * By deleting the copy constructor, we prevent accidentally using it when one intends to create a cache on top of a base cache.
     */
    CCoinsViewCache(const CCoinsViewCache &) = delete;

    // Sapling methods
    bool GetSaplingAnchorAt(const uint256 &rt, SaplingMerkleTree &tree) const override;
    bool GetNullifier(const uint256 &nullifier) const override;
    uint256 GetBestAnchor() const override;

    // Adds the tree to mapSaplingAnchors
    // and sets the current commitment root to this root.
    template<typename Tree> void PushAnchor(const Tree &tree);

    // Removes the current commitment root from mapAnchors and sets
    // the new current root.
    void PopAnchor(const uint256 &rt);

    // Marks nullifiers for a given transaction as spent or not.
    void SetNullifiers(const CTransaction& tx, bool spent);

    //! Check whether all sapling spend requirements (anchors/nullifiers) are satisfied
    bool HaveShieldedRequirements(const CTransaction& tx) const;

    // Standard CCoinsView methods
    bool GetCoin(const COutPoint& outpoint, Coin& coin) const override;
    bool HaveCoin(const COutPoint& outpoint) const override;
    uint256 GetBestBlock() const override;
    void SetBestBlock(const uint256& hashBlock);

    //! Get the coin and check if it's spent
    bool GetUTXOCoin(const COutPoint& outpoint, Coin& coin) const;

    bool BatchWrite(CCoinsMap& mapCoins,
                    const uint256& hashBlock,
                    const uint256& hashSaplingAnchor,
                    CAnchorsSaplingMap& mapSaplingAnchors,
                    CNullifiersMap& mapSaplingNullifiers) override;

    /**
     * Check if we have the given utxo already loaded in this cache.
     * The semantics are the same as HaveCoin(), but no calls to
     * the backing CCoinsView are made.
     */
    bool HaveCoinInCache(const COutPoint& outpoint) const;

    /**
     * Return a reference to a Coin in the cache, or a pruned one if not found. This is
     * more efficient than GetCoin. Modifications to other cache entries are
     * allowed while accessing the returned pointer.
     */
    const Coin& AccessCoin(const COutPoint& output) const;

    /**
     * Add a coin. Set potential_overwrite to true if a non-pruned version may
     * already exist.
     */
    void AddCoin(const COutPoint& outpoint, Coin&& coin, bool potential_overwrite);

    /**
     * Spend a coin. Pass moveto in order to get the deleted data.
     * If no unspent output exists for the passed outpoint, this call
     * has no effect.
     */
    void SpendCoin(const COutPoint &outpoint, Coin* moveto = nullptr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /**
     * Push the modifications applied to this cache to its base.
     * Failure to call this method before destruction will cause the changes to be forgotten.
     * If false is returned, the state of this cache (and its backing view) will be undefined.
     */
    bool Flush();

<<<<<<< HEAD
    //! Calculate the size of the cache (in number of transactions)
    unsigned int GetCacheSize() const;

    //! Check whether all prevouts of the transaction are present in the UTXO set represented by this view
    bool HaveInputs(const CTransaction& tx) const;

    const CTxOut& GetOutputFor(const CTxIn& input) const;

    friend class CCoinsModifier;

private:
    CCoinsMap::iterator FetchCoins(const uint256& txid);
    CCoinsMap::const_iterator FetchCoins(const uint256& txid) const;
};

=======
    /**
     * Removes the UTXO with the given outpoint from the cache, if it is not modified.
     */
    void Uncache(const COutPoint &outpoint);

    //! Calculate the size of the cache (in number of transaction outputs)
    unsigned int GetCacheSize() const;

    //! Calculate the size of the cache (in bytes)
    size_t DynamicMemoryUsage() const;

    /**
     * Amount of pivx coming in to a transaction
     * Note that lightweight clients may not know anything besides the hash of previous transactions,
     * so may not be able to calculate this.
     *
     * @param[in] tx	transaction for which we are checking input total
     * @return	Sum of value of all inputs (scriptSigs)
     */
    CAmount GetValueIn(const CTransaction& tx) const;

    //! Check whether all prevouts of the transaction are present in the UTXO set represented by this view
    bool HaveInputs(const CTransaction& tx) const;

    /*
     * Return the depth of a coin at height nHeight, or -1 if not found
     */
    int GetCoinDepthAtHeight(const COutPoint& output, int nHeight) const;

    /*
     * Return the sum of the value of all transaction outputs
     */
    CAmount GetTotalAmount() const;

    /*
     * Prune zerocoin mints and frozen outputs - do it once, after initialization
     */
    bool PruneInvalidEntries();


private:
    CCoinsMap::iterator FetchCoin(const COutPoint& outpoint) const;

    //! Generalized interface for popping anchors
    template<typename Tree, typename Cache, typename CacheEntry>
    void AbstractPopAnchor(
            const uint256 &newrt,
            Cache &cacheAnchors,
            uint256 &hash
    );

    //! Generalized interface for pushing anchors
    template<typename Tree, typename Cache, typename CacheIterator, typename CacheEntry>
    void AbstractPushAnchor(
            const Tree &tree,
            Cache &cacheAnchors,
            uint256 &hash
    );

    //! Interface for bringing an anchor into the cache.
    template<typename Tree>
    void BringBestAnchorIntoCache(
            const uint256 &currentRoot,
            Tree &tree
    );
};

//! Utility function to add all of a transaction's outputs to a cache.
// PIVX: When check is false, this assumes that overwrites are never possible due to BIP34 always in effect
// When check is true, the underlying view may be queried to determine whether an addition is
// an overwrite.
// When fSkipInvalid is true, the invalid_out list is checked before adding the coin.
void AddCoins(CCoinsViewCache& cache, const CTransaction& tx, int nHeight, bool check = false, bool fSkipInvalid = false);

//! Utility function to find any unspent output with a given txid.
const Coin& AccessByTxid(const CCoinsViewCache& cache, const uint256& txid);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif // BITCOIN_COINS_H
