// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2009-2021 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2011-2013 The PPCoin developers
// Copyright (c) 2013-2014 The NovaCoin Developers
// Copyright (c) 2014-2018 The BlackCoin Developers
// Copyright (c) 2015-2021 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
<<<<<<< HEAD
#include "config/prcycoin-config.h"
=======
#include "config/pivx-config.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif

#include "init.h"

#include "activemasternode.h"
#include "addrman.h"
#include "amount.h"
<<<<<<< HEAD
#include "checkpoints.h"
#include "compat/sanity.h"
#include "httpserver.h"
#include "httprpc.h"
#include "key.h"
#include "main.h"
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "miner.h"
#include "net.h"
#include "rpc/server.h"
#include "script/standard.h"
#include "scheduler.h"
#include "txdb.h"
#include "torcontrol.h"
#include "guiinterface.h"
#include "util.h"
#include "utilmoneystr.h"
#include "util/threadnames.h"
#include "validationinterface.h"
#ifdef ENABLE_WALLET
#include "wallet/db.h"
#include "wallet/wallet.h"
#include "wallet/walletdb.h"

#endif

#include <fstream>
#include <stdint.h>
#include <stdio.h>

#ifndef WIN32
#include <signal.h>
#endif

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/thread.hpp>
#include <openssl/crypto.h>
=======
#include "bls/bls_wrapper.h"
#include "checkpoints.h"
#include "compat/sanity.h"
#include "consensus/upgrades.h"
#include "evo/deterministicmns.h"
#include "evo/evonotificationinterface.h"
#include "fs.h"
#include "httpserver.h"
#include "httprpc.h"
#include "invalid.h"
#include "llmq/quorums_init.h"
#include "key.h"
#include "mapport.h"
#include "masternodeconfig.h"
#include "miner.h"
#include "netbase.h"
#include "net_processing.h"
#include "policy/feerate.h"
#include "policy/policy.h"
#include "rpc/register.h"
#include "rpc/server.h"
#include "script/sigcache.h"
#include "script/standard.h"
#include "scheduler.h"
#include "shutdown.h"
#include "spork.h"
#include "sporkdb.h"
#include "evo/evodb.h"
#include "tiertwo/init.h"
#include "txdb.h"
#include "torcontrol.h"
#include "guiinterface.h"
#include "guiinterfaceutil.h"
#include "util/system.h"
#include "utilmoneystr.h"
#include "util/threadnames.h"
#include "validation.h"
#include "validationinterface.h"
#include "zpivchain.h"
#include "warnings.h"

#ifdef ENABLE_WALLET
#include "wallet/init.h"
#include "wallet/wallet.h"
#include "wallet/rpcwallet.h"
#endif

#include <atomic>
#include <fstream>
#include <stdint.h>
#include <stdio.h>
#include <memory>

#ifndef WIN32
#include <signal.h>
#include <sys/stat.h>
#endif

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread.hpp>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#if ENABLE_ZMQ
#include "zmq/zmqnotificationinterface.h"
#endif

<<<<<<< HEAD
using namespace boost;
using namespace std;

#ifdef ENABLE_WALLET
CWallet* pwalletMain = NULL;
int nWalletBackups = 10;
#endif
volatile bool fFeeEstimatesInitialized = false;
volatile bool fRestartRequested = false; // true: restart false: shutdown
=======

volatile bool fFeeEstimatesInitialized = false;
static const bool DEFAULT_PROXYRANDOMIZE = true;
static const bool DEFAULT_REST_ENABLE = false;
static const bool DEFAULT_DISABLE_SAFEMODE = false;
static const bool DEFAULT_STOPAFTERBLOCKIMPORT = false;
static const bool DEFAULT_MNCONFLOCK = true;

std::unique_ptr<CConnman> g_connman;
std::unique_ptr<PeerLogicValidation> peerLogic;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#if ENABLE_ZMQ
static CZMQNotificationInterface* pzmqNotificationInterface = NULL;
#endif

<<<<<<< HEAD
=======
static EvoNotificationInterface* pEvoNotificationInterface = nullptr;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#ifdef WIN32
// Win32 LevelDB doesn't use filedescriptors, and the ones used for
// accessing block files, don't count towards to fd_set size limit
// anyway.
#define MIN_CORE_FILEDESCRIPTORS 0
#else
#define MIN_CORE_FILEDESCRIPTORS 150
#endif

<<<<<<< HEAD
/** Used to pass flags to the Bind() function */
enum BindFlags {
    BF_NONE = 0,
    BF_EXPLICIT = (1U << 0),
    BF_REPORT_ERROR = (1U << 1),
    BF_WHITELIST = (1U << 2),
};

static const char* FEE_ESTIMATES_FILENAME = "fee_estimates.dat";
CClientUIInterface uiInterface;
=======
static const char* DEFAULT_ASMAP_FILENAME="ip_asn.map";

static const char* FEE_ESTIMATES_FILENAME = "fee_estimates.dat";
CClientUIInterface uiInterface;  // Declared but not defined in guiinterface.h
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

//////////////////////////////////////////////////////////////////////////////
//
// Shutdown
//

//
// Thread management and startup/shutdown:
//
// The network-processing threads are all part of a thread group
// created by AppInit() or the Qt main() function.
//
// A clean exit happens when StartShutdown() or the SIGTERM
<<<<<<< HEAD
// signal handler sets fRequestShutdown, which triggers
=======
// signal handler sets ShutdownRequested(), which triggers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// the DetectShutdownThread(), which interrupts the main thread group.
// DetectShutdownThread() then exits, which causes AppInit() to
// continue (it .joins the shutdown thread).
// Shutdown() is then
// called to clean up database connections, and stop other
// threads that should only be stopped after the main network-processing
// threads have exited.
//
<<<<<<< HEAD
// Note that if running -daemon the parent process returns from AppInit2
// before adding any threads to the threadGroup, so .join_all() returns
// immediately and the parent exits from main().
//
// Shutdown for Qt is very similar, only it uses a QTimer to detect
// fRequestShutdown getting set, and then does the normal Qt
// shutdown thing.
//

volatile bool fRequestShutdown = false;

void StartShutdown()
{
    fRequestShutdown = true;
}
bool ShutdownRequested()
{
    return fRequestShutdown || fRestartRequested;
}

=======
// Shutdown for Qt is very similar, only it uses a QTimer to detect
// ShutdownRequested() getting set, and then does the normal Qt
// shutdown thing.
//

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class CCoinsViewErrorCatcher : public CCoinsViewBacked
{
public:
    CCoinsViewErrorCatcher(CCoinsView* view) : CCoinsViewBacked(view) {}
<<<<<<< HEAD
    bool GetCoins(const uint256& txid, CCoins& coins) const
    {
        try {
            return CCoinsViewBacked::GetCoins(txid, coins);
=======
    bool GetCoin(const COutPoint& outpoint, Coin& coin) const override
    {
        try {
            return CCoinsViewBacked::GetCoin(outpoint, coin);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } catch (const std::runtime_error& e) {
            uiInterface.ThreadSafeMessageBox(_("Error reading from database, shutting down."), "", CClientUIInterface::MSG_ERROR);
            LogPrintf("Error reading from database: %s\n", e.what());
            // Starting the shutdown sequence and returning false to the caller would be
            // interpreted as 'entry not found' (as opposed to unable to read data), and
            // could lead to invalid interpration. Just exit immediately, as we can't
            // continue anyway, and all writes should be atomic.
            abort();
        }
    }
    // Writes do not need similar protection, as failure to write is handled by the caller.
};

<<<<<<< HEAD
static CCoinsViewDB* pcoinsdbview = NULL;
static CCoinsViewErrorCatcher* pcoinscatcher = NULL;
=======
static std::unique_ptr<CCoinsViewErrorCatcher> pcoinscatcher;
static std::unique_ptr<ECCVerifyHandle> globalVerifyHandle;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static boost::thread_group threadGroup;
static CScheduler scheduler;
void Interrupt()
{
    InterruptHTTPServer();
    InterruptHTTPRPC();
    InterruptRPC();
    InterruptREST();
    InterruptTorControl();
<<<<<<< HEAD
}

/** Preparing steps before shutting down or restarting the wallet */
void PrepareShutdown()
{
    fRequestShutdown = true;  // Needed when we shutdown the wallet
    fRestartRequested = true; // Needed when we restart the wallet
=======
    InterruptMapPort();
    if (g_connman)
        g_connman->Interrupt();
}

void Shutdown()
{
    StartShutdown();  // Needed when we shutdown the wallet
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LogPrintf("%s: In progress...\n", __func__);
    static RecursiveMutex cs_Shutdown;
    TRY_LOCK(cs_Shutdown, lockShutdown);
    if (!lockShutdown)
        return;

<<<<<<< HEAD
    /// Note: Shutdown() must be able to handle cases in which AppInit2() failed part of the way,
    /// for example if the data directory was found to be locked.
    /// Be sure that anything that writes files or flushes caches only does this if the respective
    /// module was initialized.
    util::ThreadRename("prcycoin-shutoff");
=======
    /// Note: Shutdown() must be able to handle cases in which initialization failed part of the way,
    /// for example if the data directory was found to be locked.
    /// Be sure that anything that writes files or flushes caches only does this if the respective
    /// module was initialized.
    util::ThreadRename("pivx-shutoff");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    mempool.AddTransactionsUpdated(1);
    StopHTTPRPC();
    StopREST();
    StopRPC();
    StopHTTPServer();
<<<<<<< HEAD
#ifdef ENABLE_WALLET
    if (pwalletMain)
        bitdb.Flush(false);
    GeneratePrcycoins(false, NULL, 0);
#endif
    StopNode();

    // After everything has been shut down, but before things get flushed, stop the
    // CScheduler/checkqueue threadGroup
    threadGroup.interrupt_all();
    threadGroup.join_all();

    DumpMasternodes();
    DumpBudgets();
    DumpMasternodePayments();
    UnregisterNodeSignals(GetNodeSignals());

    if (fFeeEstimatesInitialized) {
        boost::filesystem::path est_path = GetDataDir() / FEE_ESTIMATES_FILENAME;
        CAutoFile est_fileout(fopen(est_path.string().c_str(), "wb"), SER_DISK, CLIENT_VERSION);
=======
    llmq::StopLLMQSystem();
#ifdef ENABLE_WALLET
    for (CWalletRef pwallet : vpwallets) {
        pwallet->Flush(false);
    }
    GenerateBitcoins(false, NULL, 0);
#endif
    StopMapPort();

    // Because these depend on each-other, we make sure that neither can be
    // using the other before destroying them.
    if (peerLogic) UnregisterValidationInterface(peerLogic.get());
    if (g_connman) g_connman->Stop();

    StopTorControl();

    // After everything has been shut down, but before things get flushed, stop the
    // CScheduler/checkqueue threadGroup
    scheduler.stop();
    threadGroup.interrupt_all();
    threadGroup.join_all();

    // After the threads that potentially access these pointers have been stopped,
    // destruct and reset all to nullptr.
    g_connman.reset();
    peerLogic.reset();

    DumpTierTwo();
    if (::mempool.IsLoaded() && gArgs.GetBoolArg("-persistmempool", DEFAULT_PERSIST_MEMPOOL)) {
        DumpMempool(::mempool);
    }

    if (fFeeEstimatesInitialized) {
        fs::path est_path = GetDataDir() / FEE_ESTIMATES_FILENAME;
        CAutoFile est_fileout(fsbridge::fopen(est_path, "wb"), SER_DISK, CLIENT_VERSION);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!est_fileout.IsNull())
            mempool.WriteFeeEstimates(est_fileout);
        else
            LogPrintf("%s: Failed to write fee estimates to %s\n", __func__, est_path.string());
        fFeeEstimatesInitialized = false;
    }

<<<<<<< HEAD
=======
    // FlushStateToDisk generates a SetBestChain callback, which we should avoid missing
    if (pcoinsTip != nullptr) {
        FlushStateToDisk();
    }

    // After there are no more peers/RPC left to give us new data which may generate
    // CValidationInterface callbacks, flush them...
    GetMainSignals().FlushBackgroundCallbacks();

    // Any future callbacks will be dropped. This should absolutely be safe - if
    // missing a callback results in an unrecoverable situation, unclean shutdown
    // would too. The only reason to do the above flushes is to let the wallet catch
    // up with our current chain to avoid any strange pruning edge cases and make
    // next startup faster by avoiding rescan.

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_main);
        if (pcoinsTip != NULL) {
            FlushStateToDisk();

            //record that client took the proper shutdown procedure
            pblocktree->WriteFlag("shutdown", true);
        }
<<<<<<< HEAD
        delete pcoinsTip;
        pcoinsTip = NULL;
        delete pcoinscatcher;
        pcoinscatcher = NULL;
        delete pcoinsdbview;
        pcoinsdbview = NULL;
        delete pblocktree;
        pblocktree = NULL;
    }
#ifdef ENABLE_WALLET
    if (pwalletMain)
        bitdb.Flush(true);
#endif

=======
        pcoinsTip.reset();
        pcoinscatcher.reset();
        pcoinsdbview.reset();
        pblocktree.reset();
        zerocoinDB.reset();
        accumulatorCache.reset();
        pSporkDB.reset();
        llmq::DestroyLLMQSystem();
        deterministicMNManager.reset();
        evoDb.reset();
    }
#ifdef ENABLE_WALLET
    for (CWalletRef pwallet : vpwallets) {
        pwallet->Flush(true);
    }
#endif

    if (pEvoNotificationInterface) {
        UnregisterValidationInterface(pEvoNotificationInterface);
        delete pEvoNotificationInterface;
        pEvoNotificationInterface = nullptr;
    }

    if (activeMasternodeManager) {
        UnregisterValidationInterface(activeMasternodeManager);
        delete activeMasternodeManager;
        activeMasternodeManager = nullptr;
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#if ENABLE_ZMQ
    if (pzmqNotificationInterface) {
        UnregisterValidationInterface(pzmqNotificationInterface);
        delete pzmqNotificationInterface;
<<<<<<< HEAD
        pzmqNotificationInterface = NULL;
=======
        pzmqNotificationInterface = nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
#endif

    // Disconnect all slots
    UnregisterAllValidationInterfaces();
<<<<<<< HEAD

#ifndef WIN32
    try {
        boost::filesystem::remove(GetPidFile());
    } catch (const boost::filesystem::filesystem_error& e) {
        LogPrintf("%s: Unable to remove pidfile: %s\n", __func__, e.what());
    }
#endif
}

/**
* Shutdown is split into 2 parts:
* Part 1: shut down everything but the main wallet instance (done in PrepareShutdown() )
* Part 2: delete wallet instance
*
* In case of a restart PrepareShutdown() was already called before, but this method here gets
* called implicitly when the parent object is deleted. In this case we have to skip the
* PrepareShutdown() part because it was already executed and just delete the wallet instance.
*/
void Shutdown()
{
    // Shutdown part 1: prepare shutdown
    if (!fRestartRequested) {
        PrepareShutdown();
    }

// Shutdown part 2: Stop TOR thread and delete wallet instance
    StopTorControl();
#ifdef ENABLE_WALLET
    delete pwalletMain;
    pwalletMain = NULL;
#endif
    DestroyContext();
=======
    GetMainSignals().UnregisterBackgroundSignalScheduler();

#ifndef WIN32
    try {
        if (!fs::remove(GetPidFile())) {
            LogPrintf("%s: Unable to remove PID file: File does not exist\n", __func__);
        }
    } catch (const fs::filesystem_error& e) {
        LogPrintf("%s: Unable to remove pidfile: %s\n", __func__, e.what());
    }
#endif

#ifdef ENABLE_WALLET
    for (CWalletRef pwallet : vpwallets) {
        delete pwallet;
    }
    vpwallets.clear();
#endif
    globalVerifyHandle.reset();
    ECC_Stop();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LogPrintf("%s: done\n", __func__);
}

/**
 * Signal handlers are very limited in what they are allowed to do, so:
 */
void HandleSIGTERM(int)
{
    StartShutdown();
}

void HandleSIGHUP(int)
{
<<<<<<< HEAD
    fReopenDebugLog = true;
=======
    g_logger->m_reopen_file = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

#ifndef WIN32
static void registerSignalHandler(int signal, void(*handler)(int))
{
<<<<<<< HEAD
    struct sigaction sa;
=======
    struct sigaction sa{};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(signal, &sa, nullptr);
}
#endif

<<<<<<< HEAD
bool static InitError(const std::string& str)
{
    uiInterface.ThreadSafeMessageBox(str, "", CClientUIInterface::MSG_ERROR);
    return false;
}

bool static InitWarning(const std::string& str)
{
    uiInterface.ThreadSafeMessageBox(str, "", CClientUIInterface::MSG_WARNING);
    return true;
}

bool static Bind(const CService& addr, unsigned int flags)
{
    if (!(flags & BF_EXPLICIT) && IsLimited(addr))
        return false;
    std::string strError;
    if (!BindListenPort(addr, strError, (flags & BF_WHITELIST) != 0)) {
        if (flags & BF_REPORT_ERROR)
            return InitError(strError);
        return false;
    }
    return true;
}

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void OnRPCStarted()
{
    uiInterface.NotifyBlockTip.connect(RPCNotifyBlockChange);
}

void OnRPCStopped()
{
    uiInterface.NotifyBlockTip.disconnect(RPCNotifyBlockChange);
    //RPCNotifyBlockChange(0);
    g_best_block_cv.notify_all();
<<<<<<< HEAD
    LogPrint("rpc", "RPC stopped.\n");
=======
    LogPrint(BCLog::RPC, "RPC stopped.\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void OnRPCPreCommand(const CRPCCommand& cmd)
{
<<<<<<< HEAD
#ifdef ENABLE_WALLET
    if (cmd.reqWallet && !pwalletMain)
        throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found (disabled)");
#endif
    // Observe safe mode
    string strWarning = GetWarnings("rpc");
    if (strWarning != "" && !GetBoolArg("-disablesafemode", false) &&
        !cmd.okSafeMode)
        throw JSONRPCError(RPC_FORBIDDEN_BY_SAFE_MODE, string("Safe mode: ") + strWarning);
=======
    // Observe safe mode
    std::string strWarning = GetWarnings("rpc");
    if (!strWarning.empty() && !gArgs.GetBoolArg("-disablesafemode", DEFAULT_DISABLE_SAFEMODE) &&
        !cmd.okSafeMode)
        throw JSONRPCError(RPC_FORBIDDEN_BY_SAFE_MODE, std::string("Safe mode: ") + strWarning);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

std::string HelpMessage(HelpMessageMode mode)
{
<<<<<<< HEAD

    // When adding new options to the categories, please keep and ensure alphabetical ordering.
    string strUsage = HelpMessageGroup(_("Options:"));
    strUsage += HelpMessageOpt("-?", _("This help message"));
    strUsage += HelpMessageOpt("-version", _("Print version and exit"));
    strUsage += HelpMessageOpt("-alertnotify=<cmd>", _("Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)"));
    strUsage += HelpMessageOpt("-blocknotify=<cmd>", _("Execute command when the best block changes (%s in cmd is replaced by block hash)"));
    strUsage += HelpMessageOpt("-blocksizenotify=<cmd>", _("Execute command when the best block changes and its size is over (%s in cmd is replaced by block hash, %d with the block size)"));
    strUsage += HelpMessageOpt("-checkblocks=<n>", strprintf(_("How many blocks to check at startup (default: %u, 0 = all)"), 500));
    strUsage += HelpMessageOpt("-conf=<file>", strprintf(_("Specify configuration file (default: %s)"), "prcycoin.conf"));
    if (mode == HMM_BITCOIND) {
#if !defined(WIN32)
        strUsage += HelpMessageOpt("-daemon", _("Run in the background as a daemon and accept commands"));
#endif
    }
    strUsage += HelpMessageOpt("-datadir=<dir>", _("Specify data directory"));
    strUsage += HelpMessageOpt("-backuppath=<(dir/file)>", _("Specify custom backup path to add a copy of any wallet backup. If set as dir, every backup generates a timestamped file. If set as file, will rewrite to that file every backup."));
    strUsage += HelpMessageOpt("-dbcache=<n>", strprintf(_("Set database cache size in megabytes (%d to %d, default: %d)"), nMinDbCache, nMaxDbCache, nDefaultDbCache));
    strUsage += HelpMessageOpt("-loadblock=<file>", _("Imports blocks from external blk000??.dat file") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-maxreorg=<n>", strprintf(_("Set the Maximum reorg depth (default: %u)"), Params(CBaseChainParams::MAIN).MaxReorganizationDepth()));
    strUsage += HelpMessageOpt("-maxorphantx=<n>", strprintf(_("Keep at most <n> unconnectable transactions in memory (default: %u)"), DEFAULT_MAX_ORPHAN_TRANSACTIONS));
    strUsage += HelpMessageOpt("-par=<n>", strprintf(_("Set the number of script verification threads (%u to %d, 0 = auto, <0 = leave that many cores free, default: %d)"), -(int)boost::thread::hardware_concurrency(), MAX_SCRIPTCHECK_THREADS, DEFAULT_SCRIPTCHECK_THREADS));
#ifndef WIN32
    strUsage += HelpMessageOpt("-pid=<file>", strprintf(_("Specify pid file (default: %s)"), "prcycoind.pid"));
#endif
    strUsage += HelpMessageOpt("-reindex", _("Rebuild block chain index from current blk000??.dat files") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-reindexmoneysupply", _("Reindex the PRCY money supply statistics") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-resync", _("Delete blockchain folders and resync from scratch") + " " + _("on startup"));
#if !defined(WIN32)
    strUsage += HelpMessageOpt("-sysperms", _("Create new files with system default permissions, instead of umask 077 (only effective with disabled wallet functionality)"));
#endif
    strUsage += HelpMessageOpt("-txindex", strprintf(_("Maintain a full transaction index, used by the getrawtransaction rpc call (default: %u)"), 0));
    strUsage += HelpMessageOpt("-forcestart", _("Attempt to force blockchain corruption recovery") + " " + _("on startup"));

    strUsage += HelpMessageGroup(_("Connection options:"));
    strUsage += HelpMessageOpt("-addnode=<ip>", _("Add a node to connect to and attempt to keep the connection open"));
    strUsage += HelpMessageOpt("-banscore=<n>", strprintf(_("Threshold for disconnecting misbehaving peers (default: %u)"), 100));
    strUsage += HelpMessageOpt("-bantime=<n>", strprintf(_("Number of seconds to keep misbehaving peers from reconnecting (default: %u)"), 86400));
    strUsage += HelpMessageOpt("-bind=<addr>", _("Bind to given address and always listen on it. Use [host]:port notation for IPv6"));
    strUsage += HelpMessageOpt("-connect=<ip>", _("Connect only to the specified node(s); -noconnect or -connect=0 alone to disable automatic connections"));
    strUsage += HelpMessageOpt("-discover", _("Discover own IP address (default: 1 when listening and no -externalip)"));
    strUsage += HelpMessageOpt("-dns", _("Allow DNS lookups for -addnode, -seednode and -connect") + " " + _("(default: 1)"));
    strUsage += HelpMessageOpt("-dnsseed", _("Query for peer addresses via DNS lookup, if low on addresses (default: 1 unless -connect/-noconnect)"));
    strUsage += HelpMessageOpt("-externalip=<ip>", _("Specify your own public address"));
    strUsage += HelpMessageOpt("-forcednsseed", strprintf(_("Always query for peer addresses via DNS lookup (default: %u)"), 0));
    strUsage += HelpMessageOpt("-listen", _("Accept connections from outside (default: 1 if no -proxy or -connect/-noconnect)"));
    strUsage += HelpMessageOpt("-listenonion", strprintf(_("Automatically create Tor hidden service (default: %d)"), DEFAULT_LISTEN_ONION));
    strUsage += HelpMessageOpt("-maxconnections=<n>", strprintf(_("Maintain at most <n> connections to peers (default: %u)"), DEFAULT_MAX_PEER_CONNECTIONS));
    strUsage += HelpMessageOpt("-maxreceivebuffer=<n>", strprintf(_("Maximum per-connection receive buffer, <n>*1000 bytes (default: %u)"), 5000));
    strUsage += HelpMessageOpt("-maxsendbuffer=<n>", strprintf(_("Maximum per-connection send buffer, <n>*1000 bytes (default: %u)"), 1000));
    strUsage += HelpMessageOpt("-onion=<ip:port>", strprintf(_("Use separate SOCKS5 proxy to reach peers via Tor hidden services (default: %s)"), "-proxy"));
    strUsage += HelpMessageOpt("-onlynet=<net>", _("Only connect to nodes in network <net> (ipv4, ipv6 or onion)"));
    strUsage += HelpMessageOpt("-permitbaremultisig", strprintf(_("Relay non-P2SH multisig (default: %u)"), 1));
    strUsage += HelpMessageOpt("-peerbloomfilters", strprintf(_("Support filtering of blocks and transaction with bloom filters (default: %u)"), DEFAULT_PEERBLOOMFILTERS));
    strUsage += HelpMessageOpt("-port=<port>", strprintf(_("Listen for connections on <port> (default: %u or testnet: %u)"), 59682, 59684));
    strUsage += HelpMessageOpt("-proxy=<ip:port>", _("Connect through SOCKS5 proxy"));
    strUsage += HelpMessageOpt("-proxyrandomize", strprintf(_("Randomize credentials for every proxy connection. This enables Tor stream isolation (default: %u)"), 1));
    strUsage += HelpMessageOpt("-seednode=<ip>", _("Connect to a node to retrieve peer addresses, and disconnect"));
    strUsage += HelpMessageOpt("-timeout=<n>", strprintf(_("Specify connection timeout in milliseconds (minimum: 1, default: %d)"), DEFAULT_CONNECT_TIMEOUT));
    strUsage += HelpMessageOpt("-torcontrol=<ip>:<port>", strprintf(_("Tor control port to use if onion listening enabled (default: %s)"), DEFAULT_TOR_CONTROL));
    strUsage += HelpMessageOpt("-torpassword=<pass>", _("Tor control port password (default: empty)"));
#ifdef USE_UPNP
#if USE_UPNP
    strUsage += HelpMessageOpt("-upnp", _("Use UPnP to map the listening port (default: 1 when listening)"));
#else
    strUsage += HelpMessageOpt("-upnp", strprintf(_("Use UPnP to map the listening port (default: %u)"), 0));
#endif
#endif
    strUsage += HelpMessageOpt("-whitebind=<addr>", _("Bind to given address and whitelist peers connecting to it. Use [host]:port notation for IPv6"));
    strUsage += HelpMessageOpt("-whitelist=<netmask>", _("Whitelist peers connecting from the given netmask or IP address. Can be specified multiple times.") +
        " " + _("Whitelisted peers cannot be DoS banned and their transactions are always relayed, even if they are already in the mempool, useful e.g. for a gateway"));


#ifdef ENABLE_WALLET
    strUsage += HelpMessageGroup(_("Wallet options:"));
    strUsage += HelpMessageOpt("-createwalletbackups=<n>", _("Number of automatic wallet backups (default: 10)"));
    strUsage += HelpMessageOpt("-disablewallet", _("Do not load the wallet and disable wallet RPC calls"));
    strUsage += HelpMessageOpt("-keypool=<n>", strprintf(_("Set key pool size to <n> (default: %u)"), 100));
    if (GetBoolArg("-help-debug", false))
        strUsage += HelpMessageOpt("-mintxfee=<amt>", strprintf(_("Fees (in PRCY/Kb) smaller than this are considered zero fee for transaction creation (default: %s)"),
            FormatMoney(CWallet::minTxFee.GetFeePerK())));
    strUsage += HelpMessageOpt("-paytxfee=<amt>", strprintf(_("Fee (in PRCY/kB) to add to transactions you send (default: %s)"), FormatMoney(payTxFee.GetFeePerK())));
    strUsage += HelpMessageOpt("-rescan", _("Rescan the block chain for missing wallet transactions") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-salvagewallet", _("Attempt to recover private keys from a corrupt wallet.dat") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-sendfreetransactions", strprintf(_("Send transactions as zero-fee transactions if possible (default: %u)"), 0));
    strUsage += HelpMessageOpt("-spendzeroconfchange", strprintf(_("Spend unconfirmed change when sending transactions (default: %u)"), 1));
    strUsage += HelpMessageOpt("-disablesystemnotifications", strprintf(_("Disable OS notifications for incoming transactions (default: %u)"), 0));
    strUsage += HelpMessageOpt("-txconfirmtarget=<n>", strprintf(_("If paytxfee is not set, include enough fee so transactions begin confirmation on average within n blocks (default: %u)"), 1));
    strUsage += HelpMessageOpt("-maxtxfee=<amt>", strprintf(_("Maximum total fees to use in a single wallet transaction, setting too low may abort large transactions (default: %s)"),
        FormatMoney(maxTxFee)));
    strUsage += HelpMessageOpt("-upgradewallet", _("Upgrade wallet to latest format") + " " + _("on startup"));
    strUsage += HelpMessageOpt("-wallet=<file>", _("Specify wallet file (within data directory)") + " " + strprintf(_("(default: %s)"), "wallet.dat"));
    strUsage += HelpMessageOpt("-walletnotify=<cmd>", _("Execute command when a wallet transaction changes (%s in cmd is replaced by TxID)"));
    if (mode == HMM_BITCOIN_QT)
        strUsage += HelpMessageOpt("-windowtitle=<name>", _("Wallet window title"));
    strUsage += HelpMessageOpt("-zapwallettxes=<mode>", _("Delete all wallet transactions and only recover those parts of the blockchain through -rescan on startup") +
        " " + _("(1 = keep tx meta data e.g. account owner and payment request information, 2 = drop tx meta data)"));
#endif

#if ENABLE_ZMQ
    strUsage += HelpMessageGroup(_("ZeroMQ notification options:"));
    strUsage += HelpMessageOpt("-zmqpubhashblock=<address>", _("Enable publish hash block in <address>"));
    strUsage += HelpMessageOpt("-zmqpubhashtx=<address>", _("Enable publish hash transaction in <address>"));
    strUsage += HelpMessageOpt("-zmqpubhashtxlock=<address>", _("Enable publish hash transaction (locked via SwiftX) in <address>"));
    strUsage += HelpMessageOpt("-zmqpubrawblock=<address>", _("Enable publish raw block in <address>"));
    strUsage += HelpMessageOpt("-zmqpubrawtx=<address>", _("Enable publish raw transaction in <address>"));
    strUsage += HelpMessageOpt("-zmqpubrawtxlock=<address>", _("Enable publish raw transaction (locked via SwiftX) in <address>"));
#endif

    strUsage += HelpMessageGroup(_("Debugging/Testing options:"));
    strUsage += HelpMessageOpt("-uacomment=<cmt>", _("Append comment to the user agent string"));
    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-checkblockindex", strprintf("Do a full consistency check for mapBlockIndex, setBlockIndexCandidates, chainActive and mapBlocksUnlinked occasionally. Also sets -checkmempool (default: %u)", Params(CBaseChainParams::MAIN).DefaultConsistencyChecks()));
        strUsage += HelpMessageOpt("-checkmempool=<n>", strprintf("Run checks every <n> transactions (default: %u)", Params(CBaseChainParams::MAIN).DefaultConsistencyChecks()));
        strUsage += HelpMessageOpt("-checkpoints", strprintf(_("Only accept block chain matching built-in checkpoints (default: %u)"), 1));
        strUsage += HelpMessageOpt("-dblogsize=<n>", strprintf(_("Flush database activity from memory pool to disk log every <n> megabytes (default: %u)"), 100));
        strUsage += HelpMessageOpt("-disablesafemode", strprintf(_("Disable safemode, override a real safe mode event (default: %u)"), 0));
        strUsage += HelpMessageOpt("-testsafemode", strprintf(_("Force safe mode (default: %u)"), 0));
        strUsage += HelpMessageOpt("-dropmessagestest=<n>", _("Randomly drop 1 of every <n> network messages"));
        strUsage += HelpMessageOpt("-fuzzmessagestest=<n>", _("Randomly fuzz 1 of every <n> network messages"));
        strUsage += HelpMessageOpt("-flushwallet", strprintf(_("Run a thread to flush wallet periodically (default: %u)"), 1));
        strUsage += HelpMessageOpt("-maxreorg", strprintf(_("Use a custom max chain reorganization depth (default: %u)"), 100));
        strUsage += HelpMessageOpt("-stopafterblockimport", strprintf(_("Stop running after importing blocks from disk (default: %u)"), 0));
    }
    string debugCategories = "addrman, alert, bench, coindb, db, lock, rand, rpc, selectcoins, tor, mempool, net, proxy, http, libevent, prcycoin, (obfuscation, swiftx, masternode, mnpayments, mnbudget, zero)"; // Don't translate these and qt below
    if (mode == HMM_BITCOIN_QT)
        debugCategories += ", qt";
    strUsage += HelpMessageOpt("-debug=<category>", strprintf(_("Output debugging information (default: %u, supplying <category> is optional)"), 0) + ". " +
        _("If <category> is not supplied, output all debugging information.") + _("<category> can be:") + " " + debugCategories + ".");
    if (GetBoolArg("-help-debug", false))
        strUsage += HelpMessageOpt("-nodebug", "Turn off debugging messages, same as -debug=0");
#ifdef ENABLE_WALLET
    strUsage += HelpMessageOpt("-gen", strprintf(_("Generate coins (default: %u)"), 0));
    strUsage += HelpMessageOpt("-genproclimit=<n>", strprintf(_("Set the number of threads for coin generation if enabled (-1 = all cores, default: %d)"), 1));
#endif
    strUsage += HelpMessageOpt("-help-debug", _("Show all debugging options (usage: --help -help-debug)"));
    strUsage += HelpMessageOpt("-logips", strprintf(_("Include IP addresses in debug output (default: %u)"), 0));
    strUsage += HelpMessageOpt("-logtimestamps", strprintf(_("Prepend debug output with timestamp (default: %u)"), 1));
    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-limitfreerelay=<n>", strprintf(_("Continuously rate-limit free transactions to <n>*1000 bytes per minute (default:%u)"), 15));
        strUsage += HelpMessageOpt("-relaypriority", strprintf(_("Require high priority for relaying free or low-fee transactions (default:%u)"), 1));
        strUsage += HelpMessageOpt("-maxsigcachesize=<n>", strprintf(_("Limit size of signature cache to <n> entries (default: %u)"), 50000));
    }
    strUsage += HelpMessageOpt("-maxtipage=<n>", strprintf("Maximum tip age in seconds to consider node in initial block download (default: %u)", DEFAULT_MAX_TIP_AGE));
    strUsage += HelpMessageOpt("-minrelaytxfee=<amt>", strprintf(_("Fees (in PRCY/Kb) smaller than this are considered zero fee for relaying (default: %s)"), FormatMoney(::minRelayTxFee.GetFeePerK())));
    strUsage += HelpMessageOpt("-printtoconsole", strprintf(_("Send trace/debug info to console instead of debug.log file (default: %u)"), 0));
    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-printpriority", strprintf(_("Log transaction priority and fee per kB when mining blocks (default: %u)"), 0));
        strUsage += HelpMessageOpt("-privdb", strprintf(_("Sets the DB_PRIVATE flag in the wallet db environment (default: %u)"), 1));
        strUsage += HelpMessageOpt("-regtest", _("Enter regression test mode, which uses a special chain in which blocks can be solved instantly.") + " " +
            _("This is intended for regression testing tools and app development.") + " " +
            _("In this mode -genproclimit controls how many blocks are generated immediately."));
    }
    strUsage += HelpMessageOpt("-shrinkdebugfile", _("Shrink debug.log file on client startup (default: 1 when no -debug)"));
    strUsage += HelpMessageOpt("-testnet", _("Use the test network"));

#ifdef ENABLE_WALLET
    strUsage += HelpMessageGroup(_("Staking options:"));
    strUsage += HelpMessageOpt("-staking=<n>", strprintf(_("Enable staking functionality (0-1, default: %u)"), 1));
    strUsage += HelpMessageOpt("-reservebalance=<amt>", _("Keep the specified amount available for spending at all times (default: 0)"));
    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-printstakemodifier", _("Display the stake modifier calculations in the debug.log file."));
        strUsage += HelpMessageOpt("-printcoinstake", _("Display verbose coin stake messages in the debug.log file."));
    }
#endif

    strUsage += HelpMessageGroup(_("Masternode options:"));
    strUsage += HelpMessageOpt("-masternode=<n>", strprintf(_("Enable the client to act as a masternode (0-1, default: %u)"), 0));
    strUsage += HelpMessageOpt("-mnconf=<file>", strprintf(_("Specify masternode configuration file (default: %s)"), "masternode.conf"));
    strUsage += HelpMessageOpt("-mnconflock=<n>", strprintf(_("Lock masternodes from masternode configuration file (default: %u)"), 1));
    strUsage += HelpMessageOpt("-masternodeprivkey=<n>", _("Set the masternode private key"));
    strUsage += HelpMessageOpt("-masternodeaddr=<n>", strprintf(_("Set external address:port to get to this masternode (example: %s)"), "128.127.106.235:59682"));
    strUsage += HelpMessageOpt("-budgetvotemode=<mode>", _("Change automatic finalized budget voting behavior. mode=auto: Vote for only exact finalized budget match to my generated budget. (string, default: auto)"));

    strUsage += HelpMessageGroup(_("SwiftX options:"));
    strUsage += HelpMessageOpt("-enableswifttx=<n>", strprintf(_("Enable SwiftX, show confirmations for locked transactions (bool, default: %s)"), "true"));
    strUsage += HelpMessageOpt("-swifttxdepth=<n>", strprintf(_("Show N confirmations for a successfully locked transaction (0-9999, default: %u)"), nSwiftTXDepth));

    strUsage += HelpMessageGroup(_("Node relay options:"));
    strUsage += HelpMessageOpt("-datacarrier", strprintf(_("Relay and mine data carrier transactions (default: %u)"), 1));
    strUsage += HelpMessageOpt("-datacarriersize", strprintf(_("Maximum size of data in data carrier transactions we relay and mine (default: %u)"), MAX_OP_RETURN_RELAY));
    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-blockversion=<n>", "Override block version to test forking scenarios");
    }

    strUsage += HelpMessageGroup(_("Block creation options:"));
    strUsage += HelpMessageOpt("-blockminsize=<n>", strprintf(_("Set minimum block size in bytes (default: %u)"), 0));
    strUsage += HelpMessageOpt("-blockmaxsize=<n>", strprintf(_("Set maximum block size in bytes (default: %d)"), DEFAULT_BLOCK_MAX_SIZE));
    strUsage += HelpMessageOpt("-blockprioritysize=<n>", strprintf(_("Set maximum size of high-priority/low-fee transactions in bytes (default: %d)"), DEFAULT_BLOCK_PRIORITY_SIZE));

    strUsage += HelpMessageGroup(_("RPC server options:"));
    strUsage += HelpMessageOpt("-server", _("Accept command line and JSON-RPC commands"));
    strUsage += HelpMessageOpt("-rest", strprintf(_("Accept public REST requests (default: %u)"), 0));
    strUsage += HelpMessageOpt("-rpcbind=<addr>", _("Bind to given address to listen for JSON-RPC connections. Use [host]:port notation for IPv6. This option can be specified multiple times (default: bind to all interfaces)"));
    strUsage += HelpMessageOpt("-rpcuser=<user>", _("Username for JSON-RPC connections"));
    strUsage += HelpMessageOpt("-rpcpassword=<pw>", _("Password for JSON-RPC connections"));
    strUsage += HelpMessageOpt("-rpcport=<port>", strprintf(_("Listen for JSON-RPC connections on <port> (default: %u or testnet: %u)"), 59683, 59685));
    strUsage += HelpMessageOpt("-rpcallowip=<ip>", _("Allow JSON-RPC connections from specified source. Valid for <ip> are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times"));
    strUsage += HelpMessageOpt("-rpcthreads=<n>", strprintf(_("Set the number of threads to service RPC calls (default: %d)"), DEFAULT_HTTP_THREADS));

    if (GetBoolArg("-help-debug", false)) {
        strUsage += HelpMessageOpt("-rpcworkqueue=<n>", strprintf("Set the depth of the work queue to service RPC calls (default: %d)", DEFAULT_HTTP_WORKQUEUE));
        strUsage += HelpMessageOpt("-rpcservertimeout=<n>", strprintf("Timeout during HTTP requests (default: %d)",  DEFAULT_HTTP_SERVER_TIMEOUT));
    }

=======
    const auto defaultBaseParams = CreateBaseChainParams(CBaseChainParams::MAIN);
    const auto testnetBaseParams = CreateBaseChainParams(CBaseChainParams::TESTNET);
    const auto defaultChainParams = CreateChainParams(CBaseChainParams::MAIN);
    const auto testnetChainParams = CreateChainParams(CBaseChainParams::TESTNET);
    const bool showDebug = gArgs.GetBoolArg("-help-debug", false);

    // When adding new options to the categories, please keep and ensure alphabetical ordering.
    std::string strUsage = HelpMessageGroup("Options:");
    strUsage += HelpMessageOpt("-?", "This help message");
    strUsage += HelpMessageOpt("-version", "Print version and exit");
    strUsage += HelpMessageOpt("-alertnotify=<cmd>", "Execute command when a relevant alert is received or we see a really long fork (%s in cmd is replaced by message)");
    strUsage += HelpMessageOpt("-blocksdir=<dir>", "Specify directory to hold blocks subdirectory for *.dat files (default: <datadir>)");
    strUsage += HelpMessageOpt("-blocknotify=<cmd>", "Execute command when the best block changes (%s in cmd is replaced by block hash)");
    strUsage += HelpMessageOpt("-checkblocks=<n>", strprintf("How many blocks to check at startup (default: %u, 0 = all)", DEFAULT_CHECKBLOCKS));
    strUsage += HelpMessageOpt("-checklevel=<n>", strprintf("How thorough the block verification of -checkblocks is (0-4, default: %u)", DEFAULT_CHECKLEVEL));

    strUsage += HelpMessageOpt("-conf=<file>", strprintf("Specify configuration file (default: %s)", PIVX_CONF_FILENAME));
    if (mode == HMM_BITCOIND) {
#if !defined(WIN32)
        strUsage += HelpMessageOpt("-daemon", "Run in the background as a daemon and accept commands");
#endif
    }
    strUsage += HelpMessageOpt("-datadir=<dir>", "Specify data directory");
    if (showDebug) {
        strUsage += HelpMessageOpt("-dbbatchsize", strprintf("Maximum database write batch size in bytes (default: %u)", nDefaultDbBatchSize));
    }
    strUsage += HelpMessageOpt("-paramsdir=<dir>", strprintf("Specify zk params directory (default: %s)", ZC_GetParamsDir().string()));
    strUsage += HelpMessageOpt("-debuglogfile=<file>", strprintf("Specify location of debug log file: this can be an absolute path or a path relative to the data directory (default: %s)", DEFAULT_DEBUGLOGFILE));
    strUsage += HelpMessageOpt("-disablesystemnotifications", strprintf("Disable OS notifications for incoming transactions (default: %u)", 0));
    strUsage += HelpMessageOpt("-dbcache=<n>", strprintf("Set database cache size in megabytes (%d to %d, default: %d)", nMinDbCache, nMaxDbCache, nDefaultDbCache));
    strUsage += HelpMessageOpt("-loadblock=<file>", "Imports blocks from external blk000??.dat file on startup");
    strUsage += HelpMessageOpt("-maxreorg=<n>", strprintf("Set the Maximum reorg depth (default: %u)", DEFAULT_MAX_REORG_DEPTH));
    strUsage += HelpMessageOpt("-maxorphantx=<n>", strprintf("Keep at most <n> unconnectable transactions in memory (default: %u)", DEFAULT_MAX_ORPHAN_TRANSACTIONS));
    strUsage += HelpMessageOpt("-maxmempool=<n>", strprintf("Keep the transaction memory pool below <n> megabytes (default: %u)", DEFAULT_MAX_MEMPOOL_SIZE));
    strUsage += HelpMessageOpt("-mempoolexpiry=<n>", strprintf("Do not keep transactions in the mempool longer than <n> hours (default: %u)", DEFAULT_MEMPOOL_EXPIRY));
    strUsage += HelpMessageOpt("-persistmempool", strprintf("Whether to save the mempool on shutdown and load on restart (default: %u)", DEFAULT_PERSIST_MEMPOOL));
    strUsage += HelpMessageOpt("-par=<n>", strprintf("Set the number of script verification threads (%u to %d, 0 = auto, <0 = leave that many cores free, default: %d)", -GetNumCores(), MAX_SCRIPTCHECK_THREADS, DEFAULT_SCRIPTCHECK_THREADS));
#ifndef WIN32
    strUsage += HelpMessageOpt("-pid=<file>", strprintf("Specify pid file (default: %s)", PIVX_PID_FILENAME));
#endif
    strUsage += HelpMessageOpt("-reindex-chainstate", "Rebuild chain state from the currently indexed blocks");
    strUsage += HelpMessageOpt("-reindex", "Rebuild block chain index from current blk000??.dat files on startup");
    strUsage += HelpMessageOpt("-resync", "Delete blockchain folders and resync from scratch on startup");
#if !defined(WIN32)
    strUsage += HelpMessageOpt("-sysperms", "Create new files with system default permissions, instead of umask 077 (only effective with disabled wallet functionality)");
#endif
    strUsage += HelpMessageOpt("-txindex", strprintf("Maintain a full transaction index, used by the getrawtransaction rpc call (default: %u)", DEFAULT_TXINDEX));
    strUsage += HelpMessageOpt("-forcestart", "Attempt to force blockchain corruption recovery on startup");

    strUsage += HelpMessageGroup("Connection options:");
    strUsage += HelpMessageOpt("-addnode=<ip>", "Add a node to connect to and attempt to keep the connection open");
    strUsage += HelpMessageOpt("-asmap=<file>", strprintf("Specify asn mapping used for bucketing of the peers (default: %s). Relative paths will be prefixed by the net-specific datadir location.", DEFAULT_ASMAP_FILENAME));
    strUsage += HelpMessageOpt("-banscore=<n>", strprintf("Threshold for disconnecting misbehaving peers (default: %u)", DEFAULT_BANSCORE_THRESHOLD));
    strUsage += HelpMessageOpt("-bantime=<n>", strprintf("Number of seconds to keep misbehaving peers from reconnecting (default: %u)", DEFAULT_MISBEHAVING_BANTIME));
    strUsage += HelpMessageOpt("-bind=<addr>", "Bind to given address and always listen on it. Use [host]:port notation for IPv6");
    strUsage += HelpMessageOpt("-connect=<ip>", "Connect only to the specified node(s); -noconnect or -connect=0 alone to disable automatic connections");
    strUsage += HelpMessageOpt("-discover", "Discover own IP address (default: 1 when listening and no -externalip)");
    strUsage += HelpMessageOpt("-dns", strprintf("Allow DNS lookups for -addnode, -seednode and -connect (default: %u)", DEFAULT_NAME_LOOKUP));
    strUsage += HelpMessageOpt("-dnsseed", "Query for peer addresses via DNS lookup, if low on addresses (default: 1 unless -connect/-noconnect)");
    strUsage += HelpMessageOpt("-externalip=<ip>", "Specify your own public address");
    strUsage += HelpMessageOpt("-forcednsseed", strprintf("Always query for peer addresses via DNS lookup (default: %u)", DEFAULT_FORCEDNSSEED));
    strUsage += HelpMessageOpt("-listen", strprintf("Accept connections from outside (default: %u if no -proxy or -connect/-noconnect)", DEFAULT_LISTEN));
    strUsage += HelpMessageOpt("-listenonion", strprintf("Automatically create Tor hidden service (default: %d)", DEFAULT_LISTEN_ONION));
    strUsage += HelpMessageOpt("-maxconnections=<n>", strprintf("Maintain at most <n> connections to peers (default: %u)", DEFAULT_MAX_PEER_CONNECTIONS));
    strUsage += HelpMessageOpt("-maxreceivebuffer=<n>", strprintf("Maximum per-connection receive buffer, <n>*1000 bytes (default: %u)", DEFAULT_MAXRECEIVEBUFFER));
    strUsage += HelpMessageOpt("-maxsendbuffer=<n>", strprintf("Maximum per-connection send buffer, <n>*1000 bytes (default: %u)", DEFAULT_MAXSENDBUFFER));
    strUsage += HelpMessageOpt("-onion=<ip:port>", strprintf("Use separate SOCKS5 proxy to reach peers via Tor hidden services (default: %s)", "-proxy"));
    strUsage += HelpMessageOpt("-onlynet=<net>", "Only connect to nodes in network <net> (ipv4, ipv6 or onion)");
    strUsage += HelpMessageOpt("-permitbaremultisig", strprintf("Relay non-P2SH multisig (default: %u)", DEFAULT_PERMIT_BAREMULTISIG));
    strUsage += HelpMessageOpt("-peerbloomfilters", strprintf("Support filtering of blocks and transaction with bloom filters (default: %u)", DEFAULT_PEERBLOOMFILTERS));
    strUsage += HelpMessageOpt("-port=<port>", strprintf("Listen for connections on <port> (default: %u or testnet: %u)", defaultChainParams->GetDefaultPort(), testnetChainParams->GetDefaultPort()));
    strUsage += HelpMessageOpt("-proxy=<ip:port>", "Connect through SOCKS5 proxy");
    strUsage += HelpMessageOpt("-proxyrandomize", strprintf("Randomize credentials for every proxy connection. This enables Tor stream isolation (default: %u)", DEFAULT_PROXYRANDOMIZE));
    strUsage += HelpMessageOpt("-seednode=<ip>", "Connect to a node to retrieve peer addresses, and disconnect");
    strUsage += HelpMessageOpt("-timeout=<n>", strprintf("Specify connection timeout in milliseconds (minimum: 1, default: %d)", DEFAULT_CONNECT_TIMEOUT));
    strUsage += HelpMessageOpt("-torcontrol=<ip>:<port>", strprintf("Tor control port to use if onion listening enabled (default: %s)", DEFAULT_TOR_CONTROL));
    strUsage += HelpMessageOpt("-torpassword=<pass>", "Tor control port password (default: empty)");
    strUsage += HelpMessageOpt("-upnp", strprintf("Use UPnP to map the listening port (default: %u)", DEFAULT_UPNP));
#ifdef USE_NATPMP
    strUsage += HelpMessageOpt("-natpmp", strprintf("Use NAT-PMP to map the listening port (default: %s)", DEFAULT_NATPMP ? "1 when listening and no -proxy" : "0"));
#endif // USE_NATPMP
    strUsage += HelpMessageOpt("-whitebind=<addr>", "Bind to given address and whitelist peers connecting to it. Use [host]:port notation for IPv6");
    strUsage += HelpMessageOpt("-whitelist=<netmask>", "Whitelist peers connecting from the given netmask or IP address. Can be specified multiple times."
        " Whitelisted peers cannot be DoS banned and their transactions are always relayed, even if they are already in the mempool, useful e.g. for a gateway");

#if ENABLE_WALLET
    strUsage += GetWalletHelpString(showDebug);
#endif

    if (mode == HMM_BITCOIN_QT) {
        strUsage += HelpMessageOpt("-windowtitle=<name>", "Wallet window title");
    }

#if ENABLE_ZMQ
    strUsage += HelpMessageGroup("ZeroMQ notification options:");
    strUsage += HelpMessageOpt("-zmqpubhashblock=<address>", "Enable publish hash block in <address>");
    strUsage += HelpMessageOpt("-zmqpubhashtx=<address>", "Enable publish hash transaction in <address>");
    strUsage += HelpMessageOpt("-zmqpubrawblock=<address>", "Enable publish raw block in <address>");
    strUsage += HelpMessageOpt("-zmqpubrawtx=<address>", "Enable publish raw transaction in <address>");
#endif

    strUsage += HelpMessageGroup("Debugging/Testing options:");
    strUsage += HelpMessageOpt("-uacomment=<cmt>", "Append comment to the user agent string");
    if (showDebug) {
        strUsage += HelpMessageOpt("-checkblockindex", strprintf("Do a full consistency check for mapBlockIndex, setBlockIndexCandidates, chainActive and mapBlocksUnlinked occasionally. Also sets -checkmempool (default: %u)", defaultChainParams->DefaultConsistencyChecks()));
        strUsage += HelpMessageOpt("-checkmempool=<n>", strprintf("Run checks every <n> transactions (default: %u)", defaultChainParams->DefaultConsistencyChecks()));
        strUsage += HelpMessageOpt("-checkpoints", strprintf("Only accept block chain matching built-in checkpoints (default: %u)", DEFAULT_CHECKPOINTS_ENABLED));
        strUsage += HelpMessageOpt("-disablesafemode", strprintf("Disable safemode, override a real safe mode event (default: %u)", DEFAULT_DISABLE_SAFEMODE));
        strUsage += HelpMessageOpt("-testsafemode", strprintf("Force safe mode (default: %u)", DEFAULT_TESTSAFEMODE));
        strUsage += HelpMessageOpt("-deprecatedrpc=<method>", "Allows deprecated RPC method(s) to be used");
        strUsage += HelpMessageOpt("-dropmessagestest=<n>", "Randomly drop 1 of every <n> network messages");
        strUsage += HelpMessageOpt("-fuzzmessagestest=<n>", "Randomly fuzz 1 of every <n> network messages");
        strUsage += HelpMessageOpt("-stopafterblockimport", strprintf("Stop running after importing blocks from disk (default: %u)", DEFAULT_STOPAFTERBLOCKIMPORT));
        strUsage += HelpMessageOpt("-limitancestorcount=<n>", strprintf("Do not accept transactions if number of in-mempool ancestors is <n> or more (default: %u)", DEFAULT_ANCESTOR_LIMIT));
        strUsage += HelpMessageOpt("-limitancestorsize=<n>", strprintf("Do not accept transactions whose size with all in-mempool ancestors exceeds <n> kilobytes (default: %u)", DEFAULT_ANCESTOR_SIZE_LIMIT));
        strUsage += HelpMessageOpt("-limitdescendantcount=<n>", strprintf("Do not accept transactions if any ancestor would have <n> or more in-mempool descendants (default: %u)", DEFAULT_DESCENDANT_LIMIT));
        strUsage += HelpMessageOpt("-limitdescendantsize=<n>", strprintf("Do not accept transactions if any ancestor would have more than <n> kilobytes of in-mempool descendants (default: %u).", DEFAULT_DESCENDANT_SIZE_LIMIT));
        strUsage += HelpMessageOpt("-sporkkey=<privkey>", "Enable spork administration functionality with the appropriate private key.");
        strUsage += HelpMessageOpt("-nuparams=upgradeName:activationHeight", "Use given activation height for specified network upgrade (regtest-only)");
    }
    strUsage += HelpMessageOpt("-debug=<category>", strprintf("Output debugging information (default: %u, supplying <category> is optional)", 0) + ". " +
        "If <category> is not supplied, output all debugging information. <category> can be: " + ListLogCategories() + ".");
    strUsage += HelpMessageOpt("-debugexclude=<category>", "Exclude debugging information for a category. Can be used in conjunction with -debug=1 to output debug logs for all categories except one or more specified categories.");
    if (showDebug)
        strUsage += HelpMessageOpt("-nodebug", "Turn off debugging messages, same as -debug=0");

    strUsage += HelpMessageOpt("-help-debug", "Show all debugging options (usage: --help -help-debug)");
    strUsage += HelpMessageOpt("-logips", strprintf("Include IP addresses in debug output (default: %u)", DEFAULT_LOGIPS));
    strUsage += HelpMessageOpt("-logtimestamps", strprintf("Prepend debug output with timestamp (default: %u)", DEFAULT_LOGTIMESTAMPS));
    strUsage += HelpMessageOpt("-logtimemicros", strprintf("Add microsecond precision to debug timestamps (default: %u)", DEFAULT_LOGTIMEMICROS));
    if (showDebug) {
        strUsage += HelpMessageOpt("-mocktime=<n>", "Replace actual time with <n> seconds since epoch (default: 0)");
        strUsage += HelpMessageOpt("-maxsigcachesize=<n>", strprintf("Limit size of signature cache to <n> MiB (default: %u)", DEFAULT_MAX_SIG_CACHE_SIZE));
    }
    strUsage += HelpMessageOpt("-maxtipage=<n>", strprintf("Maximum tip age in seconds to consider node in initial block download (default: %u)", DEFAULT_MAX_TIP_AGE));
    strUsage += HelpMessageOpt("-minrelaytxfee=<amt>", strprintf("Fees (in %s/Kb) smaller than this are considered zero fee for relaying, mining and transaction creation (default: %s)", CURRENCY_UNIT, FormatMoney(::minRelayTxFee.GetFeePerK())));
    strUsage += HelpMessageOpt("-printtoconsole", strprintf("Send trace/debug info to console instead of debug.log file (default: %u)", 0));
    if (showDebug) {
        strUsage += HelpMessageOpt("-dustrelayfee=<amt>", strprintf("Fee rate (in %s/kB) used to define dust, the value of an output such that it will cost more than its value in fees at this fee rate to spend it. (default: %s)", CURRENCY_UNIT, FormatMoney(DUST_RELAY_TX_FEE)));
        strUsage += HelpMessageOpt("-printpriority", strprintf("Log transaction fee per kB when mining blocks (default: %u)", DEFAULT_PRINTPRIORITY));
    }
    strUsage += HelpMessageOpt("-shrinkdebugfile", "Shrink debug.log file on client startup (default: 1 when no -debug)");
    AppendParamsHelpMessages(strUsage, showDebug);

    strUsage += HelpMessageGroup("Masternode options:");
    strUsage += HelpMessageOpt("-masternode=<n>", strprintf("Enable the client to act as a masternode (0-1, default: %u)", DEFAULT_MASTERNODE));
    strUsage += HelpMessageOpt("-mnconf=<file>", strprintf("Specify masternode configuration file (default: %s)", PIVX_MASTERNODE_CONF_FILENAME));
    strUsage += HelpMessageOpt("-mnconflock=<n>", strprintf("Lock masternodes from masternode configuration file (default: %u)", DEFAULT_MNCONFLOCK));
    strUsage += HelpMessageOpt("-masternodeprivkey=<n>", "Set the masternode private key");
    strUsage += HelpMessageOpt("-masternodeaddr=<n>", strprintf("Set external address:port to get to this masternode (example: %s)", "128.127.106.235:51472"));
    strUsage += HelpMessageOpt("-budgetvotemode=<mode>", "Change automatic finalized budget voting behavior. mode=auto: Vote for only exact finalized budget match to my generated budget. (string, default: auto)");
    strUsage += HelpMessageOpt("-mnoperatorprivatekey=<WIF>", "Set the masternode operator private key. Only valid with -masternode=1. When set, the masternode acts as a deterministic masternode.");

    strUsage += HelpMessageGroup("Node relay options:");
    if (showDebug) {
        strUsage += HelpMessageOpt("-acceptnonstdtxn",
                                   strprintf("Relay and mine \"non-standard\" transactions (%sdefault: %u)",
                                             "testnet/regtest only; ",
                                             !CreateChainParams(CBaseChainParams::TESTNET)->RequireStandard()));
    }
    strUsage += HelpMessageOpt("-datacarrier", strprintf("Relay and mine data carrier transactions (default: %u)", DEFAULT_ACCEPT_DATACARRIER));
    strUsage += HelpMessageOpt("-datacarriersize", strprintf("Maximum size of data in data carrier transactions we relay and mine (default: %u)", MAX_OP_RETURN_RELAY));
    if (showDebug) {
        strUsage += HelpMessageOpt("-blockversion=<n>", "Override block version to test forking scenarios");
    }

    strUsage += HelpMessageGroup("Block creation options:");
    strUsage += HelpMessageOpt("-blockmaxsize=<n>", strprintf("Set maximum block size in bytes (default: %d)", DEFAULT_BLOCK_MAX_SIZE));
    if (showDebug)
        strUsage += HelpMessageOpt("-blockversion=<n>", "Override block version to test forking scenarios");

    strUsage += HelpMessageGroup("RPC server options:");
    strUsage += HelpMessageOpt("-server", "Accept command line and JSON-RPC commands");
    strUsage += HelpMessageOpt("-rest", strprintf("Accept public REST requests (default: %u)", DEFAULT_REST_ENABLE));
    strUsage += HelpMessageOpt("-rpcbind=<addr>", "Bind to given address to listen for JSON-RPC connections. Do not expose the RPC server to untrusted networks such as the public internet! This option is ignored unless -rpcallowip is also passed. Port is optional and overrides -rpcport. Use [host]:port notation for IPv6. This option can be specified multiple times (default: 127.0.0.1 and ::1 i.e., localhost)");
    strUsage += HelpMessageOpt("-rpccookiefile=<loc>", "Location of the auth cookie (default: data dir)");
    strUsage += HelpMessageOpt("-rpcuser=<user>", "Username for JSON-RPC connections");
    strUsage += HelpMessageOpt("-rpcpassword=<pw>", "Password for JSON-RPC connections");
    strUsage += HelpMessageOpt("-rpcauth=<userpw>", "Username and hashed password for JSON-RPC connections. The field <userpw> comes in the format: <USERNAME>:<SALT>$<HASH>. A canonical python script is included in share/rpcuser. The client then connects normally using the rpcuser=<USERNAME>/rpcpassword=<PASSWORD> pair of arguments. This option can be specified multiple times");
    strUsage += HelpMessageOpt("-rpcport=<port>", strprintf("Listen for JSON-RPC connections on <port> (default: %u or testnet: %u)", defaultBaseParams->RPCPort(), testnetBaseParams->RPCPort()));
    strUsage += HelpMessageOpt("-rpcallowip=<ip>", "Allow JSON-RPC connections from specified source. Valid for <ip> are a single IP (e.g. 1.2.3.4), a network/netmask (e.g. 1.2.3.4/255.255.255.0) or a network/CIDR (e.g. 1.2.3.4/24). This option can be specified multiple times");
    strUsage += HelpMessageOpt("-rpcthreads=<n>", strprintf("Set the number of threads to service RPC calls (default: %d)", DEFAULT_HTTP_THREADS));
    if (showDebug) {
        strUsage += HelpMessageOpt("-rpcworkqueue=<n>", strprintf("Set the depth of the work queue to service RPC calls (default: %d)", DEFAULT_HTTP_WORKQUEUE));
        strUsage += HelpMessageOpt("-rpcservertimeout=<n>", strprintf("Timeout during HTTP requests (default: %d)", DEFAULT_HTTP_SERVER_TIMEOUT));
    }

    strUsage += HelpMessageOpt("-blockspamfilter=<n>", strprintf("Use block spam filter (default: %u)", DEFAULT_BLOCK_SPAM_FILTER));
    strUsage += HelpMessageOpt("-blockspamfiltermaxsize=<n>", strprintf("Maximum size of the list of indexes in the block spam filter (default: %u)", DEFAULT_BLOCK_SPAM_FILTER_MAX_SIZE));
    strUsage += HelpMessageOpt("-blockspamfiltermaxavg=<n>", strprintf("Maximum average size of an index occurrence in the block spam filter (default: %u)", DEFAULT_BLOCK_SPAM_FILTER_MAX_AVG));

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return strUsage;
}

std::string LicenseInfo()
{
    return FormatParagraph(strprintf(_("Copyright (C) 2009-%i The Bitcoin Core Developers"), COPYRIGHT_YEAR)) + "\n" +
           "\n" +
           FormatParagraph(strprintf(_("Copyright (C) 2014-%i The Dash Core Developers"), COPYRIGHT_YEAR)) + "\n" +
           "\n" +
<<<<<<< HEAD
           FormatParagraph(strprintf(_("Copyright (C) 2015-%i The PIVX Core Developers"), COPYRIGHT_YEAR)) + "\n" +
           "\n" +
           FormatParagraph(strprintf(_("Copyright (C) 2018-%i The DAPS Project developers"), COPYRIGHT_YEAR)) + "\n" +
           "\n" +
           FormatParagraph(strprintf(_("Copyright (C) 2020-%i The PRCY developers"), COPYRIGHT_YEAR)) + "\n" +
=======
           FormatParagraph(strprintf(_("Copyright (C) 2015-%i The %s Developers"), COPYRIGHT_YEAR, PACKAGE_NAME)) + "\n" +
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
           "\n" +
           FormatParagraph(_("This is experimental software.")) + "\n" +
           "\n" +
           FormatParagraph(_("Distributed under the MIT software license, see the accompanying file COPYING or <http://www.opensource.org/licenses/mit-license.php>.")) + "\n" +
           "\n" +
<<<<<<< HEAD
           FormatParagraph(_("This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit <https://www.openssl.org/> and cryptographic software written by Eric Young and UPnP software written by Thomas Bernard.")) +
=======
           FormatParagraph(_("This product includes UPnP software written by Thomas Bernard.")) +
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
           "\n";
}

static void BlockNotifyCallback(bool initialSync, const CBlockIndex *pBlockIndex)
{
<<<<<<< HEAD
    if (initialSync || !pBlockIndex)
        return;

    std::string strCmd = GetArg("-blocknotify", "");
=======

    if (initialSync || !pBlockIndex)
        return;

    std::string strCmd = gArgs.GetArg("-blocknotify", "");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (!strCmd.empty()) {
        boost::replace_all(strCmd, "%s", pBlockIndex->GetBlockHash().GetHex());
        std::thread t(runCommand, strCmd);
        t.detach(); // thread runs free
    }
}

<<<<<<< HEAD
static void BlockSizeNotifyCallback(int size, const uint256& hashNewTip)
{
    std::string strCmd = GetArg("-blocksizenotify", "");

    boost::replace_all(strCmd, "%s", hashNewTip.GetHex());
    boost::replace_all(strCmd, "%d", std::to_string(size));
    boost::thread t(runCommand, strCmd); // thread runs free
}

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
////////////////////////////////////////////////////

static bool fHaveGenesis = false;
static std::mutex cs_GenesisWait;
static std::condition_variable condvar_GenesisWait;

static void BlockNotifyGenesisWait(bool, const CBlockIndex *pBlockIndex)
{
    if (pBlockIndex != nullptr) {
        {
            std::unique_lock<std::mutex> lock_GenesisWait(cs_GenesisWait);
            fHaveGenesis = true;
        }
        condvar_GenesisWait.notify_all();
    }
}

////////////////////////////////////////////////////


struct CImportingNow {
    CImportingNow()
    {
        assert(fImporting == false);
        fImporting = true;
    }

    ~CImportingNow()
    {
        assert(fImporting == true);
        fImporting = false;
    }
};

<<<<<<< HEAD
void ThreadImport(std::vector<boost::filesystem::path> vImportFiles)
{
    util::ThreadRename("prcycoin-loadblk");

    // -reindex
    if (fReindex) {
        CImportingNow imp;
        int nFile = 0;
        while (true) {
            CDiskBlockPos pos(nFile, 0);
            if (!boost::filesystem::exists(GetBlockPosFilename(pos, "blk")))
=======
void ThreadImport(const std::vector<fs::path>& vImportFiles)
{
    util::ThreadRename("pivx-loadblk");
    CImportingNow imp;
    ScheduleBatchPriority();

    // -reindex
    if (fReindex) {
        int nFile = 0;
        while (true) {
            FlatFilePos pos(nFile, 0);
            if (!fs::exists(GetBlockPosFilename(pos)))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                break; // No block files left to reindex
            FILE* file = OpenBlockFile(pos, true);
            if (!file)
                break; // This error is logged in OpenBlockFile
            LogPrintf("Reindexing block file blk%05u.dat...\n", (unsigned int)nFile);
            LoadExternalBlockFile(file, &pos);
            nFile++;
        }
        pblocktree->WriteReindexing(false);
        fReindex = false;
        LogPrintf("Reindexing finished\n");
        // To avoid ending up in a situation without genesis block, re-try initializing (no-op if reindexing worked):
<<<<<<< HEAD
        InitBlockIndex();
    }

    // hardcoded $DATADIR/bootstrap.dat
    filesystem::path pathBootstrap = GetDataDir() / "bootstrap.dat";
    if (filesystem::exists(pathBootstrap)) {
        FILE* file = fopen(pathBootstrap.string().c_str(), "rb");
        if (file) {
            CImportingNow imp;
            filesystem::path pathBootstrapOld = GetDataDir() / "bootstrap.dat.old";
=======
        if (!LoadGenesisBlock()) {
            throw std::runtime_error("Error initializing block database");
        }
    }

    // hardcoded $DATADIR/bootstrap.dat
    fs::path pathBootstrap = GetDataDir() / "bootstrap.dat";
    if (fs::exists(pathBootstrap)) {
        FILE* file = fsbridge::fopen(pathBootstrap, "rb");
        if (file) {
            fs::path pathBootstrapOld = GetDataDir() / "bootstrap.dat.old";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            LogPrintf("Importing bootstrap.dat...\n");
            LoadExternalBlockFile(file);
            RenameOver(pathBootstrap, pathBootstrapOld);
        } else {
            LogPrintf("Warning: Could not open bootstrap file %s\n", pathBootstrap.string());
        }
    }

    // -loadblock=
<<<<<<< HEAD
    for (boost::filesystem::path& path : vImportFiles) {
        FILE* file = fopen(path.string().c_str(), "rb");
        if (file) {
            CImportingNow imp;
=======
    for (const fs::path& path : vImportFiles) {
        FILE* file = fsbridge::fopen(path, "rb");
        if (file) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            LogPrintf("Importing blocks file %s...\n", path.string());
            LoadExternalBlockFile(file);
        } else {
            LogPrintf("Warning: Could not open blocks file %s\n", path.string());
        }
    }

<<<<<<< HEAD
    if (GetBoolArg("-stopafterblockimport", false)) {
        LogPrintf("Stopping after block import\n");
        StartShutdown();
    }
}

/** Sanity checks
 *  Ensure that PRCY is running in a usable environment with all
=======
    if (gArgs.GetBoolArg("-stopafterblockimport", DEFAULT_STOPAFTERBLOCKIMPORT)) {
        LogPrintf("Stopping after block import\n");
        StartShutdown();
    }

    // scan for better chains in the block chain database, that are not yet connected in the active best chain
    CValidationState state;
    if (!ActivateBestChain(state)) {
        LogPrintf("Failed to connect best block");
        StartShutdown();
    }

    // force UpdatedBlockTip to initialize nCachedBlockHeight for DS, MN payments and budgets
    // but don't call it directly to prevent triggering of other listeners like zmq etc.
    pEvoNotificationInterface->InitializeCurrentBlockTip();

    if (gArgs.GetBoolArg("-persistmempool", DEFAULT_PERSIST_MEMPOOL)) {
        LoadMempool(::mempool);
    }
    ::mempool.SetIsLoaded(!ShutdownRequested());
}

/** Sanity checks
 *  Ensure that PIVX is running in a usable environment with all
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *  necessary library support.
 */
bool InitSanityCheck(void)
{
    if (!ECC_InitSanityCheck()) {
<<<<<<< HEAD
        InitError("OpenSSL appears to lack support for elliptic curve cryptography. For more "
                  "information, visit https://en.bitcoin.it/wiki/OpenSSL_and_EC_Libraries");
        return false;
    }

    if (!glibc_sanity_test() || !glibcxx_sanity_test())
        return false;

    if (!Random_SanityCheck()) {
        InitError("OS cryptographic RNG sanity check failure. Aborting.");
=======
        UIError(_("Elliptic curve cryptography sanity check failure. Aborting."));
        return false;
    }

    if (!glibc_sanity_test() || !glibcxx_sanity_test()) {
        return false;
    }

    if (!Random_SanityCheck()) {
        UIError(_("OS cryptographic RNG sanity check failure. Aborting."));
        return false;
    }

    if (!BLSInit()) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    }

    return true;
}

<<<<<<< HEAD
=======
static void LoadSaplingParams()
{
    struct timeval tv_start{}, tv_end{};
    float elapsed;
    gettimeofday(&tv_start, nullptr);

    try {
        initZKSNARKS();
    } catch (std::runtime_error &e) {
        uiInterface.ThreadSafeMessageBox(strprintf(
                _("Cannot find the Sapling parameters in the following directory:\n"
                  "%s\n"
                  "Please run 'sapling-fetch-params' or './util/fetch-params.sh' and then restart."),
                ZC_GetParamsDir()),
                                         "", CClientUIInterface::MSG_ERROR);
        StartShutdown();
        return;
    }

    gettimeofday(&tv_end, nullptr);
    elapsed = float(tv_end.tv_sec-tv_start.tv_sec) + (tv_end.tv_usec-tv_start.tv_usec)/float(1000000);
    LogPrintf("Loaded Sapling parameters in %fs seconds.\n", elapsed);
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
bool AppInitServers()
{
    RPCServer::OnStarted(&OnRPCStarted);
    RPCServer::OnStopped(&OnRPCStopped);
    RPCServer::OnPreCommand(&OnRPCPreCommand);
    if (!InitHTTPServer())
        return false;
    if (!StartRPC())
        return false;
    if (!StartHTTPRPC())
        return false;
<<<<<<< HEAD
    if (GetBoolArg("-rest", false) && !StartREST())
=======
    if (gArgs.GetBoolArg("-rest", DEFAULT_REST_ENABLE) && !StartREST())
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    if (!StartHTTPServer())
        return false;
    return true;
}

[[noreturn]] static void new_handler_terminate()
{
    // Rather than throwing std::bad-alloc if allocation fails, terminate
    // immediately to (try to) avoid chain corruption.
    // Since LogPrintf may itself allocate memory, set the handler directly
    // to terminate first.
    std::set_new_handler(std::terminate);
    LogPrintf("Error: Out of memory. Terminating.\n");

    // The log was successful, terminate now.
    std::terminate();
<<<<<<< HEAD
};
=======
}

namespace { // Variables internal to initialization process only

    ServiceFlags nRelevantServices = NODE_NETWORK;
    int nMaxConnections;
    int nUserMaxConnections;
    int nFD;
    ServiceFlags nLocalServices = NODE_NETWORK;
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

bool AppInitBasicSetup()
{
// ********************************************************* Step 1: setup
#ifdef _MSC_VER
    // Turn off Microsoft heap dump noise
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, CreateFileA("NUL", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0));
#endif
#if _MSC_VER >= 1400
    // Disable confusing "helpful" text message on abort, Ctrl-C
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
#ifdef WIN32
<<<<<<< HEAD
// Enable Data Execution Prevention (DEP)
=======
    // Enable Data Execution Prevention (DEP)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Minimum supported OS versions: WinXP SP3, WinVista >= SP1, Win Server 2008
// A failure is non-critical and needs no further attention!
#ifndef PROCESS_DEP_ENABLE
// We define this here, because GCCs winbase.h limits this to _WIN32_WINNT >= 0x0601 (Windows 7),
// which is not correct. Can be removed, when GCCs winbase.h is fixed!
#define PROCESS_DEP_ENABLE 0x00000001
#endif
    typedef BOOL(WINAPI * PSETPROCDEPPOL)(DWORD);
    PSETPROCDEPPOL setProcDEPPol = (PSETPROCDEPPOL)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "SetProcessDEPPolicy");
    if (setProcDEPPol != NULL) setProcDEPPol(PROCESS_DEP_ENABLE);
#endif

    if (!SetupNetworking())
<<<<<<< HEAD
        return InitError("Error: Initializing networking failed");

#ifndef WIN32
    if (GetBoolArg("-sysperms", false)) {
#ifdef ENABLE_WALLET
        if (!GetBoolArg("-disablewallet", false))
            return InitError("Error: -sysperms is not allowed in combination with enabled wallet functionality");
#endif
    } else {
=======
        return UIError(_("Error: Initializing networking failed"));

#ifndef WIN32
    if (!gArgs.GetBoolArg("-sysperms", false)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        umask(077);
    }

    // Clean shutdown on SIGTERMx
    registerSignalHandler(SIGTERM, HandleSIGTERM);
    registerSignalHandler(SIGINT, HandleSIGTERM);

    // Reopen debug.log on SIGHUP
    registerSignalHandler(SIGHUP, HandleSIGHUP);

<<<<<<< HEAD
// Ignore SIGPIPE, otherwise it will bring the daemon down if the client closes unexpectedly
=======
    // Ignore SIGPIPE, otherwise it will bring the daemon down if the client closes unexpectedly
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    signal(SIGPIPE, SIG_IGN);
#endif

    std::set_new_handler(new_handler_terminate);

    return true;
}

// Parameter interaction based on rules
void InitParameterInteraction()
{
<<<<<<< HEAD
    if (mapArgs.count("-bind") || mapArgs.count("-whitebind")) {
        // when specifying an explicit binding address, you want to listen on it
        // even when -connect or -proxy is specified
        if (SoftSetBoolArg("-listen", true))
            LogPrintf("%s : parameter interaction: -bind or -whitebind set -> setting -listen=1\n", __func__);
    }

    if (mapArgs.count("-connect") && mapMultiArgs["-connect"].size() > 0) {
        // when only connecting to trusted nodes, do not seed via DNS, or listen by default
        if (SoftSetBoolArg("-dnsseed", false))
            LogPrintf("%s : parameter interaction: -connect set -> setting -dnsseed=0\n", __func__);
        if (SoftSetBoolArg("-listen", false))
            LogPrintf("%s : parameter interaction: -connect set -> setting -listen=0\n", __func__);
    }

    if (mapArgs.count("-proxy")) {
        // to protect privacy, do not listen by default if a default proxy server is specified
        if (SoftSetBoolArg("-listen", false))
            LogPrintf("%s: parameter interaction: -proxy set -> setting -listen=0\n", __func__);
        // to protect privacy, do not use UPNP when a proxy is set. The user may still specify -listen=1
        // to listen locally, so don't rely on this happening through -listen below.
        if (SoftSetBoolArg("-upnp", false))
            LogPrintf("%s: parameter interaction: -proxy set -> setting -upnp=0\n", __func__);
        // to protect privacy, do not discover addresses by default
        if (SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -proxy set -> setting -discover=0\n", __func__);
    }

    if (!GetBoolArg("-listen", true)) {
        // do not map ports or try to retrieve public IP when not listening (pointless)
        if (SoftSetBoolArg("-upnp", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -upnp=0\n", __func__);
        if (SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -discover=0\n", __func__);
        if (SoftSetBoolArg("-listenonion", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -listenonion=0\n", __func__);
    }

    if (mapArgs.count("-externalip")) {
        // if an explicit public IP is specified, do not try to find others
        if (SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -externalip set -> setting -discover=0\n", __func__);
    }

    if (GetBoolArg("-salvagewallet", false)) {
        // Rewrite just private keys: rescan to find transactions
        if (SoftSetBoolArg("-rescan", true))
            LogPrintf("%s : parameter interaction: -salvagewallet=1 -> setting -rescan=1\n", __func__);
    }

    // -zapwallettx implies a rescan
    if (GetBoolArg("-zapwallettxes", false)) {
        if (SoftSetBoolArg("-rescan", true))
            LogPrintf("%s : parameter interaction: -zapwallettxes=<mode> -> setting -rescan=1\n", __func__);
    }

    if (!GetBoolArg("-enableswifttx", fEnableSwiftTX)) {
        if (SoftSetArg("-swifttxdepth", "0"))
            LogPrintf("%s : parameter interaction: -enableswifttx=false -> setting -nSwiftTXDepth=0\n", __func__);
    }
=======
    if (gArgs.IsArgSet("-bind") || gArgs.IsArgSet("-whitebind")) {
        // when specifying an explicit binding address, you want to listen on it
        // even when -connect or -proxy is specified
        if (gArgs.SoftSetBoolArg("-listen", true))
            LogPrintf("%s : parameter interaction: -bind or -whitebind set -> setting -listen=1\n", __func__);
    }

    if (gArgs.IsArgSet("-connect")) {
        // when only connecting to trusted nodes, do not seed via DNS, or listen by default
        if (gArgs.SoftSetBoolArg("-dnsseed", false))
            LogPrintf("%s : parameter interaction: -connect set -> setting -dnsseed=0\n", __func__);
        if (gArgs.SoftSetBoolArg("-listen", false))
            LogPrintf("%s : parameter interaction: -connect set -> setting -listen=0\n", __func__);
    }

    if (gArgs.IsArgSet("-proxy")) {
        // to protect privacy, do not listen by default if a default proxy server is specified
        if (gArgs.SoftSetBoolArg("-listen", false))
            LogPrintf("%s: parameter interaction: -proxy set -> setting -listen=0\n", __func__);
        // to protect privacy, do not use map ports when a proxy is set. The user may still specify -listen=1
        // to listen locally, so don't rely on this happening through -listen below.
        if (gArgs.SoftSetBoolArg("-upnp", false))
            LogPrintf("%s: parameter interaction: -proxy set -> setting -upnp=0\n", __func__);
        if (gArgs.SoftSetBoolArg("-natpmp", false)) {
            LogPrintf("%s: parameter interaction: -proxy set -> setting -natpmp=0\n", __func__);
        }
        // to protect privacy, do not discover addresses by default
        if (gArgs.SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -proxy set -> setting -discover=0\n", __func__);
    }

    if (!gArgs.GetBoolArg("-listen", DEFAULT_LISTEN)) {
        // do not map ports or try to retrieve public IP when not listening (pointless)
        if (gArgs.SoftSetBoolArg("-upnp", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -upnp=0\n", __func__);
        if (gArgs.SoftSetBoolArg("-natpmp", false)) {
            LogPrintf("%s: parameter interaction: -listen=0 -> setting -natpmp=0\n", __func__);
        }
        if (gArgs.SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -discover=0\n", __func__);
        if (gArgs.SoftSetBoolArg("-listenonion", false))
            LogPrintf("%s : parameter interaction: -listen=0 -> setting -listenonion=0\n", __func__);
    }

    if (gArgs.IsArgSet("-externalip")) {
        // if an explicit public IP is specified, do not try to find others
        if (gArgs.SoftSetBoolArg("-discover", false))
            LogPrintf("%s : parameter interaction: -externalip set -> setting -discover=0\n", __func__);
    }
}

bool InitNUParams()
{
    if (gArgs.IsArgSet("-nuparams")) {
        // Allow overriding network upgrade parameters for testing
        if (Params().NetworkIDString() != "regtest") {
            return UIError(_("Network upgrade parameters may only be overridden on regtest."));
        }
        for (const std::string& strDeployment : gArgs.GetArgs("-nuparams")) {
            std::vector<std::string> vDeploymentParams;
            boost::split(vDeploymentParams, strDeployment, boost::is_any_of(":"));
            if (vDeploymentParams.size() != 2) {
                return UIError(strprintf(_("Network upgrade parameters malformed, expecting %s"), "hexBranchId:activationHeight"));
            }
            int nActivationHeight;
            if (!ParseInt32(vDeploymentParams[1], &nActivationHeight)) {
                return UIError(strprintf(_("Invalid activation height (%s)"), vDeploymentParams[1]));
            }
            bool found = false;
            // Exclude base network from upgrades
            for (auto j = Consensus::BASE_NETWORK + 1; j < Consensus::MAX_NETWORK_UPGRADES; ++j) {
                if (vDeploymentParams[0] == NetworkUpgradeInfo[j].strName) {
                    UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex(j), nActivationHeight);
                    found = true;
                    LogPrintf("Setting network upgrade activation parameters for %s to height=%d\n", vDeploymentParams[0], nActivationHeight);
                    break;
                }
            }
            if (!found) {
                return UIError(strprintf(_("Invalid network upgrade (%s)"), vDeploymentParams[0]));
            }
        }
    }
    return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

static std::string ResolveErrMsg(const char * const optname, const std::string& strBind)
{
    return strprintf(_("Cannot resolve -%s address: '%s'"), optname, strBind);
}

<<<<<<< HEAD
static std::string AmountErrMsg(const char * const optname, const std::string& strValue)
{
    return strprintf(_("Invalid amount for -%s=<amount>: '%s'"), optname, strValue);
}

void InitLogging()
{
    fPrintToConsole = GetBoolArg("-printtoconsole", false);
    fLogTimestamps = GetBoolArg("-logtimestamps", true);
    fLogIPs = GetBoolArg("-logips", false);

    LogPrintf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    LogPrintf("PRCY version %s (%s)\n", FormatFullVersion(), CLIENT_DATE);
}

/** Initialize prcy.
 *  @pre Parameters should be parsed and config file should be read.
 */
bool AppInit2(bool isDaemon)
{
    // ********************************************************* Step 1: setup
    if (!AppInitBasicSetup())
        return false;

    // ********************************************************* Step 2: parameter interactions
#ifdef ENABLE_WALLET
    if (mapArgs.count("-reservebalance")) {
        if (!ParseMoney(mapArgs["-reservebalance"], nReserveBalance)) {
            InitError(_("Invalid amount for -reservebalance=<amount>"));
            return false;
        }
    }
#endif

    // Make sure enough file descriptors are available
    int nBind = std::max((int)mapArgs.count("-bind") + (int)mapArgs.count("-whitebind"), 1);
    nMaxConnections = GetArg("-maxconnections", DEFAULT_MAX_PEER_CONNECTIONS);
    nMaxConnections = std::max(std::min(nMaxConnections, (int)(FD_SETSIZE - nBind - MIN_CORE_FILEDESCRIPTORS)), 0);
    int nFD = RaiseFileDescriptorLimit(nMaxConnections + MIN_CORE_FILEDESCRIPTORS);
    if (nFD < MIN_CORE_FILEDESCRIPTORS)
        return InitError(_("Not enough file descriptors available."));
    if (nFD - MIN_CORE_FILEDESCRIPTORS < nMaxConnections)
        nMaxConnections = nFD - MIN_CORE_FILEDESCRIPTORS;

    // ********************************************************* Step 3: parameter-to-internal-flags

    fDebug = !mapMultiArgs["-debug"].empty();
    // Special-case: if -debug=0/-nodebug is set, turn off debugging messages
    const vector<string>& categories = mapMultiArgs["-debug"];
    if (GetBoolArg("-nodebug", false) || find(categories.begin(), categories.end(), string("0")) != categories.end())
        fDebug = false;

    // Check for -debugnet
    if (GetBoolArg("-debugnet", false))
        InitWarning(_("Warning: Unsupported argument -debugnet ignored, use -debug=net."));
    // Check for -socks - as this is a privacy risk to continue, exit here
    if (mapArgs.count("-socks"))
        return InitError(_("Error: Unsupported argument -socks found. Setting SOCKS version isn't possible anymore, only SOCKS5 proxies are supported."));
    // Check for -tor - as this is a privacy risk to continue, exit here
    if (GetBoolArg("-tor", false))
        return InitError(_("Error: Unsupported argument -tor found, use -onion."));

    if (GetBoolArg("-benchmark", false))
        InitWarning(_("Warning: Unsupported argument -benchmark ignored, use -debug=bench."));

    // Checkmempool and checkblockindex default to true in regtest mode
    mempool.setSanityCheck(GetBoolArg("-checkmempool", Params().DefaultConsistencyChecks()));
    fCheckBlockIndex = GetBoolArg("-checkblockindex", Params().DefaultConsistencyChecks());
    Checkpoints::fEnabled = GetBoolArg("-checkpoints", true);

    // -par=0 means autodetect, but nScriptCheckThreads==0 means no concurrency
    nScriptCheckThreads = GetArg("-par", DEFAULT_SCRIPTCHECK_THREADS);
    if (nScriptCheckThreads <= 0)
        nScriptCheckThreads += boost::thread::hardware_concurrency();
=======
void InitLogging()
{
    g_logger->m_print_to_file = !gArgs.IsArgNegated("-debuglogfile");
    g_logger->m_file_path = AbsPathForConfigVal(gArgs.GetArg("-debuglogfile", DEFAULT_DEBUGLOGFILE));

    // Add newlines to the logfile to distinguish this execution from the last
    // one; called before console logging is set up, so this is only sent to
    // debug.log.
    LogPrintf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

    g_logger->m_print_to_console = gArgs.GetBoolArg("-printtoconsole", !gArgs.GetBoolArg("-daemon", false));
    g_logger->m_log_timestamps = gArgs.GetBoolArg("-logtimestamps", DEFAULT_LOGTIMESTAMPS);
    g_logger->m_log_time_micros = gArgs.GetBoolArg("-logtimemicros", DEFAULT_LOGTIMEMICROS);

    fLogIPs = gArgs.GetBoolArg("-logips", DEFAULT_LOGIPS);

    std::string version_string = FormatFullVersion();
#ifdef DEBUG
    version_string += " (debug build)";
#else
    version_string += " (release build)";
#endif
    LogPrintf("PIVX version %s\n", version_string);
}

bool AppInitParameterInteraction()
{
    // ********************************************************* Step 2: parameter interactions

    if (!fs::is_directory(GetBlocksDir())) {
        return UIError(strprintf(_("Specified blocks directory \"%s\" does not exist."), gArgs.GetArg("-blocksdir", "").c_str()));
    }

    // Make sure enough file descriptors are available

    // -bind and -whitebind can't be set when not listening
    size_t nUserBind =
            (gArgs.IsArgSet("-bind") ? gArgs.GetArgs("-bind").size() : 0) +
            (gArgs.IsArgSet("-whitebind") ? gArgs.GetArgs("-whitebind").size() : 0);
    if (nUserBind != 0 && !gArgs.GetBoolArg("-listen", DEFAULT_LISTEN)) {
        return UIError(strprintf(_("Cannot set %s or %s together with %s"), "-bind", "-whitebind", "-listen=0"));
    }

    // Make sure enough file descriptors are available
    int nBind = std::max(nUserBind, size_t(1));
    nUserMaxConnections = gArgs.GetArg("-maxconnections", DEFAULT_MAX_PEER_CONNECTIONS);
    nMaxConnections = std::max(nUserMaxConnections, 0);

    // Trim requested connection counts, to fit into system limitations
    nMaxConnections = std::max(std::min(nMaxConnections, (int)(FD_SETSIZE - nBind - MIN_CORE_FILEDESCRIPTORS - MAX_ADDNODE_CONNECTIONS)), 0);
    nFD = RaiseFileDescriptorLimit(nMaxConnections + MIN_CORE_FILEDESCRIPTORS + MAX_ADDNODE_CONNECTIONS);
    if (nFD < MIN_CORE_FILEDESCRIPTORS)
        return UIError(_("Not enough file descriptors available."));
    nMaxConnections = std::min(nFD - MIN_CORE_FILEDESCRIPTORS - MAX_ADDNODE_CONNECTIONS, nMaxConnections);

    // ********************************************************* Step 3: parameter-to-internal-flags

    // Special-case: if -debug=0/-nodebug is set, turn off debugging messages
    const std::vector<std::string>& categories = gArgs.GetArgs("-debug");

    if (!(gArgs.GetBoolArg("-nodebug", false) ||
          find(categories.begin(), categories.end(), std::string("0")) != categories.end())) {
        for (const auto& cat : categories) {
            if (!g_logger->EnableCategory(cat)) {
                UIWarning(strprintf(_("Unsupported logging category %s=%s."), "-debug", cat));
            }
        }
    }

    // Now remove the logging categories which were explicitly excluded
    for (const std::string& cat : gArgs.GetArgs("-debugexclude")) {
        if (!g_logger->DisableCategory(cat)) {
            UIWarning(strprintf(_("Unsupported logging category %s=%s."), "-debugexclude", cat));
        }
    }

    // Check for -debugnet
    if (gArgs.GetBoolArg("-debugnet", false))
        UIWarning(strprintf(_("Warning: Unsupported argument %s ignored, use %s."), "-debugnet", "-debug=net"));
    // Check for -socks - as this is a privacy risk to continue, exit here
    if (gArgs.IsArgSet("-socks"))
        return UIError(
                strprintf(_("Error: Unsupported argument %s found. Setting SOCKS version isn't possible anymore, only SOCKS5 proxies are supported."), "-socks"));
    // Check for -tor - as this is a privacy risk to continue, exit here
    if (gArgs.GetBoolArg("-tor", false))
        return UIError(strprintf(_("Error: Unsupported argument %s found, use %s."), "-tor", "-onion"));
    // Check level must be 4 for zerocoin checks
    if (gArgs.IsArgSet("-checklevel"))
        return UIError(strprintf(_("Error: Unsupported argument %s found. Checklevel must be level 4."), "-checklevel"));
    // Exit early if -masternode=1 and -listen=0
    if (gArgs.GetBoolArg("-masternode", DEFAULT_MASTERNODE) && !gArgs.GetBoolArg("-listen", DEFAULT_LISTEN))
        return UIError(strprintf(_("Error: %s must be true if %s is set."), "-listen", "-masternode"));
    if (gArgs.GetBoolArg("-benchmark", false))
        UIWarning(strprintf(_("Warning: Unsupported argument %s ignored, use %s"), "-benchmark", "-debug=bench."));

    // Checkmempool and checkblockindex default to true in regtest mode
    int ratio = std::min<int>(
            std::max<int>(gArgs.GetArg("-checkmempool", Params().DefaultConsistencyChecks() ? 1 : 0), 0), 1000000);
    if (ratio != 0) {
        mempool.setSanityCheck(1.0 / ratio);
    }
    fCheckBlockIndex = gArgs.GetBoolArg("-checkblockindex", Params().DefaultConsistencyChecks());
    Checkpoints::fEnabled = gArgs.GetBoolArg("-checkpoints", DEFAULT_CHECKPOINTS_ENABLED);

    // -mempoollimit limits
    int64_t nMempoolSizeLimit = gArgs.GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000;
    int64_t nMempoolDescendantSizeLimit = gArgs.GetArg("-limitdescendantsize", DEFAULT_DESCENDANT_SIZE_LIMIT) * 1000;
    if (nMempoolSizeLimit < 0 || nMempoolSizeLimit < nMempoolDescendantSizeLimit * 40)
        return UIError(strprintf(_("Error: %s must be at least %d MB"), "-maxmempool",
                                 gArgs.GetArg("-limitdescendantsize", DEFAULT_DESCENDANT_SIZE_LIMIT) / 25));

    // -par=0 means autodetect, but nScriptCheckThreads==0 means no concurrency
    nScriptCheckThreads = gArgs.GetArg("-par", DEFAULT_SCRIPTCHECK_THREADS);
    if (nScriptCheckThreads <= 0)
        nScriptCheckThreads += GetNumCores();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (nScriptCheckThreads <= 1)
        nScriptCheckThreads = 0;
    else if (nScriptCheckThreads > MAX_SCRIPTCHECK_THREADS)
        nScriptCheckThreads = MAX_SCRIPTCHECK_THREADS;

    setvbuf(stdout, NULL, _IOLBF, 0); /// ***TODO*** do we still need this after -printtoconsole is gone?

<<<<<<< HEAD
    // Staking needs a CWallet instance, so make sure wallet is enabled
#ifdef ENABLE_WALLET
    bool fDisableWallet = GetBoolArg("-disablewallet", false);
    if (fDisableWallet) {
        if (SoftSetBoolArg("-staking", false))
            LogPrintf("%s : parameter interaction: wallet functionality not enabled -> setting -staking=0\n", __func__);
    }
#endif

    nConnectTimeout = GetArg("-timeout", DEFAULT_CONNECT_TIMEOUT);
    if (nConnectTimeout <= 0)
        nConnectTimeout = DEFAULT_CONNECT_TIMEOUT;

    // Fee-per-kilobyte amount considered the same as "free"
    // If you are mining, be careful setting this:
    // if you set it to zero then
    // a transaction spammer can cheaply fill blocks using
    // 1-satoshi-fee transactions. It should be set above the real
    // cost to you of processing a transaction.
    if (mapArgs.count("-minrelaytxfee")) {
        CAmount n = 0;
        if (ParseMoney(mapArgs["-minrelaytxfee"], n) && n > 0)
            ::minRelayTxFee = CFeeRate(n);
        else
            return InitError(AmountErrMsg("minrelaytxfee", mapArgs["-minrelaytxfee"]));
    }

#ifdef ENABLE_WALLET
    if (mapArgs.count("-mintxfee")) {
        CAmount n = 0;
        if (ParseMoney(mapArgs["-mintxfee"], n) && n > 0)
            CWallet::minTxFee = CFeeRate(n);
        else
            return InitError(AmountErrMsg("mintxfee", mapArgs["-mintxfee"]));
    }
    if (mapArgs.count("-paytxfee")) {
        CAmount nFeePerK = 0;
        if (!ParseMoney(mapArgs["-paytxfee"], nFeePerK))
            return InitError(AmountErrMsg("paytxfee", mapArgs["-paytxfee"]));
        if (nFeePerK > nHighTransactionFeeWarning)
            InitWarning(_("Warning: -paytxfee is set very high! This is the transaction fee you will pay if you send a transaction."));
        payTxFee = CFeeRate(nFeePerK, 1000);
        if (payTxFee < ::minRelayTxFee) {
            return InitError(strprintf(_("Invalid amount for -paytxfee=<amount>: '%s' (must be at least %s)"),
                mapArgs["-paytxfee"], ::minRelayTxFee.ToString()));
        }
    }
    if (mapArgs.count("-maxtxfee")) {
        CAmount nMaxFee = 0;
        if (!ParseMoney(mapArgs["-maxtxfee"], nMaxFee))
            return InitError(AmountErrMsg("maxtxfee", mapArgs["-maxtxfee"]));
        if (nMaxFee > nHighTransactionMaxFeeWarning)
            InitWarning(_("Warning: -maxtxfee is set very high! Fees this large could be paid on a single transaction."));
        maxTxFee = nMaxFee;
        if (CFeeRate(maxTxFee, 1000) < ::minRelayTxFee) {
            return InitError(strprintf(_("Invalid amount for -maxtxfee=<amount>: '%s' (must be at least the minrelay fee of %s to prevent stuck transactions)"),
                mapArgs["-maxtxfee"], ::minRelayTxFee.ToString()));
        }
    }
    nTxConfirmTarget = GetArg("-txconfirmtarget", 1);
    bSpendZeroConfChange = GetBoolArg("-spendzeroconfchange", false);
    bdisableSystemnotifications = GetBoolArg("-disablesystemnotifications", false);
    fSendFreeTransactions = GetBoolArg("-sendfreetransactions", false);

    std::string strWalletFile = GetArg("-wallet", "wallet.dat");
#endif // ENABLE_WALLET

    fIsBareMultisigStd = GetBoolArg("-permitbaremultisig", true) != 0;
    nMaxDatacarrierBytes = GetArg("-datacarriersize", nMaxDatacarrierBytes);


    if (GetBoolArg("-peerbloomfilters", DEFAULT_PEERBLOOMFILTERS))
        nLocalServices |= NODE_BLOOM;

    nMaxTipAge = GetArg("-maxtipage", DEFAULT_MAX_TIP_AGE);

    // ********************************************************* Step 4: application initialization: dir lock, daemonize, pidfile, debug log

    RandomInit();

    // Sanity check
    if (!InitSanityCheck())
        return InitError(_("Initialization sanity check failed. PRCY is shutting down."));

    std::string strDataDir = GetDataDir().string();
#ifdef ENABLE_WALLET
    // Wallet file must be a plain filename without a directory
    if (strWalletFile != boost::filesystem::basename(strWalletFile) + boost::filesystem::extension(strWalletFile))
        return InitError(strprintf(_("Wallet %s resides outside data directory %s"), strWalletFile, strDataDir));
#endif
    // Make sure only a single PRCY process is using the data directory.
    boost::filesystem::path pathLockFile = GetDataDir() / ".lock";
    FILE* file = fopen(pathLockFile.string().c_str(), "a"); // empty lock file; created if it doesn't exist.
    if (file) fclose(file);
    static boost::interprocess::file_lock lock(pathLockFile.string().c_str());

    // Wait maximum 10 seconds if an old wallet is still running. Avoids lockup during restart
    if (!lock.timed_lock(boost::get_system_time() + boost::posix_time::seconds(10)))
        return InitError(strprintf(_("Cannot obtain a lock on data directory %s. PRCY is probably already running."), strDataDir));
=======
#ifndef ENABLE_WALLET
    if (gArgs.SoftSetBoolArg("-staking", false))
        LogPrintf("AppInit2 : parameter interaction: wallet functionality not enabled -> setting -staking=0\n");
#endif

    nConnectTimeout = gArgs.GetArg("-timeout", DEFAULT_CONNECT_TIMEOUT);
    if (nConnectTimeout <= 0)
        nConnectTimeout = DEFAULT_CONNECT_TIMEOUT;

    // Fee-per-kilobyte amount required for mempool acceptance and relay
    // If you are mining, be careful setting this:
    // if you set it to zero then
    // a transaction spammer can cheaply fill blocks using
    // 0-fee transactions. It should be set above the real
    // cost to you of processing a transaction.
    if (gArgs.IsArgSet("-minrelaytxfee")) {
        CAmount n = 0;
        if (!ParseMoney(gArgs.GetArg("-minrelaytxfee", ""), n)) {
            return UIError(AmountErrMsg("minrelaytxfee", gArgs.GetArg("-minrelaytxfee", "")));
        }
        // High fee check is done afterward in CWallet::ParameterInteraction()
        ::minRelayTxFee = CFeeRate(n);
    }

    const CChainParams& chainparams = Params();
    fRequireStandard = !gArgs.GetBoolArg("-acceptnonstdtxn", !chainparams.RequireStandard());
    if (!chainparams.IsTestChain() && !fRequireStandard)
        return UIError(strprintf("%s is not currently supported for %s chain", "-acceptnonstdtxn", chainparams.NetworkIDString()));

    // Feerate used to define dust.  Shouldn't be changed lightly as old
    // implementations may inadvertently create non-standard transactions
    if (gArgs.IsArgSet("-dustrelayfee")) {
        CAmount n = 0;
        if (!ParseMoney(gArgs.GetArg("-dustrelayfee", ""), n) || 0 == n)
            return UIError(AmountErrMsg("dustrelayfee", gArgs.GetArg("-dustrelayfee", "")));
        dustRelayFee = CFeeRate(n);
    }

#ifdef ENABLE_WALLET
    if (!WalletParameterInteraction())
        return false;
#endif // ENABLE_WALLET

    fIsBareMultisigStd = gArgs.GetBoolArg("-permitbaremultisig", DEFAULT_PERMIT_BAREMULTISIG);
    nMaxDatacarrierBytes = gArgs.GetArg("-datacarriersize", nMaxDatacarrierBytes);

    // Option to startup with mocktime set (used for regression testing):
    if (Params().IsRegTestNet()) {
        SetMockTime(gArgs.GetArg("-mocktime", 0)); // SetMockTime(0) is a no-op
    }

    if (gArgs.GetBoolArg("-peerbloomfilters", DEFAULT_PEERBLOOMFILTERS))
        nLocalServices = ServiceFlags(nLocalServices | NODE_BLOOM);

    nMaxTipAge = gArgs.GetArg("-maxtipage", DEFAULT_MAX_TIP_AGE);

    if (!InitNUParams())
        return false;

    return true;
}

static bool LockDataDirectory(bool probeOnly)
{
    // Make sure only a single PIVX process is using the data directory.
    fs::path datadir = GetDataDir();
    if (!DirIsWritable(datadir)) {
        return UIError(strprintf(_("Cannot write to data directory '%s'; check permissions."), datadir.string()));
    }
    if (!LockDirectory(datadir, ".lock", probeOnly)) {
        return UIError(strprintf(_("Cannot obtain a lock on data directory %s. %s is probably already running."), datadir.string(), PACKAGE_NAME));
    }
    return true;
}

bool AppInitSanityChecks()
{
    // ********************************************************* Step 4: sanity checks

    // Initialize elliptic curve code
    RandomInit();
    ECC_Start();
    globalVerifyHandle.reset(new ECCVerifyHandle());

    // Sanity check
    if (!InitSanityCheck())
        return UIError(strprintf(_("Initialization sanity check failed. %s is shutting down."), PACKAGE_NAME));

    // Probe the data directory lock to give an early error message, if possible
    return LockDataDirectory(true);
}

bool AppInitMain()
{
    // ********************************************************* Step 4a: application initialization
    // After daemonization get the data directory lock again and hold on to it until exit
    // This creates a slight window for a race condition to happen, however this condition is harmless: it
    // will at most make us exit without printing a message to console.
    if (!LockDataDirectory(false)) {
        // Detailed error printed inside LockDataDirectory
        return false;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifndef WIN32
    CreatePidFile(GetPidFile(), getpid());
#endif
<<<<<<< HEAD
    if (GetBoolArg("-shrinkdebugfile", !fDebug))
        ShrinkDebugFile();

    LogPrintf("Using OpenSSL version %s\n", SSLeay_version(SSLEAY_VERSION));
#ifdef ENABLE_WALLET
    LogPrintf("Using BerkeleyDB version %s\n", DbEnv::version(0, 0, 0));
#endif
    if (!fLogTimestamps)
        LogPrintf("Startup time: %s\n", DateTimeStrFormat("%Y-%m-%d %H:%M:%S", GetTime()));
    LogPrintf("Default data directory %s\n", GetDefaultDataDir().string());
    LogPrintf("Using data directory %s\n", strDataDir);
    LogPrintf("Using config file %s\n", GetConfigFile().string());
    LogPrintf("Using at most %i connections (%i file descriptors available)\n", nMaxConnections, nFD);
    std::ostringstream strErrors;

=======
    if (g_logger->m_print_to_file) {
        if (gArgs.GetBoolArg("-shrinkdebugfile", g_logger->DefaultShrinkDebugFile()))
            g_logger->ShrinkDebugFile();
        if (!g_logger->OpenDebugLog())
            return UIError(strprintf(_("Could not open debug log file %s"), g_logger->m_file_path.string()));
    }
#ifdef ENABLE_WALLET
    LogPrintf("Using BerkeleyDB version %s\n", DbEnv::version(0, 0, 0));
#endif
    if (!g_logger->m_log_timestamps)
        LogPrintf("Startup time: %s\n", FormatISO8601DateTime(GetTime()));
    LogPrintf("Default data directory %s\n", GetDefaultDataDir().string());
    LogPrintf("Using data directory %s\n", GetDataDir().string());
    LogPrintf("Using config file %s\n", GetConfigFile(gArgs.GetArg("-conf", PIVX_CONF_FILENAME)).string());
    LogPrintf("Using at most %i automatic connections (%i file descriptors available)\n", nMaxConnections, nFD);
    std::ostringstream strErrors;

    // Warn about relative -datadir path.
    if (gArgs.IsArgSet("-datadir") && !fs::path(gArgs.GetArg("-datadir", "")).is_absolute()) {
        LogPrintf("Warning: relative datadir option '%s' specified, which will be interpreted relative to the "
                  "current working directory '%s'. This is fragile because if PIVX is started in the future "
                  "from a different location. It will be unable to locate the current data files. There could "
                  "also be data loss if PIVX is started while in a temporary directory.\n",
            gArgs.GetArg("-datadir", ""), fs::current_path().string());
    }

    InitSignatureCache();

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LogPrintf("Using %u threads for script verification\n", nScriptCheckThreads);
    if (nScriptCheckThreads) {
        for (int i = 0; i < nScriptCheckThreads - 1; i++)
            threadGroup.create_thread(&ThreadScriptCheck);
    }

<<<<<<< HEAD
    // Start the lightweight task scheduler thread
    CScheduler::Function serviceLoop = boost::bind(&CScheduler::serviceQueue, &scheduler);
    threadGroup.create_thread(boost::bind(&TraceThread<CScheduler::Function>, "scheduler", serviceLoop));
=======
    if (gArgs.IsArgSet("-sporkkey")) // spork priv key
    {
        if (!sporkManager.SetPrivKey(gArgs.GetArg("-sporkkey", "")))
            return UIError(_("Unable to sign spork message, wrong key?"));
    }

    // Start the lightweight task scheduler thread
    CScheduler::Function serviceLoop = std::bind(&CScheduler::serviceQueue, &scheduler);
    threadGroup.create_thread(std::bind(&TraceThread<CScheduler::Function>, "scheduler", serviceLoop));

    // Gather some entropy once per minute.
    scheduler.scheduleEvery([]{
        RandAddPeriodic();
    }, 60000);

    GetMainSignals().RegisterBackgroundSignalScheduler(scheduler);

    // Initialize Sapling circuit parameters
    LoadSaplingParams();

    /* Register RPC commands regardless of -server setting so they will be
     * available in the GUI RPC console even if external calls are disabled.
     */
    RegisterAllCoreRPCCommands(tableRPC);
#ifdef ENABLE_WALLET
    RegisterWalletRPCCommands(tableRPC);
#endif
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /* Start the RPC server already.  It will be started in "warmup" mode
     * and not really process calls already (but it will signify connections
     * that the server is there and will be ready later).  Warmup mode will
     * be disabled when initialisation is finished.
     */
<<<<<<< HEAD
    if (GetBoolArg("-server", false)) {
        uiInterface.InitMessage.connect(SetRPCWarmupStatus);
        if (!AppInitServers())
            return InitError(_("Unable to start HTTP server. See debug log for details."));
    }

// ********************************************************* Step 5: Backup wallet and verify wallet database integrity
#ifdef ENABLE_WALLET
    if (!fDisableWallet) {
        filesystem::path backupDir = GetDataDir() / "backups";
        if (!filesystem::exists(backupDir)) {
            // Always create backup folder to not confuse the operating system's file browser
            filesystem::create_directories(backupDir);
        }
        nWalletBackups = GetArg("-createwalletbackups", 10);
        nWalletBackups = std::max(0, std::min(10, nWalletBackups));
        if (nWalletBackups > 0) {
            if (filesystem::exists(backupDir)) {
                // Create backup of the wallet
                std::string dateTimeStr = DateTimeStrFormat(".%Y-%m-%d-%H-%M", GetTime());
                std::string backupPathStr = backupDir.string();
                backupPathStr += "/" + strWalletFile;
                std::string sourcePathStr = GetDataDir().string();
                sourcePathStr += "/" + strWalletFile;
                boost::filesystem::path sourceFile = sourcePathStr;
                boost::filesystem::path backupFile = backupPathStr + dateTimeStr;
                sourceFile.make_preferred();
                backupFile.make_preferred();
                if (boost::filesystem::exists(sourceFile)) {
#if BOOST_VERSION >= 105800
                    try {
                        boost::filesystem::copy_file(sourceFile, backupFile);
                        LogPrintf("Creating backup of %s -> %s\n", sourceFile, backupFile);
                    } catch (const boost::filesystem::filesystem_error& error) {
                        LogPrintf("Failed to create backup %s\n", error.what());
                    }
#else
                    std::ifstream src(sourceFile.string(), std::ios::binary);
                    std::ofstream dst(backupFile.string(), std::ios::binary);
                    dst << src.rdbuf();
#endif
                }
                // Keep only the last 10 backups, including the new one of course
                typedef std::multimap<std::time_t, boost::filesystem::path> folder_set_t;
                folder_set_t folder_set;
                boost::filesystem::directory_iterator end_iter;
                boost::filesystem::path backupFolder = backupDir.string();
                backupFolder.make_preferred();
                // Build map of backup files for current(!) wallet sorted by last write time
                boost::filesystem::path currentFile;
                for (boost::filesystem::directory_iterator dir_iter(backupFolder); dir_iter != end_iter; ++dir_iter) {
                    // Only check regular files
                    if (boost::filesystem::is_regular_file(dir_iter->status())) {
                        currentFile = dir_iter->path().filename();
                        // Only add the backups for the current wallet, e.g. wallet.dat.*
                        if (dir_iter->path().stem().string() == strWalletFile) {
                            folder_set.insert(folder_set_t::value_type(boost::filesystem::last_write_time(dir_iter->path()), *dir_iter));
                        }
                    }
                }
                // Loop backward through backup files and keep the N newest ones (1 <= N <= 10)
                int counter = 0;
                BOOST_REVERSE_FOREACH (PAIRTYPE(const std::time_t, boost::filesystem::path) file, folder_set) {
                    counter++;
                    if (counter > nWalletBackups) {
                        // More than nWalletBackups backups: delete oldest one(s)
                        try {
                            boost::filesystem::remove(file.second);
                            LogPrintf("Old backup deleted: %s\n", file.second);
                        } catch (const boost::filesystem::filesystem_error& error) {
                            LogPrintf("Failed to delete backup %s\n", error.what());
                        }
                    }
                }
            }
        }

        if (GetBoolArg("-resync", false)) {
            uiInterface.InitMessage(_("Preparing for resync..."));
            // Delete the local blockchain folders to force a resync from scratch to get a consitent blockchain-state
            filesystem::path blocksDir = GetDataDir() / "blocks";
            filesystem::path chainstateDir = GetDataDir() / "chainstate";

            // We delete in 4 individual steps in case one of the folder is missing already
            try {
                if (filesystem::exists(blocksDir)){
                    boost::filesystem::remove_all(blocksDir);
                    LogPrintf("-resync: folder deleted: %s\n", blocksDir.string().c_str());
                }

                if (filesystem::exists(chainstateDir)){
                    boost::filesystem::remove_all(chainstateDir);
                    LogPrintf("-resync: folder deleted: %s\n", chainstateDir.string().c_str());
                }
            } catch (const boost::filesystem::filesystem_error& error) {
                LogPrintf("Failed to delete blockchain folders %s\n", error.what());
            }
        }

        LogPrintf("Using wallet %s\n", strWalletFile);
        uiInterface.InitMessage(_("Verifying wallet..."));

        if (!bitdb.Open(GetDataDir())) {
            // try moving the database env out of the way
            boost::filesystem::path pathDatabase = GetDataDir() / "database";
            boost::filesystem::path pathDatabaseBak = GetDataDir() / strprintf("database.%d.bak", GetTime());
            try {
                boost::filesystem::rename(pathDatabase, pathDatabaseBak);
                LogPrintf("Moved old %s to %s. Retrying.\n", pathDatabase.string(), pathDatabaseBak.string());
            } catch (const boost::filesystem::filesystem_error& error) {
                // failure is ok (well, not really, but it's not worse than what we started with)
            }

            // try again
            if (!bitdb.Open(GetDataDir())) {
                // if it still fails, it probably means we can't even create the database env
                string msg = strprintf(_("Error initializing wallet database environment %s!"), strDataDir);
                return InitError(msg);
            }
        }

        if (GetBoolArg("-salvagewallet", false)) {
            // Recover readable keypairs:
            if (!CWalletDB::Recover(bitdb, strWalletFile, true))
                return false;
        }

        if (filesystem::exists(GetDataDir() / strWalletFile)) {
            CDBEnv::VerifyResult r = bitdb.Verify(strWalletFile, CWalletDB::Recover);
            if (r == CDBEnv::RECOVER_OK) {
                string msg = strprintf(_("Warning: wallet.dat corrupt, data salvaged!"
                                         " Original wallet.dat saved as wallet.{timestamp}.bak in %s; if"
                                         " your balance or transactions are incorrect you should"
                                         " restore from a backup."),
                    strDataDir);
                InitWarning(msg);
            }
            if (r == CDBEnv::RECOVER_FAIL)
                return InitError(_("wallet.dat corrupt, salvage failed"));
        }

    }  // (!fDisableWallet)
#endif // ENABLE_WALLET
    // ********************************************************* Step 6: network initialization
    //run daemon only
    if (isDaemon)
        RegisterNodeSignals(GetNodeSignals());       // block first after unlock/lock retry register

    // sanitize comments per BIP-0014, format user agent and check total size
    std::vector<std::string> uacomments;
    for (const std::string& cmt : mapMultiArgs["-uacomment"]) {
        if (cmt != SanitizeString(cmt, SAFE_CHARS_UA_COMMENT))
            return InitError(strprintf(_("User Agent comment (%s) contains unsafe characters."), cmt));
=======
    if (gArgs.GetBoolArg("-server", false)) {
        uiInterface.InitMessage.connect(SetRPCWarmupStatus);
        if (!AppInitServers())
            return UIError(_("Unable to start HTTP server. See debug log for details."));
    }

    if (gArgs.GetBoolArg("-resync", false)) {
        uiInterface.InitMessage(_("Preparing for resync..."));
        // Delete the local blockchain folders to force a resync from scratch to get a consitent blockchain-state
        fs::path blocksDir = GetBlocksDir();
        fs::path chainstateDir = GetDataDir() / "chainstate";
        fs::path sporksDir = GetDataDir() / "sporks";
        fs::path zerocoinDir = GetDataDir() / "zerocoin";

        LogPrintf("Deleting blockchain folders blocks, chainstate, sporks and zerocoin\n");
        // We delete in 4 individual steps in case one of the folder is missing already
        try {
            if (fs::exists(blocksDir)){
                fs::remove_all(blocksDir);
                LogPrintf("-resync: folder deleted: %s\n", blocksDir.string().c_str());
            }

            if (fs::exists(chainstateDir)){
                fs::remove_all(chainstateDir);
                LogPrintf("-resync: folder deleted: %s\n", chainstateDir.string().c_str());
            }

            if (fs::exists(sporksDir)){
                fs::remove_all(sporksDir);
                LogPrintf("-resync: folder deleted: %s\n", sporksDir.string().c_str());
            }

            if (fs::exists(zerocoinDir)){
                fs::remove_all(zerocoinDir);
                LogPrintf("-resync: folder deleted: %s\n", zerocoinDir.string().c_str());
            }
        } catch (const fs::filesystem_error& error) {
            LogPrintf("Failed to delete blockchain folders %s\n", error.what());
        }
    }

// ********************************************************* Step 5: Verify wallet database integrity
#ifdef ENABLE_WALLET
    if (!WalletVerify()) {
        return false;
    }
#endif

    // ********************************************************* Step 6: network initialization
    // Note that we absolutely cannot open any actual connections
    // until the very end ("start node") as the UTXO/block state
    // is not yet setup and may end up being set up twice if we
    // need to reindex later.

    assert(!g_connman);
    g_connman = std::make_unique<CConnman>(GetRand(std::numeric_limits<uint64_t>::max()), GetRand(std::numeric_limits<uint64_t>::max()));
    CConnman& connman = *g_connman;

    peerLogic.reset(new PeerLogicValidation(&connman));
    RegisterValidationInterface(peerLogic.get());

    // sanitize comments per BIP-0014, format user agent and check total size
    std::vector<std::string> uacomments;
    for (const std::string& cmt : gArgs.GetArgs("-uacomment")) {
        if (cmt != SanitizeString(cmt, SAFE_CHARS_UA_COMMENT))
            return UIError(strprintf(_("User Agent comment (%s) contains unsafe characters."), cmt));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        uacomments.push_back(cmt);
    }

    // format user agent, check total size
    strSubVersion = FormatSubVersion(CLIENT_NAME, CLIENT_VERSION, uacomments);
    if (strSubVersion.size() > MAX_SUBVERSION_LENGTH) {
<<<<<<< HEAD
        return InitError(strprintf(_("Total length of network version string (%i) exceeds maximum length (%i). Reduce the number or size of uacomments."),
            strSubVersion.size(), MAX_SUBVERSION_LENGTH));
    }

    if (mapArgs.count("-onlynet")) {
        std::set<enum Network> nets;
        for (std::string snet : mapMultiArgs["-onlynet"]) {
            enum Network net = ParseNetwork(snet);
            if (net == NET_UNROUTABLE)
                return InitError(strprintf(_("Unknown network specified in -onlynet: '%s'"), snet));
=======
        return UIError(strprintf(_("Total length of network version string (%i) exceeds maximum length (%i). Reduce the number or size of %s."),
            strSubVersion.size(), MAX_SUBVERSION_LENGTH, "-uacomment"));
    }

    if (gArgs.IsArgSet("-onlynet")) {
        std::set<enum Network> nets;
        for (const std::string& snet : gArgs.GetArgs("-onlynet")) {
            enum Network net = ParseNetwork(snet);
            if (net == NET_UNROUTABLE)
                return UIError(strprintf(_("Unknown network specified in %s: '%s'"), "-onlynet", snet));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            nets.insert(net);
        }
        for (int n = 0; n < NET_MAX; n++) {
            enum Network net = (enum Network)n;
            if (!nets.count(net))
<<<<<<< HEAD
                SetLimited(net);
        }
    }

    if (mapArgs.count("-whitelist")) {
        for (const std::string& net : mapMultiArgs["-whitelist"]) {
            CSubNet subnet(net);
            if (!subnet.IsValid())
                return InitError(strprintf(_("Invalid netmask specified in -whitelist: '%s'"), net));
            CNode::AddWhitelistedRange(subnet);
=======
                SetReachable(net, false);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    // Check for host lookup allowed before parsing any network related parameters
<<<<<<< HEAD
    fNameLookup = GetBoolArg("-dns", DEFAULT_NAME_LOOKUP);

    bool proxyRandomize = GetBoolArg("-proxyrandomize", true);
    // -proxy sets a proxy for all outgoing network traffic
    // -noproxy (or -proxy=0) as well as the empty string can be used to not set a proxy, this is the default
    std::string proxyArg = GetArg("-proxy", "");
    SetLimited(NET_TOR);
    if (proxyArg != "" && proxyArg != "0") {
        CService proxyAddr;
        if (!Lookup(proxyArg.c_str(), proxyAddr, 9050, fNameLookup)) {
            return InitError(strprintf(_("Lookup(): Invalid -proxy address or hostname: '%s'"), proxyArg));
=======
    fNameLookup = gArgs.GetBoolArg("-dns", DEFAULT_NAME_LOOKUP);

    bool proxyRandomize = gArgs.GetBoolArg("-proxyrandomize", DEFAULT_PROXYRANDOMIZE);
    // -proxy sets a proxy for all outgoing network traffic
    // -noproxy (or -proxy=0) as well as the empty string can be used to not set a proxy, this is the default
    std::string proxyArg = gArgs.GetArg("-proxy", "");
    SetReachable(NET_ONION, false);
    if (!proxyArg.empty() && proxyArg != "0") {
        CService proxyAddr;
        if (!Lookup(proxyArg, proxyAddr, 9050, fNameLookup)) {
            return UIError(strprintf(_("%s Invalid %s address or hostname: '%s'"), "Lookup():", "-proxy", proxyArg));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        proxyType addrProxy = proxyType(proxyAddr, proxyRandomize);
        if (!addrProxy.IsValid())
<<<<<<< HEAD
            return InitError(strprintf(_("isValid(): Invalid -proxy address or hostname: '%s'"), proxyArg));

        SetProxy(NET_IPV4, addrProxy);
        SetProxy(NET_IPV6, addrProxy);
        SetProxy(NET_TOR, addrProxy);
        SetNameProxy(addrProxy);
        SetLimited(NET_TOR, false); // by default, -proxy sets onion as reachable, unless -noonion later
=======
            return UIError(strprintf(_("%s Invalid %s address or hostname: '%s'"), "isValid():", "-proxy", proxyArg));

        SetProxy(NET_IPV4, addrProxy);
        SetProxy(NET_IPV6, addrProxy);
        SetProxy(NET_ONION, addrProxy);
        SetNameProxy(addrProxy);
        SetReachable(NET_ONION, true); // by default, -proxy sets onion as reachable, unless -noonion later
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // -onion can be used to set only a proxy for .onion, or override normal proxy for .onion addresses
    // -noonion (or -onion=0) disables connecting to .onion entirely
    // An empty string is used to not override the onion proxy (in which case it defaults to -proxy set above, or none)
<<<<<<< HEAD
    std::string onionArg = GetArg("-onion", "");
    if (onionArg != "") {
        if (onionArg == "0") { // Handle -noonion/-onion=0
            SetLimited(NET_TOR); // set onions as unreachable
        } else {
            CService onionProxy;
            if (!Lookup(onionArg.c_str(), onionProxy, 9050, fNameLookup)) {
                return InitError(strprintf(_("Invalid -onion address or hostname: '%s'"), onionArg));
            }
            proxyType addrOnion = proxyType(onionProxy, proxyRandomize);
            if (!addrOnion.IsValid())
                return InitError(strprintf(_("Invalid -onion address or hostname: '%s'"), onionArg));
            SetProxy(NET_TOR, addrOnion);
            SetLimited(NET_TOR, false);
=======
    std::string onionArg = gArgs.GetArg("-onion", "");
    if (!onionArg.empty()) {
        if (onionArg == "0") { // Handle -noonion/-onion=0
            SetReachable(NET_ONION, false);
        } else {
            CService onionProxy;
            if (!Lookup(onionArg, onionProxy, 9050, fNameLookup)) {
                return UIError(strprintf(_("%s Invalid %s address or hostname: '%s'"), "Lookup():", "-onion", onionArg));
            }
            proxyType addrOnion = proxyType(onionProxy, proxyRandomize);
            if (!addrOnion.IsValid())
                return UIError(strprintf(_("%s Invalid %s address or hostname: '%s'"), "isValid():", "-onion", onionArg));
            SetProxy(NET_ONION, addrOnion);
            SetReachable(NET_ONION, true);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    // see Step 2: parameter interactions for more information about these
<<<<<<< HEAD
    fListen = GetBoolArg("-listen", DEFAULT_LISTEN);
    fDiscover = GetBoolArg("-discover", true);

    bool fBound = false;
    if (fListen) {
        if (mapArgs.count("-bind") || mapArgs.count("-whitebind")) {
            for (std::string strBind : mapMultiArgs["-bind"]) {
                CService addrBind;
                if (!Lookup(strBind.c_str(), addrBind, GetListenPort(), false))
                    return InitError(ResolveErrMsg("bind", strBind));
                fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR));
            }
            for (std::string strBind : mapMultiArgs["-whitebind"]) {
                CService addrBind;
                if (!Lookup(strBind.c_str(), addrBind, 0, false))
                    return InitError(ResolveErrMsg("whitebind", strBind));
                if (addrBind.GetPort() == 0)
                    return InitError(strprintf(_("Need to specify a port with -whitebind: '%s'"), strBind));
                fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR | BF_WHITELIST));
            }
        } else {
            struct in_addr inaddr_any;
            inaddr_any.s_addr = INADDR_ANY;
            fBound |= Bind(CService((in6_addr)IN6ADDR_ANY_INIT, GetListenPort()), BF_NONE);
            fBound |= Bind(CService(inaddr_any, GetListenPort()), !fBound ? BF_REPORT_ERROR : BF_NONE);
        }
        if (!fBound)
            return InitError(_("Failed to listen on any port. Use -listen=0 if you want this."));
    }

    if (mapArgs.count("-externalip")) {
        for (string strAddr : mapMultiArgs["-externalip"]) {
            CService addrLocal;
            if (Lookup(strAddr.c_str(), addrLocal, GetListenPort(), fNameLookup) && addrLocal.IsValid())
                AddLocal(addrLocal,LOCAL_MANUAL);
            else
                return InitError(ResolveErrMsg("externalip", strAddr));
        }
    }

    for (string strDest : mapMultiArgs["-seednode"])
        AddOneShot(strDest);

#if ENABLE_ZMQ
    pzmqNotificationInterface = CZMQNotificationInterface::CreateWithArguments(mapArgs);
=======
    fListen = gArgs.GetBoolArg("-listen", DEFAULT_LISTEN);
    fDiscover = gArgs.GetBoolArg("-discover", true);

    for (const std::string& strAddr : gArgs.GetArgs("-externalip")) {
        CService addrLocal;
        if (Lookup(strAddr, addrLocal, GetListenPort(), fNameLookup) && addrLocal.IsValid())
            AddLocal(addrLocal, LOCAL_MANUAL);
        else
            return UIError(ResolveErrMsg("externalip", strAddr));
    }

    // Read asmap file if configured
    if (gArgs.IsArgSet("-asmap")) {
        fs::path asmap_path = fs::path(gArgs.GetArg("-asmap", ""));
        if (asmap_path.empty()) {
            asmap_path = DEFAULT_ASMAP_FILENAME;
        }
        if (!asmap_path.is_absolute()) {
            asmap_path = GetDataDir() / asmap_path;
        }
        if (!fs::exists(asmap_path)) {
            UIError(strprintf(_("Could not find asmap file %s"), asmap_path));
            return false;
        }
        std::vector<bool> asmap = CAddrMan::DecodeAsmap(asmap_path);
        if (asmap.size() == 0) {
            UIError(strprintf(_("Could not parse asmap file %s"), asmap_path));
            return false;
        }
        const uint256 asmap_version = SerializeHash(asmap);
        connman.SetAsmap(std::move(asmap));
        LogPrintf("Using asmap version %s for IP bucketing\n", asmap_version.ToString());
    } else {
        LogPrintf("Using /16 prefix for IP bucketing\n");
    }

    // Warn if network-specific options (-addnode, -connect, etc) are
    // specified in default section of config file, but not overridden
    // on the command line or in this network's section of the config file.
    gArgs.WarnForSectionOnlyArgs();

#if ENABLE_ZMQ
    pzmqNotificationInterface = CZMQNotificationInterface::Create();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (pzmqNotificationInterface) {
        RegisterValidationInterface(pzmqNotificationInterface);
    }
#endif

<<<<<<< HEAD
    // ********************************************************* Step 7: load block chain

    fReindex = GetBoolArg("-reindex", false);

    // Create blocks directory if it doesn't already exist
    boost::filesystem::create_directories(GetDataDir() / "blocks");

    // cache size calculations
    size_t nTotalCache = (GetArg("-dbcache", nDefaultDbCache) << 20);
    if (nTotalCache < (nMinDbCache << 20))
        nTotalCache = (nMinDbCache << 20); // total cache cannot be less than nMinDbCache
    else if (nTotalCache > (nMaxDbCache << 20))
        nTotalCache = (nMaxDbCache << 20); // total cache cannot be greater than nMaxDbCache
    size_t nBlockTreeDBCache = nTotalCache / 8;
    if (nBlockTreeDBCache > (1 << 21) && !GetBoolArg("-txindex", true))
        nBlockTreeDBCache = (1 << 21); // block tree db cache shouldn't be larger than 2 MiB
    nTotalCache -= nBlockTreeDBCache;
    size_t nCoinDBCache = nTotalCache / 2; // use half of the remaining cache for coindb cache
    nTotalCache -= nCoinDBCache;
    nCoinCacheSize = nTotalCache / 300; // coins in memory require around 300 bytes
=======
    pEvoNotificationInterface = new EvoNotificationInterface();
    RegisterValidationInterface(pEvoNotificationInterface);

    // ********************************************************* Step 7: load block chain

    fReindex = gArgs.GetBoolArg("-reindex", false);
    bool fReindexChainState = gArgs.GetBoolArg("-reindex-chainstate", false);

    // cache size calculations
    int64_t nTotalCache = (gArgs.GetArg("-dbcache", nDefaultDbCache) << 20);
    nTotalCache = std::max(nTotalCache, nMinDbCache << 20); // total cache cannot be less than nMinDbCache
    nTotalCache = std::min(nTotalCache, nMaxDbCache << 20); // total cache cannot be greater than nMaxDbcache
    int64_t nBlockTreeDBCache = nTotalCache / 8;
    nBlockTreeDBCache = std::min(nBlockTreeDBCache, (gArgs.GetBoolArg("-txindex", DEFAULT_TXINDEX) ? nMaxBlockDBAndTxIndexCache : nMaxBlockDBCache) << 20);
    nTotalCache -= nBlockTreeDBCache;
    int64_t nCoinDBCache = std::min(nTotalCache / 2, (nTotalCache / 4) + (1 << 23)); // use 25%-50% of the remainder for disk cache
    nCoinDBCache = std::min(nCoinDBCache, nMaxCoinsDBCache << 20); // cap total coins db cache
    nTotalCache -= nCoinDBCache;
    nCoinCacheUsage = nTotalCache; // the rest goes to in-memory cache
    int64_t nEvoDbCache = 1024 * 1024 * 16; // TODO
    LogPrintf("Cache configuration:\n");
    LogPrintf("* Using %.1fMiB for block index database\n", nBlockTreeDBCache * (1.0 / 1024 / 1024));
    LogPrintf("* Using %.1fMiB for chain state database\n", nCoinDBCache * (1.0 / 1024 / 1024));
    LogPrintf("* Using %.1fMiB for in-memory UTXO set\n", nCoinCacheUsage * (1.0 / 1024 / 1024));

    const CChainParams& chainparams = Params();
    const Consensus::Params& consensus = chainparams.GetConsensus();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    bool fLoaded = false;
    while (!fLoaded && !ShutdownRequested()) {
        bool fReset = fReindex;
        std::string strLoadError;

<<<<<<< HEAD
        uiInterface.InitMessage(_("Loading block index..."));

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        do {
            const int64_t load_block_index_start_time = GetTimeMillis();

            try {
                UnloadBlockIndex();
<<<<<<< HEAD
                delete pcoinsTip;
                delete pcoinsdbview;
                delete pcoinscatcher;
                delete pblocktree;

                pblocktree = new CBlockTreeDB(nBlockTreeDBCache, false, fReindex);
                pcoinsdbview = new CCoinsViewDB(nCoinDBCache, false, fReindex);
                pcoinscatcher = new CCoinsViewErrorCatcher(pcoinsdbview);
                pcoinsTip = new CCoinsViewCache(pcoinscatcher);

                if (fReindex)
                    pblocktree->WriteReindexing(true);
=======
                pcoinsTip.reset();
                pcoinsdbview.reset();
                pcoinscatcher.reset();
                pblocktree.reset(new CBlockTreeDB(nBlockTreeDBCache, false, fReset));

                //PIVX specific: zerocoin and spork DB's
                zerocoinDB.reset(new CZerocoinDB(0, false, fReindex));
                pSporkDB.reset(new CSporkDB(0, false, false));
                accumulatorCache.reset(new AccumulatorCache(zerocoinDB.get()));

                deterministicMNManager.reset();
                evoDb.reset();
                evoDb.reset(new CEvoDB(nEvoDbCache, false, fReindex));
                deterministicMNManager.reset(new CDeterministicMNManager(*evoDb));

                if (fReset) {
                    pblocktree->WriteReindexing(true);
                }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

                // End loop if shutdown was requested
                if (ShutdownRequested()) break;

<<<<<<< HEAD
                uiInterface.InitMessage(_("Loading block index..."));
                string strBlockIndexError = "";
=======
                // PIVX: load previous sessions sporks if we have them.
                uiInterface.InitMessage(_("Loading sporks..."));
                sporkManager.LoadSporksFromDB();

                // LoadBlockIndex will load fTxIndex from the db, or set it if
                // we're reindexing. It will also load fHavePruned if we've
                // ever removed a block file from disk.
                // Note that it also sets fReindex based on the disk flag!
                // From here on out fReindex and fReset mean something different!
                uiInterface.InitMessage(_("Loading block index..."));
                std::string strBlockIndexError;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                if (!LoadBlockIndex(strBlockIndexError)) {
                    if (ShutdownRequested()) break;
                    strLoadError = _("Error loading block database");
                    strLoadError = strprintf("%s : %s", strLoadError, strBlockIndexError);
                    break;
                }

                // If the loaded chain has a wrong genesis, bail out immediately
                // (we're likely using a testnet datadir, or the other way around).
<<<<<<< HEAD
                if (!mapBlockIndex.empty() && mapBlockIndex.count(Params().HashGenesisBlock()) == 0)
                    return InitError(_("Incorrect or no genesis block found. Wrong datadir for network?"));

                // Initialize the block index (no-op if non-empty database was already loaded)
                if (!InitBlockIndex()) {
=======
                if (!mapBlockIndex.empty() && mapBlockIndex.count(consensus.hashGenesisBlock) == 0)
                    return UIError(_("Incorrect or no genesis block found. Wrong datadir for network?"));

                // Check for changed -txindex state
                if (fTxIndex != gArgs.GetBoolArg("-txindex", DEFAULT_TXINDEX)) {
                    strLoadError = strprintf(_("You need to rebuild the database using %s to change %s"), "-reindex-chainstate", "-txindex");
                    break;
                }

                // At this point blocktree args are consistent with what's on disk.
                // If we're not mid-reindex (based on disk + args), add a genesis block on disk.
                // This is called again in ThreadImport in the reindex completes.
                if (!fReindex && !LoadGenesisBlock()) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    strLoadError = _("Error initializing block database");
                    break;
                }

<<<<<<< HEAD
                // Check for changed -txindex state
                if (fTxIndex != GetBoolArg("-txindex", true)) {
                    strLoadError = _("You need to rebuild the database using -reindex to change -txindex");
                    break;
                }

                // Recalculate money supply for blocks
                if (GetBoolArg("-reindexmoneysupply", false)) {
                    RecalculatePRCYSupply(1);
                }

                if (!fReindex) {
                    uiInterface.InitMessage(_("Verifying blocks..."));

                    // Flag sent to validation code to let it know it can skip certain checks
                    fVerifyingBlocks = true;

                    {
                        LOCK(cs_main);
                        CBlockIndex *tip = chainActive[chainActive.Height()];
=======
                // At this point we're either in reindex or we've loaded a useful
                // block tree into mapBlockIndex!

                pcoinsdbview.reset(new CCoinsViewDB(nCoinDBCache, false, fReset || fReindexChainState));
                pcoinscatcher.reset(new CCoinsViewErrorCatcher(pcoinsdbview.get()));

                // If necessary, upgrade from older database format.
                // This is a no-op if we cleared the coinsviewdb with -reindex or -reindex-chainstate
                uiInterface.InitMessage(_("Upgrading coins database if needed..."));
                // If necessary, upgrade from older database format.
                if (!pcoinsdbview->Upgrade()) {
                    strLoadError = _("Error upgrading chainstate database");
                    break;
                }

                // ReplayBlocks is a no-op if we cleared the coinsviewdb with -reindex or -reindex-chainstate
                if (!ReplayBlocks(chainparams, pcoinsdbview.get())) {
                    strLoadError = strprintf(_("Unable to replay blocks. You will need to rebuild the database using %s."), "-reindex");
                    break;
                }

                // The on-disk coinsdb is now in a good state, create the cache
                pcoinsTip.reset(new CCoinsViewCache(pcoinscatcher.get()));

                // Initialize LLMQ system
                llmq::InitLLMQSystem(*evoDb);

                bool is_coinsview_empty = fReset || fReindexChainState || pcoinsTip->GetBestBlock().IsNull();
                if (!is_coinsview_empty) {
                    // LoadChainTip sets chainActive based on pcoinsTip's best block
                    if (!LoadChainTip(chainparams)) {
                        strLoadError = _("Error initializing block database");
                        break;
                    }
                    assert(chainActive.Tip() != nullptr);
                }

                if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
                    // Prune zerocoin invalid outs if they were improperly stored in the coins database
                    LOCK(cs_main);
                    int chainHeight = chainActive.Height();
                    bool fZerocoinActive = chainHeight > 0 && consensus.NetworkUpgradeActive(chainHeight, Consensus::UPGRADE_ZC);

                    uiInterface.InitMessage(_("Loading/Pruning invalid outputs..."));
                    if (fZerocoinActive) {
                        if (!pcoinsTip->PruneInvalidEntries()) {
                            strLoadError = _("System error while flushing the chainstate after pruning invalid entries. Possible corrupt database.");
                            break;
                        }
                        MoneySupply.Update(pcoinsTip->GetTotalAmount(), chainHeight);
                        // No need to keep the invalid outs in memory. Clear the map 100 blocks after the last invalid UTXO
                        if (chainHeight > consensus.height_last_invalid_UTXO + 100) {
                            invalid_out::setInvalidOutPoints.clear();
                        }
                    } else {
                        // Populate list of invalid/fraudulent outpoints that are banned from the chain
                        // They will not be added to coins view
                        invalid_out::LoadOutpoints();
                    }
                }

                if (!is_coinsview_empty) {
                    uiInterface.InitMessage(_("Verifying blocks..."));
                    {
                        LOCK(cs_main);
                        CBlockIndex *tip = chainActive.Tip();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                        RPCNotifyBlockChange(true, tip);
                        if (tip && tip->nTime > GetAdjustedTime() + 2 * 60 * 60) {
                            strLoadError = _("The block database contains a block which appears to be from the future. "
                                             "This may be due to your computer's date and time being set incorrectly. "
                                             "Only rebuild the block database if you are sure that your computer's date and time are correct");
                            break;
                        }
                    }

<<<<<<< HEAD
                    //Must check at level 4
                    if (!CVerifyDB().VerifyDB(pcoinsdbview, 4, GetArg("-checkblocks", 100))) {
                        strLoadError = _("Corrupted block database detected");
                        fVerifyingBlocks = false;
=======
                    if (!CVerifyDB().VerifyDB(pcoinsdbview.get(), gArgs.GetArg("-checklevel", DEFAULT_CHECKLEVEL),
                            gArgs.GetArg("-checkblocks", DEFAULT_CHECKBLOCKS))) {
                        strLoadError = _("Corrupted block database detected");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                        break;
                    }
                }
            } catch (const std::exception& e) {
<<<<<<< HEAD
                if (fDebug) LogPrintf("%s\n", e.what());
                strLoadError = _("Error opening block database");
                fVerifyingBlocks = false;
                break;
            }

            fVerifyingBlocks = false;
=======
                LogPrintf("%s\n", e.what());
                strLoadError = _("Error opening block database");
                break;
            }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            fLoaded = true;
            LogPrintf(" block index %15dms\n", GetTimeMillis() - load_block_index_start_time);
        } while (false);

        if (!fLoaded && !ShutdownRequested()) {
            // first suggest a reindex
            if (!fReset) {
                bool fRet = uiInterface.ThreadSafeMessageBox(
                    strLoadError + ".\n\n" + _("Do you want to rebuild the block database now?"),
                    "", CClientUIInterface::MSG_ERROR | CClientUIInterface::BTN_ABORT);
                if (fRet) {
                    fReindex = true;
<<<<<<< HEAD
                    fRequestShutdown = false;

                    filesystem::path blocksDir = GetDataDir() / "blocks";
                    filesystem::path chainstateDir = GetDataDir() / "chainstate";

                    // We delete in 4 individual steps in case one of the folder is missing already
                    try {
                        if (filesystem::exists(blocksDir)){
                            boost::filesystem::remove_all(blocksDir);
                            LogPrintf("-resync: folder deleted: %s\n", blocksDir.string().c_str());
                        }

                        if (filesystem::exists(chainstateDir)){
                            boost::filesystem::remove_all(chainstateDir);
                            LogPrintf("-resync: folder deleted: %s\n", chainstateDir.string().c_str());
                        }

                        boost::filesystem::create_directories(blocksDir);
                        boost::filesystem::create_directories(chainstateDir);
                    } catch (const boost::filesystem::filesystem_error& error) {
                        LogPrintf("Failed to delete blockchain folders %s\n", error.what());
                    }

=======
                    AbortShutdown();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                } else {
                    LogPrintf("Aborted block database rebuild. Exiting.\n");
                    return false;
                }
            } else {
<<<<<<< HEAD
                return InitError(strLoadError);
=======
                return UIError(strLoadError);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }
    }

    // As LoadBlockIndex can take several minutes, it's possible the user
    // requested to kill the GUI during the last operation. If so, exit.
    // As the program has not fully started yet, Shutdown() is possibly overkill.
    if (ShutdownRequested()) {
        LogPrintf("Shutdown requested. Exiting.\n");
        return false;
    }

<<<<<<< HEAD
    boost::filesystem::path est_path = GetDataDir() / FEE_ESTIMATES_FILENAME;
    CAutoFile est_filein(fopen(est_path.string().c_str(), "rb"), SER_DISK, CLIENT_VERSION);
=======
    fs::path est_path = GetDataDir() / FEE_ESTIMATES_FILENAME;
    CAutoFile est_filein(fsbridge::fopen(est_path, "rb"), SER_DISK, CLIENT_VERSION);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Allowed to fail as this file IS missing on first startup.
    if (!est_filein.IsNull())
        mempool.ReadFeeEstimates(est_filein);
    fFeeEstimatesInitialized = true;

<<<<<<< HEAD
// ********************************************************* Step 8: load wallet
#ifdef ENABLE_WALLET
    if (fDisableWallet) {
        pwalletMain = NULL;
        LogPrintf("Wallet disabled!\n");
    } else {
        // needed to restore wallet transaction meta data after -zapwallettxes
        std::vector<CWalletTx> vWtx;

        if (GetBoolArg("-zapwallettxes", false)) {
            uiInterface.InitMessage(_("Zapping all transactions from wallet..."));

            pwalletMain = new CWallet(strWalletFile);
            DBErrors nZapWalletRet = pwalletMain->ZapWalletTx(vWtx);
            if (nZapWalletRet != DB_LOAD_OK) {
                uiInterface.InitMessage(_("Error loading wallet.dat: Wallet corrupted"));
                return false;
            }

            delete pwalletMain;
            pwalletMain = NULL;
        }

        uiInterface.InitMessage(_("Loading... Please do not interrupt this process as it could lead to a corrupt wallet."));
        fVerifyingBlocks = true;

        const int64_t nWalletStartTime = GetTimeMillis();
        bool fFirstRun = true;
        pwalletMain = new CWallet(strWalletFile);
        DBErrors nLoadWalletRet = pwalletMain->LoadWallet(fFirstRun);
        if (nLoadWalletRet != DB_LOAD_OK) {
            if (nLoadWalletRet == DB_CORRUPT)
                strErrors << _("Error loading wallet.dat: Wallet corrupted") << "\n";
            else if (nLoadWalletRet == DB_NONCRITICAL_ERROR) {
                string msg(_("Warning: error reading wallet.dat! All keys read correctly, but transaction data"
                             " or address book entries might be missing or incorrect."));
                InitWarning(msg);
            } else if (nLoadWalletRet == DB_TOO_NEW)
                strErrors << _("Error loading wallet.dat: Wallet requires newer version of PRCYcoin") << "\n";
            else if (nLoadWalletRet == DB_NEED_REWRITE) {
                strErrors << _("Wallet needed to be rewritten: restart PRCY to complete") << "\n";
                LogPrintf("%s", strErrors.str());
                return InitError(strErrors.str());
            } else
                strErrors << _("Error loading wallet.dat") << "\n";
        }

        if (GetBoolArg("-upgradewallet", fFirstRun)) {
            int nMaxVersion = GetArg("-upgradewallet", 0);
            if (nMaxVersion == 0) // the -upgradewallet without argument case
            {
                LogPrintf("Performing wallet upgrade to %i\n", FEATURE_LATEST);
                nMaxVersion = CLIENT_VERSION;
                pwalletMain->SetMinVersion(FEATURE_LATEST); // permanently upgrade the wallet immediately
            } else
                LogPrintf("Allowing wallet upgrade up to %i\n", nMaxVersion);
            if (nMaxVersion < pwalletMain->GetVersion())
                strErrors << _("Cannot downgrade wallet") << "\n";
            pwalletMain->SetMaxVersion(nMaxVersion);
        }

        if (fFirstRun) {
            // Create new keyUser and set as default key
            if (!pwalletMain->IsHDEnabled()) {
                if (!isDaemon) {
                    uiInterface.ShowRecoveryDialog();
                }

                if (!pwalletMain->IsHDEnabled()) {
                    // generate a new master key
                    pwalletMain->GenerateNewHDChain();
                }
            }

            CPubKey newDefaultKey;
            if (pwalletMain->GetKeyFromPool(newDefaultKey)) {
                pwalletMain->SetDefaultKey(newDefaultKey);
                if (!pwalletMain->SetAddressBook(pwalletMain->vchDefaultKey.GetID(), "", "receive"))
                    strErrors << _("Cannot write default address") << "\n";
            }

            pwalletMain->SetBestChain(chainActive.GetLocator());
        }

        LogPrintf("%s", strErrors.str());
        LogPrintf("Wallet completed loading in %15dms\n", GetTimeMillis() - nWalletStartTime);

        RegisterValidationInterface(pwalletMain);
        int height = -1;
        CBlockIndex* pindexRescan = chainActive.Tip();
        if (GetBoolArg("-rescan", false)) {
            pindexRescan = chainActive.Genesis();
        } else {
            CWalletDB walletdb(strWalletFile);
            CBlockLocator locator;
            if (walletdb.ReadBestBlock(locator)) {
                pindexRescan = FindForkInGlobalIndex(chainActive, locator);
            } else {
                if (!walletdb.ReadScannedBlockHeight(height)) {
                    if (height > chainActive.Height()) {
                        pindexRescan = chainActive.Genesis();
                    } else {
                        pindexRescan = chainActive[height];
                    }
                } else
                    pindexRescan = chainActive.Genesis();
            }
        }
        if (chainActive.Tip() && chainActive.Tip() != pindexRescan) {
            uiInterface.InitMessage(_("Rescanning..."));
            LogPrintf("Rescanning last %i blocks (from block %i)...\n", chainActive.Height() - pindexRescan->nHeight, pindexRescan->nHeight);
            const int64_t nWalletRescanTime = GetTimeMillis();
            if (pwalletMain->ScanForWalletTransactions(pindexRescan, true, true) == -1) {
                return error("Shutdown requested over the txs scan. Exiting.");
            }
            LogPrintf("Rescan completed in %15dms\n", GetTimeMillis() - nWalletRescanTime);
            pwalletMain->SetBestChain(chainActive.GetLocator());
            nWalletDBUpdated++;

            // Restore wallet transaction metadata after -zapwallettxes=1
            if (GetBoolArg("-zapwallettxes", false) && GetArg("-zapwallettxes", "1") != "2") {
                for (const CWalletTx& wtxOld : vWtx) {
                    uint256 hash = wtxOld.GetHash();
                    std::map<uint256, CWalletTx>::iterator mi = pwalletMain->mapWallet.find(hash);
                    if (mi != pwalletMain->mapWallet.end()) {
                        const CWalletTx* copyFrom = &wtxOld;
                        CWalletTx* copyTo = &mi->second;
                        copyTo->mapValue = copyFrom->mapValue;
                        copyTo->vOrderForm = copyFrom->vOrderForm;
                        copyTo->nTimeReceived = copyFrom->nTimeReceived;
                        copyTo->nTimeSmart = copyFrom->nTimeSmart;
                        copyTo->fFromMe = copyFrom->fFromMe;
                        copyTo->strFromAccount = copyFrom->strFromAccount;
                        copyTo->nOrderPos = copyFrom->nOrderPos;
                        copyTo->WriteToDisk();
                    }
                }
            }
        }
        fVerifyingBlocks = false;

    }  // (!fDisableWallet)
#else  // ENABLE_WALLET
    LogPrintf("No wallet compiled in!\n");
#endif // !ENABLE_WALLET
    // ********************************************************* Step 9: import blocks

    if (!CheckDiskSpace())
        return false;
=======
// ********************************************************* Step 8: Backup and Load wallet
#ifdef ENABLE_WALLET
    if (!InitLoadWallet())
        return false;
#else
    LogPrintf("No wallet compiled in!\n");
#endif
    // ********************************************************* Step 9: import blocks

    if (!CheckDiskSpace(GetDataDir())) {
        UIError(strprintf(_("Error: Disk space is low for %s"), GetDataDir()));
        return false;
    }
    if (!CheckDiskSpace(GetBlocksDir())) {
        UIError(strprintf(_("Error: Disk space is low for %s"), GetBlocksDir()));
        return false;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // Either install a handler to notify us when genesis activates, or set fHaveGenesis directly.
    // No locking, as this happens before any background thread is started.
    if (chainActive.Tip() == nullptr) {
        uiInterface.NotifyBlockTip.connect(BlockNotifyGenesisWait);
    } else {
        fHaveGenesis = true;
    }

<<<<<<< HEAD
    if (mapArgs.count("-blocknotify"))
        uiInterface.NotifyBlockTip.connect(BlockNotifyCallback);

    if (mapArgs.count("-blocksizenotify"))
        uiInterface.NotifyBlockSize.connect(BlockSizeNotifyCallback);

    // scan for better chains in the block chain database, that are not yet connected in the active best chain
    CValidationState state;
    if (!ActivateBestChain(state)) {
        strErrors << "Failed to connect best block";
        StartShutdown();
        return false;
    }
=======
    if (gArgs.IsArgSet("-blocknotify"))
        uiInterface.NotifyBlockTip.connect(BlockNotifyCallback);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // update g_best_block if needed
    {
        LOCK(g_best_block_mutex);
        if (g_best_block.IsNull() && chainActive.Tip()) {
<<<<<<< HEAD
            g_best_block = chainActive.Tip()->GetBlockHash();
=======
            CBlockIndex* tip = chainActive.Tip();
            g_best_block = tip->GetBlockHash();
            g_best_block_time = tip->GetBlockTime();;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            g_best_block_cv.notify_all();
        }
    }

<<<<<<< HEAD
    std::vector<boost::filesystem::path> vImportFiles;
    if (mapArgs.count("-loadblock")) {
        for (string strFile : mapMultiArgs["-loadblock"])
            vImportFiles.push_back(strFile);
    }
    threadGroup.create_thread(boost::bind(&ThreadImport, vImportFiles));
=======
    std::vector<fs::path> vImportFiles;
    for (const std::string& strFile : gArgs.GetArgs("-loadblock")) {
        vImportFiles.emplace_back(strFile);
    }
    threadGroup.create_thread(std::bind(&ThreadImport, vImportFiles));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // Wait for genesis block to be processed
    LogPrintf("Waiting for genesis block to be imported...\n");
    {
        std::unique_lock<std::mutex> lockG(cs_GenesisWait);
<<<<<<< HEAD
        while (!fHaveGenesis) {
            condvar_GenesisWait.wait(lockG);
=======
        // We previously could hang here if StartShutdown() is called prior to
        // ThreadImport getting started, so instead we just wait on a timer to
        // check ShutdownRequested() regularly.
        while (!fHaveGenesis && !ShutdownRequested()) {
            condvar_GenesisWait.wait_for(lockG, std::chrono::milliseconds(500));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        uiInterface.NotifyBlockTip.disconnect(BlockNotifyGenesisWait);
    }

<<<<<<< HEAD
    // ********************************************************* Step 10: setup ObfuScation

    uiInterface.InitMessage(_("Loading masternode cache..."));

    CMasternodeDB mndb;
    CMasternodeDB::ReadResult readResult = mndb.Read(mnodeman);
    if (readResult == CMasternodeDB::FileError)
        LogPrintf("Missing masternode cache file - mncache.dat, will try to recreate\n");
    else if (readResult != CMasternodeDB::Ok) {
        LogPrintf("Error reading mncache.dat: ");
        if (readResult == CMasternodeDB::IncorrectFormat)
            LogPrintf("magic is ok but data has invalid format, will try to recreate\n");
        else
            LogPrintf("file format is unknown or invalid, please fix it manually\n");
    }

    uiInterface.InitMessage(_("Loading budget cache..."));

    CBudgetDB budgetdb;
    CBudgetDB::ReadResult readResult2 = budgetdb.Read(budget);

    if (readResult2 == CBudgetDB::FileError)
        LogPrintf("Missing budget cache - budget.dat, will try to recreate\n");
    else if (readResult2 != CBudgetDB::Ok) {
        LogPrintf("Error reading budget.dat: ");
        if (readResult2 == CBudgetDB::IncorrectFormat)
            LogPrintf("magic is ok but data has invalid format, will try to recreate\n");
        else
            LogPrintf("file format is unknown or invalid, please fix it manually\n");
    }

    //flag our cached items so we send them to our peers
    budget.ResetSync();
    budget.ClearSeen();


    uiInterface.InitMessage(_("Loading masternode payment cache..."));

    CMasternodePaymentDB mnpayments;
    CMasternodePaymentDB::ReadResult readResult3 = mnpayments.Read(masternodePayments);

    if (readResult3 == CMasternodePaymentDB::FileError)
        LogPrintf("Missing masternode payment cache - mnpayments.dat, will try to recreate\n");
    else if (readResult3 != CMasternodePaymentDB::Ok) {
        LogPrintf("Error reading mnpayments.dat: ");
        if (readResult3 == CMasternodePaymentDB::IncorrectFormat)
            LogPrintf("magic is ok but data has invalid format, will try to recreate\n");
        else
            LogPrintf("file format is unknown or invalid, please fix it manually\n");
    }

    fMasterNode = GetBoolArg("-masternode", false);

    if ((fMasterNode || masternodeConfig.getCount() > -1) && fTxIndex == false) {
        return InitError("Enabling Masternode support requires turning on transaction indexing."
                         "Please add txindex=1 to your configuration and start with -reindex");
    }

    if (fMasterNode) {
        LogPrintf("IS MASTER NODE\n");
        strMasterNodeAddr = GetArg("-masternodeaddr", "");

        LogPrintf(" addr %s\n", strMasterNodeAddr.c_str());

        if (!strMasterNodeAddr.empty()) {
            CService addrTest = CService(strMasterNodeAddr);
            if (!addrTest.IsValid()) {
                return InitError("Invalid -masternodeaddr address: " + strMasterNodeAddr);
            }
        }

        strMasterNodePrivKey = GetArg("-masternodeprivkey", "");
        if (!strMasterNodePrivKey.empty()) {
            std::string errorMessage;

            CKey key;
            CPubKey pubkey;

            if (!obfuScationSigner.SetKey(strMasterNodePrivKey, errorMessage, key, pubkey)) {
                return InitError(_("Invalid masternodeprivkey. Please see documenation."));
            }

            activeMasternode.pubKeyMasternode = pubkey;

        } else {
            return InitError(_("You must specify a masternodeprivkey in the configuration. Please see documentation for help."));
        }
    }

    //get the mode of budget voting for this masternode
    strBudgetMode = GetArg("-budgetvotemode", "auto");

#ifdef ENABLE_WALLET
    if (GetBoolArg("-mnconflock", true) && pwalletMain) {
        LOCK(pwalletMain->cs_wallet);
=======
    if (ShutdownRequested()) {
        return false;
    }

    int chain_active_height;

    //// debug print
    {
        LOCK(cs_main);
        chain_active_height = chainActive.Height();
        LogPrintf("mapBlockIndex.size() = %u\n", mapBlockIndex.size());
    }
    LogPrintf("chainActive.Height() = %d\n", chain_active_height);

    // Update money supply
    if (!fReindex && !fReindexChainState) {
        uiInterface.InitMessage(_("Calculating money supply..."));
        MoneySupply.Update(pcoinsTip->GetTotalAmount(), chain_active_height);
    }


    // ********************************************************* Step 10: setup layer 2 data

    LoadTierTwo(chain_active_height);
    if (!InitActiveMN()) return false;
    RegisterTierTwoValidationInterface();

    // set the mode of budget voting for this node
    SetBudgetFinMode(gArgs.GetArg("-budgetvotemode", "auto"));

#ifdef ENABLE_WALLET
    // !TODO: remove after complete transition to DMN
    // use only the first wallet here. This section can be removed after transition to DMN
    if (gArgs.GetBoolArg("-mnconflock", DEFAULT_MNCONFLOCK) && !vpwallets.empty() && vpwallets[0]) {
        LOCK(vpwallets[0]->cs_wallet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        LogPrintf("Locking Masternodes collateral utxo:\n");
        uint256 mnTxHash;
        for (const auto& mne : masternodeConfig.getEntries()) {
            mnTxHash.SetHex(mne.getTxHash());
<<<<<<< HEAD
            COutPoint outpoint = COutPoint(mnTxHash, (unsigned int) std::stoul(mne.getOutputIndex().c_str()));
            pwalletMain->LockCoin(outpoint);
=======
            COutPoint outpoint = COutPoint(mnTxHash, (unsigned int) std::stoul(mne.getOutputIndex()));
            vpwallets[0]->LockCoin(outpoint);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            LogPrintf("Locked collateral, MN: %s, tx hash: %s, output index: %s\n",
                      mne.getAlias(), mne.getTxHash(), mne.getOutputIndex());
        }
    }
<<<<<<< HEAD
#endif

    fEnableSwiftTX = GetBoolArg("-enableswifttx", fEnableSwiftTX);
    nSwiftTXDepth = GetArg("-swifttxdepth", nSwiftTXDepth);
    nSwiftTXDepth = std::min(std::max(nSwiftTXDepth, 0), 60);

    // lite mode disables all Masternode and Obfuscation related functionality
    fLiteMode = GetBoolArg("-litemode", false);
    if (fMasterNode && fLiteMode) {
        return InitError("You can not start a masternode in litemode");
    }

    LogPrintf("fLiteMode %d\n", fLiteMode);
    LogPrintf("nSwiftTXDepth %d\n", nSwiftTXDepth);
    LogPrintf("Budget Mode %s\n", strBudgetMode.c_str());

    /* Denominations

       A note about convertability. Within Obfuscation pools, each denomination
       is convertable to another.

       For example:
       1PRCY+1000 == (.1PRCY+100)*10
       10PRCY+10000 == (1PRCY+1000)*10
    */
    obfuScationDenominations.push_back((10000 * COIN) + 10000000);
    obfuScationDenominations.push_back((1000 * COIN) + 1000000);
    obfuScationDenominations.push_back((100 * COIN) + 100000);
    obfuScationDenominations.push_back((10 * COIN) + 10000);
    obfuScationDenominations.push_back((1 * COIN) + 1000);
    obfuScationDenominations.push_back((.1 * COIN) + 100);

    obfuScationPool.InitCollateralAddress();

    threadGroup.create_thread(boost::bind(&ThreadCheckObfuScationPool));
=======

    // automatic lock for DMN
    if (gArgs.GetBoolArg("-mnconflock", DEFAULT_MNCONFLOCK)) {
        const auto& mnList = deterministicMNManager->GetListAtChainTip();
        for (CWallet* pwallet : vpwallets) {
            pwallet->ScanMasternodeCollateralsAndLock(mnList);
        }
    }
#endif

    // Start tier two threads and jobs
    StartTierTwoThreadsAndScheduleJobs(threadGroup, scheduler);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (ShutdownRequested()) {
        LogPrintf("Shutdown requested. Exiting.\n");
        return false;
    }

<<<<<<< HEAD
    // ********************************************************* Step 11: start node

    if (!strErrors.str().empty())
        return InitError(strErrors.str());

    //// debug print
    LogPrintf("mapBlockIndex.size() = %u\n", mapBlockIndex.size());
    LogPrintf("chainActive.Height() = %d\n", chainActive.Height());
#ifdef ENABLE_WALLET
    LogPrintf("setKeyPool.size() = %u\n", pwalletMain ? pwalletMain->setKeyPool.size() : 0);
    LogPrintf("mapWallet.size() = %u\n", pwalletMain ? pwalletMain->mapWallet.size() : 0);
    LogPrintf("mapAddressBook.size() = %u\n", pwalletMain ? pwalletMain->mapAddressBook.size() : 0);
#endif

    if (GetBoolArg("-listenonion", DEFAULT_LISTEN_ONION))
        StartTorControl(threadGroup);

    StartNode(threadGroup, scheduler);

#ifdef ENABLE_WALLET
    // Generate coins in the background
    if (pwalletMain)
        GeneratePrcycoins(GetBoolArg("-gen", false), pwalletMain, GetArg("-genproclimit", 1));
=======
    // start LLMQ system
    llmq::StartLLMQSystem();

    // ********************************************************* Step 11: start node

    if (!strErrors.str().empty())
        return UIError(strErrors.str());

#ifdef ENABLE_WALLET
    {
        int idx = 0;
        for (CWalletRef pwallet : vpwallets) {
            LogPrintf("Wallet %d\n", idx++);
            LOCK(pwallet->cs_wallet);
            LogPrintf("setKeyPool.size() = %u\n", pwallet->GetKeyPoolSize());
            LogPrintf("mapWallet.size() = %u\n", pwallet->mapWallet.size());
            LogPrintf("mapAddressBook.size() = %u\n", pwallet->GetAddressBookSize());
        }
    }
#endif

    if (gArgs.GetBoolArg("-listenonion", DEFAULT_LISTEN_ONION))
        StartTorControl();

    Discover();

    // Map ports with UPnP or NAT-PMP
    StartMapPort(gArgs.GetBoolArg("-upnp", DEFAULT_UPNP), gArgs.GetBoolArg("-natpmp", DEFAULT_NATPMP));

    CConnman::Options connOptions;
    connOptions.nLocalServices = nLocalServices;
    connOptions.nRelevantServices = nRelevantServices;
    connOptions.nMaxConnections = nMaxConnections;
    connOptions.nMaxOutbound = std::min(MAX_OUTBOUND_CONNECTIONS, connOptions.nMaxConnections);
    connOptions.nMaxAddnode = MAX_ADDNODE_CONNECTIONS;
    connOptions.nMaxFeeler = 1;
    connOptions.nBestHeight = chain_active_height;
    connOptions.uiInterface = &uiInterface;
    connOptions.m_msgproc = peerLogic.get();
    connOptions.nSendBufferMaxSize = 1000*gArgs.GetArg("-maxsendbuffer", DEFAULT_MAXSENDBUFFER);
    connOptions.nReceiveFloodSize = 1000*gArgs.GetArg("-maxreceivebuffer", DEFAULT_MAXRECEIVEBUFFER);
    connOptions.m_added_nodes = gArgs.GetArgs("-addnode");

    if (gArgs.IsArgSet("-bind")) {
        for (const std::string& strBind : gArgs.GetArgs("-bind")) {
            CService addrBind;
            if (!Lookup(strBind, addrBind, GetListenPort(), false)) {
                return UIError(ResolveErrMsg("bind", strBind));
            }
            connOptions.vBinds.emplace_back(addrBind);
        }
    }
    if (gArgs.IsArgSet("-whitebind")) {
        for (const std::string& strBind : gArgs.GetArgs("-whitebind")) {
            CService addrBind;
            if (!Lookup(strBind, addrBind, 0, false)) {
                return UIError(ResolveErrMsg("whitebind", strBind));
            }
            if (addrBind.GetPort() == 0) {
                return UIError(strprintf(_("Need to specify a port with %s: '%s'"), "-whitebind", strBind));
            }
            connOptions.vWhiteBinds.emplace_back(addrBind);
        }
    }

    for (const auto& net : gArgs.GetArgs("-whitelist")) {
        CSubNet subnet;
        LookupSubNet(net, subnet);
        if (!subnet.IsValid())
            return UIError(strprintf(_("Invalid netmask specified in %s: '%s'"), "-whitelist", net));
        connOptions.vWhitelistedRange.emplace_back(subnet);
    }

    connOptions.vSeedNodes = gArgs.GetArgs("-seednode");

    // Initiate outbound connections unless connect=0
    connOptions.m_use_addrman_outgoing = !gArgs.IsArgSet("-connect");
    if (!connOptions.m_use_addrman_outgoing) {
        const auto connect = gArgs.GetArgs("-connect");
        if (connect.size() != 1 || connect[0] != "0") {
            connOptions.m_specified_outgoing = connect;
        }
    }
    if (!connman.Start(scheduler, connOptions)) {
        return false;
    }

#ifdef ENABLE_WALLET
    // Generate coins in the background (disabled on mainnet. use only wallet 0)
    if (!vpwallets.empty())
        GenerateBitcoins(gArgs.GetBoolArg("-gen", DEFAULT_GENERATE), vpwallets[0], gArgs.GetArg("-genproclimit", DEFAULT_GENERATE_PROCLIMIT));
#endif

#ifdef ENABLE_WALLET
    uiInterface.InitMessage(_("Reaccepting wallet transactions..."));
    for (CWalletRef pwallet : vpwallets) {
        pwallet->postInitProcess(scheduler);
    }
    // StakeMiner thread disabled by default on regtest
    if (!vpwallets.empty() && gArgs.GetBoolArg("-staking", !Params().IsRegTestNet() && DEFAULT_STAKING)) {
        threadGroup.create_thread(std::bind(&ThreadStakeMinter));
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif

    // ********************************************************* Step 12: finished

    SetRPCWarmupFinished();
    uiInterface.InitMessage(_("Done loading"));

<<<<<<< HEAD
#ifdef ENABLE_WALLET
    bool storedStakingStatus = false;

    if (pwalletMain) {
        // Add wallet transactions that aren't already in a block to mapTransactions
        pwalletMain->ReacceptWalletTransactions();
        pwalletMain->fCombineDust = GetBoolArg("-combinedust", true);
        // Run a thread to flush wallet periodically
        threadGroup.create_thread(boost::bind(&ThreadFlushWalletDB, boost::ref(pwalletMain->strWalletFile)));
		
        if (pwalletMain->fCombineDust){
            LogPrintf("Autocombinedust is enabled\n");
        } else {
            LogPrintf("Autocombinedust is disabled\n");
        }
		
        storedStakingStatus = pwalletMain->ReadStakingStatus();
        if (GetBoolArg("-staking", false) || storedStakingStatus) {
            fGeneratePrcycoins = true;
            pwalletMain->WriteStakingStatus(true);
            LogPrintf("Starting staking\n");
            threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "stakemint", &ThreadStakeMinter));
            // combineMode should be OFF on first launch or keep previous setting when available
            // This changes that setting only if staking is on
            if (GetBoolArg("-autoconsolidate", false)){
                LogPrintf("Autoconsolidate is enabled and we are setting CombineMode::ON now\n");
                pwalletMain->combineMode = CombineMode::ON;
            } else {
                pwalletMain->combineMode = CombineMode::OFF;
                LogPrintf("Autoconsolidate is disabled\n");
            }
        } else {
            LogPrintf("Staking is disabled\n");
        }
        //read decoy confirmation min
        pwalletMain->DecoyConfirmationMinimum = GetArg("-decoyconfirm", 15);
    }

    CWalletDB walletdb(strWalletFile);
    double reserveBalance;
    if (walletdb.ReadReserveAmount(reserveBalance))
        nReserveBalance = reserveBalance * COIN;
#endif

    return !fRequestShutdown;
=======
    return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
