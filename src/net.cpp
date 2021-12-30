// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
<<<<<<< HEAD
#include "config/prcycoin-config.h"
=======
#include "config/pivx-config.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif

#include "net.h"

<<<<<<< HEAD
#include "addrman.h"
#include "chainparams.h"
#include "clientversion.h"
#include "miner.h"
#include "obfuscation.h"
#include "primitives/transaction.h"
#include "scheduler.h"
#include "guiinterface.h"
=======
#include "chainparams.h"
#include "clientversion.h"
#include "crypto/common.h"
#include "crypto/sha256.h"
#include "guiinterface.h"
#include "netaddress.h"
#include "netbase.h"
#include "netmessagemaker.h"
#include "optional.h"
#include "primitives/transaction.h"
#include "scheduler.h"
#include "validation.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifdef WIN32
#include <string.h>
#else
<<<<<<< HEAD

#include <fcntl.h>

#endif

#ifdef USE_UPNP
#include <miniupnpc/miniupnpc.h>
#include <miniupnpc/miniwget.h>
#include <miniupnpc/upnpcommands.h>
#include <miniupnpc/upnperrors.h>
#endif

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
=======
#include <fcntl.h>
#endif

#if HAVE_DECL_GETIFADDRS && HAVE_DECL_FREEIFADDRS
#include <ifaddrs.h>
#endif

#include <cstdint>

#include <math.h>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

// Dump addresses to peers.dat and banlist.dat every 15 minutes (900s)
#define DUMP_ADDRESSES_INTERVAL 900

<<<<<<< HEAD
=======
// We add a random period time (0 to 1 seconds) to feeler connections to prevent synchronization.
#define FEELER_SLEEP_WINDOW 1

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#if !defined(HAVE_MSG_NOSIGNAL) && !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

// Fix for ancient MinGW versions, that don't have defined these in ws2tcpip.h.
// Todo: Can be removed when our pull-tester is upgraded to a modern MinGW version.
#ifdef WIN32
#ifndef PROTECTION_LEVEL_UNRESTRICTED
#define PROTECTION_LEVEL_UNRESTRICTED 10
#endif
#ifndef IPV6_PROTECTION_LEVEL
#define IPV6_PROTECTION_LEVEL 23
#endif
#endif

<<<<<<< HEAD
using namespace boost;
using namespace std;

namespace {
    const int MAX_OUTBOUND_CONNECTIONS = 16;

    struct ListenSocket {
        SOCKET socket;
        bool whitelisted;

        ListenSocket(SOCKET socket, bool whitelisted) : socket(socket), whitelisted(whitelisted) {}
    };
}

=======
/** Used to pass flags to the Bind() function */
enum BindFlags {
    BF_NONE         = 0,
    BF_EXPLICIT     = (1U << 0),
    BF_REPORT_ERROR = (1U << 1),
    BF_WHITELIST    = (1U << 2),
};

const static std::string NET_MESSAGE_COMMAND_OTHER = "*other*";

static const uint64_t RANDOMIZER_ID_NETGROUP = 0x6c0edd8036ef4036ULL; // SHA256("netgroup")[0:8]
static const uint64_t RANDOMIZER_ID_LOCALHOSTNONCE = 0xd93e69e2bbfa5735ULL; // SHA256("localhostnonce")[0:8]
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
//
// Global state variables
//
bool fDiscover = true;
bool fListen = true;
<<<<<<< HEAD
uint64_t nLocalServices = NODE_NETWORK;
RecursiveMutex cs_mapLocalHost;
map <CNetAddr, LocalServiceInfo> mapLocalHost;
//static bool vfReachable[NET_MAX] = {};
static bool vfLimited[NET_MAX] = {};
static CNode *pnodeLocalHost = NULL;
uint64_t nLocalHostNonce = 0;
static std::vector <ListenSocket> vhListenSocket;
CAddrMan addrman;
int nMaxConnections = DEFAULT_MAX_PEER_CONNECTIONS;
bool fAddressesInitialized = false;
std::string strSubVersion;

vector<CNode *> vNodes;
RecursiveMutex cs_vNodes;
map <CInv, CDataStream> mapRelay;
deque <pair<int64_t, CInv>> vRelayExpiration;
RecursiveMutex cs_mapRelay;
limitedmap<CInv, int64_t> mapAlreadyAskedFor(MAX_INV_SZ);

static deque <string> vOneShots;
RecursiveMutex cs_vOneShots;

set <CNetAddr> setservAddNodeAddresses;
RecursiveMutex cs_setservAddNodeAddresses;

vector <std::string> vAddedNodes;
RecursiveMutex cs_vAddedNodes;

NodeId nLastNodeId = 0;
RecursiveMutex cs_nLastNodeId;

static CSemaphore *semOutbound = NULL;
boost::condition_variable messageHandlerCondition;

// Signals for message handling
static CNodeSignals g_signals;

CNodeSignals &GetNodeSignals() { return g_signals; }

void AddOneShot(string strDest) {
=======
RecursiveMutex cs_mapLocalHost;
std::map<CNetAddr, LocalServiceInfo> mapLocalHost;
static bool vfLimited[NET_MAX] = {};
std::string strSubVersion;

limitedmap<CInv, int64_t> mapAlreadyAskedFor(MAX_INV_SZ);

void CConnman::AddOneShot(const std::string& strDest)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_vOneShots);
    vOneShots.push_back(strDest);
}

<<<<<<< HEAD
unsigned short GetListenPort() {
    return (unsigned short) (GetArg("-port", Params().GetDefaultPort()));
}

bool IsUnsupportedVersion(std::string strSubVer, int nHeight) {
    if (nHeight >= Params().HardFork()) {
        return (strSubVer == "/PRCY:1.0.0.2/" || strSubVer == "/PRCY:1.0.0.3/" || strSubVer == "/PRCY:1.0.0.4/" || strSubVer == "/PRCY:1.0.0.5/" || strSubVer == "/PRCY:1.0.0.6/" || strSubVer == "/PRCY:1.0.0.7/");
    }
    return (strSubVer == "/PRCY:1.0.0.2/" || strSubVer == "/PRCY:1.0.0.3/" || strSubVer == "/PRCY:1.0.0.4/" || strSubVer == "/PRCY:1.0.0.5/" || strSubVer == "/PRCY:1.0.0.6/");
}

// find 'best' local address for a particular peer
bool GetLocal(CService &addr, const CNetAddr *paddrPeer) {
=======
uint16_t GetListenPort()
{
    return (uint16_t)(gArgs.GetArg("-port", Params().GetDefaultPort()));
}

// find 'best' local address for a particular peer
bool GetLocal(CService& addr, const CNetAddr* paddrPeer)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fListen)
        return false;

    int nBestScore = -1;
    int nBestReachability = -1;
    {
        LOCK(cs_mapLocalHost);
<<<<<<< HEAD
        for (map<CNetAddr, LocalServiceInfo>::iterator it = mapLocalHost.begin(); it != mapLocalHost.end(); it++) {
            int nScore = (*it).second.nScore;
            int nReachability = (*it).first.GetReachabilityFrom(paddrPeer);
            if (nReachability > nBestReachability || (nReachability == nBestReachability && nScore > nBestScore)) {
                addr = CService((*it).first, (*it).second.nPort);
=======
        for (const auto& entry : mapLocalHost) {
            int nScore = entry.second.nScore;
            int nReachability = entry.first.GetReachabilityFrom(paddrPeer);
            if (nReachability > nBestReachability || (nReachability == nBestReachability && nScore > nBestScore)) {
                addr = CService(entry.first, entry.second.nPort);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                nBestReachability = nReachability;
                nBestScore = nScore;
            }
        }
    }
    return nBestScore >= 0;
}

<<<<<<< HEAD
//! Convert the pnSeeds6 array into usable address objects.
static std::vector<CAddress> convertSeed6(const std::vector<SeedSpec6>& vSeedsIn)
=======
//! Convert the serialized seeds into usable address objects.
static std::vector<CAddress> ConvertSeeds(const std::vector<uint8_t> &vSeedsIn)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    std::vector<CAddress> vSeedsOut;
<<<<<<< HEAD
    vSeedsOut.reserve(vSeedsIn.size());
    for (std::vector<SeedSpec6>::const_iterator i(vSeedsIn.begin()); i != vSeedsIn.end(); ++i) {
        struct in6_addr ip;
        memcpy(&ip, i->addr, sizeof(ip));
        CAddress addr(CService(ip, i->port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
=======
    FastRandomContext rng;
    CDataStream s(vSeedsIn, SER_NETWORK, PROTOCOL_VERSION | ADDRV2_FORMAT);
    while (!s.eof()) {
        CService endpoint;
        s >> endpoint;
        CAddress addr{endpoint, NODE_NETWORK};
        addr.nTime = GetTime() - rng.randrange(nOneWeek) - nOneWeek;
        LogPrint(BCLog::NET, "Added hardcoded seed: %s\n", addr.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        vSeedsOut.push_back(addr);
    }
    return vSeedsOut;
}

// get best local address for a particular peer as a CAddress
// Otherwise, return the unroutable 0.0.0.0 but filled in with
// the normal parameters, since the IP may be changed to a useful
// one by discovery.
<<<<<<< HEAD
CAddress GetLocalAddress(const CNetAddr *paddrPeer) {
    CAddress ret(CService("0.0.0.0", GetListenPort()), 0);
    CService addr;
    if (GetLocal(addr, paddrPeer)) {
        ret = CAddress(addr);
    }
    ret.nServices = nLocalServices;
=======
CAddress GetLocalAddress(const CNetAddr* paddrPeer, ServiceFlags nLocalServices)
{
    CAddress ret(CService(CNetAddr(), GetListenPort()), nLocalServices);
    CService addr;
    if (GetLocal(addr, paddrPeer)) {
        ret = CAddress(addr, nLocalServices);
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ret.nTime = GetAdjustedTime();
    return ret;
}

<<<<<<< HEAD
bool RecvLine(SOCKET hSocket, string &strLine) {
    strLine = "";
    while (true) {
        char c;
        int nBytes = recv(hSocket, &c, 1, 0);
        if (nBytes > 0) {
            if (c == '\n')
                continue;
            if (c == '\r')
                return true;
            strLine += c;
            if (strLine.size() >= 9000)
                return true;
        } else if (nBytes <= 0) {
            boost::this_thread::interruption_point();
            if (nBytes < 0) {
                int nErr = WSAGetLastError();
                if (nErr == WSAEMSGSIZE)
                    continue;
                if (nErr == WSAEWOULDBLOCK || nErr == WSAEINTR || nErr == WSAEINPROGRESS) {
                    MilliSleep(10);
                    continue;
                }
            }
            if (!strLine.empty())
                return true;
            if (nBytes == 0) {
                // socket closed
                LogPrint("net", "socket closed\n");
                return false;
            } else {
                // socket error
                int nErr = WSAGetLastError();
                LogPrint("net", "recv failed: %s\n", NetworkErrorString(nErr));
                return false;
            }
        }
    }
}

int GetnScore(const CService &addr) {
=======
int GetnScore(const CService& addr)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_mapLocalHost);
    if (mapLocalHost.count(addr) == LOCAL_NONE)
        return 0;
    return mapLocalHost[addr].nScore;
}

// Is our peer's addrLocal potentially useful as an external IP source?
<<<<<<< HEAD
bool IsPeerAddrLocalGood(CNode *pnode) {
    return fDiscover && pnode->addr.IsRoutable() && pnode->addrLocal.IsRoutable() &&
           !IsLimited(pnode->addrLocal.GetNetwork());
=======
bool IsPeerAddrLocalGood(CNode* pnode)
{
    CService addrLocal = pnode->GetAddrLocal();
    return fDiscover && pnode->addr.IsRoutable() && addrLocal.IsRoutable() &&
           IsReachable(addrLocal.GetNetwork());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

// pushes our own address to a peer
void AdvertiseLocal(CNode* pnode)
{
    if (fListen && pnode->fSuccessfullyConnected) {
<<<<<<< HEAD
        CAddress addrLocal = GetLocalAddress(&pnode->addr);
        // If discovery is enabled, sometimes give our peer the address it
        // tells us that it sees us as in case it has a better idea of our
        // address than we do.
        if (IsPeerAddrLocalGood(pnode) && (!addrLocal.IsRoutable() ||
                                           GetRand((GetnScore(addrLocal) > LOCAL_MANUAL) ? 8 : 2) == 0)) {
            addrLocal.SetIP(pnode->addrLocal);
        }
        if (addrLocal.IsRoutable()) {
            LogPrintf("%s: advertising address %s\n", __func__, addrLocal.ToString());
            FastRandomContext insecure_rand;
            pnode->PushAddress(addrLocal, insecure_rand);
=======
        CAddress addrLocal = GetLocalAddress(&pnode->addr, pnode->GetLocalServices());
        // If discovery is enabled, sometimes give our peer the address it
        // tells us that it sees us as in case it has a better idea of our
        // address than we do.
        FastRandomContext rng;
        if (IsPeerAddrLocalGood(pnode) && (!addrLocal.IsRoutable() ||
             rng.randbits((GetnScore(addrLocal) > LOCAL_MANUAL) ? 3 : 1) == 0)) {
            addrLocal.SetIP(pnode->GetAddrLocal());
        }
        if (addrLocal.IsRoutable()) {
            LogPrintf("%s: advertising address %s\n", __func__, addrLocal.ToString());
            pnode->PushAddress(addrLocal, rng);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }
}

// learn a new local address
<<<<<<< HEAD
bool AddLocal(const CService &addr, int nScore) {
=======
bool AddLocal(const CService& addr, int nScore)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!addr.IsRoutable())
        return false;

    if (!fDiscover && nScore < LOCAL_MANUAL)
        return false;

<<<<<<< HEAD
    if (IsLimited(addr))
=======
    if (!IsReachable(addr))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;

    LogPrintf("AddLocal(%s,%i)\n", addr.ToString(), nScore);

    {
        LOCK(cs_mapLocalHost);
        bool fAlready = mapLocalHost.count(addr) > 0;
<<<<<<< HEAD
        LocalServiceInfo &info = mapLocalHost[addr];
=======
        LocalServiceInfo& info = mapLocalHost[addr];
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!fAlready || nScore >= info.nScore) {
            info.nScore = nScore + (fAlready ? 1 : 0);
            info.nPort = addr.GetPort();
        }
    }

    return true;
}

<<<<<<< HEAD
bool AddLocal(const CNetAddr &addr, int nScore) {
    return AddLocal(CService(addr, GetListenPort()), nScore);
}

bool RemoveLocal(const CService &addr) {
=======
bool AddLocal(const CNetAddr& addr, int nScore)
{
    return AddLocal(CService(addr, GetListenPort()), nScore);
}

bool RemoveLocal(const CService& addr)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_mapLocalHost);
    LogPrintf("RemoveLocal(%s)\n", addr.ToString());
    mapLocalHost.erase(addr);
    return true;
}

<<<<<<< HEAD
/** Make a particular network entirely off-limits (no automatic connects to it) */
void SetLimited(enum Network net, bool fLimited) {
    if (net == NET_UNROUTABLE)
        return;
    LOCK(cs_mapLocalHost);
    vfLimited[net] = fLimited;
}

bool IsLimited(enum Network net) {
    LOCK(cs_mapLocalHost);
    return vfLimited[net];
}

bool IsLimited(const CNetAddr &addr) {
    return IsLimited(addr.GetNetwork());
}

/** vote for a local address */
bool SeenLocal(const CService &addr) {
=======
void SetReachable(enum Network net, bool reachable)
{
    if (net == NET_UNROUTABLE || net == NET_INTERNAL)
        return;
    LOCK(cs_mapLocalHost);
    vfLimited[net] = !reachable;
}

bool IsReachable(enum Network net)
{
    LOCK(cs_mapLocalHost);
    return !vfLimited[net];
}

bool IsReachable(const CNetAddr& addr)
{
    return IsReachable(addr.GetNetwork());
}

/** vote for a local address */
bool SeenLocal(const CService& addr)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_mapLocalHost);
        if (mapLocalHost.count(addr) == 0)
            return false;
        mapLocalHost[addr].nScore++;
    }
    return true;
}


/** check whether a given address is potentially local */
<<<<<<< HEAD
bool IsLocal(const CService &addr) {
=======
bool IsLocal(const CService& addr)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_mapLocalHost);
    return mapLocalHost.count(addr) > 0;
}

<<<<<<< HEAD
/** check whether a given network is one we can probably connect to */
bool IsReachable(enum Network net) {
    LOCK(cs_mapLocalHost);
    return !vfLimited[net];
}

/** check whether a given address is in a network we can probably connect to */
bool IsReachable(const CNetAddr &addr) {
    enum Network net = addr.GetNetwork();
    return !vfLimited[net];
}

void AddressCurrentlyConnected(const CService &addr) {
    addrman.Connected(addr);
}


uint64_t CNode::nTotalBytesRecv = 0;
uint64_t CNode::nTotalBytesSent = 0;
RecursiveMutex CNode::cs_totalBytesRecv;
RecursiveMutex CNode::cs_totalBytesSent;

CNode *FindNode(const CNetAddr &ip) {
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    if ((CNetAddr) pnode->addr == ip)
        return (pnode);
    return NULL;
}

CNode *FindNode(const CSubNet &subNet) {
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    if (subNet.Match((CNetAddr) pnode->addr))
        return (pnode);
    return NULL;
}

CNode *FindNode(const std::string &addrName) {
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    if (pnode->addrName == addrName)
        return (pnode);
    return NULL;
}

CNode *FindNode(const CService &addr) {
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    {
        if (Params().NetworkID() == CBaseChainParams::REGTEST) {
            //if using regtest, just check the IP
            if ((CNetAddr) pnode->addr == (CNetAddr) addr)
                return (pnode);
        } else {
            if (pnode->addr == addr)
                return (pnode);
        }
    }
    return NULL;
}

CNode *ConnectNode(CAddress addrConnect, const char *pszDest, bool obfuScationMaster) {
    if (pszDest == NULL) {
        // we clean masternode connections in CMasternodeMan::ProcessMasternodeConnections()
        // so should be safe to skip this and connect to local Hot MN on CActiveMasternode::ManageStatus()
        if (IsLocal(addrConnect) && !obfuScationMaster)
            return NULL;

        // Look for an existing connection
        CNode *pnode = FindNode((CService) addrConnect);
        if (pnode) {
            pnode->fObfuScationMaster = obfuScationMaster;

            pnode->AddRef();
            return pnode;
=======
CNode* CConnman::FindNode(const CNetAddr& ip)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (static_cast<CNetAddr>(pnode->addr) == ip) {
            return pnode;
        }
    }
    return nullptr;
}

CNode* CConnman::FindNode(const CSubNet& subNet)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (subNet.Match(static_cast<CNetAddr>(pnode->addr))) {
            return pnode;
        }
    }
    return nullptr;
}

CNode* CConnman::FindNode(const std::string& addrName)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (pnode->GetAddrName() == addrName) {
            return pnode;
        }
    }
    return NULL;
}

CNode* CConnman::FindNode(const CService& addr)
{
    LOCK(cs_vNodes);
    const bool isRegTestNet = Params().IsRegTestNet();
    for (CNode* pnode : vNodes) {
        if (isRegTestNet) {
            //if using regtest, just check the IP
            if (static_cast<CNetAddr>(pnode->addr) == static_cast<CNetAddr>(addr))
                return pnode;
        } else {
            if (pnode->addr == addr)
                return pnode;
        }
    }
    return nullptr;
}

bool CConnman::CheckIncomingNonce(uint64_t nonce)
{
    LOCK(cs_vNodes);
    for(CNode* pnode : vNodes) {
        if (!pnode->fSuccessfullyConnected && !pnode->fInbound && pnode->GetLocalNonce() == nonce)
            return false;
    }
    return true;
}

CNode* CConnman::ConnectNode(CAddress addrConnect, const char* pszDest, bool fCountFailure)
{
    if (pszDest == nullptr) {
        if (IsLocal(addrConnect)) {
            LogPrintf("%s: cannot connect to local node\n", __func__);
            return nullptr;
        }

        // Look for an existing connection
        CNode* pnode = FindNode(static_cast<CService>(addrConnect));
        if (pnode) {
            LogPrintf("Failed to open new connection, already connected\n");
            return nullptr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    /// debug print
<<<<<<< HEAD
    LogPrint("net", "trying connection %s lastseen=%.1fhrs\n",
             pszDest ? pszDest : addrConnect.ToString(),
             pszDest ? 0.0 : (double) (GetAdjustedTime() - addrConnect.nTime) / 3600.0);

    // Connect
    SOCKET hSocket = INVALID_SOCKET;
    bool proxyConnectionFailed = false;
    if (pszDest ? ConnectSocketByName(addrConnect, hSocket, pszDest, Params().GetDefaultPort(), nConnectTimeout,
                                      &proxyConnectionFailed) :
        ConnectSocket(addrConnect, hSocket, nConnectTimeout, &proxyConnectionFailed)) {
        if (!IsSelectableSocket(hSocket)) {
            LogPrintf("Cannot create connection: non-selectable socket created (fd >= FD_SETSIZE ?)\n");
            CloseSocket(hSocket);
            return NULL;
        }

        addrman.Attempt(addrConnect);

        // Add node
        CNode *pnode = new CNode(hSocket, addrConnect, pszDest ? pszDest : "", false);
        pnode->AddRef();

        {
            LOCK(cs_vNodes);
            vNodes.push_back(pnode);
        }

        pnode->nTimeConnected = GetTime();
        if (obfuScationMaster) pnode->fObfuScationMaster = true;

        return pnode;
    } else if (!proxyConnectionFailed) {
        // If connecting to the node failed, and failure is not caused by a problem connecting to
        // the proxy, mark this as an attempt.
        addrman.Attempt(addrConnect);
    }

    return NULL;
}

void CNode::CloseSocketDisconnect() {
    fDisconnect = true;
    if (hSocket != INVALID_SOCKET) {
        LogPrint("net", "disconnecting peer=%d\n", id);
        CloseSocket(hSocket);
    }

    // in case this fails, we'll empty the recv buffer when the CNode is deleted
    TRY_LOCK(cs_vRecvMsg, lockRecv);
    if (lockRecv)
        vRecvMsg.clear();
}

bool CNode::DisconnectOldProtocol(int nVersionRequired, string strLastCommand) {
    fDisconnect = false;
    if (nVersion < nVersionRequired) {
        LogPrintf("%s : peer=%d using obsolete version %i; disconnecting\n", __func__, id, nVersion);
        PushMessage("reject", strLastCommand, REJECT_OBSOLETE,
                    strprintf("Version must be %d or greater", ActiveProtocol()));
        fDisconnect = true;
    }

    return fDisconnect;
}

void CNode::PushVersion() {
    int nBestHeight = g_signals.GetHeight().get_value_or(0);

    /// when NTP implemented, change to just nTime = GetAdjustedTime()
    int64_t nTime = (fInbound ? GetAdjustedTime() : GetTime());
    CAddress addrYou = (addr.IsRoutable() && !IsProxy(addr) ? addr : CAddress(CService("0.0.0.0", 0)));
    CAddress addrMe = GetLocalAddress(&addr);
    GetRandBytes((unsigned char *) &nLocalHostNonce, sizeof(nLocalHostNonce));
    if (fLogIPs)
        LogPrint("net", "send version message: version %d, blocks=%d, us=%s, them=%s, peer=%d\n", PROTOCOL_VERSION,
                 nBestHeight, addrMe.ToString(), addrYou.ToString(), id);
    else
        LogPrint("net", "send version message: version %d, blocks=%d, us=%s, peer=%d\n", PROTOCOL_VERSION, nBestHeight,
                 addrMe.ToString(), id);
    PushMessage("version", PROTOCOL_VERSION, nLocalServices, nTime, addrYou, addrMe,
        nLocalHostNonce, strSubVersion, nBestHeight, true);
}


banmap_t CNode::setBanned;
RecursiveMutex CNode::cs_setBanned;
bool CNode::setBannedIsDirty;

void CNode::ClearBanned() {
=======
    LogPrint(BCLog::NET, "trying connection %s lastseen=%.1fhrs\n",
        pszDest ? pszDest : addrConnect.ToString(),
        pszDest ? 0.0 : (double)(GetAdjustedTime() - addrConnect.nTime) / 3600.0);

    // Resolve
    const int default_port = Params().GetDefaultPort();
    if (pszDest) {
        std::vector<CService> resolved;
        if (Lookup(pszDest, resolved,  default_port, fNameLookup && !HaveNameProxy(), 256) && !resolved.empty()) {
            addrConnect = CAddress(resolved[GetRand(resolved.size())], NODE_NONE);
            if (!addrConnect.IsValid()) {
                LogPrint(BCLog::NET, "Resolver returned invalid address %s for %s", addrConnect.ToString(), pszDest);
                return nullptr;
            }
        }
    }

    // Connect
    bool connected = false;
    SOCKET hSocket = INVALID_SOCKET;
    proxyType proxy;
    if (addrConnect.IsValid()) {
        bool proxyConnectionFailed = false;

        if (GetProxy(addrConnect.GetNetwork(), proxy)) {
            hSocket = CreateSocket(proxy.proxy);
            if (hSocket == INVALID_SOCKET) {
                return nullptr;
            }
            connected = ConnectThroughProxy(proxy, addrConnect.ToStringIP(), addrConnect.GetPort(), hSocket, nConnectTimeout, &proxyConnectionFailed);
        } else {
            // no proxy needed (none set for target network)
            hSocket = CreateSocket(addrConnect);
            if (hSocket == INVALID_SOCKET) {
                return nullptr;
            }
            connected = ConnectSocketDirectly(addrConnect, hSocket, nConnectTimeout);
        }
        if (!proxyConnectionFailed) {
            // If a connection to the node was attempted, and failure (if any) is not caused by a problem connecting to
            // the proxy, mark this as an attempt.
            addrman.Attempt(addrConnect, fCountFailure);
        }
    } else if (pszDest && GetNameProxy(proxy)) {
        hSocket = CreateSocket(proxy.proxy);
        if (hSocket == INVALID_SOCKET) {
            return nullptr;
        }
        std::string host;
        int port = default_port;
        SplitHostPort(std::string(pszDest), port, host);
        connected = ConnectThroughProxy(proxy, host, port, hSocket, nConnectTimeout, nullptr);
    }
    if (!connected) {
        CloseSocket(hSocket);
        return nullptr;
    }

    // Add node
    NodeId id = GetNewNodeId();
    uint64_t nonce = GetDeterministicRandomizer(RANDOMIZER_ID_LOCALHOSTNONCE).Write(id).Finalize();
    CNode* pnode = new CNode(id, nLocalServices, GetBestHeight(), hSocket, addrConnect, CalculateKeyedNetGroup(addrConnect), nonce, pszDest ? pszDest : "", false);
    pnode->nServicesExpected = ServiceFlags(addrConnect.nServices & nRelevantServices);
    pnode->fWhitelisted = IsWhitelistedRange(addrConnect);
    pnode->AddRef();

    // We're making a new connection, harvest entropy from the time (and our peer count)
    RandAddEvent((uint32_t)id);

    return pnode;
}

void CConnman::DumpBanlist()
{
    SweepBanned(); // clean unused entries (if bantime has expired)

    if (!BannedSetIsDirty())
        return;

    int64_t nStart = GetTimeMillis();

    CBanDB bandb;
    banmap_t banmap;
    GetBanned(banmap);
    if (bandb.Write(banmap)) {
        SetBannedSetDirty(false);
    }

    LogPrint(BCLog::NET, "Flushed %d banned node ips/subnets to banlist.dat  %dms\n",
        banmap.size(), GetTimeMillis() - nStart);
}

void CNode::CloseSocketDisconnect()
{
    fDisconnect = true;
    LOCK(cs_hSocket);
    if (hSocket != INVALID_SOCKET) {
        LogPrint(BCLog::NET, "disconnecting peer=%d\n", id);
        CloseSocket(hSocket);
    }
}

bool CNode::DisconnectOldProtocol(int nVersionIn, int nVersionRequired)
{
    fDisconnect = false;
    if (nVersionIn < nVersionRequired) {
        LogPrintf("%s : peer=%d using obsolete version %i; disconnecting\n", __func__, id, nVersionIn);
        fDisconnect = true;
    }
    return fDisconnect;
}

void CConnman::ClearBanned()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_setBanned);
        setBanned.clear();
        setBannedIsDirty = true;
    }
    DumpBanlist(); // store banlist to Disk
<<<<<<< HEAD
    uiInterface.BannedListChanged();
}

bool CNode::IsBanned(CNetAddr ip) {
    bool fResult = false;
    {
        LOCK(cs_setBanned);
        for (banmap_t::iterator it = setBanned.begin(); it != setBanned.end(); it++) {
            CSubNet subNet = (*it).first;
            CBanEntry banEntry = (*it).second;
            if (subNet.Match(ip) && GetTime() < banEntry.nBanUntil)
                fResult = true;
        }
    }
    return fResult;
    return false;
}

bool CNode::IsBanned(CSubNet subnet) {
    bool fResult = false;
    {
        LOCK(cs_setBanned);
        banmap_t::iterator i = setBanned.find(subnet);
        if (i != setBanned.end()) {
            CBanEntry banEntry = (*i).second;
            if (GetTime() < banEntry.nBanUntil)
                fResult = true;
        }
    }
    return fResult;
}

void CNode::Ban(const CNetAddr &addr, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch) {
=======
    if(clientInterface)
        clientInterface->BannedListChanged();
}

bool CConnman::IsBanned(CNetAddr ip)
{
    LOCK(cs_setBanned);
    for (const auto& it : setBanned) {
        CSubNet subNet = it.first;
        CBanEntry banEntry = it.second;

        if (subNet.Match(ip) && GetTime() < banEntry.nBanUntil) {
            return true;
        }
    }
    return false;
}

bool CConnman::IsBanned(CSubNet subnet)
{
    LOCK(cs_setBanned);
    banmap_t::iterator i = setBanned.find(subnet);
    if (i != setBanned.end()) {
        CBanEntry banEntry = (*i).second;
        if (GetTime() < banEntry.nBanUntil) {
            return true;
        }
    }
    return false;
}

void CConnman::Ban(const CNetAddr& addr, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CSubNet subNet(addr);
    Ban(subNet, banReason, bantimeoffset, sinceUnixEpoch);
}

<<<<<<< HEAD
void CNode::Ban(const CSubNet &subNet, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch) {
    CBanEntry banEntry(GetTime());
    banEntry.banReason = banReason;
    if (bantimeoffset <= 0) {
        bantimeoffset = GetArg("-bantime", 60 * 60 * 24); // Default 24-hour ban
        sinceUnixEpoch = false;
    }
    banEntry.nBanUntil = (sinceUnixEpoch ? 0 : GetTime()) + bantimeoffset;
=======
void CConnman::Ban(const CSubNet& subNet, const BanReason &banReason, int64_t bantimeoffset, bool sinceUnixEpoch)
{
    CBanEntry banEntry(GetTime());
    banEntry.banReason = banReason;
    if (bantimeoffset <= 0)
    {
        bantimeoffset = gArgs.GetArg("-bantime", DEFAULT_MISBEHAVING_BANTIME); // Default 24-hour ban
        sinceUnixEpoch = false;
    }
    banEntry.nBanUntil = (sinceUnixEpoch ? 0 : GetTime() )+bantimeoffset;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_setBanned);
        if (setBanned[subNet].nBanUntil < banEntry.nBanUntil) {
            setBanned[subNet] = banEntry;
            setBannedIsDirty = true;
        }
<<<<<<< HEAD
        else return;
    }
    uiInterface.BannedListChanged();
    {
        LOCK(cs_vNodes);
        for (CNode* pnode : vNodes) {
            if (subNet.Match((CNetAddr)pnode->addr))
=======
        else
            return;
    }
    if(clientInterface)
        clientInterface->BannedListChanged();
    {
        LOCK(cs_vNodes);
        for (CNode* pnode : vNodes) {
            if (subNet.Match(static_cast<CNetAddr>(pnode->addr)))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                pnode->fDisconnect = true;
        }
    }
    if(banReason == BanReasonManuallyAdded)
        DumpBanlist(); //store banlist to disk immediately if user requested ban
<<<<<<< HEAD

}

bool CNode::Unban(const CNetAddr &addr) {
=======
}

bool CConnman::Unban(const CNetAddr &addr)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CSubNet subNet(addr);
    return Unban(subNet);
}

<<<<<<< HEAD
bool CNode::Unban(const CSubNet &subNet) {
=======
bool CConnman::Unban(const CSubNet &subNet)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_setBanned);
        if (!setBanned.erase(subNet))
            return false;
        setBannedIsDirty = true;
    }
<<<<<<< HEAD
    uiInterface.BannedListChanged();
=======
    if(clientInterface)
        clientInterface->BannedListChanged();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    DumpBanlist(); //store banlist to disk immediately
    return true;
}

<<<<<<< HEAD
void CNode::GetBanned(banmap_t &banMap) {
    LOCK(cs_setBanned);
    banMap = setBanned; //create a thread safe copy
}

void CNode::SetBanned(const banmap_t &banMap) {
=======
void CConnman::GetBanned(banmap_t &banMap)
{
    LOCK(cs_setBanned);
    // Sweep the banlist so expired bans are not returned
    SweepBanned();
    banMap = setBanned; //create a thread safe copy
}

void CConnman::SetBanned(const banmap_t &banMap)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_setBanned);
    setBanned = banMap;
    setBannedIsDirty = true;
}

<<<<<<< HEAD
void CNode::SweepBanned() {
    int64_t now = GetTime();
=======
void CConnman::SweepBanned()
{
    int64_t now = GetTime();

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool notifyUI = false;
    {
        LOCK(cs_setBanned);
        banmap_t::iterator it = setBanned.begin();
        while(it != setBanned.end())
        {
            CSubNet subNet = (*it).first;
            CBanEntry banEntry = (*it).second;
            if(now > banEntry.nBanUntil)
            {
                setBanned.erase(it++);
                setBannedIsDirty = true;
                notifyUI = true;
<<<<<<< HEAD
                LogPrint("net", "%s: Removed banned node ip/subnet from banlist.dat: %s\n", __func__, subNet.ToString());
=======
                LogPrint(BCLog::NET, "%s: Removed banned node ip/subnet from banlist.dat: %s\n", __func__, subNet.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
            else
                ++it;
        }
    }
    // update UI
    if(notifyUI) {
        uiInterface.BannedListChanged();
    }
}

<<<<<<< HEAD
bool CNode::BannedSetIsDirty() {
=======
bool CConnman::BannedSetIsDirty()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_setBanned);
    return setBannedIsDirty;
}

<<<<<<< HEAD
void CNode::SetBannedSetDirty(bool dirty) {
=======
void CConnman::SetBannedSetDirty(bool dirty)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_setBanned); //reuse setBanned lock for the isDirty flag
    setBannedIsDirty = dirty;
}

<<<<<<< HEAD
std::vector <CSubNet> CNode::vWhitelistedRange;
RecursiveMutex CNode::cs_vWhitelistedRange;

bool CNode::IsWhitelistedRange(const CNetAddr &addr) {
    LOCK(cs_vWhitelistedRange);
    for (const CSubNet &subnet : vWhitelistedRange) {
=======

bool CConnman::IsWhitelistedRange(const CNetAddr& addr)
{
    for (const CSubNet& subnet : vWhitelistedRange) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (subnet.Match(addr))
            return true;
    }
    return false;
}

<<<<<<< HEAD
void CNode::AddWhitelistedRange(const CSubNet &subnet) {
    LOCK(cs_vWhitelistedRange);
    vWhitelistedRange.push_back(subnet);
=======
std::string CNode::GetAddrName() const {
    LOCK(cs_addrName);
    return addrName;
}

void CNode::MaybeSetAddrName(const std::string& addrNameIn) {
    LOCK(cs_addrName);
    if (addrName.empty()) {
        addrName = addrNameIn;
    }
}

CService CNode::GetAddrLocal() const {
    LOCK(cs_addrLocal);
    return addrLocal;
}

void CNode::SetAddrLocal(const CService& addrLocalIn) {
    LOCK(cs_addrLocal);
    if (addrLocal.IsValid()) {
        error("Addr local already set for node: %i. Refusing to change from %s to %s", id, addrLocal.ToString(), addrLocalIn.ToString());
    } else {
        addrLocal = addrLocalIn;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

#undef X
#define X(name) stats.name = name
<<<<<<< HEAD

void CNode::copyStats(CNodeStats &stats) {
    stats.nodeid = this->GetId();
    X(nServices);
    X(addr);
=======
void CNode::copyStats(CNodeStats& stats, const std::vector<bool>& m_asmap)
{
    stats.nodeid = this->GetId();
    X(nServices);
    stats.m_mapped_as = addr.GetMappedAS(m_asmap);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    X(nLastSend);
    X(nLastRecv);
    X(nTimeConnected);
    X(nTimeOffset);
<<<<<<< HEAD
    X(addrName);
    X(nVersion);
    X(cleanSubVer);
    X(fInbound);
    X(nStartingHeight);
    X(nSendBytes);
    X(nRecvBytes);
=======
    stats.addrName = GetAddrName();
    X(nVersion);
    {
        LOCK(cs_SubVer);
        X(cleanSubVer);
    }
    X(fInbound);
    X(fAddnode);
    X(nStartingHeight);
    {
        LOCK(cs_vSend);
        X(mapSendBytesPerMsgCmd);
        X(nSendBytes);
    }
    {
        LOCK(cs_vRecv);
        X(mapRecvBytesPerMsgCmd);
        X(nRecvBytes);
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    X(fWhitelisted);

    // It is common for nodes with good ping times to suddenly become lagged,
    // due to a new block arriving or other large transfer.
    // Merely reporting pingtime might fool the caller into thinking the node was still responsive,
    // since pingtime does not update until the ping is complete, which might take a while.
    // So, if a ping is taking an unusually long time in flight,
    // the caller can immediately detect that this is happening.
    int64_t nPingUsecWait = 0;
    if ((0 != nPingNonceSent) && (0 != nPingUsecStart)) {
        nPingUsecWait = GetTimeMicros() - nPingUsecStart;
    }

<<<<<<< HEAD
    // Raw ping time is in microseconds, but show it to user as whole seconds (PRCY users should be well used to small numbers with many decimal places by now :)
    stats.dPingTime = (((double) nPingUsecTime) / 1e6);
    stats.dPingWait = (((double) nPingUsecWait) / 1e6);

    // Leave string empty if addrLocal invalid (not filled in yet)
    stats.addrLocal = addrLocal.IsValid() ? addrLocal.ToString() : "";
}

#undef X

// requires LOCK(cs_vRecvMsg)
bool CNode::ReceiveMsgBytes(const char *pch, unsigned int nBytes) {
=======
    // Raw ping time is in microseconds, but show it to user as whole seconds (PIVX users should be well used to small numbers with many decimal places by now :)
    stats.dPingTime = (((double)nPingUsecTime) / 1e6);
    stats.dPingWait = (((double)nPingUsecWait) / 1e6);

    // Leave string empty if addrLocal invalid (not filled in yet)
    CService addrLocalUnlocked = GetAddrLocal();
    stats.addrLocal = addrLocalUnlocked.IsValid() ? addrLocalUnlocked.ToString() : "";
}
#undef X

bool CNode::ReceiveMsgBytes(const char* pch, unsigned int nBytes, bool& complete)
{
    complete = false;
    int64_t nTimeMicros = GetTimeMicros();
    LOCK(cs_vRecv);
    nLastRecv = nTimeMicros / 1000000;
    nRecvBytes += nBytes;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    while (nBytes > 0) {
        // get current incomplete message, or create a new one
        if (vRecvMsg.empty() ||
            vRecvMsg.back().complete())
<<<<<<< HEAD
            vRecvMsg.push_back(CNetMessage(SER_NETWORK, nRecvVersion));

        CNetMessage &msg = vRecvMsg.back();
=======
            vRecvMsg.emplace_back(Params().MessageStart(), SER_NETWORK, INIT_PROTO_VERSION);

        CNetMessage& msg = vRecvMsg.back();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // absorb network data
        int handled;
        if (!msg.in_data)
            handled = msg.readHeader(pch, nBytes);
        else
            handled = msg.readData(pch, nBytes);

        if (handled < 0)
            return false;

        if (msg.in_data && msg.hdr.nMessageSize > MAX_PROTOCOL_MESSAGE_LENGTH) {
<<<<<<< HEAD
            LogPrint("net", "Oversized message from peer=%i, disconnecting", GetId());
=======
            LogPrint(BCLog::NET, "Oversized message from peer=%i, disconnecting\n", GetId());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return false;
        }

        pch += handled;
        nBytes -= handled;

        if (msg.complete()) {
<<<<<<< HEAD
            msg.nTime = GetTimeMicros();
            messageHandlerCondition.notify_one();
=======

            // Store received bytes per message command
            // to prevent a memory DOS, only allow valid commands
            mapMsgCmdSize::iterator i = mapRecvBytesPerMsgCmd.find(msg.hdr.pchCommand);
            if (i == mapRecvBytesPerMsgCmd.end())
                i = mapRecvBytesPerMsgCmd.find(NET_MESSAGE_COMMAND_OTHER);
            assert(i != mapRecvBytesPerMsgCmd.end());
            i->second += msg.hdr.nMessageSize + CMessageHeader::HEADER_SIZE;

            msg.nTime = nTimeMicros;
            complete = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    return true;
}

<<<<<<< HEAD
int CNetMessage::readHeader(const char *pch, unsigned int nBytes) {
=======
void CNode::SetSendVersion(int nVersionIn)
{
    // Send version may only be changed in the version message, and
    // only one version message is allowed per session. We can therefore
    // treat this value as const and even atomic as long as it's only used
    // once a version message has been successfully processed. Any attempt to
    // set this twice is an error.
    if (nSendVersion != 0) {
        error("Send version already set for node: %i. Refusing to change from %i to %i", id, nSendVersion, nVersionIn);
    } else {
        nSendVersion = nVersionIn;
    }
}

int CNode::GetSendVersion() const
{
    // The send version should always be explicitly set to
    // INIT_PROTO_VERSION rather than using this value until SetSendVersion
    // has been called.
    if (nSendVersion == 0) {
        error("Requesting unset send version for node: %i. Using %i", id, INIT_PROTO_VERSION);
        return INIT_PROTO_VERSION;
    }
    return nSendVersion;
}

int CNetMessage::readHeader(const char* pch, unsigned int nBytes)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // copy data to temporary parsing buffer
    unsigned int nRemaining = 24 - nHdrPos;
    unsigned int nCopy = std::min(nRemaining, nBytes);

    memcpy(&hdrbuf[nHdrPos], pch, nCopy);
    nHdrPos += nCopy;

    // if header incomplete, exit
    if (nHdrPos < 24)
        return nCopy;

    // deserialize to CMessageHeader
    try {
        hdrbuf >> hdr;
<<<<<<< HEAD
    } catch (const std::exception &) {
=======
    } catch (const std::exception&) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return -1;
    }

    // reject messages larger than MAX_SIZE
    if (hdr.nMessageSize > MAX_SIZE)
        return -1;

    // switch state to reading message data
    in_data = true;

    return nCopy;
}

<<<<<<< HEAD
int CNetMessage::readData(const char *pch, unsigned int nBytes) {
=======
int CNetMessage::readData(const char* pch, unsigned int nBytes)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned int nRemaining = hdr.nMessageSize - nDataPos;
    unsigned int nCopy = std::min(nRemaining, nBytes);

    if (vRecv.size() < nDataPos + nCopy) {
        // Allocate up to 256 KiB ahead, but never more than the total message size.
        vRecv.resize(std::min(hdr.nMessageSize, nDataPos + nCopy + 256 * 1024));
    }

<<<<<<< HEAD
=======
    hasher.Write((const unsigned char*)pch, nCopy);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    memcpy(&vRecv[nDataPos], pch, nCopy);
    nDataPos += nCopy;

    return nCopy;
}

<<<<<<< HEAD

// requires LOCK(cs_vSend)
void SocketSendData(CNode *pnode) {
    std::deque<CSerializeData>::iterator it = pnode->vSendMsg.begin();

    while (it != pnode->vSendMsg.end()) {
        const CSerializeData &data = *it;
        assert(data.size() > pnode->nSendOffset);
        int nBytes = send(pnode->hSocket, &data[pnode->nSendOffset], data.size() - pnode->nSendOffset,
                          MSG_NOSIGNAL | MSG_DONTWAIT);
        if (nBytes > 0) {
            pnode->nLastSend = GetTime();
            pnode->nSendBytes += nBytes;
            pnode->nSendOffset += nBytes;
            pnode->RecordBytesSent(nBytes);
            if (pnode->nSendOffset == data.size()) {
                pnode->nSendOffset = 0;
                pnode->nSendSize -= data.size();
=======
const uint256& CNetMessage::GetMessageHash() const
{
    assert(complete());
    if (data_hash.IsNull())
        hasher.Finalize(data_hash.begin());
    return data_hash;
}

// requires LOCK(cs_vSend)
size_t CConnman::SocketSendData(CNode* pnode)
{
    auto it = pnode->vSendMsg.begin();
    size_t nSentSize = 0;

    while (it != pnode->vSendMsg.end()) {
        const auto& data = *it;
        assert(data.size() > pnode->nSendOffset);
        int nBytes = 0;
        {
            LOCK(pnode->cs_hSocket);
            if (pnode->hSocket == INVALID_SOCKET)
                break;
            nBytes = send(pnode->hSocket, reinterpret_cast<const char*>(data.data()) + pnode->nSendOffset, data.size() - pnode->nSendOffset, MSG_NOSIGNAL | MSG_DONTWAIT);
        }
        if (nBytes > 0) {
            pnode->nLastSend = GetSystemTimeInSeconds();
            pnode->nSendBytes += nBytes;
            pnode->nSendOffset += nBytes;
            nSentSize += nBytes;
            if (pnode->nSendOffset == data.size()) {
                pnode->nSendOffset = 0;
                pnode->nSendSize -= data.size();
                pnode->fPauseSend = pnode->nSendSize > nSendBufferMaxSize;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                it++;
            } else {
                // could not send full message; stop sending more
                break;
            }
        } else {
            if (nBytes < 0) {
                // error
                int nErr = WSAGetLastError();
                if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR && nErr != WSAEINPROGRESS) {
                    LogPrintf("socket send error %s\n", NetworkErrorString(nErr));
                    pnode->CloseSocketDisconnect();
                }
            }
            // couldn't send anything at all
            break;
        }
    }

    if (it == pnode->vSendMsg.end()) {
        assert(pnode->nSendOffset == 0);
        assert(pnode->nSendSize == 0);
    }
    pnode->vSendMsg.erase(pnode->vSendMsg.begin(), it);
<<<<<<< HEAD
}

static list<CNode *> vNodesDisconnected;

void ThreadSocketHandler() {
    unsigned int nPrevNodeCount = 0;
    while (true) {
        //
        // Disconnect nodes
        //
        {
            LOCK(cs_vNodes);
            // Disconnect unused nodes
            vector < CNode * > vNodesCopy = vNodes;
            for (CNode * pnode : vNodesCopy)
            {
                if (pnode->fDisconnect ||
                    (pnode->GetRefCount() <= 0 && pnode->vRecvMsg.empty() && pnode->nSendSize == 0 &&
                     pnode->ssSend.empty())) {
                    // remove from vNodes
                    vNodes.erase(remove(vNodes.begin(), vNodes.end(), pnode), vNodes.end());

                    // release outbound grant (if any)
                    pnode->grantOutbound.Release();

                    // close socket and cleanup
                    pnode->CloseSocketDisconnect();

                    // hold in disconnected pool until all refs are released
                    if (pnode->fNetworkNode || pnode->fInbound)
                        pnode->Release();
                    vNodesDisconnected.push_back(pnode);
                }
            }
        }
        {
            // Delete disconnected nodes
            list < CNode * > vNodesDisconnectedCopy = vNodesDisconnected;
            for (CNode * pnode : vNodesDisconnectedCopy)
            {
                // wait until threads are done using it
                if (pnode->GetRefCount() <= 0) {
                    bool fDelete = false;
                    {
                        TRY_LOCK(pnode->cs_vSend, lockSend);
                        if (lockSend) {
                            TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                            if (lockRecv) {
                                TRY_LOCK(pnode->cs_inventory, lockInv);
                                if (lockInv)
                                    fDelete = true;
                            }
                        }
                    }
                    if (fDelete) {
                        vNodesDisconnected.remove(pnode);
                        delete pnode;
                    }
                }
            }
        }
        size_t vNodesSize;
        {
            LOCK(cs_vNodes);
            vNodesSize = vNodes.size();
        }
        if (vNodesSize != nPrevNodeCount) {
            nPrevNodeCount = vNodesSize;
            uiInterface.NotifyNumConnectionsChanged(nPrevNodeCount);
        }

        //
        // Find which sockets have data to receive
        //
        struct timeval timeout;
=======
    return nSentSize;
}

void CheckOffsetDisconnectedPeers(const CNetAddr& ip)
{
    int nConnections = 0;
    if (g_connman) {
        g_connman->ForEachNode([&nConnections](CNode* pnode) {
            if (pnode->fSuccessfullyConnected)
                nConnections++;
            if (nConnections == ENOUGH_CONNECTIONS)
                return;
        });
    }

    // Not enough connections. Insert peer.
    static std::set<CNetAddr> setOffsetDisconnectedPeers;
    setOffsetDisconnectedPeers.insert(ip);
    if (setOffsetDisconnectedPeers.size() >= MAX_TIMEOFFSET_DISCONNECTIONS) {
        // clear the set
        setOffsetDisconnectedPeers.clear();
        // Trigger the warning
        std::string strWarn1 = _("Peers are being disconnected due time differences.");
        std::string strWarn2 = strprintf(_("Please check that your computer's date and time are correct! If your clock is wrong %s will not work properly."), PACKAGE_NAME);

        LogPrintf("*** Warning: %s %s\n", strWarn1, strWarn2);

        static int64_t nLastGUINotif = 0;
        int64_t now = GetTime();
        if (nLastGUINotif + 40 < now) { // Notify the GUI if needed.
            nLastGUINotif = now;
            uiInterface.ThreadSafeMessageBox(strprintf("%s\n\n%s", strWarn1, strWarn2), _("Warning"), CClientUIInterface::MSG_ERROR);
        }
    }
}

struct NodeEvictionCandidate
{
    NodeId id;
    int64_t nTimeConnected;
    int64_t nMinPingUsecTime;
    CAddress addr;
    uint64_t nKeyedNetGroup;
};

static bool ReverseCompareNodeMinPingTime(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    return a.nMinPingUsecTime > b.nMinPingUsecTime;
}

static bool ReverseCompareNodeTimeConnected(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b)
{
    return a.nTimeConnected > b.nTimeConnected;
}

static bool CompareNetGroupKeyed(const NodeEvictionCandidate &a, const NodeEvictionCandidate &b) {
    return a.nKeyedNetGroup < b.nKeyedNetGroup;
};

/** Try to find a connection to evict when the node is full.
 *  Extreme care must be taken to avoid opening the node to attacker
 *   triggered network partitioning.
 *  The strategy used here is to protect a small number of peers
 *   for each of several distinct characteristics which are difficult
 *   to forge.  In order to partition a node the attacker must be
 *   simultaneously better at all of them than honest peers.
 */
bool CConnman::AttemptToEvictConnection(bool fPreferNewConnection)
{
    std::vector<NodeEvictionCandidate> vEvictionCandidates;
    {
        LOCK(cs_vNodes);

        for (const CNode* node : vNodes) {
            if (node->fWhitelisted)
                continue;
            if (!node->fInbound)
                continue;
            if (node->fDisconnect)
                continue;
            NodeEvictionCandidate candidate = {node->GetId(), node->nTimeConnected, node->nMinPingUsecTime, node->addr, node->nKeyedNetGroup};
            vEvictionCandidates.push_back(candidate);
        }
    }

    if (vEvictionCandidates.empty()) return false;

    // Protect connections with certain characteristics

    // Deterministically select 4 peers to protect by netgroup.
    // An attacker cannot predict which netgroups will be protected
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), CompareNetGroupKeyed);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(4, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect the 8 nodes with the lowest minimum ping time.
    // An attacker cannot manipulate this metric without physically moving nodes closer to the target.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), ReverseCompareNodeMinPingTime);
    vEvictionCandidates.erase(vEvictionCandidates.end() - std::min(8, static_cast<int>(vEvictionCandidates.size())), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Protect the half of the remaining nodes which have been connected the longest.
    // This replicates the non-eviction implicit behavior, and precludes attacks that start later.
    std::sort(vEvictionCandidates.begin(), vEvictionCandidates.end(), ReverseCompareNodeTimeConnected);
    vEvictionCandidates.erase(vEvictionCandidates.end() - static_cast<int>(vEvictionCandidates.size() / 2), vEvictionCandidates.end());

    if (vEvictionCandidates.empty()) return false;

    // Identify the network group with the most connections and youngest member.
    // (vEvictionCandidates is already sorted by reverse connect time)
    uint64_t naMostConnections;
    unsigned int nMostConnections = 0;
    int64_t nMostConnectionsTime = 0;
    std::map<uint64_t, std::vector<NodeEvictionCandidate> > mapAddrCounts;
    for (const NodeEvictionCandidate& node : vEvictionCandidates) {
        mapAddrCounts[node.nKeyedNetGroup].push_back(node);
        int64_t grouptime = mapAddrCounts[node.nKeyedNetGroup][0].nTimeConnected;
        size_t groupsize = mapAddrCounts[node.nKeyedNetGroup].size();
        if (groupsize > nMostConnections || (groupsize == nMostConnections && grouptime > nMostConnectionsTime)) {
            nMostConnections = groupsize;
            nMostConnectionsTime = grouptime;
            naMostConnections = node.nKeyedNetGroup;
        }
    }

    // Reduce to the network group with the most connections
    vEvictionCandidates = std::move(mapAddrCounts[naMostConnections]);

    // Do not disconnect peers if there is only 1 connection from their network group
    if (vEvictionCandidates.size() <= 1)
        // unless we prefer the new connection (for whitelisted peers)
        if (!fPreferNewConnection)
            return false;

    // Disconnect from the network group with the most connections
    NodeId evicted = vEvictionCandidates.front().id;
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (pnode->GetId() == evicted) {
            pnode->fDisconnect = true;
            return true;
        }
    }
    return false;
}

void CConnman::AcceptConnection(const ListenSocket& hListenSocket) {
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    SOCKET hSocket = accept(hListenSocket.socket, (struct sockaddr*)&sockaddr, &len);
    CAddress addr;
    int nInbound = 0;
    int nMaxInbound = nMaxConnections - (nMaxOutbound + nMaxFeeler);

    if (hSocket != INVALID_SOCKET)
        if (!addr.SetSockAddr((const struct sockaddr*)&sockaddr))
            LogPrintf("Warning: Unknown socket family\n");

    bool whitelisted = hListenSocket.whitelisted || IsWhitelistedRange(addr);
    {
        LOCK(cs_vNodes);
        for (const CNode* pnode : vNodes) {
            if (pnode->fInbound) nInbound++;
        }
    }

    if (hSocket == INVALID_SOCKET) {
        int nErr = WSAGetLastError();
        if (nErr != WSAEWOULDBLOCK)
            LogPrintf("socket error accept failed: %s\n", NetworkErrorString(nErr));
        return;
    }

    if (!fNetworkActive) {
        LogPrintf("connection from %s dropped: not accepting new connections\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    if (!IsSelectableSocket(hSocket)) {
        LogPrintf("connection from %s dropped: non-selectable socket\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    if (IsBanned(addr) && !whitelisted) {
        LogPrint(BCLog::NET, "connection from %s dropped (banned)\n", addr.ToString());
        CloseSocket(hSocket);
        return;
    }

    if (nInbound >= nMaxConnections - MAX_OUTBOUND_CONNECTIONS) {
        if (!AttemptToEvictConnection(whitelisted)) {
            // No connection to evict, disconnect the new connection
            LogPrint(BCLog::NET, "failed to find an eviction candidate - connection dropped (full)\n");
            CloseSocket(hSocket);
            return;
        }
    }

    NodeId id = GetNewNodeId();
    uint64_t nonce = GetDeterministicRandomizer(RANDOMIZER_ID_LOCALHOSTNONCE).Write(id).Finalize();

    CNode* pnode = new CNode(id, nLocalServices, GetBestHeight(), hSocket, addr, CalculateKeyedNetGroup(addr), nonce, "", true);
    pnode->AddRef();
    pnode->fWhitelisted = whitelisted;
    m_msgproc->InitializeNode(pnode);

    LogPrint(BCLog::NET, "connection from %s accepted\n", addr.ToString());

    {
        LOCK(cs_vNodes);
        vNodes.push_back(pnode);
    }

    // We received a new connection, harvest entropy from the time (and our peer count)
    RandAddEvent((uint32_t)id);
}

void CConnman::ThreadSocketHandler()
{
    unsigned int nPrevNodeCount = 0;
    while (!interruptNet) {
        //
        // Disconnect nodes
        //
        {
            LOCK(cs_vNodes);
            // Disconnect unused nodes
            std::vector<CNode*> vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy) {
                if (pnode->fDisconnect) {
                    // remove from vNodes
                    vNodes.erase(remove(vNodes.begin(), vNodes.end(), pnode), vNodes.end());

                    // release outbound grant (if any)
                    pnode->grantOutbound.Release();

                    // close socket and cleanup
                    pnode->CloseSocketDisconnect();

                    // hold in disconnected pool until all refs are released
                    pnode->Release();
                    vNodesDisconnected.push_back(pnode);
                }
            }
        }
        {
            // Delete disconnected nodes
            std::list<CNode*> vNodesDisconnectedCopy = vNodesDisconnected;
            for (CNode* pnode : vNodesDisconnectedCopy) {
                // wait until threads are done using it
                if (pnode->GetRefCount() <= 0) {
                    bool fDelete = false;
                    {
                        TRY_LOCK(pnode->cs_inventory, lockInv);
                        if (lockInv) {
                            TRY_LOCK(pnode->cs_vSend, lockSend);
                            if (lockSend) {
                                fDelete = true;
                            }
                        }
                    }
                    if (fDelete) {
                        vNodesDisconnected.remove(pnode);
                        DeleteNode(pnode);
                    }
                }
            }
        }
        size_t vNodesSize;
        {
            LOCK(cs_vNodes);
            vNodesSize = vNodes.size();
        }
        if(vNodesSize != nPrevNodeCount) {
            nPrevNodeCount = vNodesSize;
            if(clientInterface)
                clientInterface->NotifyNumConnectionsChanged(nPrevNodeCount);
        }

        //
        // Find which sockets have data to receive
        //
        struct timeval timeout;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000; // frequency to poll pnode->vSend

        fd_set fdsetRecv;
        fd_set fdsetSend;
        fd_set fdsetError;
        FD_ZERO(&fdsetRecv);
        FD_ZERO(&fdsetSend);
        FD_ZERO(&fdsetError);
        SOCKET hSocketMax = 0;
        bool have_fds = false;

<<<<<<< HEAD
        for (const ListenSocket &hListenSocket : vhListenSocket) {
            FD_SET(hListenSocket.socket, &fdsetRecv);
            hSocketMax = max(hSocketMax, hListenSocket.socket);
=======
        for (const ListenSocket& hListenSocket : vhListenSocket) {
            FD_SET(hListenSocket.socket, &fdsetRecv);
            hSocketMax = std::max(hSocketMax, hListenSocket.socket);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            have_fds = true;
        }

        {
            LOCK(cs_vNodes);
<<<<<<< HEAD
            for (CNode * pnode : vNodes)
            {
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;
                FD_SET(pnode->hSocket, &fdsetError);
                hSocketMax = max(hSocketMax, pnode->hSocket);
                have_fds = true;

=======
            for (CNode* pnode : vNodes) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                // Implement the following logic:
                // * If there is data to send, select() for sending data. As this only
                //   happens when optimistic write failed, we choose to first drain the
                //   write buffer in this case before receiving more. This avoids
                //   needlessly queueing received data, if the remote peer is not themselves
                //   receiving data. This means properly utilizing TCP flow control signalling.
<<<<<<< HEAD
                // * Otherwise, if there is no (complete) message in the receive buffer,
                //   or there is space left in the buffer, select() for receiving data.
                // * (if neither of the above applies, there is certainly one message
                //   in the receiver buffer ready to be processed).
                // Together, that means that at least one of the following is always possible,
                // so we don't deadlock:
                // * We send some data.
                // * We wait for data to be received (and disconnect after timeout).
                // * We process a message in the buffer (message handler thread).
                {
                    TRY_LOCK(pnode->cs_vSend, lockSend);
                    if (lockSend && !pnode->vSendMsg.empty()) {
                        FD_SET(pnode->hSocket, &fdsetSend);
                        continue;
                    }
                }
                {
                    TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                    if (lockRecv && (pnode->vRecvMsg.empty() || !pnode->vRecvMsg.front().complete() ||
                                     pnode->GetTotalRecvSize() <= ReceiveFloodSize()))
                        FD_SET(pnode->hSocket, &fdsetRecv);
=======
                // * Otherwise, if there is space left in the receive buffer, select() for
                //   receiving data.
                // * Hand off all complete messages to the processor, to be handled without
                //   blocking here.

                bool select_recv = !pnode->fPauseRecv;
                bool select_send;
                {
                    LOCK(pnode->cs_vSend);
                    select_send = !pnode->vSendMsg.empty();
                }

                LOCK(pnode->cs_hSocket);
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;

                FD_SET(pnode->hSocket, &fdsetError);
                hSocketMax = std::max(hSocketMax, pnode->hSocket);
                have_fds = true;

                if (select_send) {
                    FD_SET(pnode->hSocket, &fdsetSend);
                    continue;
                }
                if (select_recv) {
                    FD_SET(pnode->hSocket, &fdsetRecv);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                }
            }
        }

        int nSelect = select(have_fds ? hSocketMax + 1 : 0,
                             &fdsetRecv, &fdsetSend, &fdsetError, &timeout);
<<<<<<< HEAD
        boost::this_thread::interruption_point();
=======
        if (interruptNet)
            return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        if (nSelect == SOCKET_ERROR) {
            if (have_fds) {
                int nErr = WSAGetLastError();
                LogPrintf("socket select error %s\n", NetworkErrorString(nErr));
                for (unsigned int i = 0; i <= hSocketMax; i++)
                    FD_SET(i, &fdsetRecv);
            }
            FD_ZERO(&fdsetSend);
            FD_ZERO(&fdsetError);
<<<<<<< HEAD
            MilliSleep(timeout.tv_usec / 1000);
=======
            if (!interruptNet.sleep_for(std::chrono::milliseconds(timeout.tv_usec/1000)))
                return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        //
        // Accept new connections
        //
<<<<<<< HEAD
        for (const ListenSocket &hListenSocket : vhListenSocket) {
            if (hListenSocket.socket != INVALID_SOCKET && FD_ISSET(hListenSocket.socket, &fdsetRecv)) {
                struct sockaddr_storage sockaddr;
                socklen_t len = sizeof(sockaddr);
                SOCKET hSocket = accept(hListenSocket.socket, (struct sockaddr *) &sockaddr, &len);
                CAddress addr;
                int nInbound = 0;

                if (hSocket != INVALID_SOCKET)
                    if (!addr.SetSockAddr((const struct sockaddr *) &sockaddr))
                        LogPrintf("Warning: Unknown socket family\n");

                bool whitelisted = hListenSocket.whitelisted || CNode::IsWhitelistedRange(addr);
                {
                    LOCK(cs_vNodes);
                    for (CNode * pnode : vNodes)
                    if (pnode->fInbound)
                        nInbound++;
                }

                if (hSocket == INVALID_SOCKET) {
                    int nErr = WSAGetLastError();
                    if (nErr != WSAEWOULDBLOCK)
                        LogPrintf("socket error accept failed: %s\n", NetworkErrorString(nErr));
                } else if (!IsSelectableSocket(hSocket)) {
                    LogPrintf("connection from %s dropped: non-selectable socket\n", addr.ToString());
                    CloseSocket(hSocket);
                } else if (nInbound >= nMaxConnections - MAX_OUTBOUND_CONNECTIONS) {
                    LogPrint("net", "connection from %s dropped (full)\n", addr.ToString());
                    CloseSocket(hSocket);
                } else if (CNode::IsBanned(addr) && !whitelisted) {
                    LogPrintf("connection from %s dropped (banned)\n", addr.ToString());
                    CloseSocket(hSocket);
                } else {
                    CNode *pnode = new CNode(hSocket, addr, "", true);
                    pnode->AddRef();
                    pnode->fWhitelisted = whitelisted;

                    {
                        LOCK(cs_vNodes);
                        vNodes.push_back(pnode);
                    }
                }
=======
        for (const ListenSocket& hListenSocket : vhListenSocket) {
            if (hListenSocket.socket != INVALID_SOCKET && FD_ISSET(hListenSocket.socket, &fdsetRecv)) {
                AcceptConnection(hListenSocket);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }

        //
        // Service each socket
        //
<<<<<<< HEAD
        vector < CNode * > vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode * pnode : vNodesCopy)
            pnode->AddRef();
        }
        for (CNode * pnode : vNodesCopy)
        {
            boost::this_thread::interruption_point();
=======
        std::vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy)
                pnode->AddRef();
        }
        for (CNode* pnode : vNodesCopy) {
            if (interruptNet)
                return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            //
            // Receive
            //
<<<<<<< HEAD
            if (pnode->hSocket == INVALID_SOCKET)
                continue;
            if (FD_ISSET(pnode->hSocket, &fdsetRecv) || FD_ISSET(pnode->hSocket, &fdsetError)) {
                TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                if (lockRecv) {
                    {
                        // typical socket buffer is 8K-64K
                        char pchBuf[0x10000];
                        int nBytes = recv(pnode->hSocket, pchBuf, sizeof(pchBuf), MSG_DONTWAIT);
                        if (nBytes > 0) {
                            if (!pnode->ReceiveMsgBytes(pchBuf, nBytes))
                                pnode->CloseSocketDisconnect();
                            pnode->nLastRecv = GetTime();
                            pnode->nRecvBytes += nBytes;
                            pnode->RecordBytesRecv(nBytes);
                        } else if (nBytes == 0) {
                            // socket closed gracefully
                            if (!pnode->fDisconnect)
                                LogPrint("net", "socket closed\n");
=======
            bool recvSet = false;
            bool sendSet = false;
            bool errorSet = false;
            {
                LOCK(pnode->cs_hSocket);
                if (pnode->hSocket == INVALID_SOCKET)
                    continue;
                recvSet = FD_ISSET(pnode->hSocket, &fdsetRecv);
                sendSet = FD_ISSET(pnode->hSocket, &fdsetSend);
                errorSet = FD_ISSET(pnode->hSocket, &fdsetError);
            }
            if (recvSet || errorSet) {
                {
                    {
                        // typical socket buffer is 8K-64K
                        char pchBuf[0x10000];
                        int nBytes = 0;
                        {
                            LOCK(pnode->cs_hSocket);
                            if (pnode->hSocket == INVALID_SOCKET)
                                continue;
                            nBytes = recv(pnode->hSocket, pchBuf, sizeof(pchBuf), MSG_DONTWAIT);
                        }
                        if (nBytes > 0) {
                            bool notify = false;
                            if (!pnode->ReceiveMsgBytes(pchBuf, nBytes, notify))
                                pnode->CloseSocketDisconnect();
                            RecordBytesRecv(nBytes);
                            if (notify) {
                                size_t nSizeAdded = 0;
                                auto it(pnode->vRecvMsg.begin());
                                for (; it != pnode->vRecvMsg.end(); ++it) {
                                    if (!it->complete())
                                        break;
                                    nSizeAdded += it->vRecv.size() + CMessageHeader::HEADER_SIZE;
                                }
                                {
                                    LOCK(pnode->cs_vProcessMsg);
                                    pnode->vProcessMsg.splice(pnode->vProcessMsg.end(), pnode->vRecvMsg, pnode->vRecvMsg.begin(), it);
                                    pnode->nProcessQueueSize += nSizeAdded;
                                    pnode->fPauseRecv = pnode->nProcessQueueSize > nReceiveFloodSize;
                                }
                                WakeMessageHandler();
                            }
                        } else if (nBytes == 0) {
                            // socket closed gracefully
                            if (!pnode->fDisconnect)
                                LogPrint(BCLog::NET, "socket closed\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                            pnode->CloseSocketDisconnect();
                        } else if (nBytes < 0) {
                            // error
                            int nErr = WSAGetLastError();
<<<<<<< HEAD
                            if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR &&
                                nErr != WSAEINPROGRESS) {
=======
                            if (nErr != WSAEWOULDBLOCK && nErr != WSAEMSGSIZE && nErr != WSAEINTR && nErr != WSAEINPROGRESS) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                                if (!pnode->fDisconnect)
                                    LogPrintf("socket recv error %s\n", NetworkErrorString(nErr));
                                pnode->CloseSocketDisconnect();
                            }
                        }
                    }
                }
            }

            //
            // Send
            //
<<<<<<< HEAD
            if (pnode->hSocket == INVALID_SOCKET)
                continue;
            if (FD_ISSET(pnode->hSocket, &fdsetSend)) {
                TRY_LOCK(pnode->cs_vSend, lockSend);
                if (lockSend)
                    SocketSendData(pnode);
=======
            if (sendSet) {
                LOCK(pnode->cs_vSend);
                size_t nBytes = SocketSendData(pnode);
                if (nBytes)
                    RecordBytesSent(nBytes);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }

            //
            // Inactivity checking
            //
<<<<<<< HEAD
            int64_t nTime = GetTime();
            if (nTime - pnode->nTimeConnected > 60) {
                if (pnode->nLastRecv == 0 || pnode->nLastSend == 0) {
                    LogPrint("net", "socket no message in first 60 seconds, %d %d from %d\n", pnode->nLastRecv != 0,
                             pnode->nLastSend != 0, pnode->id);
=======
            int64_t nTime = GetSystemTimeInSeconds();
            if (nTime - pnode->nTimeConnected > 60) {
                if (pnode->nLastRecv == 0 || pnode->nLastSend == 0) {
                    LogPrint(BCLog::NET, "socket no message in first 60 seconds, %d %d from %d\n", pnode->nLastRecv != 0, pnode->nLastSend != 0, pnode->GetId());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    pnode->fDisconnect = true;
                } else if (nTime - pnode->nLastSend > TIMEOUT_INTERVAL) {
                    LogPrintf("socket sending timeout: %is\n", nTime - pnode->nLastSend);
                    pnode->fDisconnect = true;
<<<<<<< HEAD
                } else if (nTime - pnode->nLastRecv >
                           (pnode->nVersion > BIP0031_VERSION ? TIMEOUT_INTERVAL : 90 * 60)) {
                    LogPrintf("socket receive timeout: %is\n", nTime - pnode->nLastRecv);
                    pnode->fDisconnect = true;
                } else if (pnode->nPingNonceSent &&
                           pnode->nPingUsecStart + TIMEOUT_INTERVAL * 1000000 < GetTimeMicros()) {
=======
                } else if (nTime - pnode->nLastRecv > TIMEOUT_INTERVAL) {
                    LogPrintf("socket receive timeout: %is\n", nTime - pnode->nLastRecv);
                    pnode->fDisconnect = true;
                } else if (pnode->nPingNonceSent && pnode->nPingUsecStart + TIMEOUT_INTERVAL * 1000000 < GetTimeMicros()) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    LogPrintf("ping timeout: %fs\n", 0.000001 * (GetTimeMicros() - pnode->nPingUsecStart));
                    pnode->fDisconnect = true;
                }
            }
        }
        {
            LOCK(cs_vNodes);
<<<<<<< HEAD
            for (CNode * pnode : vNodesCopy)
            pnode->Release();
=======
            for (CNode* pnode : vNodesCopy)
                pnode->Release();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }
}

<<<<<<< HEAD

#ifdef USE_UPNP
void ThreadMapPort()
{
    std::string port = strprintf("%u", GetListenPort());
    const char* multicastif = 0;
    const char* minissdpdpath = 0;
    struct UPNPDev* devlist = 0;
    char lanaddr[64];

#ifndef UPNPDISCOVER_SUCCESS
    /* miniupnpc 1.5 */
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0);
#elif MINIUPNPC_API_VERSION < 14
    /* miniupnpc 1.6 */
    int error = 0;
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, &error);
#else
    /* miniupnpc 1.9.20150730 */
    int error = 0;
    devlist = upnpDiscover(2000, multicastif, minissdpdpath, 0, 0, 2, &error);
#endif

    struct UPNPUrls urls;
    struct IGDdatas data;
    int r;

    r = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr));
    if (r == 1) {
        if (fDiscover) {
            char externalIPAddress[40];
            r = UPNP_GetExternalIPAddress(urls.controlURL, data.first.servicetype, externalIPAddress);
            if (r != UPNPCOMMAND_SUCCESS)
                LogPrintf("UPnP: GetExternalIPAddress() returned %d\n", r);
            else {
                if (externalIPAddress[0]) {
                    LogPrintf("UPnP: ExternalIPAddress = %s\n", externalIPAddress);
                    AddLocal(CNetAddr(externalIPAddress), LOCAL_UPNP);
                } else
                    LogPrintf("UPnP: GetExternalIPAddress failed.\n");
            }
        }

        string strDesc = "PRCY " + FormatFullVersion();

        try {
            while (true) {
#ifndef UPNPDISCOVER_SUCCESS
                /* miniupnpc 1.5 */
                r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                    port.c_str(), port.c_str(), lanaddr, strDesc.c_str(), "TCP", 0);
#else
                /* miniupnpc 1.6 */
                r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype,
                    port.c_str(), port.c_str(), lanaddr, strDesc.c_str(), "TCP", 0, "0");
#endif

                if (r != UPNPCOMMAND_SUCCESS)
                    LogPrintf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
                        port, port, lanaddr, r, strupnperror(r));
                else
                    LogPrintf("UPnP Port Mapping successful.\n");
                ;

                MilliSleep(20 * 60 * 1000); // Refresh every 20 minutes
            }
        } catch (const boost::thread_interrupted&) {
            r = UPNP_DeletePortMapping(urls.controlURL, data.first.servicetype, port.c_str(), "TCP", 0);
            LogPrintf("UPNP_DeletePortMapping() returned : %d\n", r);
            freeUPNPDevlist(devlist);
            devlist = 0;
            FreeUPNPUrls(&urls);
            throw;
        }
    } else {
        LogPrintf("No valid UPnP IGDs found\n");
        freeUPNPDevlist(devlist);
        devlist = 0;
        if (r != 0)
            FreeUPNPUrls(&urls);
    }
}

void MapPort(bool fUseUPnP)
{
    static boost::thread* upnp_thread = NULL;

    if (fUseUPnP) {
        if (upnp_thread) {
            upnp_thread->interrupt();
            upnp_thread->join();
            delete upnp_thread;
        }
        upnp_thread = new boost::thread(boost::bind(&TraceThread<void (*)()>, "upnp", &ThreadMapPort));
    } else if (upnp_thread) {
        upnp_thread->interrupt();
        upnp_thread->join();
        delete upnp_thread;
        upnp_thread = NULL;
    }
}

#else

void MapPort(bool) {
    // Intentionally left blank.
}

#endif


void ThreadDNSAddressSeed() {
    // goal: only query DNS seeds if address need is acute
    if ((addrman.size() > 0) &&
        (!GetBoolArg("-forcednsseed", false))) {
        MilliSleep(11 * 1000);

        LOCK(cs_vNodes);
        if (vNodes.size() >= 3) {
=======
void CConnman::WakeMessageHandler()
{
    {
        std::lock_guard<std::mutex> lock(mutexMsgProc);
        fMsgProcWake = true;
    }
    condMsgProc.notify_one();
}


static std::string GetDNSHost(const CDNSSeedData& data, ServiceFlags* requiredServiceBits)
{
    //use default host for non-filter-capable seeds or if we use the default service bits (NODE_NETWORK)
    if (!data.supportsServiceBitsFiltering || *requiredServiceBits == NODE_NETWORK) {
        *requiredServiceBits = NODE_NETWORK;
        return data.host;
    }

    return strprintf("x%x.%s", *requiredServiceBits, data.host);
}


void CConnman::ThreadDNSAddressSeed()
{
    // goal: only query DNS seeds if address need is acute
    if ((addrman.size() > 0) &&
        (!gArgs.GetBoolArg("-forcednsseed", DEFAULT_FORCEDNSSEED))) {
        if (!interruptNet.sleep_for(std::chrono::seconds(11)))
            return;

        LOCK(cs_vNodes);
        if (vNodes.size() >= 2) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            LogPrintf("P2P peers available. Skipped DNS seeding.\n");
            return;
        }
    }

<<<<<<< HEAD
    const vector <CDNSSeedData> &vSeeds = Params().DNSSeeds();
=======
    const std::vector<CDNSSeedData>& vSeeds = Params().DNSSeeds();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int found = 0;

    LogPrintf("Loading addresses from DNS seeds (could take a while)\n");

<<<<<<< HEAD
    for (const CDNSSeedData &seed : vSeeds) {
        if (HaveNameProxy()) {
            AddOneShot(seed.host);
        } else {
            vector <CNetAddr> vIPs;
            vector <CAddress> vAdd;
            uint64_t requiredServiceBits = NODE_NETWORK;
            if (LookupHost(seed.getHost(requiredServiceBits).c_str(), vIPs, 0, true)) {
                for (CNetAddr & ip : vIPs)
                {
                    int nOneDay = 24 * 3600;
                    CAddress addr = CAddress(CService(ip, Params().GetDefaultPort()), requiredServiceBits);
                    addr.nTime =
                            GetTime() - 3 * nOneDay - GetRand(4 * nOneDay); // use a random age between 3 and 7 days old
                    vAdd.push_back(addr);
                    found++;
                }
            }
            // TODO: The seed name resolve may fail, yielding an IP of [::], which results in
            // addrman assigning the same source to results from different seeds.
            // This should switch to a hard-coded stable dummy IP for each seed name, so that the
            // resolve is not required at all.
            if (!vIPs.empty()) {
                CService seedSource;
                Lookup(seed.name.c_str(), seedSource, 0, true);
                addrman.Add(vAdd, seedSource);
=======
    for (const CDNSSeedData& seed : vSeeds) {
        if (interruptNet) {
            return;
        }
        if (HaveNameProxy()) {
            AddOneShot(seed.host);
        } else {
            std::vector<CNetAddr> vIPs;
            std::vector<CAddress> vAdd;
            ServiceFlags requiredServiceBits = nRelevantServices;
            std::string host = GetDNSHost(seed, &requiredServiceBits);
            CNetAddr resolveSource;
            if (!resolveSource.SetInternal(host)) {
                continue;
            }
            if (LookupHost(host, vIPs, 0, true)) {
                for (CNetAddr& ip : vIPs) {
                    int nOneDay = 24*3600;
                    CAddress addr = CAddress(CService(ip, Params().GetDefaultPort()), requiredServiceBits);
                    addr.nTime = GetTime() - 3 * nOneDay - GetRand(4 * nOneDay); // use a random age between 3 and 7 days old
                    vAdd.push_back(addr);
                    found++;
                }
                addrman.Add(vAdd, resolveSource);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }
    }

    LogPrintf("%d addresses found from DNS seeds\n", found);
}


<<<<<<< HEAD
void DumpAddresses() {
=======










void CConnman::DumpAddresses()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int64_t nStart = GetTimeMillis();

    CAddrDB adb;
    adb.Write(addrman);

<<<<<<< HEAD
    LogPrint("net", "Flushed %d addresses to peers.dat  %dms\n",
             addrman.size(), GetTimeMillis() - nStart);
}

void DumpData() {
=======
    LogPrint(BCLog::NET, "Flushed %d addresses to peers.dat  %dms\n",
        addrman.size(), GetTimeMillis() - nStart);
}

void CConnman::DumpData()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    DumpAddresses();
    DumpBanlist();
}

<<<<<<< HEAD
void static ProcessOneShot() {
    string strDest;
=======
void CConnman::ProcessOneShot()
{
    std::string strDest;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_vOneShots);
        if (vOneShots.empty())
            return;
        strDest = vOneShots.front();
        vOneShots.pop_front();
    }
    CAddress addr;
    CSemaphoreGrant grant(*semOutbound, true);
    if (grant) {
<<<<<<< HEAD
        OpenNetworkConnection(addr, &grant, strDest.c_str(), true);
    }
}

void ThreadOpenConnections() {
    // Connect to specific addresses
    if (mapArgs.count("-connect") && mapMultiArgs["-connect"].size() > 0) {
        for (int64_t nLoop = 0;; nLoop++) {
            ProcessOneShot();
            for (string strAddr : mapMultiArgs["-connect"]) {
                CAddress addr;
                OpenNetworkConnection(addr, NULL, strAddr.c_str());
                for (int i = 0; i < 10 && i < nLoop; i++) {
                    MilliSleep(500);
                }
            }
            MilliSleep(500);
=======
        OpenNetworkConnection(addr, false, &grant, strDest.c_str(), true);
    }
}

void CConnman::ThreadOpenConnections(const std::vector<std::string> connect)
{
    // Connect to specific addresses
    if (!connect.empty()) {
        for (int64_t nLoop = 0;; nLoop++) {
            ProcessOneShot();
            for (const std::string& strAddr : connect) {
                CAddress addr(CService(), NODE_NONE);
                OpenNetworkConnection(addr, false, nullptr, strAddr.c_str());
                for (int i = 0; i < 10 && i < nLoop; i++) {
                    if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                        return;
                }
            }
            if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    // Initiate network connections
    int64_t nStart = GetTime();
<<<<<<< HEAD
    while (true) {
        ProcessOneShot();

        MilliSleep(500);

        CSemaphoreGrant grant(*semOutbound);
        boost::this_thread::interruption_point();
=======

    // Minimum time before next feeler connection (in microseconds).
    int64_t nNextFeeler = PoissonNextSend(nStart * 1000 * 1000, FEELER_INTERVAL);
    while (!interruptNet) {
        ProcessOneShot();

        if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
            return;

        CSemaphoreGrant grant(*semOutbound);
        if (interruptNet)
            return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Add seed nodes if DNS seeds are all down (an infrastructure attack?).
        if (addrman.size() == 0 && (GetTime() - nStart > 60)) {
            static bool done = false;
            if (!done) {
                LogPrintf("Adding fixed seed nodes as DNS doesn't seem to be available.\n");
<<<<<<< HEAD
                addrman.Add(convertSeed6(Params().FixedSeeds()), CNetAddr("127.0.0.1"));
=======
                CNetAddr local;
                local.SetInternal("fixedseeds");
                addrman.Add(ConvertSeeds(Params().FixedSeeds()), local);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                done = true;
            }
        }

        //
        // Choose an address to connect to based on most recently seen
        //
        CAddress addrConnect;

        // Only connect out to one peer per network group (/16 for IPv4).
        // Do this here so we don't have to critsect vNodes inside mapAddresses critsect.
        int nOutbound = 0;
<<<<<<< HEAD
        set <vector<unsigned char>> setConnected;
        {
            LOCK(cs_vNodes);
            for (CNode * pnode : vNodes)
            {
                if (!pnode->fInbound) {
                    setConnected.insert(pnode->addr.GetGroup());
=======
        std::set<std::vector<unsigned char> > setConnected;
        {
            LOCK(cs_vNodes);
            for (const CNode* pnode : vNodes) {
                if (!pnode->fInbound && !pnode->fAddnode) {
                    // Netgroups for inbound and addnode peers are not excluded because our goal here
                    // is to not use multiple of our limited outbound slots on a single netgroup
                    // but inbound and addnode peers do not use our outbound slots. Inbound peers
                    // also have the added issue that they're attacker controlled and could be used
                    // to prevent us from connecting to particular hosts if we used them here.
                    setConnected.insert(pnode->addr.GetGroup(addrman.m_asmap));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    nOutbound++;
                }
            }
        }

<<<<<<< HEAD
        int64_t nANow = GetAdjustedTime();

        int nTries = 0;
        while (true) {
            CAddrInfo addr = addrman.Select();

            // if we selected an invalid address, restart
            if (!addr.IsValid() || setConnected.count(addr.GetGroup()) || IsLocal(addr))
                break;
=======
        // Feeler Connections
        //
        // Design goals:
        //  * Increase the number of connectable addresses in the tried table.
        //
        // Method:
        //  * Choose a random address from new and attempt to connect to it if we can connect
        //    successfully it is added to tried.
        //  * Start attempting feeler connections only after node finishes making outbound
        //    connections.
        //  * Only make a feeler connection once every few minutes.
        //
        bool fFeeler = false;
        if (nOutbound >= nMaxOutbound) {
            int64_t nTime = GetTimeMicros(); // The current time right now (in microseconds).
            if (nTime > nNextFeeler) {
                nNextFeeler = PoissonNextSend(nTime, FEELER_INTERVAL);
                fFeeler = true;
            } else {
                continue;
            }
        }

        addrman.ResolveCollisions();

        int64_t nANow = GetAdjustedTime();
        int nTries = 0;
        while (!interruptNet) {
            CAddrInfo addr = addrman.SelectTriedCollision();

            // SelectTriedCollision returns an invalid address if it is empty.
            if (!fFeeler || !addr.IsValid()) {
                addr = addrman.Select(fFeeler);
            }

            // Require outbound connections, other than feelers, to be to distinct network groups
            if (!fFeeler && setConnected.count(addr.GetGroup(addrman.m_asmap))) {
                break;
            }

            // if we selected an invalid or local address, restart
            if (!addr.IsValid() || IsLocal(addr)) {
                break;
            }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            // If we didn't find an appropriate destination after trying 100 addresses fetched from addrman,
            // stop this loop, and let the outer loop run again (which sleeps, adds seed nodes, recalculates
            // already-connected network ranges, ...) before trying new addrman addresses.
            nTries++;
            if (nTries > 100)
                break;

<<<<<<< HEAD
            if (IsLimited(addr))
=======
            if (!IsReachable(addr))
                continue;

            // only connect to full nodes
            if ((addr.nServices & REQUIRED_SERVICES) != REQUIRED_SERVICES)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                continue;

            // only consider very recently tried nodes after 30 failed attempts
            if (nANow - addr.nLastTry < 600 && nTries < 30)
                continue;

            // do not allow non-default ports, unless after 50 invalid addresses selected already
            if (addr.GetPort() != Params().GetDefaultPort() && nTries < 50)
                continue;

            addrConnect = addr;
            break;
        }

<<<<<<< HEAD
        if (addrConnect.IsValid())
            OpenNetworkConnection(addrConnect, &grant);
    }
}

void ThreadOpenAddedConnections() {
    {
        LOCK(cs_vAddedNodes);
        vAddedNodes = mapMultiArgs["-addnode"];
    }

    if (HaveNameProxy()) {
        while (true) {
            list <string> lAddresses(0);
            {
                LOCK(cs_vAddedNodes);
                for (string & strAddNode : vAddedNodes)
                lAddresses.push_back(strAddNode);
            }
            for (string & strAddNode : lAddresses)
            {
                CAddress addr;
                CSemaphoreGrant grant(*semOutbound);
                OpenNetworkConnection(addr, &grant, strAddNode.c_str());
                MilliSleep(500);
            }
            MilliSleep(120000); // Retry every 2 minutes
        }
    }

    for (unsigned int i = 0; true; i++) {
        list <string> lAddresses(0);
        {
            LOCK(cs_vAddedNodes);
            for (string & strAddNode : vAddedNodes)
            lAddresses.push_back(strAddNode);
        }

        list <vector<CService>> lservAddressesToAdd(0);
        for (string & strAddNode : lAddresses)
        {
            vector <CService> vservNode(0);
            if (Lookup(strAddNode.c_str(), vservNode, Params().GetDefaultPort(), fNameLookup, 0)) {
                lservAddressesToAdd.push_back(vservNode);
                {
                    LOCK(cs_setservAddNodeAddresses);
                    for (CService & serv : vservNode)
                    setservAddNodeAddresses.insert(serv);
                }
            }
        }
        // Attempt to connect to each IP for each addnode entry until at least one is successful per addnode entry
        // (keeping in mind that addnode entries can have many IPs if fNameLookup)
        {
            LOCK(cs_vNodes);
            for (CNode * pnode : vNodes)
            for (list < vector < CService > > ::iterator it = lservAddressesToAdd.begin(); it !=
                                                                                           lservAddressesToAdd.end();
            it++)
            for (CService & addrNode : *(it))
            if (pnode->addr == addrNode) {
                it = lservAddressesToAdd.erase(it);
                it--;
                break;
            }
        }
        for (vector < CService > &vserv : lservAddressesToAdd)
        {
            CSemaphoreGrant grant(*semOutbound);
            OpenNetworkConnection(CAddress(vserv[i % vserv.size()]), &grant);
            MilliSleep(500);
        }
        MilliSleep(120000); // Retry every 2 minutes
=======
        if (addrConnect.IsValid()) {
            if (fFeeler) {
                // Add small amount of random noise before connection to avoid synchronization.
                int randsleep = GetRandInt(FEELER_SLEEP_WINDOW * 1000);
                if (!interruptNet.sleep_for(std::chrono::milliseconds(randsleep)))
                    return;
                LogPrint(BCLog::NET, "Making feeler connection to %s\n", addrConnect.ToString());
            }

            OpenNetworkConnection(addrConnect, (int)setConnected.size() >= std::min(nMaxConnections - 1, 2), &grant, nullptr, false, fFeeler);
        }
    }
}

std::vector<AddedNodeInfo> CConnman::GetAddedNodeInfo()
{
    std::vector<AddedNodeInfo> ret;

    std::list<std::string> lAddresses(0);
    {
        LOCK(cs_vAddedNodes);
        ret.reserve(vAddedNodes.size());
        std::copy(vAddedNodes.cbegin(), vAddedNodes.cend(), std::back_inserter(lAddresses));
    }


    // Build a map of all already connected addresses (by IP:port and by name) to inbound/outbound and resolved CService
    std::map<CService, bool> mapConnected;
    std::map<std::string, std::pair<bool, CService> > mapConnectedByName;
    {
        LOCK(cs_vNodes);
        for (const CNode* pnode : vNodes) {
            if (pnode->addr.IsValid()) {
                mapConnected[pnode->addr] = pnode->fInbound;
            }
            std::string addrName = pnode->GetAddrName();
            if (!addrName.empty()) {
                mapConnectedByName[std::move(addrName)] = std::make_pair(pnode->fInbound, static_cast<const CService&>(pnode->addr));
            }
        }
    }

    for (const std::string& strAddNode : lAddresses) {
        CService service(LookupNumeric(strAddNode, Params().GetDefaultPort()));
        AddedNodeInfo addedNode{strAddNode, CService(), false, false};
        if (service.IsValid()) {
            // strAddNode is an IP:port
            auto it = mapConnected.find(service);
            if (it != mapConnected.end()) {
                addedNode.resolvedAddress = service;
                addedNode.fConnected = true;
                addedNode.fInbound = it->second;
            }
        } else {
            // strAddNode is a name
            auto it = mapConnectedByName.find(strAddNode);
            if (it != mapConnectedByName.end()) {
                addedNode.resolvedAddress = it->second.second;
                addedNode.fConnected = true;
                addedNode.fInbound = it->second.first;
            }
        }
        ret.emplace_back(std::move(addedNode));
    }

    return ret;
}

void CConnman::ThreadOpenAddedConnections()
{
    while (true) {
        CSemaphoreGrant grant(*semAddnode);
        std::vector<AddedNodeInfo> vInfo = GetAddedNodeInfo();
        bool tried = false;
        for (const AddedNodeInfo& info : vInfo) {
            if (!info.fConnected) {
                if (!grant.TryAcquire()) {
                    // If we've used up our semaphore and need a new one, lets not wait here since while we are waiting
                    // the addednodeinfo state might change.
                    break;
                }
                tried = true;
                CAddress addr(CService(), NODE_NONE);
                OpenNetworkConnection(addr, false, &grant, info.strAddedNode.c_str(), false, false, true);
                if (!interruptNet.sleep_for(std::chrono::milliseconds(500)))
                    return;
            }
        }
        // Retry every 60 seconds if a connection was attempted, otherwise two seconds
        if (!interruptNet.sleep_for(std::chrono::seconds(tried ? 60 : 2)))
            return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

// if successful, this moves the passed grant to the constructed node
<<<<<<< HEAD
void OpenNetworkConnection(const CAddress &addrConnect, CSemaphoreGrant *grantOutbound, const char *pszDest, bool fOneShot) {
    //
    // Initiate outbound network connection
    //
    boost::this_thread::interruption_point();
    if (!pszDest) {
        if (IsLocal(addrConnect) ||
            FindNode((CNetAddr) addrConnect) || CNode::IsBanned(addrConnect) ||
=======
void CConnman::OpenNetworkConnection(const CAddress& addrConnect, bool fCountFailure, CSemaphoreGrant* grantOutbound, const char* pszDest, bool fOneShot, bool fFeeler, bool fAddnode)
{
    //
    // Initiate outbound network connection
    //
    if (interruptNet || !fNetworkActive) {
        return;
    }
    if (!pszDest) {
        if (IsLocal(addrConnect) ||
            FindNode(static_cast<CNetAddr>(addrConnect)) || IsBanned(addrConnect) ||
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            FindNode(addrConnect.ToStringIPPort()))
            return;
    } else if (FindNode(pszDest))
        return;

<<<<<<< HEAD
    CNode *pnode = ConnectNode(addrConnect, pszDest);
    boost::this_thread::interruption_point();
=======
    CNode* pnode = ConnectNode(addrConnect, pszDest, fCountFailure);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (!pnode)
        return;
    if (grantOutbound)
        grantOutbound->MoveTo(pnode->grantOutbound);
<<<<<<< HEAD
    pnode->fNetworkNode = true;
    if (fOneShot)
        pnode->fOneShot = true;
}

void ThreadMessageHandler() {
    boost::mutex condition_mutex;
    boost::unique_lock<boost::mutex> lock(condition_mutex);

    SetThreadPriority(THREAD_PRIORITY_BELOW_NORMAL);
    while (true) {
        vector < CNode * > vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode * pnode : vNodesCopy)
            {
=======
    if (fOneShot)
        pnode->fOneShot = true;
    if (fFeeler)
        pnode->fFeeler = true;
    if (fAddnode)
        pnode->fAddnode = true;

    m_msgproc->InitializeNode(pnode);
    {
        LOCK(cs_vNodes);
        vNodes.push_back(pnode);
    }
}

void CConnman::ThreadMessageHandler()
{
    while (!flagInterruptMsgProc) {
        std::vector<CNode*> vNodesCopy;
        {
            LOCK(cs_vNodes);
            vNodesCopy = vNodes;
            for (CNode* pnode : vNodesCopy) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                pnode->AddRef();
            }
        }

<<<<<<< HEAD
        // Poll the connected nodes for messages
        CNode *pnodeTrickle = NULL;
        if (!vNodesCopy.empty())
            pnodeTrickle = vNodesCopy[GetRand(vNodesCopy.size())];

        bool fSleep = true;
        for (CNode * pnode : vNodesCopy)
        {   
            if (!pnode) continue;
            if (pnode->fDisconnect)
                continue;
            // Receive messages
            {
                TRY_LOCK(pnode->cs_vRecvMsg, lockRecv);
                if (lockRecv) {
                    if (!g_signals.ProcessMessages(pnode)) {
                        pnode->CloseSocketDisconnect();
                    }
                    if (pnode->nSendSize < SendBufferSize()) {
                        if (!pnode->vRecvGetData.empty() ||
                            (!pnode->vRecvMsg.empty() && pnode->vRecvMsg[0].complete())) {
                            fSleep = false;
                        }
                    }
                }
            }
            boost::this_thread::interruption_point();
            // Send messages
            {
                TRY_LOCK(cs_main, lockMain);
                    if (lockMain) {
                    TRY_LOCK(pnode->cs_vSend, lockSend);
                    if (lockSend)
                        g_signals.SendMessages(pnode, pnode == pnodeTrickle || pnode->fWhitelisted);
                }
            }
            boost::this_thread::interruption_point();
        }

        {
            LOCK(cs_vNodes);
            for (CNode * pnode : vNodesCopy)
            pnode->Release();
        }

        if (fSleep)
            messageHandlerCondition.timed_wait(lock, boost::posix_time::microsec_clock::universal_time() +
                                                     boost::posix_time::milliseconds(100));
    }
}

bool BindListenPort(const CService &addrBind, string &strError, bool fWhitelisted) {
=======
        bool fMoreWork = false;

        for (CNode* pnode : vNodesCopy) {
            if (pnode->fDisconnect)
                continue;

            // Receive messages
            bool fMoreNodeWork = m_msgproc->ProcessMessages(pnode, flagInterruptMsgProc);
            fMoreWork |= (fMoreNodeWork && !pnode->fPauseSend);
            if (flagInterruptMsgProc)
                return;

            // Send messages
            {
                LOCK(pnode->cs_sendProcessing);
                m_msgproc->SendMessages(pnode, flagInterruptMsgProc);
            }

            if (flagInterruptMsgProc)
                return;
        }


        {
            LOCK(cs_vNodes);
            for (CNode* pnode : vNodesCopy)
                pnode->Release();
        }

        std::unique_lock<std::mutex> lock(mutexMsgProc);
        if (!fMoreWork) {
            condMsgProc.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::milliseconds(100), [this] { return fMsgProcWake; });
        }
        fMsgProcWake = false;
    }
}

bool CConnman::BindListenPort(const CService& addrBind, std::string& strError, bool fWhitelisted)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    strError = "";
    int nOne = 1;

    // Create socket for listening for incoming connections
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
<<<<<<< HEAD
    if (!addrBind.GetSockAddr((struct sockaddr *) &sockaddr, &len)) {
=======
    if (!addrBind.GetSockAddr((struct sockaddr*)&sockaddr, &len)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        strError = strprintf("Error: Bind address family for %s not supported", addrBind.ToString());
        LogPrintf("%s\n", strError);
        return false;
    }

<<<<<<< HEAD
    SOCKET hListenSocket = socket(((struct sockaddr *) &sockaddr)->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (hListenSocket == INVALID_SOCKET) {
        strError = strprintf("Error: Couldn't open socket for incoming connections (socket returned error %s)",
                             NetworkErrorString(WSAGetLastError()));
        LogPrintf("%s\n", strError);
        return false;
    }
    if (!IsSelectableSocket(hListenSocket)) {
        strError = "Error: Couldn't create a listenable socket for incoming connections";
=======
    SOCKET hListenSocket = CreateSocket(addrBind);
    if (hListenSocket == INVALID_SOCKET) {
        strError = strprintf("Error: Couldn't open socket for incoming connections (socket returned error %s)", NetworkErrorString(WSAGetLastError()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        LogPrintf("%s\n", strError);
        return false;
    }

<<<<<<< HEAD

#ifndef WIN32
#ifdef SO_NOSIGPIPE
    // Different way of disabling SIGPIPE on BSD
    setsockopt(hListenSocket, SOL_SOCKET, SO_NOSIGPIPE, (void*)&nOne, sizeof(int));
#endif
    // Allow binding if the port is still in TIME_WAIT state after
    // the program was closed and restarted. Not an issue on windows!
    setsockopt(hListenSocket, SOL_SOCKET, SO_REUSEADDR, (void *) &nOne, sizeof(int));
#endif

    // Set to non-blocking, incoming connections will also inherit this
    if (!SetSocketNonBlocking(hListenSocket, true)) {
        strError = strprintf("BindListenPort: Setting listening socket to non-blocking failed, error %s\n",
                             NetworkErrorString(WSAGetLastError()));
        LogPrintf("%s\n", strError);
        return false;
    }

=======
#ifndef WIN32
    // Allow binding if the port is still in TIME_WAIT state after
    // the program was closed and restarted. Not an issue on windows!
    setsockopt(hListenSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&nOne, sizeof(int));
#endif

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // some systems don't have IPV6_V6ONLY but are always v6only; others do have the option
    // and enable it by default or not. Try to enable it, if possible.
    if (addrBind.IsIPv6()) {
#ifdef IPV6_V6ONLY
<<<<<<< HEAD
#ifdef WIN32
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&nOne, sizeof(int));
#else
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&nOne, sizeof(int));
#endif
=======
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (sockopt_arg_type)&nOne, sizeof(int));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif
#ifdef WIN32
        int nProtLevel = PROTECTION_LEVEL_UNRESTRICTED;
        setsockopt(hListenSocket, IPPROTO_IPV6, IPV6_PROTECTION_LEVEL, (const char*)&nProtLevel, sizeof(int));
#endif
    }

<<<<<<< HEAD
    if (::bind(hListenSocket, (struct sockaddr *) &sockaddr, len) == SOCKET_ERROR) {
        int nErr = WSAGetLastError();
        if (nErr == WSAEADDRINUSE)
            strError = strprintf(_("Unable to bind to %s on this computer. PRCY is probably already running."),
                                 addrBind.ToString());
        else
            strError = strprintf(_("Unable to bind to %s on this computer (bind returned error %s)"),
                                 addrBind.ToString(), NetworkErrorString(nErr));
=======
    if (::bind(hListenSocket, (struct sockaddr*)&sockaddr, len) == SOCKET_ERROR) {
        int nErr = WSAGetLastError();
        if (nErr == WSAEADDRINUSE)
            strError = strprintf(_("Unable to bind to %s on this computer. %s is probably already running."), addrBind.ToString(), PACKAGE_NAME);
        else
            strError = strprintf(_("Unable to bind to %s on this computer (bind returned error %s)"), addrBind.ToString(), NetworkErrorString(nErr));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        LogPrintf("%s\n", strError);
        CloseSocket(hListenSocket);
        return false;
    }
    LogPrintf("Bound to %s\n", addrBind.ToString());

    // Listen for incoming connections
    if (listen(hListenSocket, SOMAXCONN) == SOCKET_ERROR) {
<<<<<<< HEAD
        strError = strprintf(_("Error: Listening for incoming connections failed (listen returned error %s)"),
                             NetworkErrorString(WSAGetLastError()));
=======
        strError = strprintf(_("Error: Listening for incoming connections failed (listen returned error %s)"), NetworkErrorString(WSAGetLastError()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        LogPrintf("%s\n", strError);
        CloseSocket(hListenSocket);
        return false;
    }

<<<<<<< HEAD
    vhListenSocket.push_back(ListenSocket(hListenSocket, fWhitelisted));
=======
    vhListenSocket.emplace_back(hListenSocket, fWhitelisted);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (addrBind.IsRoutable() && fDiscover && !fWhitelisted)
        AddLocal(addrBind, LOCAL_BIND);

    return true;
}

<<<<<<< HEAD
void static Discover(boost::thread_group &threadGroup) {
=======
void Discover()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fDiscover)
        return;

#ifdef WIN32
    // Get local host IP
    char pszHostName[256] = "";
    if (gethostname(pszHostName, sizeof(pszHostName)) != SOCKET_ERROR) {
<<<<<<< HEAD
        vector<CNetAddr> vaddr;
=======
        std::vector<CNetAddr> vaddr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (LookupHost(pszHostName, vaddr, 0, true)) {
            for (const CNetAddr& addr : vaddr) {
                if (AddLocal(addr, LOCAL_IF))
                    LogPrintf("%s: %s - %s\n", __func__, pszHostName, addr.ToString());
            }
        }
    }
<<<<<<< HEAD
#else
    // Get local host ip
    struct ifaddrs *myaddrs;
    if (getifaddrs(&myaddrs) == 0) {
        for (struct ifaddrs *ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
=======
#elif (HAVE_DECL_GETIFADDRS && HAVE_DECL_FREEIFADDRS)
    // Get local host ip
    struct ifaddrs* myaddrs;
    if (getifaddrs(&myaddrs) == 0) {
        for (struct ifaddrs* ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (ifa->ifa_addr == NULL) continue;
            if ((ifa->ifa_flags & IFF_UP) == 0) continue;
            if (strcmp(ifa->ifa_name, "lo") == 0) continue;
            if (strcmp(ifa->ifa_name, "lo0") == 0) continue;
            if (ifa->ifa_addr->sa_family == AF_INET) {
<<<<<<< HEAD
                struct sockaddr_in *s4 = (struct sockaddr_in *) (ifa->ifa_addr);
=======
                struct sockaddr_in* s4 = (struct sockaddr_in*)(ifa->ifa_addr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                CNetAddr addr(s4->sin_addr);
                if (AddLocal(addr, LOCAL_IF))
                    LogPrintf("%s: IPv4 %s: %s\n", __func__, ifa->ifa_name, addr.ToString());
            } else if (ifa->ifa_addr->sa_family == AF_INET6) {
<<<<<<< HEAD
                struct sockaddr_in6 *s6 = (struct sockaddr_in6 *) (ifa->ifa_addr);
=======
                struct sockaddr_in6* s6 = (struct sockaddr_in6*)(ifa->ifa_addr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                CNetAddr addr(s6->sin6_addr);
                if (AddLocal(addr, LOCAL_IF))
                    LogPrintf("%s: IPv6 %s: %s\n", __func__, ifa->ifa_name, addr.ToString());
            }
        }
        freeifaddrs(myaddrs);
    }
#endif
}

<<<<<<< HEAD
void StartNode(boost::thread_group &threadGroup, CScheduler &scheduler) {
    uiInterface.InitMessage(_("Loading addresses..."));
=======
void CConnman::SetNetworkActive(bool active)
{
    LogPrint(BCLog::NET, "SetNetworkActive: %s\n", active);

    if (fNetworkActive == active) {
        return;
    }

    fNetworkActive = active;

    if (!fNetworkActive) {
        LOCK(cs_vNodes);
        // Close sockets to all nodes
        for(CNode* pnode : vNodes) {
            pnode->CloseSocketDisconnect();
        }
    }

    if (clientInterface) clientInterface->NotifyNetworkActiveChanged(fNetworkActive);
}

CConnman::CConnman(uint64_t nSeed0In, uint64_t nSeed1In) : nSeed0(nSeed0In), nSeed1(nSeed1In)
{
    setBannedIsDirty = false;
    fAddressesInitialized = false;
    nLastNodeId = 0;
    nSendBufferMaxSize = 0;
    nReceiveFloodSize = 0;
    flagInterruptMsgProc = false;

    Options connOptions;
    Init(connOptions);
}

NodeId CConnman::GetNewNodeId()
{
    return nLastNodeId.fetch_add(1, std::memory_order_relaxed);
}

bool CConnman::Bind(const CService& addr, unsigned int flags) {
    if (!(flags & BF_EXPLICIT) && !IsReachable(addr))
        return false;
    std::string strError;
    if (!BindListenPort(addr, strError, (flags & BF_WHITELIST) != 0)) {
        if ((flags & BF_REPORT_ERROR) && clientInterface) {
            clientInterface->ThreadSafeMessageBox(strError, "", CClientUIInterface::MSG_ERROR);
        }
        return false;
    }
    return true;
}

bool CConnman::InitBinds(const std::vector<CService>& binds, const std::vector<CService>& whiteBinds) {
    bool fBound = false;
    for (const auto& addrBind : binds) {
        fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR));
    }
    for (const auto& addrBind : whiteBinds) {
        fBound |= Bind(addrBind, (BF_EXPLICIT | BF_REPORT_ERROR | BF_WHITELIST));
    }
    if (binds.empty() && whiteBinds.empty()) {
        struct in_addr inaddr_any;
        inaddr_any.s_addr = INADDR_ANY;
        fBound |= Bind(CService((in6_addr)IN6ADDR_ANY_INIT, GetListenPort()), BF_NONE);
        fBound |= Bind(CService(inaddr_any, GetListenPort()), !fBound ? BF_REPORT_ERROR : BF_NONE);
    }
    return fBound;
}

bool CConnman::Start(CScheduler& scheduler, const Options& connOptions)
{
    Init(connOptions);

    {
        LOCK(cs_totalBytesRecv);
        nTotalBytesRecv = 0;
    }
    {
        LOCK(cs_totalBytesSent);
        nTotalBytesSent = 0;
    }

    if (fListen && !InitBinds(connOptions.vBinds, connOptions.vWhiteBinds)) {
        if (clientInterface) {
            clientInterface->ThreadSafeMessageBox(
                    _("Failed to listen on any port. Use -listen=0 if you want this."),
                    "", CClientUIInterface::MSG_ERROR);
        }
        return false;
    }

    for (const auto& strDest : connOptions.vSeedNodes) {
        AddOneShot(strDest);
    }

    if (clientInterface)
        clientInterface->InitMessage(_("Loading addresses..."));
    m_msgproc = connOptions.m_msgproc;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Load addresses from peers.dat
    int64_t nStart = GetTimeMillis();
    {
        CAddrDB adb;
        if (adb.Read(addrman))
            LogPrintf("Loaded %i addresses from peers.dat  %dms\n", addrman.size(), GetTimeMillis() - nStart);
        else {
            addrman.Clear(); // Addrman can be in an inconsistent state after failure, reset it
            LogPrintf("Invalid or missing peers.dat; recreating\n");
            DumpAddresses();
        }
    }
<<<<<<< HEAD

    uiInterface.InitMessage(_("Loading banlist..."));
=======
    if (clientInterface)
        clientInterface->InitMessage(_("Loading banlist..."));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Load addresses from banlist.dat
    nStart = GetTimeMillis();
    CBanDB bandb;
    banmap_t banmap;
    if (bandb.Read(banmap)) {
<<<<<<< HEAD
        CNode::SetBanned(banmap); // thread save setter
        CNode::SetBannedSetDirty(false); // no need to write down, just read data
        CNode::SweepBanned(); // sweep out unused entries

        LogPrint("net", "Loaded %d banned node ips/subnets from banlist.dat  %dms\n",
            banmap.size(), GetTimeMillis() - nStart);
    } else {
        LogPrintf("Invalid or missing banlist.dat; recreating\n");
        CNode::SetBannedSetDirty(true); // force write
        DumpBanlist();
    }
    // Initialize random numbers. Even when rand() is only usable for trivial use-cases most nodes should have a different
    // seed after all the file-IO done at this point. Should be good enough even when nodes are started via scripts.
    srand(time(NULL));
    
    fAddressesInitialized = true;

    if (semOutbound == NULL) {
        // initialize semaphore
        int nMaxOutbound = min(MAX_OUTBOUND_CONNECTIONS, nMaxConnections);
        semOutbound = new CSemaphore(nMaxOutbound);
    }

    if (pnodeLocalHost == NULL)
        pnodeLocalHost = new CNode(INVALID_SOCKET, CAddress(CService("127.0.0.1", 0), nLocalServices));

    Discover(threadGroup);
=======
        SetBanned(banmap); // thread save setter
        SetBannedSetDirty(false); // no need to write down, just read data
        SweepBanned(); // sweep out unused entries

        LogPrint(BCLog::NET, "Loaded %d banned node ips/subnets from banlist.dat  %dms\n",
            banmap.size(), GetTimeMillis() - nStart);
    } else {
        LogPrintf("Invalid or missing banlist.dat; recreating\n");
        SetBannedSetDirty(true); // force write
        DumpBanlist();
    }

    // Initialize random numbers. Even when rand() is only usable for trivial use-cases most nodes should have a different
    // seed after all the file-IO done at this point. Should be good enough even when nodes are started via scripts.
    srand(time(NULL));

    fAddressesInitialized = true;

    if (semOutbound == nullptr) {
        // initialize semaphore
        semOutbound = std::make_unique<CSemaphore>(std::min((nMaxOutbound + nMaxFeeler), nMaxConnections));
    }
    if (semAddnode == nullptr) {
        // initialize semaphore
        semAddnode = std::make_unique<CSemaphore>(nMaxAddnode);
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    //
    // Start threads
    //
<<<<<<< HEAD

    if (!GetBoolArg("-dnsseed", true))
        LogPrintf("DNS seeding disabled\n");
    else
        threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "dnsseed", &ThreadDNSAddressSeed));

    // Map ports with UPnP
    MapPort(GetBoolArg("-upnp", DEFAULT_UPNP));

    // Send and receive from sockets, accept connections
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "net", &ThreadSocketHandler));

    // Initiate outbound connections from -addnode
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "addcon", &ThreadOpenAddedConnections));

    // Initiate outbound connections unless connect=0
    if (!mapArgs.count("-connect") || mapMultiArgs["-connect"].size() != 1 || mapMultiArgs["-connect"][0] != "0")
        threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "opencon", &ThreadOpenConnections));

    // Process messages
    threadGroup.create_thread(boost::bind(&TraceThread<void (*)()>, "msghand", &ThreadMessageHandler));

    // Dump network addresses
    scheduler.scheduleEvery(&DumpData, DUMP_ADDRESSES_INTERVAL);
}

bool StopNode() {
    LogPrintf("StopNode()\n");
    MapPort(false);
    if (semOutbound)
        for (int i = 0; i < MAX_OUTBOUND_CONNECTIONS; i++)
            semOutbound->post();

    if (fAddressesInitialized) {
        DumpData();
        fAddressesInitialized = false;
    }
=======
    assert(m_msgproc);
    InterruptSocks5(false);
    interruptNet.reset();
    flagInterruptMsgProc = false;

    {
        std::unique_lock<std::mutex> lock(mutexMsgProc);
        fMsgProcWake = false;
    }

    // Send and receive from sockets, accept connections
    threadSocketHandler = std::thread(&TraceThread<std::function<void()> >, "net", std::function<void()>(std::bind(&CConnman::ThreadSocketHandler, this)));

    if (!gArgs.GetBoolArg("-dnsseed", true))
        LogPrintf("DNS seeding disabled\n");
    else
        threadDNSAddressSeed = std::thread(&TraceThread<std::function<void()> >, "dnsseed", std::function<void()>(std::bind(&CConnman::ThreadDNSAddressSeed, this)));

    // Initiate outbound connections from -addnode
    threadOpenAddedConnections = std::thread(&TraceThread<std::function<void()> >, "addcon", std::function<void()>(std::bind(&CConnman::ThreadOpenAddedConnections, this)));

    if (connOptions.m_use_addrman_outgoing && !connOptions.m_specified_outgoing.empty()) {
        if (clientInterface) {
            clientInterface->ThreadSafeMessageBox(
                    _("Cannot provide specific connections and have addrman find outgoing connections at the same."),
                    "", CClientUIInterface::MSG_ERROR);
        }
        return false;
    }
    if (connOptions.m_use_addrman_outgoing || !connOptions.m_specified_outgoing.empty()) {
        threadOpenConnections = std::thread(&TraceThread<std::function<void()> >, "opencon", std::function<void()>(
                std::bind(&CConnman::ThreadOpenConnections, this, connOptions.m_specified_outgoing)));
    }

    // Process messages
    threadMessageHandler = std::thread(&TraceThread<std::function<void()> >, "msghand", std::function<void()>(std::bind(&CConnman::ThreadMessageHandler, this)));

    // Dump network addresses
    scheduler.scheduleEvery(std::bind(&CConnman::DumpData, this), DUMP_ADDRESSES_INTERVAL * 1000);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return true;
}

<<<<<<< HEAD
class CNetCleanup {
public:
    CNetCleanup() {}

    ~CNetCleanup() {
        // Close sockets
        for (CNode * pnode : vNodes)
        if (pnode->hSocket != INVALID_SOCKET)
            CloseSocket(pnode->hSocket);
        for (ListenSocket & hListenSocket : vhListenSocket)
        if (hListenSocket.socket != INVALID_SOCKET)
            if (!CloseSocket(hListenSocket.socket))
                LogPrintf("CloseSocket(hListenSocket) failed with error %s\n", NetworkErrorString(WSAGetLastError()));

        // clean up some globals (to help leak detection)
        for (CNode * pnode : vNodes)
        delete pnode;
        for (CNode * pnode : vNodesDisconnected)
        delete pnode;
        vNodes.clear();
        vNodesDisconnected.clear();
        vhListenSocket.clear();
        delete semOutbound;
        semOutbound = NULL;
        delete pnodeLocalHost;
        pnodeLocalHost = NULL;

=======
class CNetCleanup
{
public:
    CNetCleanup() {}

    ~CNetCleanup()
    {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#ifdef WIN32
        // Shutdown Windows Sockets
        WSACleanup();
#endif
    }
<<<<<<< HEAD
} instance_of_cnetcleanup;

void CExplicitNetCleanup::callCleanup() {
    // Explicit call to destructor of CNetCleanup because it's not implicitly called
    // when the wallet is restarted from within the wallet itself.
    CNetCleanup *tmp = new CNetCleanup();
    delete tmp; // Stroustrup's gonna kill me for that
}

void RelayTransaction(const CTransaction &tx) {
    CDataStream ss(SER_NETWORK, PROTOCOL_VERSION);
    ss.reserve(50000);
    ss << tx;
    RelayTransaction(tx, ss);
}

void RelayTransaction(const CTransaction &tx, const CDataStream &ss) {
    CInv inv(MSG_TX, tx.GetHash());
    {
        LOCK(cs_mapRelay);
        // Expire old relay messages
        while (!vRelayExpiration.empty() && vRelayExpiration.front().first < GetTime()) {
            mapRelay.erase(vRelayExpiration.front().second);
            vRelayExpiration.pop_front();
        }

        // Save original serialized message so newer versions are preserved
        mapRelay.insert(std::make_pair(inv, ss));
        vRelayExpiration.push_back(std::make_pair(GetTime() + 15 * 60, inv));
    }
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    {
        if (!pnode->fRelayTxes)
            continue;
        LOCK(pnode->cs_filter);
        if (pnode->pfilter) {
            if (pnode->pfilter->IsRelevantAndUpdate(tx))
                pnode->PushInventory(inv);
        } else
            pnode->PushInventory(inv);
    }
}

void RelayTransactionLockReq(const CTransaction &tx, bool relayToAll) {
    CInv inv(MSG_TXLOCK_REQUEST, tx.GetHash());

    //broadcast the new lock
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    {
        if (!relayToAll && !pnode->fRelayTxes)
            continue;

        pnode->PushMessage("ix", tx);
    }
}

void RelayInv(CInv &inv) {
    LOCK(cs_vNodes);
    for (CNode * pnode : vNodes)
    {
        if ((pnode->nServices == NODE_BLOOM_WITHOUT_MN) && inv.IsMasterNodeType())continue;
        if (pnode->nVersion >= ActiveProtocol())
            pnode->PushInventory(inv);
    }
}

void CNode::RecordBytesRecv(uint64_t bytes) {
    LOCK(cs_totalBytesRecv);
    nTotalBytesRecv += bytes;
}

void CNode::RecordBytesSent(uint64_t bytes) {
    LOCK(cs_totalBytesSent);
    nTotalBytesSent += bytes;
}

uint64_t CNode::GetTotalBytesRecv() {
    LOCK(cs_totalBytesRecv);
    return nTotalBytesRecv;
}

uint64_t CNode::GetTotalBytesSent() {
    LOCK(cs_totalBytesSent);
    return nTotalBytesSent;
}

void CNode::Fuzz(int nChance) {
    if (!fSuccessfullyConnected) return; // Don't fuzz initial handshake
    if (GetRand(nChance) != 0) return;   // Fuzz 1 of every nChance messages

    switch (GetRand(3)) {
        case 0:
            // xor a random byte with a random value:
            if (!ssSend.empty()) {
                CDataStream::size_type pos = GetRand(ssSend.size());
                ssSend[pos] ^= (unsigned char) (GetRand(256));
            }
            break;
        case 1:
            // delete a random byte:
            if (!ssSend.empty()) {
                CDataStream::size_type pos = GetRand(ssSend.size());
                ssSend.erase(ssSend.begin() + pos);
            }
            break;
        case 2:
            // insert a random byte at a random position
        {
            CDataStream::size_type pos = GetRand(ssSend.size());
            char ch = (char) GetRand(256);
            ssSend.insert(ssSend.begin() + pos, ch);
        }
            break;
    }
    // Chance of more than one change half the time:
    // (more changes exponentially less likely):
    Fuzz(2);
}

//
// CAddrDB
//

CAddrDB::CAddrDB() {
    pathAddr = GetDataDir() / "peers.dat";
}

bool CAddrDB::Write(const CAddrMan &addr) {
    // Generate random temporary filename
    unsigned short randv = 0;
    GetRandBytes((unsigned char *) &randv, sizeof(randv));
    std::string tmpfn = strprintf("peers.dat.%04x", randv);

    // serialize addresses, checksum data up to that point, then append csum
    CDataStream ssPeers(SER_DISK, CLIENT_VERSION);
    ssPeers << FLATDATA(Params().MessageStart());
    ssPeers << addr;
    uint256 hash = Hash(ssPeers.begin(), ssPeers.end());
    ssPeers << hash;

    // open output file, and associate with CAutoFile
    boost::filesystem::path pathAddr = GetDataDir() / "peers.dat";
    FILE *file = fopen(pathAddr.string().c_str(), "wb");
    CAutoFile fileout(file, SER_DISK, CLIENT_VERSION);
    if (fileout.IsNull())
        return error("%s : Failed to open file %s", __func__, pathAddr.string());

    // Write and commit header, data
    try {
        fileout << ssPeers;
    } catch (const std::exception& e) {
        return error("%s : Serialize or I/O error - %s", __func__, e.what());
    }
    FileCommit(fileout.Get());
    fileout.fclose();

    return true;
}

bool CAddrDB::Read(CAddrMan &addr) {
    // open input file, and associate with CAutoFile
    FILE *file = fopen(pathAddr.string().c_str(), "rb");
    CAutoFile filein(file, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull())
        return error("%s : Failed to open file %s", __func__, pathAddr.string());

    // use file size to size memory buffer
    uint64_t fileSize = boost::filesystem::file_size(pathAddr);
    uint64_t dataSize = fileSize - sizeof(uint256);
    // Don't try to resize to a negative number if file is small
    if (fileSize >= sizeof(uint256))
        dataSize = fileSize - sizeof(uint256);
    else
        return error("%s : Failed to open file %s", __func__, pathAddr.string());

    vector<unsigned char> vchData;
    vchData.resize(dataSize);
    uint256 hashIn;

    // read data and checksum from file
    try {
        filein.read((char *) &vchData[0], dataSize);
        filein >> hashIn;
    } catch (const std::exception& e) {
        return error("%s : Deserialize or I/O error - %s", __func__, e.what());
    }
    filein.fclose();

    CDataStream ssPeers(vchData, SER_DISK, CLIENT_VERSION);

    // verify stored checksum matches input data
    uint256 hashTmp = Hash(ssPeers.begin(), ssPeers.end());
    if (hashIn != hashTmp)
        return error("%s : Checksum mismatch, data corrupted", __func__);

    return Read(addr, ssPeers);
}

bool CAddrDB::Read(CAddrMan& addr, CDataStream& ssPeers)
{
    unsigned char pchMsgTmp[4];
    try {
        // de-serialize file header (network specific magic number) and ..
        ssPeers >> FLATDATA(pchMsgTmp);

        // ... verify the network matches ours
        if (memcmp(pchMsgTmp, Params().MessageStart(), sizeof(pchMsgTmp)))
            return error("%s : Invalid network magic number", __func__);

        // de-serialize address data into one CAddrMan object
        ssPeers >> addr;
    } catch (const std::exception& e) {
        // de-serialization has failed, ensure addrman is left in a clean state
        addr.Clear();
        return error("%s : Deserialize or I/O error - %s", __func__, e.what());
    }

    return true;
}

unsigned int ReceiveFloodSize() { return 1000 * GetArg("-maxreceivebuffer", 5 * 1000); }

unsigned int SendBufferSize() { return 1000 * GetArg("-maxsendbuffer", 1 * 1000); }

CNode::CNode(SOCKET hSocketIn, CAddress addrIn, std::string addrNameIn, bool fInboundIn) : ssSend(SER_NETWORK,
                                                                                                  INIT_PROTO_VERSION),
                                                                                           setAddrKnown(5000) {
    nServices = 0;
=======
}
instance_of_cnetcleanup;

void CExplicitNetCleanup::callCleanup()
{
    // Explicit call to destructor of CNetCleanup because it's not implicitly called
    // when the wallet is restarted from within the wallet itself.
    CNetCleanup* tmp = new CNetCleanup();
    delete tmp; // Stroustrup's gonna kill me for that
}

void CConnman::Interrupt()
{
    {
        std::lock_guard<std::mutex> lock(mutexMsgProc);
        flagInterruptMsgProc = true;
    }
    condMsgProc.notify_all();

    interruptNet();
    InterruptSocks5(true);

    if (semOutbound) {
        for (int i=0; i<(nMaxOutbound + nMaxFeeler); i++) {
            semOutbound->post();
        }
    }

    if (semAddnode) {
        for (int i=0; i<nMaxAddnode; i++) {
            semAddnode->post();
        }
    }
}

void CConnman::Stop()
{
    if (threadMessageHandler.joinable())
        threadMessageHandler.join();
    if (threadOpenConnections.joinable())
        threadOpenConnections.join();
    if (threadOpenAddedConnections.joinable())
        threadOpenAddedConnections.join();
    if (threadDNSAddressSeed.joinable())
        threadDNSAddressSeed.join();
    if (threadSocketHandler.joinable())
        threadSocketHandler.join();

    if (fAddressesInitialized)
    {
        DumpData();
        fAddressesInitialized = false;
    }

    // Close sockets
    for(CNode* pnode : vNodes)
        pnode->CloseSocketDisconnect();
    for(ListenSocket& hListenSocket : vhListenSocket)
        if (hListenSocket.socket != INVALID_SOCKET)
            if (!CloseSocket(hListenSocket.socket))
                LogPrintf("CloseSocket(hListenSocket) failed with error %s\n", NetworkErrorString(WSAGetLastError()));

    // clean up some globals (to help leak detection)
    for(CNode* pnode : vNodes) {
        DeleteNode(pnode);
    }
    for(CNode* pnode : vNodesDisconnected) {
        DeleteNode(pnode);
    }
    vNodes.clear();
    vNodesDisconnected.clear();
    vhListenSocket.clear();
    semOutbound.reset();
    semAddnode.reset();
}

void CConnman::DeleteNode(CNode* pnode)
{
    assert(pnode);
    bool fUpdateConnectionTime = false;
    m_msgproc->FinalizeNode(pnode->GetId(), fUpdateConnectionTime);
    if (fUpdateConnectionTime) {
        addrman.Connected(pnode->addr);
    }
    delete pnode;
}

CConnman::~CConnman()
{
    Interrupt();
    Stop();
}

void CConnman::SetServices(const CService &addr, ServiceFlags nServices)
{
    addrman.SetServices(addr, nServices);
}

void CConnman::MarkAddressGood(const CAddress& addr)
{
    addrman.Good(addr);
}

void CConnman::AddNewAddress(const CAddress& addr, const CAddress& addrFrom, int64_t nTimePenalty)
{
    addrman.Add(addr, addrFrom, nTimePenalty);
}

bool CConnman::AddNewAddresses(const std::vector<CAddress>& vAddr, const CAddress& addrFrom, int64_t nTimePenalty)
{
    return addrman.Add(vAddr, addrFrom, nTimePenalty);
}

std::vector<CAddress> CConnman::GetAddresses(size_t max_addresses, size_t max_pct, Optional<Network> network)
{
    return addrman.GetAddr(max_addresses, max_pct, network);
}

bool CConnman::AddNode(const std::string& strNode)
{
    LOCK(cs_vAddedNodes);
    for (const std::string& it : vAddedNodes) {
        if (strNode == it) return false;
    }

    vAddedNodes.push_back(strNode);
    return true;
}

bool CConnman::RemoveAddedNode(const std::string& strNode)
{
    LOCK(cs_vAddedNodes);
    for(std::vector<std::string>::iterator it = vAddedNodes.begin(); it != vAddedNodes.end(); ++it) {
        if (strNode == *it) {
            vAddedNodes.erase(it);
            return true;
        }
    }
    return false;
}

size_t CConnman::GetNodeCount(NumConnections flags)
{
    LOCK(cs_vNodes);
    if (flags == CConnman::CONNECTIONS_ALL) // Shortcut if we want total
        return vNodes.size();

    int nNum = 0;
    for (const auto& pnode : vNodes) {
        if (flags & (pnode->fInbound ? CONNECTIONS_IN : CONNECTIONS_OUT)) {
            nNum++;
        }
    }

    return nNum;
}

void CConnman::GetNodeStats(std::vector<CNodeStats>& vstats)
{
    vstats.clear();
    LOCK(cs_vNodes);
    vstats.reserve(vNodes.size());
    for (CNode* pnode : vNodes) {
        vstats.emplace_back();
        pnode->copyStats(vstats.back(), addrman.m_asmap);
    }
}

bool CConnman::DisconnectNode(const std::string& strNode)
{
    LOCK(cs_vNodes);
    if (CNode* pnode = FindNode(strNode)) {
        pnode->fDisconnect = true;
        return true;
    }
    return false;
}

bool CConnman::DisconnectNode(NodeId id)
{
    LOCK(cs_vNodes);
    for(CNode* pnode : vNodes) {
        if (id == pnode->GetId()) {
            pnode->fDisconnect = true;
            return true;
        }
    }
    return false;
}

void CConnman::RelayInv(CInv& inv)
{
    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes){
        if (!pnode->fSuccessfullyConnected) continue;
        if ((pnode->nServices == NODE_BLOOM_WITHOUT_MN) && inv.IsMasterNodeType()) continue;
        if (pnode->nVersion >= ActiveProtocol())
            pnode->PushInventory(inv);
    }
}

void CConnman::RemoveAskFor(const uint256& invHash, int invType)
{
    mapAlreadyAskedFor.erase(CInv(invType, invHash));

    LOCK(cs_vNodes);
    for (const auto& pnode : vNodes) {
        pnode->AskForInvReceived(invHash);
    }
}

void CConnman::RecordBytesRecv(uint64_t bytes)
{
    LOCK(cs_totalBytesRecv);
    nTotalBytesRecv += bytes;
}

void CConnman::RecordBytesSent(uint64_t bytes)
{
    LOCK(cs_totalBytesSent);
    nTotalBytesSent += bytes;
}

uint64_t CConnman::GetTotalBytesRecv()
{
    LOCK(cs_totalBytesRecv);
    return nTotalBytesRecv;
}

uint64_t CConnman::GetTotalBytesSent()
{
    LOCK(cs_totalBytesSent);
    return nTotalBytesSent;
}

ServiceFlags CConnman::GetLocalServices() const
{
    return nLocalServices;
}

void CConnman::SetBestHeight(int height)
{
    nBestHeight.store(height, std::memory_order_release);
}

int CConnman::GetBestHeight() const
{
    return nBestHeight.load(std::memory_order_acquire);
}

unsigned int CConnman::GetReceiveFloodSize() const { return nReceiveFloodSize; }
unsigned int CConnman::GetSendBufferSize() const{ return nSendBufferMaxSize; }

CNode::CNode(NodeId idIn, ServiceFlags nLocalServicesIn, int nMyStartingHeightIn, SOCKET hSocketIn, const CAddress& addrIn, uint64_t nKeyedNetGroupIn, uint64_t nLocalHostNonceIn, const std::string& addrNameIn, bool fInboundIn) :
    nTimeConnected(GetSystemTimeInSeconds()),
    addr(addrIn),
    fInbound(fInboundIn),
    nKeyedNetGroup(nKeyedNetGroupIn),
    addrKnown(5000, 0.001),
    filterInventoryKnown(50000, 0.000001),
    id(idIn),
    nLocalHostNonce(nLocalHostNonceIn),
    nLocalServices(nLocalServicesIn),
    nMyStartingHeight(nMyStartingHeightIn),
    nSendVersion(0)
{
    nServices = NODE_NONE;
    nServicesExpected = NODE_NONE;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    hSocket = hSocketIn;
    nRecvVersion = INIT_PROTO_VERSION;
    nLastSend = 0;
    nLastRecv = 0;
    nSendBytes = 0;
    nRecvBytes = 0;
<<<<<<< HEAD
    nTimeConnected = GetTime();
    nTimeOffset = 0;
    addr = addrIn;
=======
    nTimeOffset = 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    addrName = addrNameIn == "" ? addr.ToStringIPPort() : addrNameIn;
    nVersion = 0;
    strSubVer = "";
    fWhitelisted = false;
    fOneShot = false;
<<<<<<< HEAD
    fClient = false; // set by version message
    fInbound = fInboundIn;
    fNetworkNode = false;
=======
    fAddnode = false;
    fClient = false; // set by version message
    fFeeler = false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    fSuccessfullyConnected = false;
    fDisconnect = false;
    nRefCount = 0;
    nSendSize = 0;
    nSendOffset = 0;
<<<<<<< HEAD
    hashContinue = 0;
    nStartingHeight = -1;
    fGetAddr = false;
    fRelayTxes = false;
    setInventoryKnown.max_size(SendBufferSize() / 1000);
    pfilter = new CBloomFilter();
=======
    hashContinue = UINT256_ZERO;
    nStartingHeight = -1;
    filterInventoryKnown.reset();
    fSendMempool = false;
    fGetAddr = false;
    fRelayTxes = false;
    pfilter = std::make_unique<CBloomFilter>();
    timeLastMempoolReq = 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    nPingNonceSent = 0;
    nPingUsecStart = 0;
    nPingUsecTime = 0;
    fPingQueued = false;
<<<<<<< HEAD
    fObfuScationMaster = false;

    {
        LOCK(cs_nLastNodeId);
        id = nLastNodeId++;
    }

    if (fLogIPs)
        LogPrint("net", "Added connection to %s peer=%d\n", addrName, id);
    else
        LogPrint("net", "Added connection peer=%d\n", id);

    // Be shy and don't send version until we hear
    if (hSocket != INVALID_SOCKET && !fInbound)
        PushVersion();

    GetNodeSignals().InitializeNode(GetId(), this);
}

CNode::~CNode() {
    CloseSocket(hSocket);

    if (pfilter)
        delete pfilter;

    GetNodeSignals().FinalizeNode(GetId());
}

void CNode::AskFor(const CInv &inv, bool fImmediateRetry) {
    if (mapAskFor.size() > MAPASKFOR_MAX_SZ)
        return;
=======
    nMinPingUsecTime = std::numeric_limits<int64_t>::max();
    fPauseRecv = false;
    fPauseSend = false;
    nProcessQueueSize = 0;

    for (const std::string &msg : getAllNetMessageTypes())
        mapRecvBytesPerMsgCmd[msg] = 0;
    mapRecvBytesPerMsgCmd[NET_MESSAGE_COMMAND_OTHER] = 0;

    if (fLogIPs)
        LogPrint(BCLog::NET, "Added connection to %s peer=%d\n", addrName, id);
    else
        LogPrint(BCLog::NET, "Added connection peer=%d\n", id);
}

CNode::~CNode()
{
    CloseSocket(hSocket);
}

void CNode::AskFor(const CInv& inv)
{
    if (mapAskFor.size() > MAPASKFOR_MAX_SZ || setAskFor.size() > SETASKFOR_MAX_SZ)
        return;
    // a peer may not have multiple non-responded queue positions for a single inv item
    if (!setAskFor.insert(inv.hash).second)
        return;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // We're using mapAskFor as a priority queue,
    // the key is the earliest time the request can be sent
    int64_t nRequestTime;
    limitedmap<CInv, int64_t>::const_iterator it = mapAlreadyAskedFor.find(inv);
    if (it != mapAlreadyAskedFor.end())
        nRequestTime = it->second;
    else
        nRequestTime = 0;
<<<<<<< HEAD
    LogPrint("net", "askfor %s  %d (%s) peer=%d\n", inv.ToString(), nRequestTime,
             DateTimeStrFormat("%H:%M:%S", nRequestTime / 1000000), id);
    //LogPrintf("askfor %s  %d (%s) peer=%d\n", inv.ToString(), nRequestTime,
             //DateTimeStrFormat("%H:%M:%S", nRequestTime / 1000000), id);
=======
    LogPrint(BCLog::NET, "askfor %s  %d (%s) peer=%d\n", inv.ToString(), nRequestTime, FormatISO8601Time(nRequestTime / 1000000), id);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Make sure not to reuse time indexes to keep things in the same order
    int64_t nNow = GetTimeMicros() - 1000000;
    static int64_t nLastTime;
    ++nLastTime;
    nNow = std::max(nNow, nLastTime);
    nLastTime = nNow;

    // Each retry is 2 minutes after the last
    nRequestTime = std::max(nRequestTime + 2 * 60 * 1000000, nNow);
<<<<<<< HEAD
    if (fImmediateRetry)
        nRequestTime = nNow;
    else
        nRequestTime = std::max(nRequestTime + 2 * 60 * 1000000, nNow);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (it != mapAlreadyAskedFor.end())
        mapAlreadyAskedFor.update(it, nRequestTime);
    else
        mapAlreadyAskedFor.insert(std::make_pair(inv, nRequestTime));
    mapAskFor.insert(std::make_pair(nRequestTime, inv));
}

<<<<<<< HEAD
void CNode::BeginMessage(const char *pszCommand) EXCLUSIVE_LOCK_FUNCTION(cs_vSend) {
    ENTER_CRITICAL_SECTION(cs_vSend);
    assert(ssSend.size() == 0);
    ssSend << CMessageHeader(pszCommand, 0);
    LogPrint("net", "sending: %s ", SanitizeString(pszCommand));
}

void CNode::AbortMessage() UNLOCK_FUNCTION(cs_vSend) {
    ssSend.clear();

    LEAVE_CRITICAL_SECTION(cs_vSend);

    LogPrint("net", "(aborted)\n");
}

void CNode::EndMessage() UNLOCK_FUNCTION(cs_vSend) {
    // The -*messagestest options are intentionally not documented in the help message,
    // since they are only used during development to debug the networking code and are
    // not intended for end-users.
    if (mapArgs.count("-dropmessagestest") && GetRand(GetArg("-dropmessagestest", 2)) == 0) {
        LogPrint("net", "dropmessages DROPPING SEND MESSAGE\n");
        AbortMessage();
        return;
    }
    if (mapArgs.count("-fuzzmessagestest"))
        Fuzz(GetArg("-fuzzmessagestest", 10));

    if (ssSend.size() == 0) {
        LEAVE_CRITICAL_SECTION(cs_vSend);
        return;
    }

    // Set the size
    unsigned int nSize = ssSend.size() - CMessageHeader::HEADER_SIZE;
    memcpy((char *) &ssSend[CMessageHeader::MESSAGE_SIZE_OFFSET], &nSize, sizeof(nSize));

    // Set the checksum
    uint256 hash = Hash(ssSend.begin() + CMessageHeader::HEADER_SIZE, ssSend.end());
    unsigned int nChecksum = 0;
    memcpy(&nChecksum, &hash, sizeof(nChecksum));
    assert(ssSend.size() >= CMessageHeader::CHECKSUM_OFFSET + sizeof(nChecksum));
    memcpy((char *) &ssSend[CMessageHeader::CHECKSUM_OFFSET], &nChecksum, sizeof(nChecksum));

    LogPrint("net", "(%d bytes) peer=%d\n", nSize, id);

    std::deque<CSerializeData>::iterator it = vSendMsg.insert(vSendMsg.end(), CSerializeData());
    ssSend.GetAndClear(*it);
    nSendSize += (*it).size();

    // If write queue empty, attempt "optimistic write"
    if (it == vSendMsg.begin())
        SocketSendData(this);

    LEAVE_CRITICAL_SECTION(cs_vSend);
}

//
// CBanDB
//

CBanDB::CBanDB() {
    pathBanlist = GetDataDir() / "banlist.dat";
}

bool CBanDB::Write(const banmap_t &banSet) {
    // Generate random temporary filename
    unsigned short randv = 0;
    GetRandBytes((unsigned char *) &randv, sizeof(randv));
    std::string tmpfn = strprintf("banlist.dat.%04x", randv);
    // serialize banlist, checksum data up to that point, then append csum
    CDataStream ssBanlist(SER_DISK, CLIENT_VERSION);
    ssBanlist << FLATDATA(Params().MessageStart());
    ssBanlist << banSet;
    uint256 hash = Hash(ssBanlist.begin(), ssBanlist.end());
    ssBanlist << hash;
    // open temp output file, and associate with CAutoFile
    boost::filesystem::path pathTmp = GetDataDir() / tmpfn;
    FILE *file = fopen(pathTmp.string().c_str(), "wb");
    CAutoFile fileout(file, SER_DISK, CLIENT_VERSION);
    if (fileout.IsNull())
        return error("%s: Failed to open file %s", __func__, pathTmp.string());
    // Write and commit header, data
    try {
        fileout << ssBanlist;
    }
    catch (const std::exception& e) {
        return error("%s: Serialize or I/O error - %s", __func__, e.what());
    }
    FileCommit(fileout.Get());
    fileout.fclose();
    // replace existing banlist.dat, if any, with new banlist.dat.XXXX
    if (!RenameOver(pathTmp, pathBanlist))
        return error("%s: Rename-into-place failed", __func__);
    return true;
}

bool CBanDB::Read(banmap_t &banSet) {
    // open input file, and associate with CAutoFile
    FILE *file = fopen(pathBanlist.string().c_str(), "rb");
    CAutoFile filein(file, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull())
        return error("%s: Failed to open file %s", __func__, pathBanlist.string());
    // use file size to size memory buffer
    uint64_t fileSize = boost::filesystem::file_size(pathBanlist);
    uint64_t dataSize = 0;
    // Don't try to resize to a negative number if file is small
    if (fileSize >= sizeof(uint256))
        dataSize = fileSize - sizeof(uint256);
    vector<unsigned char> vchData;
    vchData.resize(dataSize);
    uint256 hashIn;
    // read data and checksum from file
    try {
        filein.read((char *) &vchData[0], dataSize);
        filein >> hashIn;
    }
    catch (const std::exception& e) {
        return error("%s: Deserialize or I/O error - %s", __func__, e.what());
    }
    filein.fclose();
    CDataStream ssBanlist(vchData, SER_DISK, CLIENT_VERSION);
    // verify stored checksum matches input data
    uint256 hashTmp = Hash(ssBanlist.begin(), ssBanlist.end());
    if (hashIn != hashTmp)
        return error("%s: Checksum mismatch, data corrupted", __func__);
    unsigned char pchMsgTmp[4];
    try {
        // de-serialize file header (network specific magic number) and ..
        ssBanlist >> FLATDATA(pchMsgTmp);
        // ... verify the network matches ours
        if (memcmp(pchMsgTmp, Params().MessageStart(), sizeof(pchMsgTmp)))
            return error("%s: Invalid network magic number", __func__);
        // de-serialize address data into one CAddrMan object
        ssBanlist >> banSet;
    }
    catch (const std::exception& e) {
        return error("%s: Deserialize or I/O error - %s", __func__, e.what());
    }
    return true;
}

void DumpBanlist() {
    CNode::SweepBanned(); //clean unused entires (if bantime has expired)

    if (!CNode::BannedSetIsDirty())
        return;
    int64_t nStart = GetTimeMillis();

    CBanDB bandb;
    banmap_t banmap;
    CNode::GetBanned(banmap);
    if (bandb.Write(banmap))
        CNode::SetBannedSetDirty(false);

    LogPrint("net", "Flushed %d banned node ips/subnets to banlist.dat  %dms\n",
             banmap.size(), GetTimeMillis() - nStart);
}
=======
void CNode::AskForInvReceived(const uint256& invHash)
{
    setAskFor.erase(invHash);
    for (auto it = mapAskFor.begin(); it != mapAskFor.end();) {
        if (it->second.hash == invHash) {
            it = mapAskFor.erase(it);
        } else {
            ++it;
        }
    }
}

bool CConnman::NodeFullyConnected(const CNode* pnode)
{
    return pnode && pnode->fSuccessfullyConnected && !pnode->fDisconnect;
}

void CConnman::PushMessage(CNode* pnode, CSerializedNetMsg&& msg)
{
    size_t nMessageSize = msg.data.size();
    size_t nTotalSize = nMessageSize + CMessageHeader::HEADER_SIZE;
    LogPrint(BCLog::NET, "sending %s (%d bytes) peer=%d\n",  SanitizeString(msg.command.c_str()), nMessageSize, pnode->GetId());

    std::vector<unsigned char> serializedHeader;
    serializedHeader.reserve(CMessageHeader::HEADER_SIZE);
    uint256 hash = Hash(msg.data.data(), msg.data.data() + nMessageSize);
    CMessageHeader hdr(Params().MessageStart(), msg.command.c_str(), nMessageSize);
    memcpy(hdr.pchChecksum, hash.begin(), CMessageHeader::CHECKSUM_SIZE);

    CVectorWriter{SER_NETWORK, INIT_PROTO_VERSION, serializedHeader, 0, hdr};

    size_t nBytesSent = 0;
    {
        LOCK(pnode->cs_vSend);
        bool optimisticSend(pnode->vSendMsg.empty());

        //log total amount of bytes per command
        pnode->mapSendBytesPerMsgCmd[msg.command] += nTotalSize;
        pnode->nSendSize += nTotalSize;

        if (pnode->nSendSize > nSendBufferMaxSize)
            pnode->fPauseSend = true;
        pnode->vSendMsg.push_back(std::move(serializedHeader));
        if (nMessageSize)
            pnode->vSendMsg.push_back(std::move(msg.data));

        // If write queue empty, attempt "optimistic write"
        if (optimisticSend == true)
            nBytesSent = SocketSendData(pnode);
    }
    if (nBytesSent)
        RecordBytesSent(nBytesSent);
}

bool CConnman::ForNode(NodeId id, std::function<bool(CNode* pnode)> func)
{
    CNode* found = nullptr;
    LOCK(cs_vNodes);
    for (auto&& pnode : vNodes) {
        if(pnode->GetId() == id) {
            found = pnode;
            break;
        }
    }
    return found != nullptr && NodeFullyConnected(found) && func(found);
}

bool CConnman::IsNodeConnected(const CAddress& addr)
{
    return FindNode(addr.ToStringIPPort());
}

CNode* CConnman::ConnectNode(CAddress addrConnect)
{
    return ConnectNode(addrConnect, nullptr, true);
}

// valid, reachable and routable address (except for RegTest)
bool validateMasternodeIP(const std::string& addrStr)
{
    CNetAddr resolved;
    if (LookupHost(addrStr, resolved, false)) {
        return ((IsReachable(resolved) && resolved.IsRoutable()) ||
                (Params().IsRegTestNet() && resolved.IsValid()));
    }
    return false;
}

int64_t PoissonNextSend(int64_t nNow, int average_interval_seconds) {
    return nNow + (int64_t)(log1p(GetRand(1ULL << 48) * -0.0000000000000035527136788 /* -1/2^48 */) * average_interval_seconds * -1000000.0 + 0.5);
}

CSipHasher CConnman::GetDeterministicRandomizer(uint64_t id)
{
    return CSipHasher(nSeed0, nSeed1).Write(id);
}

uint64_t CConnman::CalculateKeyedNetGroup(const CAddress& ad)
{
    std::vector<unsigned char> vchNetGroup(ad.GetGroup(addrman.m_asmap));

    return GetDeterministicRandomizer(RANDOMIZER_ID_NETGROUP).Write(vchNetGroup.data(), vchNetGroup.size()).Finalize();
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
