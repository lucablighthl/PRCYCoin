// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifdef HAVE_CONFIG_H
#include "config/prcycoin-config.h"
#endif

#include "netbase.h"

#include "hash.h"
#include "sync.h"
#include "uint256.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <atomic>

#ifdef HAVE_GETADDRINFO_A
#include <netdb.h>
#endif

#ifndef WIN32
#if HAVE_INET_PTON
#include <arpa/inet.h>
#endif
#include <fcntl.h>
#endif

#include <boost/algorithm/string/case_conv.hpp> // for to_lower()
#include <boost/algorithm/string/predicate.hpp> // for startswith() and endswith()
#include <boost/thread.hpp>

=======
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include "netbase.h"

#include "sync.h"
#include "random.h"
#include "util/string.h"
#include "util/system.h"
#include "utilstrencodings.h"

#include <atomic>
#include <cstdint>
#include <limits>

#ifndef WIN32
#include <fcntl.h>
#else
#include <codecvt>
#endif

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#if !defined(HAVE_MSG_NOSIGNAL) && !defined(MSG_NOSIGNAL)
#define MSG_NOSIGNAL 0
#endif

<<<<<<< HEAD
using namespace std;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

// Settings
static proxyType proxyInfo[NET_MAX];
static proxyType nameProxy;
static RecursiveMutex cs_proxyInfos;
int nConnectTimeout = DEFAULT_CONNECT_TIMEOUT;
bool fNameLookup = false;

<<<<<<< HEAD
static const unsigned char pchIPv4[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff};

// Need ample time for negotiation for very slow proxies such as Tor (milliseconds)
static const int SOCKS5_RECV_TIMEOUT = 20 * 1000;

enum Network ParseNetwork(std::string net)
{
    boost::to_lower(net);
    if (net == "ipv4") return NET_IPV4;
    if (net == "ipv6") return NET_IPV6;
    if (net == "tor" || net == "onion") return NET_TOR;
=======
// Need ample time for negotiation for very slow proxies such as Tor (milliseconds)
static const int SOCKS5_RECV_TIMEOUT = 20 * 1000;
static std::atomic<bool> interruptSocks5Recv(false);

enum Network ParseNetwork(std::string net)
{
    Downcase(net);
    if (net == "ipv4") return NET_IPV4;
    if (net == "ipv6") return NET_IPV6;
    if (net == "tor" || net == "onion") return NET_ONION;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return NET_UNROUTABLE;
}

std::string GetNetworkName(enum Network net)
{
    switch (net) {
<<<<<<< HEAD
    case NET_IPV4:
        return "ipv4";
    case NET_IPV6:
        return "ipv6";
    case NET_TOR:
        return "onion";
    default:
        return "";
    }
=======
        case NET_UNROUTABLE: return "unroutable";
        case NET_IPV4: return "ipv4";
        case NET_IPV6: return "ipv6";
        case NET_ONION: return "onion";
        case NET_I2P: return "i2p";
        case NET_CJDNS: return "cjdns";
        case NET_INTERNAL: return "internal";
        case NET_MAX: assert(false);
    } // no default case, so the compiler can warn about missing cases

    assert(false);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void SplitHostPort(std::string in, int& portOut, std::string& hostOut)
{
    size_t colon = in.find_last_of(':');
    // if a : is found, and it either follows a [...], or no other : is in the string, treat it as port separator
    bool fHaveColon = colon != in.npos;
    bool fBracketed = fHaveColon && (in[0] == '[' && in[colon - 1] == ']'); // if there is a colon, and in[0]=='[', colon is not 0, so in[colon-1] is safe
    bool fMultiColon = fHaveColon && (in.find_last_of(':', colon - 1) != in.npos);
    if (fHaveColon && (colon == 0 || fBracketed || !fMultiColon)) {
        int32_t n;
        if (ParseInt32(in.substr(colon + 1), &n) && n > 0 && n < 0x10000) {
            in = in.substr(0, colon);
            portOut = n;
        }
    }
    if (in.size() > 0 && in[0] == '[' && in[in.size() - 1] == ']')
        hostOut = in.substr(1, in.size() - 2);
    else
        hostOut = in;
}

<<<<<<< HEAD
bool static LookupIntern(const char* pszName, std::vector<CNetAddr>& vIP, unsigned int nMaxSolutions, bool fAllowLookup)
{
    vIP.clear();

    {
        CNetAddr addr;
        if (addr.SetSpecial(std::string(pszName))) {
=======
bool static LookupIntern(const std::string& name, std::vector<CNetAddr>& vIP, unsigned int nMaxSolutions, bool fAllowLookup)
{
    vIP.clear();

    if (!ValidAsCString(name)) {
        return false;
    }

    {
        CNetAddr addr;
        if (addr.SetSpecial(name)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            vIP.push_back(addr);
            return true;
        }
    }

<<<<<<< HEAD
#ifdef HAVE_GETADDRINFO_A
    struct in_addr ipv4_addr;
#ifdef HAVE_INET_PTON
    if (inet_pton(AF_INET, pszName, &ipv4_addr) > 0) {
        vIP.push_back(CNetAddr(ipv4_addr));
        return true;
    }

    struct in6_addr ipv6_addr;
    if (inet_pton(AF_INET6, pszName, &ipv6_addr) > 0) {
        vIP.push_back(CNetAddr(ipv6_addr));
        return true;
    }
#else
    ipv4_addr.s_addr = inet_addr(pszName);
    if (ipv4_addr.s_addr != INADDR_NONE) {
        vIP.push_back(CNetAddr(ipv4_addr));
        return true;
    }
#endif
#endif

    struct addrinfo aiHint;
    memset(&aiHint, 0, sizeof(struct addrinfo));
=======
    struct addrinfo aiHint;
    memset(&aiHint, 0, sizeof(struct addrinfo));

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    aiHint.ai_socktype = SOCK_STREAM;
    aiHint.ai_protocol = IPPROTO_TCP;
    aiHint.ai_family = AF_UNSPEC;
#ifdef WIN32
    aiHint.ai_flags = fAllowLookup ? 0 : AI_NUMERICHOST;
#else
    aiHint.ai_flags = fAllowLookup ? AI_ADDRCONFIG : AI_NUMERICHOST;
#endif
<<<<<<< HEAD

    struct addrinfo* aiRes = NULL;
#ifdef HAVE_GETADDRINFO_A
    struct gaicb gcb, *query = &gcb;
    memset(query, 0, sizeof(struct gaicb));
    gcb.ar_name = pszName;
    gcb.ar_request = &aiHint;
    int nErr = getaddrinfo_a(GAI_NOWAIT, &query, 1, NULL);
    if (nErr)
        return false;

    do {
        // Should set the timeout limit to a resonable value to avoid
        // generating unnecessary checking call during the polling loop,
        // while it can still response to stop request quick enough.
        // 2 seconds looks fine in our situation.
        struct timespec ts = {2, 0};
        gai_suspend(&query, 1, &ts);
        boost::this_thread::interruption_point();

        nErr = gai_error(query);
        if (0 == nErr)
            aiRes = query->ar_result;
    } while (nErr == EAI_INPROGRESS);
#else
    int nErr = getaddrinfo(pszName, NULL, &aiHint, &aiRes);
#endif
=======
    struct addrinfo* aiRes = NULL;
    int nErr = getaddrinfo(name.c_str(), NULL, &aiHint, &aiRes);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (nErr)
        return false;

    struct addrinfo* aiTrav = aiRes;
    while (aiTrav != NULL && (nMaxSolutions == 0 || vIP.size() < nMaxSolutions)) {
<<<<<<< HEAD
        if (aiTrav->ai_family == AF_INET) {
            assert(aiTrav->ai_addrlen >= sizeof(sockaddr_in));
            vIP.push_back(CNetAddr(((struct sockaddr_in*)(aiTrav->ai_addr))->sin_addr));
=======
        CNetAddr resolved;
        if (aiTrav->ai_family == AF_INET) {
            assert(aiTrav->ai_addrlen >= sizeof(sockaddr_in));
            resolved = CNetAddr(((struct sockaddr_in*)(aiTrav->ai_addr))->sin_addr);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        if (aiTrav->ai_family == AF_INET6) {
            assert(aiTrav->ai_addrlen >= sizeof(sockaddr_in6));
<<<<<<< HEAD
            vIP.push_back(CNetAddr(((struct sockaddr_in6*)(aiTrav->ai_addr))->sin6_addr));
=======
            struct sockaddr_in6* s6 = (struct sockaddr_in6*) aiTrav->ai_addr;
            resolved = CNetAddr(s6->sin6_addr, s6->sin6_scope_id);
        }
        /* Never allow resolving to an internal address. Consider any such result invalid */
        if (!resolved.IsInternal()) {
            vIP.push_back(resolved);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        aiTrav = aiTrav->ai_next;
    }

    freeaddrinfo(aiRes);

    return (vIP.size() > 0);
}

<<<<<<< HEAD
bool LookupHost(const char* pszName, std::vector<CNetAddr>& vIP, unsigned int nMaxSolutions, bool fAllowLookup)
{
    std::string strHost(pszName);
    if (strHost.empty())
        return false;
    if (boost::algorithm::starts_with(strHost, "[") && boost::algorithm::ends_with(strHost, "]")) {
        strHost = strHost.substr(1, strHost.size() - 2);
    }

    return LookupIntern(strHost.c_str(), vIP, nMaxSolutions, fAllowLookup);
}

bool Lookup(const char* pszName, std::vector<CService>& vAddr, int portDefault, bool fAllowLookup, unsigned int nMaxSolutions)
{
    if (pszName[0] == 0)
        return false;
    int port = portDefault;
    std::string hostname = "";
    SplitHostPort(std::string(pszName), port, hostname);

    std::vector<CNetAddr> vIP;
    bool fRet = LookupIntern(hostname.c_str(), vIP, nMaxSolutions, fAllowLookup);
=======
bool LookupHost(const std::string& name, std::vector<CNetAddr>& vIP, unsigned int nMaxSolutions, bool fAllowLookup)
{
    if (!ValidAsCString(name)) {
        return false;
    }
    std::string strHost = name;
    if (strHost.empty())
        return false;
    if (strHost.front() == '[' && strHost.back() == ']') {
        strHost = strHost.substr(1, strHost.size() - 2);
    }

    return LookupIntern(strHost, vIP, nMaxSolutions, fAllowLookup);
}

bool LookupHost(const std::string& name, CNetAddr& addr, bool fAllowLookup)
{
    if (!ValidAsCString(name)) {
        return false;
    }
    std::vector<CNetAddr> vIP;
    LookupHost(name, vIP, 1, fAllowLookup);
    if (vIP.empty())
        return false;
    addr = vIP.front();
    return true;
}

bool Lookup(const std::string& name, std::vector<CService>& vAddr, int portDefault, bool fAllowLookup, unsigned int nMaxSolutions)
{
    if (name.empty() || !ValidAsCString(name)) {
        return false;
    }
    int port = portDefault;
    std::string hostname;
    SplitHostPort(name, port, hostname);

    std::vector<CNetAddr> vIP;
    bool fRet = LookupIntern(hostname, vIP, nMaxSolutions, fAllowLookup);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fRet)
        return false;
    vAddr.resize(vIP.size());
    for (unsigned int i = 0; i < vIP.size(); i++)
        vAddr[i] = CService(vIP[i], port);
    return true;
}

<<<<<<< HEAD
bool Lookup(const char* pszName, CService& addr, int portDefault, bool fAllowLookup)
{
    std::vector<CService> vService;
    bool fRet = Lookup(pszName, vService, portDefault, fAllowLookup, 1);
=======
bool Lookup(const std::string& name, CService& addr, int portDefault, bool fAllowLookup)
{
    if (!ValidAsCString(name)) {
        return false;
    }
    std::vector<CService> vService;
    bool fRet = Lookup(name, vService, portDefault, fAllowLookup, 1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!fRet)
        return false;
    addr = vService[0];
    return true;
}

<<<<<<< HEAD
bool LookupNumeric(const char* pszName, CService& addr, int portDefault)
{
    return Lookup(pszName, addr, portDefault, false);
=======
CService LookupNumeric(const std::string& name, int portDefault)
{
    if (!ValidAsCString(name)) {
        return {};
    }
    CService addr;
    // "1.2:345" will fail to resolve the ip, but will still set the port.
    // If the ip fails to resolve, re-init the result.
    if (!Lookup(name, addr, portDefault, false))
        addr = CService();
    return addr;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

struct timeval MillisToTimeval(int64_t nTimeout)
{
    struct timeval timeout;
    timeout.tv_sec = nTimeout / 1000;
    timeout.tv_usec = (nTimeout % 1000) * 1000;
    return timeout;
}

<<<<<<< HEAD
=======
/** SOCKS version */
enum SOCKSVersion: uint8_t {
    SOCKS4 = 0x04,
    SOCKS5 = 0x05
};

/** Values defined for METHOD in RFC1928 */
enum SOCKS5Method: uint8_t {
    NOAUTH = 0x00,        //! No authentication required
    GSSAPI = 0x01,        //! GSSAPI
    USER_PASS = 0x02,     //! Username/password
    NO_ACCEPTABLE = 0xff, //! No acceptable methods
};

/** Values defined for CMD in RFC1928 */
enum SOCKS5Command: uint8_t {
    CONNECT = 0x01,
    BIND = 0x02,
    UDP_ASSOCIATE = 0x03
};

/** Values defined for REP in RFC1928 */
enum SOCKS5Reply: uint8_t {
    SUCCEEDED = 0x00,        //! Succeeded
    GENFAILURE = 0x01,       //! General failure
    NOTALLOWED = 0x02,       //! Connection not allowed by ruleset
    NETUNREACHABLE = 0x03,   //! Network unreachable
    HOSTUNREACHABLE = 0x04,  //! Network unreachable
    CONNREFUSED = 0x05,      //! Connection refused
    TTLEXPIRED = 0x06,       //! TTL expired
    CMDUNSUPPORTED = 0x07,   //! Command not supported
    ATYPEUNSUPPORTED = 0x08, //! Address type not supported
};

/** Values defined for ATYPE in RFC1928 */
enum SOCKS5Atyp: uint8_t {
    IPV4 = 0x01,
    DOMAINNAME = 0x03,
    IPV6 = 0x04,
};

/** Status codes that can be returned by InterruptibleRecv */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
enum class IntrRecvError {
    OK,
    Timeout,
    Disconnected,
    NetworkError,
    Interrupted
};

/**
 * Read bytes from socket. This will either read the full number of bytes requested
 * or return False on error or timeout.
<<<<<<< HEAD
 * This function can be interrupted by boost thread interrupt.
=======
 * This function can be interrupted by calling InterruptSocks5()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *
 * @param data Buffer to receive into
 * @param len  Length of data to receive
 * @param timeout  Timeout in milliseconds for receive operation
 *
 * @note This function requires that hSocket is in non-blocking mode.
 */
<<<<<<< HEAD
static IntrRecvError InterruptibleRecv(char* data, size_t len, int timeout, SOCKET& hSocket)
=======
static IntrRecvError InterruptibleRecv(uint8_t* data, size_t len, int timeout, const SOCKET& hSocket)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    int64_t curTime = GetTimeMillis();
    int64_t endTime = curTime + timeout;
    // Maximum time to wait in one select call. It will take up until this time (in millis)
    // to break off in case of an interruption.
    const int64_t maxWait = 1000;
    while (len > 0 && curTime < endTime) {
<<<<<<< HEAD
        ssize_t ret = recv(hSocket, data, len, 0); // Optimistically try the recv first
=======
        ssize_t ret = recv(hSocket, (char*)data, len, 0); // Optimistically try the recv first
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (ret > 0) {
            len -= ret;
            data += ret;
        } else if (ret == 0) { // Unexpected disconnection
            return IntrRecvError::Disconnected;
        } else { // Other error or blocking
            int nErr = WSAGetLastError();
            if (nErr == WSAEINPROGRESS || nErr == WSAEWOULDBLOCK || nErr == WSAEINVAL) {
                if (!IsSelectableSocket(hSocket)) {
                    return IntrRecvError::NetworkError;
                }
                struct timeval tval = MillisToTimeval(std::min(endTime - curTime, maxWait));
                fd_set fdset;
                FD_ZERO(&fdset);
                FD_SET(hSocket, &fdset);
                int nRet = select(hSocket + 1, &fdset, NULL, NULL, &tval);
                if (nRet == SOCKET_ERROR) {
                    return IntrRecvError::NetworkError;
                }
            } else {
                return IntrRecvError::NetworkError;
            }
        }
<<<<<<< HEAD
        boost::this_thread::interruption_point();
=======
        if (interruptSocks5Recv)
            return IntrRecvError::Interrupted;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        curTime = GetTimeMillis();
    }
    return len == 0 ? IntrRecvError::OK : IntrRecvError::Timeout;
}

<<<<<<< HEAD
=======
/** Credentials for proxy authentication */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
struct ProxyCredentials
{
    std::string username;
    std::string password;
};

<<<<<<< HEAD
/** Connect using SOCKS5 (as described in RFC1928) */
bool static Socks5(string strDest, int port, const ProxyCredentials *auth, SOCKET& hSocket)
{
    IntrRecvError recvr;
    LogPrintf("SOCKS5 connecting %s\n", strDest);
    if (strDest.size() > 255) {
        CloseSocket(hSocket);
=======
/** Convert SOCKS5 reply to a an error message */
static std::string Socks5ErrorString(uint8_t err)
{
    switch(err) {
        case SOCKS5Reply::GENFAILURE:
            return "general failure";
        case SOCKS5Reply::NOTALLOWED:
            return "connection not allowed";
        case SOCKS5Reply::NETUNREACHABLE:
            return "network unreachable";
        case SOCKS5Reply::HOSTUNREACHABLE:
            return "host unreachable";
        case SOCKS5Reply::CONNREFUSED:
            return "connection refused";
        case SOCKS5Reply::TTLEXPIRED:
            return "TTL expired";
        case SOCKS5Reply::CMDUNSUPPORTED:
            return "protocol error";
        case SOCKS5Reply::ATYPEUNSUPPORTED:
            return "address type not supported";
        default:
            return "unknown";
    }
}

/** Connect using SOCKS5 (as described in RFC1928) */
bool static Socks5(std::string strDest, int port, const ProxyCredentials *auth, const SOCKET& hSocket)
{
    IntrRecvError recvr;
    LogPrint(BCLog::NET, "SOCKS5 connecting %s\n", strDest);
    if (strDest.size() > 255) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return error("Hostname too long");
    }
    // Accepted authentication methods
    std::vector<uint8_t> vSocks5Init;
<<<<<<< HEAD
    vSocks5Init.push_back(0x05);
    if (auth) {
        vSocks5Init.push_back(0x02); // # METHODS
        vSocks5Init.push_back(0x00); // X'00' NO AUTHENTICATION REQUIRED
        vSocks5Init.push_back(0x02); // X'02' USERNAME/PASSWORD (RFC1929)
    } else {
        vSocks5Init.push_back(0x01); // # METHODS
        vSocks5Init.push_back(0x00); // X'00' NO AUTHENTICATION REQUIRED
    }
    ssize_t ret = send(hSocket, (const char*)vSocks5Init.data(), vSocks5Init.size(), MSG_NOSIGNAL);
    if (ret != (ssize_t)vSocks5Init.size()) {
        CloseSocket(hSocket);
        return error("Error sending to proxy");
    }
    char pchRet1[2];
    if ((recvr = InterruptibleRecv(pchRet1, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
        CloseSocket(hSocket);
        return error("Error reading proxy response");
    }
    if (pchRet1[0] != 0x05) {
        CloseSocket(hSocket);
        return error("Proxy failed to initialize");
    }
    if (pchRet1[1] == 0x02 && auth) {
        // Perform username/password authentication (as described in RFC1929)
        std::vector<uint8_t> vAuth;
        vAuth.push_back(0x01);
=======
    vSocks5Init.push_back(SOCKSVersion::SOCKS5);
    if (auth) {
        vSocks5Init.push_back(0x02); // Number of methods
        vSocks5Init.push_back(SOCKS5Method::NOAUTH);
        vSocks5Init.push_back(SOCKS5Method::USER_PASS);
    } else {
        vSocks5Init.push_back(0x01); // Number of methods
        vSocks5Init.push_back(SOCKS5Method::NOAUTH);
    }
    ssize_t ret = send(hSocket, (const char*)vSocks5Init.data(), vSocks5Init.size(), MSG_NOSIGNAL);
    if (ret != (ssize_t)vSocks5Init.size()) {
        return error("Error sending to proxy");
    }
    uint8_t pchRet1[2];
    if ((recvr = InterruptibleRecv(pchRet1, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
        LogPrintf("Socks5() connect to %s:%d failed: InterruptibleRecv() timeout or other failure\n", strDest, port);
        return false;
    }
    if (pchRet1[0] != SOCKSVersion::SOCKS5) {
        return error("Proxy failed to initialize");
    }
    if (pchRet1[1] == SOCKS5Method::USER_PASS && auth) {
        // Perform username/password authentication (as described in RFC1929)
        std::vector<uint8_t> vAuth;
        vAuth.push_back(0x01); // Current (and only) version of user/pass subnegotiation
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (auth->username.size() > 255 || auth->password.size() > 255)
            return error("Proxy username or password too long");
        vAuth.push_back(auth->username.size());
        vAuth.insert(vAuth.end(), auth->username.begin(), auth->username.end());
        vAuth.push_back(auth->password.size());
        vAuth.insert(vAuth.end(), auth->password.begin(), auth->password.end());
        ret = send(hSocket, (const char*)vAuth.data(), vAuth.size(), MSG_NOSIGNAL);
        if (ret != (ssize_t)vAuth.size()) {
<<<<<<< HEAD
            CloseSocket(hSocket);
            return error("Error sending authentication to proxy");
        }
        LogPrint("proxy", "SOCKS5 sending proxy authentication %s:%s\n", auth->username, auth->password);
        char pchRetA[2];
        if ((recvr = InterruptibleRecv(pchRetA, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
            CloseSocket(hSocket);
            return error("Error reading proxy authentication response");
        }
        if (pchRetA[0] != 0x01 || pchRetA[1] != 0x00) {
            CloseSocket(hSocket);
            return error("Proxy authentication unsuccesful");
        }
    } else if (pchRet1[1] == 0x00) {
        // Perform no authentication
    } else {
        CloseSocket(hSocket);
        return error("Proxy requested wrong authentication method %02x", pchRet1[1]);
    }
    std::vector<uint8_t> vSocks5;
    vSocks5.push_back(0x05); // VER protocol version
    vSocks5.push_back(0x01); // CMD CONNECT
    vSocks5.push_back(0x00); // RSV Reserved
    vSocks5.push_back(0x03); // ATYP DOMAINNAME
=======
            return error("Error sending authentication to proxy");
        }
        LogPrint(BCLog::PROXY, "SOCKS5 sending proxy authentication %s:%s\n", auth->username, auth->password);
        uint8_t pchRetA[2];
        if ((recvr = InterruptibleRecv(pchRetA, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
            return error("Error reading proxy authentication response");
        }
        if (pchRetA[0] != 0x01 || pchRetA[1] != 0x00) {
            return error("Proxy authentication unsuccesful");
        }
    } else if (pchRet1[1] == SOCKS5Method::NOAUTH) {
        // Perform no authentication
    } else {
        return error("Proxy requested wrong authentication method %02x", pchRet1[1]);
    }
    std::vector<uint8_t> vSocks5;
    vSocks5.push_back(SOCKSVersion::SOCKS5); // VER protocol version
    vSocks5.push_back(SOCKS5Command::CONNECT); // CMD CONNECT
    vSocks5.push_back(0x00); // RSV Reserved must be 0
    vSocks5.push_back(SOCKS5Atyp::DOMAINNAME); // ATYP DOMAINNAME
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    vSocks5.push_back(strDest.size()); // Length<=255 is checked at beginning of function
    vSocks5.insert(vSocks5.end(), strDest.begin(), strDest.end());
    vSocks5.push_back((port >> 8) & 0xFF);
    vSocks5.push_back((port >> 0) & 0xFF);
    ret = send(hSocket, (const char*)vSocks5.data(), vSocks5.size(), MSG_NOSIGNAL);
    if (ret != (ssize_t)vSocks5.size()) {
<<<<<<< HEAD
        CloseSocket(hSocket);
        return error("Error sending to proxy");
    }
    char pchRet2[4];
    if ((recvr = InterruptibleRecv(pchRet2, 4, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
        CloseSocket(hSocket);
=======
        return error("Error sending to proxy");
    }
    uint8_t pchRet2[4];
    if ((recvr = InterruptibleRecv(pchRet2, 4, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (recvr == IntrRecvError::Timeout) {
            /* If a timeout happens here, this effectively means we timed out while connecting
             * to the remote node. This is very common for Tor, so do not print an
             * error message. */
            return false;
        } else {
            return error("Error while reading proxy response");
        }
    }
<<<<<<< HEAD
    if (pchRet2[0] != 0x05) {
        CloseSocket(hSocket);
        return error("Proxy failed to accept request");
    }
    if (pchRet2[1] != 0x00) {
        CloseSocket(hSocket);
        switch (pchRet2[1]) {
        case 0x01:
            return error("Proxy error: general failure");
        case 0x02:
            return error("Proxy error: connection not allowed");
        case 0x03:
            return error("Proxy error: network unreachable");
        case 0x04:
            return error("Proxy error: host unreachable");
        case 0x05:
            return error("Proxy error: connection refused");
        case 0x06:
            return error("Proxy error: TTL expired");
        case 0x07:
            return error("Proxy error: protocol error");
        case 0x08:
            return error("Proxy error: address type not supported");
        default:
            return error("Proxy error: unknown");
        }
    }
    if (pchRet2[2] != 0x00) {
        CloseSocket(hSocket);
        return error("Error: malformed proxy response");
    }
    char pchRet3[256];
    switch (pchRet2[3]) {
    case 0x01:
        recvr = InterruptibleRecv(pchRet3, 4, SOCKS5_RECV_TIMEOUT, hSocket);
        break;
    case 0x04:
        recvr = InterruptibleRecv(pchRet3, 16, SOCKS5_RECV_TIMEOUT, hSocket);
        break;
    case 0x03: {
        recvr = InterruptibleRecv(pchRet3, 1, SOCKS5_RECV_TIMEOUT, hSocket);
        if (recvr != IntrRecvError::OK) {
            CloseSocket(hSocket);
            return error("Error reading from proxy");
        }
        int nRecv = pchRet3[0];
        recvr = InterruptibleRecv(pchRet3, nRecv, SOCKS5_RECV_TIMEOUT, hSocket);
        break;
    }
    default:
        CloseSocket(hSocket);
        return error("Error: malformed proxy response");
    }
    if (recvr != IntrRecvError::OK) {
        CloseSocket(hSocket);
        return error("Error reading from proxy");
    }
    if ((recvr = InterruptibleRecv(pchRet3, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
        CloseSocket(hSocket);
        return error("Error reading from proxy");
    }
    LogPrintf("SOCKS5 connected %s\n", strDest);
    return true;
}

bool static ConnectSocketDirectly(const CService& addrConnect, SOCKET& hSocketRet, int nTimeout)
{
    hSocketRet = INVALID_SOCKET;

    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    if (!addrConnect.GetSockAddr((struct sockaddr*)&sockaddr, &len)) {
        LogPrintf("Cannot connect to %s: unsupported network\n", addrConnect.ToString());
        return false;
=======
    if (pchRet2[0] != SOCKSVersion::SOCKS5) {
        return error("Proxy failed to accept request");
    }
    if (pchRet2[1] != SOCKS5Reply::SUCCEEDED) {
        // Failures to connect to a peer that are not proxy errors
        LogPrintf("Socks5() connect to %s:%d failed: %s\n", strDest, port, Socks5ErrorString(pchRet2[1]));
        return false;
    }
    if (pchRet2[2] != 0x00) { // Reserved field must be 0
        return error("Error: malformed proxy response");
    }
    uint8_t pchRet3[256];
    switch (pchRet2[3])
    {
        case SOCKS5Atyp::IPV4: recvr = InterruptibleRecv(pchRet3, 4, SOCKS5_RECV_TIMEOUT, hSocket); break;
        case SOCKS5Atyp::IPV6: recvr = InterruptibleRecv(pchRet3, 16, SOCKS5_RECV_TIMEOUT, hSocket); break;
        case SOCKS5Atyp::DOMAINNAME:
        {
            recvr = InterruptibleRecv(pchRet3, 1, SOCKS5_RECV_TIMEOUT, hSocket);
            if (recvr != IntrRecvError::OK) {
                return error("Error reading from proxy");
            }
            int nRecv = pchRet3[0];
            recvr = InterruptibleRecv(pchRet3, nRecv, SOCKS5_RECV_TIMEOUT, hSocket);
            break;
        }
        default: return error("Error: malformed proxy response");
    }
    if (recvr != IntrRecvError::OK) {
        return error("Error reading from proxy");
    }
    if ((recvr = InterruptibleRecv(pchRet3, 2, SOCKS5_RECV_TIMEOUT, hSocket)) != IntrRecvError::OK) {
        return error("Error reading from proxy");
    }
    LogPrint(BCLog::NET, "SOCKS5 connected %s\n", strDest);
    return true;
}

SOCKET CreateSocket(const CService& addrConnect)
{
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    if (!addrConnect.GetSockAddr((struct sockaddr*)&sockaddr, &len)) {
        LogPrintf("Cannot create socket for %s: unsupported network\n", addrConnect.ToString());
        return INVALID_SOCKET;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    SOCKET hSocket = socket(((struct sockaddr*)&sockaddr)->sa_family, SOCK_STREAM, IPPROTO_TCP);
    if (hSocket == INVALID_SOCKET)
<<<<<<< HEAD
        return false;
=======
        return INVALID_SOCKET;

    if (!IsSelectableSocket(hSocket)) {
        CloseSocket(hSocket);
        LogPrintf("Cannot create connection: non-selectable socket created (fd >= FD_SETSIZE ?)\n");
        return INVALID_SOCKET;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifdef SO_NOSIGPIPE
    int set = 1;
    // Different way of disabling SIGPIPE on BSD
    setsockopt(hSocket, SOL_SOCKET, SO_NOSIGPIPE, (void*)&set, sizeof(int));
#endif

    // Set to non-blocking
<<<<<<< HEAD
    if (!SetSocketNonBlocking(hSocket, true))
        return error("ConnectSocketDirectly: Setting socket to non-blocking failed, error %s\n", NetworkErrorString(WSAGetLastError()));

=======
    if (!SetSocketNonBlocking(hSocket, true)) {
        CloseSocket(hSocket);
        LogPrintf("ConnectSocketDirectly: Setting socket to non-blocking failed, error %s\n", NetworkErrorString(WSAGetLastError()));
    }
    return hSocket;
}

bool ConnectSocketDirectly(const CService& addrConnect, const SOCKET& hSocket, int nTimeout)
{
    struct sockaddr_storage sockaddr;
    socklen_t len = sizeof(sockaddr);
    if (hSocket == INVALID_SOCKET) {
        LogPrintf("Cannot connect to %s: invalid socket\n", addrConnect.ToString());
        return false;
    }
    if (!addrConnect.GetSockAddr((struct sockaddr*)&sockaddr, &len)) {
        LogPrintf("Cannot connect to %s: unsupported network\n", addrConnect.ToString());
        return false;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (connect(hSocket, (struct sockaddr*)&sockaddr, len) == SOCKET_ERROR) {
        int nErr = WSAGetLastError();
        // WSAEINVAL is here because some legacy version of winsock uses it
        if (nErr == WSAEINPROGRESS || nErr == WSAEWOULDBLOCK || nErr == WSAEINVAL) {
            struct timeval timeout = MillisToTimeval(nTimeout);
            fd_set fdset;
            FD_ZERO(&fdset);
            FD_SET(hSocket, &fdset);
            int nRet = select(hSocket + 1, NULL, &fdset, NULL, &timeout);
            if (nRet == 0) {
<<<<<<< HEAD
                LogPrint("net", "connection to %s timeout\n", addrConnect.ToString());
                CloseSocket(hSocket);
=======
                LogPrint(BCLog::NET, "connection to %s timeout\n", addrConnect.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                return false;
            }
            if (nRet == SOCKET_ERROR) {
                LogPrintf("select() for %s failed: %s\n", addrConnect.ToString(), NetworkErrorString(WSAGetLastError()));
<<<<<<< HEAD
                CloseSocket(hSocket);
                return false;
            }
            socklen_t nRetSize = sizeof(nRet);
#ifdef WIN32
            if (getsockopt(hSocket, SOL_SOCKET, SO_ERROR, (char*)(&nRet), &nRetSize) == SOCKET_ERROR)
#else
            if (getsockopt(hSocket, SOL_SOCKET, SO_ERROR, &nRet, &nRetSize) == SOCKET_ERROR)
#endif
            {
                LogPrintf("getsockopt() for %s failed: %s\n", addrConnect.ToString(), NetworkErrorString(WSAGetLastError()));
                CloseSocket(hSocket);
=======
                return false;
            }
            socklen_t nRetSize = sizeof(nRet);
            if (getsockopt(hSocket, SOL_SOCKET, SO_ERROR, (sockopt_arg_type)&nRet, &nRetSize) == SOCKET_ERROR)
            {
                LogPrintf("getsockopt() for %s failed: %s\n", addrConnect.ToString(), NetworkErrorString(WSAGetLastError()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                return false;
            }
            if (nRet != 0) {
                LogPrintf("connect() to %s failed after select(): %s\n", addrConnect.ToString(), NetworkErrorString(nRet));
<<<<<<< HEAD
                CloseSocket(hSocket);
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                return false;
            }
        }
#ifdef WIN32
        else if (WSAGetLastError() != WSAEISCONN)
#else
        else
#endif
        {
            LogPrintf("connect() to %s failed: %s\n", addrConnect.ToString(), NetworkErrorString(WSAGetLastError()));
<<<<<<< HEAD
            CloseSocket(hSocket);
            return false;
        }
    }

    hSocketRet = hSocket;
=======
            return false;
        }
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return true;
}

bool SetProxy(enum Network net, const proxyType &addrProxy)
{
    assert(net >= 0 && net < NET_MAX);
    if (!addrProxy.IsValid())
        return false;
    LOCK(cs_proxyInfos);
    proxyInfo[net] = addrProxy;
    return true;
}

bool GetProxy(enum Network net, proxyType& proxyInfoOut)
{
    assert(net >= 0 && net < NET_MAX);
    LOCK(cs_proxyInfos);
    if (!proxyInfo[net].IsValid())
        return false;
    proxyInfoOut = proxyInfo[net];
    return true;
}

bool SetNameProxy(const proxyType &addrProxy)
{
    if (!addrProxy.IsValid())
        return false;
    LOCK(cs_proxyInfos);
    nameProxy = addrProxy;
    return true;
}

bool GetNameProxy(proxyType &nameProxyOut)
{
    LOCK(cs_proxyInfos);
    if (!nameProxy.IsValid())
        return false;
    nameProxyOut = nameProxy;
    return true;
}

bool HaveNameProxy()
{
    LOCK(cs_proxyInfos);
    return nameProxy.IsValid();
}

bool IsProxy(const CNetAddr& addr)
{
    LOCK(cs_proxyInfos);
    for (int i = 0; i < NET_MAX; i++) {
<<<<<<< HEAD
        if (addr == (CNetAddr)proxyInfo[i].proxy)
=======
        if (addr == static_cast<CNetAddr>(proxyInfo[i].proxy))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return true;
    }
    return false;
}

<<<<<<< HEAD
static bool ConnectThroughProxy(const proxyType &proxy, const std::string strDest, int port, SOCKET& hSocketRet, int nTimeout, bool *outProxyConnectionFailed)
{
    SOCKET hSocket = INVALID_SOCKET;
=======
bool ConnectThroughProxy(const proxyType &proxy, const std::string& strDest, int port, const SOCKET& hSocket, int nTimeout, bool *outProxyConnectionFailed)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // first connect to proxy server
    if (!ConnectSocketDirectly(proxy.proxy, hSocket, nTimeout)) {
        if (outProxyConnectionFailed)
            *outProxyConnectionFailed = true;
        return false;
    }
    // do socks negotiation
    if (proxy.randomize_credentials) {
        ProxyCredentials random_auth;
        static std::atomic_int counter;
        random_auth.username = random_auth.password = strprintf("%i", counter++);
<<<<<<< HEAD
        if (!Socks5(strDest, (unsigned short)port, &random_auth, hSocket))
            return false;
    } else {
        if (!Socks5(strDest, (unsigned short)port, 0, hSocket))
            return false;
    }

    hSocketRet = hSocket;
    return true;
}

bool ConnectSocket(const CService &addrDest, SOCKET& hSocketRet, int nTimeout, bool *outProxyConnectionFailed)
{
    proxyType proxy;
    if (outProxyConnectionFailed)
        *outProxyConnectionFailed = false;

    if (GetProxy(addrDest.GetNetwork(), proxy))
        return ConnectThroughProxy(proxy, addrDest.ToStringIP(), addrDest.GetPort(), hSocketRet, nTimeout, outProxyConnectionFailed);
    else // no proxy needed (none set for target network)
        return ConnectSocketDirectly(addrDest, hSocketRet, nTimeout);
}

bool ConnectSocketByName(CService& addr, SOCKET& hSocketRet, const char* pszDest, int portDefault, int nTimeout, bool* outProxyConnectionFailed)
{
    string strDest;
    int port = portDefault;

    if (outProxyConnectionFailed)
        *outProxyConnectionFailed = false;

    SplitHostPort(string(pszDest), port, strDest);

    proxyType nameProxy;
    GetNameProxy(nameProxy);

    CService addrResolved;
    if (Lookup(strDest.c_str(), addrResolved, port, fNameLookup && !HaveNameProxy())) {
        if (addrResolved.IsValid()) {
            addr = addrResolved;
            return ConnectSocket(addr, hSocketRet, nTimeout);
        }
    }

    addr = CService("0.0.0.0:0");

    if (!HaveNameProxy())
        return false;
    return ConnectThroughProxy(nameProxy, strDest, port, hSocketRet, nTimeout, outProxyConnectionFailed);
}

void CNetAddr::Init()
{
    memset(ip, 0, sizeof(ip));
}

void CNetAddr::SetIP(const CNetAddr& ipIn)
{
    memcpy(ip, ipIn.ip, sizeof(ip));
}

void CNetAddr::SetRaw(Network network, const uint8_t* ip_in)
{
    switch (network) {
    case NET_IPV4:
        memcpy(ip, pchIPv4, 12);
        memcpy(ip + 12, ip_in, 4);
        break;
    case NET_IPV6:
        memcpy(ip, ip_in, 16);
        break;
    default:
        assert(!"invalid network");
    }
}

static const unsigned char pchOnionCat[] = {0xFD, 0x87, 0xD8, 0x7E, 0xEB, 0x43};

bool CNetAddr::SetSpecial(const std::string& strName)
{
    if (strName.size() > 6 && strName.substr(strName.size() - 6, 6) == ".onion") {
        std::vector<unsigned char> vchAddr = DecodeBase32(strName.substr(0, strName.size() - 6).c_str());
        if (vchAddr.size() != 16 - sizeof(pchOnionCat))
            return false;
        memcpy(ip, pchOnionCat, sizeof(pchOnionCat));
        for (unsigned int i = 0; i < 16 - sizeof(pchOnionCat); i++)
            ip[i + sizeof(pchOnionCat)] = vchAddr[i];
        return true;
    }
    return false;
}

CNetAddr::CNetAddr()
{
    Init();
}

CNetAddr::CNetAddr(const struct in_addr& ipv4Addr)
{
    SetRaw(NET_IPV4, (const uint8_t*)&ipv4Addr);
}

CNetAddr::CNetAddr(const struct in6_addr& ipv6Addr)
{
    SetRaw(NET_IPV6, (const uint8_t*)&ipv6Addr);
}

CNetAddr::CNetAddr(const char* pszIp)
{
    Init();
    std::vector<CNetAddr> vIP;
    if (LookupHost(pszIp, vIP, 1, false))
        *this = vIP[0];
}

CNetAddr::CNetAddr(const std::string& strIp)
{
    Init();
    std::vector<CNetAddr> vIP;
    if (LookupHost(strIp.c_str(), vIP, 1, false))
        *this = vIP[0];
}

unsigned int CNetAddr::GetByte(int n) const
{
    return ip[15 - n];
}

bool CNetAddr::IsIPv4() const
{
    return (memcmp(ip, pchIPv4, sizeof(pchIPv4)) == 0);
}

bool CNetAddr::IsIPv6() const
{
    return (!IsIPv4() && !IsTor());
}

bool CNetAddr::IsRFC1918() const
{
    return IsIPv4() && (GetByte(3) == 10 ||
                           (GetByte(3) == 192 && GetByte(2) == 168) ||
                           (GetByte(3) == 172 && (GetByte(2) >= 16 && GetByte(2) <= 31)));
}

bool CNetAddr::IsRFC2544() const
{
    return IsIPv4() && GetByte(3) == 198 && (GetByte(2) == 18 || GetByte(2) == 19);
}

bool CNetAddr::IsRFC3927() const
{
    return IsIPv4() && (GetByte(3) == 169 && GetByte(2) == 254);
}

bool CNetAddr::IsRFC6598() const
{
    return IsIPv4() && GetByte(3) == 100 && GetByte(2) >= 64 && GetByte(2) <= 127;
}

bool CNetAddr::IsRFC5737() const
{
    return IsIPv4() && ((GetByte(3) == 192 && GetByte(2) == 0 && GetByte(1) == 2) ||
                           (GetByte(3) == 198 && GetByte(2) == 51 && GetByte(1) == 100) ||
                           (GetByte(3) == 203 && GetByte(2) == 0 && GetByte(1) == 113));
}

bool CNetAddr::IsRFC3849() const
{
    return GetByte(15) == 0x20 && GetByte(14) == 0x01 && GetByte(13) == 0x0D && GetByte(12) == 0xB8;
}

bool CNetAddr::IsRFC3964() const
{
    return (GetByte(15) == 0x20 && GetByte(14) == 0x02);
}

bool CNetAddr::IsRFC6052() const
{
    static const unsigned char pchRFC6052[] = {0, 0x64, 0xFF, 0x9B, 0, 0, 0, 0, 0, 0, 0, 0};
    return (memcmp(ip, pchRFC6052, sizeof(pchRFC6052)) == 0);
}

bool CNetAddr::IsRFC4380() const
{
    return (GetByte(15) == 0x20 && GetByte(14) == 0x01 && GetByte(13) == 0 && GetByte(12) == 0);
}

bool CNetAddr::IsRFC4862() const
{
    static const unsigned char pchRFC4862[] = {0xFE, 0x80, 0, 0, 0, 0, 0, 0};
    return (memcmp(ip, pchRFC4862, sizeof(pchRFC4862)) == 0);
}

bool CNetAddr::IsRFC4193() const
{
    return ((GetByte(15) & 0xFE) == 0xFC);
}

bool CNetAddr::IsRFC6145() const
{
    static const unsigned char pchRFC6145[] = {0, 0, 0, 0, 0, 0, 0, 0, 0xFF, 0xFF, 0, 0};
    return (memcmp(ip, pchRFC6145, sizeof(pchRFC6145)) == 0);
}

bool CNetAddr::IsRFC4843() const
{
    return (GetByte(15) == 0x20 && GetByte(14) == 0x01 && GetByte(13) == 0x00 && (GetByte(12) & 0xF0) == 0x10);
}

bool CNetAddr::IsTor() const
{
    return (memcmp(ip, pchOnionCat, sizeof(pchOnionCat)) == 0);
}

bool CNetAddr::IsLocal() const
{
    // IPv4 loopback
    if (IsIPv4() && (GetByte(3) == 127 || GetByte(3) == 0))
        return true;

    // IPv6 loopback (::1/128)
    static const unsigned char pchLocal[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    if (memcmp(ip, pchLocal, 16) == 0)
        return true;

    return false;
}

bool CNetAddr::IsMulticast() const
{
    return (IsIPv4() && (GetByte(3) & 0xF0) == 0xE0) || (GetByte(15) == 0xFF);
}

bool CNetAddr::IsValid() const
{
    // Cleanup 3-byte shifted addresses caused by garbage in size field
    // of addr messages from versions before 0.2.9 checksum.
    // Two consecutive addr messages look like this:
    // header20 vectorlen3 addr26 addr26 addr26 header20 vectorlen3 addr26 addr26 addr26...
    // so if the first length field is garbled, it reads the second batch
    // of addr misaligned by 3 bytes.
    if (memcmp(ip, pchIPv4 + 3, sizeof(pchIPv4) - 3) == 0)
        return false;

    // unspecified IPv6 address (::/128)
    unsigned char ipNone6[16] = {};
    if (memcmp(ip, ipNone6, 16) == 0)
        return false;

    // documentation IPv6 address
    if (IsRFC3849())
        return false;

    if (IsIPv4()) {
        // INADDR_NONE
        uint32_t ipNone = INADDR_NONE;
        if (memcmp(ip + 12, &ipNone, 4) == 0)
            return false;

        // 0
        ipNone = 0;
        if (memcmp(ip + 12, &ipNone, 4) == 0)
            return false;
    }

    return true;
}

bool CNetAddr::IsRoutable() const
{
    return IsValid() && !(IsRFC1918() || IsRFC2544() || IsRFC3927() || IsRFC4862() || IsRFC6598() || IsRFC5737() || (IsRFC4193() && !IsTor()) || IsRFC4843() || IsLocal());
}

enum Network CNetAddr::GetNetwork() const
{
    if (!IsRoutable())
        return NET_UNROUTABLE;

    if (IsIPv4())
        return NET_IPV4;

    if (IsTor())
        return NET_TOR;

    return NET_IPV6;
}

std::string CNetAddr::ToStringIP() const
{
    if (IsTor())
        return EncodeBase32(&ip[6], 10) + ".onion";
    CService serv(*this, 0);
    struct sockaddr_storage sockaddr;
    socklen_t socklen = sizeof(sockaddr);
    if (serv.GetSockAddr((struct sockaddr*)&sockaddr, &socklen)) {
        char name[1025] = "";
        if (!getnameinfo((const struct sockaddr*)&sockaddr, socklen, name, sizeof(name), NULL, 0, NI_NUMERICHOST))
            return std::string(name);
    }
    if (IsIPv4())
        return strprintf("%u.%u.%u.%u", GetByte(3), GetByte(2), GetByte(1), GetByte(0));
    else
        return strprintf("%x:%x:%x:%x:%x:%x:%x:%x",
            GetByte(15) << 8 | GetByte(14), GetByte(13) << 8 | GetByte(12),
            GetByte(11) << 8 | GetByte(10), GetByte(9) << 8 | GetByte(8),
            GetByte(7) << 8 | GetByte(6), GetByte(5) << 8 | GetByte(4),
            GetByte(3) << 8 | GetByte(2), GetByte(1) << 8 | GetByte(0));
}

std::string CNetAddr::ToString() const
{
    return ToStringIP();
}

bool operator==(const CNetAddr& a, const CNetAddr& b)
{
    return (memcmp(a.ip, b.ip, 16) == 0);
}

bool operator!=(const CNetAddr& a, const CNetAddr& b)
{
    return (memcmp(a.ip, b.ip, 16) != 0);
}

bool operator<(const CNetAddr& a, const CNetAddr& b)
{
    return (memcmp(a.ip, b.ip, 16) < 0);
}

bool CNetAddr::GetInAddr(struct in_addr* pipv4Addr) const
{
    if (!IsIPv4())
        return false;
    memcpy(pipv4Addr, ip + 12, 4);
    return true;
}

bool CNetAddr::GetIn6Addr(struct in6_addr* pipv6Addr) const
{
    memcpy(pipv6Addr, ip, 16);
    return true;
}

// get canonical identifier of an address' group
// no two connections will be attempted to addresses with the same group
std::vector<unsigned char> CNetAddr::GetGroup() const
{
    std::vector<unsigned char> vchRet;
    int nClass = NET_IPV6;
    int nStartByte = 0;
    int nBits = 16;

    // all local addresses belong to the same group
    if (IsLocal()) {
        nClass = 255;
        nBits = 0;
    }

    // all unroutable addresses belong to the same group
    if (!IsRoutable()) {
        nClass = NET_UNROUTABLE;
        nBits = 0;
    }
    // for IPv4 addresses, '1' + the 16 higher-order bits of the IP
    // includes mapped IPv4, SIIT translated IPv4, and the well-known prefix
    else if (IsIPv4() || IsRFC6145() || IsRFC6052()) {
        nClass = NET_IPV4;
        nStartByte = 12;
    }
    // for 6to4 tunnelled addresses, use the encapsulated IPv4 address
    else if (IsRFC3964()) {
        nClass = NET_IPV4;
        nStartByte = 2;
    }
    // for Teredo-tunnelled IPv6 addresses, use the encapsulated IPv4 address
    else if (IsRFC4380()) {
        vchRet.push_back(NET_IPV4);
        vchRet.push_back(GetByte(3) ^ 0xFF);
        vchRet.push_back(GetByte(2) ^ 0xFF);
        return vchRet;
    } else if (IsTor()) {
        nClass = NET_TOR;
        nStartByte = 6;
        nBits = 4;
    }
    // for he.net, use /36 groups
    else if (GetByte(15) == 0x20 && GetByte(14) == 0x01 && GetByte(13) == 0x04 && GetByte(12) == 0x70)
        nBits = 36;
    // for the rest of the IPv6 network, use /32 groups
    else
        nBits = 32;

    vchRet.push_back(nClass);
    while (nBits >= 8) {
        vchRet.push_back(GetByte(15 - nStartByte));
        nStartByte++;
        nBits -= 8;
    }
    if (nBits > 0)
        vchRet.push_back(GetByte(15 - nStartByte) | ((1 << nBits) - 1));

    return vchRet;
}

uint64_t CNetAddr::GetHash() const
{
    uint256 hash = Hash(&ip[0], &ip[16]);
    uint64_t nRet;
    memcpy(&nRet, &hash, sizeof(nRet));
    return nRet;
}

// private extensions to enum Network, only returned by GetExtNetwork,
// and only used in GetReachabilityFrom
static const int NET_UNKNOWN = NET_MAX + 0;
static const int NET_TEREDO = NET_MAX + 1;
int static GetExtNetwork(const CNetAddr* addr)
{
    if (addr == NULL)
        return NET_UNKNOWN;
    if (addr->IsRFC4380())
        return NET_TEREDO;
    return addr->GetNetwork();
}

/** Calculates a metric for how reachable (*this) is from a given partner */
int CNetAddr::GetReachabilityFrom(const CNetAddr* paddrPartner) const
{
    enum Reachability {
        REACH_UNREACHABLE,
        REACH_DEFAULT,
        REACH_TEREDO,
        REACH_IPV6_WEAK,
        REACH_IPV4,
        REACH_IPV6_STRONG,
        REACH_PRIVATE
    };

    if (!IsRoutable())
        return REACH_UNREACHABLE;

    int ourNet = GetExtNetwork(this);
    int theirNet = GetExtNetwork(paddrPartner);
    bool fTunnel = IsRFC3964() || IsRFC6052() || IsRFC6145();

    switch (theirNet) {
    case NET_IPV4:
        switch (ourNet) {
        default:
            return REACH_DEFAULT;
        case NET_IPV4:
            return REACH_IPV4;
        }
    case NET_IPV6:
        switch (ourNet) {
        default:
            return REACH_DEFAULT;
        case NET_TEREDO:
            return REACH_TEREDO;
        case NET_IPV4:
            return REACH_IPV4;
        case NET_IPV6:
            return fTunnel ? REACH_IPV6_WEAK : REACH_IPV6_STRONG; // only prefer giving our IPv6 address if it's not tunnelled
        }
    case NET_TOR:
        switch (ourNet) {
        default:
            return REACH_DEFAULT;
        case NET_IPV4:
            return REACH_IPV4; // Tor users can connect to IPv4 as well
        case NET_TOR:
            return REACH_PRIVATE;
        }
    case NET_TEREDO:
        switch (ourNet) {
        default:
            return REACH_DEFAULT;
        case NET_TEREDO:
            return REACH_TEREDO;
        case NET_IPV6:
            return REACH_IPV6_WEAK;
        case NET_IPV4:
            return REACH_IPV4;
        }
    case NET_UNKNOWN:
    case NET_UNROUTABLE:
    default:
        switch (ourNet) {
        default:
            return REACH_DEFAULT;
        case NET_TEREDO:
            return REACH_TEREDO;
        case NET_IPV6:
            return REACH_IPV6_WEAK;
        case NET_IPV4:
            return REACH_IPV4;
        case NET_TOR:
            return REACH_PRIVATE; // either from Tor, or don't care about our address
        }
    }
}

void CService::Init()
{
    port = 0;
}

CService::CService()
{
    Init();
}

CService::CService(const CNetAddr& cip, unsigned short portIn) : CNetAddr(cip), port(portIn)
{
}

CService::CService(const struct in_addr& ipv4Addr, unsigned short portIn) : CNetAddr(ipv4Addr), port(portIn)
{
}

CService::CService(const struct in6_addr& ipv6Addr, unsigned short portIn) : CNetAddr(ipv6Addr), port(portIn)
{
}

CService::CService(const struct sockaddr_in& addr) : CNetAddr(addr.sin_addr), port(ntohs(addr.sin_port))
{
    assert(addr.sin_family == AF_INET);
}

CService::CService(const struct sockaddr_in6& addr) : CNetAddr(addr.sin6_addr), port(ntohs(addr.sin6_port))
{
    assert(addr.sin6_family == AF_INET6);
}

bool CService::SetSockAddr(const struct sockaddr* paddr)
{
    switch (paddr->sa_family) {
    case AF_INET:
        *this = CService(*(const struct sockaddr_in*)paddr);
        return true;
    case AF_INET6:
        *this = CService(*(const struct sockaddr_in6*)paddr);
        return true;
    default:
        return false;
    }
}

CService::CService(const char* pszIpPort)
{
    Init();
    CService ip;
    if (Lookup(pszIpPort, ip, 0, false))
        *this = ip;
}

CService::CService(const char* pszIpPort, int portDefault)
{
    Init();
    CService ip;
    if (Lookup(pszIpPort, ip, portDefault, false))
        *this = ip;
}

CService::CService(const std::string& strIpPort)
{
    Init();
    CService ip;
    if (Lookup(strIpPort.c_str(), ip, 0, false))
        *this = ip;
}

CService::CService(const std::string& strIpPort, int portDefault)
{
    Init();
    CService ip;
    if (Lookup(strIpPort.c_str(), ip, portDefault, false))
        *this = ip;
}

unsigned short CService::GetPort() const
{
    return port;
}

bool operator==(const CService& a, const CService& b)
{
    return (CNetAddr)a == (CNetAddr)b && a.port == b.port;
}

bool operator!=(const CService& a, const CService& b)
{
    return (CNetAddr)a != (CNetAddr)b || a.port != b.port;
}

bool operator<(const CService& a, const CService& b)
{
    return (CNetAddr)a < (CNetAddr)b || ((CNetAddr)a == (CNetAddr)b && a.port < b.port);
}

bool CService::GetSockAddr(struct sockaddr* paddr, socklen_t* addrlen) const
{
    if (IsIPv4()) {
        if (*addrlen < (socklen_t)sizeof(struct sockaddr_in))
            return false;
        *addrlen = sizeof(struct sockaddr_in);
        struct sockaddr_in* paddrin = (struct sockaddr_in*)paddr;
        memset(paddrin, 0, *addrlen);
        if (!GetInAddr(&paddrin->sin_addr))
            return false;
        paddrin->sin_family = AF_INET;
        paddrin->sin_port = htons(port);
        return true;
    }
    if (IsIPv6()) {
        if (*addrlen < (socklen_t)sizeof(struct sockaddr_in6))
            return false;
        *addrlen = sizeof(struct sockaddr_in6);
        struct sockaddr_in6* paddrin6 = (struct sockaddr_in6*)paddr;
        memset(paddrin6, 0, *addrlen);
        if (!GetIn6Addr(&paddrin6->sin6_addr))
            return false;
        paddrin6->sin6_family = AF_INET6;
        paddrin6->sin6_port = htons(port);
        return true;
    }
    return false;
}

std::vector<unsigned char> CService::GetKey() const
{
    std::vector<unsigned char> vKey;
    vKey.resize(18);
    memcpy(&vKey[0], ip, 16);
    vKey[16] = port / 0x100;
    vKey[17] = port & 0x0FF;
    return vKey;
}

std::string CService::ToStringPort() const
{
    return strprintf("%u", port);
}

std::string CService::ToStringIPPort() const
{
    if (IsIPv4() || IsTor()) {
        return ToStringIP() + ":" + ToStringPort();
    } else {
        return "[" + ToStringIP() + "]:" + ToStringPort();
    }
}

std::string CService::ToString() const
{
    return ToStringIPPort();
}

void CService::SetPort(unsigned short portIn)
{
    port = portIn;
}

CSubNet::CSubNet() : valid(false)
{
    memset(netmask, 0, sizeof(netmask));
}

CSubNet::CSubNet(const std::string& strSubnet)
{
    size_t slash = strSubnet.find_last_of('/');
    std::vector<CNetAddr> vIP;

    valid = true;
    // Default to /32 (IPv4) or /128 (IPv6), i.e. match single address
    memset(netmask, 255, sizeof(netmask));

    std::string strAddress = strSubnet.substr(0, slash);
    if (LookupHost(strAddress.c_str(), vIP, 1, false))
    {
        network = vIP[0];
        if (slash != strSubnet.npos) {
            std::string strNetmask = strSubnet.substr(slash + 1);
            int32_t n;
            // IPv4 addresses start at offset 12, and first 12 bytes must match, so just offset n
            const int astartofs = network.IsIPv4() ? 12 : 0;
            if (ParseInt32(strNetmask, &n)) // If valid number, assume /24 symtex
            {
                if (n >= 0 && n <= (128 - astartofs * 8)) // Only valid if in range of bits of address
                {
                    n += astartofs * 8;
                    // Clear bits [n..127]
                    for (; n < 128; ++n)
                        netmask[n >> 3] &= ~(1 << (7 - (n & 7)));
                } else {
                    valid = false;
                }
            } else // If not a valid number, try full netmask syntax
            {
                if (LookupHost(strNetmask.c_str(), vIP, 1, false)) // Never allow lookup for netmask
                {
                    // Copy only the *last* four bytes in case of IPv4, the rest of the mask should stay 1's as
                    // we don't want pchIPv4 to be part of the mask.
                    for (int x = astartofs; x < 16; ++x)
                        netmask[x] = vIP[0].ip[x];
                } else {
                    valid = false;
                }
            }
        }
    } else {
        valid = false;
    }

    // Normalize network according to netmask
    for (int x = 0; x < 16; ++x)
        network.ip[x] &= netmask[x];
}

CSubNet::CSubNet(const CNetAddr &addr):
        valid(addr.IsValid())
{
    memset(netmask, 255, sizeof(netmask));
    network = addr;
}

bool CSubNet::Match(const CNetAddr& addr) const
{
    if (!valid || !addr.IsValid())
        return false;
    for (int x = 0; x < 16; ++x)
        if ((addr.ip[x] & netmask[x]) != network.ip[x])
            return false;
    return true;
}

static inline int NetmaskBits(uint8_t x)
{
    switch(x) {
        case 0x00: return 0; break;
        case 0x80: return 1; break;
        case 0xc0: return 2; break;
        case 0xe0: return 3; break;
        case 0xf0: return 4; break;
        case 0xf8: return 5; break;
        case 0xfc: return 6; break;
        case 0xfe: return 7; break;
        case 0xff: return 8; break;
        default: return -1; break;
    }
}

std::string CSubNet::ToString() const
{
    /* Parse binary 1{n}0{N-n} to see if mask can be represented as /n */
    int cidr = 0;
    bool valid_cidr = true;
    int n = network.IsIPv4() ? 12 : 0;
    for (; n < 16 && netmask[n] == 0xff; ++n)
        cidr += 8;
    if (n < 16) {
        int bits = NetmaskBits(netmask[n]);
        if (bits < 0)
            valid_cidr = false;
        else
            cidr += bits;
        ++n;
    }
    for (; n < 16 && valid_cidr; ++n)
        if (netmask[n] != 0x00)
            valid_cidr = false;

    /* Format output */
    std::string strNetmask;
    if (valid_cidr) {
        strNetmask = strprintf("%u", cidr);
    } else {
        if (network.IsIPv4())
            strNetmask = strprintf("%u.%u.%u.%u", netmask[12], netmask[13], netmask[14], netmask[15]);
        else
            strNetmask = strprintf("%x:%x:%x:%x:%x:%x:%x:%x",
                                   netmask[0] << 8 | netmask[1], netmask[2] << 8 | netmask[3],
                                   netmask[4] << 8 | netmask[5], netmask[6] << 8 | netmask[7],
                                   netmask[8] << 8 | netmask[9], netmask[10] << 8 | netmask[11],
                                   netmask[12] << 8 | netmask[13], netmask[14] << 8 | netmask[15]);
    }

    return network.ToString() + "/" + strNetmask;
}

bool CSubNet::IsValid() const
{
    return valid;
}

bool operator==(const CSubNet& a, const CSubNet& b)
{
    return a.valid == b.valid && a.network == b.network && !memcmp(a.netmask, b.netmask, 16);
}

bool operator!=(const CSubNet& a, const CSubNet& b)
{
    return !(a == b);
}

bool operator<(const CSubNet& a, const CSubNet& b)
{
    return (a.network < b.network || (a.network == b.network && memcmp(a.netmask, b.netmask, 16) < 0));
=======
        if (!Socks5(strDest, (uint16_t)port, &random_auth, hSocket)) {
            return false;
        }
    } else {
        if (!Socks5(strDest, (uint16_t)port, 0, hSocket)) {
            return false;
        }
    }
    return true;
}

bool LookupSubNet(const std::string& strSubnet, CSubNet& ret)
{
    if (!ValidAsCString(strSubnet)) {
        return false;
    }
    size_t slash = strSubnet.find_last_of('/');
    std::vector<CNetAddr> vIP;

    std::string strAddress = strSubnet.substr(0, slash);
    if (LookupHost(strAddress, vIP, 1, false))
    {
        CNetAddr network = vIP[0];
        if (slash != strSubnet.npos) {
            std::string strNetmask = strSubnet.substr(slash + 1);
            uint8_t n;
            if (ParseUInt8(strNetmask, &n)) {
                // If valid number, assume CIDR variable-length subnet masking
                ret = CSubNet(network, n);
                return ret.IsValid();
            } else // If not a valid number, try full netmask syntax
            {
                // Never allow lookup for netmask
                if (LookupHost(strNetmask, vIP, 1, false)) {
                    ret = CSubNet(network, vIP[0]);
                    return ret.IsValid();
                }
            }
        } else {
            ret = CSubNet(network);
            return ret.IsValid();
        }
    }
    return false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

#ifdef WIN32
std::string NetworkErrorString(int err)
{
<<<<<<< HEAD
    char buf[256];
    buf[0] = 0;
    if (FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
            NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buf, sizeof(buf), NULL)) {
        return strprintf("%s (%d)", buf, err);
=======
    wchar_t buf[256];
    buf[0] = 0;
    if(FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
            nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            buf, ARRAYSIZE(buf), nullptr))
    {
        const auto& bufConvert = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>,wchar_t>().to_bytes(buf);
        return strprintf("%s (%d)", bufConvert, err);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    } else {
        return strprintf("Unknown error (%d)", err);
    }
}
#else
std::string NetworkErrorString(int err)
{
    char buf[256];
<<<<<<< HEAD
    const char* s = buf;
    buf[0] = 0;
/* Too bad there are two incompatible implementations of the
     * thread-safe strerror. */
#ifdef STRERROR_R_CHAR_P /* GNU variant can return a pointer outside the passed buffer */
    s = strerror_r(err, buf, sizeof(buf));
#else                    /* POSIX variant always returns message in buffer */
=======
    buf[0] = 0;
/* Too bad there are two incompatible implementations of the
     * thread-safe strerror. */
    const char *s;
#ifdef STRERROR_R_CHAR_P /* GNU variant can return a pointer outside the passed buffer */
    s = strerror_r(err, buf, sizeof(buf));
#else                    /* POSIX variant always returns message in buffer */
    s = buf;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (strerror_r(err, buf, sizeof(buf)))
        buf[0] = 0;
#endif
    return strprintf("%s (%d)", s, err);
}
#endif

bool CloseSocket(SOCKET& hSocket)
{
    if (hSocket == INVALID_SOCKET)
        return false;
#ifdef WIN32
    int ret = closesocket(hSocket);
#else
    int ret = close(hSocket);
#endif
<<<<<<< HEAD
    if (ret) {
        LogPrint("net", "Socket close failed: %d. Error: %s\n", hSocket, NetworkErrorString(WSAGetLastError()));
    }
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    hSocket = INVALID_SOCKET;
    return ret != SOCKET_ERROR;
}

bool SetSocketNonBlocking(SOCKET& hSocket, bool fNonBlocking)
{
    if (fNonBlocking) {
#ifdef WIN32
        u_long nOne = 1;
        if (ioctlsocket(hSocket, FIONBIO, &nOne) == SOCKET_ERROR) {
#else
        int fFlags = fcntl(hSocket, F_GETFL, 0);
        if (fcntl(hSocket, F_SETFL, fFlags | O_NONBLOCK) == SOCKET_ERROR) {
#endif
            CloseSocket(hSocket);
            return false;
        }
    } else {
#ifdef WIN32
        u_long nZero = 0;
        if (ioctlsocket(hSocket, FIONBIO, &nZero) == SOCKET_ERROR) {
#else
        int fFlags = fcntl(hSocket, F_GETFL, 0);
        if (fcntl(hSocket, F_SETFL, fFlags & ~O_NONBLOCK) == SOCKET_ERROR) {
#endif
            CloseSocket(hSocket);
            return false;
        }
    }

    return true;
}
<<<<<<< HEAD
=======

void InterruptSocks5(bool interrupt)
{
    interruptSocks5Recv = interrupt;
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
