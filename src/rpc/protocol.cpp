// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc/protocol.h"

#include "random.h"
#include "tinyformat.h"
<<<<<<< HEAD
#include "util.h"
#include "utilstrencodings.h"
#include "utiltime.h"
#include "version.h"

#include <stdint.h>
#include <fstream>

using namespace std;


/**
 * JSON-RPC protocol.  PRCY speaks version 1.0 for maximum compatibility,
 * but uses JSON-RPC 1.1/2.0 standards for parts of the 1.0 standard that were
 * unspecified (HTTP errors and contents of 'error').
 * 
=======
#include "util/system.h"
#include "utilstrencodings.h"
#include "utiltime.h"

/**
 * JSON-RPC protocol.  PIVX speaks version 1.0 for maximum compatibility,
 * but uses JSON-RPC 1.1/2.0 standards for parts of the 1.0 standard that were
 * unspecified (HTTP errors and contents of 'error').
 *
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 * 1.0 spec: http://json-rpc.org/wiki/specification
 * 1.2 spec: http://jsonrpc.org/historical/json-rpc-over-http.html
 * http://www.codeproject.com/KB/recipes/JSON_Spirit.aspx
 */

<<<<<<< HEAD
string JSONRPCRequest(const string& strMethod, const UniValue& params, const UniValue& id)
{
    UniValue request(UniValue::VOBJ);
    request.push_back(Pair("method", strMethod));
    request.push_back(Pair("params", params));
    request.push_back(Pair("id", id));
    return request.write() + "\n";
=======
UniValue JSONRPCRequestObj(const std::string& strMethod, const UniValue& params, const UniValue& id)
{
    UniValue request(UniValue::VOBJ);
    request.pushKV("method", strMethod);
    request.pushKV("params", params);
    request.pushKV("id", id);
    return request;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

UniValue JSONRPCReplyObj(const UniValue& result, const UniValue& error, const UniValue& id)
{
    UniValue reply(UniValue::VOBJ);
    if (!error.isNull())
<<<<<<< HEAD
        reply.push_back(Pair("result", NullUniValue));
    else
        reply.push_back(Pair("result", result));
    reply.push_back(Pair("error", error));
    reply.push_back(Pair("id", id));
    return reply;
}

string JSONRPCReply(const UniValue& result, const UniValue& error, const UniValue& id)
=======
        reply.pushKV("result", NullUniValue);
    else
        reply.pushKV("result", result);
    reply.pushKV("error", error);
    reply.pushKV("id", id);
    return reply;
}

std::string JSONRPCReply(const UniValue& result, const UniValue& error, const UniValue& id)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    UniValue reply = JSONRPCReplyObj(result, error, id);
    return reply.write() + "\n";
}

<<<<<<< HEAD
UniValue JSONRPCError(int code, const string& message)
{
    UniValue error(UniValue::VOBJ);
    error.push_back(Pair("code", code));
    error.push_back(Pair("message", message));
    return error;
}


=======
UniValue JSONRPCError(int code, const std::string& message)
{
    UniValue error(UniValue::VOBJ);
    error.pushKV("code", code);
    error.pushKV("message", message);
    return error;
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/** Username used when cookie authentication is in use (arbitrary, only for
 * recognizability in debugging/logging purposes)
 */
static const std::string COOKIEAUTH_USER = "__cookie__";
/** Default name for auth cookie file */
static const std::string COOKIEAUTH_FILE = ".cookie";

<<<<<<< HEAD
boost::filesystem::path GetAuthCookieFile()
{
    boost::filesystem::path path(GetArg("-rpccookiefile", COOKIEAUTH_FILE));
    if (!path.is_complete()) path = GetDataDir() / path;
    return path;
=======
fs::path GetAuthCookieFile()
{
    fs::path path(gArgs.GetArg("-rpccookiefile", COOKIEAUTH_FILE));
    return AbsPathForConfigVal(path);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool GenerateAuthCookie(std::string *cookie_out)
{
<<<<<<< HEAD
    unsigned char rand_pwd[32];
    GetRandBytes(rand_pwd, 32);
    std::string cookie = COOKIEAUTH_USER + ":" + EncodeBase64(&rand_pwd[0],32);
=======
    const size_t COOKIE_SIZE = 32;
    unsigned char rand_pwd[COOKIE_SIZE];
    GetRandBytes(rand_pwd, COOKIE_SIZE);
    std::string cookie = COOKIEAUTH_USER + ":" + HexStr(rand_pwd);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /** the umask determines what permissions are used to create this file -
     * these are set to 077 in init.cpp unless overridden with -sysperms.
     */
<<<<<<< HEAD
    std::ofstream file;
    boost::filesystem::path filepath = GetAuthCookieFile();
    file.open(filepath.string().c_str());
    if (!file.is_open()) {
        LogPrintf("Unable to open cookie authentication file %s for writing\n", filepath.string());
=======
    fsbridge::ofstream file;
    fs::path filepath_tmp = GetAuthCookieFile();
    file.open(filepath_tmp);
    if (!file.is_open()) {
        LogPrintf("Unable to open cookie authentication file %s for writing\n", filepath_tmp.string());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    }
    file << cookie;
    file.close();
<<<<<<< HEAD
    LogPrintf("Generated RPC authentication cookie %s\n", filepath.string());
=======
    LogPrintf("Generated RPC authentication cookie %s\n", filepath_tmp.string());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (cookie_out)
        *cookie_out = cookie;
    return true;
}

bool GetAuthCookie(std::string *cookie_out)
{
<<<<<<< HEAD
    std::ifstream file;
    std::string cookie;
    boost::filesystem::path filepath = GetAuthCookieFile();
    file.open(filepath.string().c_str());
=======
    fsbridge::ifstream file;
    std::string cookie;
    fs::path filepath = GetAuthCookieFile();
    file.open(filepath);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (!file.is_open())
        return false;
    std::getline(file, cookie);
    file.close();

    if (cookie_out)
        *cookie_out = cookie;
    return true;
}

void DeleteAuthCookie()
{
    try {
<<<<<<< HEAD
        boost::filesystem::remove(GetAuthCookieFile());
    } catch (const boost::filesystem::filesystem_error& e) {
        LogPrintf("%s: Unable to remove random auth cookie file: %s\n", __func__, e.what());
=======
        fs::remove(GetAuthCookieFile());
    } catch (const fs::filesystem_error& e) {
        LogPrintf("%s: Unable to remove random auth cookie file: %s\n", __func__, fsbridge::get_filesystem_error_message(e));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}
