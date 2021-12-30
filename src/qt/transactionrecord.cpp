// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "transactionrecord.h"

<<<<<<< HEAD
#include "base58.h"
#include "obfuscation.h"
#include "swifttx.h"
#include "timedata.h"
#include "wallet/wallet.h"

#include <stdint.h>

/*
 * Decompose CWallet transaction to model transaction records.
 */
QList<TransactionRecord> TransactionRecord::decomposeTransaction(const CWallet* wallet, const CWalletTx& wtx)
{
    QList<TransactionRecord> parts;
    int64_t nTime = wtx.GetComputedTxTime();
    CAmount nCredit = wtx.GetCredit(ISMINE_ALL);
    CAmount nDebit = wtx.GetDebit(ISMINE_ALL);
    CAmount nNet = nCredit - nDebit;
    uint256 hash = wtx.GetHash();
    std::map<std::string, std::string> mapValue = wtx.mapValue;
    if (wtx.IsCoinStake()) {
        TransactionRecord sub(hash, nTime);
        CTxDestination address;
        if (!ExtractDestination(wtx.vout[1].scriptPubKey, address))
            return parts;
        if (!IsMine(*wallet, address)) {
            //if the address is not yours then it means you have a tx sent to you in someone elses coinstake tx
            for (unsigned int i = 1; i < wtx.vout.size(); i++) {
                CTxDestination outAddress;
                if (ExtractDestination(wtx.vout[i].scriptPubKey, outAddress)) {
                    if (IsMine(*wallet, outAddress)) {
                        isminetype mine = wallet->IsMine(wtx.vout[i]);
                        sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
                        sub.type = TransactionRecord::MNReward;
                        sub.address = CBitcoinAddress(outAddress).ToString();
                        sub.credit = wallet->getCTxOutValue(wtx, wtx.vout[i]);
                    }
                }
            }
        } else {
            //stake reward
            isminetype mine = wallet->IsMine(wtx.vout[1]);
            sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
            sub.type = TransactionRecord::StakeMint;
            sub.address = CBitcoinAddress(address).ToString();
            sub.credit = nNet;
            sub.debit = 0;
        }
        parts.append(sub);
    } else if (nNet > 0 || wtx.IsCoinBase() || wtx.IsCoinAudit()) {
        //
        // Credit
        //
        for (const CTxOut& txout : wtx.vout) {
            isminetype mine = wallet->IsMine(txout);
            if (mine) {
                TransactionRecord sub(hash, nTime);
                CTxDestination address;
                sub.idx = parts.size(); // sequence number
                sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
                if (ExtractDestination(txout.scriptPubKey, address) && IsMine(*wallet, address)) {
                    // Received by PRCY Address
                    sub.credit = IsMine(*wallet, address) ? wallet->getCTxOutValue(wtx, txout) : 0;
                    sub.type = TransactionRecord::RecvWithAddress;
                    sub.address = CBitcoinAddress(address).ToString();
                } else {
                    // Received by IP connection (deprecated features), or a multisignature or other non-simple transaction
                    sub.type = TransactionRecord::RecvFromOther;
                    sub.address = mapValue["from"];
                }
                if (wtx.IsCoinBase() || wtx.IsCoinAudit()) {
                    // Generated
                    sub.type = TransactionRecord::Generated;
                }

                parts.append(sub);
            }
        }
    } else {
        bool fAllFromMeDenom = true;
        int nFromMe = 0;
        bool involvesWatchAddress = false;
        isminetype fAllFromMe = ISMINE_SPENDABLE;
        for (const CTxIn& txin : wtx.vin) {
            if (wallet->IsMine(txin)) {
                fAllFromMeDenom = fAllFromMeDenom && wallet->IsDenominated(txin);
                nFromMe++;
            }
            isminetype mine = wallet->IsMine(txin);
            if (mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
            if (fAllFromMe > mine) fAllFromMe = mine;
        }

        isminetype fAllToMe = ISMINE_SPENDABLE;
        bool fAllToMeDenom = true;
        int nToMe = 0;
        for (const CTxOut& txout : wtx.vout) {
            if (wallet->IsMine(txout)) {
                CAmount c = wallet->getCTxOutValue(wtx, txout);
                fAllToMeDenom = fAllToMeDenom && wallet->IsDenominatedAmount(c);
                nToMe++;
            }
            isminetype mine = wallet->IsMine(txout);
            if (mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
            if (fAllToMe > mine) fAllToMe = mine;
        }

        if (fAllFromMeDenom && fAllToMeDenom && nFromMe && nToMe) {
            parts.append(TransactionRecord(hash, nTime, TransactionRecord::ObfuscationDenominate, "", -nDebit, nCredit));
            parts.last().involvesWatchAddress = false; // maybe pass to TransactionRecord as constructor argument
        } else if (fAllFromMe && fAllToMe) {
            // Payment to self
            TransactionRecord sub(hash, nTime);
            // Payment to self by default
            sub.type = TransactionRecord::SendToSelf;
            sub.address = "";

            for (unsigned int nOut = 0; nOut < wtx.vout.size(); nOut++) {
                    const CTxOut& txout = wtx.vout[nOut];
                    sub.idx = parts.size();

                    if (wallet->IsCollateralAmount(wallet->getCTxOutValue(wtx, txout))) sub.type = TransactionRecord::ObfuscationMakeCollaterals;
                    if (nDebit - wtx.GetValueOut() == OBFUSCATION_COLLATERAL) sub.type = TransactionRecord::ObfuscationCollateralPayment;
                }
                CTxDestination address;
                if (ExtractDestination(wtx.vout[0].scriptPubKey, address)) {
                    // Sent to PRCY Address
                    sub.address = CBitcoinAddress(address).ToString();
            }

            //a sendtoself transaction has second output as change
            CAmount nChange = 0;
            if (wtx.vout.size() >= 2) nChange = pwalletMain->getCTxOutValue(wtx, wtx.vout[1]);

            sub.debit = nCredit - nChange;
            sub.credit = 0;
            parts.append(sub);
            parts.last().involvesWatchAddress = involvesWatchAddress; // maybe pass to TransactionRecord as constructor argument
        } else if (fAllFromMe) {
            //
            // Debit
            //
            CAmount nTxFee = 0;//nDebit - wtx.GetValueOut();

            for (unsigned int nOut = 0; nOut < wtx.vout.size(); nOut++) {
                const CTxOut& txout = wtx.vout[nOut];
                TransactionRecord sub(hash, nTime);
                sub.idx = parts.size();
                sub.involvesWatchAddress = involvesWatchAddress;

                if (wallet->IsMine(txout)) {
                    // Ignore parts sent to self, as this is usually the change
                    // from a transaction sent back to our own address.
                    continue;
                }

                CTxDestination address;
                if (ExtractDestination(txout.scriptPubKey, address)) {
                    // Sent to PRCY Address
                    sub.type = TransactionRecord::SendToAddress;
                    sub.address = CBitcoinAddress(address).ToString();
                } else {
                    // Sent to IP, or other non-address transaction like OP_EVAL
                    sub.type = TransactionRecord::SendToOther;
                    sub.address = mapValue["to"];
                }

                CAmount nValue = wallet->getCTxOutValue(wtx, txout);
                /* Add fee to first output */
                if (nTxFee > 0) {
                    nValue += nTxFee;
                    nTxFee = 0;
                }
                sub.debit = nDebit - nCredit - wtx.nTxFee;

                parts.append(sub);
            }
        } else {
            //
            // Mixed debit transaction, can't break down payees
            //
            parts.append(TransactionRecord(hash, nTime, TransactionRecord::Other, "", nNet, 0));
            parts.last().involvesWatchAddress = involvesWatchAddress;
        }
    }

    return parts;
}

void TransactionRecord::updateStatus(const CWalletTx& wtx)
{
    AssertLockHeld(cs_main);
    // Determine transaction status

    // Find the block the tx is in
    CBlockIndex* pindex = NULL;
    BlockMap::iterator mi = mapBlockIndex.find(wtx.hashBlock);
    if (mi != mapBlockIndex.end())
        pindex = (*mi).second;

    // Sort order, unrecorded transactions sort to the top
    status.sortKey = strprintf("%010d-%01d-%010u-%03d",
        (pindex ? pindex->nHeight : std::numeric_limits<int>::max()),
        (wtx.IsCoinBase() ? 1 : 0),
        wtx.nTimeReceived,
        idx);
    status.countsForBalance = wtx.IsTrusted() && !(wtx.GetBlocksToMaturity() > 0);
    status.depth = wtx.GetDepthInMainChain();
    status.cur_num_blocks = chainActive.Height();
    status.cur_num_ix_locks = nCompleteTXLocks;

    if (!IsFinalTx(wtx, chainActive.Height() + 1)) {
        if (wtx.nLockTime < LOCKTIME_THRESHOLD) {
            status.status = TransactionStatus::OpenUntilBlock;
            status.open_for = wtx.nLockTime - chainActive.Height();
        } else {
            status.status = TransactionStatus::OpenUntilDate;
            status.open_for = wtx.nLockTime;
        }
    }
    // For generated transactions, determine maturity
    else if (type == TransactionRecord::Generated || type == TransactionRecord::StakeMint || type == TransactionRecord::MNReward) {
        if (wtx.GetBlocksToMaturity() > 0) {
            status.status = TransactionStatus::Immature;

            if (wtx.IsInMainChain()) {
                status.matures_in = wtx.GetBlocksToMaturity();

                // Check if the block was requested by anyone
                if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0)
                    status.status = TransactionStatus::MaturesWarning;
            } else {
=======
#include "key_io.h"
#include "budget/budgetproposal.h"
#include "sapling/key_io_sapling.h"
#include "wallet/wallet.h"

#include <algorithm>
#include <stdint.h>

std::string TransactionRecord::getValueOrReturnEmpty(const std::map<std::string, std::string>& mapValue, const std::string& key)
{
    const auto& it = mapValue.find(key);
    return it != mapValue.end() ? it->second : "";
}

bool TransactionRecord::decomposeCoinStake(const CWallet* wallet, const CWalletTx& wtx,
        const CAmount& nCredit, const CAmount& nDebit,
        QList<TransactionRecord>& parts)
{
    // Return if it's not a coinstake
    if (!wtx.IsCoinStake()) {
        return false;
    }

    const uint256& hash = wtx.GetHash();
    TransactionRecord sub(hash, wtx.GetTxTime(), wtx.tx->GetTotalSize());

    if (isminetype mine = wallet->IsMine(wtx.tx->vout[1])) {
        // Check for cold stakes.
        if (wtx.tx->HasP2CSOutputs()) {
            sub.credit = nCredit;
            sub.debit = -nDebit;
            loadHotOrColdStakeOrContract(wallet, wtx, sub);
        } else {
            // PIV stake reward
            CTxDestination address;
            if (!ExtractDestination(wtx.tx->vout[1].scriptPubKey, address))
                return true;

            sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
            sub.type = TransactionRecord::StakeMint;
            sub.address = EncodeDestination(address);
            sub.credit = nCredit - nDebit;
        }
    } else {
        //Masternode reward
        CTxDestination destMN;
        int nIndexMN = (int) wtx.tx->vout.size() - 1;
        if (ExtractDestination(wtx.tx->vout[nIndexMN].scriptPubKey, destMN) && (mine = IsMine(*wallet, destMN)) ) {
            sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
            sub.address = EncodeDestination(destMN);
            sub.credit = wtx.tx->vout[nIndexMN].nValue;
            // Simple way to differentiate budget payments from MN rewards.
            CAmount mn_reward = Params().GetConsensus().nMNBlockReward;
            sub.type = sub.credit > mn_reward ? TransactionRecord::BudgetPayment : TransactionRecord::MNReward;
        }
    }

    parts.append(sub);
    return true;
}

bool TransactionRecord::decomposeZcSpendTx(const CWallet* wallet, const CWalletTx& wtx,
                                           const CAmount& nCredit, const CAmount& nDebit,
                                           QList<TransactionRecord>& parts)
{

    // Return if it's not a zc spend
    if (!wtx.tx->HasZerocoinSpendInputs()) {
        return false;
    }

    // Basic values
    const uint256& hash = wtx.GetHash();
    int64_t nTime = wtx.GetTxTime();

    //zerocoin spend outputs
    for (unsigned int nOut = 0; nOut < wtx.tx->vout.size(); nOut++) {
        const CTxOut& txout = wtx.tx->vout[nOut];
        // change that was reminted as zerocoins
        if (txout.IsZerocoinMint()) {
            continue;
        }

        std::string strAddress;
        CTxDestination address;
        if (ExtractDestination(txout.scriptPubKey, address))
            strAddress = EncodeDestination(address);

        // a zerocoinspend that was sent to an address held by this wallet
        isminetype mine = wallet->IsMine(txout);
        if (mine) {
            TransactionRecord sub(hash, nTime, wtx.tx->GetTotalSize());
            sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
            sub.type = TransactionRecord::RecvFromZerocoinSpend;
            sub.credit = txout.nValue;
            sub.address = (!strAddress.empty()) ? strAddress : getValueOrReturnEmpty(wtx.mapValue, "recvzerocoinspend");
            sub.idx = (int) nOut;
            parts.append(sub);
            continue;
        }
    }
    return true;
}

bool TransactionRecord::decomposeP2CS(const CWallet* wallet, const CWalletTx& wtx,
                                           const CAmount& nCredit, const CAmount& nDebit,
                                           QList<TransactionRecord>& parts)
{
    if (wtx.tx->HasP2CSOutputs()) {
        // Delegate tx.
        TransactionRecord sub(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize());
        sub.credit = nCredit;
        sub.debit = -nDebit;
        loadHotOrColdStakeOrContract(wallet, wtx, sub, true);
        parts.append(sub);
        return true;
    } else if (wtx.HasP2CSInputs()) {
        // Delegation unlocked
        TransactionRecord sub(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize());
        loadUnlockColdStake(wallet, wtx, sub);
        parts.append(sub);
        return true;
    }
    return false;
}

/**
 * Decompose a credit transaction into a record for each received output.
 */
bool TransactionRecord::decomposeCreditTransaction(const CWallet* wallet, const CWalletTx& wtx, QList<TransactionRecord>& parts)
{
    TransactionRecord sub(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize());
    for (unsigned int nOut = 0; nOut < wtx.tx->vout.size(); nOut++) {
        const CTxOut& txout = wtx.tx->vout[nOut];
        isminetype mine = wallet->IsMine(txout);
        if (mine) {
            CTxDestination address;
            sub.idx = (int) nOut; // vout index
            sub.credit = txout.nValue;
            sub.involvesWatchAddress = mine & ISMINE_WATCH_ONLY;
            if (ExtractDestination(txout.scriptPubKey, address) && IsMine(*wallet, address)) {
                // Received by PIVX Address
                sub.type = TransactionRecord::RecvWithAddress;
                sub.address = EncodeDestination(address);
            } else {
                // Received by IP connection (deprecated features), or a multisignature or other non-simple transaction
                sub.type = TransactionRecord::RecvFromOther;
                sub.address = getValueOrReturnEmpty(wtx.mapValue, "from");
            }
            if (wtx.IsCoinBase()) {
                // Generated
                sub.type = TransactionRecord::Generated;
            }

            parts.append(sub);
        }
    }

    if (wtx.tx->hasSaplingData()) {
        auto sspkm = wallet->GetSaplingScriptPubKeyMan();
        for (int i = 0; i < (int) wtx.tx->sapData->vShieldedOutput.size(); ++i) {
            SaplingOutPoint out(sub.hash, i);
            auto opAddr = sspkm->GetOutPointAddress(wtx, out);
            if (opAddr) {
                // skip it if change
                if (sspkm->IsNoteSaplingChange(out, *opAddr)) {
                    continue;
                }

                sub.address = (opAddr) ? KeyIO::EncodePaymentAddress(*opAddr) : "";
                sub.type = TransactionRecord::RecvWithShieldedAddress;
                sub.credit = sspkm->GetOutPointValue(wtx, out);
                sub.memo = sspkm->GetOutPointMemo(wtx, out);
                sub.idx = i;
                parts.append(sub);
            }
        }
    }

    return true;
}

bool TransactionRecord::decomposeSendToSelfTransaction(const CWalletTx& wtx, const CAmount& nCredit,
                                                       const CAmount& nDebit, bool involvesWatchAddress,
                                                       QList<TransactionRecord>& parts, const CWallet* wallet)
{
    // Payment to self tx is presented as a single record.
    TransactionRecord sub(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize());
    sub.address = "";
    CAmount nChange = wtx.GetChange();
    if (!wtx.tx->hasSaplingData()) {
        sub.type = TransactionRecord::SendToSelf;
        // Label for payment to self
        CTxDestination address;
        if (ExtractDestination(wtx.tx->vout[0].scriptPubKey, address)) {
            sub.address = EncodeDestination(address);
        }
    } else {
        // we know that all of the inputs and outputs are mine and that have shielded data.
        // Let's see if only have transparent inputs, so we know that this is a
        // transparent -> shield transaction
        if (wtx.tx->sapData->vShieldedSpend.empty()) {
            sub.type = TransactionRecord::SendToSelfShieldedAddress;
            sub.shieldedCredit = wtx.GetCredit(ISMINE_SPENDABLE_SHIELDED);
            nChange += wtx.GetShieldedChange();

            const auto& sspkm = wallet->GetSaplingScriptPubKeyMan();
            SaplingOutPoint out(sub.hash, 0);
            auto opAddr = sspkm->GetOutPointAddress(wtx, out);
            if (opAddr) {
                sub.address = KeyIO::EncodePaymentAddress(*opAddr);
            }
            sub.memo = sspkm->GetOutPointMemo(wtx, out);
        } else {
            // we know that the inputs are shielded now, let's see if
            // if we have transparent outputs. if we have then we are converting back coins,
            // from shield to transparent
            if (!wtx.tx->vout.empty()) {
                sub.type = TransactionRecord::SendToSelfShieldToTransparent;
                // Label for payment to self
                CTxDestination address;
                if (ExtractDestination(wtx.tx->vout[0].scriptPubKey, address)) {
                    sub.address = EncodeDestination(address);
                }
                // little hack to show the correct amount
                sub.shieldedCredit = wtx.GetCredit(ISMINE_SPENDABLE_TRANSPARENT);
            } else {
                // we know that the outputs are only shield, this is purely a change address tx.
                // show only the fee.
                sub.type = TransactionRecord::SendToSelfShieldToShieldChangeAddress;
            }
        }
    }

    sub.debit = -(nDebit - nChange);
    sub.credit = nCredit - nChange;
    sub.involvesWatchAddress = involvesWatchAddress;
    parts.append(sub);
    return true;
}

bool TransactionRecord::decomposeShieldedDebitTransaction(const CWallet* wallet, const CWalletTx& wtx, CAmount nTxFee,
                                                          bool involvesWatchAddress, QList<TransactionRecord>& parts)
{
    // Return early if there are no outputs.
    if (wtx.tx->sapData->vShieldedOutput.empty()) {
        return false;
    }

    TransactionRecord sub(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize());
    auto sspkm = wallet->GetSaplingScriptPubKeyMan();
    for (int i = 0; i < (int) wtx.tx->sapData->vShieldedOutput.size(); ++i) {
        SaplingOutPoint out(sub.hash, i);
        auto opAddr = sspkm->GetOutPointAddress(wtx, out);
        // skip change
        if (!opAddr || sspkm->IsNoteSaplingChange(out, *opAddr)) {
            continue;
        }
        sub.idx = i;
        sub.involvesWatchAddress = involvesWatchAddress;
        sub.type = TransactionRecord::SendToShielded;
        sub.address = KeyIO::EncodePaymentAddress(*opAddr);
        sub.memo = sspkm->GetOutPointMemo(wtx, out);
        CAmount nValue = sspkm->GetOutPointValue(wtx, out);
        /* Add fee to first output */
        if (nTxFee > 0) {
            nValue += nTxFee;
            nTxFee = 0;
        }
        sub.debit = -nValue;
        parts.append(sub);
    }
    return true;
}

/**
 * Decompose wtx outputs in records.
 */
bool TransactionRecord::decomposeDebitTransaction(const CWallet* wallet, const CWalletTx& wtx,
                                                  const CAmount& nDebit, bool involvesWatchAddress,
                                                  QList<TransactionRecord>& parts)
{
    // Return early if there are no outputs.
    if (wtx.tx->vout.empty() && wtx.tx->sapData->vShieldedOutput.empty()) {
        return false;
    }

    // GetValueOut is the sum of transparent outs and negative sapValueBalance (shielded outs minus shielded spends).
    // Therefore to get the sum of the whole outputs of the tx, must re-add the shielded inputs spent to it
    CAmount nTxFee = nDebit - (wtx.tx->GetValueOut() + wtx.GetDebit(ISMINE_SPENDABLE_SHIELDED | ISMINE_WATCH_ONLY_SHIELDED));
    unsigned int txSize = wtx.tx->GetTotalSize();
    const uint256& txHash = wtx.GetHash();
    const int64_t txTime = wtx.GetTxTime();

    for (unsigned int nOut = 0; nOut < wtx.tx->vout.size(); nOut++) {
        const CTxOut& txout = wtx.tx->vout[nOut];

        if (wallet->IsMine(txout)) {
            // Ignore parts sent to self, as this is usually the change
            // from a transaction sent back to our own address.
            continue;
        }

        // Create record
        TransactionRecord sub(txHash, txTime, txSize);
        sub.idx = (int) nOut;
        sub.involvesWatchAddress = involvesWatchAddress;

        CTxDestination address;
        if (ExtractDestination(txout.scriptPubKey, address)) {
            //This is most likely only going to happen when resyncing deterministic wallet without the knowledge of the
            //private keys that the change was sent to. Do not display a "sent to" here.
            if (wtx.tx->HasZerocoinMintOutputs())
                continue;
            // Sent to PIVX Address
            sub.type = TransactionRecord::SendToAddress;
            sub.address = EncodeDestination(address);
        } else if (txout.IsZerocoinMint()){
            sub.type = TransactionRecord::ZerocoinMint;
            sub.address = getValueOrReturnEmpty(wtx.mapValue, "zerocoinmint");
            sub.credit += txout.nValue;
        } else {
            // Sent to IP, or other non-address transaction like OP_EVAL
            sub.type = TransactionRecord::SendToOther;
            sub.address = getValueOrReturnEmpty(wtx.mapValue, "to");
            if (sub.address.empty() && txout.scriptPubKey.StartsWithOpcode(OP_RETURN)) {
                sub.type = TransactionRecord::SendToNobody;
                // Burned PIVs, op_return could be for a kind of data stored there. For now, support UTF8 comments.
                std::string comment = wtx.GetComment();
                if (!comment.empty() && IsValidUTF8(comment)) {
                    sub.address = comment;
                }
                // Check if this is a budget proposal fee (future: encapsulate functionality inside wallet/governanceModel)
                std::string prop = getValueOrReturnEmpty(wtx.mapValue, "proposal");
                if (!prop.empty()) {
                    const std::vector<unsigned char> vec = ParseHex(prop);
                    if (!vec.empty()) {
                        CDataStream ss(vec, SER_DISK, CLIENT_VERSION);
                        CBudgetProposal proposal;
                        ss >> proposal;
                        sub.address = "Proposal: " + proposal.GetName();
                    }
                }
                // future: could expand this to support base64 or hex encoded messages
            }
        }

        CAmount nValue = txout.nValue;
        /* Add fee to first output */
        if (nTxFee > 0) {
            nValue += nTxFee;
            nTxFee = 0;
        }
        sub.debit = -nValue;

        parts.append(sub);
    }

    // Decompose shielded debit
    return decomposeShieldedDebitTransaction(wallet, wtx, nTxFee, involvesWatchAddress, parts) || !parts.empty();
}

// Check whether all the shielded inputs and outputs are from and send to this wallet
std::pair<bool, bool> areInputsAndOutputsFromAndToMe(const CWalletTx& wtx, SaplingScriptPubKeyMan* sspkm, bool& involvesWatchAddress)
{
    // Check if all the shielded spends are from me
    bool allShieldedSpendsFromMe = true;
    for (const auto& spend : wtx.tx->sapData->vShieldedSpend) {
        if (!sspkm->IsSaplingNullifierFromMe(spend.nullifier)) {
            allShieldedSpendsFromMe = false;
            break;
        }
    }

    // Check if all the shielded outputs are to me
    bool allShieldedOutToMe = true;
    for (int i = 0; i < (int) wtx.tx->sapData->vShieldedOutput.size(); ++i) {
        SaplingOutPoint op(wtx.GetHash(), i);
        isminetype mine = sspkm->IsMine(wtx, op);
        if (mine & ISMINE_WATCH_ONLY_SHIELDED) involvesWatchAddress = true;
        if (mine != ISMINE_SPENDABLE_SHIELDED) allShieldedOutToMe = false;
    }

    return std::make_pair(allShieldedSpendsFromMe, allShieldedOutToMe);
}

/*
 * Decompose CWallet transaction to model transaction records.
 */
QList<TransactionRecord> TransactionRecord::decomposeTransaction(const CWallet* wallet, const CWalletTx& wtx)
{
    QList<TransactionRecord> parts;
    CAmount nCredit = wtx.GetCredit(ISMINE_ALL);
    CAmount nDebit = wtx.GetDebit(ISMINE_ALL);

    // Decompose coinstake if needed (if it's not a coinstake, the method will no perform any action).
    if (decomposeCoinStake(wallet, wtx, nCredit, nDebit, parts)) {
        return parts;
    }

    // Decompose cold staking related transactions (with the exception of cold stakes that are decoupled in decomposeCoinStake)
    // future: merge this flow with the 'credit/debit decomposing flow'.
    if (decomposeP2CS(wallet, wtx, nCredit, nDebit, parts)) {
        return parts;
    }

    // Decompose zerocoin spend tx if needed (if it's not a zc spend, the method will not perform any action)
    if (decomposeZcSpendTx(wallet, wtx, nCredit, nDebit, parts)) {
        return parts;
    }

    // Credit/Debit decomposing flow
    CAmount nNet = nCredit - nDebit;

    // Check if the tx is credit and decompose it.
    if (nNet > 0) {
        // Decouple credit tx
        if (decomposeCreditTransaction(wallet, wtx, parts)) {
            return parts;
        }
    }

    auto sspkm = wallet->GetSaplingScriptPubKeyMan();
    // As the tx is not credit, need to check if all the inputs and outputs are from and to this wallet.
    // If it's true, then it's a sendToSelf. If not, then it's an outgoing tx.

    bool involvesWatchAddress = false;
    isminetype fAllFromMe = ISMINE_SPENDABLE;
    for (const CTxIn& txin : wtx.tx->vin) {
        isminetype mine = wallet->IsMine(txin);
        if (mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
        if (fAllFromMe > mine) fAllFromMe = mine;
    }

    isminetype fAllToMe = ISMINE_SPENDABLE;
    for (const CTxOut& txout : wtx.tx->vout) {
        isminetype mine = wallet->IsMine(txout);
        if (mine & ISMINE_WATCH_ONLY) involvesWatchAddress = true;
        if (fAllToMe > mine) fAllToMe = mine;
    }

    // Check whether all the shielded spends/outputs are from or to me.
    bool allShieldedSpendsFromMe, allShieldedOutToMe = true;
    std::tie(allShieldedSpendsFromMe, allShieldedOutToMe) =
            areInputsAndOutputsFromAndToMe(wtx, sspkm, involvesWatchAddress);

    // Check if this tx is purely a payment to self.
    if (fAllFromMe && fAllToMe && allShieldedOutToMe && allShieldedSpendsFromMe) {
        // Single record for sendToSelf.
        if (decomposeSendToSelfTransaction(wtx, nCredit, nDebit, involvesWatchAddress, parts, wallet)) {
            return parts;
        }
    }

    // Check if the tx is debit and decompose it.
    if (fAllFromMe || wtx.tx->HasZerocoinMintOutputs()) {
        if (decomposeDebitTransaction(wallet, wtx, nDebit, involvesWatchAddress, parts)) {
            return parts;
        }
    }

    // Check if wasn't able to decompose the transaction
    if (parts.empty()) {
        // if we get to this point, we have a mixed debit transaction, can't break down payees.
        TransactionRecord record(wtx.GetHash(), wtx.GetTxTime(), wtx.tx->GetTotalSize(), TransactionRecord::Other, "", nNet,
                                 0);
        record.involvesWatchAddress = involvesWatchAddress;
        parts.append(record);
    }
    return parts;
}

bool ExtractAddress(const CScript& scriptPubKey, bool fColdStake, std::string& addressStr) {
    CTxDestination address;
    if (!ExtractDestination(scriptPubKey, address, fColdStake)) {
        // this shouldn't happen..
        addressStr = "No available address";
        return false;
    } else {
        addressStr = EncodeDestination(
                address,
                (fColdStake ? CChainParams::STAKING_ADDRESS : CChainParams::PUBKEY_ADDRESS)
        );
        return true;
    }
}

void TransactionRecord::loadUnlockColdStake(const CWallet* wallet, const CWalletTx& wtx, TransactionRecord& record)
{
    record.involvesWatchAddress = false;

    // Get the p2cs
    const CScript* p2csScript = nullptr;
    bool isSpendable = false;

    for (const auto &input : wtx.tx->vin) {
        const CWalletTx* tx = wallet->GetWalletTx(input.prevout.hash);
        if (tx && tx->tx->vout[input.prevout.n].scriptPubKey.IsPayToColdStaking()) {
            p2csScript = &tx->tx->vout[input.prevout.n].scriptPubKey;
            isSpendable = wallet->IsMine(input) & ISMINE_SPENDABLE_ALL;
            break;
        }
    }

    if (isSpendable) {
        // owner unlocked the cold stake
        record.type = TransactionRecord::P2CSUnlockOwner;
        record.debit = -(wtx.GetStakeDelegationDebit());
        record.credit = wtx.GetCredit(ISMINE_ALL);
    } else {
        // hot node watching the unlock
        record.type = TransactionRecord::P2CSUnlockStaker;
        record.debit = -(wtx.GetColdStakingDebit());
        record.credit = -(wtx.GetColdStakingCredit());
    }

    // Extract and set the owner address
    if (p2csScript) {
        ExtractAddress(*p2csScript, false, record.address);
    }
}

void TransactionRecord::loadHotOrColdStakeOrContract(
        const CWallet* wallet,
        const CWalletTx& wtx,
        TransactionRecord& record,
        bool isContract)
{
    record.involvesWatchAddress = false;

    // Get the p2cs
    CTxOut p2csUtxo;
    for (const auto & txout : wtx.tx->vout) {
        if (txout.scriptPubKey.IsPayToColdStaking()) {
            p2csUtxo = txout;
            break;
        }
    }

    bool isSpendable = (wallet->IsMine(p2csUtxo) & ISMINE_SPENDABLE_DELEGATED);
    bool isFromMe = wallet->IsFromMe(wtx.tx);

    if (isContract) {
        if (isSpendable && isFromMe) {
            // Wallet delegating balance
            record.type = TransactionRecord::P2CSDelegationSentOwner;
        } else if (isFromMe){
            // Wallet delegating balance and transfering ownership
            record.type = TransactionRecord::P2CSDelegationSent;
        } else {
            // Wallet receiving a delegation
            record.type = TransactionRecord::P2CSDelegation;
        }
    } else {
        // Stake
        if (isSpendable) {
            // Offline wallet receiving an stake due a delegation
            record.type = TransactionRecord::StakeDelegated;
            record.credit = wtx.GetCredit(ISMINE_SPENDABLE_DELEGATED);
            record.debit = -(wtx.GetDebit(ISMINE_SPENDABLE_DELEGATED));
        } else {
            // Online wallet receiving an stake due a received utxo delegation that won a block.
            record.type = TransactionRecord::StakeHot;
        }
    }

    // Extract and set the owner address
    ExtractAddress(p2csUtxo.scriptPubKey, false, record.address);
}

void TransactionRecord::updateStatus(const CWalletTx& wtx, int chainHeight)
{
    // Determine transaction status

    // Update time if needed
    int64_t nTxTime = wtx.GetTxTime();
    if (time != nTxTime) time = nTxTime;

    // Sort order, unrecorded transactions sort to the top
    status.sortKey = strprintf("%010d-%01d-%010u-%03d",
        wtx.m_confirm.block_height,
        ((wtx.IsCoinBase() || wtx.IsCoinStake()) ? 1 : 0),
        time,
        idx);

    bool fConflicted = false;
    int depth = 0;
    bool isTrusted = wtx.IsTrusted(depth, fConflicted);
    int nBlocksToMaturity = (wtx.IsCoinBase() || wtx.IsCoinStake()) ? std::max(0, (Params().GetConsensus().nCoinbaseMaturity + 1) - depth) : 0;

    status.countsForBalance = isTrusted && !(nBlocksToMaturity > 0);
    status.cur_num_blocks = chainHeight;
    status.depth = depth;

    if (!IsFinalTx(wtx.tx, chainHeight + 1)) {
        if (wtx.tx->nLockTime < LOCKTIME_THRESHOLD) {
            status.status = TransactionStatus::OpenUntilBlock;
            status.open_for = wtx.tx->nLockTime - chainHeight;
        } else {
            status.status = TransactionStatus::OpenUntilDate;
            status.open_for = wtx.tx->nLockTime;
        }
    }
    // For generated transactions, determine maturity
    else if (type == TransactionRecord::Generated ||
            type == TransactionRecord::StakeMint ||
            type == TransactionRecord::StakeZPIV ||
            type == TransactionRecord::MNReward ||
            type == TransactionRecord::BudgetPayment ||
            type == TransactionRecord::StakeDelegated ||
            type == TransactionRecord::StakeHot) {

        if (nBlocksToMaturity > 0) {
            status.status = TransactionStatus::Immature;
            status.matures_in = nBlocksToMaturity;

            if (status.depth < 0 || fConflicted) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                status.status = TransactionStatus::NotAccepted;
            }
        } else {
            status.status = TransactionStatus::Confirmed;
<<<<<<< HEAD
        }
    } else {
        if (status.depth < 0) {
            status.status = TransactionStatus::Conflicted;
        } else if (GetAdjustedTime() - wtx.nTimeReceived > 2 * 60 && wtx.GetRequestCount() == 0) {
            status.status = TransactionStatus::Offline;
=======
            status.matures_in = 0;
        }
    } else {
        if (status.depth < 0 || fConflicted) {
            status.status = TransactionStatus::Conflicted;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else if (status.depth == 0) {
            status.status = TransactionStatus::Unconfirmed;
        } else if (status.depth < RecommendedNumConfirmations) {
            status.status = TransactionStatus::Confirming;
        } else {
            status.status = TransactionStatus::Confirmed;
        }
    }
<<<<<<< HEAD
}

bool TransactionRecord::statusUpdateNeeded()
{
    AssertLockHeld(cs_main);
    return status.cur_num_blocks != chainActive.Height() || status.cur_num_ix_locks != nCompleteTXLocks;
}

QString TransactionRecord::getTxID() const
{
    return QString::fromStdString(hash.ToString());
}

int TransactionRecord::getOutputIndex() const
{
    return idx;
=======
    status.needsUpdate = false;
}

bool TransactionRecord::statusUpdateNeeded(int blockHeight) const
{
    return status.cur_num_blocks != blockHeight || status.needsUpdate;
}

int TransactionRecord::getOutputIndex() const
{
    return idx;
}

bool TransactionRecord::isCoinStake() const
{
    return type == TransactionRecord::StakeMint || type == TransactionRecord::Generated || type == TransactionRecord::StakeZPIV;
}

bool TransactionRecord::isMNReward() const
{
    return type == TransactionRecord::MNReward;
}

bool TransactionRecord::isAnyColdStakingType() const
{
    return (type == TransactionRecord::P2CSDelegation || type == TransactionRecord::P2CSDelegationSent
            || type == TransactionRecord::P2CSDelegationSentOwner
            || type == TransactionRecord::StakeDelegated || type == TransactionRecord::StakeHot
            || type == TransactionRecord::P2CSUnlockOwner || type == TransactionRecord::P2CSUnlockStaker);
}

bool TransactionRecord::isNull() const
{
    return hash.IsNull() || size == 0;
}

std::string TransactionRecord::statusToString(){
    switch (status.status){
        case TransactionStatus::Confirmed:
            return "Confirmed";
        case TransactionStatus::OpenUntilDate:
            return "OpenUntilDate";
        case TransactionStatus::OpenUntilBlock:
            return "OpenUntilBlock";
        case TransactionStatus::Unconfirmed:
            return "Unconfirmed";
        case TransactionStatus::Confirming:
            return "Confirming";
        case TransactionStatus::Conflicted:
            return "Conflicted";
        case TransactionStatus::Immature:
            return "Immature";
        case TransactionStatus::NotAccepted:
            return "Not Accepted";
        default:
            return "No status";
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
