// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CRYPTER_H
#define BITCOIN_CRYPTER_H

<<<<<<< HEAD
#include "allocators.h"
#include "keystore.h"
#include "serialize.h"

class uint256;

const unsigned int WALLET_CRYPTO_KEY_SIZE = 32;
const unsigned int WALLET_CRYPTO_SALT_SIZE = 8;
=======
#include "keystore.h"
#include "serialize.h"
#include "streams.h"
#include "support/allocators/zeroafterfree.h"

class uint256;

#include <atomic>

const unsigned int WALLET_CRYPTO_KEY_SIZE = 32;
const unsigned int WALLET_CRYPTO_SALT_SIZE = 8;
const unsigned int WALLET_CRYPTO_IV_SIZE = 16;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/**
 * Private key encryption is done based on a CMasterKey,
 * which holds a salt and random encryption key.
 * 
 * CMasterKeys are encrypted using AES-256-CBC using a key
 * derived using derivation method nDerivationMethod
 * (0 == EVP_sha512()) and derivation iterations nDeriveIterations.
 * vchOtherDerivationParameters is provided for alternative algorithms
 * which may require more parameters (such as scrypt).
 * 
 * Wallet Private Keys are then encrypted using AES-256-CBC
 * with the double-sha256 of the public key as the IV, and the
 * master key's key as the encryption key (see keystore.[ch]).
 */

/** Master key for wallet encryption */
class CMasterKey
{
public:
    std::vector<unsigned char> vchCryptedKey;
    std::vector<unsigned char> vchSalt;
    //! 0 = EVP_sha512()
    //! 1 = scrypt()
    unsigned int nDerivationMethod;
    unsigned int nDeriveIterations;
    //! Use this for more parameters to key derivation,
    //! such as the various parameters to scrypt
    std::vector<unsigned char> vchOtherDerivationParameters;

<<<<<<< HEAD
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vchCryptedKey);
        READWRITE(vchSalt);
        READWRITE(nDerivationMethod);
        READWRITE(nDeriveIterations);
        READWRITE(vchOtherDerivationParameters);
    }
=======
    SERIALIZE_METHODS(CMasterKey, obj) { READWRITE(obj.vchCryptedKey, obj.vchSalt, obj.nDerivationMethod, obj.nDeriveIterations, obj.vchOtherDerivationParameters); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CMasterKey()
    {
        // 25000 rounds is just under 0.1 seconds on a 1.86 GHz Pentium M
        // ie slightly lower than the lowest hardware we need bother supporting
        nDeriveIterations = 25000;
        nDerivationMethod = 0;
        vchOtherDerivationParameters = std::vector<unsigned char>(0);
    }
};

typedef std::vector<unsigned char, secure_allocator<unsigned char> > CKeyingMaterial;

<<<<<<< HEAD
/** Encryption/decryption context with key information */
class CCrypter
{
private:
    unsigned char chKey[WALLET_CRYPTO_KEY_SIZE];
    unsigned char chIV[WALLET_CRYPTO_KEY_SIZE];
    bool fKeySet;

public:
    bool SetKeyFromPassphrase(const SecureString& strKeyData, const std::vector<unsigned char>& chSalt, const unsigned int nRounds, const unsigned int nDerivationMethod);
    bool Encrypt(const CKeyingMaterial& vchPlaintext, std::vector<unsigned char>& vchCiphertext);
    bool Decrypt(const std::vector<unsigned char>& vchCiphertext, CKeyingMaterial& vchPlaintext);
=======
namespace wallet_crypto
{
    class TestCrypter;
}

class CSecureDataStream : public CBaseDataStream<CKeyingMaterial>
{
public:
    explicit CSecureDataStream(int nTypeIn, int nVersionIn) : CBaseDataStream(nTypeIn, nVersionIn) { }

    CSecureDataStream(const_iterator pbegin, const_iterator pend, int nTypeIn, int nVersionIn) :
            CBaseDataStream(pbegin, pend, nTypeIn, nVersionIn) { }

    CSecureDataStream(const vector_type& vchIn, int nTypeIn, int nVersionIn) :
            CBaseDataStream(vchIn, nTypeIn, nVersionIn) { }
};

/** Encryption/decryption context with key information */
class CCrypter
{
friend class wallet_crypto::TestCrypter; // for test access to chKey/chIV
private:
    std::vector<unsigned char, secure_allocator<unsigned char>> vchKey;
    std::vector<unsigned char, secure_allocator<unsigned char>> vchIV;
    bool fKeySet;

    int BytesToKeySHA512AES(const std::vector<unsigned char>& chSalt, const SecureString& strKeyData, int count, unsigned char *key,unsigned char *iv) const;

public:
    bool SetKeyFromPassphrase(const SecureString& strKeyData, const std::vector<unsigned char>& chSalt, const unsigned int nRounds, const unsigned int nDerivationMethod);
    bool Encrypt(const CKeyingMaterial& vchPlaintext, std::vector<unsigned char>& vchCiphertext) const;
    bool Decrypt(const std::vector<unsigned char>& vchCiphertext, CKeyingMaterial& vchPlaintext) const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool SetKey(const CKeyingMaterial& chNewKey, const std::vector<unsigned char>& chNewIV);

    void CleanKey()
    {
<<<<<<< HEAD
        memory_cleanse(chKey, sizeof(chKey));
        memory_cleanse(chIV, sizeof(chIV));
=======
        memory_cleanse(vchKey.data(), vchKey.size());
        memory_cleanse(vchIV.data(), vchIV.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        fKeySet = false;
    }

    CCrypter()
    {
        fKeySet = false;
<<<<<<< HEAD

        // Try to keep the key data out of swap (and be a bit over-careful to keep the IV that we don't even use out of swap)
        // Note that this does nothing about suspend-to-disk (which will put all our key data on disk)
        // Note as well that at no point in this program is any attempt made to prevent stealing of keys by reading the memory of the running process.
        LockedPageManager::Instance().LockRange(&chKey[0], sizeof chKey);
        LockedPageManager::Instance().LockRange(&chIV[0], sizeof chIV);
=======
        vchKey.resize(WALLET_CRYPTO_KEY_SIZE);
        vchIV.resize(WALLET_CRYPTO_IV_SIZE);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    ~CCrypter()
    {
        CleanKey();
<<<<<<< HEAD

        LockedPageManager::Instance().UnlockRange(&chKey[0], sizeof chKey);
        LockedPageManager::Instance().UnlockRange(&chIV[0], sizeof chIV);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
};

bool EncryptSecret(const CKeyingMaterial& vMasterKey, const CKeyingMaterial& vchPlaintext, const uint256& nIV, std::vector<unsigned char>& vchCiphertext);
bool DecryptSecret(const CKeyingMaterial& vMasterKey, const std::vector<unsigned char>& vchCiphertext, const uint256& nIV, CKeyingMaterial& vchPlaintext);

<<<<<<< HEAD
bool EncryptAES256(const SecureString& sKey, const SecureString& sPlaintext, const std::string& sIV, std::string& sCiphertext);
bool DecryptAES256(const SecureString& sKey, const std::string& sCiphertext, const std::string& sIV, SecureString& sPlaintext);

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Keystore which keeps the private keys encrypted.
 * It derives from the basic key store, which is used if no encryption is active.
 */
class CCryptoKeyStore : public CBasicKeyStore
{
private:
<<<<<<< HEAD
    CryptedKeyMap mapCryptedKeys;
    CHDChain cryptedHDChain;

    CKeyingMaterial vMasterKey;

    //! if fUseCrypto is true, mapKeys must be empty
    //! if fUseCrypto is false, vMasterKey must be empty
    bool fUseCrypto;

    //! keeps track of whether Unlock has run a thorough check before
    bool fDecryptionThoroughlyChecked;

protected:
=======
    //! if fUseCrypto is true, mapKeys and mapSaplingSpendingKeys must be empty
    //! if fUseCrypto is false, vMasterKey must be empty
    std::atomic<bool> fUseCrypto;

protected:
    // TODO: In the future, move this variable to the wallet class directly following upstream's structure.
    CKeyingMaterial vMasterKey;
    // Sapling
    CryptedSaplingSpendingKeyMap mapCryptedSaplingSpendingKeys;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool SetCrypted();

    //! will encrypt previously unencrypted keys
    bool EncryptKeys(CKeyingMaterial& vMasterKeyIn);
<<<<<<< HEAD
    bool EncryptHDChain(const CKeyingMaterial& vMasterKeyIn);
    bool DecryptHDChain(CHDChain& hdChainRet) const;
    bool SetHDChain(const CHDChain& chain);
    bool SetCryptedHDChain(const CHDChain& chain);
    bool GetHDChain(CHDChain& hdChainRet) const;

    bool Unlock(const CKeyingMaterial& vMasterKeyIn);

public:
    CCryptoKeyStore() : fUseCrypto(false), fDecryptionThoroughlyChecked(false)
    {
    }
=======

    CryptedKeyMap mapCryptedKeys;

    // Unlock Sapling keys
    bool UnlockSaplingKeys(const CKeyingMaterial& vMasterKeyIn, bool fDecryptionThoroughlyChecked);

public:
    CCryptoKeyStore() : fUseCrypto(false) { }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    bool IsCrypted() const
    {
        return fUseCrypto;
    }

    bool IsLocked() const
    {
        if (!IsCrypted())
            return false;
        bool result;
        {
            LOCK(cs_KeyStore);
            result = vMasterKey.empty();
        }
        return result;
    }

<<<<<<< HEAD
    bool Lock();

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    virtual bool AddCryptedKey(const CPubKey& vchPubKey, const std::vector<unsigned char>& vchCryptedSecret);
    bool AddKeyPubKey(const CKey& key, const CPubKey& pubkey);
    bool HaveKey(const CKeyID& address) const
    {
        {
            LOCK(cs_KeyStore);
            if (!IsCrypted())
                return CBasicKeyStore::HaveKey(address);
            return mapCryptedKeys.count(address) > 0;
        }
        return false;
    }
    bool GetKey(const CKeyID& address, CKey& keyOut) const;
    bool GetPubKey(const CKeyID& address, CPubKey& vchPubKeyOut) const;
    void GetKeys(std::set<CKeyID>& setAddress) const
    {
<<<<<<< HEAD
=======
        LOCK(cs_KeyStore);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!IsCrypted()) {
            CBasicKeyStore::GetKeys(setAddress);
            return;
        }
        setAddress.clear();
        CryptedKeyMap::const_iterator mi = mapCryptedKeys.begin();
        while (mi != mapCryptedKeys.end()) {
            setAddress.insert((*mi).first);
            mi++;
        }
    }

<<<<<<< HEAD
=======
    //! Sapling
    virtual bool AddCryptedSaplingSpendingKey(
            const libzcash::SaplingExtendedFullViewingKey &extfvk,
            const std::vector<unsigned char> &vchCryptedSecret);
    bool HaveSaplingSpendingKey(const libzcash::SaplingExtendedFullViewingKey &extfvk) const;
    bool GetSaplingSpendingKey(const libzcash::SaplingExtendedFullViewingKey &extfvk, libzcash::SaplingExtendedSpendingKey &skOut) const;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /**
     * Wallet status (encrypted, locked) changed.
     * Note: Called without locks held.
     */
    boost::signals2::signal<void(CCryptoKeyStore* wallet)> NotifyStatusChanged;
};

#endif // BITCOIN_CRYPTER_H
