// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc/server.h"

<<<<<<< HEAD
#include "base58.h"
#include "init.h"
#include "main.h"
#include "random.h"
#include "sync.h"
#include "guiinterface.h"
#include "util.h"
=======
#include "fs.h"
#include "key_io.h"
#include "random.h"
#include "shutdown.h"
#include "sync.h"
#include "guiinterface.h"
#include "util/system.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "utilstrencodings.h"

#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
<<<<<<< HEAD
#endif

#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string/case_conv.hpp> // for to_upper()
#include <univalue.h>

using namespace RPCServer;
using namespace std;

=======
#endif // ENABLE_WALLET

#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>

#include <memory> // for unique_ptr
#include <unordered_map>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static bool fRPCRunning = false;
static bool fRPCInWarmup = true;
static std::string rpcWarmupStatus("RPC server started");
static RecursiveMutex cs_rpcWarmup;

/* Timer-creating functions */
static RPCTimerInterface* timerInterface = NULL;
<<<<<<< HEAD
/* Map of name to timer.
 * @note Can be changed to std::unique_ptr when C++11 */
static std::map <std::string, boost::shared_ptr<RPCTimerBase>> deadlineTimers;

static struct CRPCSignals {
    boost::signals2::signal<void()> Started;
    boost::signals2::signal<void()> Stopped;
    boost::signals2::signal<void(const CRPCCommand &)> PreCommand;
    boost::signals2::signal<void(const CRPCCommand &)> PostCommand;
} g_rpcSignals;

void RPCServer::OnStarted(boost::function<void()> slot) {
    g_rpcSignals.Started.connect(slot);
}

void RPCServer::OnStopped(boost::function<void()> slot) {
    g_rpcSignals.Stopped.connect(slot);
}

void RPCServer::OnPreCommand(boost::function<void(const CRPCCommand &)> slot) {
    g_rpcSignals.PreCommand.connect(boost::bind(slot, _1));
}

void RPCServer::OnPostCommand(boost::function<void(const CRPCCommand &)> slot) {
    g_rpcSignals.PostCommand.connect(boost::bind(slot, _1));
}

void RPCTypeCheck(const UniValue &params, const list <UniValue::VType> &typesExpected, bool fAllowNull) {
=======
/* Map of name to timer. */
static std::map<std::string, std::unique_ptr<RPCTimerBase>> deadlineTimers;

static struct CRPCSignals
{
    boost::signals2::signal<void ()> Started;
    boost::signals2::signal<void ()> Stopped;
    boost::signals2::signal<void (const CRPCCommand&)> PreCommand;
} g_rpcSignals;

void RPCServer::OnStarted(std::function<void ()> slot)
{
    g_rpcSignals.Started.connect(slot);
}

void RPCServer::OnStopped(std::function<void ()> slot)
{
    g_rpcSignals.Stopped.connect(slot);
}

void RPCServer::OnPreCommand(std::function<void (const CRPCCommand&)> slot)
{
    g_rpcSignals.PreCommand.connect(std::bind(slot, std::placeholders::_1));
}

void RPCTypeCheck(const UniValue& params,
                  const std::list<UniValue::VType>& typesExpected,
                  bool fAllowNull)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned int i = 0;
    for (UniValue::VType t : typesExpected) {
        if (params.size() <= i)
            break;

        const UniValue& v = params[i];
        if (!((v.type() == t) || (fAllowNull && (v.isNull())))) {
<<<<<<< HEAD
            string err = strprintf("Expected type %s, got %s",
=======
            std::string err = strprintf("Expected type %s, got %s",
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                                   uvTypeName(t), uvTypeName(v.type()));
            throw JSONRPCError(RPC_TYPE_ERROR, err);
        }
        i++;
    }
}

<<<<<<< HEAD
void RPCTypeCheckObj(const UniValue& o,
                     const map<string, UniValue::VType>& typesExpected,
                     bool fAllowNull)
{
    for (const PAIRTYPE(string, UniValue::VType)& t : typesExpected) {
=======
void RPCTypeCheckArgument(const UniValue& value, const UniValueType& typeExpected)
{
    if (!typeExpected.typeAny && value.type() != typeExpected.type) {
        throw JSONRPCError(RPC_TYPE_ERROR, strprintf("Expected type %s, got %s", uvTypeName(typeExpected.type), uvTypeName(value.type())));
    }
}

void RPCTypeCheckObj(const UniValue& o,
    const std::map<std::string, UniValueType>& typesExpected,
    bool fAllowNull,
    bool fStrict)
{
    for (const auto& t : typesExpected) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        const UniValue& v = find_value(o, t.first);
        if (!fAllowNull && v.isNull())
            throw JSONRPCError(RPC_TYPE_ERROR, strprintf("Missing %s", t.first));

<<<<<<< HEAD
        if (!((v.type() == t.second) || (fAllowNull && (v.isNull())))) {
            string err = strprintf("Expected type %s for %s, got %s",
                                   uvTypeName(t.second), t.first, uvTypeName(v.type()));
            throw JSONRPCError(RPC_TYPE_ERROR, err);
        }
    }
}

static inline int64_t roundint64(double d) {
    return (int64_t)(d > 0 ? d + 0.5 : d - 0.5);
}

CAmount AmountFromValue(const UniValue& value) {
    if (!value.isNum())
        throw JSONRPCError(RPC_TYPE_ERROR, "Amount is not a number");

    double dAmount = value.get_real();
    if (dAmount <= 0.0 || dAmount > Params().MAX_MONEY)
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    CAmount nAmount = roundint64(dAmount * COIN);
    return nAmount;
}

UniValue ValueFromAmount(const CAmount &amount) {
=======
        if (!(t.second.typeAny || v.type() == t.second.type || (fAllowNull && v.isNull()))) {
            std::string err = strprintf("Expected type %s for %s, got %s",
                                        uvTypeName(t.second.type), t.first, uvTypeName(v.type()));
            throw JSONRPCError(RPC_TYPE_ERROR, err);
        }
    }

    if (fStrict) {
        for (const std::string& k : o.getKeys()) {
            if (typesExpected.count(k) == 0) {
                std::string err = strprintf("Unexpected key %s", k);
                throw JSONRPCError(RPC_TYPE_ERROR, err);
            }
        }
    }
}

CAmount AmountFromValue(const UniValue& value)
{
    if (!value.isNum() && !value.isStr())
        throw JSONRPCError(RPC_TYPE_ERROR,"Amount is not a number or string");
    CAmount nAmount;
    if (!ParseFixedPoint(value.getValStr(), 8, &nAmount))
        throw JSONRPCError(RPC_TYPE_ERROR, "Invalid amount");
    if (!Params().GetConsensus().MoneyRange(nAmount))
        throw JSONRPCError(RPC_TYPE_ERROR, "Amount out of range");
    return nAmount;
}

UniValue ValueFromAmount(const CAmount& amount)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool sign = amount < 0;
    int64_t n_abs = (sign ? -amount : amount);
    int64_t quotient = n_abs / COIN;
    int64_t remainder = n_abs % COIN;
    return UniValue(UniValue::VNUM,
<<<<<<< HEAD
                    strprintf("%s%d.%08d", sign ? "-" : "", quotient, remainder));
}

uint256 ParseHashV(const UniValue &v, string strName) {
    string strHex;
=======
            strprintf("%s%d.%08d", sign ? "-" : "", quotient, remainder));
}

uint256 ParseHashV(const UniValue& v, std::string strName)
{
    std::string strHex;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (v.isStr())
        strHex = v.get_str();
    if (!IsHex(strHex)) // Note: IsHex("") is false
        throw JSONRPCError(RPC_INVALID_PARAMETER, strName + " must be hexadecimal string (not '" + strHex + "')");
<<<<<<< HEAD
=======
    if (64 != strHex.length())
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("%s must be of length %d (not %d)", strName, 64, strHex.length()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    uint256 result;
    result.SetHex(strHex);
    return result;
}
<<<<<<< HEAD

uint256 ParseHashO(const UniValue &o, string strKey) {
    return ParseHashV(find_value(o, strKey), strKey);
}

vector<unsigned char> ParseHexV(const UniValue& v, string strName) {
    string strHex;
=======
uint256 ParseHashO(const UniValue& o, std::string strKey)
{
    return ParseHashV(find_value(o, strKey), strKey);
}
std::vector<unsigned char> ParseHexV(const UniValue& v, std::string strName)
{
    std::string strHex;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (v.isStr())
        strHex = v.get_str();
    if (!IsHex(strHex))
        throw JSONRPCError(RPC_INVALID_PARAMETER, strName + " must be hexadecimal string (not '" + strHex + "')");
    return ParseHex(strHex);
}
<<<<<<< HEAD

vector<unsigned char> ParseHexO(const UniValue &o, string strKey) {
    return ParseHexV(find_value(o, strKey), strKey);
}

int ParseInt(const UniValue &o, string strKey) {
    const UniValue &v = find_value(o, strKey);
    if (!v.isNum())
=======
std::vector<unsigned char> ParseHexO(const UniValue& o, std::string strKey)
{
    return ParseHexV(find_value(o, strKey), strKey);
}

int ParseInt(const UniValue& o, std::string strKey)
{
    const UniValue& v = find_value(o, strKey);
    if (v.isNum())
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid parameter, " + strKey + "is not an int");

    return v.get_int();
}

<<<<<<< HEAD
bool ParseBool(const UniValue& o, string strKey) {
    const UniValue& v = find_value(o, strKey);
    if (!v.isBool())
=======
bool ParseBool(const UniValue& o, std::string strKey)
{
    const UniValue& v = find_value(o, strKey);
    if (v.isBool())
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid parameter, " + strKey + "is not a bool");

    return v.get_bool();
}


/**
 * Note: This interface may still be subject to change.
 */

<<<<<<< HEAD
string CRPCTable::help(string strCommand) const {
    string strRet;
    string category;
    set <rpcfn_type> setDone;
    vector <pair<string, const CRPCCommand *>> vCommands;

    for (map<string, const CRPCCommand *>::const_iterator mi = mapCommands.begin(); mi != mapCommands.end(); ++mi)
        vCommands.push_back(make_pair(mi->second->category + mi->first, mi->second));
    sort(vCommands.begin(), vCommands.end());

    for (const PAIRTYPE(string, const CRPCCommand*) &command : vCommands) {
        const CRPCCommand *pcmd = command.second;
        string strMethod = pcmd->name;
        // We already filter duplicates, but these deprecated screw up the sort order
        if (strMethod.find("label") != string::npos)
            continue;
        if ((strCommand != "" || pcmd->category == "hidden") && strMethod != strCommand)
            continue;
#ifdef ENABLE_WALLET
        if (pcmd->reqWallet && !pwalletMain)
            continue;
#endif

        try {
            UniValue params;
            rpcfn_type pfn = pcmd->actor;
            if (setDone.insert(pfn).second)
                (*pfn)(params, true);
        } catch (const std::exception& e) {
            // Help text is returned in an exception
            string strHelp = string(e.what());
            if (strCommand == "") {
                if (strHelp.find('\n') != string::npos)
=======
std::string CRPCTable::help(const std::string& strCommand, const JSONRPCRequest& helpreq) const
{
    std::string strRet;
    std::string category;
    std::set<rpcfn_type> setDone;
    std::vector<std::pair<std::string, const CRPCCommand*> > vCommands;

    for (const auto& entry : mapCommands)
        vCommands.emplace_back(entry.second->category + entry.first, entry.second);
    std::sort(vCommands.begin(), vCommands.end());

    JSONRPCRequest jreq(helpreq);
    jreq.fHelp = true;
    jreq.params = UniValue();

    for (const std::pair<std::string, const CRPCCommand*>& command : vCommands) {
        const CRPCCommand* pcmd = command.second;
        std::string strMethod = pcmd->name;
        if ((strCommand != "" || pcmd->category == "hidden") && strMethod != strCommand)
            continue;
        jreq.strMethod = strMethod;
        try {
            rpcfn_type pfn = pcmd->actor;
            if (setDone.insert(pfn).second)
                (*pfn)(jreq);
        } catch (const std::exception& e) {
            // Help text is returned in an exception
            std::string strHelp = std::string(e.what());
            if (strCommand == "") {
                if (strHelp.find('\n') != std::string::npos)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    strHelp = strHelp.substr(0, strHelp.find('\n'));

                if (category != pcmd->category) {
                    if (!category.empty())
                        strRet += "\n";
                    category = pcmd->category;
<<<<<<< HEAD
                    string firstLetter = category.substr(0, 1);
                    boost::to_upper(firstLetter);
                    strRet += "== " + firstLetter + category.substr(1) + " ==\n";
=======
                    strRet += "== " + Capitalize(category) + " ==\n";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                }
            }
            strRet += strHelp + "\n";
        }
    }
    if (strRet == "")
        strRet = strprintf("help: unknown command: %s\n", strCommand);
    strRet = strRet.substr(0, strRet.size() - 1);
    return strRet;
}

<<<<<<< HEAD
UniValue help(const UniValue& params, bool fHelp) {
    if (fHelp || params.size() > 1)
        throw runtime_error(
                "help ( \"command\" )\n"
                "\nList all commands, or get help for a specified command.\n"
                "\nArguments:\n"
                "1. \"command\"     (string, optional) The command to get help on\n"
                "\nResult:\n"
                "\"text\"     (string) The help text\n");

    string strCommand;
    if (params.size() > 0)
        strCommand = params[0].get_str();

    return tableRPC.help(strCommand);
}


UniValue stop(const UniValue& params, bool fHelp) {
    // Accept the deprecated and ignored 'detach' boolean argument
    if (fHelp || params.size() > 1)
        throw runtime_error(
                "stop\n"
                "\nStop PRCY server.");
    // Event loop will exit after current HTTP requests have been handled, so
    // this reply will get back to the client.
    StartShutdown();
    return "PRCY server stopping";
=======
UniValue help(const JSONRPCRequest& jsonRequest)
{
    if (jsonRequest.fHelp || jsonRequest.params.size() > 1)
        throw std::runtime_error(
            "help ( \"command\" )\n"
            "\nList all commands, or get help for a specified command.\n"
            "\nArguments:\n"
            "1. \"command\"     (string, optional) The command to get help on\n"
            "\nResult:\n"
            "\"text\"     (string) The help text\n");

    std::string strCommand;
    if (jsonRequest.params.size() > 0)
        strCommand = jsonRequest.params[0].get_str();

    return tableRPC.help(strCommand, jsonRequest);
}


UniValue stop(const JSONRPCRequest& jsonRequest)
{
    if (jsonRequest.fHelp || !jsonRequest.params.empty())
        throw std::runtime_error(
            "stop\n"
            "\nStop PIVX server.");
    // Event loop will exit after current HTTP requests have been handled, so
    // this reply will get back to the client.
    StartShutdown();
    return "PIVX server stopping";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}


/**
 * Call Table
 */
static const CRPCCommand vRPCCommands[] =
    {
<<<<<<< HEAD
        //  category              name                      actor (function)         okSafeMode threadSafe reqWallet
        //  --------------------- ------------------------  -----------------------  ---------- ---------- ---------
        /* Overall control/query calls */
        {"control", "getinfo", &getinfo, true, false, false}, /* uses wallet if enabled */
        {"control", "getversion", &getversion, true, false, false},
        {"control", "help", &help, true, true, false},
        {"control", "stop", &stop, true, true, false},

        /* P2P networking */
        {"network", "getnetworkinfo", &getnetworkinfo, true, false, false},
        {"network", "addnode", &addnode, true, true, false},
        {"network", "disconnectnode", &disconnectnode, true, true, false},
        {"network", "getaddednodeinfo", &getaddednodeinfo, true, true, false},
        {"network", "getconnectioncount", &getconnectioncount, true, false, false},
        {"network", "getnettotals", &getnettotals, true, true, false},
        {"network", "getpeerinfo", &getpeerinfo, true, false, false},
        {"network", "ping", &ping, true, false, false},
        {"network", "setban", &setban, true, false, false},
        {"network", "listbanned", &listbanned, true, false, false},
        {"network", "clearbanned", &clearbanned, true, false, false},

        /* Block chain and UTXO */
        {"blockchain", "getsupply", &getsupply, true, false, false},
        {"blockchain", "getmaxsupply", &getmaxsupply, true, false, false},
        {"blockchain", "getblockchaininfo", &getblockchaininfo, true, false, false},
        {"blockchain", "getbestblockhash", &getbestblockhash, true, false, false},
        {"blockchain", "getblockcount", &getblockcount, true, false, false},
        {"blockchain", "getblock", &getblock, true, false, false},
        {"blockchain", "getblockhash", &getblockhash, true, false, false},
        {"blockchain", "setmaxreorgdepth", &setmaxreorgdepth, true, false, false},
        {"blockchain", "resyncfrom", &resyncfrom, true, false, false},
        {"blockchain", "getblockheader", &getblockheader, false, false, false},
        {"blockchain", "getchaintips", &getchaintips, true, false, false},
        {"blockchain", "getdifficulty", &getdifficulty, true, false, false},
        {"blockchain", "getfeeinfo", &getfeeinfo, true, false, false},
        {"blockchain", "getmempoolinfo", &getmempoolinfo, true, true, false},
        {"blockchain", "getrawmempool", &getrawmempool, true, false, false},
        {"blockchain", "gettxout", &gettxout, true, false, false},
        {"blockchain", "gettxoutsetinfo", &gettxoutsetinfo, true, false, false},
        {"blockchain", "verifychain", &verifychain, true, false, false},
        {"blockchain", "invalidateblock", &invalidateblock, true, true, false},
        {"blockchain", "reconsiderblock", &reconsiderblock, true, true, false},
        {"getinvalid", "getinvalid", &getinvalid, true, true, false},

        /* Mining */
        {"mining", "getblocktemplate", &getblocktemplate, true, false, false},
        {"mining", "getpoablocktemplate", &getpoablocktemplate, true, false, false},
        {"mining", "setminingnbits", &setminingnbits, true, false, false},
        {"mining", "getmininginfo", &getmininginfo, true, false, false},
        {"mining", "getnetworkhashps", &getnetworkhashps, true, false, false},
        {"mining", "prioritisetransaction", &prioritisetransaction, true, false, false},
        {"mining", "submitblock", &submitblock, true, true, false},
        {"mining", "reservebalance", &reservebalance, true, true, false},

#ifdef ENABLE_WALLET
        /* Coin generation */
        {"generating", "getgenerate", &getgenerate, true, false, false},
        {"generating", "gethashespersec", &gethashespersec, true, false, false},
        {"generating", "setgenerate", &setgenerate, true, true, false},
        {"generating", "generatepoa", &generatepoa, true, true, false},
#endif

        /* Raw transactions */
        {"rawtransactions", "createrawtransaction", &createrawtransaction, true, false, false},
        {"rawtransactions", "decoderawtransaction", &decoderawtransaction, true, false, false},
        {"rawtransactions", "decodescript", &decodescript, true, false, false},
        {"rawtransactions", "getrawtransaction", &getrawtransaction, true, false, false},
        {"rawtransactions", "sendrawtransaction", &sendrawtransaction, false, false, false},
        {"rawtransactions", "getrawtransactionbyblockheight", &getrawtransactionbyblockheight, true, false, false},
        /* Utility functions */
        //{"util", "createmultisig", &createmultisig, true, true, false},
        // {"util", "validateaddress", &validateaddress, true, false, false}, /* uses wallet if enabled */
        // {"util", "verifymessage", &verifymessage, true, false, false},
        //{"util", "estimatefee", &estimatefee, true, true, false},
        // {"util", "estimatepriority", &estimatepriority, true, true, false},

        /* Not shown in help */
        // {"hidden", "invalidateblock", &invalidateblock, true, true, false},
        // {"hidden", "reconsiderblock", &reconsiderblock, true, true, false},
        // {"hidden", "setmocktime", &setmocktime, true, false, false},
        { "hidden", "waitfornewblock", &waitfornewblock, true, true, false},
        { "hidden", "waitforblock", &waitforblock, true, true, false},
        { "hidden", "waitforblockheight", &waitforblockheight, true, true, false},

        /* Prcycoin features */
         {"prcycoin", "masternode", &masternode, true, true, false},
         {"prcycoin", "listmasternodes", &listmasternodes, true, true, false},
         {"prcycoin", "getmasternodecount", &getmasternodecount, true, true, false},
         {"prcycoin", "getcurrentseesawreward", &getcurrentseesawreward, true, true, false},
         {"prcycoin", "getseesawrewardratio", &getseesawrewardratio, true, true, false},
         {"prcycoin", "getseesawrewardwithheight", &getseesawrewardwithheight, true, true, false},
         {"prcycoin", "masternodeconnect", &masternodeconnect, true, true, false},
         {"prcycoin", "masternodecurrent", &masternodecurrent, true, true, false},
         {"prcycoin", "masternodedebug", &masternodedebug, true, true, false},
         {"prcycoin", "startmasternode", &startmasternode, true, true, false},
         {"prcycoin", "listmasternodeconf", &listmasternodeconf, true, true, false},
         {"prcycoin", "getmasternodewinners", &getmasternodewinners, true, true, false},
         {"prcycoin", "getmasternodescores", &getmasternodescores, true, true, false},
         {"prcycoin", "createmasternodebroadcast", &createmasternodebroadcast, true, true, false},
         {"prcycoin", "decodemasternodebroadcast", &decodemasternodebroadcast, true, true, false},
         {"prcycoin", "relaymasternodebroadcast", &relaymasternodebroadcast, true, true, false},
         {"prcycoin", "mnsync", &mnsync, true, true, false},
         {"prcycoin", "getpoolinfo", &getpoolinfo, true, true, false},

#ifdef ENABLE_WALLET
        /* Wallet */
        // {"wallet", "addmultisigaddress", &addmultisigaddress, true, false, true},
        {"wallet", "autocombinedust", &autocombinedust, false, false, true},
        {"wallet", "backupwallet", &backupwallet, true, false, true},
        // {"wallet", "dumpprivkey", &dumpprivkey, true, false, true},
        // {"wallet", "dumpwallet", &dumpwallet, true, false, true},
        // {"wallet", "bip38encrypt", &bip38encrypt, true, false, true},
        // {"wallet", "bip38decrypt", &bip38decrypt, true, false, true},
        {"wallet", "encryptwallet", &encryptwallet, true, false, true},
        // {"wallet", "createprivacywallet", &createprivacywallet, true, false, true},
        {"wallet", "createprivacyaccount", &createprivacyaccount, true, false, true},
        {"wallet", "showstealthaddress", &showstealthaddress, true, false, true},
        {"wallet", "getaccountaddress", &showstealthaddress, true, false, true},
        {"wallet", "getstealthaddress", &showstealthaddress, true, false, true},
        {"wallet", "importkeys", &importkeys, true, false, true},
        {"wallet", "revealviewprivatekey", &revealviewprivatekey, true, false, true},
        {"wallet", "revealspendprivatekey", &revealspendprivatekey, true, false, true},
        {"wallet", "showtxprivatekeys", &showtxprivatekeys, true, false, true},
        {"wallet", "rescan", &rescan, true, false, true},
        {"wallet", "rescanwallettransactions", &rescanwallettransactions, true, false, true},
        {"wallet", "setdecoyconfirmation", &setdecoyconfirmation, true, false, true},
        {"wallet", "getdecoyconfirmation", &getdecoyconfirmation, true, false, true},
        {"wallet", "decodestealthaddress", &decodestealthaddress, true, false, true},
        {"wallet", "sendtostealthaddress", &sendtostealthaddress, false, false, true},
        {"wallet", "getbalance", &getbalance, false, false, true},
        {"wallet", "getbalances", &getbalances, false, false, true},
        {"wallet", "generateintegratedaddress", &generateintegratedaddress, true, false, false},
        {"wallet", "readmasteraccount", &readmasteraccount, true, false, false},
        // {"wallet", "getnewaddress", &getnewaddress, true, false, true},
        // {"wallet", "getrawchangeaddress", &getrawchangeaddress, true, false, true},
        // {"wallet", "getreceivedbyaccount", &getreceivedbyaccount, false, false, true},
        // {"wallet", "getreceivedbyaddress", &getreceivedbyaddress, false, false, true},
        {"wallet", "getstakingstatus", &getstakingstatus, false, false, true},
        // {"wallet", "getstakesplitthreshold", &getstakesplitthreshold, false, false, true},
        {"wallet", "gettransaction", &gettransaction, false, false, true},
        {"wallet", "getunconfirmedbalance", &getunconfirmedbalance, false, false, true},
        {"wallet", "getwalletinfo", &getwalletinfo, false, false, true},
        {"wallet", "gettxcount", &gettxcount, false, false, true},
        // {"wallet", "importprivkey", &importprivkey, true, false, true},
        // {"wallet", "importwallet", &importwallet, true, false, true},
        // {"wallet", "importaddress", &importaddress, true, false, true},
        // {"wallet", "keypoolrefill", &keypoolrefill, true, false, true},
        // {"wallet", "listaccounts", &listaccounts, false, false, true},
        // {"wallet", "listaddressgroupings", &listaddressgroupings, false, false, true},
        // {"wallet", "listlockunspent", &listlockunspent, false, false, true},
        // {"wallet", "listreceivedbyaccount", &listreceivedbyaccount, false, false, true},
        // {"wallet", "listreceivedbyaddress", &listreceivedbyaddress, false, false, true},
        {"wallet", "listsinceblock", &listsinceblock, false, false, true},
        {"wallet", "listtransactions", &listtransactions, false, false, true},
        {"wallet", "listunspent", &listunspent, false, false, true},
        // {"wallet", "lockunspent", &lockunspent, true, false, true},
        // {"wallet", "move", &movecmd, false, false, true},
        // {"wallet", "multisend", &multisend, false, false, true},
        // {"wallet", "sendfrom", &sendfrom, false, false, true},
        // {"wallet", "sendmany", &sendmany, false, false, true},
        // {"wallet", "sendtoaddress", &sendtoaddress, false, false, true},
        // {"wallet", "sendtoaddressix", &sendtoaddressix, false, false, true},
        // {"wallet", "setaccount", &setaccount, true, false, true},
        // {"wallet", "setstakesplitthreshold", &setstakesplitthreshold, false, false, true},
        // {"wallet", "settxfee", &settxfee, true, false, true},
        // {"wallet", "signmessage", &signmessage, true, false, true},
        {"wallet", "walletlock", &walletlock, true, false, true},
        {"wallet", "walletpassphrasechange", &walletpassphrasechange, true, false, true},
        {"wallet", "unlockwallet", &unlockwallet, true, false, true},
        {"wallet", "revealmnemonicphrase", &revealmnemonicphrase, true, false, true}

#endif // ENABLE_WALLET
        };

CRPCTable::CRPCTable() {
    unsigned int vcidx;
    for (vcidx = 0; vcidx < (sizeof(vRPCCommands) / sizeof(vRPCCommands[0])); vcidx++) {
        const CRPCCommand *pcmd;
=======
  //  category              name                      actor (function)         okSafe argNames
  //  --------------------- ------------------------  -----------------------  ------ ----------
    /* Overall control/query calls */
    { "control",            "help",                   &help,                   true,  {"command"}  },
    { "control",            "stop",                   &stop,                   true,  {}  },
};

CRPCTable::CRPCTable()
{
    unsigned int vcidx;
    for (vcidx = 0; vcidx < (sizeof(vRPCCommands) / sizeof(vRPCCommands[0])); vcidx++) {
        const CRPCCommand* pcmd;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        pcmd = &vRPCCommands[vcidx];
        mapCommands[pcmd->name] = pcmd;
    }
}

<<<<<<< HEAD
const CRPCCommand *CRPCTable::operator[](const std::string &name) const {
    map<string, const CRPCCommand *>::const_iterator it = mapCommands.find(name);
=======
const CRPCCommand *CRPCTable::operator[](const std::string &name) const
{
    std::map<std::string, const CRPCCommand*>::const_iterator it = mapCommands.find(name);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (it == mapCommands.end())
        return NULL;
    return (*it).second;
}

<<<<<<< HEAD

bool StartRPC() {
    LogPrint("rpc", "Starting RPC\n");
=======
bool CRPCTable::appendCommand(const std::string& name, const CRPCCommand* pcmd)
{
    if (IsRPCRunning())
        return false;

    // don't allow overwriting for now
    std::map<std::string, const CRPCCommand*>::const_iterator it = mapCommands.find(name);
    if (it != mapCommands.end())
        return false;

    mapCommands[name] = pcmd;
    return true;
}

bool StartRPC()
{
    LogPrint(BCLog::RPC, "Starting RPC\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    fRPCRunning = true;
    g_rpcSignals.Started();
    return true;
}

<<<<<<< HEAD
void InterruptRPC() {
    LogPrint("rpc", "Interrupting RPC\n");
    // Interrupt e.g. running longpolls
    // Interrupt e.g. running longpolls
}

void StopRPC() {
    LogPrint("rpc", "Stopping RPC\n");
    deadlineTimers.clear();
}

bool IsRPCRunning() {
    return fRPCRunning;
}

void SetRPCWarmupStatus(const std::string &newStatus) {
=======
void InterruptRPC()
{
    LogPrint(BCLog::RPC, "Interrupting RPC\n");
    // Interrupt e.g. running longpolls
    fRPCRunning = false;
}

void StopRPC()
{
    LogPrint(BCLog::RPC, "Stopping RPC\n");
    deadlineTimers.clear();
    DeleteAuthCookie();
    g_rpcSignals.Stopped();
}

bool IsRPCRunning()
{
    return fRPCRunning;
}

void SetRPCWarmupStatus(const std::string& newStatus)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_rpcWarmup);
    rpcWarmupStatus = newStatus;
}

<<<<<<< HEAD
void SetRPCWarmupFinished() {
=======
void SetRPCWarmupFinished()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_rpcWarmup);
    assert(fRPCInWarmup);
    fRPCInWarmup = false;
}

<<<<<<< HEAD
bool RPCIsInWarmup(std::string *outStatus) {
=======
bool RPCIsInWarmup(std::string* outStatus)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_rpcWarmup);
    if (outStatus)
        *outStatus = rpcWarmupStatus;
    return fRPCInWarmup;
}

<<<<<<< HEAD
void JSONRequest::parse(const UniValue& valRequest)
=======
void JSONRPCRequest::parse(const UniValue& valRequest)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // Parse request
    if (!valRequest.isObject())
        throw JSONRPCError(RPC_INVALID_REQUEST, "Invalid Request object");
    const UniValue& request = valRequest.get_obj();

    // Parse id now so errors from here on will have the id
    id = find_value(request, "id");

    // Parse method
    UniValue valMethod = find_value(request, "method");
    if (valMethod.isNull())
        throw JSONRPCError(RPC_INVALID_REQUEST, "Missing method");
    if (!valMethod.isStr())
        throw JSONRPCError(RPC_INVALID_REQUEST, "Method must be a string");
    strMethod = valMethod.get_str();
    if (strMethod != "getblocktemplate")
<<<<<<< HEAD
        LogPrint("rpc", "ThreadRPCServer method=%s\n", SanitizeString(strMethod));

    // Parse params
    UniValue valParams = find_value(request, "params");
    if (valParams.isArray())
        params = valParams.get_array();
    else if (valParams.isNull())
        params = UniValue(UniValue::VARR);
    else
        throw JSONRPCError(RPC_INVALID_REQUEST, "Params must be an array");
}


=======
        LogPrint(BCLog::RPC, "ThreadRPCServer method=%s\n", SanitizeString(strMethod));

    // Parse params
    UniValue valParams = find_value(request, "params");
    if (valParams.isArray() || valParams.isObject())
        params = valParams;
    else if (valParams.isNull())
        params = UniValue(UniValue::VARR);
    else
        throw JSONRPCError(RPC_INVALID_REQUEST, "Params must be an array or object");
}

bool IsDeprecatedRPCEnabled(const std::string& method)
{
    const std::vector<std::string> enabled_methods = gArgs.GetArgs("-deprecatedrpc");

    return find(enabled_methods.begin(), enabled_methods.end(), method) != enabled_methods.end();
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static UniValue JSONRPCExecOne(const UniValue& req)
{
    UniValue rpc_result(UniValue::VOBJ);

<<<<<<< HEAD
    JSONRequest jreq;
    try {
        jreq.parse(req);

        UniValue result = tableRPC.execute(jreq.strMethod, jreq.params);
=======
    JSONRPCRequest jreq;
    try {
        jreq.parse(req);

        UniValue result = tableRPC.execute(jreq);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        rpc_result = JSONRPCReplyObj(result, NullUniValue, jreq.id);
    } catch (const UniValue& objError) {
        rpc_result = JSONRPCReplyObj(NullUniValue, objError, jreq.id);
    } catch (const std::exception& e) {
        rpc_result = JSONRPCReplyObj(NullUniValue,
<<<<<<< HEAD
                                     JSONRPCError(RPC_PARSE_ERROR, e.what()), jreq.id);
=======
            JSONRPCError(RPC_PARSE_ERROR, e.what()), jreq.id);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return rpc_result;
}

<<<<<<< HEAD
std::string JSONRPCExecBatch(const UniValue &vReq) {
=======
std::string JSONRPCExecBatch(const UniValue& vReq)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    UniValue ret(UniValue::VARR);
    for (unsigned int reqIdx = 0; reqIdx < vReq.size(); reqIdx++)
        ret.push_back(JSONRPCExecOne(vReq[reqIdx]));

    return ret.write() + "\n";
}

<<<<<<< HEAD
UniValue CRPCTable::execute(const std::string &strMethod, const UniValue &params) const {
=======
/**
 * Process named arguments into a vector of positional arguments, based on the
 * passed-in specification for the RPC call's arguments.
 */
static inline JSONRPCRequest transformNamedArguments(const JSONRPCRequest& in, const std::vector<std::string>& argNames)
{
    JSONRPCRequest out = in;
    out.params = UniValue(UniValue::VARR);
    // Build a map of parameters, and remove ones that have been processed, so that we can throw a focused error if
    // there is an unknown one.
    const std::vector<std::string>& keys = in.params.getKeys();
    const std::vector<UniValue>& values = in.params.getValues();
    std::unordered_map<std::string, const UniValue*> argsIn;
    for (size_t i=0; i<keys.size(); ++i) {
        argsIn[keys[i]] = &values[i];
    }
    // Process expected parameters.
    int hole = 0;
    for (const std::string &argName: argNames) {
        auto fr = argsIn.find(argName);
        if (fr != argsIn.end()) {
            for (int i = 0; i < hole; ++i) {
                // Fill hole between specified parameters with JSON nulls,
                // but not at the end (for backwards compatibility with calls
                // that act based on number of specified parameters).
                out.params.push_back(UniValue());
            }
            hole = 0;
            out.params.push_back(*fr->second);
            argsIn.erase(fr);
        } else {
            hole += 1;
        }
    }
    // If there are still arguments in the argsIn map, this is an error.
    if (!argsIn.empty()) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Unknown named parameter " + argsIn.begin()->first);
    }
    // Return request with named arguments transformed to positional arguments
    return out;
}

UniValue CRPCTable::execute(const JSONRPCRequest &request) const
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Return immediately if in warmup
    std::string strWarmupStatus;
    if (RPCIsInWarmup(&strWarmupStatus)) {
        throw JSONRPCError(RPC_IN_WARMUP, "RPC in warm-up: " + strWarmupStatus);
    }

    // Find method
<<<<<<< HEAD
    const CRPCCommand *pcmd = tableRPC[strMethod];
    if (!pcmd)
        throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Method not found");

=======
    const CRPCCommand* pcmd = tableRPC[request.strMethod];
    if (!pcmd)
        throw JSONRPCError(RPC_METHOD_NOT_FOUND, strprintf("Method not found: %s", request.strMethod));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    g_rpcSignals.PreCommand(*pcmd);

    try {
<<<<<<< HEAD
        return pcmd->actor(params, false);
    } catch (const std::exception& e) {
        throw JSONRPCError(RPC_MISC_ERROR, e.what());
    }

    g_rpcSignals.PostCommand(*pcmd);
}

std::vector <std::string> CRPCTable::listCommands() const {
    std::vector <std::string> commandList;
    typedef std::map<std::string, const CRPCCommand *> commandMap;

    std::transform(mapCommands.begin(), mapCommands.end(),
                   std::back_inserter(commandList),
                   boost::bind(&commandMap::value_type::first, _1));
    return commandList;
}

std::string HelpExampleCli(string methodname, string args) {
    return "> prcycoin-cli " + methodname + " " + args + "\n";
}

std::string HelpExampleRpc(string methodname, string args) {
    return "> curl --user myusername --data-binary '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", "
           "\"method\": \"" +
           methodname + "\", \"params\": [" + args + "] }' -H 'content-type: text/plain;' http://127.0.0.1:59683/\n";
}

void RPCSetTimerInterfaceIfUnset(RPCTimerInterface *iface) {
=======
        // Execute, convert arguments to array if necessary
        if (request.params.isObject()) {
            return pcmd->actor(transformNamedArguments(request, pcmd->argNames));
        } else {
            return pcmd->actor(request);
        }
    } catch (const std::exception& e) {
        throw JSONRPCError(RPC_MISC_ERROR, e.what());
    }
}

std::vector<std::string> CRPCTable::listCommands() const
{
    std::vector<std::string> commandList;
    for (const auto& i : mapCommands) commandList.emplace_back(i.first);
    return commandList;
}

std::string HelpExampleCli(std::string methodname, std::string args)
{
    return "> pivx-cli " + methodname + " " + args + "\n";
}

std::string HelpExampleRpc(std::string methodname, std::string args)
{
    return "> curl --user myusername --data-binary '{\"jsonrpc\": \"1.0\", \"id\":\"curltest\", "
           "\"method\": \"" +
           methodname + "\", \"params\": [" + args + "] }' -H 'content-type: text/plain;' http://127.0.0.1:51473/\n";
}

void RPCSetTimerInterfaceIfUnset(RPCTimerInterface *iface)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!timerInterface)
        timerInterface = iface;
}

<<<<<<< HEAD
void RPCSetTimerInterface(RPCTimerInterface *iface) {
    timerInterface = iface;
}

void RPCUnsetTimerInterface(RPCTimerInterface *iface) {
=======
void RPCSetTimerInterface(RPCTimerInterface *iface)
{
    timerInterface = iface;
}

void RPCUnsetTimerInterface(RPCTimerInterface *iface)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (timerInterface == iface)
        timerInterface = NULL;
}

<<<<<<< HEAD
void RPCRunLater(const std::string &name, boost::function<void(void)> func, int64_t nSeconds) {
    if (!timerInterface)
        throw JSONRPCError(RPC_INTERNAL_ERROR, "No timer handler registered for RPC");
    deadlineTimers.erase(name);
    LogPrint("rpc", "queue run of timer %s in %i seconds (using %s)\n", name, nSeconds, timerInterface->Name());
    deadlineTimers.insert(
            std::make_pair(name, boost::shared_ptr<RPCTimerBase>(timerInterface->NewTimer(func, nSeconds * 1000))));
}

const CRPCTable tableRPC;
=======
void RPCRunLater(const std::string& name, std::function<void(void)> func, int64_t nSeconds)
{
    if (!timerInterface)
        throw JSONRPCError(RPC_INTERNAL_ERROR, "No timer handler registered for RPC");
    deadlineTimers.erase(name);
    LogPrint(BCLog::RPC, "queue run of timer %s in %i seconds (using %s)\n", name, nSeconds, timerInterface->Name());
    deadlineTimers.emplace(name, std::unique_ptr<RPCTimerBase>(timerInterface->NewTimer(func, nSeconds*1000)));
}

CRPCTable tableRPC;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
