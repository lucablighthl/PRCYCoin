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

#include "rpc/client.h"

<<<<<<< HEAD
#include "rpc/protocol.h"
#include "guiinterface.h"
#include "util.h"

#include <set>
#include <stdint.h>

#include <boost/algorithm/string/case_conv.hpp> // for to_lower()
#include <univalue.h>

using namespace std;
=======
#include <set>
#include <stdint.h>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CRPCConvertParam
{
public:
    std::string methodName; //! method whose params want conversion
    int paramIdx;           //! 0-based idx of param to convert
<<<<<<< HEAD
};
static const CRPCConvertParam vRPCConvertParams[] =
    {
        {"stop", 0},
        {"setmocktime", 0},
        {"getaddednodeinfo", 0},
        {"setgenerate", 0},
        {"setgenerate", 1},
        {"generatepoa", 0},
        {"generatepoa", 1},
        {"getnetworkhashps", 0},
        {"getnetworkhashps", 1},
        {"sendtoaddress", 1},
        {"rescan", 0},
        {"rescanwallettransactions", 0},
        {"sendtostealthaddress", 1},
        {"sendtoaddressix", 1},
        {"settxfee", 0},
        {"getreceivedbyaddress", 1},
        {"getreceivedbyaccount", 1},
        {"listreceivedbyaddress", 0},
        {"listreceivedbyaddress", 1},
        {"listreceivedbyaddress", 2},
        {"listreceivedbyaccount", 0},
        {"listreceivedbyaccount", 1},
        {"listreceivedbyaccount", 2},
        {"getbalance", 1},
        {"getbalances", 0},
        {"getbalance", 2},
        {"getblockhash", 0},
        { "waitforblockheight", 0 },
        { "waitforblockheight", 1 },
        { "waitforblock", 1 },
        { "waitforblock", 2 },
        { "waitfornewblock", 0 },
        { "waitfornewblock", 1 },
        {"setmaxreorgdepth", 0},
        {"resyncfrom", 0},
        {"setdecoyconfirmation", 0},
        {"getrawtransactionbyblockheight", 0},
        {"move", 2},
        {"move", 3},
        {"sendfrom", 2},
        {"sendfrom", 3},
        {"listtransactions", 1},
        {"listtransactions", 2},
        {"listtransactions", 3},
        {"listaccounts", 0},
        {"listaccounts", 1},
        {"unlockwallet", 1},
        {"unlockwallet", 2},
        {"getblocktemplate", 0},
        {"getpoablocktemplate", 0},
        {"setminingnbits", 0},
        {"setminingnbits", 1},
        {"generateintegratedaddress", 0},
        {"generateintegratedaddress", 1},
        {"listsinceblock", 1},
        {"listsinceblock", 2},
        {"sendmany", 1},
        {"sendmany", 2},
        {"addmultisigaddress", 0},
        {"addmultisigaddress", 1},
        {"createmultisig", 0},
        {"createmultisig", 1},
        {"listunspent", 0},
        {"listunspent", 1},
        {"listunspent", 2},
        {"getblock", 1},
        {"getblockheader", 1},
        {"gettransaction", 1},
        {"getrawtransaction", 1},
        {"createrawtransaction", 0},
        {"createrawtransaction", 1},
        {"signrawtransaction", 1},
        {"signrawtransaction", 2},
        {"sendrawtransaction", 1},
        {"gettxout", 1},
        {"gettxout", 2},
        {"lockunspent", 0},
        {"lockunspent", 1},
        {"importprivkey", 2},
        {"importaddress", 2},
        {"verifychain", 0},
        {"verifychain", 1},
        {"keypoolrefill", 0},
        {"getrawmempool", 0},
        {"estimatefee", 0},
        {"getfeeinfo", 0},
        {"estimatepriority", 0},
        {"prioritisetransaction", 1},
        {"prioritisetransaction", 2},
        {"setban", 2},
        {"setban", 3},
        {"preparebudget", 2},
        {"preparebudget", 3},
        {"preparebudget", 5},
        {"submitbudget", 2},
        {"submitbudget", 3},
        {"submitbudget", 5},
        {"submitbudget", 7},
        // disabled until removal of the legacy 'masternode' command
        //{"startmasternode", 1},
        {"mnvoteraw", 1},
        {"mnvoteraw", 4},
        {"reservebalance", 0},
        {"reservebalance", 1},
        {"setstakesplitthreshold", 0},
        {"autocombinedust", 0},
        {"autocombinedust", 1}
    };
=======
    std::string paramName; //!< parameter name
};

/**
 * Specifiy a (method, idx, name) here if the argument is a non-string RPC
 * argument and needs to be converted from JSON.
 *
 * @note Parameter indexes start from 0.
 */
static const CRPCConvertParam vRPCConvertParams[] = {
    { "addmultisigaddress", 0, "nrequired" },
    { "addmultisigaddress", 1, "keys" },
    { "addpeeraddress", 1, "port" },
    { "autocombinerewards", 0, "enable" },
    { "autocombinerewards", 1, "threshold" },
    { "cleanbudget", 0, "try_sync" },
    { "createmultisig", 0, "nrequired" },
    { "createmultisig", 1, "keys" },
    { "createrawtransaction", 0, "inputs" },
    { "createrawtransaction", 1, "outputs" },
    { "createrawtransaction", 2, "locktime" },
    {"createrawmnfinalbudget", 1, "blockstart"},
    {"createrawmnfinalbudget", 2, "proposals"},
    { "delegatestake", 1, "amount" },
    { "delegatestake", 3, "ext_owner" },
    { "delegatestake", 4, "include_delegated" },
    { "delegatestake", 5, "from_shield" },
    { "estimatefee", 0, "nblocks" },
    { "estimatesmartfee", 0, "nblocks" },
    { "fundrawtransaction", 1, "options" },
    { "generate", 0, "nblocks" },
    { "generatetoaddress", 0, "nblocks" },
    { "getaddednodeinfo", 0, "dummy" },
    { "getbalance", 0, "minconf" },
    { "getbalance", 1, "include_watchonly" },
    { "getbalance", 2, "include_delegated" },
    { "getbalance", 3, "include_shield" },
    { "getblock", 1, "verbose" },
    { "getblockhash", 0, "height" },
    { "getblockheader", 1, "verbose" },
    { "getblockindexstats", 0, "height" },
    { "getblockindexstats", 1, "range" },
    { "getblocktemplate", 0, "template_request" },
    { "getfeeinfo", 0, "blocks" },
    { "getshieldbalance", 1, "minconf" },
    { "getshieldbalance", 2, "include_watchonly" },
    { "getnetworkhashps", 0, "nblocks" },
    { "getnetworkhashps", 1, "height" },
    { "getnodeaddresses", 0, "count" },
    { "getrawmempool", 0, "verbose" },
    { "getrawtransaction", 1, "verbose" },
    { "getreceivedbyaddress", 1, "minconf" },
    { "getreceivedbylabel", 1, "minconf" },
    { "getsaplingnotescount", 0, "minconf" },
    { "getsupplyinfo", 0, "force_update" },
    { "gettransaction", 1, "include_watchonly" },
    { "gettxout", 1, "n" },
    { "gettxout", 2, "include_mempool" },
    { "importaddress", 2, "rescan" },
    { "importaddress", 3, "p2sh" },
    { "importmulti", 0, "requests" },
    { "importmulti", 1, "options" },
    { "importprivkey", 2, "rescan" },
    { "importprivkey", 3, "is_staking_address" },
    { "importpubkey", 2, "rescan" },
    { "importsaplingkey", 1, "rescan" },
    { "importsaplingkey", 2, "height" },
    { "importsaplingviewingkey", 1, "rescan" },
    { "importsaplingviewingkey", 2, "height" },
    { "initmasternode", 2, "deterministic" },
    { "keypoolrefill", 0, "newsize" },
    { "listcoldutxos", 0, "not_whitelisted" },
    { "listdelegators", 0, "blacklist" },
    { "listreceivedbyaddress", 0, "minconf" },
    { "listreceivedbyaddress", 1, "include_empty" },
    { "listreceivedbyaddress", 2, "include_watchonly" },
    { "listreceivedbylabel", 0, "minconf" },
    { "listreceivedbylabel", 1, "include_empty" },
    { "listreceivedbylabel", 2, "include_watchonly" },
    { "listreceivedbyshieldaddress", 1, "minconf" },
    { "listshieldunspent", 0, "minconf" },
    { "listshieldunspent", 1, "maxconf" },
    { "listshieldunspent", 2, "include_watchonly" },
    { "listshieldunspent", 3, "addresses" },
    { "listsinceblock", 1, "target_confirmations" },
    { "listsinceblock", 2, "include_watchonly" },
    { "listtransactions", 1, "count" },
    { "listtransactions", 2, "from" },
    { "listtransactions", 3, "include_watchonly" },
    { "listtransactions", 4, "include_delegated" },
    { "listtransactions", 5, "include_cold" },
    { "listunspent", 0, "minconf" },
    { "listunspent", 1, "maxconf" },
    { "listunspent", 2, "addresses" },
    { "listunspent", 3, "watchonly_config" },
    { "listunspent", 4, "query_options" },
    { "listunspent", 5, "include_unsafe" },
    { "lockunspent", 0, "unlock" },
    { "lockunspent", 1, "transactions" },
    { "logging", 0, "include" },
    { "logging", 1, "exclude" },
    { "mnbudgetvote", 4, "legacy" },
    { "mnbudgetrawvote", 1, "collat_vout" },
    { "mnbudgetrawvote", 4, "time" },
    { "preparebudget", 2, "npayments" },
    { "preparebudget", 3, "start" },
    { "preparebudget", 5, "montly_payment" },
    { "prioritisetransaction", 1, "fee_delta" },
    { "rawdelegatestake", 1, "amount" },
    { "rawdelegatestake", 3, "ext_owner" },
    { "rawdelegatestake", 4, "include_delegated" },
    { "rawdelegatestake", 5, "from_shield" },
    { "rawdelegatestake", 6, "force" },
    { "rawshieldsendmany", 1, "amounts" },
    { "rawshieldsendmany", 2, "minconf" },
    { "rawshieldsendmany", 3, "fee" },
    { "rescanblockchain", 0, "start_height"},
    { "rescanblockchain", 1, "stop_height"},
    { "sendmany", 1, "amounts" },
    { "sendmany", 2, "minconf" },
    { "sendmany", 5, "subtract_fee_from" },
    { "sendrawtransaction", 1, "allowhighfees" },
    { "sendtoaddress", 1, "amount" },
    { "sendtoaddress", 4, "subtract_fee" },
    { "setautocombinethreshold", 0, "enable" },
    { "setautocombinethreshold", 1, "threshold" },
    { "setnetworkactive", 0, "active"},
    { "setban", 2, "bantime" },
    { "setban", 3, "absolute" },
    { "setgenerate", 0, "generate" },
    { "setgenerate", 1, "genproclimit" },
    { "sethdseed", 0, "newkeypool" },
    { "setmocktime", 0, "timestamp" },
    { "setstakesplitthreshold", 0, "value" },
    { "settxfee", 0, "amount" },
    { "shieldsendmany", 1, "amounts" },
    { "shieldsendmany", 2, "minconf" },
    { "shieldsendmany", 3, "fee" },
    { "shieldsendmany", 4, "subtract_fee_from" },
    { "signrawtransaction", 1, "prevtxs" },
    { "signrawtransaction", 2, "privkeys" },
    { "spork", 1, "value" },
    { "startmasternode", 3, "lockwallet" },
    { "submitbudget", 2, "npayments" },
    { "submitbudget", 3, "start" },
    { "submitbudget", 5, "montly_payment" },
    { "verifychain", 0, "nblocks" },
    { "waitforblock", 1, "timeout" },
    { "waitforblockheight", 0, "height" },
    { "waitforblockheight", 1, "timeout" },
    { "waitfornewblock", 0, "timeout" },
    { "walletpassphrase", 1, "timeout" },
    { "walletpassphrase", 2, "staking_only" },
    // Echo with conversion (For testing only)
    { "echojson", 0, "arg0" },
    { "echojson", 1, "arg1" },
    { "echojson", 2, "arg2" },
    { "echojson", 3, "arg3" },
    { "echojson", 4, "arg4" },
    { "echojson", 5, "arg5" },
    { "echojson", 6, "arg6" },
    { "echojson", 7, "arg7" },
    { "echojson", 8, "arg8" },
    { "echojson", 9, "arg9" },
};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CRPCConvertTable
{
private:
<<<<<<< HEAD
    std::set<std::pair<std::string, int> > members;
=======
    std::set<std::pair<std::string, int>> members;
    std::set<std::pair<std::string, std::string>> membersByName;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

public:
    CRPCConvertTable();

    bool convert(const std::string& method, int idx)
    {
        return (members.count(std::make_pair(method, idx)) > 0);
    }
<<<<<<< HEAD
=======
    bool convert(const std::string& method, const std::string& name)
    {
        return (membersByName.count(std::make_pair(method, name)) > 0);
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

CRPCConvertTable::CRPCConvertTable()
{
    const unsigned int n_elem =
        (sizeof(vRPCConvertParams) / sizeof(vRPCConvertParams[0]));

    for (unsigned int i = 0; i < n_elem; i++) {
        members.insert(std::make_pair(vRPCConvertParams[i].methodName,
<<<<<<< HEAD
            vRPCConvertParams[i].paramIdx));
=======
                                      vRPCConvertParams[i].paramIdx));
        membersByName.insert(std::make_pair(vRPCConvertParams[i].methodName,
                                            vRPCConvertParams[i].paramName));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

static CRPCConvertTable rpcCvtTable;

/** Non-RFC4627 JSON parser, accepts internal values (such as numbers, true, false, null)
 * as well as objects and arrays.
 */
UniValue ParseNonRFCJSONValue(const std::string& strVal)
{
    UniValue jVal;
    if (!jVal.read(std::string("[")+strVal+std::string("]")) ||
        !jVal.isArray() || jVal.size()!=1)
<<<<<<< HEAD
        throw runtime_error(string("Error parsing JSON:")+strVal);
    return jVal[0];
}

/** Convert strings to command-specific RPC representation */
=======
        throw std::runtime_error(std::string("Error parsing JSON:")+strVal);
    return jVal[0];
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
UniValue RPCConvertValues(const std::string &strMethod, const std::vector<std::string> &strParams)
{
    UniValue params(UniValue::VARR);

    for (unsigned int idx = 0; idx < strParams.size(); idx++) {
        const std::string& strVal = strParams[idx];

        if (!rpcCvtTable.convert(strMethod, idx)) {
            // insert string value directly
            params.push_back(strVal);
        } else {
            // parse string as JSON, insert bool/number/object/etc. value
            params.push_back(ParseNonRFCJSONValue(strVal));
        }
    }

    return params;
}
<<<<<<< HEAD
=======

UniValue RPCConvertNamedValues(const std::string &strMethod, const std::vector<std::string> &strParams)
{
    UniValue params(UniValue::VOBJ);

    for (const std::string &s: strParams) {
        size_t pos = s.find('=');
        if (pos == std::string::npos) {
            throw(std::runtime_error("No '=' in named argument '"+s+"', this needs to be present for every argument (even if it is empty)"));
        }

        std::string name = s.substr(0, pos);
        std::string value = s.substr(pos+1);

        if (!rpcCvtTable.convert(strMethod, name)) {
            // insert string value directly
            params.pushKV(name, value);
        } else {
            // parse string as JSON, insert bool/number/object/etc. value
            params.pushKV(name, ParseNonRFCJSONValue(value));
        }
    }

    return params;
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
