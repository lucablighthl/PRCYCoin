// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"
#include "main.h"
#include "rpc/server.h"
#include "sync.h"
#include "util.h"
#include "utilmoneystr.h"
#include "base58.h"

#include <stdint.h>

#include <univalue.h>
#include <mutex>
#include <condition_variable>
#include "clientversion.h"

using namespace std;
=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.

#include "budget/budgetmanager.h"
#include "checkpoints.h"
#include "clientversion.h"
#include "core_io.h"
#include "consensus/upgrades.h"
#include "kernel.h"
#include "key_io.h"
#include "masternodeman.h"
#include "policy/feerate.h"
#include "policy/policy.h"
#include "rpc/server.h"
#include "sync.h"
#include "txdb.h"
#include "util/system.h"
#include "utilmoneystr.h"
#include "utilstrencodings.h"
#include "hash.h"
#include "validationinterface.h"
#include "wallet/wallet.h"
#include "warnings.h"

#include <stdint.h>
#include <univalue.h>
#include <numeric>
#include <condition_variable>

#include <boost/thread/thread.hpp> // boost::thread::interrupt

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

struct CUpdatedBlock
{
    uint256 hash;
    int height;
};
static std::mutex cs_blockchange;
static std::condition_variable cond_blockchange;
static CUpdatedBlock latestblock;

<<<<<<< HEAD
extern void TxToJSON(const CTransaction& tx, const uint256 hashBlock, UniValue& entry);
extern void PoSBlockInfoToJSON(const uint256 hashBlock, int64_t nTime, int height, UniValue& entry);
void ScriptPubKeyToJSON(const CScript& scriptPubKey, UniValue& out, bool fIncludeHex);
=======
extern void TxToJSON(CWallet* const pwallet, const CTransaction& tx, const uint256 hashBlock, UniValue& entry);

UniValue syncwithvalidationinterfacequeue(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 0) {
        throw std::runtime_error(
                "syncwithvalidationinterfacequeue\n"
                "\nWaits for the validation interface queue to catch up on everything that was there when we entered this function.\n"
                "\nExamples:\n"
                + HelpExampleCli("syncwithvalidationinterfacequeue","")
                + HelpExampleRpc("syncwithvalidationinterfacequeue","")
        );
    }
    SyncWithValidationInterfaceQueue();
    return NullUniValue;
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

double GetDifficulty(const CBlockIndex* blockindex)
{
    // Floating point number that is a multiple of the minimum difficulty,
    // minimum difficulty = 1.0.
    if (blockindex == NULL) {
<<<<<<< HEAD
        if (chainActive.Tip() == NULL)
            return 1.0;
        else
            blockindex = chainActive.Tip();
=======
        const CBlockIndex* pChainTip = GetChainTip();
        if (!pChainTip)
            return 1.0;
        else
            blockindex = pChainTip;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    int nShift = (blockindex->nBits >> 24) & 0xff;

    double dDiff =
        (double)0x0000ffff / (double)(blockindex->nBits & 0x00ffffff);

    while (nShift < 29) {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29) {
        dDiff /= 256.0;
        nShift--;
    }

    return dDiff;
}

<<<<<<< HEAD
UniValue blockheaderToJSON(const CBlockIndex* blockindex)
{
    UniValue result(UniValue::VOBJ);
    result.push_back(Pair("hash", blockindex->GetBlockHash().GetHex()));
=======
static UniValue ValuePoolDesc(
        const Optional<CAmount> chainValue,
        const Optional<CAmount> valueDelta)
{
    UniValue rv(UniValue::VOBJ);
    rv.pushKV("chainValue",  ValueFromAmount(chainValue ? *chainValue : 0));
    rv.pushKV("valueDelta",  ValueFromAmount(valueDelta ? *valueDelta : 0));
    return rv;
}

UniValue blockheaderToJSON(const CBlockIndex* blockindex)
{
    UniValue result(UniValue::VOBJ);
    result.pushKV("hash", blockindex->GetBlockHash().GetHex());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int confirmations = -1;
    // Only report confirmations if the block is on the main chain
    if (chainActive.Contains(blockindex))
        confirmations = chainActive.Height() - blockindex->nHeight + 1;
<<<<<<< HEAD
    result.push_back(Pair("confirmations", confirmations));
    result.push_back(Pair("height", blockindex->nHeight));
    result.push_back(Pair("version", blockindex->nVersion));
    result.push_back(Pair("merkleroot", blockindex->hashMerkleRoot.GetHex()));
    result.push_back(Pair("time", (int64_t)blockindex->nTime));
    result.push_back(Pair("mediantime", (int64_t)blockindex->GetMedianTimePast()));
    result.push_back(Pair("nonce", (uint64_t)blockindex->nNonce));
    result.push_back(Pair("bits", strprintf("%08x", blockindex->nBits)));
    result.push_back(Pair("difficulty", GetDifficulty(blockindex)));
    result.push_back(Pair("chainwork", blockindex->nChainWork.GetHex()));
    result.push_back(Pair("acc_checkpoint", blockindex->nAccumulatorCheckpoint.GetHex()));

    if (blockindex->pprev)
        result.push_back(Pair("previousblockhash", blockindex->pprev->GetBlockHash().GetHex()));
    CBlockIndex *pnext = chainActive.Next(blockindex);
    if (pnext)
        result.push_back(Pair("nextblockhash", pnext->GetBlockHash().GetHex()));
=======
    result.pushKV("confirmations", confirmations);
    result.pushKV("height", blockindex->nHeight);
    result.pushKV("version", blockindex->nVersion);
    result.pushKV("merkleroot", blockindex->hashMerkleRoot.GetHex());
    result.pushKV("time", (int64_t)blockindex->nTime);
    result.pushKV("mediantime", (int64_t)blockindex->GetMedianTimePast());
    result.pushKV("nonce", (uint64_t)blockindex->nNonce);
    result.pushKV("bits", strprintf("%08x", blockindex->nBits));
    result.pushKV("difficulty", GetDifficulty(blockindex));
    result.pushKV("chainwork", blockindex->nChainWork.GetHex());
    result.pushKV("acc_checkpoint", blockindex->nAccumulatorCheckpoint.GetHex());
    // Sapling shield pool value
    result.pushKV("shield_pool_value", ValuePoolDesc(blockindex->nChainSaplingValue, blockindex->nSaplingValue));
    if (blockindex->pprev)
        result.pushKV("previousblockhash", blockindex->pprev->GetBlockHash().GetHex());
    CBlockIndex *pnext = chainActive.Next(blockindex);
    if (pnext)
        result.pushKV("nextblockhash", pnext->GetBlockHash().GetHex());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return result;
}

UniValue blockToJSON(const CBlock& block, const CBlockIndex* blockindex, bool txDetails = false)
{
    UniValue result(UniValue::VOBJ);
<<<<<<< HEAD
    result.push_back(Pair("hash", block.GetHash().GetHex()));
=======
    result.pushKV("hash", block.GetHash().GetHex());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int confirmations = -1;
    // Only report confirmations if the block is on the main chain
    if (chainActive.Contains(blockindex))
        confirmations = chainActive.Height() - blockindex->nHeight + 1;
<<<<<<< HEAD
    result.push_back(Pair("confirmations", confirmations));
    result.push_back(Pair("size", (int)::GetSerializeSize(block, SER_NETWORK, PROTOCOL_VERSION)));
    result.push_back(Pair("height", blockindex->nHeight));
    result.push_back(Pair("version", block.nVersion));
    result.push_back(Pair("merkleroot", block.hashMerkleRoot.GetHex()));
    result.push_back(Pair("acc_checkpoint", block.nAccumulatorCheckpoint.GetHex()));
    UniValue txs(UniValue::VARR);
    for (const CTransaction& tx : block.vtx) {
        if (txDetails) {
            UniValue objTx(UniValue::VOBJ);
            TxToJSON(tx, uint256(0), objTx);
=======
    result.pushKV("confirmations", confirmations);
    result.pushKV("size", (int)::GetSerializeSize(block, PROTOCOL_VERSION));
    result.pushKV("height", blockindex->nHeight);
    result.pushKV("version", block.nVersion);
    result.pushKV("merkleroot", block.hashMerkleRoot.GetHex());
    result.pushKV("acc_checkpoint", block.nAccumulatorCheckpoint.GetHex());
    result.pushKV("finalsaplingroot", block.hashFinalSaplingRoot.GetHex());
    UniValue txs(UniValue::VARR);
    for (const auto& txIn : block.vtx) {
        const CTransaction& tx = *txIn;
        if (txDetails) {
            UniValue objTx(UniValue::VOBJ);
            TxToJSON(nullptr, tx, UINT256_ZERO, objTx);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            txs.push_back(objTx);
        } else
            txs.push_back(tx.GetHash().GetHex());
    }
<<<<<<< HEAD
    result.push_back(Pair("tx", txs));
    result.push_back(Pair("time", block.GetBlockTime()));
    result.push_back(Pair("mediantime", (int64_t)blockindex->GetMedianTimePast()));
    result.push_back(Pair("nonce", (uint64_t)block.nNonce));
    result.push_back(Pair("bits", strprintf("%08x", block.nBits)));
    result.push_back(Pair("difficulty", GetDifficulty(blockindex)));
    result.push_back(Pair("chainwork", blockindex->nChainWork.GetHex()));

    if (blockindex->pprev)
        result.push_back(Pair("previousblockhash", blockindex->pprev->GetBlockHash().GetHex()));
    CBlockIndex* pnext = chainActive.Next(blockindex);
    if (pnext)
        result.push_back(Pair("nextblockhash", pnext->GetBlockHash().GetHex()));

    result.push_back(Pair("moneysupply",ValueFromAmount(blockindex->nMoneySupply)));
    std::string minetype = "PoW";
    if (blockindex->IsProofOfStake()) {
        minetype = "PoS";
    } else if (blockindex->IsProofOfAudit()) {
        minetype = "PoA";
    }

    result.push_back(Pair("minetype", minetype));

    if (blockindex->IsProofOfAudit()) {
        //This is a PoA block
        //Read information of PoS blocks audited by this PoA block
        result.push_back(Pair("previouspoahash", block.hashPrevPoABlock.GetHex()));
        UniValue posBlockInfos(UniValue::VARR);
        bool auditResult = true;
        for (int i = 0; i < block.posBlocksAudited.size(); i++) {
            UniValue objPoSBlockInfo(UniValue::VOBJ);
            PoSBlockInfoToJSON(block.posBlocksAudited[i].hash,
                                block.posBlocksAudited[i].nTime, block.posBlocksAudited[i].height, objPoSBlockInfo);
            posBlockInfos.push_back(objPoSBlockInfo);
            auditResult = auditResult & (block.posBlocksAudited[i].nTime > 0);
        }
        result.push_back(Pair("auditsuccess", auditResult? "true": "false"));
        result.push_back(Pair("posblocks", posBlockInfos));
        result.push_back(Pair("poscount", (int)block.posBlocksAudited.size()));
=======
    result.pushKV("tx", txs);
    result.pushKV("time", block.GetBlockTime());
    result.pushKV("mediantime", (int64_t)blockindex->GetMedianTimePast());
    result.pushKV("nonce", (uint64_t)block.nNonce);
    result.pushKV("bits", strprintf("%08x", block.nBits));
    result.pushKV("difficulty", GetDifficulty(blockindex));
    result.pushKV("chainwork", blockindex->nChainWork.GetHex());

    if (blockindex->pprev)
        result.pushKV("previousblockhash", blockindex->pprev->GetBlockHash().GetHex());
    CBlockIndex* pnext = chainActive.Next(blockindex);
    if (pnext)
        result.pushKV("nextblockhash", pnext->GetBlockHash().GetHex());

    //////////
    ////////// Coin stake data ////////////////
    /////////
    if (block.IsProofOfStake()) {
        uint256 hashProofOfStakeRet{UINT256_ZERO};
        if (blockindex->pprev && !GetStakeKernelHash(hashProofOfStakeRet, block, blockindex->pprev))
            throw JSONRPCError(RPC_INTERNAL_ERROR, "Cannot get proof of stake hash");

        std::string stakeModifier = (Params().GetConsensus().NetworkUpgradeActive(blockindex->nHeight, Consensus::UPGRADE_V3_4) ?
                                     blockindex->GetStakeModifierV2().GetHex() :
                                     strprintf("%016x", blockindex->GetStakeModifierV1()));
        result.pushKV("stakeModifier", stakeModifier);
        result.pushKV("hashProofOfStake", hashProofOfStakeRet.GetHex());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return result;
}

<<<<<<< HEAD
UniValue getsupply(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getsupply\n"
            "\nReturns the current supply.\n"
            "\nResult:\n"
            "n    (numeric) The current supply\n"
            "\nExamples:\n" +
            HelpExampleCli("getsupply", "") + HelpExampleRpc("getsupply", ""));

    LOCK(cs_main);
    return ValueFromAmount(chainActive.Tip()->nMoneySupply);
}

UniValue getmaxsupply(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmaxsupply\n"
            "\nReturns the max supply.\n"
            "\nResult:\n"
            "n    (numeric) The maximum supply\n"
            "\nExamples:\n" +
            HelpExampleCli("getmaxsupply", "") + HelpExampleRpc("getmaxsupply", ""));

    return ValueFromAmount(Params().TOTAL_SUPPLY);
}

UniValue getblockcount(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getblockcount\n"
            "\nReturns the number of blocks in the longest block chain.\n"
            "\nResult:\n"
            "n    (numeric) The current block count\n"
=======
UniValue getblockcount(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getblockcount\n"
            "\nReturns the number of blocks in the longest block chain.\n"

            "\nResult:\n"
            "n    (numeric) The current block count\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getblockcount", "") + HelpExampleRpc("getblockcount", ""));

    LOCK(cs_main);
    return chainActive.Height();
}

<<<<<<< HEAD
UniValue getbestblockhash(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getbestblockhash\n"
            "\nReturns the hash of the best (tip) block in the longest block chain.\n"
            "\nResult\n"
            "\"hex\"      (string) the block hash hex encoded\n"
=======
UniValue getbestblockhash(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getbestblockhash\n"
            "\nReturns the hash of the best (tip) block in the longest block chain.\n"

            "\nResult\n"
            "\"hex\"      (string) the block hash hex encoded\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples\n" +
            HelpExampleCli("getbestblockhash", "") + HelpExampleRpc("getbestblockhash", ""));

    LOCK(cs_main);
    return chainActive.Tip()->GetBlockHash().GetHex();
}

<<<<<<< HEAD
=======
UniValue getbestsaplinganchor(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getbestsaplinganchor\n"
            "\nReturns the most recent SaplingMerkleTree root.\n"

            "\nResult\n"
            "\"hex\"      (string) the sapling anchor hex encoded\n"

            "\nExamples\n" +
            HelpExampleCli("getbestsaplinganchor", "") + HelpExampleRpc("getbestsaplinganchor", ""));

    return pcoinsTip->GetBestAnchor().ToString();
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void RPCNotifyBlockChange(bool fInitialDownload, const CBlockIndex* pindex)
{
    if(pindex) {
        std::lock_guard<std::mutex> lock(cs_blockchange);
        latestblock.hash = pindex->GetBlockHash();
        latestblock.height = pindex->nHeight;
    }
    cond_blockchange.notify_all();
}

<<<<<<< HEAD
UniValue waitfornewblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
=======
UniValue waitfornewblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        throw std::runtime_error(
            "waitfornewblock ( timeout )\n"
            "\nWaits for a specific new block and returns useful info about it.\n"
            "\nReturns the current block on timeout or exit.\n"

            "\nArguments:\n"
<<<<<<< HEAD
            "1. timeout (int, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                           (json object)\n"
            "  \"hash\" : {       (string) The blockhash\n"
            "  \"height\" : {     (int) Block height\n"
=======
            "1. timeout       (numeric, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                       (json object)\n"
            "  \"hash\": \"hash\",   (string) The blockhash\n"
            "  \"height\": n         (numeric) Block height\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "}\n"

            "\nExamples:\n"
            + HelpExampleCli("waitfornewblock", "1000")
            + HelpExampleRpc("waitfornewblock", "1000")
        );
    int timeout = 0;
<<<<<<< HEAD
    if (params.size() > 0)
        timeout = params[0].get_int();
=======
    if (request.params.size() > 0)
        timeout = request.params[0].get_int();

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CUpdatedBlock block;
    {
        std::unique_lock<std::mutex> lock(cs_blockchange);
        block = latestblock;
        if(timeout)
            cond_blockchange.wait_for(lock, std::chrono::milliseconds(timeout), [&block]{return latestblock.height != block.height || latestblock.hash != block.hash || !IsRPCRunning(); });
        else
            cond_blockchange.wait(lock, [&block]{return latestblock.height != block.height || latestblock.hash != block.hash || !IsRPCRunning(); });
        block = latestblock;
    }
    UniValue ret(UniValue::VOBJ);
<<<<<<< HEAD
    ret.push_back(Pair("hash", block.hash.GetHex()));
    ret.push_back(Pair("height", block.height));
    return ret;
}

UniValue waitforblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
=======
    ret.pushKV("hash", block.hash.GetHex());
    ret.pushKV("height", block.height);
    return ret;
}

UniValue waitforblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        throw std::runtime_error(
            "waitforblock blockhash ( timeout )\n"
            "\nWaits for a specific new block and returns useful info about it.\n"
            "\nReturns the current block on timeout or exit.\n"

            "\nArguments:\n"
<<<<<<< HEAD
            "1. \"blockhash\" (required, string) Block hash to wait for.\n"
            "2. timeout       (int, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                           (json object)\n"
            "  \"hash\" : {       (string) The blockhash\n"
            "  \"height\" : {     (int) Block height\n"
=======
            "1. \"blockhash\"    (string, required) Block hash to wait for.\n"
            "2. timeout          (numeric, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                       (json object)\n"
            "  \"hash\": \"hash\",   (string) The blockhash\n"
            "  \"height\": n         (numeric) Block height\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "}\n"

            "\nExamples:\n"
            + HelpExampleCli("waitforblock", "\"0000000000079f8ef3d2c688c244eb7a4570b24c9ed7b4a8c619eb02596f8862\", 1000")
            + HelpExampleRpc("waitforblock", "\"0000000000079f8ef3d2c688c244eb7a4570b24c9ed7b4a8c619eb02596f8862\", 1000")
        );
    int timeout = 0;

<<<<<<< HEAD
    uint256 hash = uint256S(params[0].get_str());

    if (params.size() > 1)
        timeout = params[1].get_int();
=======
    uint256 hash = uint256S(request.params[0].get_str());

    if (request.params.size() > 1)
        timeout = request.params[1].get_int();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CUpdatedBlock block;
    {
        std::unique_lock<std::mutex> lock(cs_blockchange);
        if(timeout)
            cond_blockchange.wait_for(lock, std::chrono::milliseconds(timeout), [&hash]{return latestblock.hash == hash || !IsRPCRunning();});
        else
            cond_blockchange.wait(lock, [&hash]{return latestblock.hash == hash || !IsRPCRunning(); });
        block = latestblock;
    }

    UniValue ret(UniValue::VOBJ);
<<<<<<< HEAD
    ret.push_back(Pair("hash", block.hash.GetHex()));
    ret.push_back(Pair("height", block.height));
    return ret;
}

UniValue waitforblockheight(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
=======
    ret.pushKV("hash", block.hash.GetHex());
    ret.pushKV("height", block.height);
    return ret;
}

UniValue waitforblockheight(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        throw std::runtime_error(
            "waitforblockheight height ( timeout )\n"
            "\nWaits for (at least) block height and returns the height and hash\n"
            "of the current tip.\n"
            "\nReturns the current block on timeout or exit.\n"

            "\nArguments:\n"
<<<<<<< HEAD
            "1. height  (required, int) Block height to wait for (int)\n"
            "2. timeout (int, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                           (json object)\n"
            "  \"hash\" : {       (string) The blockhash\n"
            "  \"height\" : {     (int) Block height\n"
=======
            "1. height           (numeric, required) Block height to wait for\n"
            "2. timeout          (numeric, optional, default=0) Time in milliseconds to wait for a response. 0 indicates no timeout.\n"

            "\nResult:\n"
            "{                       (json object)\n"
            "  \"hash\": \"hash\",   (string) The blockhash\n"
            "  \"height\": n         (int) Block height\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "}\n"

            "\nExamples:\n"
            + HelpExampleCli("waitforblockheight", "\"100\", 1000")
            + HelpExampleRpc("waitforblockheight", "\"100\", 1000")
        );
    int timeout = 0;

<<<<<<< HEAD
    int height = params[0].get_int();

    if (params.size() > 1)
        timeout = params[1].get_int();
=======
    int height = request.params[0].get_int();

    if (request.params.size() > 1)
        timeout = request.params[1].get_int();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CUpdatedBlock block;
    {
        std::unique_lock<std::mutex> lock(cs_blockchange);
        if(timeout)
            cond_blockchange.wait_for(lock, std::chrono::milliseconds(timeout), [&height]{return latestblock.height >= height || !IsRPCRunning();});
        else
            cond_blockchange.wait(lock, [&height]{return latestblock.height >= height || !IsRPCRunning(); });
        block = latestblock;
    }
    UniValue ret(UniValue::VOBJ);
<<<<<<< HEAD
    ret.push_back(Pair("hash", block.hash.GetHex()));
    ret.push_back(Pair("height", block.height));
    return ret;
}

UniValue getdifficulty(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getdifficulty\n"
            "\nReturns the proof-of-work difficulty as a multiple of the minimum difficulty.\n"
            "\nResult:\n"
            "n.nnn       (numeric) the proof-of-work difficulty as a multiple of the minimum difficulty.\n"
=======
    ret.pushKV("hash", block.hash.GetHex());
    ret.pushKV("height", block.height);
    return ret;
}

UniValue getdifficulty(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getdifficulty\n"
            "\nReturns the proof-of-work difficulty as a multiple of the minimum difficulty.\n"

            "\nResult:\n"
            "n.nnn       (numeric) the proof-of-work difficulty as a multiple of the minimum difficulty.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getdifficulty", "") + HelpExampleRpc("getdifficulty", ""));

    LOCK(cs_main);
    return GetDifficulty();
}

<<<<<<< HEAD
=======
static std::string EntryDescriptionString()
{
    return "    \"size\" : n,             (numeric) transaction size in bytes\n"
           "    \"fee\" : n,              (numeric) transaction fee in " + CURRENCY_UNIT + "\n"
           "    \"modifiedfee\" : n,      (numeric) transaction fee with fee deltas used for mining priority\n"
           "    \"time\" : n,             (numeric) local time transaction entered pool in seconds since 1 Jan 1970 GMT\n"
           "    \"height\" : n,           (numeric) block height when transaction entered pool\n"
           "    \"descendantcount\" : n,  (numeric) number of in-mempool descendant transactions (including this one)\n"
           "    \"descendantsize\" : n,   (numeric) size of in-mempool descendants (including this one)\n"
           "    \"descendantfees\" : n,   (numeric) modified fees (see above) of in-mempool descendants (including this one)\n"
           "    \"depends\" : [           (array) unconfirmed transactions used as inputs for this transaction\n"
           "        \"transactionid\",    (string) parent transaction id\n"
           "       ... ]\n";
}

static void entryToJSON(UniValue &info, const CTxMemPoolEntry &e)
{
    AssertLockHeld(mempool.cs);

    info.pushKV("size", (int)e.GetTxSize());
    info.pushKV("fee", ValueFromAmount(e.GetFee()));
    info.pushKV("modifiedfee", ValueFromAmount(e.GetModifiedFee()));
    info.pushKV("time", e.GetTime());
    info.pushKV("height", (int)e.GetHeight());
    info.pushKV("descendantcount", e.GetCountWithDescendants());
    info.pushKV("descendantsize", e.GetSizeWithDescendants());
    info.pushKV("descendantfees", e.GetModFeesWithDescendants());
    const CTransaction& tx = e.GetTx();
    std::set<std::string> setDepends;
    for (const CTxIn& txin : tx.vin) {
        if (mempool.exists(txin.prevout.hash))
            setDepends.insert(txin.prevout.hash.ToString());
    }

    UniValue depends(UniValue::VARR);
    for (const std::string& dep : setDepends) {
        depends.push_back(dep);
    }

    info.pushKV("depends", depends);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

UniValue mempoolToJSON(bool fVerbose = false)
{
    if (fVerbose) {
        LOCK(mempool.cs);
        UniValue o(UniValue::VOBJ);
<<<<<<< HEAD
        for (const PAIRTYPE(uint256, CTxMemPoolEntry) & entry : mempool.mapTx) {
            const uint256& hash = entry.first;
            const CTxMemPoolEntry& e = entry.second;
            UniValue info(UniValue::VOBJ);
            info.push_back(Pair("size", (int)e.GetTxSize()));
            info.push_back(Pair("fee", ValueFromAmount(e.GetFee())));
            info.push_back(Pair("time", e.GetTime()));
            info.push_back(Pair("height", (int)e.GetHeight()));
            info.push_back(Pair("startingpriority", e.GetPriority(e.GetHeight())));
            info.push_back(Pair("currentpriority", e.GetPriority(chainActive.Height())));
            const CTransaction& tx = e.GetTx();
            set<string> setDepends;
            for (const CTxIn& txin : tx.vin) {
                if (mempool.exists(txin.prevout.hash))
                    setDepends.insert(txin.prevout.hash.ToString());
            }

            UniValue depends(UniValue::VARR);
            for (const string& dep : setDepends) {
                depends.push_back(dep);
            }

            info.push_back(Pair("depends", depends));
            o.push_back(Pair(hash.ToString(), info));
        }
        return o;
    } else {
        vector<uint256> vtxid;
=======
        for (const CTxMemPoolEntry& e : mempool.mapTx) {
            const uint256& hash = e.GetTx().GetHash();
            UniValue info(UniValue::VOBJ);
            entryToJSON(info, e);
            o.pushKV(hash.ToString(), info);
        }
        return o;
    } else {
        std::vector<uint256> vtxid;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        mempool.queryHashes(vtxid);

        UniValue a(UniValue::VARR);
        for (const uint256& hash : vtxid)
            a.push_back(hash.ToString());

        return a;
    }
}

<<<<<<< HEAD
UniValue getrawmempool(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
                "getrawmempool ( verbose )\n"
                "\nReturns all transaction ids in memory pool as a json array of string transaction ids.\n"
                "\nArguments:\n"
                "1. verbose           (boolean, optional, default=false) true for a json object, false for array of transaction ids\n"
                "\nResult: (for verbose = false):\n"
                "[                     (json array of string)\n"
                "  \"transactionid\"     (string) The transaction id\n"
                "  ,...\n"
                "]\n"
                "\nResult: (for verbose = true):\n"
                "{                           (json object)\n"
                "  \"transactionid\" : {       (json object)\n"
                "    \"size\" : n,             (numeric) transaction size in bytes\n"
                "    \"fee\" : n,              (numeric) transaction fee in PRCY\n"
                "    \"time\" : n,             (numeric) local time transaction entered pool in seconds since 1 Jan 1970 GMT\n"
                "    \"height\" : n,           (numeric) block height when transaction entered pool\n"
                "    \"startingpriority\" : n, (numeric) priority when transaction entered pool\n"
                "    \"currentpriority\" : n,  (numeric) transaction priority now\n"
                "    \"depends\" : [           (array) unconfirmed transactions used as inputs for this transaction\n"
                "        \"transactionid\",    (string) parent transaction id\n"
                "       ... ]\n"
                "  }, ...\n"
                "]\n"
                "\nExamples\n" +
                HelpExampleCli("getrawmempool", "true") + HelpExampleRpc("getrawmempool", "true"));
=======
UniValue getrawmempool(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
        throw std::runtime_error(
            "getrawmempool ( verbose )\n"
            "\nReturns all transaction ids in memory pool as a json array of string transaction ids.\n"

            "\nArguments:\n"
            "1. verbose           (boolean, optional, default=false) True for a json object, false for array of transaction ids\n"

            "\nResult: (for verbose = false):\n"
            "[                     (json array of string)\n"
            "  \"transactionid\"     (string) The transaction id\n"
            "  ,...\n"
            "]\n"

            "\nResult: (for verbose = true):\n"
            "{                           (json object)\n"
            "  \"transactionid\" : {       (json object)\n"
            + EntryDescriptionString()
            + "  }, ...\n"
            "}\n"
            "\nExamples\n" +
            HelpExampleCli("getrawmempool", "true") + HelpExampleRpc("getrawmempool", "true"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    LOCK(cs_main);

    bool fVerbose = false;
<<<<<<< HEAD
    if (params.size() > 0)
        fVerbose = params[0].get_bool();
=======
    if (request.params.size() > 0)
        fVerbose = request.params[0].get_bool();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return mempoolToJSON(fVerbose);
}

<<<<<<< HEAD

UniValue getblockhash(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "getblockhash index\n"
            "\nReturns hash of block in best-block-chain at index provided.\n"
            "\nArguments:\n"
            "1. index         (numeric, required) The block index\n"
            "\nResult:\n"
            "\"hash\"         (string) The block hash\n"
=======
UniValue getblockhash(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "getblockhash height\n"
            "\nReturns hash of block in best-block-chain at height provided.\n"

            "\nArguments:\n"
            "1. height        (numeric, required) The height index\n"

            "\nResult:\n"
            "\"hash\"         (string) The block hash\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getblockhash", "1000") + HelpExampleRpc("getblockhash", "1000"));

    LOCK(cs_main);

<<<<<<< HEAD
    int nHeight = params[0].get_int();
=======
    int nHeight = request.params[0].get_int();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (nHeight < 0 || nHeight > chainActive.Height())
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Block height out of range");

    CBlockIndex* pblockindex = chainActive[nHeight];
    return pblockindex->GetBlockHash().GetHex();
}

<<<<<<< HEAD
UniValue getblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "getblock \"hash\" ( verbose )\n"
            "\nIf verbose is false, returns a string that is serialized, hex-encoded data for block 'hash'.\n"
            "If verbose is true, returns an Object with information about block <hash>.\n"
            "\nArguments:\n"
            "1. \"hash\"          (string, required) The block hash\n"
            "2. verbose           (boolean, optional, default=true) true for a json object, false for the hex encoded data\n"
=======
UniValue getblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
        throw std::runtime_error(
            "getblock \"blockhash\" ( verbose )\n"
            "\nIf verbose is false, returns a string that is serialized, hex-encoded data for block 'hash'.\n"
            "If verbose is true, returns an Object with information about block <hash>.\n"

            "\nArguments:\n"
            "1. \"blockhash\"     (string, required) The block hash\n"
            "2. verbose           (boolean, optional, default=true) True for a json object, false for the hex encoded data\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult (for verbose = true):\n"
            "{\n"
            "  \"hash\" : \"hash\",     (string) the block hash (same as provided)\n"
            "  \"confirmations\" : n,   (numeric) The number of confirmations, or -1 if the block is not on the main chain\n"
            "  \"size\" : n,            (numeric) The block size\n"
            "  \"height\" : n,          (numeric) The block height or index\n"
            "  \"version\" : n,         (numeric) The block version\n"
            "  \"merkleroot\" : \"xxxx\", (string) The merkle root\n"
<<<<<<< HEAD
=======
            "  \"finalsaplingroot\" : \"xxxx\", (string) The root of the Sapling commitment tree after applying this block\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"tx\" : [               (array of string) The transaction ids\n"
            "     \"transactionid\"     (string) The transaction id\n"
            "     ,...\n"
            "  ],\n"
            "  \"time\" : ttt,          (numeric) The block time in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"mediantime\" : ttt,    (numeric) The median block time in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"nonce\" : n,           (numeric) The nonce\n"
            "  \"bits\" : \"1d00ffff\", (string) The bits\n"
            "  \"difficulty\" : x.xxx,  (numeric) The difficulty\n"
            "  \"previousblockhash\" : \"hash\",  (string) The hash of the previous block\n"
            "  \"nextblockhash\" : \"hash\"       (string) The hash of the next block\n"
<<<<<<< HEAD
            "  \"moneysupply\" : \"supply\"       (numeric) The money supply when this block was added to the blockchain\n"
            "}\n"
            "\nResult (for verbose=false):\n"
            "\"data\"             (string) A string that is serialized, hex-encoded data for block 'hash'.\n"
            "\nExamples:\n" +
            HelpExampleCli("getblock", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\"") + HelpExampleRpc("getblock", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\""));

    std::string strHash = params[0].get_str();
    uint256 hash(strHash);

    bool fVerbose = true;
    if (params.size() > 1)
        fVerbose = params[1].get_bool();
=======
            "  \"stakeModifier\" : \"xxx\",       (string) Proof of Stake modifier\n"
            "  \"hashProofOfStake\" : \"hash\",   (string) Proof of Stake hash\n"
            "  }\n"
            "}\n"

            "\nResult (for verbose=false):\n"
            "\"data\"             (string) A string that is serialized, hex-encoded data for block 'hash'.\n"

            "\nExamples:\n" +
            HelpExampleCli("getblock", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\"") +
            HelpExampleRpc("getblock", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\""));

    LOCK(cs_main);

    std::string strHash = request.params[0].get_str();
    uint256 hash(uint256S(strHash));

    bool fVerbose = true;
    if (request.params.size() > 1)
        fVerbose = request.params[1].get_bool();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (mapBlockIndex.count(hash) == 0)
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Block not found");

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];

    if (!ReadBlockFromDisk(block, pblockindex))
        throw JSONRPCError(RPC_INTERNAL_ERROR, "Can't read block from disk");

    if (!fVerbose) {
        CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
        ssBlock << block;
<<<<<<< HEAD
        std::string strHex = HexStr(ssBlock.begin(), ssBlock.end());
=======
        std::string strHex = HexStr(ssBlock);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return strHex;
    }

    return blockToJSON(block, pblockindex);
}

<<<<<<< HEAD
UniValue getblockheader(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 1 || params.size() > 2)
        throw runtime_error(
            "getblockheader \"hash\" ( verbose )\n"
            "\nIf verbose is false, returns a string that is serialized, hex-encoded data for block 'hash' header.\n"
            "If verbose is true, returns an Object with information about block <hash> header.\n"
            "\nArguments:\n"
            "1. \"hash\"          (string, required) The block hash\n"
            "2. verbose           (boolean, optional, default=true) true for a json object, false for the hex encoded data\n"
=======
UniValue getblockheader(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
        throw std::runtime_error(
            "getblockheader \"blockhash\" ( verbose )\n"
            "\nIf verbose is false, returns a string that is serialized, hex-encoded data for block 'hash' header.\n"
            "If verbose is true, returns an Object with information about block <hash> header.\n"

            "\nArguments:\n"
            "1. \"blockhash\"     (string, required) The block hash\n"
            "2. verbose           (boolean, optional, default=true) True for a json object, false for the hex encoded data\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult (for verbose = true):\n"
            "{\n"
            "  \"version\" : n,         (numeric) The block version\n"
            "  \"previousblockhash\" : \"hash\",  (string) The hash of the previous block\n"
            "  \"merkleroot\" : \"xxxx\", (string) The merkle root\n"
            "  \"time\" : ttt,          (numeric) The block time in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"mediantime\" : ttt,    (numeric) The median block time in seconds since epoch (Jan 1 1970 GMT)\n"
            "  \"nonce\" : n,           (numeric) The nonce\n"
            "  \"bits\" : \"1d00ffff\", (string) The bits\n"
<<<<<<< HEAD
            "}\n"
            "\nResult (for verbose=false):\n"
            "\"data\"             (string) A string that is serialized, hex-encoded data for block 'hash' header.\n"
            "\nExamples:\n" +
            HelpExampleCli("getblockheader", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\"") + HelpExampleRpc("getblockheader", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\""));

    std::string strHash = params[0].get_str();
    uint256 hash(strHash);

    bool fVerbose = true;
    if (params.size() > 1)
        fVerbose = params[1].get_bool();
=======
            "  \"shield_pool_value\":   (object) Block shield pool value\n"
            "  {\n"
            "     \"chainValue\":        (numeric) Total value held by the Sapling circuit up to and including this block\n"
            "     \"valueDelta\":        (numeric) Change in value held by the Sapling circuit over this block\n"
            "  }\n"
            "}"
            "}\n"

            "\nResult (for verbose=false):\n"
            "\"data\"             (string) A string that is serialized, hex-encoded data for block 'hash' header.\n"

            "\nExamples:\n" +
            HelpExampleCli("getblockheader", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\"") +
            HelpExampleRpc("getblockheader", "\"00000000000fd08c2fb661d2fcb0d49abb3a91e5f27082ce64feed3b4dede2e2\""));

    std::string strHash = request.params[0].get_str();
    uint256 hash(uint256S(strHash));

    bool fVerbose = true;
    if (request.params.size() > 1)
        fVerbose = request.params[1].get_bool();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (mapBlockIndex.count(hash) == 0)
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Block not found");

<<<<<<< HEAD
    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];

    if (!ReadBlockFromDisk(block, pblockindex))
        throw JSONRPCError(RPC_INTERNAL_ERROR, "Can't read block from disk");

    if (!fVerbose) {
        CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
        ssBlock << pblockindex->GetBlockHeader();
        std::string strHex = HexStr(ssBlock.begin(), ssBlock.end());
=======
    CBlockIndex* pblockindex = mapBlockIndex[hash];

    if (!fVerbose) {
        CDataStream ssBlock(SER_NETWORK, PROTOCOL_VERSION);
        ssBlock << pblockindex->GetBlockHeader();
        std::string strHex = HexStr(ssBlock);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return strHex;
    }

    return blockheaderToJSON(pblockindex);
}

<<<<<<< HEAD
UniValue gettxoutsetinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "gettxoutsetinfo\n"
            "\nReturns statistics about the unspent transaction output set.\n"
            "Note this call may take some time.\n"
=======
UniValue getsupplyinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
        throw std::runtime_error(
            "getsupplyinfo ( force_update )\n"
            "\nIf force_update=false (default if no argument is given): return the last cached money supply"
            "\n(sum of spendable transaction outputs) and the height of the chain when it was last updated"
            "\n(it is updated periodically, whenever the chainstate is flushed)."
            "\n"
            "\nIf force_update=true: Flush the chainstate to disk and return the money supply updated to"
            "\nthe current chain height.\n"

            "\nArguments:\n"
            "1. force_update       (boolean, optional, default=false) flush chainstate to disk and update cache\n"

            "\nResult:\n"
            "{\n"
            "  \"updateheight\" : n,       (numeric) The chain height when the transparent supply was updated\n"
            "  \"transparentsupply\" : n   (numeric) The sum of all spendable transaction outputs at height updateheight\n"
            "  \"shieldsupply\": n         (numeric) Chain tip shield pool value\n"
            "  \"totalsupply\": n          (numeric) The sum of transparentsupply and shieldsupply\n"
            "}\n"

            "\nExamples:\n" +
            HelpExampleCli("getsupplyinfo", "") + HelpExampleCli("getsupplyinfo", "true") +
            HelpExampleRpc("getsupplyinfo", ""));

    const bool fForceUpdate = request.params.size() > 0 ? request.params[0].get_bool() : false;

    if (fForceUpdate) {
        // Flush state to disk (which updates the cached supply)
        FlushStateToDisk();
    }

    UniValue ret(UniValue::VOBJ);
    const CAmount tSupply = MoneySupply.Get();
    ret.pushKV("updateheight", MoneySupply.GetCacheHeight());
    ret.pushKV("transparentsupply", ValueFromAmount(tSupply));
    Optional<CAmount> shieldedPoolValue = WITH_LOCK(cs_main, return (chainActive.Tip() ? chainActive.Tip()->nChainSaplingValue : nullopt); );
    ret.pushKV("shieldsupply", ValuePoolDesc(shieldedPoolValue, nullopt)["chainValue"]);
    const CAmount totalSupply = tSupply + (shieldedPoolValue ? *shieldedPoolValue : 0);
    ret.pushKV("totalsupply", ValueFromAmount(totalSupply));

    return ret;
}

struct CCoinsStats
{
    int nHeight{0};
    uint256 hashBlock{UINT256_ZERO};
    uint64_t nTransactions{0};
    uint64_t nTransactionOutputs{0};
    uint256 hashSerialized{UINT256_ZERO};
    uint64_t nDiskSize{0};
    CAmount nTotalAmount{0};
};

static void ApplyStats(CCoinsStats &stats, CHashWriter& ss, const uint256& hash, const std::map<uint32_t, Coin>& outputs)
{
    assert(!outputs.empty());
    ss << hash;
    const Coin& coin = outputs.begin()->second;
    ss << VARINT(coin.nHeight * 4 + (coin.fCoinBase ? 2u : 0u) + (coin.fCoinStake ? 1u : 0u));
    stats.nTransactions++;
    for (const auto& output : outputs) {
        ss << VARINT(output.first + 1);
        ss << output.second.out.scriptPubKey;
        ss << VARINT_MODE(output.second.out.nValue, VarIntMode::NONNEGATIVE_SIGNED);
        stats.nTransactionOutputs++;
        stats.nTotalAmount += output.second.out.nValue;
    }
    ss << VARINT(0u);
}

//! Calculate statistics about the unspent transaction output set
static bool GetUTXOStats(CCoinsView *view, CCoinsStats &stats)
{
    std::unique_ptr<CCoinsViewCursor> pcursor(view->Cursor());
    assert(pcursor);

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    stats.hashBlock = pcursor->GetBestBlock();
    {
        LOCK(cs_main);
        stats.nHeight = mapBlockIndex.find(stats.hashBlock)->second->nHeight;
    }
    ss << stats.hashBlock;
    uint256 prevkey;
    std::map<uint32_t, Coin> outputs;
    while (pcursor->Valid()) {
        boost::this_thread::interruption_point();
        COutPoint key;
        Coin coin;
        if (pcursor->GetKey(key) && pcursor->GetValue(coin)) {
            if (!outputs.empty() && key.hash != prevkey) {
                ApplyStats(stats, ss, prevkey, outputs);
                outputs.clear();
            }
            prevkey = key.hash;
            outputs[key.n] = std::move(coin);
        } else {
            return error("%s: unable to read value", __func__);
        }
        pcursor->Next();
    }
    if (!outputs.empty()) {
        ApplyStats(stats, ss, prevkey, outputs);
    }
    stats.hashSerialized = ss.GetHash();
    stats.nDiskSize = view->EstimateSize();
    return true;
}

UniValue gettxoutsetinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "gettxoutsetinfo\n"
            "\nReturns statistics about the unspent transaction output set.\n"
            "Note this call may take some time.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult:\n"
            "{\n"
            "  \"height\":n,     (numeric) The current block height (index)\n"
            "  \"bestblock\": \"hex\",   (string) the best block hash hex\n"
            "  \"transactions\": n,      (numeric) The number of transactions\n"
            "  \"txouts\": n,            (numeric) The number of output transactions\n"
<<<<<<< HEAD
            "  \"bytes_serialized\": n,  (numeric) The serialized size\n"
            "  \"hash_serialized\": \"hash\",   (string) The serialized hash\n"
            "  \"total_amount\": x.xxx          (numeric) The total amount\n"
            "}\n"
            "\nExamples:\n" +
            HelpExampleCli("gettxoutsetinfo", "") + HelpExampleRpc("gettxoutsetinfo", ""));

    LOCK(cs_main);

=======
            "  \"hash_serialized_2\": \"hash\",   (string) The serialized hash\n"
            "  \"disk_size\": n,         (numeric) The estimated size of the chainstate on disk\n"
            "  \"total_amount\": x.xxx          (numeric) The total amount\n"
            "}\n"

            "\nExamples:\n" +
            HelpExampleCli("gettxoutsetinfo", "") + HelpExampleRpc("gettxoutsetinfo", ""));

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    UniValue ret(UniValue::VOBJ);

    CCoinsStats stats;
    FlushStateToDisk();
<<<<<<< HEAD
    if (pcoinsTip->GetStats(stats)) {
        ret.push_back(Pair("height", (int64_t)stats.nHeight));
        ret.push_back(Pair("bestblock", stats.hashBlock.GetHex()));
        ret.push_back(Pair("transactions", (int64_t)stats.nTransactions));
        ret.push_back(Pair("txouts", (int64_t)stats.nTransactionOutputs));
        ret.push_back(Pair("bytes_serialized", (int64_t)stats.nSerializedSize));
        ret.push_back(Pair("hash_serialized", stats.hashSerialized.GetHex()));
        ret.push_back(Pair("total_amount", ValueFromAmount(chainActive.Tip()->nMoneySupply)));
=======
    if (GetUTXOStats(pcoinsTip.get(), stats)) {
        ret.pushKV("height", (int64_t)stats.nHeight);
        ret.pushKV("bestblock", stats.hashBlock.GetHex());
        ret.pushKV("transactions", (int64_t)stats.nTransactions);
        ret.pushKV("txouts", (int64_t)stats.nTransactionOutputs);
        ret.pushKV("hash_serialized_2", stats.hashSerialized.GetHex());
        ret.pushKV("total_amount", ValueFromAmount(stats.nTotalAmount));
        ret.pushKV("disk_size", stats.nDiskSize);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
    return ret;
}

<<<<<<< HEAD
UniValue gettxout(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() < 2 || params.size() > 3)
        throw runtime_error(
            "gettxout \"txid\" n ( includemempool )\n"
            "\nReturns details about an unspent transaction output.\n"
            "\nArguments:\n"
            "1. \"txid\"       (string, required) The transaction id\n"
            "2. n              (numeric, required) vout value\n"
            "3. includemempool  (boolean, optional) Whether to included the mem pool\n"
=======
UniValue gettxout(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 2 || request.params.size() > 3)
        throw std::runtime_error(
            "gettxout \"txid\" n ( include_mempool )\n"
            "\nReturns details about an unspent transaction output.\n"

            "\nArguments:\n"
            "1. \"txid\"       (string, required) The transaction id\n"
            "2. n              (numeric, required) vout value\n"
            "3. include_mempool  (boolean, optional) Whether to included the mem pool\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult:\n"
            "{\n"
            "  \"bestblock\" : \"hash\",    (string) the block hash\n"
            "  \"confirmations\" : n,       (numeric) The number of confirmations\n"
<<<<<<< HEAD
            "  \"value\" : x.xxx,           (numeric) The transaction value in PRCY\n"
=======
            "  \"value\" : x.xxx,           (numeric) The transaction value in PIV\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"scriptPubKey\" : {         (json object)\n"
            "     \"asm\" : \"code\",       (string) \n"
            "     \"hex\" : \"hex\",        (string) \n"
            "     \"reqSigs\" : n,          (numeric) Number of required signatures\n"
            "     \"type\" : \"pubkeyhash\", (string) The type, eg pubkeyhash\n"
<<<<<<< HEAD
            "     \"addresses\" : [          (array of string) array of prcycoin addresses\n"
            "     \"prcycoinaddress\"   	 	(string) prcycoin address\n"
            "        ,...\n"
            "     ]\n"
            "  },\n"
            "  \"version\" : n,            (numeric) The version\n"
=======
            "     \"addresses\" : [          (array of string) array of pivx addresses\n"
            "     \"pivxaddress\"            (string) pivx address\n"
            "        ,...\n"
            "     ]\n"
            "  },\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"coinbase\" : true|false   (boolean) Coinbase or not\n"
            "}\n"

            "\nExamples:\n"
            "\nGet unspent transactions\n" +
            HelpExampleCli("listunspent", "") +
<<<<<<< HEAD
            "\nView the details\n" + HelpExampleCli("gettxout", "\"txid\" 1") +
            "\nAs a json rpc call\n" + HelpExampleRpc("gettxout", "\"txid\", 1"));
=======
            "\nView the details\n" +
            HelpExampleCli("gettxout", "\"txid\" 1") +
            "\nAs a json rpc call\n" +
            HelpExampleRpc("gettxout", "\"txid\", 1"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    LOCK(cs_main);

    UniValue ret(UniValue::VOBJ);

<<<<<<< HEAD
    std::string strHash = params[0].get_str();
    uint256 hash(strHash);
    int n = params[1].get_int();
    bool fMempool = true;
    if (params.size() > 2)
        fMempool = params[2].get_bool();

    CCoins coins;
    if (fMempool) {
        LOCK(mempool.cs);
        CCoinsViewMemPool view(pcoinsTip, mempool);
        if (!view.GetCoins(hash, coins))
            return NullUniValue;
        mempool.pruneSpent(hash, coins); // TODO: this should be done by the CCoinsViewMemPool
    } else {
        if (!pcoinsTip->GetCoins(hash, coins))
            return NullUniValue;
    }
    if (n < 0 || (unsigned int)n >= coins.vout.size() || coins.vout[n].IsNull())
        return NullUniValue;

    BlockMap::iterator it = mapBlockIndex.find(pcoinsTip->GetBestBlock());
    CBlockIndex* pindex = it->second;
    ret.push_back(Pair("bestblock", pindex->GetBlockHash().GetHex()));
    if ((unsigned int)coins.nHeight == MEMPOOL_HEIGHT)
        ret.push_back(Pair("confirmations", 0));
    else
        ret.push_back(Pair("confirmations", pindex->nHeight - coins.nHeight + 1));
    ret.push_back(Pair("value", ValueFromAmount(coins.vout[n].nValue)));
    UniValue o(UniValue::VOBJ);
    ScriptPubKeyToJSON(coins.vout[n].scriptPubKey, o, true);
    ret.push_back(Pair("scriptPubKey", o));
    ret.push_back(Pair("version", coins.nVersion));
    ret.push_back(Pair("coinbase", coins.fCoinBase));
=======
    std::string strHash = request.params[0].get_str();
    uint256 hash(uint256S(strHash));
    int n = request.params[1].get_int();
    COutPoint out(hash, n);
    bool fMempool = true;
    if (request.params.size() > 2)
        fMempool = request.params[2].get_bool();

    Coin coin;
    if (fMempool) {
        LOCK(mempool.cs);
        CCoinsViewMemPool view(pcoinsTip.get(), mempool);
        if (!view.GetCoin(out, coin) || mempool.isSpent(out)) {// TODO: filtering spent coins should be done by the CCoinsViewMemPool
            return NullUniValue;
        }
    } else {
        if (!pcoinsTip->GetCoin(out, coin)) {
            return NullUniValue;
        }
    }

    BlockMap::iterator it = mapBlockIndex.find(pcoinsTip->GetBestBlock());
    CBlockIndex* pindex = it->second;
    assert(pindex != nullptr);
    ret.pushKV("bestblock", pindex->GetBlockHash().GetHex());
    if (coin.nHeight == MEMPOOL_HEIGHT) {
        ret.pushKV("confirmations", 0);
    } else {
        ret.pushKV("confirmations", (int64_t)(pindex->nHeight - coin.nHeight + 1));
    }
    ret.pushKV("value", ValueFromAmount(coin.out.nValue));
    UniValue o(UniValue::VOBJ);
    ScriptPubKeyToUniv(coin.out.scriptPubKey, o, true);
    ret.pushKV("scriptPubKey", o);
    ret.pushKV("coinbase", (bool)coin.fCoinBase);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return ret;
}

<<<<<<< HEAD
UniValue verifychain(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "verifychain ( numblocks )\n"
            "\nVerifies blockchain database.\n"
            "\nArguments:\n"
            "1. numblocks    (numeric, optional, default=288, 0=all) The number of blocks to check.\n"
            "\nResult:\n"
            "true|false       (boolean) Verified or not\n"
=======
UniValue verifychain(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
        throw std::runtime_error(
            "verifychain ( nblocks )\n"
            "\nVerifies blockchain database.\n"

            "\nArguments:\n"
            "1. nblocks    (numeric, optional, default=288, 0=all) The number of blocks to check.\n"

            "\nResult:\n"
            "true|false       (boolean) Verified or not\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("verifychain", "") + HelpExampleRpc("verifychain", ""));

    LOCK(cs_main);

<<<<<<< HEAD
    int nCheckLevel = 4;
    int nCheckDepth = GetArg("-checkblocks", 288);
    if (params.size() > 0)
        nCheckDepth = params[0].get_int();

    return CVerifyDB().VerifyDB(pcoinsTip, nCheckLevel, nCheckDepth);
}

/** Implementation of IsSuperMajority with better feedback */
static UniValue SoftForkMajorityDesc(int minVersion, CBlockIndex* pindex, int nRequired)
{
    int nFound = 0;
    CBlockIndex* pstart = pindex;
    for (int i = 0; i < Params().ToCheckBlockUpgradeMajority() && pstart != NULL; i++)
    {
        if (pstart->nVersion >= minVersion)
            ++nFound;
        pstart = pstart->pprev;
    }
    UniValue rv(UniValue::VOBJ);
    rv.push_back(Pair("status", nFound >= nRequired));
    rv.push_back(Pair("found", nFound));
    rv.push_back(Pair("required", nRequired));
    rv.push_back(Pair("window", Params().ToCheckBlockUpgradeMajority()));
    return rv;
}
static UniValue SoftForkDesc(const std::string &name, int version, CBlockIndex* pindex)
{
    UniValue rv(UniValue::VOBJ);
    rv.push_back(Pair("id", name));
    rv.push_back(Pair("version", version));
    rv.push_back(Pair("enforce", SoftForkMajorityDesc(version, pindex, Params().EnforceBlockUpgradeMajority())));
    rv.push_back(Pair("reject", SoftForkMajorityDesc(version, pindex, Params().RejectBlockOutdatedMajority())));
    return rv;
}

UniValue getblockchaininfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getblockchaininfo\n"
            "Returns an object containing various state info regarding block chain processing.\n"
            "\nResult:\n"
            "{\n"
            "  \"chain\": \"xxxx\",        (string) current network name as defined in BIP70 (main, test, regtest)\n"
=======
    int nCheckLevel = gArgs.GetArg("-checklevel", DEFAULT_CHECKLEVEL);
    int nCheckDepth = gArgs.GetArg("-checkblocks", DEFAULT_CHECKBLOCKS);
    if (request.params.size() > 0)
        nCheckDepth = request.params[0].get_int();

    return CVerifyDB().VerifyDB(pcoinsTip.get(), nCheckLevel, nCheckDepth);
}

/** Implementation of IsSuperMajority with better feedback */
static UniValue SoftForkMajorityDesc(int version, const CBlockIndex* pindex, const Consensus::Params& consensusParams)
{
    UniValue rv(UniValue::VOBJ);
    Consensus::UpgradeIndex idx;
    switch(version) {
    case 1:
    case 2:
    case 3:
        idx = Consensus::BASE_NETWORK;
        break;
    case 4:
        idx = Consensus::UPGRADE_ZC;
        break;
    case 5:
        idx = Consensus::UPGRADE_BIP65;
        break;
    case 6:
        idx = Consensus::UPGRADE_V3_4;
        break;
    case 7:
        idx = Consensus::UPGRADE_V4_0;
        break;
    default:
        rv.pushKV("status", false);
        return rv;
    }
    rv.pushKV("status", pindex && consensusParams.NetworkUpgradeActive(pindex->nHeight, idx));
    return rv;
}

static UniValue SoftForkDesc(const std::string &name, int version, const CBlockIndex* pindex)
{
    const Consensus::Params& consensus = Params().GetConsensus();
    UniValue rv(UniValue::VOBJ);
    rv.pushKV("id", name);
    rv.pushKV("version", version);
    rv.pushKV("reject", SoftForkMajorityDesc(version, pindex, consensus));
    return rv;
}

static UniValue NetworkUpgradeDesc(const Consensus::Params& consensusParams, Consensus::UpgradeIndex idx, int height)
{
    UniValue rv(UniValue::VOBJ);
    auto upgrade = NetworkUpgradeInfo[idx];
    rv.pushKV("activationheight", consensusParams.vUpgrades[idx].nActivationHeight);
    switch (NetworkUpgradeState(height, consensusParams, idx)) {
        case UPGRADE_DISABLED: rv.pushKV("status", "disabled"); break;
        case UPGRADE_PENDING: rv.pushKV("status", "pending"); break;
        case UPGRADE_ACTIVE: rv.pushKV("status", "active"); break;
    }
    rv.pushKV("info", upgrade.strInfo);
    return rv;
}

void NetworkUpgradeDescPushBack(
        UniValue& networkUpgrades,
        const Consensus::Params& consensusParams,
        Consensus::UpgradeIndex idx,
        int height)
{
    // Network upgrades with an activation height of NO_ACTIVATION_HEIGHT are
    // hidden. This is used when network upgrade implementations are merged
    // without specifying the activation height.
    if (consensusParams.vUpgrades[idx].nActivationHeight != Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT) {
        std::string name = NetworkUpgradeInfo[idx].strName;
        std::replace(name.begin(), name.end(), '_', ' '); // Beautify the name
        networkUpgrades.pushKV(name, NetworkUpgradeDesc(consensusParams, idx, height));
    }
}

UniValue getblockchaininfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getblockchaininfo\n"
            "Returns an object containing various state info regarding block chain processing.\n"

            "\nResult:\n"
            "{\n"
            "  \"chain\": \"xxxx\",        (string) current network name (main, test, regtest)\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"blocks\": xxxxxx,         (numeric) the current number of blocks processed in the server\n"
            "  \"headers\": xxxxxx,        (numeric) the current number of headers we have validated\n"
            "  \"bestblockhash\": \"...\", (string) the hash of the currently best block\n"
            "  \"difficulty\": xxxxxx,     (numeric) the current difficulty\n"
            "  \"verificationprogress\": xxxx, (numeric) estimate of verification progress [0..1]\n"
            "  \"chainwork\": \"xxxx\"     (string) total amount of work in active chain, in hexadecimal\n"
<<<<<<< HEAD
=======
            "  \"shield_pool_value\": {  (object) Chain tip shield pool value\n"
            "    \"chainValue\":        (numeric) Total value held by the Sapling circuit up to and including the chain tip\n"
            "    \"valueDelta\":        (numeric) Change in value held by the Sapling circuit over the chain tip block\n"
            "  },\n"
            "  \"initial_block_downloading\": true|false, (boolean) whether the node is in initial block downloading state or not\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "  \"softforks\": [            (array) status of softforks in progress\n"
            "     {\n"
            "        \"id\": \"xxxx\",        (string) name of softfork\n"
            "        \"version\": xx,         (numeric) block version\n"
<<<<<<< HEAD
            "        \"enforce\": {           (object) progress toward enforcing the softfork rules for new-version blocks\n"
            "           \"status\": xx,       (boolean) true if threshold reached\n"
            "           \"found\": xx,        (numeric) number of blocks with the new version found\n"
            "           \"required\": xx,     (numeric) number of blocks required to trigger\n"
            "           \"window\": xx,       (numeric) maximum size of examined window of recent blocks\n"
            "        },\n"
            "        \"reject\": { ... }      (object) progress toward rejecting pre-softfork blocks (same fields as \"enforce\")\n"
            "     }, ...\n"
            "  ]\n"
            "}\n"
=======
            "        \"reject\": {           (object) progress toward rejecting pre-softfork blocks\n"
            "           \"status\": xx,       (boolean) true if threshold reached\n"
            "        },\n"
            "     }, ...\n"
            "  ],\n"
            "  \"upgrades\": {                (object) status of network upgrades\n"
            "     \"name\" : {                (string) name of upgrade\n"
            "        \"activationheight\": xxxxxx,  (numeric) block height of activation\n"
            "        \"status\": \"xxxx\",      (string) status of upgrade\n"
            "        \"info\": \"xxxx\",        (string) additional information about upgrade\n"
            "     }, ...\n"
            "}\n"
            "  \"warnings\" : \"...\",         (string) any network and blockchain warnings.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getblockchaininfo", "") + HelpExampleRpc("getblockchaininfo", ""));

    LOCK(cs_main);

<<<<<<< HEAD
    UniValue obj(UniValue::VOBJ);
    obj.push_back(Pair("chain", Params().NetworkIDString()));
    obj.push_back(Pair("blocks", (int)chainActive.Height()));
    obj.push_back(Pair("headers", pindexBestHeader ? pindexBestHeader->nHeight : -1));
    obj.push_back(Pair("bestblockhash", chainActive.Tip()->GetBlockHash().GetHex()));
    obj.push_back(Pair("difficulty", (double)GetDifficulty()));
    obj.push_back(Pair("verificationprogress", Checkpoints::GuessVerificationProgress(chainActive.Tip())));
    obj.push_back(Pair("chainwork", chainActive.Tip()->nChainWork.GetHex()));
    CBlockIndex* tip = chainActive.Tip();
    UniValue softforks(UniValue::VARR);
    softforks.push_back(SoftForkDesc("bip65", 5, tip));
    obj.push_back(Pair("softforks", softforks));
=======
    const Consensus::Params& consensusParams = Params().GetConsensus();
    const CBlockIndex* pChainTip = chainActive.Tip();
    int nTipHeight = pChainTip ? pChainTip->nHeight : -1;

    UniValue obj(UniValue::VOBJ);
    obj.pushKV("chain", Params().NetworkIDString());
    obj.pushKV("blocks", nTipHeight);
    obj.pushKV("headers", pindexBestHeader ? pindexBestHeader->nHeight : -1);
    obj.pushKV("bestblockhash", pChainTip ? pChainTip->GetBlockHash().GetHex() : "");
    obj.pushKV("difficulty", (double)GetDifficulty());
    obj.pushKV("verificationprogress", Checkpoints::GuessVerificationProgress(pChainTip));
    obj.pushKV("chainwork", pChainTip ? pChainTip->nChainWork.GetHex() : "");
    // Sapling shield pool value
    obj.pushKV("shield_pool_value", pChainTip ? ValuePoolDesc(pChainTip->nChainSaplingValue, pChainTip->nSaplingValue) : 0);
    obj.pushKV("initial_block_downloading", IsInitialBlockDownload());
    UniValue softforks(UniValue::VARR);
    softforks.push_back(SoftForkDesc("bip65", 5, pChainTip));
    obj.pushKV("softforks",             softforks);
    UniValue upgrades(UniValue::VOBJ);
    for (int i = Consensus::BASE_NETWORK + 1; i < (int) Consensus::MAX_NETWORK_UPGRADES; i++) {
        NetworkUpgradeDescPushBack(upgrades, consensusParams, Consensus::UpgradeIndex(i), nTipHeight);
    }
    obj.pushKV("upgrades", upgrades);
    obj.pushKV("warnings", GetWarnings("statusbar"));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return obj;
}

/** Comparison function for sorting the getchaintips heads.  */
struct CompareBlocksByHeight {
    bool operator()(const CBlockIndex* a, const CBlockIndex* b) const
    {
        /* Make sure that unequal blocks with the same height do not compare
           equal. Use the pointers themselves to make a distinction. */

        if (a->nHeight != b->nHeight)
            return (a->nHeight > b->nHeight);

        return a < b;
    }
};

<<<<<<< HEAD
UniValue getchaintips(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getchaintips\n"
            "Return information about all known tips in the block tree,"
            " including the main chain as well as orphaned branches.\n"
=======
UniValue getchaintips(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getchaintips\n"
            "Return information about all known tips in the block tree,"
            " including the main chain as well as orphaned branches.\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"height\": xxxx,         (numeric) height of the chain tip\n"
            "    \"hash\": \"xxxx\",         (string) block hash of the tip\n"
            "    \"branchlen\": 0          (numeric) zero for main chain\n"
            "    \"status\": \"active\"      (string) \"active\" for the main chain\n"
            "  },\n"
            "  {\n"
<<<<<<< HEAD
            "    \"height\": xxxx,\n"
            "    \"hash\": \"xxxx\",\n"
            "    \"branchlen\": 1          (numeric) length of branch connecting the tip to the main chain\n"
            "    \"status\": \"xxxx\"        (string) status of the chain (active, valid-fork, valid-headers, headers-only, invalid)\n"
            "  }\n"
            "]\n"
=======
            "    \"height\": n,            (numeric) height of the chain tip\n"
            "    \"hash\": \"hash\",       (string) block hash of the tip\n"
            "    \"branchlen\": n,         (numeric) length of branch connecting the tip to the main chain\n"
            "    \"status\": \"xxxx\"      (string) status of the chain (active, valid-fork, valid-headers, headers-only, invalid)\n"
            "  }\n"
            "  ,...\n"
            "]\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "Possible values for status:\n"
            "1.  \"invalid\"               This branch contains at least one invalid block\n"
            "2.  \"headers-only\"          Not all blocks for this branch are available, but the headers are valid\n"
            "3.  \"valid-headers\"         All blocks are available for this branch, but they were never fully validated\n"
            "4.  \"valid-fork\"            This branch is not part of the active chain, but is fully validated\n"
            "5.  \"active\"                This is the tip of the active main chain, which is certainly valid\n"
<<<<<<< HEAD
=======

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getchaintips", "") + HelpExampleRpc("getchaintips", ""));

    LOCK(cs_main);

    /* Build up a list of chain tips.  We start with the list of all
       known blocks, and successively remove blocks that appear as pprev
       of another block.  */
    std::set<const CBlockIndex*, CompareBlocksByHeight> setTips;
<<<<<<< HEAD
    for (const PAIRTYPE(const uint256, CBlockIndex*) & item : mapBlockIndex) 
       if (item.second) 
        setTips.insert(item.second);
    for (const PAIRTYPE(const uint256, CBlockIndex*) & item : mapBlockIndex) {
        if (item.second) {
            const CBlockIndex* pprev = item.second->pprev;
            if (pprev)
                setTips.erase(pprev);
        }
=======
    for (const std::pair<const uint256, CBlockIndex*> & item : mapBlockIndex)
        setTips.insert(item.second);
    for (const std::pair<const uint256, CBlockIndex*> & item : mapBlockIndex) {
        const CBlockIndex* pprev = item.second->pprev;
        if (pprev)
            setTips.erase(pprev);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // Always report the currently active tip.
    setTips.insert(chainActive.Tip());

    /* Construct the output array.  */
    UniValue res(UniValue::VARR);
    for (const CBlockIndex* block : setTips) {
        UniValue obj(UniValue::VOBJ);
<<<<<<< HEAD
        obj.push_back(Pair("height", block->nHeight));
        obj.push_back(Pair("hash", block->phashBlock->GetHex()));

        const int branchLen = block->nHeight - chainActive.FindFork(block)->nHeight;
        obj.push_back(Pair("branchlen", branchLen));

        string status;
=======
        obj.pushKV("height", block->nHeight);
        obj.pushKV("hash", block->phashBlock->GetHex());

        const int branchLen = block->nHeight - chainActive.FindFork(block)->nHeight;
        obj.pushKV("branchlen", branchLen);

        std::string status;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (chainActive.Contains(block)) {
            // This block is part of the currently active chain.
            status = "active";
        } else if (block->nStatus & BLOCK_FAILED_MASK) {
            // This block or one of its ancestors is invalid.
            status = "invalid";
        } else if (block->nChainTx == 0) {
            // This block cannot be connected because full block data for it or one of its parents is missing.
            status = "headers-only";
        } else if (block->IsValid(BLOCK_VALID_SCRIPTS)) {
            // This block is fully validated, but no longer part of the active chain. It was probably the active block once, but was reorganized.
            status = "valid-fork";
        } else if (block->IsValid(BLOCK_VALID_TREE)) {
            // The headers for this block are valid, but it has not been validated. It was probably never part of the most-work chain.
            status = "valid-headers";
        } else {
            // No clue.
            status = "unknown";
        }
<<<<<<< HEAD
        obj.push_back(Pair("status", status));
=======
        obj.pushKV("status", status);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        res.push_back(obj);
    }

    return res;
}

<<<<<<< HEAD
UniValue getfeeinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
                "getfeeinfo blocks\n"
                "\nReturns details of transaction fees over the last n blocks.\n"

                "\nArguments:\n"
                "1. blocks     (int, required) the number of blocks to get transaction data from\n"

                "\nResult:\n"
                "{\n"
                "  \"txcount\": xxxxx                (numeric) Current tx count\n"
                "  \"txbytes\": xxxxx                (numeric) Sum of all tx sizes\n"
                "  \"ttlfee\": xxxxx                 (numeric) Sum of all fees\n"
                "  \"feeperkb\": xxxxx               (numeric) Average fee per kb over the block range\n"
                "  \"rec_highpriorityfee_perkb\": xxxxx    (numeric) Recommended fee per kb to use for a high priority tx\n"
                "}\n"

                "\nExamples:\n" +
                HelpExampleCli("getfeeinfo", "5") + HelpExampleRpc("getfeeinfo", "5"));

    LOCK(cs_main);

    int nBlocks = params[0].get_int();
    int nBestHeight = chainActive.Height();
    int nStartHeight = nBestHeight - nBlocks;
    if (nBlocks < 0 || nStartHeight <= 0)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "invalid start height");

    CAmount nFees = 0;
    int64_t nBytes = 0;
    int64_t nTotal = 0;
    for (int i = nStartHeight; i <= nBestHeight; i++) {
        CBlockIndex* pindex = chainActive[i];
        CBlock block;
        if (!ReadBlockFromDisk(block, pindex))
            throw JSONRPCError(RPC_DATABASE_ERROR, "failed to read block from disk");

        CAmount nValueIn = 0;
        CAmount nValueOut = 0;
        for (const CTransaction& tx : block.vtx) {
            if (tx.IsCoinBase() || tx.IsCoinStake())
                continue;

            for (unsigned int j = 0; j < tx.vin.size(); j++) {
                COutPoint prevout = tx.vin[j].prevout;
                CTransaction txPrev;
                uint256 hashBlock;
                if(!GetTransaction(prevout.hash, txPrev, hashBlock, true))
                    throw JSONRPCError(RPC_DATABASE_ERROR, "failed to read tx from disk");
                nValueIn += txPrev.vout[prevout.n].nValue;
            }

            nFees += tx.nTxFee;
            nBytes += tx.GetSerializeSize(SER_NETWORK, CLIENT_VERSION);
            nTotal++;
        }

        pindex = chainActive.Next(pindex);
        if (!pindex)
            break;
    }

    UniValue obj(UniValue::VOBJ);
    CFeeRate nFeeRate = CFeeRate(nFees, nBytes);
    obj.push_back(Pair("txcount", (int64_t)nTotal));
    obj.push_back(Pair("txbytes", (int64_t)nBytes));
    obj.push_back(Pair("ttlfee", FormatMoney(nFees)));
    obj.push_back(Pair("feeperkb", FormatMoney(nFeeRate.GetFeePerK())));
    obj.push_back(Pair("rec_highpriorityfee_perkb", FormatMoney(nFeeRate.GetFeePerK() + 1000)));

    return obj;
}

UniValue mempoolInfoToJSON()
{
    UniValue ret(UniValue::VOBJ);
    ret.push_back(Pair("size", (int64_t) mempool.size()));
    ret.push_back(Pair("bytes", (int64_t) mempool.GetTotalTxSize()));

    //ret.push_back(Pair("usage", (int64_t) mempool.DynamicMemoryUsage()));
    return ret;
}

UniValue getmempoolinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getmempoolinfo\n"
            "\nReturns details on the active state of the TX memory pool.\n"
            "\nResult:\n"
            "{\n"
            "  \"size\": xxxxx                (numeric) Current tx count\n"
            "  \"bytes\": xxxxx               (numeric) Sum of all tx sizes\n"
            "}\n"
=======
UniValue mempoolInfoToJSON()
{
    UniValue ret(UniValue::VOBJ);
    ret.pushKV("loaded", mempool.IsLoaded());
    ret.pushKV("size", (int64_t) mempool.size());
    ret.pushKV("bytes", (int64_t) mempool.GetTotalTxSize());
    ret.pushKV("usage", (int64_t) mempool.DynamicMemoryUsage());
    size_t maxmempool = gArgs.GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000;
    ret.pushKV("mempoolminfee", ValueFromAmount(std::max(mempool.GetMinFee(maxmempool), ::minRelayTxFee).GetFeePerK()));
    ret.pushKV("minrelaytxfee", ValueFromAmount(::minRelayTxFee.GetFeePerK()));

    return ret;
}

UniValue getmempoolinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getmempoolinfo\n"
            "\nReturns details on the active state of the TX memory pool.\n"

            "\nResult:\n"
            "{\n"
            "  \"loaded\": true|false         (boolean) True if the mempool is fully loaded\n"
            "  \"size\": xxxxx                (numeric) Current tx count\n"
            "  \"bytes\": xxxxx               (numeric) Sum of all tx sizes\n"
            "  \"usage\": xxxxx               (numeric) Total memory usage for the mempool\n"
            "  \"maxmempool\": xxxxx,         (numeric) Maximum memory usage for the mempool\n"
            "  \"mempoolminfee\": xxxxx       (numeric) Minimum fee rate in " + CURRENCY_UNIT + "/kB for tx to be accepted. Is the maximum of minrelaytxfee and minimum mempool fee\n"
            "  \"minrelaytxfee\": xxxxx       (numeric) Current minimum relay fee for transactions\n"
            "}\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getmempoolinfo", "") + HelpExampleRpc("getmempoolinfo", ""));

    return mempoolInfoToJSON();
}

<<<<<<< HEAD
UniValue invalidateblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "invalidateblock \"hash\"\n"
            "\nPermanently marks a block as invalid, as if it violated a consensus rule.\n"
            "\nArguments:\n"
            "1. hash   (string, required) the hash of the block to mark as invalid\n"
            "\nResult:\n"
            "\nExamples:\n" +
            HelpExampleCli("invalidateblock", "\"blockhash\"") + HelpExampleRpc("invalidateblock", "\"blockhash\""));

    std::string strHash = params[0].get_str();
    uint256 hash(strHash);
=======
UniValue invalidateblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "invalidateblock \"blockhash\"\n"
            "\nPermanently marks a block as invalid, as if it violated a consensus rule.\n"

            "\nArguments:\n"
            "1. blockhash   (string, required) the hash of the block to mark as invalid\n"

            "\nExamples:\n" +
            HelpExampleCli("invalidateblock", "\"blockhash\"") + HelpExampleRpc("invalidateblock", "\"blockhash\""));

    std::string strHash = request.params[0].get_str();
    uint256 hash(uint256S(strHash));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CValidationState state;

    {
        LOCK(cs_main);
        if (mapBlockIndex.count(hash) == 0)
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Block not found");

        CBlockIndex* pblockindex = mapBlockIndex[hash];
<<<<<<< HEAD
        InvalidateBlock(state, pblockindex);
=======
        InvalidateBlock(state, Params(), pblockindex);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    if (state.IsValid()) {
        ActivateBestChain(state);
<<<<<<< HEAD
=======
        int nHeight = WITH_LOCK(cs_main, return chainActive.Height(); );
        g_budgetman.SetBestHeight(nHeight);
        mnodeman.SetBestHeight(nHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    if (!state.IsValid()) {
        throw JSONRPCError(RPC_DATABASE_ERROR, state.GetRejectReason());
    }

<<<<<<< HEAD
    return "Done";
}

UniValue resyncfrom(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "resyncfrom \"block height\"\n"
            "\nPermanently marks a block as invalid, as if it violated a consensus rule.\n"
            "\nArguments:\n"
            "1. height   (numeric, required) the hash of the block to mark as invalid\n"
            "\nResult:\n"
            "\nExamples:\n" +
            HelpExampleCli("resyncfrom", "\"height\"") + HelpExampleRpc("resyncfrom", "\"100000\""));

    int height = params[0].get_int();
=======
    return NullUniValue;
}

UniValue reconsiderblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "reconsiderblock \"blockhash\"\n"
            "\nRemoves invalidity status of a block and its descendants, reconsider them for activation.\n"
            "This can be used to undo the effects of invalidateblock.\n"

            "\nArguments:\n"
            "1. blockhash   (string, required) the hash of the block to reconsider\n"

            "\nExamples:\n" +
            HelpExampleCli("reconsiderblock", "\"blockhash\"") + HelpExampleRpc("reconsiderblock", "\"blockhash\""));

    std::string strHash = request.params[0].get_str();
    uint256 hash(uint256S(strHash));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CValidationState state;

    {
        LOCK(cs_main);
<<<<<<< HEAD
        if (chainActive.Height() < height)
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid block height");

        CBlockIndex* pblockindex = chainActive[height];
        InvalidateBlock(state, pblockindex);
=======
        if (mapBlockIndex.count(hash) == 0)
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Block not found");

        CBlockIndex* pblockindex = mapBlockIndex[hash];
        ReconsiderBlock(state, pblockindex);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    if (state.IsValid()) {
        ActivateBestChain(state);
<<<<<<< HEAD
=======
        int nHeight = WITH_LOCK(cs_main, return chainActive.Height(); );
        g_budgetman.SetBestHeight(nHeight);
        mnodeman.SetBestHeight(nHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    if (!state.IsValid()) {
        throw JSONRPCError(RPC_DATABASE_ERROR, state.GetRejectReason());
    }

    return NullUniValue;
}

<<<<<<< HEAD
UniValue reconsiderblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "reconsiderblock \"hash\"\n"
            "\nRemoves invalidity status of a block and its descendants, reconsider them for activation.\n"
            "This can be used to undo the effects of invalidateblock.\n"
            "\nArguments:\n"
            "1. hash   (string, required) the hash of the block to reconsider\n"
            "\nResult:\n"
            "\nExamples:\n" +
            HelpExampleCli("reconsiderblock", "\"blockhash\"") + HelpExampleRpc("reconsiderblock", "\"blockhash\""));

    std::string strHash = params[0].get_str();
    uint256 hash(strHash);
    CValidationState state;

    {
        LOCK(cs_main);
        if (mapBlockIndex.count(hash) == 0)
            throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Block not found");

        CBlockIndex* pblockindex = mapBlockIndex[hash];
        ReconsiderBlock(state, pblockindex);
    }

    if (state.IsValid()) {
        ActivateBestChain(state);
    }

    if (!state.IsValid()) {
        throw JSONRPCError(RPC_DATABASE_ERROR, state.GetRejectReason());
    }

    return "Done";
}

UniValue getinvalid (const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
                "getinvalid \n"
                        "\nGet a summary of invalidated outpoints.\n"
                        "\nArguments:\n"
                        "1. all   (string, optional) return a full list of outpoints even if they are spent\n"
                        "\nExamples:\n" +
                HelpExampleCli("getinvalid", "\"all\"") + HelpExampleRpc("getinvalid", "\"all\""));

    string strCommand;
    if (params.size() == 1){
        strCommand = params[0].get_str();
    }

    if (strCommand == "serials") {
        UniValue ret(UniValue::VARR);
        CAmount nSerialTotal = 0;
        for (auto it : mapInvalidSerials) {
            UniValue objSerial(UniValue::VOBJ);
            objSerial.push_back(Pair(it.first.GetHex(), FormatMoney(it.second)));
            nSerialTotal += it.second;
            ret.push_back(objSerial);
        }

        UniValue objTotal(UniValue::VOBJ);
        objTotal.push_back(Pair("total_value", FormatMoney(nSerialTotal)));
        ret.push_back(objTotal);
        return ret;
    }

    bool fShowAll = false;
    if (strCommand == "all")
        fShowAll = true;

    CAmount nUnspent = 0;
    CAmount nMint = 0;
    CAmount nMixedValid = 0;
    map<CBitcoinAddress, CAmount> mapBanAddress;
    map<COutPoint, int> mapMixedValid;

    UniValue ret(UniValue::VARR);
    for (auto it : mapInvalidOutPoints) {
        COutPoint out = it.first;
        //Get the tx that the outpoint is from
        CTransaction tx;
        uint256 hashBlock;
        if (!GetTransaction(out.hash, tx, hashBlock, true)) {
            continue;
        }

        UniValue objTx(UniValue::VOBJ);
        objTx.push_back(Pair("inv_out", it.first.ToString()));

        CAmount nValue = tx.vout[out.n].nValue;
        objTx.push_back(Pair("value", FormatMoney(nValue)));

        //Search the txin's to see if any of them are "valid".
        UniValue objMixedValid(UniValue::VOBJ);

        //if some of the other inputs are valid
        for(CTxIn in2 : tx.vin) {
            //See if this is already accounted for
            if(mapInvalidOutPoints.count(in2.prevout) || mapMixedValid.count(in2.prevout))
                continue;

            CTransaction txPrev;
            uint256 hashBlock;
            if(!GetTransaction(in2.prevout.hash, txPrev, hashBlock, true))
                continue;

            //This is a valid outpoint that mixed with an invalid outpoint. Investigate this person.
            //Information leakage, not covering their tracks well enough
            CAmount nValid = txPrev.vout[in2.prevout.n].nValue;
            objMixedValid.push_back(Pair(FormatMoney(nValid), in2.prevout.ToString()));

            nMixedValid += nValid;
            mapMixedValid[in2.prevout] = 1;
        }

        //Check whether this bad outpoint has been spent
        bool fSpent = false;
        CCoinsViewCache cache(pcoinsTip);
        const CCoins* coins = cache.AccessCoins(out.hash);
        if (!coins || !coins->IsAvailable(out.n))
            fSpent = true;

        objTx.push_back(Pair("spent", fSpent));
        if (!objMixedValid.empty())
            objTx.push_back(Pair("mixed_with_valid", objMixedValid));

        CScript scriptPubKey = tx.vout[out.n].scriptPubKey;
        if (!fSpent) {
            CTxDestination dest;
            if (!ExtractDestination(scriptPubKey, dest)) {
                continue;
            }
            CBitcoinAddress address(dest);
            mapBanAddress[address] += nValue;
            nUnspent += nValue;
        }

        if (fSpent && !fShowAll)
            continue;

        ret.push_back(objTx);
    }

    UniValue objAddresses(UniValue::VOBJ);
    for (auto it : mapBanAddress)
        objAddresses.push_back(Pair(it.first.ToString(), FormatMoney(it.second)));

    UniValue obj(UniValue::VOBJ);
    obj.push_back(Pair("addresses_with_invalid", objAddresses));
    obj.push_back(Pair("total_unspent", FormatMoney(nUnspent)));
    obj.push_back(Pair("total_minted", FormatMoney(nMint)));
    obj.push_back(Pair("total_valid_used", FormatMoney(nMixedValid)));

    ret.push_back(obj);
    return ret;
}

UniValue setmaxreorgdepth(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 1)
        throw runtime_error(
            "setmaxreorgdepth <value>\n"
            "\nSet max reorganization depth to a value.\n"
            "\nArguments:\n"
            "1. num   (numeric, required) the number of blocks\n"
            "\nResult:\n"
            "\nExamples:\n" +
            HelpExampleCli("setmaxreorgdepth", "100") + HelpExampleRpc("setmaxreorgdepth", "100"));

    int num = params[0].get_int();
    if (num <= 5) 
        throw runtime_error("Invalid number");
    {
        LOCK(cs_main);
        Params().ChangeMaxReorg(num);
    }

    return NullUniValue;
=======
void validaterange(const UniValue& params, int& heightStart, int& heightEnd, int minHeightStart = 1)
{
    if (params.size() < 2) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Not enough parameters in validaterange");
    }

    int nBestHeight;
    {
        LOCK(cs_main);
        nBestHeight = chainActive.Height();
    }

    heightStart = params[0].get_int();
    if (heightStart > nBestHeight) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Invalid starting block (%d). Out of range.", heightStart));
    }

    const int range = params[1].get_int();
    if (range < 1) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid block range. Must be strictly positive.");
    }

    heightEnd = heightStart + range - 1;

    if (heightStart < minHeightStart && heightEnd >= minHeightStart) {
        heightStart = minHeightStart;
    }

    if (heightEnd > nBestHeight) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Invalid ending block (%d). Out of range.", heightEnd));
    }
}

UniValue getblockindexstats(const JSONRPCRequest& request) {
    if (request.fHelp || request.params.size() != 2)
        throw std::runtime_error(
                "getblockindexstats height range\n"
                "\nReturns aggregated BlockIndex data for blocks "
                "\n[height, height+1, height+2, ..., height+range-1]\n"

                "\nArguments:\n"
                "1. height             (numeric, required) block height where the search starts.\n"
                "2. range              (numeric, required) number of blocks to include.\n"

                "\nResult:\n"
                "{\n"
                "  \"first_block\": \"x\"            (integer) First counted block\n"
                "  \"last_block\": \"x\"             (integer) Last counted block\n"
                "  \"txcount\": xxxxx                (numeric) tx count (excluding coinbase/coinstake)\n"
                "  \"txcount_all\": xxxxx            (numeric) tx count (including coinbase/coinstake)\n"
                "  \"txbytes\": xxxxx                (numeric) Sum of the size of all txes over block range\n"
                "  \"ttlfee\": xxxxx                 (numeric) Sum of the fee amount of all txes over block range\n"
                "  \"feeperkb\": xxxxx               (numeric) Average fee per kb (excluding zc txes)\n"
                "}\n"

                "\nExamples:\n" +
                HelpExampleCli("getblockindexstats", "1200000 1000") +
                HelpExampleRpc("getblockindexstats", "1200000, 1000"));

    int heightStart, heightEnd;
    validaterange(request.params, heightStart, heightEnd);
    // return object
    UniValue ret(UniValue::VOBJ);
    ret.pushKV("Starting block", heightStart);
    ret.pushKV("Ending block", heightEnd);

    CAmount nFees = 0;
    int64_t nBytes = 0;
    int64_t nTxCount = 0;
    int64_t nTxCount_all = 0;

    const CBlockIndex* pindex = WITH_LOCK(cs_main, return chainActive[heightEnd]);
    if (!pindex)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "invalid block height");

    while (pindex && pindex->nHeight >= heightStart) {
        CBlock block;
        if (!ReadBlockFromDisk(block, pindex)) {
            throw JSONRPCError(RPC_DATABASE_ERROR, "failed to read block from disk");
        }

        CAmount nValueIn = 0;
        CAmount nValueOut = 0;
        const int ntx = block.vtx.size();
        const int firstTxIndex = block.IsProofOfStake() ? 2 : 1;
        nTxCount_all += ntx;
        nTxCount = nTxCount + ntx - firstTxIndex;

        // loop through each tx in block and save size and fee (except for coinbase/coinstake)
        for (int idx = firstTxIndex; idx < ntx; idx++) {
            const CTransaction& tx = *(block.vtx[idx]);

            // zerocoin txes have fixed fee, don't count them here.
            if (tx.ContainsZerocoins())
                continue;

            // Transaction size
            nBytes += GetSerializeSize(tx, CLIENT_VERSION);

            // Transparent inputs
            for (unsigned int j = 0; j < tx.vin.size(); j++) {
                COutPoint prevout = tx.vin[j].prevout;
                CTransactionRef txPrev;
                uint256 hashBlock;
                if(!GetTransaction(prevout.hash, txPrev, hashBlock, true))
                    throw JSONRPCError(RPC_DATABASE_ERROR, "failed to read tx from disk");
                nValueIn += txPrev->vout[prevout.n].nValue;
            }
            // Shield inputs
            nValueIn += tx.GetShieldedValueIn();

            // Tranparent/Shield outputs
            nValueOut += tx.GetValueOut();

            // update fee
            nFees += nValueIn - nValueOut;
        }
        pindex = pindex->pprev;
    }

    // get fee rate
    CFeeRate nFeeRate = CFeeRate(nFees, nBytes);

    // return UniValue object
    ret.pushKV("txcount", (int64_t)nTxCount);
    ret.pushKV("txcount_all", (int64_t)nTxCount_all);
    ret.pushKV("txbytes", (int64_t)nBytes);
    ret.pushKV("ttlfee", FormatMoney(nFees));
    ret.pushKV("feeperkb", FormatMoney(nFeeRate.GetFeePerK()));

    return ret;
}

UniValue getfeeinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "getfeeinfo blocks\n"
            "\nReturns details of transaction fees over the last n blocks.\n"

            "\nArguments:\n"
            "1. blocks     (int, required) the number of blocks to get transaction data from\n"

            "\nResult:\n"
            "{\n"
            "  \"txcount\": xxxxx                (numeric) Current tx count\n"
            "  \"txbytes\": xxxxx                (numeric) Sum of all tx sizes\n"
            "  \"ttlfee\": xxxxx                 (numeric) Sum of all fees\n"
            "  \"feeperkb\": xxxxx               (numeric) Average fee per kb over the block range\n"
            "  \"rec_highpriorityfee_perkb\": xxxxx    (numeric) Recommended fee per kb to use for a high priority tx\n"
            "}\n"

            "\nExamples:\n" +
            HelpExampleCli("getfeeinfo", "5") + HelpExampleRpc("getfeeinfo", "5"));

    int nBlocks = request.params[0].get_int();
    int nBestHeight;
    {
        LOCK(cs_main);
        nBestHeight = chainActive.Height();
    }
    int nStartHeight = nBestHeight - nBlocks;
    if (nBlocks < 0 || nStartHeight <= 0)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "invalid start height");

    JSONRPCRequest newRequest;
    UniValue newParams(UniValue::VARR);
    newParams.push_back(UniValue(nStartHeight));
    newParams.push_back(UniValue(nBlocks));
    newRequest.params = newParams;

    return getblockindexstats(newRequest);
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         okSafe argNames
  //  --------------------- ------------------------  -----------------------  ------ --------
    { "blockchain",         "getbestblockhash",       &getbestblockhash,       true,  {} },
    { "blockchain",         "getbestsaplinganchor",   &getbestsaplinganchor,   true,  {} },
    { "blockchain",         "getblock",               &getblock,               true,  {"blockhash","verbose"} },
    { "blockchain",         "getblockchaininfo",      &getblockchaininfo,      true,  {} },
    { "blockchain",         "getblockcount",          &getblockcount,          true,  {} },
    { "blockchain",         "getblockhash",           &getblockhash,           true,  {"height"} },
    { "blockchain",         "getblockheader",         &getblockheader,         false, {"blockhash","verbose"} },
    { "blockchain",         "getblockindexstats",     &getblockindexstats,     true,  {"height","range"} },
    { "blockchain",         "getchaintips",           &getchaintips,           true,  {} },
    { "blockchain",         "getdifficulty",          &getdifficulty,          true,  {} },
    { "blockchain",         "getfeeinfo",             &getfeeinfo,             true,  {"blocks"} },
    { "blockchain",         "getmempoolinfo",         &getmempoolinfo,         true,  {} },
    { "blockchain",         "getrawmempool",          &getrawmempool,          true,  {"verbose"} },
    { "blockchain",         "getsupplyinfo",          &getsupplyinfo,          true,  {"force_update"} },
    { "blockchain",         "gettxout",               &gettxout,               true,  {"txid","n","include_mempool"} },
    { "blockchain",         "gettxoutsetinfo",        &gettxoutsetinfo,        true,  {} },
    { "blockchain",         "verifychain",            &verifychain,            true,  {"nblocks"} },

    /* Not shown in help */
    { "hidden",             "invalidateblock",        &invalidateblock,        true,  {"blockhash"} },
    { "hidden",             "reconsiderblock",        &reconsiderblock,        true,  {"blockhash"} },
    { "hidden",             "waitforblock",           &waitforblock,           true,  {"blockhash","timeout"} },
    { "hidden",             "waitforblockheight",     &waitforblockheight,     true,  {"height","timeout"} },
    { "hidden",             "waitfornewblock",        &waitfornewblock,        true,  {"timeout"} },
    { "hidden",             "syncwithvalidationinterfacequeue", &syncwithvalidationinterfacequeue, true,  {} },


};

void RegisterBlockchainRPCCommands(CRPCTable &tableRPC)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        tableRPC.appendCommand(commands[vcidx].name, &commands[vcidx]);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
