// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_PUBKEY_H
#define BITCOIN_PUBKEY_H
=======
// Copyright (c) 2016-2018 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_PUBKEY_H
#define PIVX_PUBKEY_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include "hash.h"
#include "serialize.h"
#include "uint256.h"

#include <stdexcept>
#include <vector>

<<<<<<< HEAD
/** 
 * secp256k1:
 * const unsigned int PRIVATE_KEY_SIZE = 279;
 * const unsigned int PUBLIC_KEY_SIZE  = 65;
 * const unsigned int SIGNATURE_SIZE   = 72;
 *
 * see www.keylength.com
 * script supports up to 75 for single byte push
 */

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
const unsigned int BIP32_EXTKEY_SIZE = 74;

/** A reference to a CKey: the Hash160 of its serialized public key */
class CKeyID : public uint160
{
public:
<<<<<<< HEAD
    CKeyID() : uint160(0) {}
    CKeyID(const uint160& in) : uint160(in) {}
};

/** An encapsulated public key. */
class CPubKey
{
=======
    CKeyID() : uint160() {}
    explicit CKeyID(const uint160& in) : uint160(in) {}
};

typedef uint256 ChainCode;

/** An encapsulated public key. */
class CPubKey
{
public:
    /**
     * secp256k1:
     */
    static constexpr unsigned int PUBLIC_KEY_SIZE             = 65;
    static constexpr unsigned int COMPRESSED_PUBLIC_KEY_SIZE  = 33;
    static constexpr unsigned int SIGNATURE_SIZE              = 72;
    static constexpr unsigned int COMPACT_SIGNATURE_SIZE      = 65;
    /**
     * see www.keylength.com
     * script supports up to 75 for single byte push
     */
    static_assert(
        PUBLIC_KEY_SIZE >= COMPRESSED_PUBLIC_KEY_SIZE,
        "COMPRESSED_PUBLIC_KEY_SIZE is larger than PUBLIC_KEY_SIZE");

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
private:
    /**
     * Just store the serialized data.
     * Its length can very cheaply be computed from the first byte.
     */
<<<<<<< HEAD
    unsigned char vch[65];
=======
    unsigned char vch[PUBLIC_KEY_SIZE];
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //! Compute the length of a pubkey with a given first byte.
    unsigned int static GetLen(unsigned char chHeader)
    {
        if (chHeader == 2 || chHeader == 3)
<<<<<<< HEAD
            return 33;
        if (chHeader == 4 || chHeader == 6 || chHeader == 7)
            return 65;
=======
            return COMPRESSED_PUBLIC_KEY_SIZE;
        if (chHeader == 4 || chHeader == 6 || chHeader == 7)
            return PUBLIC_KEY_SIZE;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return 0;
    }

    //! Set this key data to be invalid
    void Invalidate()
    {
        vch[0] = 0xFF;
    }

public:
<<<<<<< HEAD
=======

    bool static ValidSize(const std::vector<unsigned char> &vch) {
      return vch.size() > 0 && GetLen(vch[0]) == vch.size();
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    //! Construct an invalid public key.
    CPubKey()
    {
        Invalidate();
    }

    //! Initialize a public key using begin/end iterators to byte data.
    template <typename T>
    void Set(const T pbegin, const T pend)
    {
        int len = pend == pbegin ? 0 : GetLen(pbegin[0]);
        if (len && len == (pend - pbegin))
            memcpy(vch, (unsigned char*)&pbegin[0], len);
        else
            Invalidate();
    }

    //! Construct a public key using begin/end iterators to byte data.
    template <typename T>
    CPubKey(const T pbegin, const T pend)
    {
        Set(pbegin, pend);
    }

    //! Construct a public key from a byte vector.
<<<<<<< HEAD
    CPubKey(const std::vector<unsigned char>& vch)
    {
        Set(vch.begin(), vch.end());
=======
    explicit CPubKey(const std::vector<unsigned char>& _vch)
    {
        Set(_vch.begin(), _vch.end());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    //! Simple read-only vector-like interface to the pubkey data.
    unsigned int size() const { return GetLen(vch[0]); }
<<<<<<< HEAD
=======
    const unsigned char* data() const { return vch; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    const unsigned char* begin() const { return vch; }
    const unsigned char* end() const { return vch + size(); }
    const unsigned char& operator[](unsigned int pos) const { return vch[pos]; }

    //! Comparator implementation.
    friend bool operator==(const CPubKey& a, const CPubKey& b)
    {
        return a.vch[0] == b.vch[0] &&
               memcmp(a.vch, b.vch, a.size()) == 0;
    }
    friend bool operator!=(const CPubKey& a, const CPubKey& b)
    {
        return !(a == b);
    }
    friend bool operator<(const CPubKey& a, const CPubKey& b)
    {
        return a.vch[0] < b.vch[0] ||
               (a.vch[0] == b.vch[0] && memcmp(a.vch, b.vch, a.size()) < 0);
    }

    //! Implement serialization, as if this was a byte vector.
<<<<<<< HEAD
    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        return size() + 1;
    }
    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
=======
    template <typename Stream>
    void Serialize(Stream& s) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        unsigned int len = size();
        ::WriteCompactSize(s, len);
        s.write((char*)vch, len);
    }
    template <typename Stream>
<<<<<<< HEAD
    void Unserialize(Stream& s, int nType, int nVersion)
    {
        unsigned int len = ::ReadCompactSize(s);
        if (len <= 65) {
=======
    void Unserialize(Stream& s)
    {
        unsigned int len = ::ReadCompactSize(s);
        if (len <= PUBLIC_KEY_SIZE) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            s.read((char*)vch, len);
        } else {
            // invalid pubkey, skip available data
            char dummy;
            while (len--)
                s.read(&dummy, 1);
            Invalidate();
        }
    }

    //! Get the KeyID of this public key (hash of its serialization)
    CKeyID GetID() const
    {
        return CKeyID(Hash160(vch, vch + size()));
    }

    //! Get the 256-bit hash of this public key.
    uint256 GetHash() const
    {
        return Hash(vch, vch + size());
    }

    /*
     * Check syntactic correctness.
<<<<<<< HEAD
     * 
=======
     *
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     * Note that this is consensus critical as CheckSig() calls it!
     */
    bool IsValid() const
    {
        return size() > 0;
    }

    //! fully validate whether this is a valid public key (more expensive than IsValid())
    bool IsFullyValid() const;

    //! Check whether this is a compressed public key.
    bool IsCompressed() const
    {
<<<<<<< HEAD
        return size() == 33;
=======
        return size() == COMPRESSED_PUBLIC_KEY_SIZE;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    /**
     * Verify a DER signature (~72 bytes).
     * If this public key is not fully valid, the return value will be false.
     */
    bool Verify(const uint256& hash, const std::vector<unsigned char>& vchSig) const;

<<<<<<< HEAD
=======
    /**
     * Check whether a signature is normalized (lower-S).
     */
    static bool CheckLowS(const std::vector<unsigned char>& vchSig);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    //! Recover a public key from a compact signature.
    bool RecoverCompact(const uint256& hash, const std::vector<unsigned char>& vchSig);

    //! Turn this public key into an uncompressed public key.
    bool Decompress();

    //! Derive BIP32 child pubkey.
<<<<<<< HEAD
    bool Derive(CPubKey& pubkeyChild, unsigned char ccChild[32], unsigned int nChild, const unsigned char cc[32]) const;
=======
    bool Derive(CPubKey& pubkeyChild, ChainCode &ccChild, unsigned int nChild, const ChainCode& cc) const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    std::vector<unsigned char> Raw() const
    {
        return std::vector<unsigned char>(vch, vch + size());
    }

<<<<<<< HEAD
    std::string GetHex() const
    {
        unsigned int sz = size();
        char psz[sz * 2 + 1];
        for (unsigned int i = 0; i < sz; i++)
            sprintf(psz + i * 2, "%02x", vch[sz - i - 1]);
        return std::string(psz, psz + sz * 2);
    }
};

typedef CPubKey CKeyImage;

=======
};

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
struct CExtPubKey {
    unsigned char nDepth;
    unsigned char vchFingerprint[4];
    unsigned int nChild;
<<<<<<< HEAD
    unsigned char vchChainCode[32];
=======
    ChainCode chaincode;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CPubKey pubkey;

    friend bool operator==(const CExtPubKey& a, const CExtPubKey& b)
    {
<<<<<<< HEAD
        return a.nDepth == b.nDepth && memcmp(&a.vchFingerprint[0], &b.vchFingerprint[0], 4) == 0 && a.nChild == b.nChild &&
               memcmp(&a.vchChainCode[0], &b.vchChainCode[0], 32) == 0 && a.pubkey == b.pubkey;
    }

    void Encode(unsigned char code[74]) const;
    void Decode(const unsigned char code[74]);
    bool Derive(CExtPubKey& out, unsigned int nChild) const;

    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersion) const
=======
        return a.nDepth == b.nDepth &&
            memcmp(&a.vchFingerprint[0], &b.vchFingerprint[0], sizeof(vchFingerprint)) == 0 &&
            a.nChild == b.nChild &&
            a.chaincode == b.chaincode &&
            a.pubkey == b.pubkey;
    }

    void Encode(unsigned char code[BIP32_EXTKEY_SIZE]) const;
    void Decode(const unsigned char code[BIP32_EXTKEY_SIZE]);
    bool Derive(CExtPubKey& out, unsigned int nChild) const;

    void Serialize(CSizeComputer& s) const
    {
        // Optimized implementation for ::GetSerializeSize that avoids copying.
        s.seek(BIP32_EXTKEY_SIZE + 1); // add one byte for the size (compact int)
    }
    template <typename Stream>
    void Serialize(Stream& s) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        unsigned int len = BIP32_EXTKEY_SIZE;
        ::WriteCompactSize(s, len);
        unsigned char code[BIP32_EXTKEY_SIZE];
        Encode(code);
        s.write((const char *)&code[0], len);
    }
    template <typename Stream>
<<<<<<< HEAD
    void Unserialize(Stream& s, int nType, int nVersion)
=======
    void Unserialize(Stream& s)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        unsigned int len = ::ReadCompactSize(s);
        unsigned char code[BIP32_EXTKEY_SIZE];
        if (len != BIP32_EXTKEY_SIZE)
            throw std::runtime_error("Invalid extended key size\n");
        s.read((char *)&code[0], len);
        Decode(code);
    }
};

<<<<<<< HEAD
#endif // BITCOIN_PUBKEY_H
=======
/** Users of this module must hold an ECCVerifyHandle. The constructor and
 *  destructor of these are not allowed to run in parallel, though. */
class ECCVerifyHandle
{
    static int refcount;

public:
    ECCVerifyHandle();
    ~ECCVerifyHandle();
};

#endif // PIVX_PUBKEY_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
