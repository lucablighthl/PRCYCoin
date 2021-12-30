<<<<<<< HEAD
// Copyright (c) 2014-2015 The Dash Developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "activemasternode.h"
#include "wallet/db.h"
#include "init.h"
#include "main.h"
#include "masternode-budget.h"
#include "masternode-payments.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "rpc/server.h"
#include "utilmoneystr.h"

#include <univalue.h>

#include <fstream>

using namespace std;

void budgetToJSON(CBudgetProposal* pbudgetProposal, UniValue& bObj)
{
    CTxDestination address1;
    ExtractDestination(pbudgetProposal->GetPayee(), address1);
    CBitcoinAddress address2(address1);

    bObj.push_back(Pair("Name", pbudgetProposal->GetName()));
    bObj.push_back(Pair("URL", pbudgetProposal->GetURL()));
    bObj.push_back(Pair("Hash", pbudgetProposal->GetHash().ToString()));
    bObj.push_back(Pair("FeeHash", pbudgetProposal->nFeeTXHash.ToString()));
    bObj.push_back(Pair("BlockStart", (int64_t)pbudgetProposal->GetBlockStart()));
    bObj.push_back(Pair("BlockEnd", (int64_t)pbudgetProposal->GetBlockEnd()));
    bObj.push_back(Pair("TotalPaymentCount", (int64_t)pbudgetProposal->GetTotalPaymentCount()));
    bObj.push_back(Pair("RemainingPaymentCount", (int64_t)pbudgetProposal->GetRemainingPaymentCount()));
    bObj.push_back(Pair("PaymentAddress", address2.ToString()));
    bObj.push_back(Pair("Ratio", pbudgetProposal->GetRatio()));
    bObj.push_back(Pair("Yeas", (int64_t)pbudgetProposal->GetYeas()));
    bObj.push_back(Pair("Nays", (int64_t)pbudgetProposal->GetNays()));
    bObj.push_back(Pair("Abstains", (int64_t)pbudgetProposal->GetAbstains()));
    bObj.push_back(Pair("TotalPayment", ValueFromAmount(pbudgetProposal->GetAmount() * pbudgetProposal->GetTotalPaymentCount())));
    bObj.push_back(Pair("MonthlyPayment", ValueFromAmount(pbudgetProposal->GetAmount())));
    bObj.push_back(Pair("IsEstablished", pbudgetProposal->IsEstablished()));

    std::string strError = "";
    bObj.push_back(Pair("IsValid", pbudgetProposal->IsValid(strError)));
    bObj.push_back(Pair("IsValidReason", strError.c_str()));
    bObj.push_back(Pair("fValid", pbudgetProposal->fValid));
}

UniValue preparebudget(const UniValue& params, bool fHelp)
{
    int nBlockMin = 0;
    CBlockIndex* pindexPrev = chainActive.Tip();

    if (fHelp || params.size() != 6)
        throw runtime_error(
            "preparebudget \"proposal-name\" \"url\" payment-count block-start \"prcycoin-address\" monthly-payment\n"
            "\nPrepare proposal for network by signing and creating tx\n"

            "\nArguments:\n"
            "1. \"proposal-name\":  (string, required) Desired proposal name (20 character limit)\n"
            "2. \"url\":            (string, required) URL of proposal details (64 character limit)\n"
            "3. payment-count:    (numeric, required) Total number of monthly payments\n"
            "4. block-start:      (numeric, required) Starting super block height\n"
            "5. \"prcycoin-address\":   (string, required) PRCY address to send payments to\n"
            "6. monthly-payment:  (numeric, required) Monthly payment amount\n"

            "\nResult:\n"
            "\"xxxx\"       (string) proposal fee hash (if successful) or error message (if failed)\n"
            "\nExamples:\n" +
            HelpExampleCli("preparebudget", "\"test-proposal\" \"https://forum.prcycoin.com/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500") +
            HelpExampleRpc("preparebudget", "\"test-proposal\" \"https://forum.prcycoin.com/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500"));

    LOCK2(cs_main, pwalletMain->cs_wallet);

    if (pwalletMain->IsLocked())
        throw JSONRPCError(RPC_WALLET_UNLOCK_NEEDED, "Error: Please enter the wallet passphrase with unlockwallet first.");

    std::string strProposalName = SanitizeString(params[0].get_str());
    if (strProposalName.size() > 20)
        throw runtime_error("Invalid proposal name, limit of 20 characters.");

    std::string strURL = SanitizeString(params[1].get_str());
    if (strURL.size() > 64)
        throw runtime_error("Invalid url, limit of 64 characters.");

    int nPaymentCount = params[2].get_int();
    if (nPaymentCount < 1)
        throw runtime_error("Invalid payment count, must be more than zero.");

    // Start must be in the next budget cycle
    if (pindexPrev != NULL) nBlockMin = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();

    int nBlockStart = params[3].get_int();
    if (nBlockStart % GetBudgetPaymentCycleBlocks() != 0) {
        int nNext = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();
        throw runtime_error(strprintf("Invalid block start - must be a budget cycle block. Next valid block: %d", nNext));
    }

    int nBlockEnd = nBlockStart + GetBudgetPaymentCycleBlocks() * nPaymentCount; // End must be AFTER current cycle

    if (nBlockStart < nBlockMin)
        throw runtime_error("Invalid block start, must be more than current height.");

    if (nBlockEnd < pindexPrev->nHeight)
        throw runtime_error("Invalid ending block, starting block + (payment_cycle*payments) must be more than current height.");

    CBitcoinAddress address(params[4].get_str());
    if (!address.IsValid())
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Prcycoin address");

    // Parse Prcycoin address
    CScript scriptPubKey = GetScriptForDestination(address.Get());
    CAmount nAmount = AmountFromValue(params[5]);

    //*************************************************************************

    // create transaction 15 minutes into the future, to allow for confirmation time
    CBudgetProposalBroadcast budgetProposalBroadcast(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, 0);

    std::string strError = "";
    if (!budgetProposalBroadcast.IsValid(strError, false))
        throw runtime_error("Proposal is not valid - " + budgetProposalBroadcast.GetHash().ToString() + " - " + strError);

    bool useIX = false; //true;

    CWalletTx wtx;
    if (!pwalletMain->GetBudgetSystemCollateralTX(wtx, budgetProposalBroadcast.GetHash(), useIX)) {
        throw runtime_error("Error making collateral transaction for proposal. Please check your wallet balance.");
    }

    // make our change address
    CReserveKey reservekey(pwalletMain);
    //send the tx to the network
    pwalletMain->CommitTransaction(wtx, reservekey, useIX ? "ix" : "tx");

    return wtx.GetHash().ToString();
}

UniValue submitbudget(const UniValue& params, bool fHelp)
{
    int nBlockMin = 0;
    CBlockIndex* pindexPrev = chainActive.Tip();

    if (fHelp || params.size() != 7)
        throw runtime_error(
            "submitbudget \"proposal-name\" \"url\" payment-count block-start \"prcycoin-address\" monthly-payment \"fee-tx\"\n"
            "\nSubmit proposal to the network\n"

            "\nArguments:\n"
            "1. \"proposal-name\":  (string, required) Desired proposal name (20 character limit)\n"
            "2. \"url\":            (string, required) URL of proposal details (64 character limit)\n"
            "3. payment-count:    (numeric, required) Total number of monthly payments\n"
            "4. block-start:      (numeric, required) Starting super block height\n"
            "5. \"prcycoin-address\":   (string, required) PRCY address to send payments to\n"
            "6. monthly-payment:  (numeric, required) Monthly payment amount\n"
            "7. \"fee-tx\":         (string, required) Transaction hash from preparebudget command\n"

            "\nResult:\n"
            "\"xxxx\"       (string) proposal hash (if successful) or error message (if failed)\n"
            "\nExamples:\n" +
            HelpExampleCli("submitbudget", "\"test-proposal\" \"https://forum.prcycoin.com/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500") +
            HelpExampleRpc("submitbudget", "\"test-proposal\" \"https://forum.prcycoin.com/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500"));

    // Check these inputs the same way we check the vote commands:
    // **********************************************************

    std::string strProposalName = SanitizeString(params[0].get_str());
    if (strProposalName.size() > 20)
        throw runtime_error("Invalid proposal name, limit of 20 characters.");

    std::string strURL = SanitizeString(params[1].get_str());
    if (strURL.size() > 64)
        throw runtime_error("Invalid url, limit of 64 characters.");

    int nPaymentCount = params[2].get_int();
    if (nPaymentCount < 1)
        throw runtime_error("Invalid payment count, must be more than zero.");

    // Start must be in the next budget cycle
    if (pindexPrev != NULL) nBlockMin = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();

    int nBlockStart = params[3].get_int();
    if (nBlockStart % GetBudgetPaymentCycleBlocks() != 0) {
        int nNext = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();
        throw runtime_error(strprintf("Invalid block start - must be a budget cycle block. Next valid block: %d", nNext));
    }

    int nBlockEnd = nBlockStart + (GetBudgetPaymentCycleBlocks() * nPaymentCount); // End must be AFTER current cycle

    if (nBlockStart < nBlockMin)
        throw runtime_error("Invalid block start, must be more than current height.");

    if (nBlockEnd < pindexPrev->nHeight)
        throw runtime_error("Invalid ending block, starting block + (payment_cycle*payments) must be more than current height.");

    CBitcoinAddress address(params[4].get_str());
    if (!address.IsValid())
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Prcycoin address");

    // Parse Prcycoin address
    CScript scriptPubKey = GetScriptForDestination(address.Get());
    CAmount nAmount = AmountFromValue(params[5]);
    uint256 hash = ParseHashV(params[6], "parameter 1");

    //create the proposal incase we're the first to make it
    CBudgetProposalBroadcast budgetProposalBroadcast(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, hash);

    std::string strError = "";
    int nConf = 0;
    if (!IsBudgetCollateralValid(hash, budgetProposalBroadcast.GetHash(), strError, budgetProposalBroadcast.nTime, nConf)) {
        throw runtime_error("Proposal FeeTX is not valid - " + hash.ToString() + " - " + strError);
    }

    if (!masternodeSync.IsBlockchainSynced()) {
        throw runtime_error("Must wait for client to sync with masternode network. Try again in a minute or so.");
    }

    budget.mapSeenMasternodeBudgetProposals.insert(make_pair(budgetProposalBroadcast.GetHash(), budgetProposalBroadcast));
    budgetProposalBroadcast.Relay();
    if(budget.AddProposal(budgetProposalBroadcast)) {
        return budgetProposalBroadcast.GetHash().ToString();
    }
    throw runtime_error("Invalid proposal, see debug.log for details.");
}

UniValue mnbudgetvote(const UniValue& params, bool fHelp)
{
    std::string strCommand;
    if (params.size() >= 1) {
        strCommand = params[0].get_str();
=======
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "budget/budgetmanager.h"
#include "budget/budgetutil.h"
#include "db.h"
#include "evo/deterministicmns.h"
#include "key_io.h"
#include "masternode-payments.h"
#include "masternode-sync.h"
#include "masternodeconfig.h"
#include "masternodeman.h"
#include "messagesigner.h"
#include "rpc/server.h"
#include "utilmoneystr.h"
#ifdef ENABLE_WALLET
#include "wallet/wallet.h"
#include "wallet/rpcwallet.h"
#endif

#include <univalue.h>

void budgetToJSON(const CBudgetProposal* pbudgetProposal, UniValue& bObj, int nCurrentHeight)
{
    CTxDestination address1;
    ExtractDestination(pbudgetProposal->GetPayee(), address1);

    bObj.pushKV("Name", pbudgetProposal->GetName());
    bObj.pushKV("URL", pbudgetProposal->GetURL());
    bObj.pushKV("Hash", pbudgetProposal->GetHash().ToString());
    bObj.pushKV("FeeHash", pbudgetProposal->GetFeeTXHash().ToString());
    bObj.pushKV("BlockStart", (int64_t)pbudgetProposal->GetBlockStart());
    bObj.pushKV("BlockEnd", (int64_t)pbudgetProposal->GetBlockEnd());
    bObj.pushKV("TotalPaymentCount", (int64_t)pbudgetProposal->GetTotalPaymentCount());
    bObj.pushKV("RemainingPaymentCount", (int64_t)pbudgetProposal->GetRemainingPaymentCount(nCurrentHeight));
    bObj.pushKV("PaymentAddress", EncodeDestination(address1));
    bObj.pushKV("Ratio", pbudgetProposal->GetRatio());
    bObj.pushKV("Yeas", (int64_t)pbudgetProposal->GetYeas());
    bObj.pushKV("Nays", (int64_t)pbudgetProposal->GetNays());
    bObj.pushKV("Abstains", (int64_t)pbudgetProposal->GetAbstains());
    bObj.pushKV("TotalPayment", ValueFromAmount(pbudgetProposal->GetAmount() * pbudgetProposal->GetTotalPaymentCount()));
    bObj.pushKV("MonthlyPayment", ValueFromAmount(pbudgetProposal->GetAmount()));
    bObj.pushKV("IsEstablished", pbudgetProposal->IsEstablished());
    bool fValid = pbudgetProposal->IsValid();
    bObj.pushKV("IsValid", fValid);
    if (!fValid)
        bObj.pushKV("IsInvalidReason", pbudgetProposal->IsInvalidReason());
    bObj.pushKV("Allotted", ValueFromAmount(pbudgetProposal->GetAllotted()));
}

void checkBudgetInputs(const UniValue& params, std::string &strProposalName, std::string &strURL,
                       int &nPaymentCount, int &nBlockStart, CTxDestination &address, CAmount &nAmount)
{
    strProposalName = SanitizeString(params[0].get_str());
    if (strProposalName.size() > 20)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid proposal name, limit of 20 characters.");

    strURL = SanitizeString(params[1].get_str());
    std::string strErr;
    if (!validateURL(strURL, strErr))
        throw JSONRPCError(RPC_INVALID_PARAMETER, strErr);

    nPaymentCount = params[2].get_int();
    if (nPaymentCount < 1)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid payment count, must be more than zero.");

    int nMaxPayments = Params().GetConsensus().nMaxProposalPayments;
    if (nPaymentCount > nMaxPayments) {
        throw JSONRPCError(RPC_INVALID_PARAMETER,
                strprintf("Invalid payment count, must be <= %d", nMaxPayments));
    }

    CBlockIndex* pindexPrev = GetChainTip();
    if (!pindexPrev)
        throw JSONRPCError(RPC_IN_WARMUP, "Try again after active chain is loaded");

    // Start must be in the next budget cycle or later
    const int budgetCycleBlocks = Params().GetConsensus().nBudgetCycleBlocks;
    int pHeight = pindexPrev->nHeight;

    int nBlockMin = pHeight - (pHeight % budgetCycleBlocks) + budgetCycleBlocks;

    nBlockStart = params[3].get_int();
    if ((nBlockStart < nBlockMin) || ((nBlockStart % budgetCycleBlocks) != 0))
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Invalid block start - must be a budget cycle block. Next valid block: %d", nBlockMin));

    address = DecodeDestination(params[4].get_str());
    if (!IsValidDestination(address))
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid PIVX address");

    nAmount = AmountFromValue(params[5]);
    if (nAmount < 10 * COIN)
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Invalid amount - Payment of %s is less than minimum 10 %s allowed", FormatMoney(nAmount), CURRENCY_UNIT));

    const CAmount& nTotalBudget = g_budgetman.GetTotalBudget(nBlockStart);
    if (nAmount > nTotalBudget)
        throw JSONRPCError(RPC_INVALID_PARAMETER, strprintf("Invalid amount - Payment of %s more than max of %s", FormatMoney(nAmount), FormatMoney(nTotalBudget)));
}

UniValue preparebudget(const JSONRPCRequest& request)
{
    CWallet * const pwallet = GetWalletForJSONRPCRequest(request);

    if (!EnsureWalletIsAvailable(pwallet, request.fHelp))
        return NullUniValue;

    if (request.fHelp || request.params.size() != 6)
        throw std::runtime_error(
            "preparebudget \"name\" \"url\" npayments start \"address\" monthly_payment\n"
            "\nPrepare proposal for network by signing and creating tx\n"

            "\nArguments:\n"
            "1. \"name\":        (string, required) Desired proposal name (20 character limit)\n"
            "2. \"url\":         (string, required) URL of proposal details (64 character limit)\n"
            "3. npayments:       (numeric, required) Total number of monthly payments\n"
            "4. start:           (numeric, required) Starting super block height\n"
            "5. \"address\":     (string, required) PIVX address to send payments to\n"
            "6. monthly_payment: (numeric, required) Monthly payment amount\n"

            "\nResult:\n"
            "\"xxxx\"       (string) proposal fee hash (if successful) or error message (if failed)\n"

            "\nExamples:\n" +
            HelpExampleCli("preparebudget", "\"test-proposal\" \"https://forum.pivx.org/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500") +
            HelpExampleRpc("preparebudget", "\"test-proposal\" \"https://forum.pivx.org/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500"));

    LOCK2(cs_main, pwallet->cs_wallet);

    EnsureWalletIsUnlocked(pwallet);

    std::string strProposalName;
    std::string strURL;
    int nPaymentCount;
    int nBlockStart;
    CTxDestination address;
    CAmount nAmount;

    checkBudgetInputs(request.params, strProposalName, strURL, nPaymentCount, nBlockStart, address, nAmount);

    // Parse PIVX address
    CScript scriptPubKey = GetScriptForDestination(address);

    // create transaction 15 minutes into the future, to allow for confirmation time
    CBudgetProposal proposal(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, UINT256_ZERO);
    const uint256& nHash = proposal.GetHash();
    if (!proposal.IsWellFormed(g_budgetman.GetTotalBudget(proposal.GetBlockStart())))
        throw std::runtime_error("Proposal is not valid " + proposal.IsInvalidReason());

    CTransactionRef wtx;
    // make our change address
    CReserveKey keyChange(pwallet);
    if (!pwallet->CreateBudgetFeeTX(wtx, nHash, keyChange, BUDGET_FEE_TX_OLD)) { // 50 PIV collateral for proposal
        throw std::runtime_error("Error making collateral transaction for proposal. Please check your wallet balance.");
    }

    //send the tx to the network
    const CWallet::CommitResult& res = pwallet->CommitTransaction(wtx, keyChange, g_connman.get());
    if (res.status != CWallet::CommitStatus::OK)
        throw JSONRPCError(RPC_WALLET_ERROR, res.ToString());

    // Store proposal name as a comment
    auto it = pwallet->mapWallet.find(wtx->GetHash());
    assert(it != pwallet->mapWallet.end());
    it->second.SetComment("Proposal: " + strProposalName);

    return wtx->GetHash().ToString();
}

UniValue submitbudget(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 7)
        throw std::runtime_error(
            "submitbudget \"name\" \"url\" npayments start \"address\" monthly_payment \"fee_txid\"\n"
            "\nSubmit proposal to the network\n"

            "\nArguments:\n"
            "1. \"name\":         (string, required) Desired proposal name (20 character limit)\n"
            "2. \"url\":          (string, required) URL of proposal details (64 character limit)\n"
            "3. npayments:        (numeric, required) Total number of monthly payments\n"
            "4. start:            (numeric, required) Starting super block height\n"
            "5. \"address\":      (string, required) PIVX address to send payments to\n"
            "6. monthly_payment:  (numeric, required) Monthly payment amount\n"
            "7. \"fee_txid\":     (string, required) Transaction hash from preparebudget command\n"

            "\nResult:\n"
            "\"xxxx\"       (string) proposal hash (if successful) or error message (if failed)\n"

            "\nExamples:\n" +
            HelpExampleCli("submitbudget", "\"test-proposal\" \"https://forum.pivx.org/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500") +
            HelpExampleRpc("submitbudget", "\"test-proposal\" \"https://forum.pivx.org/t/test-proposal\" 2 820800 \"D9oc6C3dttUbv8zd7zGNq1qKBGf4ZQ1XEE\" 500"));

    std::string strProposalName;
    std::string strURL;
    int nPaymentCount;
    int nBlockStart;
    CTxDestination address;
    CAmount nAmount;

    checkBudgetInputs(request.params, strProposalName, strURL, nPaymentCount, nBlockStart, address, nAmount);

    // Parse PIVX address
    CScript scriptPubKey = GetScriptForDestination(address);
    const uint256& hash = ParseHashV(request.params[6], "parameter 1");

    if (!masternodeSync.IsBlockchainSynced()) {
        throw std::runtime_error("Must wait for client to sync with masternode network. Try again in a minute or so.");
    }

    // create the proposal in case we're the first to make it
    CBudgetProposal proposal(strProposalName, strURL, nPaymentCount, scriptPubKey, nAmount, nBlockStart, hash);
    if(!g_budgetman.AddProposal(proposal)) {
        std::string strError = strprintf("invalid budget proposal - %s", proposal.IsInvalidReason());
        throw std::runtime_error(strError);
    }
    proposal.Relay();

    return proposal.GetHash().ToString();
}

static CBudgetVote::VoteDirection parseVote(const std::string& strVote)
{
    if (strVote != "yes" && strVote != "no") throw JSONRPCError(RPC_MISC_ERROR, "You can only vote 'yes' or 'no'");
    CBudgetVote::VoteDirection nVote = CBudgetVote::VOTE_ABSTAIN;
    if (strVote == "yes") nVote = CBudgetVote::VOTE_YES;
    if (strVote == "no") nVote = CBudgetVote::VOTE_NO;
    return nVote;
}

UniValue mnbudgetvote(const JSONRPCRequest& request)
{
    std::string strCommand;
    if (request.params.size() >= 1) {
        strCommand = request.params[0].get_str();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        // Backwards compatibility with legacy `mnbudget` command
        if (strCommand == "vote") strCommand = "local";
        if (strCommand == "vote-many") strCommand = "many";
        if (strCommand == "vote-alias") strCommand = "alias";
    }

<<<<<<< HEAD
    if (fHelp || (params.size() == 3 && (strCommand != "local" && strCommand != "many")) || (params.size() == 4 && strCommand != "alias") ||
        params.size() > 4 || params.size() < 3)
        throw runtime_error(
            "mnbudgetvote \"local|many|alias\" \"votehash\" \"yes|no\" ( \"alias\" )\n"
            "\nVote on a budget proposal\n"

            "\nArguments:\n"
            "1. \"mode\"      (string, required) The voting mode. 'local' for voting directly from a masternode, 'many' for voting with a MN controller and casting the same vote for each MN, 'alias' for voting with a MN controller and casting a vote for a single MN\n"
            "2. \"votehash\"  (string, required) The vote hash for the proposal\n"
            "3. \"votecast\"  (string, required) Your vote. 'yes' to vote for the proposal, 'no' to vote against\n"
            "4. \"alias\"     (string, required for 'alias' mode) The MN alias to cast a vote for.\n"
=======
    CWallet * const pwallet = GetWalletForJSONRPCRequest(request);

    if (request.fHelp || (request.params.size() == 3 && (strCommand != "local" && strCommand != "many")) || (request.params.size() == 4 && strCommand != "alias") ||
        request.params.size() > 5 || request.params.size() < 3)
        throw std::runtime_error(
            "mnbudgetvote \"local|many|alias\" \"hash\" \"yes|no\" ( \"alias\" legacy )\n"
            "\nVote on a budget proposal\n"
            "\nAfter V6 enforcement, the deterministic masternode system is used by default. Set the \"legacy\" parameter to true to vote with legacy masternodes."

            "\nArguments:\n"
            "1. \"mode\"      (string, required) The voting mode. 'local' for voting directly from a masternode, 'many' for voting with a MN controller and casting the same vote for each MN, 'alias' for voting with a MN controller and casting a vote for a single MN\n"
            "2. \"hash\"      (string, required) The budget proposal hash\n"
            "3. \"votecast\"  (string, required) Your vote. 'yes' to vote for the proposal, 'no' to vote against\n"
            "4. \"alias\"     (string, required for 'alias' mode) The MN alias to cast a vote for (for deterministic masternodes it's the hash of the proTx transaction).\n"
            "5. \"legacy\"    (boolean, optional, default=false) Use the legacy masternode system after deterministic masternodes enforcement.\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            "\nResult:\n"
            "{\n"
            "  \"overall\": \"xxxx\",      (string) The overall status message for the vote cast\n"
            "  \"detail\": [\n"
            "    {\n"
            "      \"node\": \"xxxx\",      (string) 'local' or the MN alias\n"
            "      \"result\": \"xxxx\",    (string) Either 'Success' or 'Failed'\n"
            "      \"error\": \"xxxx\",     (string) Error message, if vote failed\n"
            "    }\n"
            "    ,...\n"
            "  ]\n"
            "}\n"

            "\nExamples:\n" +
<<<<<<< HEAD
            HelpExampleCli("mnbudgetvote", "\"local\" \"ed2f83cedee59a91406f5f47ec4d60bf5a7f9ee6293913c82976bd2d3a658041\" \"yes\"") +
            HelpExampleRpc("mnbudgetvote", "\"local\" \"ed2f83cedee59a91406f5f47ec4d60bf5a7f9ee6293913c82976bd2d3a658041\" \"yes\""));

    uint256 hash = ParseHashV(params[1], "parameter 1");
    std::string strVote = params[2].get_str();

    if (strVote != "yes" && strVote != "no") return "You can only vote 'yes' or 'no'";
    int nVote = VOTE_ABSTAIN;
    if (strVote == "yes") nVote = VOTE_YES;
    if (strVote == "no") nVote = VOTE_NO;

    int success = 0;
    int failed = 0;

    UniValue resultsObj(UniValue::VARR);

    if (strCommand == "local") {
        CPubKey pubKeyMasternode;
        CKey keyMasternode;
        std::string errorMessage;

        UniValue statusObj(UniValue::VOBJ);

        while (true) {
            if (!obfuScationSigner.SetKey(strMasterNodePrivKey, errorMessage, keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("node", "local"));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Masternode signing error, could not set key correctly: " + errorMessage));
                resultsObj.push_back(statusObj);
                break;
            }

            CMasternode* pmn = mnodeman.Find(activeMasternode.vin);
            if (pmn == NULL) {
                failed++;
                statusObj.push_back(Pair("node", "local"));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Failure to find masternode in list : " + activeMasternode.vin.ToString()));
                resultsObj.push_back(statusObj);
                break;
            }

            CBudgetVote vote(activeMasternode.vin, hash, nVote);
            if (!vote.Sign(keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("node", "local"));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Failure to sign."));
                resultsObj.push_back(statusObj);
                break;
            }

            std::string strError = "";
            if (budget.UpdateProposal(vote, NULL, strError)) {
                success++;
                budget.mapSeenMasternodeBudgetVotes.insert(make_pair(vote.GetHash(), vote));
                vote.Relay();
                statusObj.push_back(Pair("node", "local"));
                statusObj.push_back(Pair("result", "success"));
                statusObj.push_back(Pair("error", ""));
            } else {
                failed++;
                statusObj.push_back(Pair("node", "local"));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Error voting : " + strError));
            }
            resultsObj.push_back(statusObj);
            break;
        }

        UniValue returnObj(UniValue::VOBJ);
        returnObj.push_back(Pair("overall", strprintf("Voted successfully %d time(s) and failed %d time(s).", success, failed)));
        returnObj.push_back(Pair("detail", resultsObj));

        return returnObj;
    }

    if (strCommand == "many") {
        for (CMasternodeConfig::CMasternodeEntry mne : masternodeConfig.getEntries()) {
            std::string errorMessage;
            std::vector<unsigned char> vchMasterNodeSignature;
            std::string strMasterNodeSignMessage;

            CPubKey pubKeyCollateralAddress;
            CKey keyCollateralAddress;
            CPubKey pubKeyMasternode;
            CKey keyMasternode;

            UniValue statusObj(UniValue::VOBJ);

            if (!obfuScationSigner.SetKey(mne.getPrivKey(), errorMessage, keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Masternode signing error, could not set key correctly: " + errorMessage));
                resultsObj.push_back(statusObj);
                continue;
            }

            CMasternode* pmn = mnodeman.Find(pubKeyMasternode);
            if (pmn == NULL) {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Can't find masternode by pubkey"));
                resultsObj.push_back(statusObj);
                continue;
            }

            CBudgetVote vote(pmn->vin, hash, nVote);
            if (!vote.Sign(keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Failure to sign."));
                resultsObj.push_back(statusObj);
                continue;
            }

            std::string strError = "";
            if (budget.UpdateProposal(vote, NULL, strError)) {
                budget.mapSeenMasternodeBudgetVotes.insert(make_pair(vote.GetHash(), vote));
                vote.Relay();
                success++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "success"));
                statusObj.push_back(Pair("error", ""));
            } else {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", strError.c_str()));
            }

            resultsObj.push_back(statusObj);
        }

        UniValue returnObj(UniValue::VOBJ);
        returnObj.push_back(Pair("overall", strprintf("Voted successfully %d time(s) and failed %d time(s).", success, failed)));
        returnObj.push_back(Pair("detail", resultsObj));

        return returnObj;
    }

    if (strCommand == "alias") {
        std::string strAlias = params[3].get_str();
        std::vector<CMasternodeConfig::CMasternodeEntry> mnEntries;
        mnEntries = masternodeConfig.getEntries();

        for (CMasternodeConfig::CMasternodeEntry mne : masternodeConfig.getEntries()) {

            if( strAlias != mne.getAlias()) continue;

            std::string errorMessage;
            std::vector<unsigned char> vchMasterNodeSignature;
            std::string strMasterNodeSignMessage;

            CPubKey pubKeyCollateralAddress;
            CKey keyCollateralAddress;
            CPubKey pubKeyMasternode;
            CKey keyMasternode;

            UniValue statusObj(UniValue::VOBJ);

            if(!obfuScationSigner.SetKey(mne.getPrivKey(), errorMessage, keyMasternode, pubKeyMasternode)){
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Masternode signing error, could not set key correctly: " + errorMessage));
                resultsObj.push_back(statusObj);
                continue;
            }

            CMasternode* pmn = mnodeman.Find(pubKeyMasternode);
            if(pmn == NULL)
            {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Can't find masternode by pubkey"));
                resultsObj.push_back(statusObj);
                continue;
            }

            CBudgetVote vote(pmn->vin, hash, nVote);
            if(!vote.Sign(keyMasternode, pubKeyMasternode)){
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", "Failure to sign."));
                resultsObj.push_back(statusObj);
                continue;
            }

            std::string strError = "";
            if(budget.UpdateProposal(vote, NULL, strError)) {
                budget.mapSeenMasternodeBudgetVotes.insert(make_pair(vote.GetHash(), vote));
                vote.Relay();
                success++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "success"));
                statusObj.push_back(Pair("error", ""));
            } else {
                failed++;
                statusObj.push_back(Pair("node", mne.getAlias()));
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("error", strError.c_str()));
            }

            resultsObj.push_back(statusObj);
        }

        UniValue returnObj(UniValue::VOBJ);
        returnObj.push_back(Pair("overall", strprintf("Voted successfully %d time(s) and failed %d time(s).", success, failed)));
        returnObj.push_back(Pair("detail", resultsObj));

        return returnObj;
=======
            HelpExampleCli("mnbudgetvote", "\"alias\" \"ed2f83cedee59a91406f5f47ec4d60bf5a7f9ee6293913c82976bd2d3a658041\" \"yes\" \"4f9de28fca1f0574a217c5d3c59cc51125ec671de82a2f80b6ceb69673115041\"") +
            HelpExampleRpc("mnbudgetvote", "\"alias\" \"ed2f83cedee59a91406f5f47ec4d60bf5a7f9ee6293913c82976bd2d3a658041\" \"yes\" \"4f9de28fca1f0574a217c5d3c59cc51125ec671de82a2f80b6ceb69673115041\""));

    const uint256& hash = ParseHashV(request.params[1], "parameter 1");
    CBudgetVote::VoteDirection nVote = parseVote(request.params[2].get_str());

    bool fLegacyMN = !deterministicMNManager->IsDIP3Enforced() || (request.params.size() > 4 && request.params[4].get_bool());

    if (strCommand == "local") {
        if (!fLegacyMN) {
            throw JSONRPCError(RPC_MISC_ERROR, _("\"local\" vote is no longer available with DMNs. Use \"alias\" from the wallet with the voting key."));
        }
        return mnLocalBudgetVoteInner(true, hash, false, nVote);
    }

    // DMN require wallet with voting key
    if (!fLegacyMN) {
        if (!EnsureWalletIsAvailable(pwallet, false)) {
            return NullUniValue;
        }
        EnsureWalletIsUnlocked(pwallet);
    }

    bool isAlias = false;
    if (strCommand == "many" || (isAlias = strCommand == "alias")) {
        Optional<std::string> mnAlias = isAlias ? Optional<std::string>(request.params[3].get_str()) : nullopt;
        return mnBudgetVoteInner(pwallet, fLegacyMN, hash, false, nVote, mnAlias);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return NullUniValue;
}

<<<<<<< HEAD
UniValue getbudgetvotes(const UniValue& params, bool fHelp)
{
    if (params.size() != 1)
        throw runtime_error(
            "getbudgetvotes \"proposal-name\"\n"
            "\nPrint vote information for a budget proposal\n"

            "\nArguments:\n"
            "1. \"proposal-name\":      (string, required) Name of the proposal\n"
=======
UniValue getbudgetvotes(const JSONRPCRequest& request)
{
    if (request.params.size() != 1)
        throw std::runtime_error(
            "getbudgetvotes \"name\"\n"
            "\nPrint vote information for a budget proposal\n"

            "\nArguments:\n"
            "1. \"name\":      (string, required) Name of the proposal\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            "\nResult:\n"
            "[\n"
            "  {\n"
<<<<<<< HEAD
            "    \"mnId\": \"xxxx\",        (string) Hash of the masternode's collateral transaction\n"
=======
            "    \"mnId\": \"xxxx-x\",      (string) Masternode's outpoint collateral transaction (hash-n)\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "    \"nHash\": \"xxxx\",       (string) Hash of the vote\n"
            "    \"Vote\": \"YES|NO\",      (string) Vote cast ('YES' or 'NO')\n"
            "    \"nTime\": xxxx,         (numeric) Time in seconds since epoch the vote was cast\n"
            "    \"fValid\": true|false,  (boolean) 'true' if the vote is valid, 'false' otherwise\n"
            "  }\n"
            "  ,...\n"
            "]\n"
<<<<<<< HEAD
            "\nExamples:\n" +
            HelpExampleCli("getbudgetvotes", "\"test-proposal\"") + HelpExampleRpc("getbudgetvotes", "\"test-proposal\""));

    std::string strProposalName = SanitizeString(params[0].get_str());

    UniValue ret(UniValue::VARR);

    CBudgetProposal* pbudgetProposal = budget.FindProposal(strProposalName);

    if (pbudgetProposal == NULL) throw runtime_error("Unknown proposal name");

    std::map<uint256, CBudgetVote>::iterator it = pbudgetProposal->mapVotes.begin();
    while (it != pbudgetProposal->mapVotes.end()) {
        UniValue bObj(UniValue::VOBJ);
        bObj.push_back(Pair("mnId", (*it).second.vin.prevout.hash.ToString()));
        bObj.push_back(Pair("nHash", (*it).first.ToString().c_str()));
        bObj.push_back(Pair("Vote", (*it).second.GetVoteString()));
        bObj.push_back(Pair("nTime", (int64_t)(*it).second.nTime));
        bObj.push_back(Pair("fValid", (*it).second.fValid));

        ret.push_back(bObj);

        it++;
    }

    return ret;
}

UniValue getnextsuperblock(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
=======

            "\nExamples:\n" +
            HelpExampleCli("getbudgetvotes", "\"test-proposal\"") + HelpExampleRpc("getbudgetvotes", "\"test-proposal\""));

    std::string strProposalName = SanitizeString(request.params[0].get_str());
    const CBudgetProposal* pbudgetProposal = g_budgetman.FindProposalByName(strProposalName);
    if (pbudgetProposal == NULL) throw std::runtime_error("Unknown proposal name");
    return pbudgetProposal->GetVotesArray();
}

UniValue getnextsuperblock(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "getnextsuperblock\n"
            "\nPrint the next super block height\n"

            "\nResult:\n"
            "n      (numeric) Block height of the next super block\n"
<<<<<<< HEAD
            "\nExamples:\n" +
            HelpExampleCli("getnextsuperblock", "") + HelpExampleRpc("getnextsuperblock", ""));

    CBlockIndex* pindexPrev = chainActive.Tip();
    if (!pindexPrev) return "unknown";

    int nNext = pindexPrev->nHeight - pindexPrev->nHeight % GetBudgetPaymentCycleBlocks() + GetBudgetPaymentCycleBlocks();
    return nNext;
}

UniValue getbudgetprojection(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "getbudgetprojection\n"
            "\nShow the projection of which proposals will be paid the next cycle\n"
=======

            "\nExamples:\n" +
            HelpExampleCli("getnextsuperblock", "") + HelpExampleRpc("getnextsuperblock", ""));

    int nChainHeight = WITH_LOCK(cs_main, return chainActive.Height());
    if (nChainHeight < 0) return "unknown";

    const int nBlocksPerCycle = Params().GetConsensus().nBudgetCycleBlocks;
    int nNext = nChainHeight - nChainHeight % nBlocksPerCycle + nBlocksPerCycle;
    return nNext;
}

UniValue getbudgetprojection(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getbudgetprojection\n"
            "\nShow the projection of which proposals will be paid the next cycle\n"
            "Proposal fee tx time need to be +24hrs old from the current time. (Testnet is 5 mins)\n"
            "Net Votes needs to be above Masternode Count divided by 10\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"Name\": \"xxxx\",               (string) Proposal Name\n"
            "    \"URL\": \"xxxx\",                (string) Proposal URL\n"
            "    \"Hash\": \"xxxx\",               (string) Proposal vote hash\n"
            "    \"FeeHash\": \"xxxx\",            (string) Proposal fee hash\n"
            "    \"BlockStart\": n,              (numeric) Proposal starting block\n"
            "    \"BlockEnd\": n,                (numeric) Proposal ending block\n"
            "    \"TotalPaymentCount\": n,       (numeric) Number of payments\n"
            "    \"RemainingPaymentCount\": n,   (numeric) Number of remaining payments\n"
<<<<<<< HEAD
            "    \"PaymentAddress\": \"xxxx\",     (string) PRCY address of payment\n"
=======
            "    \"PaymentAddress\": \"xxxx\",     (string) PIVX address of payment\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "    \"Ratio\": x.xxx,               (numeric) Ratio of yeas vs nays\n"
            "    \"Yeas\": n,                    (numeric) Number of yea votes\n"
            "    \"Nays\": n,                    (numeric) Number of nay votes\n"
            "    \"Abstains\": n,                (numeric) Number of abstains\n"
<<<<<<< HEAD
            "    \"TotalPayment\": xxx.xxx,      (numeric) Total payment amount\n"
            "    \"MonthlyPayment\": xxx.xxx,    (numeric) Monthly payment amount\n"
            "    \"IsEstablished\": true|false,  (boolean) Established (true) or (false)\n"
            "    \"IsValid\": true|false,        (boolean) Valid (true) or Invalid (false)\n"
            "    \"IsValidReason\": \"xxxx\",      (string) Error message, if any\n"
            "    \"fValid\": true|false,         (boolean) Valid (true) or Invalid (false)\n"
            "    \"Alloted\": xxx.xxx,           (numeric) Amount alloted in current period\n"
            "    \"TotalBudgetAlloted\": xxx.xxx (numeric) Total alloted\n"
            "  }\n"
            "  ,...\n"
            "]\n"
=======
            "    \"TotalPayment\": xxx.xxx,      (numeric) Total payment amount in PIV\n"
            "    \"MonthlyPayment\": xxx.xxx,    (numeric) Monthly payment amount in PIV\n"
            "    \"IsEstablished\": true|false,  (boolean) Proposal is considered established, 24 hrs after being submitted to network. (Testnet is 5 mins)\n"
            "    \"IsValid\": true|false,        (boolean) Valid (true) or Invalid (false)\n"
            "    \"IsInvalidReason\": \"xxxx\",  (string) Error message, if any\n"
            "    \"Allotted\": xxx.xxx,           (numeric) Amount of PIV allotted in current period\n"
            "    \"TotalBudgetAllotted\": xxx.xxx (numeric) Total PIV allotted\n"
            "  }\n"
            "  ,...\n"
            "]\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getbudgetprojection", "") + HelpExampleRpc("getbudgetprojection", ""));

    UniValue ret(UniValue::VARR);
    UniValue resultObj(UniValue::VOBJ);
    CAmount nTotalAllotted = 0;

<<<<<<< HEAD
    std::vector<CBudgetProposal*> winningProps = budget.GetBudget();
    for (CBudgetProposal* pbudgetProposal : winningProps) {
        nTotalAllotted += pbudgetProposal->GetAllotted();

        CTxDestination address1;
        ExtractDestination(pbudgetProposal->GetPayee(), address1);
        CBitcoinAddress address2(address1);

        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(pbudgetProposal, bObj);
        bObj.push_back(Pair("Alloted", ValueFromAmount(pbudgetProposal->GetAllotted())));
        bObj.push_back(Pair("TotalBudgetAlloted", ValueFromAmount(nTotalAllotted)));

=======
    std::vector<CBudgetProposal> winningProps = g_budgetman.GetBudget();
    for (const CBudgetProposal& p : winningProps) {
        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(&p, bObj, g_budgetman.GetBestHeight());
        nTotalAllotted += p.GetAllotted();
        bObj.pushKV("TotalBudgetAllotted", ValueFromAmount(nTotalAllotted));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ret.push_back(bObj);
    }

    return ret;
}

<<<<<<< HEAD
UniValue getbudgetinfo(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() > 1)
        throw runtime_error(
            "getbudgetinfo ( \"proposal\" )\n"
            "\nShow current masternode budgets\n"

            "\nArguments:\n"
            "1. \"proposal\"    (string, optional) Proposal name\n"
=======
UniValue getbudgetinfo(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
        throw std::runtime_error(
            "getbudgetinfo ( \"name\" )\n"
            "\nShow current masternode budgets\n"

            "\nArguments:\n"
            "1. \"name\"    (string, optional) Proposal name\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"Name\": \"xxxx\",               (string) Proposal Name\n"
            "    \"URL\": \"xxxx\",                (string) Proposal URL\n"
            "    \"Hash\": \"xxxx\",               (string) Proposal vote hash\n"
            "    \"FeeHash\": \"xxxx\",            (string) Proposal fee hash\n"
            "    \"BlockStart\": n,              (numeric) Proposal starting block\n"
            "    \"BlockEnd\": n,                (numeric) Proposal ending block\n"
            "    \"TotalPaymentCount\": n,       (numeric) Number of payments\n"
            "    \"RemainingPaymentCount\": n,   (numeric) Number of remaining payments\n"
<<<<<<< HEAD
            "    \"PaymentAddress\": \"xxxx\",     (string) PRCY address of payment\n"
=======
            "    \"PaymentAddress\": \"xxxx\",     (string) PIVX address of payment\n"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "    \"Ratio\": x.xxx,               (numeric) Ratio of yeas vs nays\n"
            "    \"Yeas\": n,                    (numeric) Number of yea votes\n"
            "    \"Nays\": n,                    (numeric) Number of nay votes\n"
            "    \"Abstains\": n,                (numeric) Number of abstains\n"
<<<<<<< HEAD
            "    \"TotalPayment\": xxx.xxx,      (numeric) Total payment amount\n"
            "    \"MonthlyPayment\": xxx.xxx,    (numeric) Monthly payment amount\n"
            "    \"IsEstablished\": true|false,  (boolean) Established (true) or (false)\n"
            "    \"IsValid\": true|false,        (boolean) Valid (true) or Invalid (false)\n"
            "    \"IsValidReason\": \"xxxx\",      (string) Error message, if any\n"
            "    \"fValid\": true|false,         (boolean) Valid (true) or Invalid (false)\n"
            "  }\n"
            "  ,...\n"
            "]\n"
=======
            "    \"TotalPayment\": xxx.xxx,      (numeric) Total payment amount in PIV\n"
            "    \"MonthlyPayment\": xxx.xxx,    (numeric) Monthly payment amount in PIV\n"
            "    \"IsEstablished\": true|false,  (boolean) Proposal is considered established, 24 hrs after being submitted to network. (5 mins for Testnet)\n"
            "    \"IsValid\": true|false,        (boolean) Valid (true) or Invalid (false)\n"
            "    \"IsInvalidReason\": \"xxxx\",      (string) Error message, if any\n"
            "  }\n"
            "  ,...\n"
            "]\n"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            "\nExamples:\n" +
            HelpExampleCli("getbudgetprojection", "") + HelpExampleRpc("getbudgetprojection", ""));

    UniValue ret(UniValue::VARR);
<<<<<<< HEAD

    std::string strShow = "valid";
    if (params.size() == 1) {
        std::string strProposalName = SanitizeString(params[0].get_str());
        CBudgetProposal* pbudgetProposal = budget.FindProposal(strProposalName);
        if (pbudgetProposal == NULL) throw runtime_error("Unknown proposal name");
        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(pbudgetProposal, bObj);
=======
    int nCurrentHeight = g_budgetman.GetBestHeight();

    if (request.params.size() == 1) {
        std::string strProposalName = SanitizeString(request.params[0].get_str());
        const CBudgetProposal* pbudgetProposal = g_budgetman.FindProposalByName(strProposalName);
        if (pbudgetProposal == NULL) throw std::runtime_error("Unknown proposal name");
        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(pbudgetProposal, bObj, nCurrentHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ret.push_back(bObj);
        return ret;
    }

<<<<<<< HEAD
    std::vector<CBudgetProposal*> winningProps = budget.GetAllProposals();
    for (CBudgetProposal* pbudgetProposal : winningProps) {
        if (strShow == "valid" && !pbudgetProposal->fValid) continue;

        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(pbudgetProposal, bObj);

=======
    std::vector<CBudgetProposal*> winningProps = g_budgetman.GetAllProposalsOrdered();
    for (CBudgetProposal* pbudgetProposal : winningProps) {
        if (!pbudgetProposal->IsValid()) continue;

        UniValue bObj(UniValue::VOBJ);
        budgetToJSON(pbudgetProposal, bObj, nCurrentHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ret.push_back(bObj);
    }

    return ret;
}

<<<<<<< HEAD
UniValue mnbudgetrawvote(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 6)
        throw runtime_error(
            "mnbudgetrawvote \"masternode-tx-hash\" masternode-tx-index \"proposal-hash\" yes|no time \"vote-sig\"\n"
            "\nCompile and relay a proposal vote with provided external signature instead of signing vote internally\n"

            "\nArguments:\n"
            "1. \"masternode-tx-hash\"  (string, required) Transaction hash for the masternode\n"
            "2. masternode-tx-index   (numeric, required) Output index for the masternode\n"
            "3. \"proposal-hash\"       (string, required) Proposal vote hash\n"
            "4. yes|no                (boolean, required) Vote to cast\n"
            "5. time                  (numeric, required) Time since epoch in seconds\n"
            "6. \"vote-sig\"            (string, required) External signature\n"

            "\nResult:\n"
            "\"status\"     (string) Vote status or error message\n"
            "\nExamples:\n" +
            HelpExampleCli("mnbudgetrawvote", "") + HelpExampleRpc("mnbudgetrawvote", ""));

    uint256 hashMnTx = ParseHashV(params[0], "mn tx hash");
    int nMnTxIndex = params[1].get_int();
    CTxIn vin = CTxIn(hashMnTx, nMnTxIndex);

    uint256 hashProposal = ParseHashV(params[2], "Proposal hash");
    std::string strVote = params[3].get_str();

    if (strVote != "yes" && strVote != "no") return "You can only vote 'yes' or 'no'";
    int nVote = VOTE_ABSTAIN;
    if (strVote == "yes") nVote = VOTE_YES;
    if (strVote == "no") nVote = VOTE_NO;

    int64_t nTime = params[4].get_int64();
    std::string strSig = params[5].get_str();
    bool fInvalid = false;
    vector<unsigned char> vchSig = DecodeBase64(strSig.c_str(), &fInvalid);
=======
UniValue mnbudgetrawvote(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 6)
        throw std::runtime_error(
            "mnbudgetrawvote \"collat_txid\" collat_vout \"hash\" votecast time \"sig\"\n"
            "\nCompile and relay a proposal vote with provided external signature instead of signing vote internally\n"

            "\nArguments:\n"
            "1. \"collat_txid\"   (string, required) Transaction hash for the masternode collateral\n"
            "2. collat_vout       (numeric, required) Output index for the masternode collateral\n"
            "3. \"hash\"          (string, required) Budget Proposal hash\n"
            "4. \"votecast\"      (string, required) Your vote. 'yes' to vote for the proposal, 'no' to vote against\n"
            "5. time              (numeric, required) Time since epoch in seconds\n"
            "6. \"sig\"           (string, required) External signature\n"

            "\nResult:\n"
            "\"status\"     (string) Vote status or error message\n"

            "\nExamples:\n" +
            HelpExampleCli("mnbudgetrawvote", "") + HelpExampleRpc("mnbudgetrawvote", ""));

    const uint256& hashMnTx = ParseHashV(request.params[0], "mn tx hash");
    int nMnTxIndex = request.params[1].get_int();
    const CTxIn& vin = CTxIn(hashMnTx, nMnTxIndex);

    const uint256& hashProposal = ParseHashV(request.params[2], "Proposal hash");
    CBudgetVote::VoteDirection nVote = parseVote(request.params[3].get_str());

    int64_t nTime = request.params[4].get_int64();
    std::string strSig = request.params[5].get_str();
    bool fInvalid = false;
    std::vector<unsigned char> vchSig = DecodeBase64(strSig.c_str(), &fInvalid);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (fInvalid)
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Malformed base64 encoding");

<<<<<<< HEAD
    CMasternode* pmn = mnodeman.Find(vin);
    if (pmn == NULL) {
=======
    CMasternode* pmn = mnodeman.Find(vin.prevout);
    if (!pmn) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return "Failure to find masternode in list : " + vin.ToString();
    }

    CBudgetVote vote(vin, hashProposal, nVote);
<<<<<<< HEAD
    vote.nTime = nTime;
    vote.vchSig = vchSig;

    if (!vote.SignatureValid(true)) {
        return "Failure to verify signature.";
    }

    std::string strError = "";
    if (budget.UpdateProposal(vote, NULL, strError)) {
        budget.mapSeenMasternodeBudgetVotes.insert(make_pair(vote.GetHash(), vote));
        vote.Relay();
=======
    vote.SetTime(nTime);
    vote.SetVchSig(vchSig);

    if (!vote.CheckSignature(pmn->pubKeyMasternode.GetID())) {
        return "Failure to verify signature.";
    }

    std::string strError;
    if (g_budgetman.AddAndRelayProposalVote(vote, strError)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return "Voted successfully";
    } else {
        return "Error voting : " + strError;
    }
}

<<<<<<< HEAD
UniValue mnfinalbudget(const UniValue& params, bool fHelp)
{
    string strCommand;
    if (params.size() >= 1)
        strCommand = params[0].get_str();

    if (fHelp ||
        (strCommand != "suggest" && strCommand != "vote-many" && strCommand != "vote" && strCommand != "show" && strCommand != "getvotes"))
        throw runtime_error(
            "mnfinalbudget \"command\"... ( \"passphrase\" )\n"
            "Vote or show current budgets\n"
            "\nAvailable commands:\n"
            "  vote-many   - Vote on a finalized budget\n"
            "  vote        - Vote on a finalized budget\n"
            "  show        - Show existing finalized budgets\n"
            "  getvotes     - Get vote information for each finalized budget\n");

    if (strCommand == "vote-many") {
        if (params.size() != 2)
            throw runtime_error("Correct usage is 'mnfinalbudget vote-many BUDGET_HASH'");

        std::string strHash = params[1].get_str();
        uint256 hash(strHash);

        int success = 0;
        int failed = 0;

        UniValue resultsObj(UniValue::VOBJ);

        for (CMasternodeConfig::CMasternodeEntry mne : masternodeConfig.getEntries()) {
            std::string errorMessage;
            std::vector<unsigned char> vchMasterNodeSignature;
            std::string strMasterNodeSignMessage;

            CPubKey pubKeyCollateralAddress;
            CKey keyCollateralAddress;
            CPubKey pubKeyMasternode;
            CKey keyMasternode;

            UniValue statusObj(UniValue::VOBJ);

            if (!obfuScationSigner.SetKey(mne.getPrivKey(), errorMessage, keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("errorMessage", "Masternode signing error, could not set key correctly: " + errorMessage));
                resultsObj.push_back(Pair(mne.getAlias(), statusObj));
                continue;
            }

            CMasternode* pmn = mnodeman.Find(pubKeyMasternode);
            if (pmn == NULL) {
                failed++;
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("errorMessage", "Can't find masternode by pubkey"));
                resultsObj.push_back(Pair(mne.getAlias(), statusObj));
                continue;
            }


            CFinalizedBudgetVote vote(pmn->vin, hash);
            if (!vote.Sign(keyMasternode, pubKeyMasternode)) {
                failed++;
                statusObj.push_back(Pair("result", "failed"));
                statusObj.push_back(Pair("errorMessage", "Failure to sign."));
                resultsObj.push_back(Pair(mne.getAlias(), statusObj));
                continue;
            }

            std::string strError = "";
            if (budget.UpdateFinalizedBudget(vote, NULL, strError)) {
                budget.mapSeenFinalizedBudgetVotes.insert(make_pair(vote.GetHash(), vote));
                vote.Relay();
                success++;
                statusObj.push_back(Pair("result", "success"));
            } else {
                failed++;
                statusObj.push_back(Pair("result", strError.c_str()));
            }

            resultsObj.push_back(Pair(mne.getAlias(), statusObj));
        }

        UniValue returnObj(UniValue::VOBJ);
        returnObj.push_back(Pair("overall", strprintf("Voted successfully %d time(s) and failed %d time(s).", success, failed)));
        returnObj.push_back(Pair("detail", resultsObj));

        return returnObj;
    }

    if (strCommand == "vote") {
        if (params.size() != 2)
            throw runtime_error("Correct usage is 'mnfinalbudget vote BUDGET_HASH'");

        std::string strHash = params[1].get_str();
        uint256 hash(strHash);

        CPubKey pubKeyMasternode;
        CKey keyMasternode;
        std::string errorMessage;

        if (!obfuScationSigner.SetKey(strMasterNodePrivKey, errorMessage, keyMasternode, pubKeyMasternode))
            return "Error upon calling SetKey";

        CMasternode* pmn = mnodeman.Find(activeMasternode.vin);
        if (pmn == NULL) {
            return "Failure to find masternode in list : " + activeMasternode.vin.ToString();
        }

        CFinalizedBudgetVote vote(activeMasternode.vin, hash);
        if (!vote.Sign(keyMasternode, pubKeyMasternode)) {
            return "Failure to sign.";
        }

        std::string strError = "";
        if (budget.UpdateFinalizedBudget(vote, NULL, strError)) {
            budget.mapSeenFinalizedBudgetVotes.insert(make_pair(vote.GetHash(), vote));
            vote.Relay();
            return "success";
        } else {
            return "Error voting : " + strError;
        }
=======
UniValue mnfinalbudgetsuggest(const JSONRPCRequest& request)
{
    if (request.fHelp || !request.params.empty())
        throw std::runtime_error(
                "mnfinalbudgetsuggest\n"
                "\nTry to submit a budget finalization\n"
                "returns the budget hash if it was broadcasted sucessfully");

    if (!Params().IsRegTestNet()) {
        throw JSONRPCError(RPC_MISC_ERROR, "command available only for RegTest network");
    }

    const uint256& budgetHash = g_budgetman.SubmitFinalBudget();
    return (budgetHash.IsNull()) ? NullUniValue : budgetHash.ToString();
}

UniValue createrawmnfinalbudget(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 4)
        throw std::runtime_error(
                "createrawmnfinalbudget\n"
                "\nTry to submit the raw budget finalization\n"
                "returns the budget hash if it was broadcasted sucessfully"
                "\nArguments:\n"
                "1. \"budgetname\"    (string, required) finalization name\n"
                "2. \"blockstart\"    (numeric, required) superblock height\n"
                "3. \"proposals\"     (string, required) A json array of json objects\n"
                "     [\n"
                "       {\n"
                "         \"proposalid\":\"id\",  (string, required) The proposal id\n"
                "         \"payee\":n,         (hex, required) The payee script\n"
                "         \"amount\":n            (numeric, optional) The payee amount\n"
                "       }\n"
                "       ,...\n"
                "     ]\n"
                "4. \"feetxid\"    (string, optional) the transaction fee hash\n"
                ""
                "\nResult:\n"
                "{\n"
                    "\"result\"     (string) Budget suggest broadcast or error\n"
                    "\"id\"         (string) id of the fee tx or the finalized budget\n"
                "}\n"
                ); // future: add examples.

    if (!Params().IsRegTestNet()) {
        throw JSONRPCError(RPC_MISC_ERROR, "command available only for RegTest network");
    }

    // future: add inputs error checking..
    std::string budName = request.params[0].get_str();
    int nBlockStart = request.params[1].get_int();
    std::vector<CTxBudgetPayment> vecTxBudgetPayments;
    UniValue budgetVec = request.params[2].get_array();
    for (unsigned int idx = 0; idx < budgetVec.size(); idx++) {
        const UniValue& prop = budgetVec[idx].get_obj();
        uint256 propId = ParseHashO(prop, "proposalid");
        std::vector<unsigned char> scriptData(ParseHexO(prop, "payee"));
        CScript payee = CScript(scriptData.begin(), scriptData.end());
        CAmount amount = AmountFromValue(find_value(prop, "amount"));
        vecTxBudgetPayments.emplace_back(propId, payee, amount);
    }

    Optional<uint256> txFeeId = nullopt;
    if (request.params.size() > 3) {
        txFeeId = ParseHashV(request.params[3], "parameter 4");
    }

    if (!txFeeId) {
        CFinalizedBudget tempBudget(budName, nBlockStart, vecTxBudgetPayments, UINT256_ZERO);
        const uint256& budgetHash = tempBudget.GetHash();

        // create fee tx
        CTransactionRef wtx;
        CReserveKey keyChange(vpwallets[0]);
        if (!vpwallets[0]->CreateBudgetFeeTX(wtx, budgetHash, keyChange, BUDGET_FEE_TX)) {
            throw std::runtime_error("Can't make collateral transaction");
        }
        // Send the tx to the network
        const CWallet::CommitResult& res = vpwallets[0]->CommitTransaction(wtx, keyChange, g_connman.get());
        UniValue ret(UniValue::VOBJ);
        if (res.status == CWallet::CommitStatus::OK) {
            ret.pushKV("result", "tx_fee_sent");
            ret.pushKV("id", wtx->GetHash().ToString());
        } else {
            ret.pushKV("result", "error");
        }
        return ret;
    }

    UniValue ret(UniValue::VOBJ);
    // Collateral tx already exists, see if it's mature enough.
    CFinalizedBudget fb(budName, nBlockStart, vecTxBudgetPayments, *txFeeId);
    if (g_budgetman.AddFinalizedBudget(fb)) {
        fb.Relay();
        ret.pushKV("result", "fin_budget_sent");
        ret.pushKV("id", fb.GetHash().ToString());
    } else {
        // future: add proper error
        ret.pushKV("result", "error");
    }
    return ret;
}

UniValue mnfinalbudget(const JSONRPCRequest& request)
{
    std::string strCommand;
    if (request.params.size() >= 1)
        strCommand = request.params[0].get_str();

    CWallet * const pwallet = GetWalletForJSONRPCRequest(request);

    if (request.fHelp ||
        (strCommand != "vote-many" && strCommand != "vote" && strCommand != "show" && strCommand != "getvotes"))
        throw std::runtime_error(
            "mnfinalbudget \"command\"... ( \"passphrase\" )\n"
            "\nVote or show current budgets\n"

            "\nAvailable commands:\n"
            "  vote-many   - Vote on a finalized budget\n"
            "  vote        - Vote on a finalized budget with local masternode\n"
            "  show        - Show existing finalized budgets\n"
            "  getvotes     - Get vote information for each finalized budget\n");

    if (strCommand == "vote-many" || strCommand == "vote") {
        if (request.params.size() < 2 || request.params.size() > 3) {
            throw std::runtime_error(strprintf("Correct usage is 'mnfinalbudget %s BUDGET_HASH (fLegacy)'", strCommand));
        }
        const uint256& hash = ParseHashV(request.params[1], "BUDGET_HASH");
        bool fLegacyMN = !deterministicMNManager->IsDIP3Enforced() || (request.params.size() > 2 && request.params[2].get_bool());

        // DMN require wallet with operator keys for vote-many
        if (!fLegacyMN && strCommand == "vote-many" && !EnsureWalletIsAvailable(pwallet, false)) {
            return NullUniValue;
        }

        return (strCommand == "vote-many" ? mnBudgetVoteInner(pwallet, fLegacyMN, hash, true, CBudgetVote::VOTE_YES, nullopt)
                                          : mnLocalBudgetVoteInner(fLegacyMN, hash, true, CBudgetVote::VOTE_YES));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    if (strCommand == "show") {
        UniValue resultObj(UniValue::VOBJ);

<<<<<<< HEAD
        std::vector<CFinalizedBudget*> winningFbs = budget.GetFinalizedBudgets();
        for (CFinalizedBudget* finalizedBudget : winningFbs) {
            UniValue bObj(UniValue::VOBJ);
            bObj.push_back(Pair("FeeTX", finalizedBudget->nFeeTXHash.ToString()));
            bObj.push_back(Pair("Hash", finalizedBudget->GetHash().ToString()));
            bObj.push_back(Pair("BlockStart", (int64_t)finalizedBudget->GetBlockStart()));
            bObj.push_back(Pair("BlockEnd", (int64_t)finalizedBudget->GetBlockEnd()));
            bObj.push_back(Pair("Proposals", finalizedBudget->GetProposals()));
            bObj.push_back(Pair("VoteCount", (int64_t)finalizedBudget->GetVoteCount()));
            bObj.push_back(Pair("Status", finalizedBudget->GetStatus()));

            std::string strError = "";
            bObj.push_back(Pair("IsValid", finalizedBudget->IsValid(strError)));
            bObj.push_back(Pair("IsValidReason", strError.c_str()));

            resultObj.push_back(Pair(finalizedBudget->GetName(), bObj));
=======
        std::vector<CFinalizedBudget*> winningFbs = g_budgetman.GetFinalizedBudgets();
        for (CFinalizedBudget* finalizedBudget : winningFbs) {
            const uint256& nHash = finalizedBudget->GetHash();
            UniValue bObj(UniValue::VOBJ);
            bObj.pushKV("FeeTX", finalizedBudget->GetFeeTXHash().ToString());
            bObj.pushKV("BlockStart", (int64_t)finalizedBudget->GetBlockStart());
            bObj.pushKV("BlockEnd", (int64_t)finalizedBudget->GetBlockEnd());
            bObj.pushKV("Proposals", finalizedBudget->GetProposalsStr());
            bObj.pushKV("VoteCount", (int64_t)finalizedBudget->GetVoteCount());
            bObj.pushKV("Status", g_budgetman.GetFinalizedBudgetStatus(nHash));

            bool fValid = finalizedBudget->IsValid();
            bObj.pushKV("IsValid", fValid);
            if (!fValid)
                bObj.pushKV("IsInvalidReason", finalizedBudget->IsInvalidReason());

            std::string strName = strprintf("%s (%s)", finalizedBudget->GetName(), nHash.ToString());
            resultObj.pushKV(strName, bObj);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        return resultObj;
    }

    if (strCommand == "getvotes") {
<<<<<<< HEAD
        if (params.size() != 2)
            throw runtime_error("Correct usage is 'mnbudget getvotes budget-hash'");

        std::string strHash = params[1].get_str();
        uint256 hash(strHash);

        UniValue obj(UniValue::VOBJ);

        CFinalizedBudget* pfinalBudget = budget.FindFinalizedBudget(hash);

        if (pfinalBudget == NULL) return "Unknown budget hash";

        std::map<uint256, CFinalizedBudgetVote>::iterator it = pfinalBudget->mapVotes.begin();
        while (it != pfinalBudget->mapVotes.end()) {
            UniValue bObj(UniValue::VOBJ);
            bObj.push_back(Pair("nHash", (*it).first.ToString().c_str()));
            bObj.push_back(Pair("nTime", (int64_t)(*it).second.nTime));
            bObj.push_back(Pair("fValid", (*it).second.fValid));

            obj.push_back(Pair((*it).second.vin.prevout.ToStringShort(), bObj));

            it++;
        }

        return obj;
=======
        if (request.params.size() != 2)
            throw std::runtime_error("Correct usage is 'mnbudget getvotes budget-hash'");

        LOCK(g_budgetman.cs_budgets);
        std::string strHash = request.params[1].get_str();
        uint256 hash(uint256S(strHash));
        CFinalizedBudget* pfinalBudget = g_budgetman.FindFinalizedBudget(hash);
        if (pfinalBudget == NULL) return "Unknown budget hash";
        return pfinalBudget->GetVotesObject();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return NullUniValue;
}

<<<<<<< HEAD
UniValue checkbudgets(const UniValue& params, bool fHelp)
{
    if (fHelp || params.size() != 0)
        throw runtime_error(
            "checkbudgets\n"
            "\nInitiates a budget check cycle manually\n"
            "\nExamples:\n" +
            HelpExampleCli("checkbudgets", "") + HelpExampleRpc("checkbudgets", ""));

    budget.CheckAndRemove();

    return NullUniValue;
}
=======
UniValue checkbudgets(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "checkbudgets\n"
            "\nInitiates a budget check cycle manually\n"

            "\nExamples:\n" +
            HelpExampleCli("checkbudgets", "") + HelpExampleRpc("checkbudgets", ""));

    if (!masternodeSync.IsSynced())
        throw JSONRPCError(RPC_CLIENT_IN_INITIAL_DOWNLOAD, "Masternode/Budget sync not finished yet");

    g_budgetman.CheckAndRemove();
    return NullUniValue;
}

UniValue cleanbudget(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 1)
        throw std::runtime_error(
                "cleanbudget ( try_sync )\n"
                "\nCleans the budget data manually\n"
                "\nArguments:\n"
                "1. try_sync          (boolean, optional, default=false) resets tier two sync to a pre-budget data request\n"
                "\nExamples:\n" +
                HelpExampleCli("cleanbudget", "") + HelpExampleRpc("cleanbudget", ""));

    g_budgetman.Clear();
    LogPrintf("Budget data cleaned\n");

    // reset sync if requested
    bool reset = request.params.size() > 0 ? request.params[0].get_bool() : false;
    if (reset) {
        masternodeSync.ClearFulfilledRequest();
        masternodeSync.Reset();
        LogPrintf("Masternode sync restarted\n");
    }
    return NullUniValue;
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         okSafe argNames
  //  --------------------- ------------------------  -----------------------  ------ --------
    { "budget",             "checkbudgets",           &checkbudgets,           true,  {} },
    { "budget",             "getbudgetinfo",          &getbudgetinfo,          true,  {"name"} },
    { "budget",             "getbudgetprojection",    &getbudgetprojection,    true,  {} },
    { "budget",             "getbudgetvotes",         &getbudgetvotes,         true,  {"name"} },
    { "budget",             "getnextsuperblock",      &getnextsuperblock,      true,  {} },
    { "budget",             "mnbudgetrawvote",        &mnbudgetrawvote,        true,  {"collat_txid","collat_vout","hash","votecast","time","sig"} },
    { "budget",             "mnbudgetvote",           &mnbudgetvote,           true,  {"mode","hash","votecast","alias","legacy"} },
    { "budget",             "mnfinalbudget",          &mnfinalbudget,          true,  {"command"} },
    { "budget",             "preparebudget",          &preparebudget,          true,  {"name","url","npayments","start","address","monthly_payment"} },
    { "budget",             "submitbudget",           &submitbudget,           true,  {"name","url","npayments","start","address","monthly_payment","fee_txid"}  },

    /* Not shown in help */
    { "hidden",             "mnfinalbudgetsuggest",   &mnfinalbudgetsuggest,   true,  {} },
    { "hidden",             "createrawmnfinalbudget", &createrawmnfinalbudget, true,  {"budgetname", "blockstart", "proposals", "feetxid"} },
    { "hidden",             "cleanbudget",            &cleanbudget,            true,  {"try_sync"} },

};

void RegisterBudgetRPCCommands(CRPCTable &tableRPC)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        tableRPC.appendCommand(commands[vcidx].name, &commands[vcidx]);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
