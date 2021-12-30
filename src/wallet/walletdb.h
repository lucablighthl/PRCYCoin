// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2013 The Bitcoin developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_WALLETDB_H
#define BITCOIN_WALLETDB_H

#include "amount.h"
#include "wallet/db.h"
#include "key.h"
#include "keystore.h"
=======
// Copyright (c) 2009-2021 The Bitcoin developers
// Copyright (c) 2016-2021 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_WALLETDB_H
#define PIVX_WALLETDB_H

#include "amount.h"
#include "wallet/db.h"
#include "wallet/hdchain.h"
#include "key.h"
#include "keystore.h"
#include "script/keyorigin.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <list>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>
<<<<<<< HEAD
#include "bignum.h"

class CAccount;
class CStealthAccount;
class CAccountingEntry;
=======

/**
 * Overview of wallet database classes:
 *
 * - WalletBatch is an abstract modifier object for the wallet database, and encapsulates a database
 *   batch update as well as methods to act on the database. It should be agnostic to the database implementation.
 *
 * The following classes are implementation specific:
 * - BerkeleyEnvironment is an environment in which the database exists.
 * - BerkeleyDatabase represents a wallet database.
 * - BerkeleyBatch is a low-level database batch update.
 */

static const bool DEFAULT_FLUSHWALLET = true;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
struct CBlockLocator;
class CKeyPool;
class CMasterKey;
class CScript;
class CWallet;
class CWalletTx;
class uint160;
class uint256;

<<<<<<< HEAD
=======
/** Backend-agnostic database type. */
using WalletDatabase = BerkeleyDatabase;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/** Error statuses for the wallet database */
enum DBErrors {
    DB_LOAD_OK,
    DB_CORRUPT,
    DB_NONCRITICAL_ERROR,
    DB_TOO_NEW,
    DB_LOAD_FAIL,
    DB_NEED_REWRITE
};

class CKeyMetadata
{
public:
<<<<<<< HEAD
    static const int CURRENT_VERSION = 1;
    int nVersion;
    int64_t nCreateTime; // 0 means unknown
=======
    // Metadata versions
    static const int VERSION_BASIC = 1;
    static const int VERSION_WITH_KEY_ORIGIN = 12;
    // Active version
    static const int CURRENT_VERSION = VERSION_WITH_KEY_ORIGIN;

    int nVersion;
    int64_t nCreateTime; // 0 means unknown
    CKeyID hd_seed_id; //id of the HD seed used to derive this key
    KeyOriginInfo key_origin; // Key origin info with path and fingerprint
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CKeyMetadata()
    {
        SetNull();
    }
    CKeyMetadata(int64_t nCreateTime_)
    {
<<<<<<< HEAD
        nVersion = CKeyMetadata::CURRENT_VERSION;
        nCreateTime = nCreateTime_;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(nCreateTime);
=======
        SetNull();
        nCreateTime = nCreateTime_;
    }

    SERIALIZE_METHODS(CKeyMetadata, obj)
    {
        READWRITE(obj.nVersion, obj.nCreateTime);
        if (obj.HasKeyOrigin()) {
            READWRITE(obj.hd_seed_id, obj.key_origin);
        }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    void SetNull()
    {
        nVersion = CKeyMetadata::CURRENT_VERSION;
        nCreateTime = 0;
<<<<<<< HEAD
    }
};

/** Access to the wallet database (wallet.dat) */
class CWalletDB : public CDB
{
public:
    CWalletDB(const std::string& strFilename, const char* pszMode = "r+") : CDB(strFilename, pszMode)
    {
    }
=======
        hd_seed_id.SetNull();
        key_origin.clear();
    }

    bool HasKeyOrigin() const
    {
        return this->nVersion >= VERSION_WITH_KEY_ORIGIN;
    }
};

/** Access to the wallet database.
 * This represents a single transaction at the
 * database. It will be committed when the object goes out of scope.
 * Optionally (on by default) it will flush to disk as well.
 */
class WalletBatch
{
private:
    template <typename K, typename T>
    bool WriteIC(const K& key, const T& value, bool fOverwrite = true)
    {
        if (!m_batch.Write(key, value, fOverwrite)) {
            return false;
        }
        m_database.IncrementUpdateCounter();
        return true;
    }

    template <typename K>
    bool EraseIC(const K& key)
    {
        if (!m_batch.Erase(key)) {
            return false;
        }
        m_database.IncrementUpdateCounter();
        return true;
    }

public:
    explicit WalletBatch(WalletDatabase& database, const char* pszMode = "r+", bool _fFlushOnClose = true) :
        m_batch(database, pszMode, _fFlushOnClose),
        m_database(database)
    {
    }
    WalletBatch(const WalletBatch&) = delete;
    WalletBatch& operator=(const WalletBatch&) = delete;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    bool WriteName(const std::string& strAddress, const std::string& strName);
    bool EraseName(const std::string& strAddress);

    bool WritePurpose(const std::string& strAddress, const std::string& purpose);
    bool ErasePurpose(const std::string& strAddress);

<<<<<<< HEAD
    bool WriteTx(uint256 hash, const CWalletTx& wtx);
    bool EraseTx(uint256 hash);

    bool WriteStakingStatus(bool status);
    bool ReadStakingStatus();

    bool Write2FA(bool status);
    bool Read2FA();

    bool Write2FASecret(std::string secret);
    std::string Read2FASecret();

    bool Write2FAPeriod(int period);
    int Read2FAPeriod();
    
    bool Write2FALastTime(uint64_t lastTime);
    uint64_t Read2FALastTime();

    bool WriteScannedBlockHeight(int height);
    bool ReadScannedBlockHeight(int& height);

    bool WriteReserveAmount(const double &amount);
    bool ReadReserveAmount(double &amount);

    bool WriteTxPrivateKey(const std::string& outpointKey, const std::string& k);
    bool ReadTxPrivateKey(const std::string& outpointKey, std::string& k);

    bool WriteKeyImage(const std::string& outpointKey, const CKeyImage& k);
    bool ReadKeyImage(const std::string& outpointKey, CKeyImage& k);

=======
    bool WriteTx(const CWalletTx& wtx);
    bool EraseTx(uint256 hash);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool WriteKey(const CPubKey& vchPubKey, const CPrivKey& vchPrivKey, const CKeyMetadata& keyMeta);
    bool WriteCryptedKey(const CPubKey& vchPubKey, const std::vector<unsigned char>& vchCryptedSecret, const CKeyMetadata& keyMeta);
    bool WriteMasterKey(unsigned int nID, const CMasterKey& kMasterKey);

    bool WriteCScript(const uint160& hash, const CScript& redeemScript);

    bool WriteWatchOnly(const CScript& script);
    bool EraseWatchOnly(const CScript& script);

<<<<<<< HEAD
    bool WriteMultiSig(const CScript& script);
    bool EraseMultiSig(const CScript& script);

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool WriteBestBlock(const CBlockLocator& locator);
    bool ReadBestBlock(CBlockLocator& locator);

    bool WriteOrderPosNext(int64_t nOrderPosNext);

<<<<<<< HEAD
    // presstab
    bool WriteStakeSplitThreshold(uint64_t nStakeSplitThreshold);
    bool WriteMultiSend(std::vector<std::pair<std::string, int> > vMultiSend);
    bool EraseMultiSend(std::vector<std::pair<std::string, int> > vMultiSend);
    bool WriteMSettings(bool fMultiSendStake, bool fMultiSendMasternode, int nLastMultiSendHeight);
    bool WriteMSDisabledAddresses(std::vector<std::string> vDisabledAddresses);
    bool EraseMSDisabledAddresses(std::vector<std::string> vDisabledAddresses);
    bool WriteAutoCombineSettings(bool fEnable, CAmount nCombineThreshold);

    bool WriteDefaultKey(const CPubKey& vchPubKey);

=======
    bool WriteStakeSplitThreshold(const CAmount& nStakeSplitThreshold);
    bool WriteUseCustomFee(bool fUse);
    bool WriteCustomFeeValue(const CAmount& nCustomFee);
    bool WriteAutoCombineSettings(bool fEnable, CAmount nCombineThreshold);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool ReadPool(int64_t nPool, CKeyPool& keypool);
    bool WritePool(int64_t nPool, const CKeyPool& keypool);
    bool ErasePool(int64_t nPool);

    bool WriteMinVersion(int nVersion);

<<<<<<< HEAD
    /// This writes directly to the database, and will not update the CWallet's cached accounting entries!
    /// Use wallet.AddAccountingEntry instead, to write *and* update its caches.
    bool WriteAccountingEntry_Backend(const CAccountingEntry& acentry);

    bool ReadAccount(const std::string& strAccount, CAccount& account);
    bool WriteAccount(const std::string& strAccount, const CAccount& account);

    bool WriteAutoConsolidateSettingTime(uint32_t settingTime);
    uint32_t ReadAutoConsolidateSettingTime();

    bool ReadStealthAccount(const std::string& strAccount, CStealthAccount& account);
    bool WriteStealthAccount(const std::string& strAccount, const CStealthAccount& account);
=======
    //! write the hdchain model (external/internal chain child index counter)
    bool WriteHDChain(const CHDChain& chain);

    /// Write extended spending key to wallet database, where the key is the incoming viewing key
    bool WriteSaplingZKey(const libzcash::SaplingIncomingViewingKey &ivk,
                          const libzcash::SaplingExtendedSpendingKey &key,
                          const CKeyMetadata  &keyMeta);

    bool WriteSaplingPaymentAddress(const libzcash::SaplingPaymentAddress &addr,
                                    const libzcash::SaplingIncomingViewingKey &ivk);

    bool WriteCryptedSaplingZKey(const libzcash::SaplingExtendedFullViewingKey &extfvk,
                                 const std::vector<unsigned char>& vchCryptedSecret,
                                 const CKeyMetadata &keyMeta);

    /// Common output viewing key, used when shielding transparent funds
    bool WriteSaplingCommonOVK(const uint256& ovk);
    bool ReadSaplingCommonOVK(uint256& ovkRet);

    bool WriteWitnessCacheSize(int64_t nWitnessCacheSize);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /// Write destination data key,value tuple to database
    bool WriteDestData(const std::string& address, const std::string& key, const std::string& value);
    /// Erase destination data tuple from wallet database
    bool EraseDestData(const std::string& address, const std::string& key);
<<<<<<< HEAD
    bool WriteHDChain(const CHDChain& chain);
    bool WriteCryptedHDChain(const CHDChain& chain);
    bool WriteHDPubKey(const CHDPubKey& hdPubKey, const CKeyMetadata& keyMeta);

    CAmount GetAccountCreditDebit(const std::string& strAccount);
    void ListAccountCreditDebit(const std::string& strAccount, std::list<CAccountingEntry>& acentries);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    DBErrors ReorderTransactions(CWallet* pwallet);
    DBErrors LoadWallet(CWallet* pwallet);
    DBErrors FindWalletTx(CWallet* pwallet, std::vector<uint256>& vTxHash, std::vector<CWalletTx>& vWtx);
    DBErrors ZapWalletTx(CWallet* pwallet, std::vector<CWalletTx>& vWtx);
<<<<<<< HEAD
    static bool Recover(CDBEnv& dbenv, std::string filename, bool fOnlyKeys);
    static bool Recover(CDBEnv& dbenv, std::string filename);
    std::list<CBigNum> ListMintedCoinsSerial();
    std::list<CBigNum> ListSpentCoinsSerial();

    bool AppendStealthAccountList(const std::string& accountName);
    bool ReadStealthAccountList(std::string& accountList);
private:
    CWalletDB(const CWalletDB&);
    void operator=(const CWalletDB&);

    bool WriteAccountingEntry(const uint64_t nAccEntryNum, const CAccountingEntry& acentry);
};

bool BackupWallet(const CWallet& wallet, const boost::filesystem::path& strDest, bool fEnableCustom = true);
bool AttemptBackupWallet(const CWallet& wallet, const boost::filesystem::path& pathSrc, const boost::filesystem::path& pathDest);

#endif // BITCOIN_WALLETDB_H
=======
    /* Try to (very carefully!) recover wallet database (with a possible key type filter) */
    static bool Recover(const fs::path& wallet_path, void *callbackDataIn, bool (*recoverKVcallback)(void* callbackData, CDataStream ssKey, CDataStream ssValue), std::string& out_backup_filename);
    /* Recover convenience-function to bypass the key filter callback, called when verify fails, recovers everything */
    static bool Recover(const fs::path& wallet_path, std::string& out_backup_filename);
    /* Recover filter (used as callback), will only let keys (cryptographical keys) as KV/key-type pass through */
    static bool RecoverKeysOnlyFilter(void *callbackData, CDataStream ssKey, CDataStream ssValue);
    /* Function to determin if a certain KV/key-type is a key (cryptographical key) type */
    static bool IsKeyType(const std::string& strType);
    /* verifies the database environment */
    static bool VerifyEnvironment(const fs::path& wallet_path, std::string& errorStr);
    /* verifies the database file */
    static bool VerifyDatabaseFile(const fs::path& wallet_path, std::string& warningStr, std::string& errorStr);

    //! Begin a new transaction
    bool TxnBegin();
    //! Commit current transaction
    bool TxnCommit();
    //! Abort current transaction
    bool TxnAbort();
    //! Read wallet version
    bool ReadVersion(int& nVersion);
    //! Write wallet version
    bool WriteVersion(int nVersion);
private:
    BerkeleyBatch m_batch;
    WalletDatabase& m_database;
};

//! Called during init: Automatic backups
bool AutoBackupWallet(CWallet& wallet, std::string& strBackupWarning, std::string& strBackupError);

//! Compacts BDB state so that wallet.dat is self-contained (if there are changes)
void MaybeCompactWalletDB();

#endif // PIVX_WALLETDB_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
