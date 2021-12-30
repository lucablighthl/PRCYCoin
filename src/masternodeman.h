// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODEMAN_H
#define MASTERNODEMAN_H

<<<<<<< HEAD
#include "base58.h"
#include "key.h"
#include "main.h"
#include "masternode.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#define MASTERNODES_DUMP_SECONDS (15 * 60)
#define MASTERNODES_DSEG_SECONDS (3 * 60 * 60)

using namespace std;

class CMasternodeMan;

extern CMasternodeMan mnodeman;
=======
#include "activemasternode.h"
#include "cyclingvector.h"
#include "key.h"
#include "key_io.h"
#include "masternode.h"
#include "net.h"
#include "sync.h"
#include "util/system.h"

#define MASTERNODES_REQUEST_SECONDS (60 * 60) // One hour.

/** Maximum number of block hashes to cache */
static const unsigned int CACHED_BLOCK_HASHES = 200;

class CMasternodeMan;
class CActiveMasternode;

extern CMasternodeMan mnodeman;
extern CActiveMasternode activeMasternode;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void DumpMasternodes();

/** Access to the MN database (mncache.dat)
 */
class CMasternodeDB
{
private:
<<<<<<< HEAD
    boost::filesystem::path pathMN;
=======
    fs::path pathMN;
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

    CMasternodeDB();
    bool Write(const CMasternodeMan& mnodemanToSave);
<<<<<<< HEAD
    ReadResult Read(CMasternodeMan& mnodemanToLoad, bool fDryRun = false);
};

=======
    ReadResult Read(CMasternodeMan& mnodemanToLoad);
};


>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class CMasternodeMan
{
private:
    // critical section to protect the inner data structures
    mutable RecursiveMutex cs;

    // critical section to protect the inner data structures specifically on messaging
    mutable RecursiveMutex cs_process_message;

<<<<<<< HEAD
    // map to hold all MNs
    std::vector<CMasternode> vMasternodes;
=======
    // map to hold all MNs (indexed by collateral outpoint)
    std::map<COutPoint, MasternodeRef> mapMasternodes;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // who's asked for the Masternode list and the last time
    std::map<CNetAddr, int64_t> mAskedUsForMasternodeList;
    // who we asked for the Masternode list and the last time
    std::map<CNetAddr, int64_t> mWeAskedForMasternodeList;
    // which Masternodes we've asked for
    std::map<COutPoint, int64_t> mWeAskedForMasternodeListEntry;

<<<<<<< HEAD
public:
    // Keep track of all broadcasts I've seen
    map<uint256, CMasternodeBroadcast> mapSeenMasternodeBroadcast;
    // Keep track of all pings I've seen
    map<uint256, CMasternodePing> mapSeenMasternodePing;

    // keep track of dsq count to prevent masternodes from gaming obfuscation queue
    int64_t nDsqCount;

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        LOCK(cs);
        READWRITE(vMasternodes);
        READWRITE(mAskedUsForMasternodeList);
        READWRITE(mWeAskedForMasternodeList);
        READWRITE(mWeAskedForMasternodeListEntry);
        READWRITE(nDsqCount);

        READWRITE(mapSeenMasternodeBroadcast);
        READWRITE(mapSeenMasternodePing);
    }

    CMasternodeMan();
    CMasternodeMan(CMasternodeMan& other);
=======
    // Memory Only. Updated in NewBlock (blocks arrive in order)
    std::atomic<int> nBestHeight;

    // Memory Only. Cache last block hashes. Used to verify mn pings and winners.
    CyclingVector<uint256> cvLastBlockHashes;

    // Return the banning score (0 if no ban score increase is needed).
    int ProcessMNBroadcast(CNode* pfrom, CMasternodeBroadcast& mnb);
    int ProcessMNPing(CNode* pfrom, CMasternodePing& mnp);
    int ProcessMessageInner(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);

    // Relay a MN
    void BroadcastInvMN(CMasternode* mn, CNode* pfrom);

    // Validation
    bool CheckInputs(CMasternodeBroadcast& mnb, int nChainHeight, int& nDoS);

public:
    // Keep track of all broadcasts I've seen
    std::map<uint256, CMasternodeBroadcast> mapSeenMasternodeBroadcast;
    // Keep track of all pings I've seen
    std::map<uint256, CMasternodePing> mapSeenMasternodePing;

    // keep track of dsq count to prevent masternodes from gaming obfuscation queue
    // TODO: Remove this from serialization
    int64_t nDsqCount;

    SERIALIZE_METHODS(CMasternodeMan, obj)
    {
        LOCK(obj.cs);
        READWRITE(obj.mapMasternodes);
        READWRITE(obj.mAskedUsForMasternodeList);
        READWRITE(obj.mWeAskedForMasternodeList);
        READWRITE(obj.mWeAskedForMasternodeListEntry);
        READWRITE(obj.nDsqCount);

        READWRITE(obj.mapSeenMasternodeBroadcast);
        READWRITE(obj.mapSeenMasternodePing);
    }

    CMasternodeMan();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /// Add an entry
    bool Add(CMasternode& mn);

    /// Ask (source) node for mnb
<<<<<<< HEAD
    void AskForMN(CNode* pnode, CTxIn& vin);

    /// Check all Masternodes
    void Check();

    /// Check all Masternodes and remove inactive
    void CheckAndRemove(bool forceExpiredRemoval = false);
=======
    void AskForMN(CNode* pnode, const CTxIn& vin);

    /// Check all Masternodes and remove inactive. Return the total masternode count.
    int CheckAndRemove(bool forceExpiredRemoval = false);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /// Clear Masternode vector
    void Clear();

<<<<<<< HEAD
    int CountEnabled(int protocolVersion = -1);

    void CountNetworks(int protocolVersion, int& ipv4, int& ipv6, int& onion);

    void DsegUpdate(CNode* pnode);

    /// Find an entry
    CMasternode* Find(const CScript& payee);
    CMasternode* Find(const CTxIn& vin);
    CMasternode* Find(const CPubKey& pubKeyMasternode);

    /// Find an entry in the masternode list that is next to be paid
    CMasternode* GetNextMasternodeInQueueForPayment(int nBlockHeight, bool fFilterSigTime, int& nCount);

    /// Find a random entry
    CMasternode* FindRandomNotInVec(std::vector<CTxIn>& vecToExclude, int protocolVersion = -1);

    /// Get the current winner for this block
    CMasternode* GetCurrentMasterNode(int mod = 1, int64_t nBlockHeight = 0, int minProtocol = 0);

    std::vector<CMasternode> GetFullMasternodeVector()
    {
        Check();
        return vMasternodes;
    }

    std::vector<pair<int, CMasternode> > GetMasternodeRanks(int64_t nBlockHeight, int minProtocol = 0);
    int GetMasternodeRank(const CTxIn& vin, int64_t nBlockHeight, int minProtocol = 0, bool fOnlyActive = true);
    CMasternode* GetMasternodeByRank(int nRank, int64_t nBlockHeight, int minProtocol = 0, bool fOnlyActive = true);

    void ProcessMasternodeConnections();

    void ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);

    /// Return the number of (unique) Masternodes
    int size() { return vMasternodes.size(); }

    /// Return the number of Masternodes older than (default) 8000 seconds
    int stable_size ();

    std::string ToString() const;

    void Remove(CTxIn vin);

    /// Update masternode list and maps using provided CMasternodeBroadcast
    void UpdateMasternodeList(CMasternodeBroadcast mnb);
};

=======
    void SetBestHeight(int height) { nBestHeight.store(height, std::memory_order_release); };
    int GetBestHeight() const { return nBestHeight.load(std::memory_order_acquire); }

    int CountEnabled(bool only_legacy = false) const;

    bool RequestMnList(CNode* pnode);

    /// Find an entry
    CMasternode* Find(const COutPoint& collateralOut);
    const CMasternode* Find(const COutPoint& collateralOut) const;
    CMasternode* Find(const CPubKey& pubKeyMasternode);

    /// Check all transactions in a block, for spent masternode collateral outpoints (marking them as spent)
    void CheckSpentCollaterals(const std::vector<CTransactionRef>& vtx);

    /// Find an entry in the masternode list that is next to be paid
    MasternodeRef GetNextMasternodeInQueueForPayment(int nBlockHeight, bool fFilterSigTime, int& nCount, const CBlockIndex* pChainTip = nullptr) const;

    /// Get the winner for this block hash
    MasternodeRef GetCurrentMasterNode(const uint256& hash) const;

    /// vector of pairs <masternode winner, height>
    std::vector<std::pair<MasternodeRef, int>> GetMnScores(int nLast) const;

    // Retrieve the known masternodes ordered by scoring without checking them. (Only used for listmasternodes RPC call)
    std::vector<std::pair<int64_t, MasternodeRef>> GetMasternodeRanks(int nBlockHeight) const;
    int GetMasternodeRank(const CTxIn& vin, int64_t nBlockHeight) const;

    bool ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv, int& dosScore);

    // Process GETMNLIST message, returning the banning score (if 0, no ban score increase is needed)
    int ProcessGetMNList(CNode* pfrom, CTxIn& vin);

    struct MNsInfo {
        // All the known MNs
        int total{0};
        // enabled MNs eligible for payments. Older than 8000 seconds.
        int stableSize{0};
        // MNs enabled.
        int enabledSize{0};

        // Networks
        int ipv4{0};
        int ipv6{0};
        int onion{0};
    };

    // Return an overall status of the MNs list
    CMasternodeMan::MNsInfo getMNsInfo() const;

    std::string ToString() const;

    void Remove(const COutPoint& collateralOut);

    /// Update masternode list and maps using provided CMasternodeBroadcast
    void UpdateMasternodeList(CMasternodeBroadcast& mnb);

    /// Get the time a masternode was last paid
    int64_t GetLastPaid(const MasternodeRef& mn, int count_enabled, const CBlockIndex* BlockReading) const;
    int64_t SecondsSincePayment(const MasternodeRef& mn, int count_enabled, const CBlockIndex* BlockReading) const;

    // Block hashes cycling vector management
    void CacheBlockHash(const CBlockIndex* pindex);
    void UncacheBlockHash(const CBlockIndex* pindex);
    uint256 GetHashAtHeight(int nHeight) const;
    bool IsWithinDepth(const uint256& nHash, int depth) const;
    uint256 GetBlockHashToPing() const { return GetHashAtHeight(GetBestHeight() - MNPING_DEPTH); }
    std::vector<uint256> GetCachedBlocks() const { return cvLastBlockHashes.GetCache(); }
};

void ThreadCheckMasternodes();

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif
