// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet/db.h"

#include "addrman.h"
#include "hash.h"
#include "protocol.h"
#include "util.h"
#include "utilstrencodings.h"
=======
// Copyright (c) 2009-2021 The Bitcoin developers
// Copyright (c) 2019-2021 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "db.h"

#include "addrman.h"
#include "guiinterfaceutil.h"
#include "hash.h"
#include "protocol.h"
#include "util/system.h"
#include "utilstrencodings.h"
#include "wallet/walletutil.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <stdint.h>

#ifndef WIN32
#include <sys/stat.h>
#endif

<<<<<<< HEAD
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <boost/version.hpp>

using namespace std;
using namespace boost;


unsigned int nWalletDBUpdated;


//
// CDB
//

CDBEnv bitdb;

void CDBEnv::EnvShutdown()
=======
#include <boost/thread.hpp>


namespace {
//! Make sure database has a unique fileid within the environment. If it
//! doesn't, throw an error. BDB caches do not work properly when more than one
//! open database has the same fileid (values written to one database may show
//! up in reads to other databases).
//!
//! BerkeleyDB generates unique fileids by default
//! (https://docs.oracle.com/cd/E17275_01/html/programmer_reference/program_copy.html),
//! so bitcoin should never create different databases with the same fileid, but
//! this error can be triggered if users manually copy database files.
void CheckUniqueFileid(const BerkeleyEnvironment& env, const std::string& filename, Db& db)
{
    if (env.IsMock()) return;

    u_int8_t fileid[DB_FILE_ID_LEN];
    int ret = db.get_mpf()->get_fileid(fileid);
    if (ret != 0) {
        throw std::runtime_error(strprintf("BerkeleyBatch: Can't open database %s (get_fileid failed with %d)", filename, ret));
    }

    for (const auto& item : env.mapDb) {
        u_int8_t item_fileid[DB_FILE_ID_LEN];
        if (item.second && item.second->get_mpf()->get_fileid(item_fileid) == 0 &&
            memcmp(fileid, item_fileid, sizeof(fileid)) == 0) {
            const char* item_filename = nullptr;
            item.second->get_dbname(&item_filename, nullptr);
            throw std::runtime_error(strprintf("BerkeleyBatch: Can't open database %s (duplicates fileid %s from %s)", filename,
                HexStr(item_fileid),
                item_filename ? item_filename : "(unknown database)"));
        }
    }
}

RecursiveMutex cs_db;
std::map<std::string, BerkeleyEnvironment> g_dbenvs; //!< Map from directory name to open db environment.
} // namespace

BerkeleyEnvironment* GetWalletEnv(const fs::path& wallet_path, std::string& database_filename)
{
    fs::path env_directory;
    if (fs::is_regular_file(wallet_path)) {
        // Special case for backwards compatibility: if wallet path points to an
        // existing file, treat it as the path to a BDB data file in a parent
        // directory that also contains BDB log files.
        env_directory = wallet_path.parent_path();
        database_filename = wallet_path.filename().string();
    } else {
        // Normal case: Interpret wallet path as a directory path containing
        // data and log files.
        env_directory = wallet_path;
        database_filename = "wallet.dat";
    }
    LOCK(cs_db);
    // Note: An ununsed temporary BerkeleyEnvironment object may be created inside the
    // emplace function if the key already exists. This is a little inefficient,
    // but not a big concern since the map will be changed in the future to hold
    // pointers instead of objects, anyway.
    return &g_dbenvs.emplace(std::piecewise_construct, std::forward_as_tuple(env_directory.string()), std::forward_as_tuple(env_directory)).first->second;
}

//
// BerkeleyBatch
//

void BerkeleyEnvironment::Close()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (!fDbEnvInit)
        return;

    fDbEnvInit = false;
<<<<<<< HEAD
    int ret = dbenv.close(0);
    if (ret != 0)
        LogPrintf("CDBEnv::EnvShutdown : Error %d shutting down database environment: %s\n", ret, DbEnv::strerror(ret));
=======

    for (auto& db : mapDb) {
        auto count = mapFileUseCount.find(db.first);
        assert(count == mapFileUseCount.end() || count->second == 0);
        if (db.second) {
            db.second->close(0);
            delete db.second;
            db.second = nullptr;
        }
    }

    int ret = dbenv->close(0);
    if (ret != 0)
        LogPrintf("Error %d shutting down database environment: %s\n", ret, DbEnv::strerror(ret));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fMockDb)
        DbEnv((u_int32_t)0).remove(strPath.c_str(), 0);
}

<<<<<<< HEAD
CDBEnv::CDBEnv() : dbenv(DB_CXX_NO_EXCEPTIONS)
{
=======
void BerkeleyEnvironment::Reset()
{
    dbenv.reset(new DbEnv(DB_CXX_NO_EXCEPTIONS));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    fDbEnvInit = false;
    fMockDb = false;
}

<<<<<<< HEAD
CDBEnv::~CDBEnv()
{
    EnvShutdown();
}

void CDBEnv::Close()
{
    EnvShutdown();
}

bool CDBEnv::Open(const boost::filesystem::path& pathIn)
=======
BerkeleyEnvironment::BerkeleyEnvironment(const fs::path& dir_path) : strPath(dir_path.string())
{
    Reset();
}

BerkeleyEnvironment::~BerkeleyEnvironment()
{
    Close();
}

bool BerkeleyEnvironment::Open(bool retry)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (fDbEnvInit)
        return true;

    boost::this_thread::interruption_point();

<<<<<<< HEAD
    strPath = pathIn.string();
    boost::filesystem::path pathLogDir = pathIn / "database";
    TryCreateDirectory(pathLogDir);
    boost::filesystem::path pathErrorFile = pathIn / "db.log";
    LogPrintf("CDBEnv::Open: LogDir=%s ErrorFile=%s\n", pathLogDir.string(), pathErrorFile.string());

    unsigned int nEnvFlags = 0;
    if (GetBoolArg("-privdb", true))
        nEnvFlags |= DB_PRIVATE;

    dbenv.set_lg_dir(pathLogDir.string().c_str());
    dbenv.set_cachesize(0, 0x100000, 1); // 1 MiB should be enough for just the wallet
    dbenv.set_lg_bsize(0x10000);
    dbenv.set_lg_max(1048576);
    dbenv.set_lk_max_locks(40000);
    dbenv.set_lk_max_objects(40000);
    dbenv.set_errfile(fopen(pathErrorFile.string().c_str(), "a")); /// debug
    dbenv.set_flags(DB_AUTO_COMMIT, 1);
    dbenv.set_flags(DB_TXN_WRITE_NOSYNC, 1);
    dbenv.log_set_config(DB_LOG_AUTO_REMOVE, 1);
    int ret = dbenv.open(strPath.c_str(),
=======
    fs::path pathIn = strPath;
    TryCreateDirectories(pathIn);
    if (!LockDirectory(pathIn, ".walletlock")) {
        LogPrintf("Cannot obtain a lock on wallet directory %s. Another instance of PIVX may be using it.\n", strPath);
        return false;
    }

    fs::path pathLogDir = pathIn / "database";
    TryCreateDirectories(pathLogDir);
    fs::path pathErrorFile = pathIn / "db.log";
    LogPrintf("BerkeleyEnvironment::Open: LogDir=%s ErrorFile=%s\n", pathLogDir.string(), pathErrorFile.string());

    unsigned int nEnvFlags = 0;
    if (gArgs.GetBoolArg("-privdb", DEFAULT_WALLET_PRIVDB))
        nEnvFlags |= DB_PRIVATE;

    dbenv->set_lg_dir(pathLogDir.string().c_str());
    dbenv->set_cachesize(0, 0x100000, 1); // 1 MiB should be enough for just the wallet
    dbenv->set_lg_bsize(0x10000);
    dbenv->set_lg_max(1048576);
    dbenv->set_lk_max_locks(40000);
    dbenv->set_lk_max_objects(40000);
    dbenv->set_errfile(fsbridge::fopen(pathErrorFile, "a")); /// debug
    dbenv->set_flags(DB_AUTO_COMMIT, 1);
    dbenv->set_flags(DB_TXN_WRITE_NOSYNC, 1);
    dbenv->log_set_config(DB_LOG_AUTO_REMOVE, 1);
    int ret = dbenv->open(strPath.c_str(),
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        DB_CREATE |
            DB_INIT_LOCK |
            DB_INIT_LOG |
            DB_INIT_MPOOL |
            DB_INIT_TXN |
            DB_THREAD |
            DB_RECOVER |
            nEnvFlags,
        S_IRUSR | S_IWUSR);
<<<<<<< HEAD
    if (ret != 0)
        return error("CDBEnv::Open : Error %d opening database environment: %s\n", ret, DbEnv::strerror(ret));
=======
    if (ret != 0) {
        dbenv->close(0);
        LogPrintf("BerkeleyEnvironment::Open: Error %d opening database environment: %s\n", ret, DbEnv::strerror(ret));
        if (retry) {
            // try moving the database env out of the way
            fs::path pathDatabaseBak = pathIn / strprintf("database.%d.bak", GetTime());
            try {
                fs::rename(pathLogDir, pathDatabaseBak);
                LogPrintf("Moved old %s to %s. Retrying.\n", pathLogDir.string(), pathDatabaseBak.string());
            } catch (const fs::filesystem_error&) {
                // failure is ok (well, not really, but it's not worse than what we started with)
            }
            // try opening it again one more time
            if (!Open(false /* retry */)) {
                // if it still fails, it probably means we can't even create the database env
                return false;
            }
        } else {
            return false;
        }
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    fDbEnvInit = true;
    fMockDb = false;
    return true;
}

<<<<<<< HEAD
void CDBEnv::MakeMock()
{
    if (fDbEnvInit)
        throw runtime_error("CDBEnv::MakeMock : Already initialized");

    boost::this_thread::interruption_point();

    LogPrint("db", "CDBEnv::MakeMock\n");

    dbenv.set_cachesize(1, 0, 1);
    dbenv.set_lg_bsize(10485760 * 4);
    dbenv.set_lg_max(10485760);
    dbenv.set_lk_max_locks(10000);
    dbenv.set_lk_max_objects(10000);
    dbenv.set_flags(DB_AUTO_COMMIT, 1);
    dbenv.log_set_config(DB_LOG_IN_MEMORY, 1);
    int ret = dbenv.open(NULL,
=======
void BerkeleyEnvironment::MakeMock()
{
    if (fDbEnvInit)
        throw std::runtime_error("BerkeleyEnvironment::MakeMock : Already initialized");

    boost::this_thread::interruption_point();

    LogPrint(BCLog::DB, "BerkeleyEnvironment::MakeMock\n");

    dbenv->set_cachesize(1, 0, 1);
    dbenv->set_lg_bsize(10485760 * 4);
    dbenv->set_lg_max(10485760);
    dbenv->set_lk_max_locks(10000);
    dbenv->set_lk_max_objects(10000);
    dbenv->set_flags(DB_AUTO_COMMIT, 1);
    dbenv->log_set_config(DB_LOG_IN_MEMORY, 1);
    int ret = dbenv->open(NULL,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        DB_CREATE |
            DB_INIT_LOCK |
            DB_INIT_LOG |
            DB_INIT_MPOOL |
            DB_INIT_TXN |
            DB_THREAD |
            DB_PRIVATE,
        S_IRUSR | S_IWUSR);
    if (ret > 0)
<<<<<<< HEAD
        throw runtime_error(strprintf("CDBEnv::MakeMock : Error %d opening database environment.", ret));
=======
        throw std::runtime_error(strprintf("BerkeleyEnvironment::MakeMock : Error %d opening database environment.", ret));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    fDbEnvInit = true;
    fMockDb = true;
}

<<<<<<< HEAD
CDBEnv::VerifyResult CDBEnv::Verify(std::string strFile, bool (*recoverFunc)(CDBEnv& dbenv, std::string strFile))
=======
BerkeleyEnvironment::VerifyResult BerkeleyEnvironment::Verify(const std::string& strFile, recoverFunc_type recoverFunc, std::string& out_backup_filename)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    LOCK(cs_db);
    assert(mapFileUseCount.count(strFile) == 0);

<<<<<<< HEAD
    Db db(&dbenv, 0);
    int result = db.verify(strFile.c_str(), NULL, NULL, 0);
    if (result == 0)
        return VERIFY_OK;
    else if (recoverFunc == NULL)
        return RECOVER_FAIL;

    // Try to recover:
    bool fRecovered = (*recoverFunc)(*this, strFile);
    return (fRecovered ? RECOVER_OK : RECOVER_FAIL);
}

bool CDBEnv::Salvage(std::string strFile, bool fAggressive, std::vector<CDBEnv::KeyValPair>& vResult)
=======
    Db db(dbenv.get(), 0);
    int result = db.verify(strFile.c_str(), nullptr, nullptr, 0);
    if (result == 0)
        return VERIFY_OK;
    else if (recoverFunc == nullptr)
        return RECOVER_FAIL;

    // Try to recover:
    bool fRecovered = (*recoverFunc)(fs::path(strPath) / strFile, out_backup_filename);
    return (fRecovered ? RECOVER_OK : RECOVER_FAIL);
}

bool BerkeleyBatch::Recover(const fs::path& file_path, void *callbackDataIn, bool (*recoverKVcallback)(void* callbackData, CDataStream ssKey, CDataStream ssValue), std::string& newFilename)
{
    std::string filename;
    BerkeleyEnvironment* env = GetWalletEnv(file_path, filename);

    // Recovery procedure:
    // move wallet file to walletfilename.timestamp.bak
    // Call Salvage with fAggressive=true to
    // get as much data as possible.
    // Rewrite salvaged data to fresh wallet file
    // Set -rescan so any missing transactions will be
    // found.
    int64_t now = GetTime();
    newFilename = strprintf("%s.%d.bak", filename, now);

    int result = env->dbenv->dbrename(nullptr, filename.c_str(), nullptr,
                                       newFilename.c_str(), DB_AUTO_COMMIT);
    if (result == 0) {
        LogPrintf("Renamed %s to %s\n", filename, newFilename);
    } else {
        LogPrintf("Failed to rename %s to %s\n", filename, newFilename);
        return false;
    }

    std::vector<BerkeleyEnvironment::KeyValPair> salvagedData;
    bool fSuccess = env->Salvage(newFilename, true, salvagedData);
    if (salvagedData.empty())
    {
        LogPrintf("Salvage(aggressive) found no records in %s.\n", newFilename);
        return false;
    }
    LogPrintf("Salvage(aggressive) found %u records\n", salvagedData.size());

    std::unique_ptr<Db> pdbCopy = std::make_unique<Db>(env->dbenv.get(), 0);
    int ret = pdbCopy->open(nullptr,            // Txn pointer
                            filename.c_str(),   // Filename
                            "main",             // Logical db name
                            DB_BTREE,           // Database type
                            DB_CREATE,          // Flags
                            0);
    if (ret > 0) {
        LogPrintf("Cannot create database file %s\n", filename);
        pdbCopy->close(0);
        return false;
    }

    DbTxn* ptxn = env->TxnBegin();
    for (BerkeleyEnvironment::KeyValPair& row : salvagedData)
    {
        if (recoverKVcallback)
        {
            CDataStream ssKey(row.first, SER_DISK, CLIENT_VERSION);
            CDataStream ssValue(row.second, SER_DISK, CLIENT_VERSION);
            if (!(*recoverKVcallback)(callbackDataIn, ssKey, ssValue))
                continue;
        }
        Dbt datKey(&row.first[0], row.first.size());
        Dbt datValue(&row.second[0], row.second.size());
        int ret2 = pdbCopy->put(ptxn, &datKey, &datValue, DB_NOOVERWRITE);
        if (ret2 > 0)
            fSuccess = false;
    }
    ptxn->commit(0);
    pdbCopy->close(0);

    return fSuccess;
}

bool BerkeleyBatch::VerifyEnvironment(const fs::path& file_path, std::string& errorStr)
{
    std::string walletFile;
    BerkeleyEnvironment* env = GetWalletEnv(file_path, walletFile);
    fs::path walletDir = env->Directory();

    LogPrintf("Using BerkeleyDB version %s\n", DbEnv::version(0, 0, 0));
    LogPrintf("Using wallet %s\n", walletFile);

    // Wallet file must be a plain filename without a directory
    fs::path wallet_file_path(walletFile);
    if (walletFile != wallet_file_path.filename().string())
        return UIError(strprintf(_("Wallet %s resides outside data directory %s"), walletFile, walletDir.string()));

    if (!env->Open(true /* retry */)) {
        errorStr = strprintf(_("Error initializing wallet database environment %s!"), walletDir);
        return false;
    }

    return true;
}

bool BerkeleyBatch::VerifyDatabaseFile(const fs::path& file_path, std::string& warningStr, std::string& errorStr, BerkeleyEnvironment::recoverFunc_type recoverFunc)
{
    std::string walletFile;
    BerkeleyEnvironment* env = GetWalletEnv(file_path, walletFile);
    fs::path walletDir = env->Directory();

    if (fs::exists(walletDir / walletFile))
    {
        std::string backup_filename;
        BerkeleyEnvironment::VerifyResult r = env->Verify(walletFile, recoverFunc, backup_filename);
        if (r == BerkeleyEnvironment::RECOVER_OK)
        {
            warningStr = strprintf(_("Warning: Wallet file corrupt, data salvaged!"
                                     " Original %s saved as %s in %s; if"
                                     " your balance or transactions are incorrect you should"
                                     " restore from a backup."),
                                   walletFile, backup_filename, walletDir);
        }
        if (r == BerkeleyEnvironment::RECOVER_FAIL) {
            errorStr = strprintf(_("%s corrupt, salvage failed"), walletFile);
            return false;
        }
    }
    // also return true if files does not exists
    return true;
}

/* End of headers, beginning of key/value data */
static const char *HEADER_END = "HEADER=END";
/* End of key/value data */
static const char *DATA_END = "DATA=END";

bool BerkeleyEnvironment::Salvage(const std::string& strFile, bool fAggressive, std::vector<BerkeleyEnvironment::KeyValPair>& vResult)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    LOCK(cs_db);
    assert(mapFileUseCount.count(strFile) == 0);

    u_int32_t flags = DB_SALVAGE;
    if (fAggressive)
        flags |= DB_AGGRESSIVE;

<<<<<<< HEAD
    stringstream strDump;

    Db db(&dbenv, 0);
    int result = db.verify(strFile.c_str(), NULL, &strDump, flags);
    if (result == DB_VERIFY_BAD) {
        LogPrintf("CDBEnv::Salvage : Database salvage found errors, all data may not be recoverable.\n");
        if (!fAggressive) {
            LogPrintf("CDBEnv::Salvage : Rerun with aggressive mode to ignore errors and continue.\n");
=======
    std::stringstream strDump;

    Db db(dbenv.get(), 0);
    int result = db.verify(strFile.c_str(), nullptr, &strDump, flags);
    if (result == DB_VERIFY_BAD) {
        LogPrintf("BerkeleyEnvironment::Salvage : Database salvage found errors, all data may not be recoverable.\n");
        if (!fAggressive) {
            LogPrintf("BerkeleyEnvironment::Salvage : Rerun with aggressive mode to ignore errors and continue.\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return false;
        }
    }
    if (result != 0 && result != DB_VERIFY_BAD) {
<<<<<<< HEAD
        LogPrintf("CDBEnv::Salvage : Database salvage failed with result %d.\n", result);
=======
        LogPrintf("BerkeleyEnvironment::Salvage : Database salvage failed with result %d.\n", result);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    }

    // Format of bdb dump is ascii lines:
    // header lines...
    // HEADER=END
    // hexadecimal key
    // hexadecimal value
    // ... repeated
    // DATA=END

<<<<<<< HEAD
    string strLine;
    while (!strDump.eof() && strLine != "HEADER=END")
        getline(strDump, strLine); // Skip past header

    std::string keyHex, valueHex;
    while (!strDump.eof() && keyHex != "DATA=END") {
        getline(strDump, keyHex);
        if (keyHex != "DATA=END") {
            getline(strDump, valueHex);
            vResult.push_back(make_pair(ParseHex(keyHex), ParseHex(valueHex)));
        }
    }

=======
    std::string strLine;
    while (!strDump.eof() && strLine != HEADER_END)
        getline(strDump, strLine); // Skip past header

    std::string keyHex, valueHex;
    while (!strDump.eof() && keyHex != DATA_END) {
        getline(strDump, keyHex);
        if (keyHex != DATA_END) {
            if (strDump.eof())
                break;
            getline(strDump, valueHex);
            if (valueHex == DATA_END) {
                LogPrintf("BerkeleyEnvironment::Salvage: WARNING: Number of keys in data does not match number of values.\n");
                break;
            }
            vResult.emplace_back(ParseHex(keyHex), ParseHex(valueHex));
        }
    }

    if (keyHex != DATA_END) {
        LogPrintf("BerkeleyEnvironment::Salvage: WARNING: Unexpected end of file while reading salvage output.\n");
        return false;
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return (result == 0);
}


<<<<<<< HEAD
void CDBEnv::CheckpointLSN(const std::string& strFile)
{
    dbenv.txn_checkpoint(0, 0, 0);
    if (fMockDb)
        return;
    dbenv.lsn_reset(strFile.c_str(), 0);
}


CDB::CDB(const std::string& strFilename, const char* pszMode) : pdb(NULL), activeTxn(NULL)
{
    int ret;
    fReadOnly = (!strchr(pszMode, '+') && !strchr(pszMode, 'w'));
    if (strFilename.empty())
        return;

    bool fCreate = strchr(pszMode, 'c') != NULL;
=======
void BerkeleyEnvironment::CheckpointLSN(const std::string& strFile)
{
    dbenv->txn_checkpoint(0, 0, 0);
    if (fMockDb)
        return;
    dbenv->lsn_reset(strFile.c_str(), 0);
}


BerkeleyBatch::BerkeleyBatch(BerkeleyDatabase& database, const char* pszMode, bool fFlushOnCloseIn) : pdb(nullptr), activeTxn(nullptr)
{
    fReadOnly = (!strchr(pszMode, '+') && !strchr(pszMode, 'w'));
    fFlushOnClose = fFlushOnCloseIn;
    env = database.env;
    if (database.IsDummy()) {
        return;
    }
    const std::string &strFilename = database.strFile;

    bool fCreate = strchr(pszMode, 'c') != nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned int nFlags = DB_THREAD;
    if (fCreate)
        nFlags |= DB_CREATE;

    {
<<<<<<< HEAD
        LOCK(bitdb.cs_db);
        if (!bitdb.Open(GetDataDir()))
            throw runtime_error("CDB : Failed to open database environment.");

        strFile = strFilename;
        ++bitdb.mapFileUseCount[strFile];
        pdb = bitdb.mapDb[strFile];
        if (pdb == NULL) {
            pdb = new Db(&bitdb.dbenv, 0);

            bool fMockDb = bitdb.IsMock();
            if (fMockDb) {
                DbMpoolFile* mpf = pdb->get_mpf();
                ret = mpf->set_flags(DB_MPOOL_NOFILE, 1);
                if (ret != 0)
                    throw runtime_error(strprintf("CDB : Failed to configure for no temp file backing for database %s", strFile));
            }

            ret = pdb->open(NULL,                   // Txn pointer
                fMockDb ? NULL : strFile.c_str(),   // Filename
                fMockDb ? strFile.c_str() : "main", // Logical db name
                DB_BTREE,                           // Database type
                nFlags,                             // Flags
                0);

            if (ret != 0) {
                delete pdb;
                pdb = NULL;
                --bitdb.mapFileUseCount[strFile];
                std::string tempCopy(strFile);
                strFile = "";
                throw std::runtime_error(strprintf("CDB : Error %d, can't open database %s", ret, tempCopy));
            }

            if (fCreate && !Exists(string("version"))) {
=======
        LOCK(cs_db);
        if (!env->Open(false /* retry */))
            throw std::runtime_error("BerkeleyBatch: Failed to open database environment.");

        pdb = env->mapDb[strFilename];
        if (pdb == nullptr) {
            int ret;
            std::unique_ptr<Db> pdb_temp = std::make_unique<Db>(env->dbenv.get(), 0);

            bool fMockDb = env->IsMock();
            if (fMockDb) {
                DbMpoolFile* mpf = pdb_temp->get_mpf();
                ret = mpf->set_flags(DB_MPOOL_NOFILE, 1);
                if (ret != 0) {
                    throw std::runtime_error(strprintf("BerkeleyBatch: Failed to configure for no temp file backing for database %s", strFilename));
                }
            }

            ret = pdb_temp->open(nullptr,                                  // Txn pointer
                                 fMockDb ? nullptr : strFilename.c_str(),  // Filename
                                 fMockDb ? strFilename.c_str() : "main",   // Logical db name
                                 DB_BTREE,                                 // Database type
                                 nFlags,                                   // Flags
                                 0);

            if (ret != 0) {
                throw std::runtime_error(strprintf("BerkeleyBatch: Error %d, can't open database %s", ret, strFilename));
            }

            // Call CheckUniqueFileid on the containing BDB environment to
            // avoid BDB data consistency bugs that happen when different data
            // files in the same environment have the same fileid.
            //
            // Also call CheckUniqueFileid on all the other g_dbenvs to prevent
            // PIVX from opening the same data file through another
            // environment when the file is referenced through equivalent but
            // not obviously identical symlinked or hard linked or bind mounted
            // paths. In the future a more relaxed check for equal inode and
            // device ids could be done instead, which would allow opening
            // different backup copies of a wallet at the same time. Maybe even
            // more ideally, an exclusive lock for accessing the database could
            // be implemented, so no equality checks are needed at all. (Newer
            // versions of BDB have an set_lk_exclusive method for this
            // purpose, but the older version we use does not.)
            for (auto& env : g_dbenvs) {
                CheckUniqueFileid(env.second, strFilename, *pdb_temp);
            }

            pdb = pdb_temp.release();
            env->mapDb[strFilename] = pdb;

            if (fCreate && !Exists(std::string("version"))) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                bool fTmp = fReadOnly;
                fReadOnly = false;
                WriteVersion(CLIENT_VERSION);
                fReadOnly = fTmp;
            }
<<<<<<< HEAD

            bitdb.mapDb[strFile] = pdb;
        }
    }
}

void CDB::Flush()
=======
        }
        ++env->mapFileUseCount[strFilename];
        strFile = strFilename;
    }
}

void BerkeleyBatch::Flush()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (activeTxn)
        return;

    // Flush database activity from memory pool to disk log
    unsigned int nMinutes = 0;
    if (fReadOnly)
        nMinutes = 1;

<<<<<<< HEAD
    bitdb.dbenv.txn_checkpoint(nMinutes ? GetArg("-dblogsize", 100) * 1024 : 0, nMinutes, 0);
}

void CDB::Close()
=======
    env->dbenv->txn_checkpoint(nMinutes ? gArgs.GetArg("-dblogsize", 100) * 1024 : 0, nMinutes, 0);
}

void BerkeleyDatabase::IncrementUpdateCounter()
{
    ++nUpdateCounter;
}

void BerkeleyBatch::Close()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (!pdb)
        return;
    if (activeTxn)
        activeTxn->abort();
    activeTxn = NULL;
    pdb = NULL;

<<<<<<< HEAD
    Flush();

    {
        LOCK(bitdb.cs_db);
        --bitdb.mapFileUseCount[strFile];
    }
}

void CDBEnv::CloseDb(const string& strFile)
=======
    if (fFlushOnClose)
        Flush();

    {
        LOCK(cs_db);
        --env->mapFileUseCount[strFile];
    }
}

void BerkeleyEnvironment::CloseDb(const std::string& strFile)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    {
        LOCK(cs_db);
        if (mapDb[strFile] != NULL) {
            // Close the database handle
            Db* pdb = mapDb[strFile];
            pdb->close(0);
            delete pdb;
            mapDb[strFile] = NULL;
        }
    }
}

<<<<<<< HEAD
bool CDBEnv::RemoveDb(const string& strFile)
{
    this->CloseDb(strFile);

    LOCK(cs_db);
    int rc = dbenv.dbremove(NULL, strFile.c_str(), NULL, DB_AUTO_COMMIT);
    return (rc == 0);
}

bool CDB::Rewrite(const string& strFile, const char* pszSkip)
{
    while (true) {
        {
            LOCK(bitdb.cs_db);
            if (!bitdb.mapFileUseCount.count(strFile) || bitdb.mapFileUseCount[strFile] == 0) {
                // Flush log data to the dat file
                bitdb.CloseDb(strFile);
                bitdb.CheckpointLSN(strFile);
                bitdb.mapFileUseCount.erase(strFile);

                bool fSuccess = true;
                LogPrintf("CDB::Rewrite : Rewriting %s...\n", strFile);
                string strFileRes = strFile + ".rewrite";
                { // surround usage of db with extra {}
                    CDB db(strFile.c_str(), "r");
                    Db* pdbCopy = new Db(&bitdb.dbenv, 0);
=======
bool BerkeleyBatch::Rewrite(BerkeleyDatabase& database, const char* pszSkip)
{
    if (database.IsDummy()) {
        return true;
    }
    BerkeleyEnvironment *env = database.env;
    const std::string& strFile = database.strFile;
    while (true) {
        {
            LOCK(cs_db);
            if (!env->mapFileUseCount.count(strFile) || env->mapFileUseCount[strFile] == 0) {
                // Flush log data to the dat file
                env->CloseDb(strFile);
                env->CheckpointLSN(strFile);
                env->mapFileUseCount.erase(strFile);

                bool fSuccess = true;
                LogPrintf("BerkeleyBatch::Rewrite : Rewriting %s...\n", strFile);
                std::string strFileRes = strFile + ".rewrite";
                { // surround usage of db with extra {}
                    BerkeleyBatch db(database, "r");
                    std::unique_ptr<Db> pdbCopy = std::make_unique<Db>(env->dbenv.get(), 0);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

                    int ret = pdbCopy->open(NULL, // Txn pointer
                        strFileRes.c_str(),       // Filename
                        "main",                   // Logical db name
                        DB_BTREE,                 // Database type
                        DB_CREATE,                // Flags
                        0);
                    if (ret > 0) {
<<<<<<< HEAD
                        LogPrintf("CDB::Rewrite : Can't create database file %s\n", strFileRes);
=======
                        LogPrintf("BerkeleyBatch::Rewrite : Can't create database file %s\n", strFileRes);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                        fSuccess = false;
                    }

                    Dbc* pcursor = db.GetCursor();
                    if (pcursor)
                        while (fSuccess) {
                            CDataStream ssKey(SER_DISK, CLIENT_VERSION);
                            CDataStream ssValue(SER_DISK, CLIENT_VERSION);
<<<<<<< HEAD
                            int ret = db.ReadAtCursor(pcursor, ssKey, ssValue, DB_NEXT);
                            if (ret == DB_NOTFOUND) {
                                pcursor->close();
                                break;
                            } else if (ret != 0) {
=======
                            int ret1 = db.ReadAtCursor(pcursor, ssKey, ssValue);
                            if (ret1 == DB_NOTFOUND) {
                                pcursor->close();
                                break;
                            } else if (ret1 != 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                                pcursor->close();
                                fSuccess = false;
                                break;
                            }
                            if (pszSkip &&
<<<<<<< HEAD
                                strncmp(&ssKey[0], pszSkip, std::min(ssKey.size(), strlen(pszSkip))) == 0)
                                continue;
                            if (strncmp(&ssKey[0], "\x07version", 8) == 0) {
=======
                                strncmp(ssKey.data(), pszSkip, std::min(ssKey.size(), strlen(pszSkip))) == 0)
                                continue;
                            if (strncmp(ssKey.data(), "\x07version", 8) == 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                                // Update version:
                                ssValue.clear();
                                ssValue << CLIENT_VERSION;
                            }
<<<<<<< HEAD
                            Dbt datKey(&ssKey[0], ssKey.size());
                            Dbt datValue(&ssValue[0], ssValue.size());
=======
                            Dbt datKey(ssKey.data(), ssKey.size());
                            Dbt datValue(ssValue.data(), ssValue.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                            int ret2 = pdbCopy->put(NULL, &datKey, &datValue, DB_NOOVERWRITE);
                            if (ret2 > 0)
                                fSuccess = false;
                        }
                    if (fSuccess) {
                        db.Close();
<<<<<<< HEAD
                        bitdb.CloseDb(strFile);
                        if (pdbCopy->close(0))
                            fSuccess = false;
                        delete pdbCopy;
                    }
                }
                if (fSuccess) {
                    Db dbA(&bitdb.dbenv, 0);
                    if (dbA.remove(strFile.c_str(), NULL, 0))
                        fSuccess = false;
                    Db dbB(&bitdb.dbenv, 0);
                    if (dbB.rename(strFileRes.c_str(), NULL, strFile.c_str(), 0))
                        fSuccess = false;
                }
                if (!fSuccess)
                    LogPrintf("CDB::Rewrite : Failed to rewrite database file %s\n", strFileRes);
=======
                        env->CloseDb(strFile);
                        if (pdbCopy->close(0))
                            fSuccess = false;
                    } else {
                        pdbCopy->close(0);
                    }
                }
                if (fSuccess) {
                    Db dbA(env->dbenv.get(), 0);
                    if (dbA.remove(strFile.c_str(), nullptr, 0))
                        fSuccess = false;
                    Db dbB(env->dbenv.get(), 0);
                    if (dbB.rename(strFileRes.c_str(), nullptr, strFile.c_str(), 0))
                        fSuccess = false;
                }
                if (!fSuccess)
                    LogPrintf("BerkeleyBatch::Rewrite : Failed to rewrite database file %s\n", strFileRes);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                return fSuccess;
            }
        }
        MilliSleep(100);
    }
<<<<<<< HEAD
    return false;
}


void CDBEnv::Flush(bool fShutdown)
{
    int64_t nStart = GetTimeMillis();
    // Flush log data to the actual data file on all files that are not in use
    LogPrint("db", "CDBEnv::Flush : Flush(%s)%s\n", fShutdown ? "true" : "false", fDbEnvInit ? "" : " database not started");
=======
}


void BerkeleyEnvironment::Flush(bool fShutdown)
{
    int64_t nStart = GetTimeMillis();
    // Flush log data to the actual data file on all files that are not in use
    LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush : Flush(%s)%s\n", fShutdown ? "true" : "false", fDbEnvInit ? "" : " database not started");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fDbEnvInit)
        return;
    {
        LOCK(cs_db);
<<<<<<< HEAD
        map<string, int>::iterator mi = mapFileUseCount.begin();
        while (mi != mapFileUseCount.end()) {
            string strFile = (*mi).first;
            int nRefCount = (*mi).second;
            LogPrint("db", "CDBEnv::Flush : Flushing %s (refcount = %d)...\n", strFile, nRefCount);
            if (nRefCount == 0) {
                // Move log data to the dat file
                CloseDb(strFile);
                LogPrint("db", "CDBEnv::Flush : %s checkpoint\n", strFile);
                dbenv.txn_checkpoint(0, 0, 0);
                LogPrint("db", "CDBEnv::Flush : %s detach\n", strFile);
                if (!fMockDb)
                    dbenv.lsn_reset(strFile.c_str(), 0);
                LogPrint("db", "CDBEnv::Flush : %s closed\n", strFile);
=======
        std::map<std::string, int>::iterator mi = mapFileUseCount.begin();
        while (mi != mapFileUseCount.end()) {
            std::string strFile = (*mi).first;
            int nRefCount = (*mi).second;
            LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush : Flushing %s (refcount = %d)...\n", strFile, nRefCount);
            if (nRefCount == 0) {
                // Move log data to the dat file
                CloseDb(strFile);
                LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush: %s checkpoint\n", strFile);
                dbenv->txn_checkpoint(0, 0, 0);
                LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush: %s detach\n", strFile);
                if (!fMockDb)
                    dbenv->lsn_reset(strFile.c_str(), 0);
                LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush: %s closed\n", strFile);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                mapFileUseCount.erase(mi++);
            } else
                mi++;
        }
<<<<<<< HEAD
        LogPrint("db", "CDBEnv::Flush : Flush(%s)%s took %15dms\n", fShutdown ? "true" : "false", fDbEnvInit ? "" : " database not started", GetTimeMillis() - nStart);
        if (fShutdown) {
            char** listp;
            if (mapFileUseCount.empty()) {
                dbenv.log_archive(&listp, DB_ARCH_REMOVE);
                Close();
                if (!fMockDb)
                    boost::filesystem::remove_all(boost::filesystem::path(strPath) / "database");
=======
        LogPrint(BCLog::DB, "BerkeleyEnvironment::Flush : Flush(%s)%s took %15dms\n", fShutdown ? "true" : "false", fDbEnvInit ? "" : " database not started", GetTimeMillis() - nStart);
        if (fShutdown) {
            char** listp;
            if (mapFileUseCount.empty()) {
                dbenv->log_archive(&listp, DB_ARCH_REMOVE);
                Close();
                if (!fMockDb) {
                    fs::remove_all(fs::path(strPath) / "database");
                }
                g_dbenvs.erase(strPath);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }
    }
}
<<<<<<< HEAD
=======

bool BerkeleyBatch::PeriodicFlush(BerkeleyDatabase& database)
{
    if (database.IsDummy()) {
        return true;
    }
    bool ret = false;
    BerkeleyEnvironment *env = database.env;
    const std::string& strFile = database.strFile;
    TRY_LOCK(cs_db, lockDb);
    if (lockDb)
    {
        // Don't do this if any databases are in use
        int nRefCount = 0;
        std::map<std::string, int>::iterator mi = env->mapFileUseCount.begin();
        while (mi != env->mapFileUseCount.end()) {
            nRefCount += (*mi).second;
            mi++;
        }

        if (nRefCount == 0) {
            boost::this_thread::interruption_point();
            std::map<std::string, int>::iterator _mi = env->mapFileUseCount.find(strFile);
            if (_mi != env->mapFileUseCount.end()) {
                LogPrint(BCLog::DB, "Flushing %s\n", strFile);
                int64_t nStart = GetTimeMillis();

                // Flush wallet file so it's self contained
                env->CloseDb(strFile);
                env->CheckpointLSN(strFile);

                env->mapFileUseCount.erase(_mi++);
                LogPrint(BCLog::DB, "Flushed %s %dms\n", strFile, GetTimeMillis() - nStart);
                ret = true;
            }
        }
    }

    return ret;
}

bool BerkeleyDatabase::Rewrite(const char* pszSkip)
{
    return BerkeleyBatch::Rewrite(*this, pszSkip);
}

bool BerkeleyDatabase::Backup(const std::string& strDest)
{
    if (IsDummy()) {
        return false;
    }
    while (true)
    {
        {
            LOCK(cs_db);
            if (!env->mapFileUseCount.count(strFile) || env->mapFileUseCount[strFile] == 0)
            {
                // Flush log data to the dat file
                env->CloseDb(strFile);
                env->CheckpointLSN(strFile);
                env->mapFileUseCount.erase(strFile);

                // Copy wallet file
                fs::path pathSrc = env->Directory() / strFile;
                fs::path pathDest(strDest);
                if (fs::is_directory(pathDest))
                    pathDest /= strFile;

                try {
                    if (fs::equivalent(pathSrc, pathDest)) {
                        LogPrintf("cannot backup to wallet source file %s\n", pathDest.string());
                        return false;
                    }

#if BOOST_VERSION >= 107400
                    fs::copy_file(pathSrc.c_str(), pathDest, fs::copy_options::overwrite_existing);
#elif BOOST_VERSION >= 105800 /* BOOST_LIB_VERSION 1_58 */
                    fs::copy_file(pathSrc.c_str(), pathDest, fs::copy_option::overwrite_if_exists);
#endif
                    LogPrintf("copied %s to %s\n", strFile, pathDest.string());
                    return true;
                } catch (const fs::filesystem_error& e) {
                    LogPrintf("error copying %s to %s - %s\n", strFile, pathDest.string(), fsbridge::get_filesystem_error_message(e));
                    return false;
                }
            }
        }
        MilliSleep(100);
    }
}

void BerkeleyDatabase::Flush(bool shutdown)
{
    if (!IsDummy()) {
        env->Flush(shutdown);
        if (shutdown) env = nullptr;
    }
}

void BerkeleyDatabase::CloseAndReset()
{
    env->Close();
    env->Reset();
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
