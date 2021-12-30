// Copyright (c) 2014-2016 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ACTIVEMASTERNODE_H
#define ACTIVEMASTERNODE_H

<<<<<<< HEAD
#include "init.h"
#include "key.h"
#include "masternode.h"
#include "net.h"
#include "obfuscation.h"
#include "sync.h"
#include "wallet/wallet.h"

#define ACTIVE_MASTERNODE_INITIAL 0 // initial state
#define ACTIVE_MASTERNODE_SYNC_IN_PROCESS 1
#define ACTIVE_MASTERNODE_INPUT_TOO_NEW 2
#define ACTIVE_MASTERNODE_NOT_CAPABLE 3
#define ACTIVE_MASTERNODE_STARTED 4

// Responsible for activating the Masternode and pinging the network
class CActiveMasternode
{
private:
    // critical section to protect the inner data structures
    mutable RecursiveMutex cs;

    /// Ping Masternode
    bool SendMasternodePing(std::string& errorMessage);

    /// Create Masternode broadcast, needs to be relayed manually after that
    bool CreateBroadcast(CTxIn vin, CService service, CKey key, CPubKey pubKey, CKey keyMasternode, CPubKey pubKeyMasternode, std::string& errorMessage, CMasternodeBroadcast &mnb);

    /// Get 5000 PRCY input that can be used for the Masternode
    bool GetMasterNodeVin(CTxIn& vin, CPubKey& pubkey, CKey& secretKey, std::string strTxHash, std::string strOutputIndex);
    bool GetVinFromOutput(COutput out, CTxIn& vin, CPubKey& pubkey, CKey& secretKey);

public:
    // Initialized by init.cpp
    // Keys for the main Masternode
    CPubKey pubKeyMasternode;

    // Initialized while registering Masternode
    CTxIn vin;
    CService service;

    int status;
    std::string notCapableReason;

    CActiveMasternode()
    {
        status = ACTIVE_MASTERNODE_INITIAL;
    }

    /// Manage status of main Masternode
    void ManageStatus();
    std::string GetStatus();

    /// Create Masternode broadcast, needs to be relayed manually after that
    bool CreateBroadcast(std::string strService, std::string strKey, std::string strTxHash, std::string strOutputIndex, std::string& errorMessage, CMasternodeBroadcast &mnb, bool fOffline = false);

    /// Get 5000 PRCY input that can be used for the Masternode
    bool GetMasterNodeVin(CTxIn& vin, CPubKey& pubkey, CKey& secretKey);
    std::vector<COutput> SelectCoinsMasternode();

    /// Enable cold wallet mode (run a Masternode with no funds)
    bool EnableHotColdMasterNode(CTxIn& vin, CService& addr);
};

=======
#include "key.h"
#include "evo/deterministicmns.h"
#include "net.h"
#include "operationresult.h"
#include "sync.h"
#include "validationinterface.h"

class CActiveDeterministicMasternodeManager;
class CBLSPublicKey;
class CBLSSecretKey;

#define ACTIVE_MASTERNODE_INITIAL 0 // initial state
#define ACTIVE_MASTERNODE_SYNC_IN_PROCESS 1
#define ACTIVE_MASTERNODE_NOT_CAPABLE 3
#define ACTIVE_MASTERNODE_STARTED 4

extern CActiveDeterministicMasternodeManager* activeMasternodeManager;

struct CActiveMasternodeInfo
{
    // Keys for the active Masternode
    CBLSPublicKey pubKeyOperator;
    CBLSSecretKey keyOperator;
    // Initialized while registering Masternode
    uint256 proTxHash{UINT256_ZERO};
    CService service;
};

class CActiveDeterministicMasternodeManager : public CValidationInterface
{
public:
    enum masternode_state_t {
        MASTERNODE_WAITING_FOR_PROTX,
        MASTERNODE_POSE_BANNED,
        MASTERNODE_REMOVED,
        MASTERNODE_OPERATOR_KEY_CHANGED,
        MASTERNODE_PROTX_IP_CHANGED,
        MASTERNODE_READY,
        MASTERNODE_ERROR,
    };

private:
    masternode_state_t state{MASTERNODE_WAITING_FOR_PROTX};
    std::string strError;
    CActiveMasternodeInfo info;

public:
    virtual ~CActiveDeterministicMasternodeManager() = default;
    virtual void UpdatedBlockTip(const CBlockIndex* pindexNew, const CBlockIndex* pindexFork, bool fInitialDownload);

    void Init(const CBlockIndex* pindexTip);
    void Reset(masternode_state_t _state, const CBlockIndex* pindexTip);
    // Sets the Deterministic Masternode Operator's private/public key
    OperationResult SetOperatorKey(const std::string& strMNOperatorPrivKey);
    // If the active masternode is ready, and the keyID matches with the registered one,
    // return private key, keyID, and pointer to dmn.
    OperationResult GetOperatorKey(CBLSSecretKey& key, CDeterministicMNCPtr& dmn) const;
    void SetNullProTx() { info.proTxHash = UINT256_ZERO; }

    const CActiveMasternodeInfo* GetInfo() const { return &info; }
    masternode_state_t GetState() const { return state; }
    std::string GetStatus() const;
    bool IsReady() const { return state == MASTERNODE_READY; }

    static bool IsValidNetAddr(const CService& addrIn);
};

// Responsible for initializing the masternode
OperationResult initMasternode(const std::string& strMasterNodePrivKey, const std::string& strMasterNodeAddr, bool isFromInit);


// Responsible for activating the Masternode and pinging the network (legacy MN list)
class CActiveMasternode
{
private:
    int status{ACTIVE_MASTERNODE_INITIAL};
    std::string notCapableReason;

public:
    CActiveMasternode() = default;

    // Initialized by init.cpp
    // Keys for the main Masternode
    CPubKey pubKeyMasternode;
    CKey privKeyMasternode;

    // Initialized while registering Masternode
    Optional<CTxIn> vin{nullopt};
    CService service;

    /// Manage status of main Masternode
    void ManageStatus();
    void ResetStatus();
    std::string GetStatusMessage() const;
    int GetStatus() const { return status; }

    /// Ping Masternode
    bool SendMasternodePing(std::string& errorMessage);
    /// Enable cold wallet mode (run a Masternode with no funds)
    bool EnableHotColdMasterNode(CTxIn& vin, CService& addr);

    void GetKeys(CKey& privKeyMasternode, CPubKey& pubKeyMasternode);
};

// Compatibility code: get vin and keys for either legacy or deterministic masternode
bool GetActiveMasternodeKeys(CTxIn& vin, Optional<CKey>& key, CBLSSecretKey& blsKey);
// Get active masternode BLS operator keys for DMN
bool GetActiveDMNKeys(CBLSSecretKey& key, CTxIn& vin);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif
