// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
=======
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chain.h"
<<<<<<< HEAD
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "main.h"
=======
#include "core_io.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "httpserver.h"
#include "rpc/server.h"
#include "streams.h"
#include "sync.h"
#include "txmempool.h"
#include "utilstrencodings.h"
<<<<<<< HEAD
#include "version.h"
#include <univalue.h>

#include <boost/algorithm/string.hpp>
#include <boost/dynamic_bitset.hpp>

static const size_t MAX_GETUTXOS_OUTPOINTS = 15; //allow a max of 15 outpoints to be queried at once

using namespace std;
=======
#include "validation.h"
#include "version.h"
#include "wallet/wallet.h"

#include <boost/algorithm/string.hpp>

#include <univalue.h>


static const size_t MAX_GETUTXOS_OUTPOINTS = 15; //allow a max of 15 outpoints to be queried at once
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

enum RetFormat {
    RF_UNDEF,
    RF_BINARY,
    RF_HEX,
    RF_JSON,
};

static const struct {
    enum RetFormat rf;
<<<<<<< HEAD
    const char *name;
} rf_names[] = {
        {RF_UNDEF,  ""},
        {RF_BINARY, "bin"},
        {RF_HEX,    "hex"},
        {RF_JSON,   "json"},
};

struct CCoin {
    uint32_t nTxVer; // Don't call this nVersion, that name has a special meaning inside IMPLEMENT_SERIALIZE
    uint32_t nHeight;
    CTxOut out;

    ADD_SERIALIZE_METHODS;

    template<typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action, int nType, int nVersion) {
        READWRITE(nTxVer);
        READWRITE(nHeight);
        READWRITE(out);
    }
};


extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, UniValue& entry);

extern UniValue blockToJSON(const CBlock& block, const CBlockIndex* blockindex, bool txDetails = false);

extern UniValue mempoolInfoToJSON();

extern UniValue mempoolToJSON(bool fVerbose = false);

extern void ScriptPubKeyToJSON(const CScript &scriptPubKey, UniValue &out, bool fIncludeHex);

extern UniValue blockheaderToJSON(const CBlockIndex *blockindex);

static bool RESTERR(HTTPRequest *req, enum HTTPStatusCode status, string message) {
=======
    const char* name;
} rf_names[] = {
      {RF_UNDEF, ""},
      {RF_BINARY, "bin"},
      {RF_HEX, "hex"},
      {RF_JSON, "json"},
};

struct CCoin {
    uint32_t nHeight;
    CTxOut out;

    CCoin() : nHeight(0) {}
    CCoin(Coin&& in) : nHeight(in.nHeight), out(std::move(in.out)) {}

    SERIALIZE_METHODS(CCoin, obj)
    {
        uint32_t nTxVerDummy = 0;
        READWRITE(nTxVerDummy, obj.nHeight, obj.out);
    }
};

extern void TxToJSON(CWallet* const pwallet, const CTransaction& tx, const uint256 hashBlock, UniValue& entry);
extern UniValue blockToJSON(const CBlock& block, const CBlockIndex* blockindex, bool txDetails = false);
extern UniValue mempoolInfoToJSON();
extern UniValue mempoolToJSON(bool fVerbose = false);
extern UniValue blockheaderToJSON(const CBlockIndex* blockindex);

static bool RESTERR(HTTPRequest* req, enum HTTPStatusCode status, std::string message)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    req->WriteHeader("Content-Type", "text/plain");
    req->WriteReply(status, message + "\r\n");
    return false;
}

<<<<<<< HEAD
static enum RetFormat ParseDataFormat(vector <string> &params, const string &strReq) {
=======
static enum RetFormat ParseDataFormat(std::vector<std::string>& params, const std::string& strReq)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    boost::split(params, strReq, boost::is_any_of("."));
    if (params.size() > 1) {
        for (unsigned int i = 0; i < ARRAYLEN(rf_names); i++)
            if (params[1] == rf_names[i].name)
                return rf_names[i].rf;
    }

    return rf_names[0].rf;
}

<<<<<<< HEAD
static string AvailableDataFormatsString() {
    string formats = "";
=======
static std::string AvailableDataFormatsString()
{
    std::string formats = "";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (unsigned int i = 0; i < ARRAYLEN(rf_names); i++)
        if (strlen(rf_names[i].name) > 0) {
            formats.append(".");
            formats.append(rf_names[i].name);
            formats.append(", ");
        }

    if (formats.length() > 0)
        return formats.substr(0, formats.length() - 2);

    return formats;
}

<<<<<<< HEAD
static bool ParseHashStr(const string &strReq, uint256 &v) {
    if (!IsHex(strReq) || (strReq.size() != 64))
        return false;

    v.SetHex(strReq);
    return true;
}

static bool CheckWarmup(HTTPRequest *req) {
    std::string statusmessage;
    if (RPCIsInWarmup(&statusmessage))
        return RESTERR(req, HTTP_SERVICE_UNAVAILABLE, "Service temporarily unavailable: " + statusmessage);
    return true;
}

static bool rest_headers(HTTPRequest *req,
                         const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;

    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);
    vector <string> path;
    boost::split(path, params[0], boost::is_any_of("/"));
=======
static bool CheckWarmup(HTTPRequest* req)
{
    std::string statusmessage;
    if (RPCIsInWarmup(&statusmessage))
         return RESTERR(req, HTTP_SERVICE_UNAVAILABLE, "Service temporarily unavailable: " + statusmessage);
    return true;
}

static bool rest_headers(HTTPRequest* req,
                         const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);
    std::vector<std::string> path;
    boost::split(path, params[0], boost::is_any_of("/"));

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (path.size() != 2)
        return RESTERR(req, HTTP_BAD_REQUEST, "No header count specified. Use /rest/headers/<count>/<hash>.<ext>.");

    long count = strtol(path[0].c_str(), NULL, 10);
    if (count < 1 || count > 2000)
        return RESTERR(req, HTTP_BAD_REQUEST, "Header count out of range: " + path[0]);
<<<<<<< HEAD
    string hashStr = path[1];
    uint256 hash;

    if (!ParseHashStr(hashStr, hash))
        return RESTERR(req, HTTP_BAD_REQUEST, "Invalid hash: " + hashStr);
    std::vector<const CBlockIndex *> headers;
    headers.reserve(count);

=======

    std::string hashStr = path[1];
    uint256 hash;
    if (!ParseHashStr(hashStr, hash))
        return RESTERR(req, HTTP_BAD_REQUEST, "Invalid hash: " + hashStr);

    std::vector<const CBlockIndex *> headers;
    headers.reserve(count);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_main);
        BlockMap::const_iterator it = mapBlockIndex.find(hash);
        const CBlockIndex *pindex = (it != mapBlockIndex.end()) ? it->second : NULL;
        while (pindex != NULL && chainActive.Contains(pindex)) {
            headers.push_back(pindex);
<<<<<<< HEAD
            if (headers.size() == (unsigned long) count)
=======
            if (headers.size() == (unsigned long)count)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                break;
            pindex = chainActive.Next(pindex);
        }
    }

    CDataStream ssHeader(SER_NETWORK, PROTOCOL_VERSION);
    for (const CBlockIndex *pindex : headers) {
        ssHeader << pindex->GetBlockHeader();
    }

    switch (rf) {
<<<<<<< HEAD
        case RF_BINARY: {
            string binaryHeader = ssHeader.str();
            req->WriteHeader("Content-Type", "application/octet-stream");
            req->WriteReply(HTTP_OK, binaryHeader);
            return true;
        }

        case RF_HEX: {
            string strHex = HexStr(ssHeader.begin(), ssHeader.end()) + "\n";
            req->WriteHeader("Content-Type", "text/plain");
            req->WriteReply(HTTP_OK, strHex);
            return true;
        }
        case RF_JSON: {
            UniValue jsonHeaders(UniValue::VARR);
            for (const CBlockIndex *pindex : headers) {
                jsonHeaders.push_back(blockheaderToJSON(pindex));
            }
            string strJSON = jsonHeaders.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }
        default: {
            return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: .bin, .hex)");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static bool rest_block(HTTPRequest *req,
                       const std::string &strURIPart,
                       bool showTxDetails) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    string hashStr = params[0];
=======
    case RF_BINARY: {
        std::string binaryHeader = ssHeader.str();
        req->WriteHeader("Content-Type", "application/octet-stream");
        req->WriteReply(HTTP_OK, binaryHeader);
        return true;
    }

    case RF_HEX: {
        std::string strHex = HexStr(ssHeader) + "\n";
        req->WriteHeader("Content-Type", "text/plain");
        req->WriteReply(HTTP_OK, strHex);
        return true;
    }
    case RF_JSON: {
        UniValue jsonHeaders(UniValue::VARR);
        for (const CBlockIndex *pindex : headers) {
            jsonHeaders.push_back(blockheaderToJSON(pindex));
        }
        std::string strJSON = jsonHeaders.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: .bin, .hex)");
    }
    }
}

static bool rest_block(HTTPRequest* req,
                       const std::string& strURIPart,
                       bool showTxDetails)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    std::string hashStr = params[0];
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    uint256 hash;
    if (!ParseHashStr(hashStr, hash))
        return RESTERR(req, HTTP_BAD_REQUEST, "Invalid hash: " + hashStr);

    CBlock block;
<<<<<<< HEAD
    CBlockIndex *pblockindex = NULL;
=======
    CBlockIndex* pblockindex = NULL;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_main);
        if (mapBlockIndex.count(hash) == 0)
            return RESTERR(req, HTTP_NOT_FOUND, hashStr + " not found");

        pblockindex = mapBlockIndex[hash];
        if (!(pblockindex->nStatus & BLOCK_HAVE_DATA) && pblockindex->nTx > 0)
            return RESTERR(req, HTTP_NOT_FOUND, hashStr + " not available (pruned data)");

        if (!ReadBlockFromDisk(block, pblockindex))
            return RESTERR(req, HTTP_NOT_FOUND, hashStr + " not found");
    }

    CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
    ssBlock << block;

    switch (rf) {
<<<<<<< HEAD
        case RF_BINARY: {
            string binaryBlock = ssBlock.str();
            req->WriteHeader("Content-Type", "application/octet-stream");
            req->WriteReply(HTTP_OK, binaryBlock);
            return true;
        }

        case RF_HEX: {
            string strHex = HexStr(ssBlock.begin(), ssBlock.end()) + "\n";
            req->WriteHeader("Content-Type", "text/plain");
            req->WriteReply(HTTP_OK, strHex);
            return true;
        }

        case RF_JSON: {
            UniValue objTx(UniValue::VOBJ);
            string strJSON = objTx.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }

        default: {
            return RESTERR(req, HTTP_NOT_FOUND,
                           "output format not found (available: " + AvailableDataFormatsString() + ")");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static bool rest_block_extended(HTTPRequest *req, const std::string &strURIPart) {
    return rest_block(req, strURIPart, true);
}

static bool rest_block_notxdetails(HTTPRequest *req, const std::string &strURIPart) {
    return rest_block(req, strURIPart, false);
}

static bool rest_chaininfo(HTTPRequest *req, const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
        case RF_JSON: {
            UniValue rpcParams(UniValue::VARR);
            UniValue chainInfoObject = getblockchaininfo(rpcParams, false);
            string strJSON = chainInfoObject.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }
        default: {
            return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static bool rest_mempool_info(HTTPRequest *req, const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
        case RF_JSON: {
            UniValue mempoolInfoObject = mempoolInfoToJSON();

            string strJSON = mempoolInfoObject.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }
        default: {
            return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static bool rest_mempool_contents(HTTPRequest *req, const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
        case RF_JSON: {
            UniValue mempoolObject = mempoolToJSON(true);

            string strJSON = mempoolObject.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }
        default: {
            return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static bool rest_tx(HTTPRequest *req, const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    string hashStr = params[0];
=======
    case RF_BINARY: {
        std::string binaryBlock = ssBlock.str();
        req->WriteHeader("Content-Type", "application/octet-stream");
        req->WriteReply(HTTP_OK, binaryBlock);
        return true;
    }

    case RF_HEX: {
        std::string strHex = HexStr(ssBlock) + "\n";
        req->WriteHeader("Content-Type", "text/plain");
        req->WriteReply(HTTP_OK, strHex);
        return true;
    }

    case RF_JSON: {
        UniValue objBlock = blockToJSON(block, pblockindex, showTxDetails);
        std::string strJSON = objBlock.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }

    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: " + AvailableDataFormatsString() + ")");
    }
    }
}

static bool rest_block_extended(HTTPRequest* req, const std::string& strURIPart)
{
    return rest_block(req, strURIPart, true);
}

static bool rest_block_notxdetails(HTTPRequest* req, const std::string& strURIPart)
{
    return rest_block(req, strURIPart, false);
}

// A bit of a hack - dependency on a function defined in rpc/blockchain.cpp
UniValue getblockchaininfo(const JSONRPCRequest& request);

static bool rest_chaininfo(HTTPRequest* req, const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
    case RF_JSON: {
        JSONRPCRequest jsonRequest;
        jsonRequest.params = UniValue(UniValue::VARR);
        UniValue chainInfoObject = getblockchaininfo(jsonRequest);
        std::string strJSON = chainInfoObject.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
    }
    }
}

static bool rest_mempool_info(HTTPRequest* req, const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
    case RF_JSON: {
        UniValue mempoolInfoObject = mempoolInfoToJSON();

        std::string strJSON = mempoolInfoObject.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
    }
    }
}

static bool rest_mempool_contents(HTTPRequest* req, const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    switch (rf) {
    case RF_JSON: {
        UniValue mempoolObject = mempoolToJSON(true);

        std::string strJSON = mempoolObject.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: json)");
    }
    }
}

static bool rest_tx(HTTPRequest* req, const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    const RetFormat rf = ParseDataFormat(params, strURIPart);

    std::string hashStr = params[0];
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    uint256 hash;
    if (!ParseHashStr(hashStr, hash))
        return RESTERR(req, HTTP_BAD_REQUEST, "Invalid hash: " + hashStr);

<<<<<<< HEAD
    CTransaction tx;
=======
    CTransactionRef tx;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    uint256 hashBlock = uint256();
    if (!GetTransaction(hash, tx, hashBlock, true))
        return RESTERR(req, HTTP_NOT_FOUND, hashStr + " not found");

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    switch (rf) {
<<<<<<< HEAD
        case RF_BINARY: {
            string binaryTx = ssTx.str();
            req->WriteHeader("Content-Type", "application/octet-stream");
            req->WriteReply(HTTP_OK, binaryTx);
            return true;
        }

        case RF_HEX: {
            string strHex = HexStr(ssTx.begin(), ssTx.end()) + "\n";
            req->WriteHeader("Content-Type", "text/plain");
            req->WriteReply(HTTP_OK, strHex);
            return true;
        }

        case RF_JSON: {
            UniValue objTx(UniValue::VOBJ);
            TxToJSON(tx, hashBlock, objTx);
            string strJSON = objTx.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }

        default: {
            return RESTERR(req, HTTP_NOT_FOUND,
                           "output format not found (available: " + AvailableDataFormatsString() + ")");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}


static bool rest_getutxos(HTTPRequest *req, const std::string &strURIPart) {
    if (!CheckWarmup(req))
        return false;
    vector <string> params;
    enum RetFormat rf = ParseDataFormat(params, strURIPart);

    vector <string> uriParts;
    if (params.size() > 0 && params[0].length() > 1) {
=======
    case RF_BINARY: {
        std::string binaryTx = ssTx.str();
        req->WriteHeader("Content-Type", "application/octet-stream");
        req->WriteReply(HTTP_OK, binaryTx);
        return true;
    }

    case RF_HEX: {
        std::string strHex = HexStr(ssTx) + "\n";
        req->WriteHeader("Content-Type", "text/plain");
        req->WriteReply(HTTP_OK, strHex);
        return true;
    }

    case RF_JSON: {
        UniValue objTx(UniValue::VOBJ);
        TxToJSON(nullptr, *tx, hashBlock, objTx);
        std::string strJSON = objTx.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }

    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: " + AvailableDataFormatsString() + ")");
    }
    }
}

static bool rest_getutxos(HTTPRequest* req, const std::string& strURIPart)
{
    if (!CheckWarmup(req))
        return false;
    std::vector<std::string> params;
    enum RetFormat rf = ParseDataFormat(params, strURIPart);

    std::vector<std::string> uriParts;
    if (params.size() > 0 && params[0].length() > 1)
    {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        std::string strUriParams = params[0].substr(1);
        boost::split(uriParts, strUriParams, boost::is_any_of("/"));
    }

    // throw exception in case of a empty request
    std::string strRequestMutable = req->ReadBody();
    if (strRequestMutable.length() == 0 && uriParts.size() == 0)
        return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Error: empty request");

    bool fInputParsed = false;
    bool fCheckMemPool = false;
<<<<<<< HEAD
    vector <COutPoint> vOutPoints;
=======
    std::vector<COutPoint> vOutPoints;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // parse/deserialize input
    // input-format = output-format, rest/getutxos/bin requires binary input, gives binary output, ...

<<<<<<< HEAD
    if (uriParts.size() > 0) {

        //inputs is sent over URI scheme (/rest/getutxos/checkmempool/txid1-n/txid2-n/...)
        if (uriParts.size() > 0 && uriParts[0] == "checkmempool")
            fCheckMemPool = true;
=======
    if (uriParts.size() > 0)
    {
        //inputs is sent over URI scheme (/rest/getutxos/checkmempool/txid1-n/txid2-n/...)
        if (uriParts[0] == "checkmempool") fCheckMemPool = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        for (size_t i = (fCheckMemPool) ? 1 : 0; i < uriParts.size(); i++)
        {
            uint256 txid;
            int32_t nOutput;
<<<<<<< HEAD
            std::string strTxid = uriParts[i].substr(0, uriParts[i].find("-"));
            std::string strOutput = uriParts[i].substr(uriParts[i].find("-") + 1);
=======
            std::string strTxid = uriParts[i].substr(0, uriParts[i].find('-'));
            std::string strOutput = uriParts[i].substr(uriParts[i].find('-')+1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            if (!ParseInt32(strOutput, &nOutput) || !IsHex(strTxid))
                return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Parse error");

            txid.SetHex(strTxid);
<<<<<<< HEAD
            vOutPoints.push_back(COutPoint(txid, (uint32_t) nOutput));
=======
            vOutPoints.emplace_back(txid, (uint32_t)nOutput);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        if (vOutPoints.size() > 0)
            fInputParsed = true;
        else
            return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Error: empty request");
    }

    switch (rf) {
<<<<<<< HEAD
        case RF_HEX: {
            // convert hex to bin, continue then with bin part
            std::vector<unsigned char> strRequestV = ParseHex(strRequestMutable);
            strRequestMutable.assign(strRequestV.begin(), strRequestV.end());
        }

        case RF_BINARY: {
            try {
                //deserialize only if user sent a request
                if (strRequestMutable.size() > 0) {
                    if (fInputParsed) //don't allow sending input over URI and HTTP RAW DATA
                        return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR,
                                       "Combination of URI scheme inputs and raw post data is not allowed");

                    CDataStream oss(SER_NETWORK, PROTOCOL_VERSION);
                    oss << strRequestMutable;
                    oss >> fCheckMemPool;
                    oss >> vOutPoints;
                }
            } catch (const std::ios_base::failure& e) {
                // abort in case of unreadable binary data
                return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Parse error");
            }
            break;
        }

        case RF_JSON: {
            if (!fInputParsed)
                return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Error: empty request");
            break;
        }

        default: {
            return RESTERR(req, HTTP_NOT_FOUND,
                           "output format not found (available: " + AvailableDataFormatsString() + ")");
        }
=======
    case RF_HEX: {
        // convert hex to bin, continue then with bin part
        std::vector<unsigned char> strRequestV = ParseHex(strRequestMutable);
        strRequestMutable.assign(strRequestV.begin(), strRequestV.end());
    }

    case RF_BINARY: {
        try {
            //deserialize only if user sent a request
            if (strRequestMutable.size() > 0)
            {
                if (fInputParsed) //don't allow sending input over URI and HTTP RAW DATA
                    return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Combination of URI scheme inputs and raw post data is not allowed");

                CDataStream oss(SER_NETWORK, PROTOCOL_VERSION);
                oss << strRequestMutable;
                oss >> fCheckMemPool;
                oss >> vOutPoints;
            }
        } catch (const std::ios_base::failure& e) {
            // abort in case of unreadable binary data
            return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Parse error");
        }
        break;
    }

    case RF_JSON: {
        if (!fInputParsed)
            return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, "Error: empty request");
        break;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: " + AvailableDataFormatsString() + ")");
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // limit max outpoints
    if (vOutPoints.size() > MAX_GETUTXOS_OUTPOINTS)
<<<<<<< HEAD
        return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR,
                       strprintf("Error: max outpoints exceeded (max: %d, tried: %d)", MAX_GETUTXOS_OUTPOINTS,
                                 vOutPoints.size()));


    // check spentness and form a bitmap (as well as a JSON capable human-readble string representation)
    vector<unsigned char> bitmap;
    vector <CCoin> outs;
    std::string bitmapStringRepresentation;
    boost::dynamic_bitset<unsigned char> hits(vOutPoints.size());
=======
        return RESTERR(req, HTTP_INTERNAL_SERVER_ERROR, strprintf("Error: max outpoints exceeded (max: %d, tried: %d)", MAX_GETUTXOS_OUTPOINTS, vOutPoints.size()));

    // check spentness and form a bitmap (as well as a JSON capable human-readble string representation)
    std::vector<unsigned char> bitmap;
    std::vector<CCoin> outs;
    std::string bitmapStringRepresentation;
    std::vector<bool> hits;
    bitmap.resize((vOutPoints.size() + 7) / 8);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK2(cs_main, mempool.cs);

        CCoinsView viewDummy;
        CCoinsViewCache view(&viewDummy);

<<<<<<< HEAD
        CCoinsViewCache &viewChain = *pcoinsTip;
=======
        CCoinsViewCache& viewChain = *pcoinsTip;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        CCoinsViewMemPool viewMempool(&viewChain, mempool);

        if (fCheckMemPool)
            view.SetBackend(viewMempool); // switch cache backend to db+mempool in case user likes to query mempool

        for (size_t i = 0; i < vOutPoints.size(); i++) {
<<<<<<< HEAD
            CCoins coins;
            uint256 hash = vOutPoints[i].hash;
            if (view.GetCoins(hash, coins)) {
                mempool.pruneSpent(hash, coins);
                if (coins.IsAvailable(vOutPoints[i].n)) {
                    hits[i] = true;
                    // Safe to index into vout here because IsAvailable checked if it's off the end of the array, or if

                    // n is valid but points to an already spent output (IsNull).
                    CCoin coin;
                    coin.nTxVer = coins.nVersion;
                    coin.nHeight = coins.nHeight;
                    coin.out = coins.vout.at(vOutPoints[i].n);
                    assert(!coin.out.IsNull());
                    outs.push_back(coin);
                }
            }

            bitmapStringRepresentation.append(
                    hits[i] ? "1" : "0"); // form a binary string representation (human-readable for json output)
        }
    }
    boost::to_block_range(hits, std::back_inserter(bitmap));
    switch (rf) {
        case RF_BINARY: {
            // serialize data
            // use exact same output as mentioned in Bip64
            CDataStream ssGetUTXOResponse(SER_NETWORK, PROTOCOL_VERSION);
            ssGetUTXOResponse << chainActive.Height() << chainActive.Tip()->GetBlockHash() << bitmap << outs;
            string ssGetUTXOResponseString = ssGetUTXOResponse.str();

            req->WriteHeader("Content-Type", "application/octet-stream");
            req->WriteReply(HTTP_OK, ssGetUTXOResponseString);
            return true;
        }

        case RF_HEX: {
            CDataStream ssGetUTXOResponse(SER_NETWORK, PROTOCOL_VERSION);
            ssGetUTXOResponse << chainActive.Height() << chainActive.Tip()->GetBlockHash() << bitmap << outs;
            string strHex = HexStr(ssGetUTXOResponse.begin(), ssGetUTXOResponse.end()) + "\n";

            req->WriteHeader("Content-Type", "text/plain");
            req->WriteReply(HTTP_OK, strHex);
            return true;
        }

        case RF_JSON: {
            UniValue objGetUTXOResponse(UniValue::VOBJ);

            // pack in some essentials
            // use more or less the same output as mentioned in Bip64
            objGetUTXOResponse.push_back(Pair("chainHeight", chainActive.Height()));
            objGetUTXOResponse.push_back(Pair("chaintipHash", chainActive.Tip()->GetBlockHash().GetHex()));
            objGetUTXOResponse.push_back(Pair("bitmap", bitmapStringRepresentation));

            UniValue utxos(UniValue::VARR);
            for (  const CCoin &coin : outs) {
                UniValue utxo(UniValue::VOBJ);
                utxo.push_back(Pair("txvers", (int32_t) coin.nTxVer));
                utxo.push_back(Pair("height", (int32_t) coin.nHeight));
                utxo.push_back(Pair("value", ValueFromAmount(coin.out.nValue)));

                // include the script in a json output
                UniValue o(UniValue::VOBJ);
                ScriptPubKeyToJSON(coin.out.scriptPubKey, o, true);
                utxo.push_back(Pair("scriptPubKey", o));
                utxos.push_back(utxo);
            }
            objGetUTXOResponse.push_back(Pair("utxos", utxos));

            // return json string
            string strJSON = objGetUTXOResponse.write() + "\n";
            req->WriteHeader("Content-Type", "application/json");
            req->WriteReply(HTTP_OK, strJSON);
            return true;
        }
        default: {
            return RESTERR(req, HTTP_NOT_FOUND,  "output format not found (available: " + AvailableDataFormatsString() + ")");
        }
    }

    // not reached
    return true; // continue to process further HTTP reqs on this cxn
}

static const struct {
    const char *prefix;
    bool (*handler)(HTTPRequest* req, const std::string& strReq);
} uri_prefixes[] = {
        {"/rest/tx/", rest_tx},
        {"/rest/block/notxdetails/", rest_block_notxdetails},
        {"/rest/block/",             rest_block_extended},
        {"/rest/chaininfo", rest_chaininfo},
        {"/rest/mempool/info", rest_mempool_info},
        {"/rest/mempool/contents", rest_mempool_contents},
        {"/rest/headers/", rest_headers},
        {"/rest/getutxos", rest_getutxos},
=======
            bool hit = false;
            Coin coin;
            if (view.GetCoin(vOutPoints[i], coin) && !mempool.isSpent(vOutPoints[i])) {
                hit = true;
                outs.emplace_back(std::move(coin));
            }

            hits.push_back(hit);
            bitmapStringRepresentation.append(hit ? "1" : "0"); // form a binary string representation (human-readable for json output)
            bitmap[i / 8] |= ((uint8_t)hit) << (i % 8);
        }
    }

    switch (rf) {
    case RF_BINARY: {
        // serialize data
        // use exact same output as mentioned in Bip64
        CDataStream ssGetUTXOResponse(SER_NETWORK, PROTOCOL_VERSION);
        ssGetUTXOResponse << chainActive.Height() << chainActive.Tip()->GetBlockHash() << bitmap << outs;
        std::string ssGetUTXOResponseString = ssGetUTXOResponse.str();

        req->WriteHeader("Content-Type", "application/octet-stream");
        req->WriteReply(HTTP_OK, ssGetUTXOResponseString);
        return true;
    }

    case RF_HEX: {
        CDataStream ssGetUTXOResponse(SER_NETWORK, PROTOCOL_VERSION);
        ssGetUTXOResponse << chainActive.Height() << chainActive.Tip()->GetBlockHash() << bitmap << outs;
        std::string strHex = HexStr(ssGetUTXOResponse) + "\n";

        req->WriteHeader("Content-Type", "text/plain");
        req->WriteReply(HTTP_OK, strHex);
        return true;
    }

    case RF_JSON: {
        UniValue objGetUTXOResponse(UniValue::VOBJ);

        // pack in some essentials
        // use more or less the same output as mentioned in Bip64
        objGetUTXOResponse.pushKV("chainHeight", chainActive.Height());
        objGetUTXOResponse.pushKV("chaintipHash", chainActive.Tip()->GetBlockHash().GetHex());
        objGetUTXOResponse.pushKV("bitmap", bitmapStringRepresentation);

        UniValue utxos(UniValue::VARR);
        for (const CCoin& coin : outs) {
            UniValue utxo(UniValue::VOBJ);
            utxo.pushKV("height", (int32_t)coin.nHeight);
            utxo.pushKV("value", ValueFromAmount(coin.out.nValue));

            // include the script in a json output
            UniValue o(UniValue::VOBJ);
            ScriptPubKeyToUniv(coin.out.scriptPubKey, o, true);
            utxo.pushKV("scriptPubKey", o);
            utxos.push_back(utxo);
        }
        objGetUTXOResponse.pushKV("utxos", utxos);

        // return json string
        std::string strJSON = objGetUTXOResponse.write() + "\n";
        req->WriteHeader("Content-Type", "application/json");
        req->WriteReply(HTTP_OK, strJSON);
        return true;
    }
    default: {
        return RESTERR(req, HTTP_NOT_FOUND, "output format not found (available: " + AvailableDataFormatsString() + ")");
    }
    }
}

static const struct {
    const char* prefix;
    bool (*handler)(HTTPRequest* req, const std::string& strReq);
} uri_prefixes[] = {
      {"/rest/tx/", rest_tx},
      {"/rest/block/notxdetails/", rest_block_notxdetails},
      {"/rest/block/", rest_block_extended},
      {"/rest/chaininfo", rest_chaininfo},
      {"/rest/mempool/info", rest_mempool_info},
      {"/rest/mempool/contents", rest_mempool_contents},
      {"/rest/headers/", rest_headers},
      {"/rest/getutxos", rest_getutxos},
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

bool StartREST()
{
    for (unsigned int i = 0; i < ARRAYLEN(uri_prefixes); i++)
        RegisterHTTPHandler(uri_prefixes[i].prefix, false, uri_prefixes[i].handler);
    return true;
}

void InterruptREST()
{
}

<<<<<<< HEAD
void StopREST() {
=======
void StopREST()
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (unsigned int i = 0; i < ARRAYLEN(uri_prefixes); i++)
        UnregisterHTTPHandler(uri_prefixes[i].prefix, false);
}
