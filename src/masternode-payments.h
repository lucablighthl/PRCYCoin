// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODE_PAYMENTS_H
#define MASTERNODE_PAYMENTS_H

#include "key.h"
<<<<<<< HEAD
#include "main.h"
#include "masternode.h"

using namespace std;
=======
#include "masternode.h"
#include "validationinterface.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

extern RecursiveMutex cs_vecPayments;
extern RecursiveMutex cs_mapMasternodeBlocks;
extern RecursiveMutex cs_mapMasternodePayeeVotes;

class CMasternodePayments;
class CMasternodePaymentWinner;
class CMasternodeBlockPayees;
<<<<<<< HEAD
=======
class CValidationState;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

extern CMasternodePayments masternodePayments;

#define MNPAYMENTS_SIGNATURES_REQUIRED 6
#define MNPAYMENTS_SIGNATURES_TOTAL 10

<<<<<<< HEAD
void ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
bool IsBlockPayeeValid(const CBlock& block, int nBlockHeight);
std::string GetRequiredPaymentsString(int nBlockHeight);
bool IsBlockValueValid(const CBlock& block, CAmount nExpectedValue, CAmount nMinted);
bool FillBlockPayee(CMutableTransaction& txNew, CAmount nFees, bool fProofOfStake);
=======
bool IsBlockPayeeValid(const CBlock& block, const CBlockIndex* pindexPrev);
std::string GetRequiredPaymentsString(int nBlockHeight);
bool IsBlockValueValid(int nHeight, CAmount& nExpectedValue, CAmount nMinted, CAmount& nBudgetAmt);
void FillBlockPayee(CMutableTransaction& txCoinbase, CMutableTransaction& txCoinstake, const CBlockIndex* pindexPrev, bool fProofOfStake);

/**
 * Check coinbase output value for blocks after v6.0 enforcement.
 * It must pay the masternode for regular blocks and a proposal during superblocks.
 */
bool IsCoinbaseValueValid(const CTransactionRef& tx, CAmount nBudgetAmt, CValidationState& _state);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

void DumpMasternodePayments();

/** Save Masternode Payment Data (mnpayments.dat)
 */
class CMasternodePaymentDB
{
private:
<<<<<<< HEAD
    boost::filesystem::path pathDB;
=======
    fs::path pathDB;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    std::string strMagicMessage;

public:
    enum ReadResult {
        Ok,
        FileError,
        HashReadError,
        IncorrectHash,
        IncorrectMagicMessage,
        IncorrectMagicNumber,
        IncorrectFormat
    };

    CMasternodePaymentDB();
    bool Write(const CMasternodePayments& objToSave);
<<<<<<< HEAD
    ReadResult Read(CMasternodePayments& objToLoad, bool fDryRun = false);
=======
    ReadResult Read(CMasternodePayments& objToLoad);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

class CMasternodePayee
{
public:
<<<<<<< HEAD
    int nVotes;
    std::vector<unsigned char> masternodeStealthAddress;

    CMasternodePayee()
    {
        nVotes = 0;
    }

    CMasternodePayee(int nVotesIn, std::vector<unsigned char> masternodeStealthAddress)
    {
        nVotes = nVotesIn;
        this->masternodeStealthAddress = masternodeStealthAddress;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nVotes);
        READWRITE(masternodeStealthAddress);
    }
=======
    CScript scriptPubKey;
    int nVotes;

    CMasternodePayee()
    {
        scriptPubKey = CScript();
        nVotes = 0;
    }

    CMasternodePayee(CScript payee, int nVotesIn)
    {
        scriptPubKey = payee;
        nVotes = nVotesIn;
    }

    SERIALIZE_METHODS(CMasternodePayee, obj) { READWRITE(obj.scriptPubKey, obj.nVotes); }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

// Keep track of votes for payees from masternodes
class CMasternodeBlockPayees
{
public:
    int nBlockHeight;
    std::vector<CMasternodePayee> vecPayments;

    CMasternodeBlockPayees()
    {
        nBlockHeight = 0;
        vecPayments.clear();
    }
    CMasternodeBlockPayees(int nBlockHeightIn)
    {
        nBlockHeight = nBlockHeightIn;
        vecPayments.clear();
    }

<<<<<<< HEAD
    void AddPayee(int nIncrement, std::vector<unsigned char> masternodeStealthAddress)
=======
    void AddPayee(const CScript& payeeIn, int nIncrement)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_vecPayments);

        for (CMasternodePayee& payee : vecPayments) {
<<<<<<< HEAD
            if (payee.masternodeStealthAddress == masternodeStealthAddress) {
=======
            if (payee.scriptPubKey == payeeIn) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                payee.nVotes += nIncrement;
                return;
            }
        }

<<<<<<< HEAD
        CMasternodePayee c(nIncrement, masternodeStealthAddress);
        vecPayments.push_back(c);
    }

    bool GetPayee(std::vector<unsigned char>& payee)
=======
        CMasternodePayee c(payeeIn, nIncrement);
        vecPayments.push_back(c);
    }

    bool GetPayee(CScript& payee) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_vecPayments);

        int nVotes = -1;
<<<<<<< HEAD
        for (CMasternodePayee& p : vecPayments) {
            if (p.nVotes > nVotes) {
                payee = p.masternodeStealthAddress;
=======
        for (const CMasternodePayee& p : vecPayments) {
            if (p.nVotes > nVotes) {
                payee = p.scriptPubKey;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                nVotes = p.nVotes;
            }
        }

        return (nVotes > -1);
    }

<<<<<<< HEAD
    bool HasPayeeWithVotes(std::vector<unsigned char> payee, int nVotesReq)
=======
    bool HasPayeeWithVotes(const CScript& payee, int nVotesReq)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs_vecPayments);

        for (CMasternodePayee& p : vecPayments) {
<<<<<<< HEAD
            if (p.nVotes >= nVotesReq && p.masternodeStealthAddress == payee) return true;
=======
            if (p.nVotes >= nVotesReq && p.scriptPubKey == payee) return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        return false;
    }

    bool IsTransactionValid(const CTransaction& txNew);
    std::string GetRequiredPaymentsString();

<<<<<<< HEAD
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nBlockHeight);
        READWRITE(vecPayments);
    }
};

// for storing the winning payments
class CMasternodePaymentWinner
{
public:
    CTxIn vinMasternode;

    int nBlockHeight;
    std::vector<unsigned char> payee;//masternode stealth public address
    std::vector<unsigned char> vchSig;

    CMasternodePaymentWinner()
    {
        nBlockHeight = 0;
        vinMasternode = CTxIn();
    }

    CMasternodePaymentWinner(CTxIn vinIn)
    {
        nBlockHeight = 0;
        vinMasternode = vinIn;
        payee = vinIn.masternodeStealthAddress;
    }

    uint256 GetHash()
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << payee;
        ss << nBlockHeight;
        ss << vinMasternode.prevout;

        return ss.GetHash();
    }

    bool Sign(CKey& keyMasternode, CPubKey& pubKeyMasternode);
    bool IsValid(CNode* pnode, std::string& strError);
    bool SignatureValid();
    void Relay();

    void AddPayee(std::vector<unsigned char> payeeIn)
=======
    SERIALIZE_METHODS(CMasternodeBlockPayees, obj) { READWRITE(obj.nBlockHeight, obj.vecPayments); }
};

// for storing the winning payments
class CMasternodePaymentWinner : public CSignedMessage
{
public:
    CTxIn vinMasternode;
    int nBlockHeight;
    CScript payee;

    CMasternodePaymentWinner() :
        CSignedMessage(),
        vinMasternode(),
        nBlockHeight(0),
        payee()
    {}

    CMasternodePaymentWinner(const CTxIn& vinIn, int nHeight):
        CSignedMessage(),
        vinMasternode(vinIn),
        nBlockHeight(nHeight),
        payee()
    {}

    uint256 GetHash() const;

    // override CSignedMessage functions
    uint256 GetSignatureHash() const override { return GetHash(); }
    std::string GetStrMessage() const override;
    CTxIn GetVin() const { return vinMasternode; };

    bool IsValid(CNode* pnode, CValidationState& state, int chainHeight);
    void Relay();

    void AddPayee(const CScript& payeeIn)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        payee = payeeIn;
    }

<<<<<<< HEAD

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vinMasternode);
        READWRITE(nBlockHeight);
        READWRITE(payee);
        READWRITE(vchSig);
    }

    std::string ToString()
    {
        std::string s(payee.begin(), payee.end());
        std::string ret = "";
        ret += vinMasternode.ToString();
        ret += ", " + std::to_string(nBlockHeight);
        ret += ", " + s;
=======
    SERIALIZE_METHODS(CMasternodePaymentWinner, obj) { READWRITE(obj.vinMasternode, obj.nBlockHeight, obj.payee, obj.vchSig, obj.nMessVersion); }

    std::string ToString()
    {
        std::string ret = "";
        ret += vinMasternode.ToString();
        ret += ", " + std::to_string(nBlockHeight);
        ret += ", " + HexStr(payee);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ret += ", " + std::to_string((int)vchSig.size());
        return ret;
    }
};

//
// Masternode Payments Class
// Keeps track of who should get paid for which blocks
//

<<<<<<< HEAD
class CMasternodePayments
{
private:
    int nSyncedFromPeer;
=======
class CMasternodePayments : public CValidationInterface
{
private:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int nLastBlockHeight;

public:
    std::map<uint256, CMasternodePaymentWinner> mapMasternodePayeeVotes;
    std::map<int, CMasternodeBlockPayees> mapMasternodeBlocks;
<<<<<<< HEAD
    std::map<uint256, int> mapMasternodesLastVote; //prevout.hash + prevout.n, nBlockHeight

    CMasternodePayments()
    {
        nSyncedFromPeer = 0;
=======

    CMasternodePayments()
    {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        nLastBlockHeight = 0;
    }

    void Clear()
    {
        LOCK2(cs_mapMasternodeBlocks, cs_mapMasternodePayeeVotes);
        mapMasternodeBlocks.clear();
        mapMasternodePayeeVotes.clear();
    }

<<<<<<< HEAD
    bool AddWinningMasternode(CMasternodePaymentWinner& winner);
    bool ProcessBlock(int nBlockHeight);

    void Sync(CNode* node, int nCountNeeded);
    void CleanPaymentList();
    int LastPayment(CMasternode& mn);

    bool GetBlockPayee(int nBlockHeight, std::vector<unsigned char>& payee);
    bool IsTransactionValid(const CTransaction& txNew, int nBlockHeight);
    bool IsScheduled(CMasternode& mn, int nNotBlockHeight);

    bool CanVote(COutPoint outMasternode, int nBlockHeight)
    {
        LOCK(cs_mapMasternodePayeeVotes);

        if (mapMasternodesLastVote.count(outMasternode.hash + outMasternode.n)) {
            if (mapMasternodesLastVote[outMasternode.hash + outMasternode.n] == nBlockHeight) {
                return false;
            }
        }

        //record this masternode voted
        mapMasternodesLastVote[outMasternode.hash + outMasternode.n] = nBlockHeight;
        return true;
    }

    int GetMinMasternodePaymentsProto();
    void ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
    std::string GetRequiredPaymentsString(int nBlockHeight);
    bool FillBlockPayee(CMutableTransaction& txNew, int64_t nFees, bool fProofOfStake);
    std::string ToString() const;
    int GetOldestBlock();
    int GetNewestBlock();

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(mapMasternodePayeeVotes);
        READWRITE(mapMasternodeBlocks);
    }
=======
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;

    void AddWinningMasternode(CMasternodePaymentWinner& winner);
    void ProcessBlock(int nBlockHeight);

    void Sync(CNode* node, int nCountNeeded);
    void CleanPaymentList(int mnCount, int nHeight);

    // get the masternode payment outs for block built on top of pindexPrev
    bool GetMasternodeTxOuts(const CBlockIndex* pindexPrev, std::vector<CTxOut>& voutMasternodePaymentsRet) const;

    // can be removed after transition to DMN
    bool GetLegacyMasternodeTxOut(int nHeight, std::vector<CTxOut>& voutMasternodePaymentsRet) const;
    bool GetBlockPayee(int nBlockHeight, CScript& payee) const;

    bool IsTransactionValid(const CTransaction& txNew, const CBlockIndex* pindexPrev);
    bool IsScheduled(const CMasternode& mn, int nNotBlockHeight);

    bool ProcessMNWinner(CMasternodePaymentWinner& winner, CNode* pfrom, CValidationState& state);
    bool ProcessMessageMasternodePayments(CNode* pfrom, std::string& strCommand, CDataStream& vRecv, CValidationState& state);
    std::string GetRequiredPaymentsString(int nBlockHeight);
    void FillBlockPayee(CMutableTransaction& txCoinbase, CMutableTransaction& txCoinstake, const CBlockIndex* pindexPrev, bool fProofOfStake) const;
    std::string ToString() const;

    SERIALIZE_METHODS(CMasternodePayments, obj) { READWRITE(obj.mapMasternodePayeeVotes, obj.mapMasternodeBlocks); }

private:
    // keep track of last voted height for mnw signers
    std::map<COutPoint, int> mapMasternodesLastVote; //prevout, nBlockHeight

    bool CanVote(const COutPoint& outMasternode, int nBlockHeight) const;
    void RecordWinnerVote(const COutPoint& outMasternode, int nBlockHeight);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};


#endif
