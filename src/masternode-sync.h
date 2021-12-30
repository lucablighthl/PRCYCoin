// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Copyright (c) 2015-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifndef MASTERNODE_SYNC_H
#define MASTERNODE_SYNC_H

<<<<<<< HEAD
#define MASTERNODE_SYNC_INITIAL 0
#define MASTERNODE_SYNC_LIST 1
#define MASTERNODE_SYNC_MNW 2
#define MASTERNODE_SYNC_BUDGET 3
=======
#include "net.h"    // for NodeId
#include "uint256.h"

#include <atomic>
#include <string>
#include <map>

#define MASTERNODE_SYNC_INITIAL 0
#define MASTERNODE_SYNC_SPORKS 1
#define MASTERNODE_SYNC_LIST 2
#define MASTERNODE_SYNC_MNW 3
#define MASTERNODE_SYNC_BUDGET 4
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#define MASTERNODE_SYNC_BUDGET_PROP 10
#define MASTERNODE_SYNC_BUDGET_FIN 11
#define MASTERNODE_SYNC_FAILED 998
#define MASTERNODE_SYNC_FINISHED 999

#define MASTERNODE_SYNC_TIMEOUT 5
#define MASTERNODE_SYNC_THRESHOLD 2

class CMasternodeSync;
extern CMasternodeSync masternodeSync;

<<<<<<< HEAD
=======
struct TierTwoPeerData {
    // map of message --> last request timestamp, bool hasResponseArrived.
    std::map<const char*, std::pair<int64_t, bool>> mapMsgData;
};

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
//
// CMasternodeSync : Sync masternode assets in stages
//

class CMasternodeSync
{
public:
    std::map<uint256, int> mapSeenSyncMNB;
    std::map<uint256, int> mapSeenSyncMNW;
    std::map<uint256, int> mapSeenSyncBudget;

    int64_t lastMasternodeList;
    int64_t lastMasternodeWinner;
    int64_t lastBudgetItem;
    int64_t lastFailure;
    int nCountFailures;

    std::atomic<int64_t> lastProcess;
    std::atomic<bool> fBlockchainSynced;

    // sum of all counts
    int sumMasternodeList;
    int sumMasternodeWinner;
    int sumBudgetItemProp;
    int sumBudgetItemFin;
    // peers that reported counts
    int countMasternodeList;
    int countMasternodeWinner;
    int countBudgetItemProp;
    int countBudgetItemFin;

    // Count peers we've requested the list from
    int RequestedMasternodeAssets;
    int RequestedMasternodeAttempt;

    // Time when current masternode asset sync started
    int64_t nAssetSyncStarted;

    CMasternodeSync();

<<<<<<< HEAD
    void AddedMasternodeList(uint256 hash);
    void AddedMasternodeWinner(uint256 hash);
    void AddedBudgetItem(uint256 hash);
    void GetNextAsset();
    std::string GetSyncStatus();
    void ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
=======
    void AddedMasternodeList(const uint256& hash);
    void AddedMasternodeWinner(const uint256& hash);
    void AddedBudgetItem(const uint256& hash);
    void SwitchToNextAsset();
    std::string GetSyncStatus();
    void ProcessSyncStatusMsg(int nItemID, int itemCount);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool IsBudgetFinEmpty();
    bool IsBudgetPropEmpty();

    void Reset();
    void Process();
<<<<<<< HEAD
    bool IsSynced();
    bool IsBlockchainSynced();
    bool IsMasternodeListSynced() { return RequestedMasternodeAssets > MASTERNODE_SYNC_LIST; }
    void ClearFulfilledRequest();
=======
    /*
     * Process sync with a single node.
     * If it returns false, the Process() step is complete.
     * Otherwise Process() calls it again for a different node.
     */
    bool SyncWithNode(CNode* pnode, bool fLegacyMnObsolete);
    bool IsSynced();
    bool NotCompleted();
    bool IsSporkListSynced();
    bool IsMasternodeListSynced();
    bool IsBlockchainSynced();
    void ClearFulfilledRequest();

    bool IsBlockchainSyncedReadOnly() const;

    // Sync message dispatcher
    bool MessageDispatcher(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);

private:

    // Tier two sync node state
    // map of nodeID --> TierTwoPeerData
    std::map<NodeId, TierTwoPeerData> peersSyncState;
    static int GetNextAsset(int currentAsset);

    void SyncRegtest(CNode* pnode);

    template <typename... Args>
    void RequestDataTo(CNode* pnode, const char* msg, bool forceRequest, Args&&... args);

    template <typename... Args>
    void PushMessage(CNode* pnode, const char* msg, Args&&... args);

    // update peer sync state data
    bool UpdatePeerSyncState(const NodeId& id, const char* msg, const int nextSyncStatus);

    // Check if an update is needed
    void CheckAndUpdateSyncStatus();

    // Mark sync timeout
    void syncTimeout(const std::string& reason);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif
