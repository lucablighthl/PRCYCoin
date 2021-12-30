// Copyright (c) 2015-2017 The Bitcoin Core developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2017-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "httprpc.h"

<<<<<<< HEAD
#include "base58.h"
#include "chainparams.h"
#include "httpserver.h"
=======
#include "chainparams.h"
#include "crypto/hmac_sha256.h"
#include "guiinterface.h"
#include "httpserver.h"
#include "key_io.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "rpc/protocol.h"
#include "rpc/server.h"
#include "random.h"
#include "sync.h"
<<<<<<< HEAD
#include "util.h"
#include "utilstrencodings.h"
#include "guiinterface.h"
=======
#include "util/system.h"
#include "utilstrencodings.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <boost/algorithm/string.hpp> // boost::trim

/** Simple one-shot callback timer to be used by the RPC mechanism to e.g.
 * re-lock the wellet.
 */
class HTTPRPCTimer : public RPCTimerBase
{
public:
<<<<<<< HEAD
    HTTPRPCTimer(struct event_base* eventBase, boost::function<void(void)>& func, int64_t millis) :
=======
    HTTPRPCTimer(struct event_base* eventBase, std::function<void(void)>& func, int64_t millis) :
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ev(eventBase, false, func)
    {
        struct timeval tv;
        tv.tv_sec = millis/1000;
        tv.tv_usec = (millis%1000)*1000;
        ev.trigger(&tv);
    }
private:
    HTTPEvent ev;
};

class HTTPRPCTimerInterface : public RPCTimerInterface
{
public:
    HTTPRPCTimerInterface(struct event_base* base) : base(base)
    {
    }
    const char* Name()
    {
        return "HTTP";
    }
<<<<<<< HEAD
    RPCTimerBase* NewTimer(boost::function<void(void)>& func, int64_t millis)
=======
    RPCTimerBase* NewTimer(std::function<void(void)>& func, int64_t millis)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        return new HTTPRPCTimer(base, func, millis);
    }
private:
    struct event_base* base;
};


/* Pre-base64-encoded authentication token */
static std::string strRPCUserColonPass;
/* Stored RPC timer interface (for unregistration) */
<<<<<<< HEAD
static HTTPRPCTimerInterface* httpRPCTimerInterface = 0;
=======
static std::unique_ptr<HTTPRPCTimerInterface> httpRPCTimerInterface;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static void JSONErrorReply(HTTPRequest* req, const UniValue& objError, const UniValue& id)
{
    // Send error reply from json-rpc error object
    int nStatus = HTTP_INTERNAL_SERVER_ERROR;
    int code = find_value(objError, "code").get_int();

    if (code == RPC_INVALID_REQUEST)
        nStatus = HTTP_BAD_REQUEST;
    else if (code == RPC_METHOD_NOT_FOUND)
        nStatus = HTTP_NOT_FOUND;

    std::string strReply = JSONRPCReply(NullUniValue, objError, id);

    req->WriteHeader("Content-Type", "application/json");
    req->WriteReply(nStatus, strReply);
}

<<<<<<< HEAD
static bool RPCAuthorized(const std::string& strAuth)
=======
//This function checks username and password against -rpcauth
//entries from config file.
static bool multiUserAuthorized(std::string strUserPass)
{
    if (strUserPass.find(':') == std::string::npos) {
        return false;
    }
    std::string strUser = strUserPass.substr(0, strUserPass.find(':'));
    std::string strPass = strUserPass.substr(strUserPass.find(':') + 1);

    for (const std::string& strRPCAuth : gArgs.GetArgs("-rpcauth")) {
        //Search for multi-user login/pass "rpcauth" from config
        std::vector<std::string> vFields;
        boost::split(vFields, strRPCAuth, boost::is_any_of(":$"));
        if (vFields.size() != 3) {
            //Incorrect formatting in config file
            continue;
        }

        std::string strName = vFields[0];
        if (!TimingResistantEqual(strName, strUser)) {
            continue;
        }

        std::string strSalt = vFields[1];
        std::string strHash = vFields[2];

        static const unsigned int KEY_SIZE = 32;
        unsigned char out[KEY_SIZE];

        CHMAC_SHA256(reinterpret_cast<const unsigned char*>(strSalt.c_str()), strSalt.size()).Write(reinterpret_cast<const unsigned char*>(strPass.c_str()), strPass.size()).Finalize(out);
        std::vector<unsigned char> hexvec(out, out+KEY_SIZE);
        std::string strHashFromPass = HexStr(hexvec);

        if (TimingResistantEqual(strHashFromPass, strHash)) {
            return true;
        }
    }
    return false;
}

static bool RPCAuthorized(const std::string& strAuth, std::string& strAuthUsernameOut)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (strRPCUserColonPass.empty()) // Belt-and-suspenders measure if InitRPCAuthentication was not called
        return false;
    if (strAuth.substr(0, 6) != "Basic ")
        return false;
    std::string strUserPass64 = strAuth.substr(6);
    boost::trim(strUserPass64);
    std::string strUserPass = DecodeBase64(strUserPass64);
<<<<<<< HEAD
    return TimingResistantEqual(strUserPass, strRPCUserColonPass);
=======

    if (strUserPass.find(':') != std::string::npos)
        strAuthUsernameOut = strUserPass.substr(0, strUserPass.find(':'));

    //Check if authorized under single-user field
    if (TimingResistantEqual(strUserPass, strRPCUserColonPass)) {
        return true;
    }
    return multiUserAuthorized(strUserPass);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

static bool HTTPReq_JSONRPC(HTTPRequest* req, const std::string &)
{
    // JSONRPC handles only POST
    if (req->GetRequestMethod() != HTTPRequest::POST) {
        req->WriteReply(HTTP_BAD_METHOD, "JSONRPC server handles only POST requests");
        return false;
    }
    // Check authorization
    std::pair<bool, std::string> authHeader = req->GetHeader("authorization");
    if (!authHeader.first) {
        req->WriteReply(HTTP_UNAUTHORIZED);
        return false;
    }

<<<<<<< HEAD
    if (!RPCAuthorized(authHeader.second)) {
=======
    JSONRPCRequest jreq;
    if (!RPCAuthorized(authHeader.second, jreq.authUser)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        LogPrintf("ThreadRPCServer incorrect password attempt from %s\n", req->GetPeer().ToString());

        /* Deter brute-forcing
           If this results in a DoS the user really
           shouldn't have their RPC port exposed. */
        MilliSleep(250);

        req->WriteReply(HTTP_UNAUTHORIZED);
        return false;
    }

<<<<<<< HEAD
    JSONRequest jreq;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    try {
        // Parse request
        UniValue valRequest;
        if (!valRequest.read(req->ReadBody()))
            throw JSONRPCError(RPC_PARSE_ERROR, "Parse error");

<<<<<<< HEAD
=======
        // Set the URI
        jreq.URI = req->GetURI();

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        std::string strReply;
        // singleton request
        if (valRequest.isObject()) {
            jreq.parse(valRequest);

<<<<<<< HEAD
            UniValue result = tableRPC.execute(jreq.strMethod, jreq.params);
=======
            UniValue result = tableRPC.execute(jreq);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            // Send reply
            strReply = JSONRPCReply(result, NullUniValue, jreq.id);

        // array of requests
        } else if (valRequest.isArray())
            strReply = JSONRPCExecBatch(valRequest.get_array());
        else
            throw JSONRPCError(RPC_PARSE_ERROR, "Top-level object parse error");

        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strReply);
    } catch (const UniValue& objError) {
        JSONErrorReply(req, objError, jreq.id);
        return false;
    } catch (const std::exception& e) {
        JSONErrorReply(req, JSONRPCError(RPC_PARSE_ERROR, e.what()), jreq.id);
        return false;
    }
    return true;
}

static bool InitRPCAuthentication()
{
<<<<<<< HEAD
    if (mapArgs["-rpcpassword"] == "")
=======
    if (gArgs.GetArg("-rpcpassword", "") == "")
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LogPrintf("No rpcpassword set - using random cookie authentication\n");
        if (!GenerateAuthCookie(&strRPCUserColonPass)) {
            uiInterface.ThreadSafeMessageBox(
                _("Error: A fatal internal error occurred, see debug.log for details"), // Same message as AbortNode
                "", CClientUIInterface::MSG_ERROR);
            return false;
        }
    } else {
<<<<<<< HEAD
        strRPCUserColonPass = mapArgs["-rpcuser"] + ":" + mapArgs["-rpcpassword"];
=======
        LogPrintf("Config options rpcuser and rpcpassword will soon be deprecated. Locally-run instances may remove rpcuser to use cookie-based auth, or may be replaced with rpcauth. Please see share/rpcuser for rpcauth auth generation.\n");
        strRPCUserColonPass = gArgs.GetArg("-rpcuser", "") + ":" + gArgs.GetArg("-rpcpassword", "");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
    return true;
}

bool StartHTTPRPC()
{
<<<<<<< HEAD
    LogPrint("rpc", "Starting HTTP RPC server\n");
=======
    LogPrint(BCLog::RPC, "Starting HTTP RPC server\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!InitRPCAuthentication())
        return false;

    RegisterHTTPHandler("/", true, HTTPReq_JSONRPC);
<<<<<<< HEAD

    assert(EventBase());
    httpRPCTimerInterface = new HTTPRPCTimerInterface(EventBase());
    RPCSetTimerInterface(httpRPCTimerInterface);
=======
#ifdef ENABLE_WALLET
    // ifdef can be removed once we switch to better endpoint support and API versioning
    RegisterHTTPHandler("/wallet/", false, HTTPReq_JSONRPC);
#endif
    assert(EventBase());
    httpRPCTimerInterface = std::make_unique<HTTPRPCTimerInterface>(EventBase());
    RPCSetTimerInterface(httpRPCTimerInterface.get());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return true;
}

void InterruptHTTPRPC()
{
<<<<<<< HEAD
    LogPrint("rpc", "Interrupting HTTP RPC server\n");
=======
    LogPrint(BCLog::RPC, "Interrupting HTTP RPC server\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void StopHTTPRPC()
{
<<<<<<< HEAD
    LogPrint("rpc", "Stopping HTTP RPC server\n");
    UnregisterHTTPHandler("/", true);
    if (httpRPCTimerInterface) {
        RPCUnsetTimerInterface(httpRPCTimerInterface);
        delete httpRPCTimerInterface;
        httpRPCTimerInterface = 0;
=======
    LogPrint(BCLog::RPC, "Stopping HTTP RPC server\n");
    UnregisterHTTPHandler("/", true);
    if (httpRPCTimerInterface) {
        RPCUnsetTimerInterface(httpRPCTimerInterface.get());
        httpRPCTimerInterface.reset();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}
