// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COMPRESSOR_H
#define BITCOIN_COMPRESSOR_H

#include "primitives/transaction.h"
#include "script/script.h"
#include "serialize.h"
<<<<<<< HEAD

class CKeyID;
class CPubKey;
class CScriptID;
=======
#include "span.h"

bool CompressScript(const CScript& script, std::vector<unsigned char> &out);
unsigned int GetSpecialScriptSize(unsigned int nSize);
bool DecompressScript(CScript& script, unsigned int nSize, const std::vector<unsigned char> &out);

uint64_t CompressAmount(uint64_t nAmount);
uint64_t DecompressAmount(uint64_t nAmount);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Compact serializer for scripts.
 *
 *  It detects common cases and encodes them much more efficiently.
 *  3 special cases are defined:
 *  * Pay to pubkey hash (encoded as 21 bytes)
 *  * Pay to script hash (encoded as 21 bytes)
 *  * Pay to pubkey starting with 0x02, 0x03 or 0x04 (encoded as 33 bytes)
 *
 *  Other scripts up to 121 bytes require 1 byte + script length. Above
 *  that, scripts up to 16505 bytes require 2 bytes + script length.
 */
<<<<<<< HEAD
class CScriptCompressor
{
private:
=======
struct ScriptCompression
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /**
     * make this static for now (there are only 6 special scripts defined)
     * this can potentially be extended together with a new nVersion for
     * transactions, in which case this value becomes dependent on nVersion
     * and nHeight of the enclosing transaction.
     */
    static const unsigned int nSpecialScripts = 6;

<<<<<<< HEAD
    CScript& script;

protected:
    /**
     * These check for scripts for which a special case with a shorter encoding is defined.
     * They are implemented separately from the CScript test, as these test for exact byte
     * sequence correspondences, and are more strict. For example, IsToPubKey also verifies
     * whether the public key is valid (as invalid ones cannot be represented in compressed
     * form).
     */
    bool IsToKeyID(CKeyID& hash) const;
    bool IsToScriptID(CScriptID& hash) const;
    bool IsToPubKey(CPubKey& pubkey) const;

    bool Compress(std::vector<unsigned char>& out) const;
    unsigned int GetSpecialSize(unsigned int nSize) const;
    bool Decompress(unsigned int nSize, const std::vector<unsigned char>& out);

public:
    CScriptCompressor(CScript& scriptIn) : script(scriptIn) {}

    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        std::vector<unsigned char> compr;
        if (Compress(compr))
            return compr.size();
        unsigned int nSize = script.size() + nSpecialScripts;
        return script.size() + VARINT(nSize).GetSerializeSize(nType, nVersion);
    }

    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
    {
        std::vector<unsigned char> compr;
        if (Compress(compr)) {
            s << CFlatData(compr);
=======
    template<typename Stream>
    void Ser(Stream &s, const CScript& script) {
        std::vector<unsigned char> compr;
        if (CompressScript(script, compr)) {
            s << MakeSpan(compr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return;
        }
        unsigned int nSize = script.size() + nSpecialScripts;
        s << VARINT(nSize);
<<<<<<< HEAD
        s << CFlatData(script);
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType, int nVersion)
    {
        unsigned int nSize = 0;
        s >> VARINT(nSize);
        if (nSize < nSpecialScripts) {
            std::vector<unsigned char> vch(GetSpecialSize(nSize), 0x00);
            s >> REF(CFlatData(vch));
            Decompress(nSize, vch);
=======
        s << MakeSpan(script);
    }

    template<typename Stream>
    void Unser(Stream &s, CScript& script) {
        unsigned int nSize = 0;
        s >> VARINT(nSize);
        if (nSize < nSpecialScripts) {
            std::vector<unsigned char> vch(GetSpecialScriptSize(nSize), 0x00);
            s >> MakeSpan(vch);
            DecompressScript(script, nSize, vch);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return;
        }
        nSize -= nSpecialScripts;
        if (nSize > MAX_SCRIPT_SIZE) {
<<<<<<< HEAD
        s >> REF(CFlatData(script));	            // Overly long script, replace with a short invalid one
=======
            // Overly long script, replace with a short invalid one
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            script << OP_RETURN;
            s.ignore(nSize);
        } else {
            script.resize(nSize);
<<<<<<< HEAD
            s >> REF(CFlatData(script));
=======
            s >> MakeSpan(script);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }
};

<<<<<<< HEAD
/** wrapper for CTxOut that provides a more compact serialization */
class CTxOutCompressor
{
private:
    CTxOut& txout;

public:
    static uint64_t CompressAmount(uint64_t nAmount);
    static uint64_t DecompressAmount(uint64_t nAmount);

    CTxOutCompressor(CTxOut& txoutIn) : txout(txoutIn) {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        if (!ser_action.ForRead()) {
            uint64_t nVal = CompressAmount(txout.nValue);
            READWRITE(VARINT(nVal));
        } else {
            uint64_t nVal = 0;
            READWRITE(VARINT(nVal));
            txout.nValue = DecompressAmount(nVal);
        }
        CScriptCompressor cscript(REF(txout.scriptPubKey));
        READWRITE(cscript);
    }
=======
struct AmountCompression
{
    template<typename Stream, typename I> void Ser(Stream& s, I val)
    {
        s << VARINT(CompressAmount(val));
    }
    template<typename Stream, typename I> void Unser(Stream& s, I& val)
    {
        uint64_t v;
        s >> VARINT(v);
        val = DecompressAmount(v);
    }
};

/** wrapper for CTxOut that provides a more compact serialization */
struct TxOutCompression
{
    FORMATTER_METHODS(CTxOut, obj) { READWRITE(Using<AmountCompression>(obj.nValue), Using<ScriptCompression>(obj.scriptPubKey)); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_COMPRESSOR_H
