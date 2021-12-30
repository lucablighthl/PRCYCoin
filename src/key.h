// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KEY_H
#define BITCOIN_KEY_H

#include "allocators.h"
#include "serialize.h"
#include "uint256.h"
=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_KEY_H
#define PIVX_KEY_H

#include "serialize.h"
#include "support/allocators/secure.h"
#include "uint256.h"
#include "pubkey.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <stdexcept>
#include <vector>

class CPubKey;

struct CExtPubKey;

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

/**
 * secure_allocator is defined in allocators.h
 * CPrivKey is a serialized private key, with all parameters included (279 bytes)
=======
/**
 * CPrivKey is a serialized private key, with all parameters included
 * (PRIVATE_KEY_SIZE bytes)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 */
typedef std::vector<unsigned char, secure_allocator<unsigned char> > CPrivKey;

/** An encapsulated private key. */
class CKey
{
<<<<<<< HEAD
=======
public:
    /**
     * secp256k1:
     */
    static const unsigned int PRIVATE_KEY_SIZE            = 279;
    static const unsigned int COMPRESSED_PRIVATE_KEY_SIZE = 214;
    /**
     * see www.keylength.com
     * script supports up to 75 for single byte push
     */
    static_assert(
        PRIVATE_KEY_SIZE >= COMPRESSED_PRIVATE_KEY_SIZE,
        "COMPRESSED_PRIVATE_KEY_SIZE is larger than PRIVATE_KEY_SIZE");

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
private:
    //! Whether this private key is valid. We check for correctness when modifying the key
    //! data, so fValid should always correspond to the actual state.
    bool fValid;

    //! Whether the public key corresponding to this private key is (to be) compressed.
    bool fCompressed;

    //! The actual byte data
<<<<<<< HEAD
    unsigned char vch[32];
=======
    std::vector<unsigned char, secure_allocator<unsigned char> > keydata;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //! Check whether the 32-byte array pointed to be vch is valid keydata.
    bool static Check(const unsigned char* vch);

public:
    //! Construct an invalid private key.
    CKey() : fValid(false), fCompressed(false)
    {
<<<<<<< HEAD
        LockObject(vch);
    }

    //! Copy constructor. This is necessary because of memlocking.
    CKey(const CKey& secret) : fValid(secret.fValid), fCompressed(secret.fCompressed)
    {
        LockObject(vch);
        memcpy(vch, secret.vch, sizeof(vch));
    }

    //! Destructor (again necessary because of memlocking).
    ~CKey()
    {
        UnlockObject(vch);
=======
        // Important: vch must be 32 bytes in length to not break serialization
        keydata.resize(32);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    friend bool operator==(const CKey& a, const CKey& b)
    {
<<<<<<< HEAD
        return a.fCompressed == b.fCompressed && a.size() == b.size() &&
               memcmp(&a.vch[0], &b.vch[0], a.size()) == 0;
=======
        return a.fCompressed == b.fCompressed &&
            a.size() == b.size() &&
            memcmp(a.keydata.data(), b.keydata.data(), a.size()) == 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    //! Initialize using begin and end iterators to byte data.
    template <typename T>
    void Set(const T pbegin, const T pend, bool fCompressedIn)
    {
<<<<<<< HEAD
        if (pend - pbegin != 32) {
            fValid = false;
            return;
        }
        if (Check(&pbegin[0])) {
            memcpy(vch, (unsigned char*)&pbegin[0], 32);
=======
        if (size_t(pend - pbegin) != keydata.size()) {
            fValid = false;
        } else if (Check(&pbegin[0])) {
            memcpy(keydata.data(), (unsigned char*)&pbegin[0], keydata.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            fValid = true;
            fCompressed = fCompressedIn;
        } else {
            fValid = false;
        }
<<<<<<< HEAD
    }

    //! Simple read-only vector-like interface.
    unsigned int size() const { return (fValid ? 32 : 0); }
    const unsigned char* begin() const { return vch; }
    const unsigned char* end() const { return vch + size(); }
=======
}

    //! Simple read-only vector-like interface.
    unsigned int size() const { return (fValid ? keydata.size() : 0); }
    const unsigned char* begin() const { return keydata.data(); }
    const unsigned char* end() const { return keydata.data() + size(); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //! Check whether this private key is valid.
    bool IsValid() const { return fValid; }

    //! Check whether the public key corresponding to this private key is (to be) compressed.
    bool IsCompressed() const { return fCompressed; }

<<<<<<< HEAD
    //! Initialize from a CPrivKey (serialized OpenSSL private key data).
    bool SetPrivKey(const CPrivKey& vchPrivKey, bool fCompressed);

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    //! Generate a new private key using a cryptographic PRNG.
    void MakeNewKey(bool fCompressed);

    uint256 GetPrivKey_256();

    /**
<<<<<<< HEAD
     * Convert the private key to a CPrivKey (serialized OpenSSL private key data).
     * This is expensive. 
=======
     * Convert the private key to a CPrivKey (serialized secp256k1 private key data).
     * This is expensive.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     */
    CPrivKey GetPrivKey() const;

    /**
     * Compute the public key from a private key.
     * This is expensive.
     */
    CPubKey GetPubKey() const;

    /**
     * Create a DER-serialized signature.
<<<<<<< HEAD
     * The test_case parameter tweaks the deterministic nonce, and is only for
     * testing. It should be zero for normal use.
=======
     * The test_case parameter tweaks the deterministic nonce.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     */
    bool Sign(const uint256& hash, std::vector<unsigned char>& vchSig, uint32_t test_case = 0) const;

    /**
     * Create a compact signature (65 bytes), which allows reconstructing the used public key.
     * The format is one header byte, followed by two times 32 bytes for the serialized r and s values.
     * The header byte: 0x1B = first key with even y, 0x1C = first key with odd y,
     *                  0x1D = second key with even y, 0x1E = second key with odd y,
     *                  add 0x04 for compressed keys.
     */
    bool SignCompact(const uint256& hash, std::vector<unsigned char>& vchSig) const;

    //! Derive BIP32 child key.
<<<<<<< HEAD
    bool Derive(CKey& keyChild, unsigned char ccChild[32], unsigned int nChild, const unsigned char cc[32]) const;
=======
    bool Derive(CKey& keyChild, ChainCode &ccChild, unsigned int nChild, const ChainCode& cc) const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /**
     * Verify thoroughly whether a private key and a public key match.
     * This is done using a different mechanism than just regenerating it.
     */
    bool VerifyPubKey(const CPubKey& vchPubKey) const;

    //! Load private key and check that public key matches.
<<<<<<< HEAD
    bool Load(CPrivKey& privkey, CPubKey& vchPubKey, bool fSkipCheck);
=======
    bool Load(const CPrivKey& privkey, const CPubKey& vchPubKey, bool fSkipCheck);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //! Check whether an element of a signature (r or s) is valid.
    static bool CheckSignatureElement(const unsigned char* vch, int len, bool half);
};

struct CExtKey {
    unsigned char nDepth;
    unsigned char vchFingerprint[4];
    unsigned int nChild;
<<<<<<< HEAD
    unsigned char vchChainCode[32];
=======
    ChainCode chaincode;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CKey key;

    friend bool operator==(const CExtKey& a, const CExtKey& b)
    {
<<<<<<< HEAD
        return a.nDepth == b.nDepth && memcmp(&a.vchFingerprint[0], &b.vchFingerprint[0], 4) == 0 && a.nChild == b.nChild &&
               memcmp(&a.vchChainCode[0], &b.vchChainCode[0], 32) == 0 && a.key == b.key;
    }

    void Encode(unsigned char code[74]) const;
    void Decode(const unsigned char code[74]);
    bool Derive(CExtKey& out, unsigned int nChild) const;
    CExtPubKey Neuter() const;
    void SetMaster(const unsigned char* seed, unsigned int nSeedLen);
};

/** Check that required EC support is available at runtime */
bool ECC_InitSanityCheck(void);

#endif // BITCOIN_KEY_H
=======
        return a.nDepth == b.nDepth &&
            memcmp(&a.vchFingerprint[0], &b.vchFingerprint[0], sizeof(vchFingerprint)) == 0 &&
            a.nChild == b.nChild &&
            a.chaincode == b.chaincode &&
            a.key == b.key;
    }

    void Encode(unsigned char code[BIP32_EXTKEY_SIZE]) const;
    void Decode(const unsigned char code[BIP32_EXTKEY_SIZE]);
    bool Derive(CExtKey& out, unsigned int nChild) const;
    CExtPubKey Neuter() const;
    void SetSeed(const unsigned char* seed, unsigned int nSeedLen);
};

/** Initialize the elliptic curve support. May not be called twice without calling ECC_Stop first. */
void ECC_Start(void);

/** Deinitialize the elliptic curve support. No-op if ECC_Start wasn't called first. */
void ECC_Stop(void);

/** Check that required EC support is available at runtime. */
bool ECC_InitSanityCheck(void);

#endif // PIVX_KEY_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
