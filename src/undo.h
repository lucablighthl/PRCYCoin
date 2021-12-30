// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2016-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_UNDO_H
#define BITCOIN_UNDO_H

<<<<<<< HEAD
#include "compressor.h"
#include "primitives/transaction.h"
#include "serialize.h"

/** Undo information for a CTxIn
 *
 *  Contains the prevout's CTxOut being spent, and if this was the
 *  last output of the affected transaction, its metadata as well
 *  (coinbase or not, height, transaction version)
 */
class CTxInUndo
{
public:
    CTxOut txout;   // the txout data before being spent
    bool fCoinBase; // if the outpoint was the last unspent: whether it belonged to a coinbase
    bool fCoinStake;
    unsigned int nHeight; // if the outpoint was the last unspent: its height
    int nVersion;         // if the outpoint was the last unspent: its version

    CTxInUndo() : txout(), fCoinBase(false), fCoinStake(false), nHeight(0), nVersion(0) {}
    CTxInUndo(const CTxOut& txoutIn, bool fCoinBaseIn = false, bool fCoinStakeIn = false, unsigned int nHeightIn = 0, int nVersionIn = 0) : txout(txoutIn), fCoinBase(fCoinBaseIn), fCoinStake(fCoinStakeIn), nHeight(nHeightIn), nVersion(nVersionIn) {}

    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        return ::GetSerializeSize(VARINT(nHeight * 4 + (fCoinBase ? 2 : 0) + (fCoinStake ? 1 : 0)), nType, nVersion) +
               (nHeight > 0 ? ::GetSerializeSize(VARINT(this->nVersion), nType, nVersion) : 0) +
               ::GetSerializeSize(CTxOutCompressor(REF(txout)), nType, nVersion);
    }

    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
    {
        ::Serialize(s, VARINT(nHeight * 4 + (fCoinBase ? 2 : 0) + (fCoinStake ? 1 : 0)), nType, nVersion);
        if (nHeight > 0)
            ::Serialize(s, VARINT(this->nVersion), nType, nVersion);
        ::Serialize(s, CTxOutCompressor(REF(txout)), nType, nVersion);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType, int nVersion)
    {
        unsigned int nCode = 0;
        ::Unserialize(s, VARINT(nCode), nType, nVersion);
        nHeight = nCode >> 2;
        fCoinBase = nCode & 2;
        fCoinStake = nCode & 1;
        if (nHeight > 0)
            ::Unserialize(s, VARINT(this->nVersion), nType, nVersion);
        ::Unserialize(s, REF(CTxOutCompressor(REF(txout))), nType, nVersion);
=======
#include "chain.h"
#include "compressor.h"
#include "consensus/consensus.h"
#include "primitives/transaction.h"
#include "serialize.h"
#include "version.h"

/** Formatter for undo information for a CTxIn
 *
 *  Contains the prevout's CTxOut being spent, and its metadata as well
 *  (coinbase/coinstake or not, height). The serialization contains a
 *  dummy value of zero. This is be compatible with older versions which
 *  expect to see the transaction version there.
 */
struct TxInUndoFormatter
{
    template<typename Stream>
    void Ser(Stream &s, const Coin& txout) {
        ::Serialize(s, VARINT(txout.nHeight * 4 + (txout.fCoinBase ? 2u : 0u) + (txout.fCoinStake ? 1u : 0u)));
        if (txout.nHeight > 0) {
            // Required to maintain compatibility with older undo format.
            ::Serialize(s, (unsigned char)0);
        }
        ::Serialize(s, Using<TxOutCompression>(txout.out));
    }

    template<typename Stream>
    void Unser(Stream &s, Coin& txout) {
        unsigned int nCode = 0;
        ::Unserialize(s, VARINT(nCode));
        txout.nHeight = nCode >> 2;
        txout.fCoinBase = nCode & 2;
        txout.fCoinStake = nCode & 1;
        if (txout.nHeight > 0) {
            // Old versions stored the version number for the last spend of
            // a transaction's outputs. Non-final spends were indicated with
            // height = 0.
            unsigned int nVersionDummy;
            ::Unserialize(s, VARINT(nVersionDummy));
        }
        ::Unserialize(s, Using<TxOutCompression>(txout.out));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
};

/** Undo information for a CTransaction */
class CTxUndo
{
public:
    // undo information for all txins
<<<<<<< HEAD
    std::vector<CTxInUndo> vprevout;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vprevout);
    }
=======
    std::vector<Coin> vprevout;

    SERIALIZE_METHODS(CTxUndo, obj) { READWRITE(Using<VectorFormatter<TxInUndoFormatter>>(obj.vprevout)); }
};

/** Undo information for a CBlock */
class CBlockUndo
{
public:
    std::vector<CTxUndo> vtxundo; // for all but the coinbase

    SERIALIZE_METHODS(CBlockUndo, obj) { READWRITE(obj.vtxundo); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_UNDO_H
