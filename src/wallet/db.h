// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
=======
// Copyright (c) 2009-2021 The Bitcoin developers
// Copyright (c) 2019-2021 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_DB_H
#define BITCOIN_DB_H

#include "clientversion.h"
<<<<<<< HEAD
=======
#include "fs.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "serialize.h"
#include "streams.h"
#include "sync.h"
#include "version.h"

<<<<<<< HEAD
=======
#include <atomic>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include <map>
#include <string>
#include <vector>

<<<<<<< HEAD
#include <boost/filesystem/path.hpp>

#include <db_cxx.h>

class CDiskBlockIndex;
class COutPoint;

struct CBlockLocator;

extern unsigned int nWalletDBUpdated;

void ThreadFlushWalletDB(const std::string& strWalletFile);


class CDBEnv
=======
#include <db_cxx.h>

static const unsigned int DEFAULT_WALLET_DBLOGSIZE = 100;
static const bool DEFAULT_WALLET_PRIVDB = true;

class BerkeleyEnvironment
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
private:
    bool fDbEnvInit;
    bool fMockDb;
<<<<<<< HEAD
    // Don't change into boost::filesystem::path, as that can result in
    // shutdown problems/crashes caused by a static initialized internal pointer.
    std::string strPath;

    void EnvShutdown();

public:
    mutable RecursiveMutex cs_db;
    DbEnv dbenv;
    std::map<std::string, int> mapFileUseCount;
    std::map<std::string, Db*> mapDb;

    CDBEnv();
    ~CDBEnv();
    void MakeMock();
    bool IsMock() { return fMockDb; }
=======
    // Don't change into fs::path, as that can result in
    // shutdown problems/crashes caused by a static initialized internal pointer.
    std::string strPath;

public:
    std::unique_ptr<DbEnv> dbenv;
    std::map<std::string, int> mapFileUseCount;
    std::map<std::string, Db*> mapDb;

    BerkeleyEnvironment(const fs::path& env_directory);
    ~BerkeleyEnvironment();
    void Reset();

    void MakeMock();
    bool IsMock() const { return fMockDb; }
    bool IsInitialized() const { return fDbEnvInit; }
    fs::path Directory() const { return strPath; }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /**
     * Verify that database file strFile is OK. If it is not,
     * call the callback to try to recover.
     * This must be called BEFORE strFile is opened.
     * Returns true if strFile is OK.
     */
    enum VerifyResult { VERIFY_OK,
<<<<<<< HEAD
        RECOVER_OK,
        RECOVER_FAIL };
    VerifyResult Verify(std::string strFile, bool (*recoverFunc)(CDBEnv& dbenv, std::string strFile));
=======
                        RECOVER_OK,
                        RECOVER_FAIL };
    typedef bool (*recoverFunc_type)(const fs::path& file_path, std::string& out_backup_filename);
    VerifyResult Verify(const std::string& strFile, recoverFunc_type recoverFunc, std::string& out_backup_filename);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /**
     * Salvage data from a file that Verify says is bad.
     * fAggressive sets the DB_AGGRESSIVE flag (see berkeley DB->verify() method documentation).
     * Appends binary key/value pairs to vResult, returns true if successful.
     * NOTE: reads the entire database into memory, so cannot be used
     * for huge databases.
     */
    typedef std::pair<std::vector<unsigned char>, std::vector<unsigned char> > KeyValPair;
<<<<<<< HEAD
    bool Salvage(std::string strFile, bool fAggressive, std::vector<KeyValPair>& vResult);

    bool Open(const boost::filesystem::path& path);
=======
    bool Salvage(const std::string& strFile, bool fAggressive, std::vector<KeyValPair>& vResult);

    bool Open(bool retry);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    void Close();
    void Flush(bool fShutdown);
    void CheckpointLSN(const std::string& strFile);

    void CloseDb(const std::string& strFile);
<<<<<<< HEAD
    bool RemoveDb(const std::string& strFile);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    DbTxn* TxnBegin(int flags = DB_TXN_WRITE_NOSYNC)
    {
        DbTxn* ptxn = NULL;
<<<<<<< HEAD
        int ret = dbenv.txn_begin(NULL, &ptxn, flags);
=======
        int ret = dbenv->txn_begin(NULL, &ptxn, flags);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!ptxn || ret != 0)
            return NULL;
        return ptxn;
    }
};

<<<<<<< HEAD
extern CDBEnv bitdb;


/** RAII class that provides access to a Berkeley database */
class CDB
=======
/** Get BerkeleyEnvironment and database filename given a wallet path. */
BerkeleyEnvironment* GetWalletEnv(const fs::path& wallet_path, std::string& database_filename);

/** An instance of this class represents one database.
 * For BerkeleyDB this is just a (env, strFile) tuple.
 **/
class BerkeleyDatabase
{
    friend class BerkeleyBatch;
public:
    /** Create dummy DB handle */
    BerkeleyDatabase() : nLastSeen(0), nLastFlushed(0), nLastWalletUpdate(0), env(nullptr)
    {
    }

    /** Create DB handle to real database */
    BerkeleyDatabase(const fs::path& wallet_path, bool mock = false) :
        nUpdateCounter(0), nLastSeen(0), nLastFlushed(0), nLastWalletUpdate(0)
    {
        env = GetWalletEnv(wallet_path, strFile);
        if (mock) {
            env->Close();
            env->Reset();
            env->MakeMock();
        }
    }

    /** Return object for accessing database at specified path. */
    static std::unique_ptr<BerkeleyDatabase> Create(const fs::path& path)
    {
        return std::make_unique<BerkeleyDatabase>(path);
    }

    /** Return object for accessing dummy database with no read/write capabilities. */
    static std::unique_ptr<BerkeleyDatabase> CreateDummy()
    {
        return std::make_unique<BerkeleyDatabase>();
    }

    /** Return object for accessing temporary in-memory database. */
    static std::unique_ptr<BerkeleyDatabase> CreateMock()
    {
        return std::make_unique<BerkeleyDatabase>("", true /* mock */);
    }

    /** Rewrite the entire database on disk, with the exception of key pszSkip if non-zero
     */
    bool Rewrite(const char* pszSkip=nullptr);

    /** Back up the entire database to a file.
     */
    bool Backup(const std::string& strDest);

    /** Make sure all changes are flushed to disk.
     */
    void Flush(bool shutdown);

    /** Close and reset.
     */
    void CloseAndReset();

    void IncrementUpdateCounter();
    std::atomic<unsigned int> nUpdateCounter;
    unsigned int nLastSeen;
    unsigned int nLastFlushed;
    int64_t nLastWalletUpdate;
    fs::path GetPathToFile() { return env->Directory() / strFile; }

private:
    /** BerkeleyDB specific */
    BerkeleyEnvironment *env;
    std::string strFile;

    /** Return whether this database handle is a dummy for testing.
     * Only to be used at a low level, application should ideally not care
     * about this.
     */
    bool IsDummy() { return env == nullptr; }
};


/** RAII class that provides access to a Berkeley database */
class BerkeleyBatch
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
protected:
    Db* pdb;
    std::string strFile;
    DbTxn* activeTxn;
    bool fReadOnly;
<<<<<<< HEAD

    explicit CDB(const std::string& strFilename, const char* pszMode = "r+");
    ~CDB() { Close(); }

public:
    void Flush();
    void Close();

private:
    CDB(const CDB&);
    void operator=(const CDB&);

protected:
=======
    bool fFlushOnClose;
    BerkeleyEnvironment *env;

public:
    explicit BerkeleyBatch(BerkeleyDatabase& database, const char* pszMode = "r+", bool fFlushOnCloseIn=true);
    ~BerkeleyBatch() { Close(); }

    BerkeleyBatch(const BerkeleyBatch&) = delete;
    BerkeleyBatch& operator=(const BerkeleyBatch&) = delete;

    void Flush();
    void Close();
    static bool Recover(const fs::path& file_path, void *callbackDataIn, bool (*recoverKVcallback)(void* callbackData, CDataStream ssKey, CDataStream ssValue), std::string& out_backup_filename);

    /* flush the wallet passively (TRY_LOCK)
       ideal to be called periodically */
    static bool PeriodicFlush(BerkeleyDatabase& database);
    /* verifies the database environment */
    static bool VerifyEnvironment(const fs::path& file_path, std::string& errorStr);
    /* verifies the database file */
    static bool VerifyDatabaseFile(const fs::path& file_path, std::string& warningStr, std::string& errorStr, BerkeleyEnvironment::recoverFunc_type recoverFunc);

public:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    template <typename K, typename T>
    bool Read(const K& key, T& value)
    {
        if (!pdb)
            return false;

        // Key
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;
<<<<<<< HEAD
        Dbt datKey(&ssKey[0], ssKey.size());
=======
        Dbt datKey(ssKey.data(), ssKey.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Read
        Dbt datValue;
        datValue.set_flags(DB_DBT_MALLOC);
        int ret = pdb->get(activeTxn, &datKey, &datValue, 0);
        memory_cleanse(datKey.get_data(), datKey.get_size());
        bool success = false;
        if (datValue.get_data() != NULL) {
            // Unserialize value
            try {
                CDataStream ssValue((char*)datValue.get_data(), (char*)datValue.get_data() + datValue.get_size(), SER_DISK, CLIENT_VERSION);
                ssValue >> value;
                success = true;
            } catch (const std::exception&) {
                // In this case success remains 'false'
            }

            // Clear and free memory
            memory_cleanse(datValue.get_data(), datValue.get_size());
            free(datValue.get_data());
        }
        return ret == 0 && success;
    }

    template <typename K, typename T>
    bool Write(const K& key, const T& value, bool fOverwrite = true)
    {
        if (!pdb)
<<<<<<< HEAD
            return false;
=======
            return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (fReadOnly)
            assert(!"Write called on database in read-only mode");

        // Key
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;
<<<<<<< HEAD
        Dbt datKey(&ssKey[0], ssKey.size());
=======
        Dbt datKey(ssKey.data(), ssKey.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Value
        CDataStream ssValue(SER_DISK, CLIENT_VERSION);
        ssValue.reserve(10000);
        ssValue << value;
<<<<<<< HEAD
        Dbt datValue(&ssValue[0], ssValue.size());
=======
        Dbt datValue(ssValue.data(), ssValue.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Write
        int ret = pdb->put(activeTxn, &datKey, &datValue, (fOverwrite ? 0 : DB_NOOVERWRITE));

        // Clear memory in case it was a private key
        memory_cleanse(datKey.get_data(), datKey.get_size());
        memory_cleanse(datValue.get_data(), datValue.get_size());
        return (ret == 0);
    }

    template <typename K>
    bool Erase(const K& key)
    {
        if (!pdb)
            return false;
        if (fReadOnly)
            assert(!"Erase called on database in read-only mode");

        // Key
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;
<<<<<<< HEAD
        Dbt datKey(&ssKey[0], ssKey.size());
=======
        Dbt datKey(ssKey.data(), ssKey.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Erase
        int ret = pdb->del(activeTxn, &datKey, 0);

        // Clear memory
<<<<<<< HEAD
         memory_cleanse(datKey.get_data(), datKey.get_size());
=======
        memory_cleanse(datKey.get_data(), datKey.get_size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return (ret == 0 || ret == DB_NOTFOUND);
    }

    template <typename K>
    bool Exists(const K& key)
    {
        if (!pdb)
            return false;

        // Key
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        ssKey.reserve(1000);
        ssKey << key;
<<<<<<< HEAD
        Dbt datKey(&ssKey[0], ssKey.size());
=======
        Dbt datKey(ssKey.data(), ssKey.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Exists
        int ret = pdb->exists(activeTxn, &datKey, 0);

        // Clear memory
        memory_cleanse(datKey.get_data(), datKey.get_size());
        return (ret == 0);
    }

    Dbc* GetCursor()
    {
        if (!pdb)
            return NULL;
        Dbc* pcursor = NULL;
        int ret = pdb->cursor(NULL, &pcursor, 0);
        if (ret != 0)
            return NULL;
        return pcursor;
    }

<<<<<<< HEAD
    int ReadAtCursor(Dbc* pcursor, CDataStream& ssKey, CDataStream& ssValue, unsigned int fFlags = DB_NEXT)
    {
        // Read at cursor
        Dbt datKey;
        datKey.set_data(NULL);
        if (fFlags == DB_SET || fFlags == DB_SET_RANGE || fFlags == DB_GET_BOTH || fFlags == DB_GET_BOTH_RANGE) {
            datKey.set_data(&ssKey[0]);
            datKey.set_size(ssKey.size());
        }
        Dbt datValue;
        datValue.set_data(NULL);
        if (fFlags == DB_GET_BOTH || fFlags == DB_GET_BOTH_RANGE) {
            datValue.set_data(&ssValue[0]);
            datValue.set_size(ssValue.size());
        }
        datKey.set_flags(DB_DBT_MALLOC);
        datValue.set_flags(DB_DBT_MALLOC);
        int ret = pcursor->get(&datKey, &datValue, fFlags);
        if (ret != 0) {
            if (datKey.get_data() != NULL)
                free(datKey.get_data());
            if (datValue.get_data() != NULL)
                free(datValue.get_data());

            return ret;
        }
        else if (datKey.get_data() == NULL || datValue.get_data() == NULL) {
            if (datKey.get_data() != NULL)
                free(datKey.get_data());
            if (datValue.get_data() != NULL)
                free(datValue.get_data());

            return 99999;
        }
=======
    int ReadAtCursor(Dbc* pcursor, CDataStream& ssKey, CDataStream& ssValue, bool setRange = false)
    {
        // Read at cursor
        Dbt datKey;
        unsigned int fFlags = DB_NEXT;
        if (setRange) {
            datKey.set_data(ssKey.data());
            datKey.set_size(ssKey.size());
            fFlags = DB_SET_RANGE;
        }
        Dbt datValue;
        datKey.set_flags(DB_DBT_MALLOC);
        datValue.set_flags(DB_DBT_MALLOC);
        int ret = pcursor->get(&datKey, &datValue, fFlags);
        if (ret != 0)
            return ret;
        else if (datKey.get_data() == NULL || datValue.get_data() == NULL)
            return 99999;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Convert to streams
        ssKey.SetType(SER_DISK);
        ssKey.clear();
        ssKey.write((char*)datKey.get_data(), datKey.get_size());
        ssValue.SetType(SER_DISK);
        ssValue.clear();
        ssValue.write((char*)datValue.get_data(), datValue.get_size());

        // Clear and free memory
        memory_cleanse(datKey.get_data(), datKey.get_size());
        memory_cleanse(datValue.get_data(), datValue.get_size());
        free(datKey.get_data());
        free(datValue.get_data());
        return 0;
    }

public:
    bool TxnBegin()
    {
        if (!pdb || activeTxn)
            return false;
<<<<<<< HEAD
        DbTxn* ptxn = bitdb.TxnBegin();
=======
        DbTxn* ptxn = env->TxnBegin();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!ptxn)
            return false;
        activeTxn = ptxn;
        return true;
    }

    bool TxnCommit()
    {
        if (!pdb || !activeTxn)
            return false;
        int ret = activeTxn->commit(0);
        activeTxn = NULL;
        return (ret == 0);
    }

    bool TxnAbort()
    {
        if (!pdb || !activeTxn)
            return false;
        int ret = activeTxn->abort();
        activeTxn = NULL;
        return (ret == 0);
    }

    bool ReadVersion(int& nVersion)
    {
        nVersion = 0;
        return Read(std::string("version"), nVersion);
    }

    bool WriteVersion(int nVersion)
    {
        return Write(std::string("version"), nVersion);
    }

<<<<<<< HEAD
    bool static Rewrite(const std::string& strFile, const char* pszSkip = NULL);
=======
    bool static Rewrite(BerkeleyDatabase& database, const char* pszSkip = NULL);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_DB_H
