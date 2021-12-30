// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef MASTERNODE_H
#define MASTERNODE_H

<<<<<<< HEAD
#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "timedata.h"
#include "util.h"

#define MASTERNODE_MIN_CONFIRMATIONS 15
#define MASTERNODE_MIN_MNP_SECONDS (10 * 60)
#define MASTERNODE_MIN_MNB_SECONDS (5 * 60)
#define MASTERNODE_PING_SECONDS (5 * 60)
#define MASTERNODE_EXPIRATION_SECONDS (120 * 60)
#define MASTERNODE_REMOVAL_SECONDS (130 * 60)
#define MASTERNODE_CHECK_SECONDS 5

using namespace std;
=======
#include "key_io.h"
#include "key.h"
#include "messagesigner.h"
#include "net.h"
#include "primitives/transaction.h"
#include "serialize.h"
#include "sync.h"
#include "timedata.h"
#include "util/system.h"

/* Depth of the block pinged by masternodes */
static const unsigned int MNPING_DEPTH = 12;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CMasternode;
class CMasternodeBroadcast;
class CMasternodePing;
<<<<<<< HEAD
extern map<int64_t, uint256> mapCacheBlockHashes;

bool GetBlockHash(uint256& hash, int nBlockHeight);

=======

typedef std::shared_ptr<CMasternode> MasternodeRef;

class CDeterministicMN;
typedef std::shared_ptr<const CDeterministicMN> CDeterministicMNCPtr;

int MasternodeMinPingSeconds();
int MasternodeBroadcastSeconds();
int MasternodeCollateralMinConf();
int MasternodePingSeconds();
int MasternodeExpirationSeconds();
int MasternodeRemovalSeconds();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

//
// The Masternode Ping Class : Contains a different serialize method for sending pings from masternodes throughout the network
//

<<<<<<< HEAD
class CMasternodePing
=======
class CMasternodePing : public CSignedMessage
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
public:
    CTxIn vin;
    uint256 blockHash;
    int64_t sigTime; //mnb message times
<<<<<<< HEAD
    std::vector<unsigned char> vchSig;
    //removed stop

    CMasternodePing();
    CMasternodePing(CTxIn& newVin);

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vin);
        READWRITE(blockHash);
        READWRITE(sigTime);
        READWRITE(vchSig);
    }

    bool CheckAndUpdate(int& nDos, bool fRequireEnabled = true, bool fCheckSigTimeOnly = false);
    bool Sign(CKey& keyMasternode, CPubKey& pubKeyMasternode);
    bool VerifySignature(CPubKey& pubKeyMasternode, int &nDos);
    void Relay();

    uint256 GetHash()
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << vin;
        ss << sigTime;
        return ss.GetHash();
    }

    void swap(CMasternodePing& first, CMasternodePing& second) // nothrow
    {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;

        // by swapping the members of two classes,
        // the two classes are effectively swapped
        swap(first.vin, second.vin);
        swap(first.blockHash, second.blockHash);
        swap(first.sigTime, second.sigTime);
        swap(first.vchSig, second.vchSig);
    }

    CMasternodePing& operator=(CMasternodePing from)
    {
        swap(*this, from);
        return *this;
    }
=======

    CMasternodePing();
    CMasternodePing(const CTxIn& newVin, const uint256& nBlockHash, uint64_t _sigTime);

    SERIALIZE_METHODS(CMasternodePing, obj) { READWRITE(obj.vin, obj.blockHash, obj.sigTime, obj.vchSig, obj.nMessVersion); }

    uint256 GetHash() const;

    // override CSignedMessage functions
    uint256 GetSignatureHash() const override { return GetHash(); }
    std::string GetStrMessage() const override;
    const CTxIn GetVin() const { return vin; };
    bool IsNull() const { return blockHash.IsNull() || vin.prevout.IsNull(); }

    bool CheckAndUpdate(int& nDos, bool fRequireAvailable = true, bool fCheckSigTimeOnly = false);
    void Relay();

    CMasternodePing& operator=(const CMasternodePing& other) = default;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    friend bool operator==(const CMasternodePing& a, const CMasternodePing& b)
    {
        return a.vin == b.vin && a.blockHash == b.blockHash;
    }
    friend bool operator!=(const CMasternodePing& a, const CMasternodePing& b)
    {
        return !(a == b);
    }
};

//
<<<<<<< HEAD
// The Masternode Class. For managing the Obfuscation process. It contains the input of the 5000 PRCY, signature to prove
// it's the one who own that ip address and code for calculating the payment election.
//
class CMasternode
=======
// The Masternode Class. It contains the input of the 10000 PIV, signature to prove
// it's the one who own that ip address and code for calculating the payment election.
//
class CMasternode : public CSignedMessage
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
private:
    // critical section to protect the inner data structures
    mutable RecursiveMutex cs;
<<<<<<< HEAD
    int64_t lastTimeChecked;
=======
    bool fCollateralSpent{false};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

public:
    enum state {
        MASTERNODE_PRE_ENABLED,
        MASTERNODE_ENABLED,
        MASTERNODE_EXPIRED,
<<<<<<< HEAD
        MASTERNODE_OUTPOINT_SPENT,
        MASTERNODE_REMOVE,
        MASTERNODE_WATCHDOG_EXPIRED,
        MASTERNODE_POSE_BAN,
        MASTERNODE_VIN_SPENT,
        MASTERNODE_POS_ERROR
=======
        MASTERNODE_REMOVE,
        MASTERNODE_VIN_SPENT,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    };

    CTxIn vin;
    CService addr;
    CPubKey pubKeyCollateralAddress;
    CPubKey pubKeyMasternode;
<<<<<<< HEAD
    CPubKey pubKeyCollateralAddress1;
    CPubKey pubKeyMasternode1;
    std::vector<unsigned char> sig;
    int activeState;
    int64_t sigTime; //mnb message time
    int cacheInputAge;
    int cacheInputAgeBlock;
    bool unitTest;
    bool allowFreeTx;
    int protocolVersion;
    int nActiveState;
    int64_t nLastDsq; //the dsq count from the last dsq broadcast of this node
=======
    int64_t sigTime; //mnb message time
    int protocolVersion;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    int nScanningErrorCount;
    int nLastScanningErrorBlockHeight;
    CMasternodePing lastPing;

<<<<<<< HEAD
    int64_t nLastDsee;  // temporary, do not save. Remove after migration to v12
    int64_t nLastDseep; // temporary, do not save. Remove after migration to v12

    CMasternode();
    CMasternode(const CMasternode& other);
    CMasternode(const CMasternodeBroadcast& mnb);


    void swap(CMasternode& first, CMasternode& second) // nothrow
    {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;

        // by swapping the members of two classes,
        // the two classes are effectively swapped
        swap(first.vin, second.vin);
        swap(first.addr, second.addr);
        swap(first.pubKeyCollateralAddress, second.pubKeyCollateralAddress);
        swap(first.pubKeyMasternode, second.pubKeyMasternode);
        swap(first.sig, second.sig);
        swap(first.activeState, second.activeState);
        swap(first.sigTime, second.sigTime);
        swap(first.lastPing, second.lastPing);
        swap(first.cacheInputAge, second.cacheInputAge);
        swap(first.cacheInputAgeBlock, second.cacheInputAgeBlock);
        swap(first.unitTest, second.unitTest);
        swap(first.allowFreeTx, second.allowFreeTx);
        swap(first.protocolVersion, second.protocolVersion);
        swap(first.nLastDsq, second.nLastDsq);
        swap(first.nScanningErrorCount, second.nScanningErrorCount);
        swap(first.nLastScanningErrorBlockHeight, second.nLastScanningErrorBlockHeight);
    }

    CMasternode& operator=(CMasternode from)
    {
        swap(*this, from);
        return *this;
    }
=======
    explicit CMasternode();
    CMasternode(const CMasternode& other);

    // Initialize from DMN. Used by the compatibility code.
    CMasternode(const CDeterministicMNCPtr& dmn, int64_t registeredTime, const uint256& registeredHash);

    // override CSignedMessage functions
    uint256 GetSignatureHash() const override;
    std::string GetStrMessage() const override;
    const CTxIn GetVin() const { return vin; };
    CPubKey GetPubKey() const { return pubKeyMasternode; }

    void SetLastPing(const CMasternodePing& _lastPing) { WITH_LOCK(cs, lastPing = _lastPing;); }

    CMasternode& operator=(const CMasternode& other)
    {
        nMessVersion = other.nMessVersion;
        vchSig = other.vchSig;
        vin = other.vin;
        addr = other.addr;
        pubKeyCollateralAddress = other.pubKeyCollateralAddress;
        pubKeyMasternode = other.pubKeyMasternode;
        sigTime = other.sigTime;
        lastPing = other.lastPing;
        protocolVersion = other.protocolVersion;
        nScanningErrorCount = other.nScanningErrorCount;
        nLastScanningErrorBlockHeight = other.nLastScanningErrorBlockHeight;
        return *this;
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    friend bool operator==(const CMasternode& a, const CMasternode& b)
    {
        return a.vin == b.vin;
    }
    friend bool operator!=(const CMasternode& a, const CMasternode& b)
    {
        return !(a.vin == b.vin);
    }

<<<<<<< HEAD
    uint256 CalculateScore(int mod = 1, int64_t nBlockHeight = 0);

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        LOCK(cs);

        READWRITE(vin);
        READWRITE(addr);
        READWRITE(pubKeyCollateralAddress);
        READWRITE(pubKeyMasternode);
        READWRITE(sig);
        READWRITE(sigTime);
        READWRITE(protocolVersion);
        READWRITE(activeState);
        READWRITE(lastPing);
        READWRITE(cacheInputAge);
        READWRITE(cacheInputAgeBlock);
        READWRITE(unitTest);
        READWRITE(allowFreeTx);
        READWRITE(nLastDsq);
        READWRITE(nScanningErrorCount);
        READWRITE(nLastScanningErrorBlockHeight);
    }

    int64_t SecondsSincePayment();

    bool UpdateFromNewBroadcast(CMasternodeBroadcast& mnb);

    void Check(bool forceCheck = false);
=======
    arith_uint256 CalculateScore(const uint256& hash) const;

    SERIALIZE_METHODS(CMasternode, obj)
    {
        LOCK(obj.cs);
        READWRITE(obj.vin, obj.addr, obj.pubKeyCollateralAddress);
        READWRITE(obj.pubKeyMasternode, obj.vchSig, obj.sigTime, obj.protocolVersion);
        READWRITE(obj.lastPing, obj.nScanningErrorCount, obj.nLastScanningErrorBlockHeight);

        if (obj.protocolVersion == MIN_BIP155_PROTOCOL_VERSION) {
            bool dummyIsBIP155Addr = false;
            READWRITE(dummyIsBIP155Addr);
        }
    }

    template <typename Stream>
    CMasternode(deserialize_type, Stream& s) {
        Unserialize(s);
    }

    bool UpdateFromNewBroadcast(CMasternodeBroadcast& mnb);

    CMasternode::state GetActiveState() const;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    bool IsBroadcastedWithin(int seconds)
    {
        return (GetAdjustedTime() - sigTime) < seconds;
    }

<<<<<<< HEAD
    bool IsPingedWithin(int seconds, int64_t now = -1)
    {
        now == -1 ? now = GetAdjustedTime() : now;

        return (lastPing == CMasternodePing()) ? false : now - lastPing.sigTime < seconds;
=======
    bool IsPingedWithin(int seconds, int64_t now = -1) const
    {
        now == -1 ? now = GetAdjustedTime() : now;
        return lastPing.IsNull() ? false : now - lastPing.sigTime < seconds;
    }

    void SetSpent()
    {
        LOCK(cs);
        fCollateralSpent = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    void Disable()
    {
<<<<<<< HEAD
=======
        LOCK(cs);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        sigTime = 0;
        lastPing = CMasternodePing();
    }

<<<<<<< HEAD
    bool IsEnabled()
    {
        return activeState == MASTERNODE_ENABLED;
    }

    int GetMasternodeInputAge()
    {
        if (chainActive.Tip() == NULL) return 0;

        if (cacheInputAge == 0) {
            cacheInputAge = GetInputAge(vin);
            cacheInputAgeBlock = chainActive.Tip()->nHeight;
        }

        return cacheInputAge + (chainActive.Tip()->nHeight - cacheInputAgeBlock);
    }

    std::string GetStatus();

    std::string Status()
    {
        std::string strStatus = "ACTIVE";

        if (activeState == CMasternode::MASTERNODE_ENABLED) strStatus = "ENABLED";
        if (activeState == CMasternode::MASTERNODE_EXPIRED) strStatus = "EXPIRED";
        if (activeState == CMasternode::MASTERNODE_VIN_SPENT) strStatus = "VIN_SPENT";
        if (activeState == CMasternode::MASTERNODE_REMOVE) strStatus = "REMOVE";
        if (activeState == CMasternode::MASTERNODE_POS_ERROR) strStatus = "POS_ERROR";

        return strStatus;
    }

    int64_t GetLastPaid();
    bool IsValidNetAddr();
=======
    bool IsEnabled() const
    {
        return GetActiveState() == MASTERNODE_ENABLED;
    }

    bool IsPreEnabled() const
    {
        return GetActiveState() == MASTERNODE_PRE_ENABLED;
    }

    bool IsAvailableState() const
    {
        state s = GetActiveState();
        return s == MASTERNODE_ENABLED || s == MASTERNODE_PRE_ENABLED;
    }

    std::string Status() const
    {
        auto activeState = GetActiveState();
        if (activeState == CMasternode::MASTERNODE_PRE_ENABLED) return "PRE_ENABLED";
        if (activeState == CMasternode::MASTERNODE_ENABLED)     return "ENABLED";
        if (activeState == CMasternode::MASTERNODE_EXPIRED)     return "EXPIRED";
        if (activeState == CMasternode::MASTERNODE_VIN_SPENT)   return "VIN_SPENT";
        if (activeState == CMasternode::MASTERNODE_REMOVE)      return "REMOVE";
        return strprintf("INVALID_%d", activeState);
    }

    bool IsValidNetAddr() const;

    /*
     * This is used only by the compatibility code for DMN, which don't share the public key (but the keyid).
     * Used by the payment-logic to include the necessary information in a temporary MasternodeRef object
     * (which is not indexed in the maps of the legacy manager).
     * A non-empty mnPayeeScript identifies this object as a "deterministic" masternode.
     * Note: this is the single payout for the masternode (if the dmn is configured to pay a portion of the reward
     * to the operator, this is done only after the disabling of the legacy system).
     */
    CScript mnPayeeScript{};
    CScript GetPayeeScript() const {
        return mnPayeeScript.empty() ? GetScriptForDestination(pubKeyCollateralAddress.GetID())
                                     : mnPayeeScript;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};


//
// The Masternode Broadcast Class : Contains a different serialize method for sending masternodes through the network
//

class CMasternodeBroadcast : public CMasternode
{
public:
    CMasternodeBroadcast();
<<<<<<< HEAD
    CMasternodeBroadcast(CService newAddr, CTxIn newVin, CPubKey newPubkey, CPubKey newPubkey2, int protocolVersionIn);
    CMasternodeBroadcast(const CMasternode& mn);

    bool CheckAndUpdate(int& nDoS);
    bool CheckInputsAndAdd(int& nDos);
    bool Sign(CKey& keyCollateralAddress);
    bool VerifySignature();
    void Relay();
    std::string GetOldStrMessage();
    std::string GetNewStrMessage();

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vin);
        READWRITE(addr);
        READWRITE(pubKeyCollateralAddress);
        READWRITE(pubKeyMasternode);
        READWRITE(sig);
        READWRITE(sigTime);
        READWRITE(protocolVersion);
        READWRITE(lastPing);
        READWRITE(nLastDsq);
    }

    uint256 GetHash()
    {
        CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
        ss << sigTime;
        ss << pubKeyCollateralAddress;
        return ss.GetHash();
    }

    /// Create Masternode broadcast, needs to be relayed manually after that
    static bool Create(CTxIn vin, CService service, CKey keyCollateralAddressNew, CPubKey pubKeyCollateralAddressNew, CKey keyMasternodeNew, CPubKey pubKeyMasternodeNew, std::string& strErrorRet, CMasternodeBroadcast& mnbRet);
    static bool Create(std::string strService, std::string strKey, std::string strTxHash, std::string strOutputIndex, std::string& strErrorRet, CMasternodeBroadcast& mnbRet, bool fOffline = false);
    static bool CheckDefaultPort(std::string strService, std::string& strErrorRet, std::string strContext);
};

=======
    CMasternodeBroadcast(CService newAddr, CTxIn newVin, CPubKey newPubkey, CPubKey newPubkey2, int protocolVersionIn, const CMasternodePing& _lastPing);
    CMasternodeBroadcast(const CMasternode& mn);

    bool CheckAndUpdate(int& nDoS);

    uint256 GetHash() const;

    void Relay();

    // special sign/verify
    bool Sign(const CKey& key, const CPubKey& pubKey);
    bool CheckSignature() const;

    SERIALIZE_METHODS(CMasternodeBroadcast, obj)
    {
        READWRITE(obj.vin);
        READWRITE(obj.addr);
        READWRITE(obj.pubKeyCollateralAddress);
        READWRITE(obj.pubKeyMasternode);
        READWRITE(obj.vchSig);
        READWRITE(obj.sigTime);
        READWRITE(obj.protocolVersion);
        READWRITE(obj.lastPing);
        READWRITE(obj.nMessVersion);
    }

    /// Create Masternode broadcast, needs to be relayed manually after that
    static bool Create(const CTxIn& vin, const CService& service, const CKey& keyCollateralAddressNew, const CPubKey& pubKeyCollateralAddressNew, const CKey& keyMasternodeNew, const CPubKey& pubKeyMasternodeNew, std::string& strErrorRet, CMasternodeBroadcast& mnbRet);
    static bool Create(const std::string& strService, const std::string& strKey, const std::string& strTxHash, const std::string& strOutputIndex, std::string& strErrorRet, CMasternodeBroadcast& mnbRet, bool fOffline, int chainHeight);
    static bool CheckDefaultPort(CService service, std::string& strErrorRet, const std::string& strContext);
};

// Temporary function used for payment compatibility code.
// Returns a shared pointer to a masternode object initialized from a DMN.
MasternodeRef MakeMasternodeRefForDMN(const CDeterministicMNCPtr& dmn);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif
