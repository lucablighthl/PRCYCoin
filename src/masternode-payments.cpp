// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternode-payments.h"
<<<<<<< HEAD
#include "addrman.h"
#include "masternode-budget.h"
#include "masternode-sync.h"
#include "masternodeman.h"
#include "obfuscation.h"
#include "sync.h"
#include "util.h"
#include "utilmoneystr.h"
#include <boost/filesystem.hpp>
=======

#include "chainparams.h"
#include "evo/deterministicmns.h"
#include "fs.h"
#include "budget/budgetmanager.h"
#include "masternode-sync.h"
#include "masternodeman.h"
#include "netmessagemaker.h"
#include "spork.h"
#include "sync.h"
#include "util/system.h"
#include "utilmoneystr.h"
#include "validation.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Object for who's going to get paid on which blocks */
CMasternodePayments masternodePayments;

RecursiveMutex cs_vecPayments;
RecursiveMutex cs_mapMasternodeBlocks;
RecursiveMutex cs_mapMasternodePayeeVotes;

<<<<<<< HEAD
=======
static const int MNPAYMENTS_DB_VERSION = 1;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
//
// CMasternodePaymentDB
//

CMasternodePaymentDB::CMasternodePaymentDB()
{
    pathDB = GetDataDir() / "mnpayments.dat";
    strMagicMessage = "MasternodePayments";
}

bool CMasternodePaymentDB::Write(const CMasternodePayments& objToSave)
{
    int64_t nStart = GetTimeMillis();

    // serialize, checksum data up to that point, then append checksum
    CDataStream ssObj(SER_DISK, CLIENT_VERSION);
<<<<<<< HEAD
    ssObj << strMagicMessage;                   // masternode cache file specific magic message
    ssObj << FLATDATA(Params().MessageStart()); // network specific magic number
=======
    ssObj << MNPAYMENTS_DB_VERSION;
    ssObj << strMagicMessage;                   // masternode cache file specific magic message
    ssObj << Params().MessageStart(); // network specific magic number
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ssObj << objToSave;
    uint256 hash = Hash(ssObj.begin(), ssObj.end());
    ssObj << hash;

    // open output file, and associate with CAutoFile
<<<<<<< HEAD
    FILE* file = fopen(pathDB.string().c_str(), "wb");
=======
    FILE* file = fsbridge::fopen(pathDB, "wb");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CAutoFile fileout(file, SER_DISK, CLIENT_VERSION);
    if (fileout.IsNull())
        return error("%s : Failed to open file %s", __func__, pathDB.string());

    // Write and commit header, data
    try {
        fileout << ssObj;
    } catch (const std::exception& e) {
        return error("%s : Serialize or I/O error - %s", __func__, e.what());
    }
    fileout.fclose();

<<<<<<< HEAD
    LogPrint("masternode", "Written info to mnpayments.dat  %dms\n", GetTimeMillis() - nStart);
=======
    LogPrint(BCLog::MASTERNODE,"Written info to mnpayments.dat  %dms\n", GetTimeMillis() - nStart);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return true;
}

<<<<<<< HEAD
CMasternodePaymentDB::ReadResult CMasternodePaymentDB::Read(CMasternodePayments& objToLoad, bool fDryRun)
{
    int64_t nStart = GetTimeMillis();
    // open input file, and associate with CAutoFile
    FILE* file = fopen(pathDB.string().c_str(), "rb");
=======
CMasternodePaymentDB::ReadResult CMasternodePaymentDB::Read(CMasternodePayments& objToLoad)
{
    int64_t nStart = GetTimeMillis();
    // open input file, and associate with CAutoFile
    FILE* file = fsbridge::fopen(pathDB, "rb");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CAutoFile filein(file, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull()) {
        error("%s : Failed to open file %s", __func__, pathDB.string());
        return FileError;
    }

    // use file size to size memory buffer
<<<<<<< HEAD
    int fileSize = boost::filesystem::file_size(pathDB);
=======
    int fileSize = fs::file_size(pathDB);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int dataSize = fileSize - sizeof(uint256);
    // Don't try to resize to a negative number if file is small
    if (dataSize < 0)
        dataSize = 0;
<<<<<<< HEAD
    vector<unsigned char> vchData;
=======
    std::vector<unsigned char> vchData;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    vchData.resize(dataSize);
    uint256 hashIn;

    // read data and checksum from file
    try {
<<<<<<< HEAD
        filein.read((char*)&vchData[0], dataSize);
=======
        filein.read((char*)vchData.data(), dataSize);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        filein >> hashIn;
    } catch (const std::exception& e) {
        error("%s : Deserialize or I/O error - %s", __func__, e.what());
        return HashReadError;
    }
    filein.fclose();

    CDataStream ssObj(vchData, SER_DISK, CLIENT_VERSION);

    // verify stored checksum matches input data
    uint256 hashTmp = Hash(ssObj.begin(), ssObj.end());
    if (hashIn != hashTmp) {
        error("%s : Checksum mismatch, data corrupted", __func__);
        return IncorrectHash;
    }

<<<<<<< HEAD
    unsigned char pchMsgTmp[4];
    std::string strMagicMessageTmp;
    try {
        // de-serialize file header (masternode cache file specific magic message) and ..
=======
    int version;
    std::string strMagicMessageTmp;
    try {
        // de-serialize file header
        ssObj >> version;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ssObj >> strMagicMessageTmp;

        // ... verify the message matches predefined one
        if (strMagicMessage != strMagicMessageTmp) {
            error("%s : Invalid masternode payement cache magic message", __func__);
            return IncorrectMagicMessage;
        }

<<<<<<< HEAD

        // de-serialize file header (network specific magic number) and ..
        ssObj >> FLATDATA(pchMsgTmp);

        // ... verify the network matches ours
        if (memcmp(pchMsgTmp, Params().MessageStart(), sizeof(pchMsgTmp))) {
=======
        // de-serialize file header (network specific magic number) and ..
        std::vector<unsigned char> pchMsgTmp(4);
        ssObj >> MakeSpan(pchMsgTmp);

        // ... verify the network matches ours
        if (memcmp(pchMsgTmp.data(), Params().MessageStart(), pchMsgTmp.size()) != 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            error("%s : Invalid network magic number", __func__);
            return IncorrectMagicNumber;
        }

        // de-serialize data into CMasternodePayments object
        ssObj >> objToLoad;
    } catch (const std::exception& e) {
        objToLoad.Clear();
        error("%s : Deserialize or I/O error - %s", __func__, e.what());
        return IncorrectFormat;
    }

<<<<<<< HEAD
    LogPrint("masternode", "Loaded info from mnpayments.dat  %dms\n", GetTimeMillis() - nStart);
    LogPrint("masternode", "  %s\n", objToLoad.ToString());
    if (!fDryRun) {
        LogPrint("masternode", "Masternode payments manager - cleaning....\n");
        objToLoad.CleanPaymentList();
        LogPrint("masternode", "Masternode payments manager - result:\n");
        LogPrint("masternode", "  %s\n", objToLoad.ToString());
    }
=======
    LogPrint(BCLog::MASTERNODE,"Loaded info from mnpayments.dat (dbversion=%d) %dms\n", version, GetTimeMillis() - nStart);
    LogPrint(BCLog::MASTERNODE,"  %s\n", objToLoad.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return Ok;
}

<<<<<<< HEAD
void DumpMasternodePayments()
{
    int64_t nStart = GetTimeMillis();

    CMasternodePaymentDB paymentdb;
    CMasternodePayments tempPayments;

    LogPrint("masternode", "Verifying mnpayments.dat format...\n");
    CMasternodePaymentDB::ReadResult readResult = paymentdb.Read(tempPayments, true);
    // there was an error and it was not an error on file opening => do not proceed
    if (readResult == CMasternodePaymentDB::FileError)
        LogPrint("masternode", "Missing budgets file - mnpayments.dat, will try to recreate\n");
    else if (readResult != CMasternodePaymentDB::Ok) {
        LogPrint("masternode", "Error reading mnpayments.dat: ");
        if (readResult == CMasternodePaymentDB::IncorrectFormat)
            LogPrint("masternode", "magic is ok but data has invalid format, will try to recreate\n");
        else {
            LogPrint("masternode", "file format is unknown or invalid, please fix it manually\n");
            return;
        }
    }
    LogPrint("masternode", "Writting info to mnpayments.dat...\n");
    paymentdb.Write(masternodePayments);

    LogPrint("masternode", "Budget dump finished  %dms\n", GetTimeMillis() - nStart);
}

bool IsBlockValueValid(const CBlock& block, CAmount nExpectedValue, CAmount nMinted)
{
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (pindexPrev == NULL) return true;

    int nHeight = 0;
    if (pindexPrev->GetBlockHash() == block.hashPrevBlock) {
        nHeight = pindexPrev->nHeight + 1;
    } else { //out of order
        BlockMap::iterator mi = mapBlockIndex.find(block.hashPrevBlock);
        if (mi != mapBlockIndex.end() && (*mi).second)
            nHeight = (*mi).second->nHeight + 1;
    }

    if (nHeight == 0) {
        LogPrint("masternode", "IsBlockValueValid() : WARNING: Couldn't find previous block\n");
    }

    if (!masternodeSync.IsSynced()) { //there is no budget data to use to check anything
        //super blocks will always be on these blocks, max 100 per budgeting
        if (nHeight % GetBudgetPaymentCycleBlocks() < 100) {
            return true;
        } else {
            if (nMinted > nExpectedValue) {
                return false;
            }
        }
    } else { // we're synced and have data so check the budget schedule

        if (budget.IsBudgetPaymentBlock(nHeight)) {
            //the value of the block is evaluated in CheckBlock
            return true;
        } else {
            if (nMinted > nExpectedValue) {
                return false;
=======
uint256 CMasternodePaymentWinner::GetHash() const
{
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << std::vector<unsigned char>(payee.begin(), payee.end());
    ss << nBlockHeight;
    ss << vinMasternode.prevout;
    return ss.GetHash();
}

std::string CMasternodePaymentWinner::GetStrMessage() const
{
    return vinMasternode.prevout.ToStringShort() + std::to_string(nBlockHeight) + HexStr(payee);
}

bool CMasternodePaymentWinner::IsValid(CNode* pnode, CValidationState& state, int chainHeight)
{
    int n = mnodeman.GetMasternodeRank(vinMasternode, nBlockHeight - 100);
    if (n < 1 || n > MNPAYMENTS_SIGNATURES_TOTAL) {
        return state.Error(strprintf("Masternode not in the top %d (%d)", MNPAYMENTS_SIGNATURES_TOTAL, n));
    }

    // Must be a P2PKH
    if (!payee.IsPayToPublicKeyHash()) {
        return state.Error("payee must be a P2PKH");
    }

    return true;
}

void CMasternodePaymentWinner::Relay()
{
    CInv inv(MSG_MASTERNODE_WINNER, GetHash());
    g_connman->RelayInv(inv);
}

void DumpMasternodePayments()
{
    int64_t nStart = GetTimeMillis();

    CMasternodePaymentDB paymentdb;
    LogPrint(BCLog::MASTERNODE,"Writing info to mnpayments.dat...\n");
    paymentdb.Write(masternodePayments);

    LogPrint(BCLog::MASTERNODE,"Budget dump finished  %dms\n", GetTimeMillis() - nStart);
}

bool IsBlockValueValid(int nHeight, CAmount& nExpectedValue, CAmount nMinted, CAmount& nBudgetAmt)
{
    const Consensus::Params& consensus = Params().GetConsensus();
    if (!masternodeSync.IsSynced()) {
        //there is no budget data to use to check anything
        //super blocks will always be on these blocks, max 100 per budgeting
        if (nHeight % consensus.nBudgetCycleBlocks < 100) {
            if (Params().IsTestnet()) {
                return true;
            }
            nExpectedValue += g_budgetman.GetTotalBudget(nHeight);
        }
    } else {
        // we're synced and have data so check the budget schedule
        // if the superblock spork is enabled
        if (sporkManager.IsSporkActive(SPORK_13_ENABLE_SUPERBLOCKS)) {
            // add current payee amount to the expected block value
            if (g_budgetman.GetExpectedPayeeAmount(nHeight, nBudgetAmt)) {
                nExpectedValue += nBudgetAmt;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }
    }

<<<<<<< HEAD
    return true;
}

bool IsBlockPayeeValid(const CBlock& block, int nBlockHeight)
{
    if (!masternodeSync.IsSynced()) { //there is no budget data to use to check anything -- find the longest chain
        LogPrint("mnpayments", "Client not synced, skipping block payee checks\n");
        return true;
    }

    const CTransaction& txNew = (nBlockHeight > Params().LAST_POW_BLOCK() ? block.vtx[1] : block.vtx[0]);

    //check for masternode payee
    if (masternodePayments.IsTransactionValid(txNew, nBlockHeight))
        return true;
    LogPrint("masternode", "Invalid mn payment detected %s\n", txNew.ToString().c_str());

    LogPrint("masternode", "Masternode payment enforcement is disabled, accepting block\n");

=======
    if (nMinted < 0 && consensus.NetworkUpgradeActive(nHeight, Consensus::UPGRADE_V5_3)) {
        return false;
    }

    return nMinted <= nExpectedValue;
}

bool IsBlockPayeeValid(const CBlock& block, const CBlockIndex* pindexPrev)
{
    int nBlockHeight = pindexPrev->nHeight + 1;
    TrxValidationStatus transactionStatus = TrxValidationStatus::InValid;

    if (!masternodeSync.IsSynced()) { //there is no budget data to use to check anything -- find the longest chain
        LogPrint(BCLog::MASTERNODE, "Client not synced, skipping block payee checks\n");
        return true;
    }

    const bool fPayCoinstake = Params().GetConsensus().NetworkUpgradeActive(nBlockHeight, Consensus::UPGRADE_POS) &&
                               !Params().GetConsensus().NetworkUpgradeActive(nBlockHeight, Consensus::UPGRADE_V6_0);
    const CTransaction& txNew = *(fPayCoinstake ? block.vtx[1] : block.vtx[0]);

    //check if it's a budget block
    if (sporkManager.IsSporkActive(SPORK_13_ENABLE_SUPERBLOCKS)) {
        if (g_budgetman.IsBudgetPaymentBlock(nBlockHeight)) {
            transactionStatus = g_budgetman.IsTransactionValid(txNew, block.GetHash(), nBlockHeight);
            if (transactionStatus == TrxValidationStatus::Valid) {
                return true;
            }

            if (transactionStatus == TrxValidationStatus::InValid) {
                LogPrint(BCLog::MASTERNODE,"Invalid budget payment detected %s\n", txNew.ToString().c_str());
                if (sporkManager.IsSporkActive(SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT))
                    return false;

                LogPrint(BCLog::MASTERNODE,"Budget enforcement is disabled, accepting block\n");
            }
        }
    }

    // If we end here the transaction was either TrxValidationStatus::InValid and Budget enforcement is disabled, or
    // a double budget payment (status = TrxValidationStatus::DoublePayment) was detected, or no/not enough masternode
    // votes (status = TrxValidationStatus::VoteThreshold) for a finalized budget were found
    // In all cases a masternode will get the payment for this block

    //check for masternode payee
    if (masternodePayments.IsTransactionValid(txNew, pindexPrev))
        return true;
    LogPrint(BCLog::MASTERNODE,"Invalid mn payment detected %s\n", txNew.ToString().c_str());

    if (sporkManager.IsSporkActive(SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT))
        return false;
    LogPrint(BCLog::MASTERNODE,"Masternode payment enforcement is disabled, accepting block\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return true;
}


<<<<<<< HEAD
bool FillBlockPayee(CMutableTransaction& txNew, CAmount nFees, bool fProofOfStake)
{
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (!pindexPrev) return false;

    return masternodePayments.FillBlockPayee(txNew, nFees, fProofOfStake);
=======
void FillBlockPayee(CMutableTransaction& txCoinbase, CMutableTransaction& txCoinstake, const CBlockIndex* pindexPrev, bool fProofOfStake)
{
    if (!sporkManager.IsSporkActive(SPORK_13_ENABLE_SUPERBLOCKS) ||           // if superblocks are not enabled
            // ... or this is not a superblock
            !g_budgetman.FillBlockPayee(txCoinbase, txCoinstake, pindexPrev->nHeight + 1, fProofOfStake) ) {
        // ... or there's no budget with enough votes, then pay a masternode
        masternodePayments.FillBlockPayee(txCoinbase, txCoinstake, pindexPrev, fProofOfStake);
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

std::string GetRequiredPaymentsString(int nBlockHeight)
{
<<<<<<< HEAD
    return masternodePayments.GetRequiredPaymentsString(nBlockHeight);
}

bool CMasternodePayments::FillBlockPayee(CMutableTransaction& txNew, int64_t nFees, bool fProofOfStake)
{
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (!pindexPrev) return false;

    bool hasPayment = true;
    std::vector<unsigned char> payeeAddr;
    CScript payee;
    CKey mnPaymentPrivTx;
    mnPaymentPrivTx.MakeNewKey(true);
    CPubKey mnPaymentPubTx = mnPaymentPrivTx.GetPubKey();

    masternodePayments.GetBlockPayee(pindexPrev->nHeight + 1, payeeAddr);
    if (payeeAddr.size() != 0) {
        bool isNotSpent = false;
        std::vector<CMasternode> mns = mnodeman.GetFullMasternodeVector();
        for (CMasternode& mn : mns) {
            if (mn.vin.masternodeStealthAddress == payeeAddr && mn.IsEnabled()) {
                isNotSpent = true;
                break;
            }
        }
        if (!isNotSpent) payeeAddr.clear();
    }
    if (payeeAddr.size() == 0) {
        //no masternode detected
        CMasternode* winningNode = mnodeman.GetCurrentMasterNode(1);
        if (winningNode) {
            //generate payee based on masternodeStealthAddress
            payeeAddr = winningNode->vin.masternodeStealthAddress;
        }

        if (payeeAddr.size() != 0) {
            bool isNotSpent = false;
            std::vector<CMasternode> mns = mnodeman.GetFullMasternodeVector();
            for (CMasternode& mn : mns) {
                if (mn.vin.masternodeStealthAddress == payeeAddr && mn.IsEnabled()) {
                    isNotSpent = true;
                    break;
                }
            }
            if (!isNotSpent) payeeAddr.clear();
        }
    }
    /*std::string mnaddress = "41im5B4oiZ6WxMrQfXivfpZ5sMsPwbqhSSpDkvxxATq2QMvBa5nppNCYcESvLhGyEiZoEXyc8F5AJE3LymkrX24i17JicpNRAq8";
    std::vector<unsigned char> temp(mnaddress.begin(), mnaddress.end());
    payeeAddr = temp;*/
    if (payeeAddr.size() != 0) {
        std::string mnsa(payeeAddr.begin(), payeeAddr.end());

        //Parse stealth address
        CPubKey pubViewKey, pubSpendKey, des;
        bool hasPaymentID;
        uint64_t paymentID;
        if (!CWallet::DecodeStealthAddress(mnsa, pubViewKey, pubSpendKey, hasPaymentID, paymentID)) {
            throw runtime_error("Stealth address mal-formatted");
        }
        if (CWallet::ComputeStealthDestination(mnPaymentPrivTx, pubViewKey, pubSpendKey, des))
            payee = GetScriptForDestination(des);
    } else {
        hasPayment = false;
    }

    CAmount posBlockReward = GetBlockValue(pindexPrev->nHeight);
    CAmount masternodePayment = GetMasternodePayment(pindexPrev->nHeight, posBlockReward);
    if (hasPayment) {
        if (fProofOfStake) {
            /**For Proof Of Stake vout[0] must be null
             * Stake reward can be split into many different outputs, so we must
             * use vout.size() to align with several different cases.
             * An additional output is appended as the masternode payment
             */
            unsigned int i = txNew.vout.size();
            /**
             * Topdev update
             */
            txNew.vout.resize(i + 1);
            txNew.vout[i].scriptPubKey = payee;
            txNew.vout[i].nValue = masternodePayment;
            txNew.vout[i].masternodeStealthAddress = payeeAddr;
            std::copy(mnPaymentPrivTx.begin(), mnPaymentPrivTx.end(), std::back_inserter(txNew.vout[i].txPriv));
            std::copy(mnPaymentPubTx.begin(), mnPaymentPubTx.end(), std::back_inserter(txNew.vout[i].txPub));
            //subtract mn payment from the stake reward
            txNew.vout[i - 1].nValue -= masternodePayment;
        } else {
            txNew.vout.resize(2);
            txNew.vout[1].scriptPubKey = payee;
            txNew.vout[1].nValue = masternodePayment;
            txNew.vout[0].nValue = posBlockReward - masternodePayment;
        }

        LogPrintf("Masternode payment of %s to %s\n", FormatMoney(masternodePayment).c_str(), payee.ToString().c_str());
    }
    return true;
}

int CMasternodePayments::GetMinMasternodePaymentsProto()
{
    return MIN_PEER_PROTO_VERSION_BEFORE_ENFORCEMENT; // Also allow old peers as long as they are allowed to run
}

void CMasternodePayments::ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv)
{
    if (!masternodeSync.IsBlockchainSynced()) return;

    if (fLiteMode) return; //disable all Obfuscation/Masternode related functionality


    if (strCommand == "mnget") { //Masternode Payments Request Sync
        if (fLiteMode) return;   //disable all Obfuscation/Masternode related functionality

        int nCountNeeded;
        vRecv >> nCountNeeded;

        if (Params().NetworkID() == CBaseChainParams::MAIN) {
            if (pfrom->HasFulfilledRequest("mnget")) {
                LogPrintf("CMasternodePayments::ProcessMessageMasternodePayments() : mnget - peer already asked me for the list\n");
                Misbehaving(pfrom->GetId(), 20);
                return;
            }
        }

        pfrom->FulfilledRequest("mnget");
        masternodePayments.Sync(pfrom, nCountNeeded);
        LogPrint("mnpayments", "mnget - Sent Masternode winners to peer %i\n", pfrom->GetId());
    } else if (strCommand == "mnw") { //Masternode Payments Declare Winner
        //this is required in litemodef
        CMasternodePaymentWinner winner;
        vRecv >> winner;

        if (pfrom->nVersion < ActiveProtocol()) return;

        int nHeight;
        {
            TRY_LOCK(cs_main, locked);
            if (!locked || chainActive.Tip() == NULL) return;
            nHeight = chainActive.Tip()->nHeight;
        }

        if (masternodePayments.mapMasternodePayeeVotes.count(winner.GetHash())) {
            LogPrint("mnpayments", "mnw - Already seen - %s bestHeight %d\n", winner.GetHash().ToString().c_str(), nHeight);
            masternodeSync.AddedMasternodeWinner(winner.GetHash());
            return;
        }

        int nFirstBlock = nHeight - (mnodeman.CountEnabled() * 1.25);
        if (winner.nBlockHeight < nFirstBlock || winner.nBlockHeight > nHeight + 20) {
            LogPrint("mnpayments", "mnw - winner out of range - FirstBlock %d Height %d bestHeight %d\n", nFirstBlock, winner.nBlockHeight, nHeight);
            return;
        }

        std::string strError = "";
        if (!winner.IsValid(pfrom, strError)) {
            return;
        }

        if (!masternodePayments.CanVote(winner.vinMasternode.prevout, winner.nBlockHeight)) {
            return;
        }

        if (!winner.SignatureValid()) {
            if (masternodeSync.IsSynced()) {
                LogPrintf("CMasternodePayments::ProcessMessageMasternodePayments() : mnw - invalid signature\n");
                Misbehaving(pfrom->GetId(), 20);
            }
            // it could just be a non-synced masternode
            mnodeman.AskForMN(pfrom, winner.vinMasternode);
            return;
        }

        if (masternodePayments.AddWinningMasternode(winner)) {
            winner.Relay();
            masternodeSync.AddedMasternodeWinner(winner.GetHash());
        }
    }
}

bool CMasternodePaymentWinner::Sign(CKey& keyMasternode, CPubKey& pubKeyMasternode)
{
    std::string errorMessage;
    std::string strMasterNodeSignMessage;
    std::string payeeString(payee.begin(), payee.end());
    HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << vinMasternode.prevout.GetHash() << nBlockHeight << payee;
    std::string strMessage = HEX_STR(ser);


    if (!obfuScationSigner.SignMessage(strMessage, errorMessage, vchSig, keyMasternode)) {
        LogPrint("masternode", "CMasternodePing::Sign() - Error: %s\n", errorMessage.c_str());
        return false;
    }

    if (!obfuScationSigner.VerifyMessage(pubKeyMasternode, vchSig, strMessage, errorMessage)) {
        LogPrint("masternode", "CMasternodePing::Sign() - Error: %s\n", errorMessage.c_str());
        return false;
    }

    return true;
}

bool CMasternodePayments::GetBlockPayee(int nBlockHeight, std::vector<unsigned char>& payee)
{
    if (mapMasternodeBlocks.count(nBlockHeight)) {
        return mapMasternodeBlocks[nBlockHeight].GetPayee(payee);
=======
    if (sporkManager.IsSporkActive(SPORK_13_ENABLE_SUPERBLOCKS) && g_budgetman.IsBudgetPaymentBlock(nBlockHeight)) {
        return g_budgetman.GetRequiredPaymentsString(nBlockHeight);
    } else {
        return masternodePayments.GetRequiredPaymentsString(nBlockHeight);
    }
}

bool CMasternodePayments::GetMasternodeTxOuts(const CBlockIndex* pindexPrev, std::vector<CTxOut>& voutMasternodePaymentsRet) const
{
    if (deterministicMNManager->LegacyMNObsolete(pindexPrev->nHeight + 1)) {
        CAmount masternodeReward = GetMasternodePayment();
        auto dmnPayee = deterministicMNManager->GetListForBlock(pindexPrev).GetMNPayee();
        if (!dmnPayee) {
            return error("%s: Failed to get payees for block at height %d", __func__, pindexPrev->nHeight + 1);
        }
        CAmount operatorReward = 0;
        if (dmnPayee->nOperatorReward != 0 && !dmnPayee->pdmnState->scriptOperatorPayout.empty()) {
            operatorReward = (masternodeReward * dmnPayee->nOperatorReward) / 10000;
            masternodeReward -= operatorReward;
        }
        if (masternodeReward > 0) {
            voutMasternodePaymentsRet.emplace_back(masternodeReward, dmnPayee->pdmnState->scriptPayout);
        }
        if (operatorReward > 0) {
            voutMasternodePaymentsRet.emplace_back(operatorReward, dmnPayee->pdmnState->scriptOperatorPayout);
        }
        return true;
    }

    // Legacy payment logic. !TODO: remove when transition to DMN is complete
    return GetLegacyMasternodeTxOut(pindexPrev->nHeight + 1, voutMasternodePaymentsRet);
}

bool CMasternodePayments::GetLegacyMasternodeTxOut(int nHeight, std::vector<CTxOut>& voutMasternodePaymentsRet) const
{
    voutMasternodePaymentsRet.clear();

    CScript payee;
    if (!GetBlockPayee(nHeight, payee)) {
        //no masternode detected
        const uint256& hash = mnodeman.GetHashAtHeight(nHeight - 1);
        MasternodeRef winningNode = mnodeman.GetCurrentMasterNode(hash);
        if (winningNode) {
            payee = winningNode->GetPayeeScript();
        } else {
            LogPrint(BCLog::MASTERNODE,"CreateNewBlock: Failed to detect masternode to pay\n");
            return false;
        }
    }
    voutMasternodePaymentsRet.emplace_back(GetMasternodePayment(), payee);
    return true;
}

static void SubtractMnPaymentFromCoinstake(CMutableTransaction& txCoinstake, CAmount masternodePayment, int stakerOuts)
{
    assert (stakerOuts >= 2);
    //subtract mn payment from the stake reward
    if (stakerOuts == 2) {
        // Majority of cases; do it quick and move on
        txCoinstake.vout[1].nValue -= masternodePayment;
    } else {
        // special case, stake is split between (stakerOuts-1) outputs
        unsigned int outputs = stakerOuts-1;
        CAmount mnPaymentSplit = masternodePayment / outputs;
        CAmount mnPaymentRemainder = masternodePayment - (mnPaymentSplit * outputs);
        for (unsigned int j=1; j<=outputs; j++) {
            txCoinstake.vout[j].nValue -= mnPaymentSplit;
        }
        // in case it's not an even division, take the last bit of dust from the last one
        txCoinstake.vout[outputs].nValue -= mnPaymentRemainder;
    }
}

void CMasternodePayments::FillBlockPayee(CMutableTransaction& txCoinbase, CMutableTransaction& txCoinstake, const CBlockIndex* pindexPrev, bool fProofOfStake) const
{
    std::vector<CTxOut> vecMnOuts;
    if (!GetMasternodeTxOuts(pindexPrev, vecMnOuts)) {
        return;
    }

    // Starting from PIVX v6.0 masternode and budgets are paid in the coinbase tx
    const int nHeight = pindexPrev->nHeight + 1;
    bool fPayCoinstake = fProofOfStake && !Params().GetConsensus().NetworkUpgradeActive(nHeight, Consensus::UPGRADE_V6_0);

    // if PoS block pays the coinbase, clear it first
    if (fProofOfStake && !fPayCoinstake) txCoinbase.vout.clear();

    const int initial_cstake_outs = txCoinstake.vout.size();

    CAmount masternodePayment{0};
    for (const CTxOut& mnOut: vecMnOuts) {
        // Add the mn payment to the coinstake/coinbase tx
        if (fPayCoinstake) {
            txCoinstake.vout.emplace_back(mnOut);
        } else {
            txCoinbase.vout.emplace_back(mnOut);
        }
        masternodePayment += mnOut.nValue;
        CTxDestination payeeDest;
        ExtractDestination(mnOut.scriptPubKey, payeeDest);
        LogPrint(BCLog::MASTERNODE,"Masternode payment of %s to %s\n", FormatMoney(mnOut.nValue), EncodeDestination(payeeDest));
    }

    // Subtract mn payment value from the block reward
    if (fProofOfStake) {
        SubtractMnPaymentFromCoinstake(txCoinstake, masternodePayment, initial_cstake_outs);
    } else {
        txCoinbase.vout[0].nValue = GetBlockValue(nHeight) - masternodePayment;
    }
}

bool CMasternodePayments::ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv, CValidationState& state)
{
    if (!masternodeSync.IsBlockchainSynced()) return true;

    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        LogPrint(BCLog::MASTERNODE, "mnw - skip obsolete message %s\n", strCommand);
        return true;
    }

    if (strCommand == NetMsgType::GETMNWINNERS) {
        //Masternode Payments Request Sync
        int nCountNeeded;
        vRecv >> nCountNeeded;

        if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
            if (pfrom->HasFulfilledRequest(NetMsgType::GETMNWINNERS)) {
                LogPrint(BCLog::MASTERNODE, "%s: mnget - peer already asked me for the list\n", __func__);
                return state.DoS(20, false, REJECT_INVALID, "getmnwinners-request-already-fulfilled");
            }
        }

        pfrom->FulfilledRequest(NetMsgType::GETMNWINNERS);
        Sync(pfrom, nCountNeeded);
        LogPrint(BCLog::MASTERNODE, "mnget - Sent Masternode winners to peer %i\n", pfrom->GetId());
    } else if (strCommand == NetMsgType::MNWINNER) {
        //Masternode Payments Declare Winner
        CMasternodePaymentWinner winner;
        vRecv >> winner;
        if (pfrom->nVersion < ActiveProtocol()) return false;

        {
            // Clear inv request
            LOCK(cs_main);
            g_connman->RemoveAskFor(winner.GetHash(), MSG_MASTERNODE_WINNER);
        }

        ProcessMNWinner(winner, pfrom, state);
        return state.IsValid();
    }

    return true;
}

bool CMasternodePayments::ProcessMNWinner(CMasternodePaymentWinner& winner, CNode* pfrom, CValidationState& state)
{
    int nHeight = mnodeman.GetBestHeight();

    if (mapMasternodePayeeVotes.count(winner.GetHash())) {
        LogPrint(BCLog::MASTERNODE, "mnw - Already seen - %s bestHeight %d\n", winner.GetHash().ToString().c_str(), nHeight);
        masternodeSync.AddedMasternodeWinner(winner.GetHash());
        return false;
    }

    int nFirstBlock = nHeight - (mnodeman.CountEnabled() * 1.25);
    if (winner.nBlockHeight < nFirstBlock || winner.nBlockHeight > nHeight + 20) {
        LogPrint(BCLog::MASTERNODE, "mnw - winner out of range - FirstBlock %d Height %d bestHeight %d\n", nFirstBlock, winner.nBlockHeight, nHeight);
        return state.Error("block height out of range");
    }

    // reject old signature version
    if (winner.nMessVersion != MessageVersion::MESS_VER_HASH) {
        LogPrint(BCLog::MASTERNODE, "mnw - rejecting old message version %d\n", winner.nMessVersion);
        return state.Error("mnw old message version");
    }

    // See if the mnw signer exists, and whether it's a legacy or DMN masternode
    const CMasternode* pmn{nullptr};
    auto dmn = deterministicMNManager->GetListAtChainTip().GetMNByCollateral(winner.vinMasternode.prevout);
    if (dmn == nullptr) {
        // legacy masternode
        pmn = mnodeman.Find(winner.vinMasternode.prevout);
        if (pmn == nullptr) {
            // it could be a non-synced masternode. ask for the mnb
            LogPrint(BCLog::MASTERNODE, "mnw - unknown masternode %s\n", winner.vinMasternode.prevout.hash.ToString());
            // Only ask for missing items after the initial syncing process is complete
            //   otherwise will think a full sync succeeded when they return a result
            if (pfrom && masternodeSync.IsSynced()) mnodeman.AskForMN(pfrom, winner.vinMasternode);
            return state.Error("Non-existent mnwinner voter");
        }
    }
    // either deterministic or legacy. not both
    assert((dmn && !pmn) || (!dmn && pmn));

    // See if the masternode is in the quorum (top-MNPAYMENTS_SIGNATURES_TOTAL)
    if (!winner.IsValid(pfrom, state, nHeight)) {
        // error cause set internally
        return false;
    }

    // See if this masternode has already voted for this block height
    if (!CanVote(winner.vinMasternode.prevout, winner.nBlockHeight)) {
        return state.Error("MN already voted");
    }

    // Check signature
    bool is_valid_sig = dmn ? winner.CheckSignature(dmn->pdmnState->pubKeyOperator.Get())
                            : winner.CheckSignature(pmn->pubKeyMasternode.GetID());

    if (!is_valid_sig) {
        LogPrint(BCLog::MASTERNODE, "%s : mnw - invalid signature for %s masternode: %s\n",
                __func__, (dmn ? "deterministic" : "legacy"), winner.vinMasternode.prevout.hash.ToString());
        return state.DoS(20, false, REJECT_INVALID, "invalid voter mnwinner signature");
    }

    // Record vote
    RecordWinnerVote(winner.vinMasternode.prevout, winner.nBlockHeight);

    // Add winner
    AddWinningMasternode(winner);

    // Relay only if we are synchronized.
    // Makes no sense to relay MNWinners to the peers from where we are syncing them.
    if (masternodeSync.IsSynced()) winner.Relay();
    masternodeSync.AddedMasternodeWinner(winner.GetHash());

    // valid
    return true;
}

bool CMasternodePayments::GetBlockPayee(int nBlockHeight, CScript& payee) const
{
    const auto it = mapMasternodeBlocks.find(nBlockHeight);
    if (it != mapMasternodeBlocks.end()) {
        return it->second.GetPayee(payee);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return false;
}

// Is this masternode scheduled to get paid soon?
// -- Only look ahead up to 8 blocks to allow for propagation of the latest 2 winners
<<<<<<< HEAD
bool CMasternodePayments::IsScheduled(CMasternode& mn, int nNotBlockHeight)
{
    LOCK(cs_mapMasternodeBlocks);

    int nHeight;
    {
        TRY_LOCK(cs_main, locked);
        if (!locked || chainActive.Tip() == NULL) return false;
        nHeight = chainActive.Tip()->nHeight;
    }

    std::vector<unsigned char> mnpayee;
    mnpayee = mn.vin.masternodeStealthAddress;

    std::vector<unsigned char> payee;
=======
bool CMasternodePayments::IsScheduled(const CMasternode& mn, int nNotBlockHeight)
{
    LOCK(cs_mapMasternodeBlocks);

    int nHeight = mnodeman.GetBestHeight();

    const CScript& mnpayee = mn.GetPayeeScript();
    CScript payee;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (int64_t h = nHeight; h <= nHeight + 8; h++) {
        if (h == nNotBlockHeight) continue;
        if (mapMasternodeBlocks.count(h)) {
            if (mapMasternodeBlocks[h].GetPayee(payee)) {
                if (mnpayee == payee) {
                    return true;
                }
            }
        }
    }

    return false;
}

<<<<<<< HEAD
bool CMasternodePayments::AddWinningMasternode(CMasternodePaymentWinner& winnerIn)
{
    uint256 blockHash = 0;
    if (!GetBlockHash(blockHash, winnerIn.nBlockHeight - 100)) {
        return false;
    }

    {
        LOCK2(cs_mapMasternodePayeeVotes, cs_mapMasternodeBlocks);

        if (mapMasternodePayeeVotes.count(winnerIn.GetHash())) {
            return false;
        }

=======
void CMasternodePayments::AddWinningMasternode(CMasternodePaymentWinner& winnerIn)
{
    {
        LOCK2(cs_mapMasternodePayeeVotes, cs_mapMasternodeBlocks);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        mapMasternodePayeeVotes[winnerIn.GetHash()] = winnerIn;

        if (!mapMasternodeBlocks.count(winnerIn.nBlockHeight)) {
            CMasternodeBlockPayees blockPayees(winnerIn.nBlockHeight);
            mapMasternodeBlocks[winnerIn.nBlockHeight] = blockPayees;
        }
    }

<<<<<<< HEAD
    mapMasternodeBlocks[winnerIn.nBlockHeight].AddPayee(1, winnerIn.vinMasternode.masternodeStealthAddress);

    return true;
=======
    CTxDestination addr;
    ExtractDestination(winnerIn.payee, addr);
    LogPrint(BCLog::MASTERNODE, "mnw - Adding winner %s for block %d\n", EncodeDestination(addr), winnerIn.nBlockHeight);
    mapMasternodeBlocks[winnerIn.nBlockHeight].AddPayee(winnerIn.payee, 1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool CMasternodeBlockPayees::IsTransactionValid(const CTransaction& txNew)
{
<<<<<<< HEAD
    LOCK2(cs_main, cs_vecPayments);

    int nMaxSignatures = 0;
    int nMasternode_Drift_Count = 0;

    std::string strPayeesPossible = "";
    nMasternode_Drift_Count = mnodeman.size() + Params().MasternodeCountDrift();
    CBlockIndex* pindexPrev = chainActive.Tip();
    CAmount requiredMasternodePayment = GetMasternodePayment(nBlockHeight, GetBlockValue(pindexPrev->nHeight));

    //require at least 6 signatures
=======
    LOCK(cs_vecPayments);

    //require at least 6 signatures
    int nMaxSignatures = 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (CMasternodePayee& payee : vecPayments)
        if (payee.nVotes >= nMaxSignatures && payee.nVotes >= MNPAYMENTS_SIGNATURES_REQUIRED)
            nMaxSignatures = payee.nVotes;

    // if we don't have at least 6 signatures on a payee, approve whichever is the longest chain
    if (nMaxSignatures < MNPAYMENTS_SIGNATURES_REQUIRED) return true;

<<<<<<< HEAD
    for (CMasternodePayee& payee : vecPayments) {
        bool found = false;
        for (CTxOut out : txNew.vout) {
            if (payee.masternodeStealthAddress == out.masternodeStealthAddress) {
                if (out.nValue >= requiredMasternodePayment)
                    found = true;
                else
                    LogPrint("masternode", "Masternode payment is out of drift range. Paid=%s Min=%s\n", FormatMoney(out.nValue).c_str(), FormatMoney(requiredMasternodePayment).c_str());
=======
    std::string strPayeesPossible = "";
    CAmount requiredMasternodePayment = GetMasternodePayment();

    for (CMasternodePayee& payee : vecPayments) {
        bool found = false;
        for (CTxOut out : txNew.vout) {
            if (payee.scriptPubKey == out.scriptPubKey) {
                if(out.nValue == requiredMasternodePayment)
                    found = true;
                else
                    LogPrintf("%s : Masternode payment value (%s) different from required value (%s).\n",
                            __func__, FormatMoney(out.nValue).c_str(), FormatMoney(requiredMasternodePayment).c_str());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            }
        }

        if (payee.nVotes >= MNPAYMENTS_SIGNATURES_REQUIRED) {
            if (found) return true;

<<<<<<< HEAD
            std::string address2(payee.masternodeStealthAddress.begin(), payee.masternodeStealthAddress.end());

            if (strPayeesPossible == "") {
                strPayeesPossible += address2;
            } else {
                strPayeesPossible += "," + address2;
            }
        }
    }
    LogPrint("masternode", "CMasternodePayments::IsTransactionValid - Missing required payment of %s to %s\n", FormatMoney(requiredMasternodePayment).c_str(), strPayeesPossible.c_str());
=======
            CTxDestination address1;
            ExtractDestination(payee.scriptPubKey, address1);

            if (strPayeesPossible != "")
                strPayeesPossible += ",";

            strPayeesPossible += EncodeDestination(address1);
        }
    }

    LogPrint(BCLog::MASTERNODE,"CMasternodePayments::IsTransactionValid - Missing required payment of %s to %s\n", FormatMoney(requiredMasternodePayment).c_str(), strPayeesPossible.c_str());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return false;
}

std::string CMasternodeBlockPayees::GetRequiredPaymentsString()
{
    LOCK(cs_vecPayments);

<<<<<<< HEAD
    std::string ret = "Unknown";

    for (CMasternodePayee& payee : vecPayments) {
        //CTxDestination address1;
        std::string paymentAddress(payee.masternodeStealthAddress.begin(), payee.masternodeStealthAddress.end());

        if (ret != "Unknown") {
            ret += ", " + paymentAddress + ":" + std::to_string(payee.nVotes);
        } else {
            ret = paymentAddress + ":" + std::to_string(payee.nVotes);
        }
    }

    return ret;
=======
    std::string ret = "";

    for (CMasternodePayee& payee : vecPayments) {
        CTxDestination address1;
        ExtractDestination(payee.scriptPubKey, address1);
        if (ret != "") {
            ret += ", ";
        }
        ret += EncodeDestination(address1) + ":" + std::to_string(payee.nVotes);
    }

    return ret.empty() ? "Unknown" : ret;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

std::string CMasternodePayments::GetRequiredPaymentsString(int nBlockHeight)
{
    LOCK(cs_mapMasternodeBlocks);

    if (mapMasternodeBlocks.count(nBlockHeight)) {
        return mapMasternodeBlocks[nBlockHeight].GetRequiredPaymentsString();
    }

    return "Unknown";
}

<<<<<<< HEAD
bool CMasternodePayments::IsTransactionValid(const CTransaction& txNew, int nBlockHeight)
{
=======
bool CMasternodePayments::IsTransactionValid(const CTransaction& txNew, const CBlockIndex* pindexPrev)
{
    const int nBlockHeight = pindexPrev->nHeight + 1;
    if (deterministicMNManager->LegacyMNObsolete(nBlockHeight)) {
        std::vector<CTxOut> vecMnOuts;
        if (!GetMasternodeTxOuts(pindexPrev, vecMnOuts)) {
            // No masternode scheduled to be paid.
            return true;
        }

        for (const CTxOut& o : vecMnOuts) {
            if (std::find(txNew.vout.begin(), txNew.vout.end(), o) == txNew.vout.end()) {
                CTxDestination mnDest;
                const std::string& payee = ExtractDestination(o.scriptPubKey, mnDest) ? EncodeDestination(mnDest)
                                                                                      : HexStr(o.scriptPubKey);
                LogPrint(BCLog::MASTERNODE, "%s: Failed to find expected payee %s in block at height %d (tx %s)",
                                            __func__, payee, pindexPrev->nHeight + 1, txNew.GetHash().ToString());
                return false;
            }
        }
        // all the expected payees have been found in txNew outputs
        return true;
    }

    // Legacy payment logic. !TODO: remove when transition to DMN is complete
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    LOCK(cs_mapMasternodeBlocks);

    if (mapMasternodeBlocks.count(nBlockHeight)) {
        return mapMasternodeBlocks[nBlockHeight].IsTransactionValid(txNew);
    }

    return true;
}

<<<<<<< HEAD
void CMasternodePayments::CleanPaymentList()
{
    LOCK2(cs_mapMasternodePayeeVotes, cs_mapMasternodeBlocks);

    int nHeight;
    {
        TRY_LOCK(cs_main, locked);
        if (!locked || chainActive.Tip() == NULL) return;
        nHeight = chainActive.Tip()->nHeight;
    }

    //keep up to five cycles for historical sake
    int nLimit = std::max(int(mnodeman.size() * 1.25), 1000);
=======
void CMasternodePayments::CleanPaymentList(int mnCount, int nHeight)
{
    LOCK2(cs_mapMasternodePayeeVotes, cs_mapMasternodeBlocks);

    //keep up to five cycles for historical sake
    int nLimit = std::max(int(mnCount * 1.25), 1000);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    std::map<uint256, CMasternodePaymentWinner>::iterator it = mapMasternodePayeeVotes.begin();
    while (it != mapMasternodePayeeVotes.end()) {
        CMasternodePaymentWinner winner = (*it).second;

        if (nHeight - winner.nBlockHeight > nLimit) {
<<<<<<< HEAD
            LogPrint("mnpayments", "CMasternodePayments::CleanPaymentList - Removing old Masternode payment - block %d\n", winner.nBlockHeight);
=======
            LogPrint(BCLog::MASTERNODE, "CMasternodePayments::CleanPaymentList - Removing old Masternode payment - block %d\n", winner.nBlockHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            masternodeSync.mapSeenSyncMNW.erase((*it).first);
            mapMasternodePayeeVotes.erase(it++);
            mapMasternodeBlocks.erase(winner.nBlockHeight);
        } else {
            ++it;
        }
    }
}

<<<<<<< HEAD
bool CMasternodePaymentWinner::IsValid(CNode* pnode, std::string& strError)
{
    CMasternode* pmn = mnodeman.Find(vinMasternode);

    if (!pmn) {
        strError = strprintf("Unknown Masternode %s", vinMasternode.prevout.hash.ToString());
        LogPrint("masternode", "CMasternodePaymentWinner::IsValid - %s\n", strError);
        mnodeman.AskForMN(pnode, vinMasternode);
        return false;
    }

    if (pmn->protocolVersion < ActiveProtocol()) {
        strError = strprintf("Masternode protocol too old %d - req %d", pmn->protocolVersion, ActiveProtocol());
        LogPrint("masternode", "CMasternodePaymentWinner::IsValid - %s\n", strError);
        return false;
    }

    int n = mnodeman.GetMasternodeRank(vinMasternode, nBlockHeight - 100, ActiveProtocol());

    if (n > MNPAYMENTS_SIGNATURES_TOTAL) {
        //It's common to have masternodes mistakenly think they are in the top 10
        // We don't want to print all of these messages, or punish them unless they're way off
        if (n > MNPAYMENTS_SIGNATURES_TOTAL * 2) {
            strError = strprintf("Masternode not in the top %d (%d)", MNPAYMENTS_SIGNATURES_TOTAL * 2, n);
            LogPrint("masternode", "CMasternodePaymentWinner::IsValid - %s\n", strError);
        }
        return false;
    }

    return true;
}

bool CMasternodePayments::ProcessBlock(int nBlockHeight)
{
    if (!fMasterNode) return false;

    //reference node - hybrid mode

    int n = mnodeman.GetMasternodeRank(activeMasternode.vin, nBlockHeight - 100, ActiveProtocol());

    if (n == -1) {
        LogPrint("mnpayments", "CMasternodePayments::ProcessBlock - Unknown Masternode\n");
        return false;
    }

    if (n > MNPAYMENTS_SIGNATURES_TOTAL) {
        LogPrint("mnpayments", "CMasternodePayments::ProcessBlock - Masternode not in the top %d (%d)\n", MNPAYMENTS_SIGNATURES_TOTAL, n);
        return false;
    }

    if (nBlockHeight <= nLastBlockHeight) return false;

    CMasternodePaymentWinner newWinner(activeMasternode.vin);

    if (budget.IsBudgetPaymentBlock(nBlockHeight)) {
        //is budget payment block -- handled by the budgeting software
    } else {
        LogPrint("masternode", "CMasternodePayments::ProcessBlock() Start nHeight %d - vin %s. \n", nBlockHeight, activeMasternode.vin.prevout.hash.ToString());

        // pay to the oldest MN that still had no payment but its input is old enough and it was active long enough
        int nCount = 0;
        CMasternode* pmn = mnodeman.GetNextMasternodeInQueueForPayment(nBlockHeight, true, nCount);

        if (pmn != NULL) {
            LogPrint("masternode", "CMasternodePayments::ProcessBlock() Found by FindOldestNotInVec \n");
            newWinner.nBlockHeight = nBlockHeight;

            CScript payee = GetScriptForDestination(pmn->pubKeyCollateralAddress);
            newWinner.AddPayee(pmn->vin.masternodeStealthAddress);

            CTxDestination address1;
            ExtractDestination(payee, address1);
            CBitcoinAddress address2(address1);
            LogPrint("masternode", "CMasternodePayments::ProcessBlock() Winner payee %s nHeight %d. \n", address2.ToString().c_str(), newWinner.nBlockHeight);
        } else {
            LogPrint("masternode", "CMasternodePayments::ProcessBlock() Failed to find masternode to pay\n");
        }
    }

    std::string errorMessage;
    CPubKey pubKeyMasternode;
    CKey keyMasternode;

    if (!obfuScationSigner.SetKey(strMasterNodePrivKey, errorMessage, keyMasternode, pubKeyMasternode)) {
        LogPrint("masternode", "CMasternodePayments::ProcessBlock() - Error upon calling SetKey: %s\n", errorMessage.c_str());
        return false;
    }

    if (newWinner.Sign(keyMasternode, pubKeyMasternode)) {
        if (AddWinningMasternode(newWinner)) {
            newWinner.Relay();
            nLastBlockHeight = nBlockHeight;
            return true;
        }
    }

    return false;
}

void CMasternodePaymentWinner::Relay()
{
    CInv inv(MSG_MASTERNODE_WINNER, GetHash());
    RelayInv(inv);
}

bool CMasternodePaymentWinner::SignatureValid()
{
    CMasternode* pmn = mnodeman.Find(vinMasternode);

    if (pmn != NULL) {
        HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << vinMasternode.prevout.GetHash() << nBlockHeight << payee;
        std::string strMessage = HEX_STR(ser);
        std::string errorMessage = "";
        if (!obfuScationSigner.VerifyMessage(pmn->pubKeyMasternode, vchSig, strMessage, errorMessage)) {
            return error("CMasternodePaymentWinner::SignatureValid() - Got bad Masternode address signature %s\n", vinMasternode.prevout.hash.ToString());
        }

        return true;
    }

    return false;
=======
void CMasternodePayments::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload)
{
    if (masternodeSync.RequestedMasternodeAssets > MASTERNODE_SYNC_LIST) {
        ProcessBlock(pindexNew->nHeight + 10);
    }
}

void CMasternodePayments::ProcessBlock(int nBlockHeight)
{
    LogPrintf("%s: Processing block %d\n", __func__, nBlockHeight);

    // No more mnw messages after transition to DMN
    if (deterministicMNManager->LegacyMNObsolete()) {
        return;
    }
    if (!fMasterNode) return;

    // Get the active masternode (operator) key
    CTxIn mnVin;
    Optional<CKey> mnKey{nullopt};
    CBLSSecretKey blsKey;
    if (!GetActiveMasternodeKeys(mnVin, mnKey, blsKey)) {
        return;
    }

    //reference node - hybrid mode
    int n = mnodeman.GetMasternodeRank(mnVin, nBlockHeight - 100);

    if (n == -1) {
        LogPrintf("%s: ERROR: active masternode is not registered yet\n", __func__);
        return;
    }

    if (n > MNPAYMENTS_SIGNATURES_TOTAL) {
        LogPrintf("%s: active masternode not in the top %d (%d)\n", __func__, MNPAYMENTS_SIGNATURES_TOTAL, n);
        return;
    }

    if (nBlockHeight <= nLastBlockHeight) return;

    if (g_budgetman.IsBudgetPaymentBlock(nBlockHeight)) {
        //is budget payment block -- handled by the budgeting software
        return;
    }

    // check winner height
    if (nBlockHeight - 100 > mnodeman.GetBestHeight() + 1) {
        LogPrintf("%s: mnw - invalid height %d > %d", __func__, nBlockHeight - 100, mnodeman.GetBestHeight() + 1);
        return;
    }

    // pay to the oldest MN that still had no payment but its input is old enough and it was active long enough
    int nCount = 0;
    MasternodeRef pmn = mnodeman.GetNextMasternodeInQueueForPayment(nBlockHeight, true, nCount);

    if (pmn == nullptr) {
        LogPrintf("%s: Failed to find masternode to pay\n", __func__);
        return;
    }

    CMasternodePaymentWinner newWinner(mnVin, nBlockHeight);
    newWinner.AddPayee(pmn->GetPayeeScript());
    if (mnKey != nullopt) {
        // Legacy MN
        if (!newWinner.Sign(*mnKey, mnKey->GetPubKey().GetID())) {
            LogPrintf("%s: Failed to sign masternode winner\n", __func__);
            return;
        }
    } else {
        // DMN
        if (!newWinner.Sign(blsKey)) {
            LogPrintf("%s: Failed to sign masternode winner with DMN\n", __func__);
            return;
        }
    }

    AddWinningMasternode(newWinner);
    newWinner.Relay();
    LogPrintf("%s: Relayed winner %s\n", __func__, newWinner.GetHash().ToString());
    nLastBlockHeight = nBlockHeight;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void CMasternodePayments::Sync(CNode* node, int nCountNeeded)
{
    LOCK(cs_mapMasternodePayeeVotes);

<<<<<<< HEAD
    int nHeight;
    {
        TRY_LOCK(cs_main, locked);
        if (!locked || chainActive.Tip() == NULL) return;
        nHeight = chainActive.Tip()->nHeight;
    }

=======
    int nHeight = mnodeman.GetBestHeight();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int nCount = (mnodeman.CountEnabled() * 1.25);
    if (nCountNeeded > nCount) nCountNeeded = nCount;

    int nInvCount = 0;
    std::map<uint256, CMasternodePaymentWinner>::iterator it = mapMasternodePayeeVotes.begin();
    while (it != mapMasternodePayeeVotes.end()) {
        CMasternodePaymentWinner winner = (*it).second;
        if (winner.nBlockHeight >= nHeight - nCountNeeded && winner.nBlockHeight <= nHeight + 20) {
            node->PushInventory(CInv(MSG_MASTERNODE_WINNER, winner.GetHash()));
            nInvCount++;
        }
        ++it;
    }
<<<<<<< HEAD
    node->PushMessage("ssc", MASTERNODE_SYNC_MNW, nInvCount);
=======
    g_connman->PushMessage(node, CNetMsgMaker(node->GetSendVersion()).Make(NetMsgType::SYNCSTATUSCOUNT, MASTERNODE_SYNC_MNW, nInvCount));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

std::string CMasternodePayments::ToString() const
{
    std::ostringstream info;

    info << "Votes: " << (int)mapMasternodePayeeVotes.size() << ", Blocks: " << (int)mapMasternodeBlocks.size();

    return info.str();
}

<<<<<<< HEAD

int CMasternodePayments::GetOldestBlock()
{
    LOCK(cs_mapMasternodeBlocks);

    int nOldestBlock = std::numeric_limits<int>::max();

    std::map<int, CMasternodeBlockPayees>::iterator it = mapMasternodeBlocks.begin();
    while (it != mapMasternodeBlocks.end()) {
        if ((*it).first < nOldestBlock) {
            nOldestBlock = (*it).first;
        }
        it++;
    }

    return nOldestBlock;
}


int CMasternodePayments::GetNewestBlock()
{
    LOCK(cs_mapMasternodeBlocks);

    int nNewestBlock = 0;

    std::map<int, CMasternodeBlockPayees>::iterator it = mapMasternodeBlocks.begin();
    while (it != mapMasternodeBlocks.end()) {
        if ((*it).first > nNewestBlock) {
            nNewestBlock = (*it).first;
        }
        it++;
    }

    return nNewestBlock;
=======
bool CMasternodePayments::CanVote(const COutPoint& outMasternode, int nBlockHeight) const
{
    LOCK(cs_mapMasternodePayeeVotes);
    const auto it = mapMasternodesLastVote.find(outMasternode);
    return it == mapMasternodesLastVote.end() || it->second != nBlockHeight;
}

void CMasternodePayments::RecordWinnerVote(const COutPoint& outMasternode, int nBlockHeight)
{
    LOCK(cs_mapMasternodePayeeVotes);
    mapMasternodesLastVote[outMasternode] = nBlockHeight;
}

bool IsCoinbaseValueValid(const CTransactionRef& tx, CAmount nBudgetAmt, CValidationState& _state)
{
    assert(tx->IsCoinBase());
    if (masternodeSync.IsSynced()) {
        const CAmount nCBaseOutAmt = tx->GetValueOut();
        if (nBudgetAmt > 0) {
            // Superblock
            if (nCBaseOutAmt != nBudgetAmt) {
                const std::string strError = strprintf("%s: invalid coinbase payment for budget (%s vs expected=%s)",
                                                       __func__, FormatMoney(nCBaseOutAmt), FormatMoney(nBudgetAmt));
                return _state.DoS(100, error(strError.c_str()), REJECT_INVALID, "bad-superblock-cb-amt");
            }
            return true;
        } else {
            // regular block
            CAmount nMnAmt = GetMasternodePayment();
            // if enforcement is disabled, there could be no masternode payment
            bool sporkEnforced = sporkManager.IsSporkActive(SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT);
            const std::string strError = strprintf("%s: invalid coinbase payment for masternode (%s vs expected=%s)",
                                                   __func__, FormatMoney(nCBaseOutAmt), FormatMoney(nMnAmt));
            if (sporkEnforced && nCBaseOutAmt != nMnAmt) {
                return _state.DoS(100, error(strError.c_str()), REJECT_INVALID, "bad-cb-amt");
            }
            if (!sporkEnforced && nCBaseOutAmt > nMnAmt) {
                return _state.DoS(100, error(strError.c_str()), REJECT_INVALID, "bad-cb-amt-spork8-disabled");
            }
            return true;
        }
    }
    return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
