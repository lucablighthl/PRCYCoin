// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternode.h"
<<<<<<< HEAD
#include "addrman.h"
#include "masternodeman.h"
#include "obfuscation.h"
#include "sync.h"
#include "util.h"
#include "streams.h"

// keep track of the scanning errors I've seen
map<uint256, int> mapSeenMasternodeScanningErrors;
// cache block hashes as we calculate them
std::map<int64_t, uint256> mapCacheBlockHashes;

//Get the last hash that matches the modulus given. Processed in reverse order
bool GetBlockHash(uint256& hash, int nBlockHeight)
{
    if (chainActive.Tip() == NULL) return false;

    if (nBlockHeight == 0)
        nBlockHeight = chainActive.Tip()->nHeight;

    if (mapCacheBlockHashes.count(nBlockHeight)) {
        hash = mapCacheBlockHashes[nBlockHeight];
        return true;
    }

    const CBlockIndex* BlockLastSolved = chainActive.Tip();
    const CBlockIndex* BlockReading = chainActive.Tip();

    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 || chainActive.Tip()->nHeight + 1 < nBlockHeight) return false;

    int nBlocksAgo = 0;
    if (nBlockHeight > 0) nBlocksAgo = (chainActive.Tip()->nHeight + 1) - nBlockHeight;
    assert(nBlocksAgo >= 0);

    int n = 0;
    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (n >= nBlocksAgo) {
            hash = BlockReading->GetBlockHash();
            mapCacheBlockHashes[nBlockHeight] = hash;
            return true;
        }
        n++;

        if (BlockReading->pprev == NULL) {
            assert(BlockReading);
            break;
        }
        BlockReading = BlockReading->pprev;
    }

    return false;
}

CMasternode::CMasternode()
=======

#include "addrman.h"
#include "masternode-sync.h"
#include "masternodeman.h"
#include "netbase.h"
#include "sync.h"
#include "wallet/wallet.h"

#define MASTERNODE_MIN_CONFIRMATIONS_REGTEST 1
#define MASTERNODE_MIN_MNP_SECONDS_REGTEST 90
#define MASTERNODE_MIN_MNB_SECONDS_REGTEST 25
#define MASTERNODE_PING_SECONDS_REGTEST 25
#define MASTERNODE_EXPIRATION_SECONDS_REGTEST 12 * 60
#define MASTERNODE_REMOVAL_SECONDS_REGTEST 13 * 60

#define MASTERNODE_MIN_CONFIRMATIONS 15
#define MASTERNODE_MIN_MNP_SECONDS (10 * 60)
#define MASTERNODE_MIN_MNB_SECONDS (5 * 60)
#define MASTERNODE_PING_SECONDS (5 * 60)
#define MASTERNODE_EXPIRATION_SECONDS (120 * 60)
#define MASTERNODE_REMOVAL_SECONDS (130 * 60)
#define MASTERNODE_CHECK_SECONDS 5

// keep track of the scanning errors I've seen
std::map<uint256, int> mapSeenMasternodeScanningErrors;


int MasternodeMinPingSeconds()
{
    return Params().IsRegTestNet() ? MASTERNODE_MIN_MNP_SECONDS_REGTEST : MASTERNODE_MIN_MNP_SECONDS;
}

int MasternodeBroadcastSeconds()
{
    return Params().IsRegTestNet() ? MASTERNODE_MIN_MNB_SECONDS_REGTEST : MASTERNODE_MIN_MNB_SECONDS;
}

int MasternodeCollateralMinConf()
{
    return Params().IsRegTestNet() ? MASTERNODE_MIN_CONFIRMATIONS_REGTEST : MASTERNODE_MIN_CONFIRMATIONS;
}

int MasternodePingSeconds()
{
    return Params().IsRegTestNet() ? MASTERNODE_PING_SECONDS_REGTEST : MASTERNODE_PING_SECONDS;
}

int MasternodeExpirationSeconds()
{
    return Params().IsRegTestNet() ? MASTERNODE_EXPIRATION_SECONDS_REGTEST : MASTERNODE_EXPIRATION_SECONDS;
}

int MasternodeRemovalSeconds()
{
    return Params().IsRegTestNet() ? MASTERNODE_REMOVAL_SECONDS_REGTEST : MASTERNODE_REMOVAL_SECONDS;
}

// Used for sigTime < maxTimeWindow
int64_t GetMaxTimeWindow()
{
    return GetAdjustedTime() + 60 * 2;
}


CMasternode::CMasternode() :
        CSignedMessage()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    LOCK(cs);
    vin = CTxIn();
    addr = CService();
    pubKeyCollateralAddress = CPubKey();
    pubKeyMasternode = CPubKey();
<<<<<<< HEAD
    sig = std::vector<unsigned char>();
    activeState = MASTERNODE_ENABLED;
    sigTime = GetAdjustedTime();
    lastPing = CMasternodePing();
    cacheInputAge = 0;
    cacheInputAgeBlock = 0;
    unitTest = false;
    allowFreeTx = true;
    nActiveState = MASTERNODE_ENABLED,
    protocolVersion = PROTOCOL_VERSION;
    nLastDsq = 0;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
    lastTimeChecked = 0;
    nLastDsee = 0;  // temporary, do not save. Remove after migration to v12
    nLastDseep = 0; // temporary, do not save. Remove after migration to v12
}

CMasternode::CMasternode(const CMasternode& other)
=======
    sigTime = 0;
    lastPing = CMasternodePing();
    protocolVersion = PROTOCOL_VERSION;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
    mnPayeeScript.clear();
}

CMasternode::CMasternode(const CMasternode& other) :
        CSignedMessage(other)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    LOCK(cs);
    vin = other.vin;
    addr = other.addr;
    pubKeyCollateralAddress = other.pubKeyCollateralAddress;
    pubKeyMasternode = other.pubKeyMasternode;
<<<<<<< HEAD
    sig = other.sig;
    activeState = other.activeState;
    sigTime = other.sigTime;
    lastPing = other.lastPing;
    cacheInputAge = other.cacheInputAge;
    cacheInputAgeBlock = other.cacheInputAgeBlock;
    unitTest = other.unitTest;
    allowFreeTx = other.allowFreeTx;
    nActiveState = MASTERNODE_ENABLED,
    protocolVersion = other.protocolVersion;
    nLastDsq = other.nLastDsq;
    nScanningErrorCount = other.nScanningErrorCount;
    nLastScanningErrorBlockHeight = other.nLastScanningErrorBlockHeight;
    lastTimeChecked = 0;
    nLastDsee = other.nLastDsee;   // temporary, do not save. Remove after migration to v12
    nLastDseep = other.nLastDseep; // temporary, do not save. Remove after migration to v12
}

CMasternode::CMasternode(const CMasternodeBroadcast& mnb)
{
    LOCK(cs);
    vin = mnb.vin;
    addr = mnb.addr;
    pubKeyCollateralAddress = mnb.pubKeyCollateralAddress;
    pubKeyMasternode = mnb.pubKeyMasternode;
    sig = mnb.sig;
    activeState = MASTERNODE_ENABLED;
    sigTime = mnb.sigTime;
    lastPing = mnb.lastPing;
    cacheInputAge = 0;
    cacheInputAgeBlock = 0;
    unitTest = false;
    allowFreeTx = true;
    nActiveState = MASTERNODE_ENABLED,
    protocolVersion = mnb.protocolVersion;
    nLastDsq = mnb.nLastDsq;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
    lastTimeChecked = 0;
    nLastDsee = 0;  // temporary, do not save. Remove after migration to v12
    nLastDseep = 0; // temporary, do not save. Remove after migration to v12
=======
    sigTime = other.sigTime;
    lastPing = other.lastPing;
    protocolVersion = other.protocolVersion;
    nScanningErrorCount = other.nScanningErrorCount;
    nLastScanningErrorBlockHeight = other.nLastScanningErrorBlockHeight;
    mnPayeeScript = other.mnPayeeScript;
}

CMasternode::CMasternode(const CDeterministicMNCPtr& dmn, int64_t registeredTime, const uint256& registeredHash) :
        CSignedMessage()
{
    LOCK(cs);
    vin = CTxIn(dmn->collateralOutpoint);
    addr = dmn->pdmnState->addr;
    pubKeyCollateralAddress = CPubKey();
    pubKeyMasternode = CPubKey();
    sigTime = registeredTime;
    lastPing = CMasternodePing(vin, registeredHash, registeredTime);
    protocolVersion = PROTOCOL_VERSION;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
    mnPayeeScript = dmn->pdmnState->scriptPayout;
}

uint256 CMasternode::GetSignatureHash() const
{
    int version = !addr.IsAddrV1Compatible() ? PROTOCOL_VERSION | ADDRV2_FORMAT : PROTOCOL_VERSION;
    CHashWriter ss(SER_GETHASH, version);
    ss << nMessVersion;
    ss << addr;
    ss << sigTime;
    ss << pubKeyCollateralAddress;
    ss << pubKeyMasternode;
    ss << protocolVersion;
    return ss.GetHash();
}

std::string CMasternode::GetStrMessage() const
{
    return (addr.ToString() +
            std::to_string(sigTime) +
            pubKeyCollateralAddress.GetID().ToString() +
            pubKeyMasternode.GetID().ToString() +
            std::to_string(protocolVersion)
    );
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

//
// When a new masternode broadcast is sent, update our information
//
bool CMasternode::UpdateFromNewBroadcast(CMasternodeBroadcast& mnb)
{
    if (mnb.sigTime > sigTime) {
<<<<<<< HEAD
        pubKeyMasternode = mnb.pubKeyMasternode;
        pubKeyCollateralAddress = mnb.pubKeyCollateralAddress;
        sigTime = mnb.sigTime;
        sig = mnb.sig;
        protocolVersion = mnb.protocolVersion;
        addr = mnb.addr;
        lastTimeChecked = 0;
        int nDoS = 0;
        if (mnb.lastPing == CMasternodePing() || (mnb.lastPing != CMasternodePing() && mnb.lastPing.CheckAndUpdate(nDoS, false))) {
            lastPing = mnb.lastPing;
            mnodeman.mapSeenMasternodePing.insert(make_pair(lastPing.GetHash(), lastPing));
=======
        // TODO: lock cs. Need to be careful as mnb.lastPing.CheckAndUpdate locks cs_main internally.
        nMessVersion = mnb.nMessVersion;
        pubKeyMasternode = mnb.pubKeyMasternode;
        pubKeyCollateralAddress = mnb.pubKeyCollateralAddress;
        sigTime = mnb.sigTime;
        vchSig = mnb.vchSig;
        protocolVersion = mnb.protocolVersion;
        addr = mnb.addr;
        int nDoS = 0;
        if (mnb.lastPing.IsNull() || (!mnb.lastPing.IsNull() && mnb.lastPing.CheckAndUpdate(nDoS, false))) {
            lastPing = mnb.lastPing;
            mnodeman.mapSeenMasternodePing.emplace(lastPing.GetHash(), lastPing);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        return true;
    }
    return false;
}

//
// Deterministically calculate a given "score" for a Masternode depending on how close it's hash is to
// the proof of work for that block. The further away they are the better, the furthest will win the election
// and get paid this block
//
<<<<<<< HEAD
uint256 CMasternode::CalculateScore(int mod, int64_t nBlockHeight)
{
    if (chainActive.Tip() == NULL) return 0;

    uint256 hash = 0;
    uint256 aux = vin.prevout.hash + vin.prevout.n;

    if (!GetBlockHash(hash, nBlockHeight)) {
        LogPrint("masternode","CalculateScore ERROR - nHeight %d - Returned 0\n", nBlockHeight);
        return 0;
    }

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << hash;
    uint256 hash2 = ss.GetHash();

    CHashWriter ss2(SER_GETHASH, PROTOCOL_VERSION);
    ss2 << hash;
    ss2 << aux;
    uint256 hash3 = ss2.GetHash();

    uint256 r = (hash3 > hash2 ? hash3 - hash2 : hash2 - hash3);

    return r;
}

void CMasternode::Check(bool forceCheck)
{
    if (ShutdownRequested()) return;
    if (!forceCheck && (GetTime() - lastTimeChecked < MASTERNODE_CHECK_SECONDS)) return;
    lastTimeChecked = GetTime();

    //once spent, stop doing the checks
    if (activeState == MASTERNODE_VIN_SPENT) return;

    if (!IsPingedWithin(MASTERNODE_REMOVAL_SECONDS)) {
        activeState = MASTERNODE_REMOVE;
        return;
    }

    if (!IsPingedWithin(MASTERNODE_EXPIRATION_SECONDS)) {
        activeState = MASTERNODE_EXPIRED;
        return;
    }

    if (!unitTest) {
        {
            TRY_LOCK(cs_main, lockMain);
            if (!lockMain) return;

            if (IsKeyImageSpend1(vin.keyImage.GetHex(), uint256())) {
                activeState = MASTERNODE_VIN_SPENT;
                return;
            }
        }
    }
    activeState = MASTERNODE_ENABLED; // OK
}

int64_t CMasternode::SecondsSincePayment()
{
    CScript pubkeyScript;
    pubkeyScript = GetScriptForDestination(pubKeyCollateralAddress);

    int64_t sec = (GetAdjustedTime() - GetLastPaid());
    int64_t month = 60 * 60 * 24 * 30;
    if (sec < month) return sec; //if it's less than 30 days, give seconds

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << vin;
    ss << sigTime;
    uint256 hash = ss.GetHash();

    // return some deterministic value for unknown/unpaid but force it to be more than 30 days old
    return month + hash.GetCompact(false);
}

int64_t CMasternode::GetLastPaid()
{
    CBlockIndex* pindexPrev = chainActive.Tip();
    if (pindexPrev == NULL) return false;

    std::vector<unsigned char> mnpayee;
    mnpayee = vin.masternodeStealthAddress;

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << vin;
    ss << sigTime;
    uint256 hash = ss.GetHash();

    // use a deterministic offset to break a tie -- 2.5 minutes
    int64_t nOffset = hash.GetCompact(false) % 150;

    if (chainActive.Tip() == NULL) return false;

    const CBlockIndex* BlockReading = chainActive.Tip();

    int nMnCount = mnodeman.CountEnabled() * 1.25;
    int n = 0;
    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (n >= nMnCount) {
            return 0;
        }
        n++;

        if (masternodePayments.mapMasternodeBlocks.count(BlockReading->nHeight)) {
            /*
                Search for this payee, with at least 2 votes. This will aid in consensus allowing the network 
                to converge on the same payees quickly, then keep the same schedule.
            */
            if (masternodePayments.mapMasternodeBlocks[BlockReading->nHeight].HasPayeeWithVotes(mnpayee, 2)) {
                return BlockReading->nTime + nOffset;
            }
        }

        if (BlockReading->pprev == NULL) {
            assert(BlockReading);
            break;
        }
        BlockReading = BlockReading->pprev;
    }

    return 0;
}

std::string CMasternode::GetStatus()
{
    switch (nActiveState) {
    case CMasternode::MASTERNODE_PRE_ENABLED:
        return "PRE_ENABLED";
    case CMasternode::MASTERNODE_ENABLED:
        return "ENABLED";
    case CMasternode::MASTERNODE_EXPIRED:
        return "EXPIRED";
    case CMasternode::MASTERNODE_OUTPOINT_SPENT:
        return "OUTPOINT_SPENT";
    case CMasternode::MASTERNODE_REMOVE:
        return "REMOVE";
    case CMasternode::MASTERNODE_WATCHDOG_EXPIRED:
        return "WATCHDOG_EXPIRED";
    case CMasternode::MASTERNODE_POSE_BAN:
        return "POSE_BAN";
    default:
        return "UNKNOWN";
    }
}

bool CMasternode::IsValidNetAddr()
{
    // TODO: regtest is fine with any addresses for now,
    // should probably be a bit smarter if one day we start to implement tests for this
    return Params().NetworkID() == CBaseChainParams::REGTEST ||
           (IsReachable(addr) && addr.IsRoutable());
}

CMasternodeBroadcast::CMasternodeBroadcast()
{
    vin = CTxIn();
    addr = CService();
    pubKeyCollateralAddress = CPubKey();
    pubKeyMasternode1 = CPubKey();
    sig = std::vector<unsigned char>();
    activeState = MASTERNODE_ENABLED;
    sigTime = GetAdjustedTime();
    lastPing = CMasternodePing();
    cacheInputAge = 0;
    cacheInputAgeBlock = 0;
    unitTest = false;
    allowFreeTx = true;
    protocolVersion = PROTOCOL_VERSION;
    nLastDsq = 0;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
}

CMasternodeBroadcast::CMasternodeBroadcast(CService newAddr, CTxIn newVin, CPubKey pubKeyCollateralAddressNew, CPubKey pubKeyMasternodeNew, int protocolVersionIn)
=======
arith_uint256 CMasternode::CalculateScore(const uint256& hash) const
{
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << hash;
    const arith_uint256& hash2 = UintToArith256(ss.GetHash());

    CHashWriter ss2(SER_GETHASH, PROTOCOL_VERSION);
    ss2 << hash;
    const arith_uint256& aux = UintToArith256(vin.prevout.hash) + vin.prevout.n;
    ss2 << aux;
    const arith_uint256& hash3 = UintToArith256(ss2.GetHash());

    return (hash3 > hash2 ? hash3 - hash2 : hash2 - hash3);
}

CMasternode::state CMasternode::GetActiveState() const
{
    LOCK(cs);
    if (fCollateralSpent) {
        return MASTERNODE_VIN_SPENT;
    }
    if (!IsPingedWithin(MasternodeRemovalSeconds())) {
        return MASTERNODE_REMOVE;
    }
    if (!IsPingedWithin(MasternodeExpirationSeconds())) {
        return MASTERNODE_EXPIRED;
    }
    if(lastPing.sigTime - sigTime < MasternodeMinPingSeconds()){
        return MASTERNODE_PRE_ENABLED;
    }
    return MASTERNODE_ENABLED;
}

bool CMasternode::IsValidNetAddr() const
{
    // TODO: regtest is fine with any addresses for now,
    // should probably be a bit smarter if one day we start to implement tests for this
    return Params().IsRegTestNet() ||
           (IsReachable(addr) && addr.IsRoutable());
}

CMasternodeBroadcast::CMasternodeBroadcast() :
        CMasternode()
{ }

CMasternodeBroadcast::CMasternodeBroadcast(CService newAddr, CTxIn newVin, CPubKey pubKeyCollateralAddressNew, CPubKey pubKeyMasternodeNew, int protocolVersionIn, const CMasternodePing& _lastPing) :
        CMasternode()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    vin = newVin;
    addr = newAddr;
    pubKeyCollateralAddress = pubKeyCollateralAddressNew;
    pubKeyMasternode = pubKeyMasternodeNew;
<<<<<<< HEAD
    sig = std::vector<unsigned char>();
    activeState = MASTERNODE_ENABLED;
    sigTime = GetAdjustedTime();
    lastPing = CMasternodePing();
    cacheInputAge = 0;
    cacheInputAgeBlock = 0;
    unitTest = false;
    allowFreeTx = true;
    protocolVersion = protocolVersionIn;
    nLastDsq = 0;
    nScanningErrorCount = 0;
    nLastScanningErrorBlockHeight = 0;
}

CMasternodeBroadcast::CMasternodeBroadcast(const CMasternode& mn)
{
    vin = mn.vin;
    addr = mn.addr;
    pubKeyCollateralAddress = mn.pubKeyCollateralAddress;
    pubKeyMasternode = mn.pubKeyMasternode;
    sig = mn.sig;
    activeState = mn.activeState;
    sigTime = mn.sigTime;
    lastPing = mn.lastPing;
    cacheInputAge = mn.cacheInputAge;
    cacheInputAgeBlock = mn.cacheInputAgeBlock;
    unitTest = mn.unitTest;
    allowFreeTx = mn.allowFreeTx;
    protocolVersion = mn.protocolVersion;
    nLastDsq = mn.nLastDsq;
    nScanningErrorCount = mn.nScanningErrorCount;
    nLastScanningErrorBlockHeight = mn.nLastScanningErrorBlockHeight;
}

bool CMasternodeBroadcast::Create(std::string strService, std::string strKeyMasternode, std::string strTxHash, std::string strOutputIndex, std::string& strErrorRet, CMasternodeBroadcast& mnbRet, bool fOffline)
=======
    protocolVersion = protocolVersionIn;
    lastPing = _lastPing;
    sigTime = lastPing.sigTime;
}

CMasternodeBroadcast::CMasternodeBroadcast(const CMasternode& mn) :
        CMasternode(mn)
{ }

bool CMasternodeBroadcast::Create(const std::string& strService,
                                  const std::string& strKeyMasternode,
                                  const std::string& strTxHash,
                                  const std::string& strOutputIndex,
                                  std::string& strErrorRet,
                                  CMasternodeBroadcast& mnbRet,
                                  bool fOffline,
                                  int chainHeight)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    CTxIn txin;
    CPubKey pubKeyCollateralAddressNew;
    CKey keyCollateralAddressNew;
    CPubKey pubKeyMasternodeNew;
    CKey keyMasternodeNew;

    //need correct blocks to send ping
    if (!fOffline && !masternodeSync.IsBlockchainSynced()) {
        strErrorRet = "Sync in progress. Must wait until sync is complete to start Masternode";
<<<<<<< HEAD
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
        return false;
    }

    if (!obfuScationSigner.GetKeysFromSecret(strKeyMasternode, keyMasternodeNew, pubKeyMasternodeNew)) {
        strErrorRet = strprintf("Invalid masternode key %s", strKeyMasternode);
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
        return false;
    }

    if (!pwalletMain->GetMasternodeVinAndKeys(txin, pubKeyCollateralAddressNew, keyCollateralAddressNew, strTxHash, strOutputIndex)) {
        strErrorRet = strprintf("Could not allocate txin %s:%s for masternode %s", strTxHash, strOutputIndex, strService);
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
        return false;
    }

    // The service needs the correct default port to work properly
    if(!CheckDefaultPort(strService, strErrorRet, "CMasternodeBroadcast::Create"))
        return false;

    return Create(txin, CService(strService), keyCollateralAddressNew, pubKeyCollateralAddressNew, keyMasternodeNew, pubKeyMasternodeNew, strErrorRet, mnbRet);
}

bool CMasternodeBroadcast::Create(CTxIn txin, CService service, CKey keyCollateralAddressNew, CPubKey pubKeyCollateralAddressNew, CKey keyMasternodeNew, CPubKey pubKeyMasternodeNew, std::string& strErrorRet, CMasternodeBroadcast& mnbRet)
=======
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strErrorRet);
        return false;
    }

    if (!CMessageSigner::GetKeysFromSecret(strKeyMasternode, keyMasternodeNew, pubKeyMasternodeNew)) {
        strErrorRet = strprintf("Invalid masternode key %s", strKeyMasternode);
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strErrorRet);
        return false;
    }

    std::string strError;
    // Use wallet-0 here. Legacy mnb creation can be removed after transition to DMN
    if (vpwallets.empty() || !vpwallets[0]->GetMasternodeVinAndKeys(txin, pubKeyCollateralAddressNew, keyCollateralAddressNew, strTxHash, strOutputIndex, strError)) {
        strErrorRet = strError; // GetMasternodeVinAndKeys logs this error. Only returned for GUI error notification.
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strprintf("Could not allocate txin %s:%s for masternode %s", strTxHash, strOutputIndex, strService));
        return false;
    }

    int nPort = 0;
    int nDefaultPort = Params().GetDefaultPort();
    std::string strHost;
    SplitHostPort(strService, nPort, strHost);
    if (nPort == 0) nPort = nDefaultPort;
    CService _service(LookupNumeric(strHost.c_str(), nPort));

    // The service needs the correct default port to work properly
    if (!CheckDefaultPort(_service, strErrorRet, "CMasternodeBroadcast::Create"))
        return false;

    return Create(txin, _service, keyCollateralAddressNew, pubKeyCollateralAddressNew, keyMasternodeNew, pubKeyMasternodeNew, strErrorRet, mnbRet);
}

bool CMasternodeBroadcast::Create(const CTxIn& txin,
                                  const CService& service,
                                  const CKey& keyCollateralAddressNew,
                                  const CPubKey& pubKeyCollateralAddressNew,
                                  const CKey& keyMasternodeNew,
                                  const CPubKey& pubKeyMasternodeNew,
                                  std::string& strErrorRet,
                                  CMasternodeBroadcast& mnbRet)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // wait for reindex and/or import to finish
    if (fImporting || fReindex) return false;

<<<<<<< HEAD
    LogPrint("masternode", "CMasternodeBroadcast::Create -- pubKeyCollateralAddressNew = %s, pubKeyMasternodeNew.GetID() = %s\n",
        CBitcoinAddress(pubKeyCollateralAddressNew.GetID()).ToString(),
        pubKeyMasternodeNew.GetID().ToString());

    CMasternodePing mnp(txin);
    if (!mnp.Sign(keyMasternodeNew, pubKeyMasternodeNew)) {
        strErrorRet = strprintf("Failed to sign ping, masternode=%s", txin.prevout.hash.ToString());
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
=======
    LogPrint(BCLog::MASTERNODE, "CMasternodeBroadcast::Create -- pubKeyCollateralAddressNew = %s, pubKeyMasternodeNew.GetID() = %s\n",
             EncodeDestination(pubKeyCollateralAddressNew.GetID()),
        pubKeyMasternodeNew.GetID().ToString());

    // Get block hash to ping (TODO: move outside of this function)
    const uint256& nBlockHashToPing = mnodeman.GetBlockHashToPing();
    CMasternodePing mnp(txin, nBlockHashToPing, GetAdjustedTime());
    if (!mnp.Sign(keyMasternodeNew, pubKeyMasternodeNew.GetID())) {
        strErrorRet = strprintf("Failed to sign ping, masternode=%s", txin.prevout.hash.ToString());
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strErrorRet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        mnbRet = CMasternodeBroadcast();
        return false;
    }

<<<<<<< HEAD
    mnbRet = CMasternodeBroadcast(service, txin, pubKeyCollateralAddressNew, pubKeyMasternodeNew, PROTOCOL_VERSION);

    if (!mnbRet.IsValidNetAddr()) {
        strErrorRet = strprintf("Invalid IP address %s, masternode=%s", mnbRet.addr.ToStringIP (), txin.prevout.hash.ToString());
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
=======
    mnbRet = CMasternodeBroadcast(service, txin, pubKeyCollateralAddressNew, pubKeyMasternodeNew, PROTOCOL_VERSION, mnp);

    if (!mnbRet.IsValidNetAddr()) {
        strErrorRet = strprintf("Invalid IP address %s, masternode=%s", mnbRet.addr.ToStringIP (), txin.prevout.hash.ToString());
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strErrorRet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        mnbRet = CMasternodeBroadcast();
        return false;
    }

<<<<<<< HEAD
    mnbRet.lastPing = mnp;
    if (!mnbRet.Sign(keyCollateralAddressNew)) {
        strErrorRet = strprintf("Failed to sign broadcast, masternode=%s", txin.prevout.hash.ToString());
        LogPrint("masternode","CMasternodeBroadcast::Create -- %s\n", strErrorRet);
=======
    if (!mnbRet.Sign(keyCollateralAddressNew, pubKeyCollateralAddressNew)) {
        strErrorRet = strprintf("Failed to sign broadcast, masternode=%s", txin.prevout.hash.ToString());
        LogPrint(BCLog::MASTERNODE,"CMasternodeBroadcast::Create -- %s\n", strErrorRet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        mnbRet = CMasternodeBroadcast();
        return false;
    }

    return true;
}

<<<<<<< HEAD

bool CMasternodeBroadcast::CheckDefaultPort(std::string strService, std::string& strErrorRet, std::string strContext)
{
    CService service = CService(strService);
    int nDefaultPort = Params().GetDefaultPort();

    if (service.GetPort() != nDefaultPort) {
        strErrorRet = strprintf("Invalid port %u for masternode %s, only %d is supported on %s-net.",
                                service.GetPort(), strService, nDefaultPort, Params().NetworkIDString());
        LogPrint("masternode", "%s - %s\n", strContext, strErrorRet);
=======
bool CMasternodeBroadcast::Sign(const CKey& key, const CPubKey& pubKey)
{
    std::string strError = "";
    nMessVersion = MessageVersion::MESS_VER_HASH;
    const std::string strMessage = GetSignatureHash().GetHex();

    if (!CMessageSigner::SignMessage(strMessage, vchSig, key)) {
        return error("%s : SignMessage() (nMessVersion=%d) failed", __func__, nMessVersion);
    }

    if (!CMessageSigner::VerifyMessage(pubKey, vchSig, strMessage, strError)) {
        return error("%s : VerifyMessage() (nMessVersion=%d) failed, error: %s\n",
                __func__, nMessVersion, strError);
    }

    return true;
}

bool CMasternodeBroadcast::CheckSignature() const
{
    std::string strError = "";
    std::string strMessage = (
                            nMessVersion == MessageVersion::MESS_VER_HASH ?
                            GetSignatureHash().GetHex() :
                            GetStrMessage()
                            );

    if(!CMessageSigner::VerifyMessage(pubKeyCollateralAddress, vchSig, strMessage, strError))
        return error("%s : VerifyMessage (nMessVersion=%d) failed: %s", __func__, nMessVersion, strError);

    return true;
}

bool CMasternodeBroadcast::CheckDefaultPort(CService service, std::string& strErrorRet, const std::string& strContext)
{
    int nDefaultPort = Params().GetDefaultPort();

    if (service.GetPort() != nDefaultPort && !Params().IsRegTestNet()) {
        strErrorRet = strprintf("Invalid port %u for masternode %s, only %d is supported on %s-net.",
            service.GetPort(), service.ToString(), nDefaultPort, Params().NetworkIDString());
        LogPrintf("%s - %s\n", strContext, strErrorRet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    }

    return true;
}

bool CMasternodeBroadcast::CheckAndUpdate(int& nDos)
{
    // make sure signature isn't in the future (past is OK)
<<<<<<< HEAD
    if (sigTime > GetAdjustedTime() + 60 * 60) {
        LogPrint("masternode","mnb - Signature rejected, too far into the future %s\n", vin.prevout.hash.ToString());
=======
    if (sigTime > GetMaxTimeWindow()) {
        LogPrint(BCLog::MASTERNODE,"mnb - Signature rejected, too far into the future %s\n", vin.prevout.hash.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        nDos = 1;
        return false;
    }

<<<<<<< HEAD
    // incorrect ping or its sigTime
    if(lastPing == CMasternodePing() || !lastPing.CheckAndUpdate(nDos, false, true)) {
        LogPrint("masternode","mnb - Incorrect ping or its sigTime");
        return false;
    }

    std::string vchPubKey(pubKeyCollateralAddress.begin(), pubKeyCollateralAddress.end());
    std::string vchPubKey2(pubKeyMasternode.begin(), pubKeyMasternode.end());

    HEX_DATA_STREAM_PROTOCOL(protocolVersion) << addr.ToString() << sigTime << pubKeyCollateralAddress << pubKeyMasternode << protocolVersion;
    std::string strMessage = HEX_STR(ser);

    if (protocolVersion < masternodePayments.GetMinMasternodePaymentsProto()) {
        LogPrint("masternode","mnb - ignoring outdated Masternode %s protocol version %d\n", vin.prevout.hash.ToString(), protocolVersion);
        return false;
    }
    std::string mstl(vin.masternodeStealthAddress.begin(), vin.masternodeStealthAddress.end());

    CScript pubkeyScript;
    pubkeyScript = GetScriptForDestination(pubKeyCollateralAddress);
    LogPrint("masternode", "\nCMasternodeBroadcast::CheckAndUpdate: pubKeyCollateralAddress=%s\n", pubkeyScript.ToString());
    if ((pubkeyScript.size() != 35) && (pubkeyScript.size() != 67)) {
        LogPrint("masternode","mnb - pubkey the wrong size\n");
=======
    // reject old signature version
    if (nMessVersion != MessageVersion::MESS_VER_HASH) {
        LogPrint(BCLog::MASTERNODE, "mnb - rejecting old message version for mn %s\n", vin.prevout.hash.ToString());
        nDos = 30;
        return false;
    }

    if (protocolVersion < ActiveProtocol()) {
        LogPrint(BCLog::MASTERNODE,"mnb - ignoring outdated Masternode %s protocol version %d\n", vin.prevout.hash.ToString(), protocolVersion);
        return false;
    }

    CScript pubkeyScript;
    pubkeyScript = GetScriptForDestination(pubKeyCollateralAddress.GetID());

    if (pubkeyScript.size() != 25) {
        LogPrint(BCLog::MASTERNODE,"mnb - pubkey the wrong size\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        nDos = 100;
        return false;
    }

    CScript pubkeyScript2;
<<<<<<< HEAD
    pubkeyScript2 = GetScriptForDestination(pubKeyMasternode);
    if ((pubkeyScript2.size() != 35) && (pubkeyScript2.size() != 67)) {
        LogPrint("masternode","mnb - pubkey2 the wrong size\n");
=======
    pubkeyScript2 = GetScriptForDestination(pubKeyMasternode.GetID());

    if (pubkeyScript2.size() != 25) {
        LogPrint(BCLog::MASTERNODE,"mnb - pubkey2 the wrong size\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        nDos = 100;
        return false;
    }

    if (!vin.scriptSig.empty()) {
<<<<<<< HEAD
        LogPrint("masternode","mnb - Ignore Not Empty ScriptSig %s\n", vin.prevout.hash.ToString());
        return false;
    }

    std::string errorMessage = "";
    //only need to verify shnorr signature
    if (!VerifyShnorrKeyImageTxIn(vin, GetTxInSignatureHash(vin))) {
        LogPrint("masternode","mnb - Got bad Masternode address signature\n");
        if (masternodeSync.IsBlockchainSynced())
            nDos = 100;
        return false;
    }
    if (Params().NetworkID() == CBaseChainParams::MAIN) {
        if (addr.GetPort() != 59682) return false;
    } else if (addr.GetPort() == 59682)
        return false;

    //search existing Masternode list, this is where we update existing Masternodes with new mnb broadcasts
    CMasternode* pmn = mnodeman.Find(vin);

    // no such masternode, nothing to update
    if (pmn == NULL)
        return true;
    else {
        // this broadcast older than we have, it's bad.
        if (pmn->sigTime > sigTime) {
            LogPrint("masternode","mnb - Bad sigTime %d for Masternode %s (existing broadcast is at %d)\n",
                sigTime, vin.prevout.hash.ToString(), pmn->sigTime);
            return false;
        }
        // masternode is not enabled yet/already, nothing to update
        if (!pmn->IsEnabled()) return true;
    }

    // mn.pubkey = pubkey, IsVinAssociatedWithPubkey is validated once below,
    //   after that they just need to match
    if (pmn->pubKeyCollateralAddress == pubKeyCollateralAddress && !pmn->IsBroadcastedWithin(MASTERNODE_MIN_MNB_SECONDS)) {
        //take the newest entry
        LogPrint("masternode","mnb - Got updated entry for %s\n", vin.prevout.hash.ToString());
        if (pmn->UpdateFromNewBroadcast((*this))) {
            pmn->Check();
=======
        LogPrint(BCLog::MASTERNODE,"mnb - Ignore Not Empty ScriptSig %s\n", vin.prevout.hash.ToString());
        return false;
    }

    std::string strError = "";
    if (!CheckSignature()) {
        // For now (till v6.0), let's be "naive" and not fully ban nodes when the node is syncing
        // This could be a bad parsed BIP155 address that got stored on db on an old software version.
        nDos = masternodeSync.IsSynced() ? 100 : 5;
        return error("%s : Got bad Masternode address signature", __func__);
    }

    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        if (addr.GetPort() != 51472) return false;
    } else if (addr.GetPort() == 51472)
        return false;

    // incorrect ping or its sigTime
    if(lastPing.IsNull() || !lastPing.CheckAndUpdate(nDos, false, true)) {
        return false;
    }

    //search existing Masternode list, this is where we update existing Masternodes with new mnb broadcasts
    CMasternode* pmn = mnodeman.Find(vin.prevout);

    // no such masternode, nothing to update
    if (pmn == NULL) return true;

    // this broadcast is older or equal than the one that we already have - it's bad and should never happen
    // unless someone is doing something fishy
    // (mapSeenMasternodeBroadcast in CMasternodeMan::ProcessMessage should filter legit duplicates)
    if(pmn->sigTime >= sigTime) {
        return error("%s : Bad sigTime %d for Masternode %20s %105s (existing broadcast is at %d)",
                      __func__, sigTime, addr.ToString(), vin.ToString(), pmn->sigTime);
    }

    // masternode is not enabled yet/already, nothing to update
    if (!pmn->IsEnabled()) return true;

    // mn.pubkey = pubkey, IsVinAssociatedWithPubkey is validated once below,
    //   after that they just need to match
    if (pmn->pubKeyCollateralAddress == pubKeyCollateralAddress && !pmn->IsBroadcastedWithin(MasternodeBroadcastSeconds())) {
        //take the newest entry
        LogPrint(BCLog::MASTERNODE,"mnb - Got updated entry for %s\n", vin.prevout.hash.ToString());
        if (pmn->UpdateFromNewBroadcast((*this))) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (pmn->IsEnabled()) Relay();
        }
        masternodeSync.AddedMasternodeList(GetHash());
    }

    return true;
}

<<<<<<< HEAD
bool CMasternodeBroadcast::CheckInputsAndAdd(int& nDoS)
{
    // we are a masternode with the same vin (i.e. already activated) and this mnb is ours (matches our Masternode privkey)
    // so nothing to do here for us
    if (fMasterNode && vin.prevout == activeMasternode.vin.prevout && pubKeyMasternode == activeMasternode.pubKeyMasternode)
        return true;

    // incorrect ping or its sigTime
    if(lastPing == CMasternodePing() || !lastPing.CheckAndUpdate(nDoS, false, true)) return false;
    
    // search existing Masternode list
    CMasternode* pmn = mnodeman.Find(vin);

    if (pmn != NULL) {
        // nothing to do here if we already know about this masternode and it's enabled
        if (pmn->IsEnabled()) return true;
        // if it's not enabled, remove old MN first and continue
        else
            mnodeman.Remove(pmn->vin);
    }

    CValidationState state;
    CMutableTransaction tx = CMutableTransaction();
    CTxOut vout = CTxOut(9999.99 * COIN, obfuScationPool.collateralPubKey);
    tx.vin.push_back(vin);
    tx.vout.push_back(vout);

    {
        TRY_LOCK(cs_main, lockMain);
        if (!lockMain) {
            // not mnb fault, let it to be checked again later
            mnodeman.mapSeenMasternodeBroadcast.erase(GetHash());
            masternodeSync.mapSeenSyncMNB.erase(GetHash());
            return false;
        }

        if (!AcceptableInputs(mempool, state, CTransaction(tx), false, NULL)) {
            //set nDos
            state.IsInvalid(nDoS);
            return false;
        }
    }

    LogPrint("masternode", "mnb - Accepted Masternode entry\n");

    if (GetInputAge(vin) < MASTERNODE_MIN_CONFIRMATIONS) {
        LogPrint("masternode","mnb - Input must have at least %d confirmations\n", MASTERNODE_MIN_CONFIRMATIONS);
        // maybe we miss few blocks, let this mnb to be checked again later
        mnodeman.mapSeenMasternodeBroadcast.erase(GetHash());
        masternodeSync.mapSeenSyncMNB.erase(GetHash());
        return false;
    }

    // verify that sig time is legit in past
    // should be at least not earlier than block when 1000 PRCY tx got MASTERNODE_MIN_CONFIRMATIONS
    uint256 hashBlock = 0;
    CTransaction tx2;
    GetTransaction(vin.prevout.hash, tx2, hashBlock, true);
    BlockMap::iterator mi = mapBlockIndex.find(hashBlock);
    if (mi != mapBlockIndex.end() && (*mi).second) {
        CBlockIndex* pMNIndex = (*mi).second;                                                        // block for 1000 PRCY tx -> 1 confirmation
        CBlockIndex* pConfIndex = chainActive[pMNIndex->nHeight + MASTERNODE_MIN_CONFIRMATIONS - 1]; // block where tx got MASTERNODE_MIN_CONFIRMATIONS
        if (pConfIndex->GetBlockTime() > sigTime) {
            LogPrint("masternode","mnb - Bad sigTime %d for Masternode %s (%i conf block is at %d)\n",
                sigTime, vin.prevout.hash.ToString(), MASTERNODE_MIN_CONFIRMATIONS, pConfIndex->GetBlockTime());
            return false;
        }
    }

    LogPrint("masternode","mnb - Got NEW Masternode entry - %s - %lli \n", vin.prevout.hash.ToString(), sigTime);
    CMasternode mn(*this);
    mnodeman.Add(mn);

    // if it matches our Masternode privkey, then we've been remotely activated
    if (pubKeyMasternode == activeMasternode.pubKeyMasternode && protocolVersion == PROTOCOL_VERSION) {
        activeMasternode.EnableHotColdMasterNode(vin, addr);
    }

    bool isLocal = addr.IsRFC1918() || addr.IsLocal();
    if (Params().NetworkID() == CBaseChainParams::REGTEST) isLocal = false;

    if (!isLocal) Relay();

    return true;
}

void CMasternodeBroadcast::Relay()
{
    CInv inv(MSG_MASTERNODE_ANNOUNCE, GetHash());
    RelayInv(inv);
}

bool CMasternodeBroadcast::Sign(CKey& keyCollateralAddress)
{
    std::string errorMessage;

    std::string vchPubKey(pubKeyCollateralAddress.begin(), pubKeyCollateralAddress.end());
    std::string vchPubKey2(pubKeyMasternode.begin(), pubKeyMasternode.end());

    sigTime = GetAdjustedTime();
    std::string ss = addr.ToString();
    HEX_DATA_STREAM_PROTOCOL(protocolVersion) << addr.ToString() << sigTime << pubKeyCollateralAddress << pubKeyMasternode << protocolVersion;
    std::string strMessage = HEX_STR(ser);

    if (!obfuScationSigner.SignMessage(strMessage, errorMessage, sig, keyCollateralAddress)) {
        LogPrint("masternode","CMasternodeBroadcast::Sign() - Error: %s\n", errorMessage);
        return false;
    }

    if (!obfuScationSigner.VerifyMessage(pubKeyCollateralAddress, sig, strMessage, errorMessage)) {
        LogPrint("masternode","CMasternodeBroadcast::Sign() - Error: %s\n", errorMessage);
        return false;
    }

    return true;
}

bool CMasternodeBroadcast::VerifySignature()
{
    std::string errorMessage;

    if(!obfuScationSigner.VerifyMessage(pubKeyCollateralAddress, sig, GetNewStrMessage(), errorMessage)
       && !obfuScationSigner.VerifyMessage(pubKeyCollateralAddress, sig, GetOldStrMessage(), errorMessage))
        return error("CMasternodeBroadcast::VerifySignature() - Error: %s", errorMessage);

    return true;
}

std::string CMasternodeBroadcast::GetOldStrMessage()
{
    HEX_DATA_STREAM_PROTOCOL(protocolVersion) << addr.ToString() << sigTime << pubKeyCollateralAddress << pubKeyMasternode << protocolVersion;
    std::string strMessage = HEX_STR(ser);
    return strMessage;
}

std:: string CMasternodeBroadcast::GetNewStrMessage()
{
    HEX_DATA_STREAM_PROTOCOL(protocolVersion) << addr.ToString() << sigTime << pubKeyCollateralAddress << pubKeyMasternode << protocolVersion;
    std::string strMessage = HEX_STR(ser);

    return strMessage;
}


CMasternodePing::CMasternodePing()
{
    vin = CTxIn();
    blockHash = uint256(0);
    sigTime = 0;
    vchSig = std::vector<unsigned char>();
}

CMasternodePing::CMasternodePing(CTxIn& newVin)
{
    vin = newVin;
    blockHash = chainActive[chainActive.Height() - 12]->GetBlockHash();
    sigTime = GetAdjustedTime();
    vchSig = std::vector<unsigned char>();
}


bool CMasternodePing::Sign(CKey& keyMasternode, CPubKey& pubKeyMasternode)
{
    std::string errorMessage;
    std::string strMasterNodeSignMessage;

    sigTime = GetAdjustedTime();
    HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << vin.ToString() << blockHash.ToString() << sigTime;
    std::string strMessage = HEX_STR(ser);

    if (!obfuScationSigner.SignMessage(strMessage, errorMessage, vchSig, keyMasternode)) {
        LogPrint("masternode","CMasternodePing::Sign() - Error: %s\n", errorMessage);
        return false;
    }

    if (!obfuScationSigner.VerifyMessage(pubKeyMasternode, vchSig, strMessage, errorMessage)) {
        LogPrint("masternode","CMasternodePing::Sign() - Error: %s\n", errorMessage);
        return false;
    }

    return true;
}

bool CMasternodePing::VerifySignature(CPubKey& pubKeyMasternode, int &nDos) {
    HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << vin.ToString() << blockHash.ToString() << sigTime;
    std::string strMessage = HEX_STR(ser);
    std::string errorMessage = "";

    if(!obfuScationSigner.VerifyMessage(pubKeyMasternode, vchSig, strMessage, errorMessage)){
        nDos = 33;
        return error("CMasternodePing::VerifySignature - Got bad Masternode ping signature %s Error: %s", vin.ToString(), errorMessage);
    }
    return true;
}

bool CMasternodePing::CheckAndUpdate(int& nDos, bool fRequireEnabled, bool fCheckSigTimeOnly)
{
    if (sigTime > GetAdjustedTime() + 60 * 60) {
        LogPrint("masternode","CMasternodePing::CheckAndUpdate - Signature rejected, too far into the future %s\n", vin.prevout.hash.ToString());
        nDos = 1;
        return false;
    }

    if (sigTime <= GetAdjustedTime() - 60 * 60) {
        LogPrint("masternode","CMasternodePing::CheckAndUpdate - Signature rejected, too far into the past %s - %d %d \n", vin.prevout.hash.ToString(), sigTime, GetAdjustedTime());
        nDos = 1;
        return false;
    }

    if(fCheckSigTimeOnly) {
        CMasternode* pmn = mnodeman.Find(vin);
        if(pmn) return VerifySignature(pmn->pubKeyMasternode, nDos);
        return true;
    }

    LogPrint("masternode","CMasternodePing::CheckAndUpdate - New Ping - %s - %lli\n", blockHash.ToString(), sigTime);

    // see if we have this Masternode
    CMasternode* pmn = mnodeman.Find(vin);
    if (pmn != NULL && pmn->protocolVersion >= masternodePayments.GetMinMasternodePaymentsProto()) {
        if (fRequireEnabled && !pmn->IsEnabled()) return false;

        // update only if there is no known ping for this masternode or
        // last ping was more then MASTERNODE_MIN_MNP_SECONDS-60 ago comparing to this one
        if (!pmn->IsPingedWithin(MASTERNODE_MIN_MNP_SECONDS - 60, sigTime)) {

            HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << vin.ToString() << blockHash.ToString() << sigTime;
            std::string strMessage = HEX_STR(ser);

            std::string errorMessage = "";
            if (!obfuScationSigner.VerifyMessage(pmn->pubKeyMasternode, vchSig, strMessage, errorMessage)) {
                LogPrint("masternode","CMasternodePing::CheckAndUpdate - Got bad Masternode address signature %s\n", vin.prevout.hash.ToString());
=======
void CMasternodeBroadcast::Relay()
{
    CInv inv(MSG_MASTERNODE_ANNOUNCE, GetHash());
    g_connman->RelayInv(inv);
}

uint256 CMasternodeBroadcast::GetHash() const
{
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << sigTime;
    ss << pubKeyCollateralAddress;
    return ss.GetHash();
}

CMasternodePing::CMasternodePing() :
        CSignedMessage(),
        vin(),
        blockHash(),
        sigTime(0)
{ }

CMasternodePing::CMasternodePing(const CTxIn& newVin, const uint256& nBlockHash, uint64_t _sigTime) :
        CSignedMessage(),
        vin(newVin),
        blockHash(nBlockHash),
        sigTime(_sigTime)
{ }

uint256 CMasternodePing::GetHash() const
{
    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << vin;
    if (nMessVersion == MessageVersion::MESS_VER_HASH) ss << blockHash;
    ss << sigTime;
    return ss.GetHash();
}

std::string CMasternodePing::GetStrMessage() const
{
    return vin.ToString() + blockHash.ToString() + std::to_string(sigTime);
}

bool CMasternodePing::CheckAndUpdate(int& nDos, bool fRequireAvailable, bool fCheckSigTimeOnly)
{
    if (sigTime > GetMaxTimeWindow()) {
        LogPrint(BCLog::MNPING,"%s: Signature rejected, too far into the future %s\n", __func__, vin.prevout.hash.ToString());
        nDos = 30;
        return false;
    }

    if (sigTime <= GetAdjustedTime() - 60 * 60) {
        LogPrint(BCLog::MNPING,"%s: Signature rejected, too far into the past %s - %d %d \n", __func__, vin.prevout.hash.ToString(), sigTime, GetAdjustedTime());
        nDos = 30;
        return false;
    }

    // reject old signature version
    if (nMessVersion != MessageVersion::MESS_VER_HASH) {
        LogPrint(BCLog::MNPING, "mnp - rejecting old message version for mn %s\n", vin.prevout.hash.ToString());
        nDos = 30;
        return false;
    }

    // Check if the ping block hash exists and it's within 24 blocks from the tip
    if (!mnodeman.IsWithinDepth(blockHash, 2 * MNPING_DEPTH)) {
        LogPrint(BCLog::MNPING,"%s: Masternode %s block hash %s is too old or has an invalid block hash\n",
                                        __func__, vin.prevout.hash.ToString(), blockHash.ToString());
        // don't ban peers relaying stale data before the active protocol enforcement
        nDos = 33;
        return false;
    }

    // see if we have this Masternode
    CMasternode* pmn = mnodeman.Find(vin.prevout);
    const bool isMasternodeFound = (pmn != nullptr);
    const bool isSignatureValid = (isMasternodeFound && CheckSignature(pmn->pubKeyMasternode.GetID()));

    if(fCheckSigTimeOnly) {
        if (isMasternodeFound && !isSignatureValid) {
            nDos = 33;
            return false;
        }
        return true;
    }

    LogPrint(BCLog::MNPING, "%s: New Ping - %s - %s - %lli\n", __func__, GetHash().ToString(), blockHash.ToString(), sigTime);

    if (isMasternodeFound && pmn->protocolVersion >= ActiveProtocol()) {

        // Update ping only if the masternode is in available state (pre-enabled or enabled)
        if (fRequireAvailable && !pmn->IsAvailableState()) {
            nDos = 20;
            return false;
        }

        // update only if there is no known ping for this masternode or
        // last ping was more then MASTERNODE_MIN_MNP_SECONDS-60 ago comparing to this one
        if (!pmn->IsPingedWithin(MasternodeMinPingSeconds() - 60, sigTime)) {
            if (!isSignatureValid) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                nDos = 33;
                return false;
            }

<<<<<<< HEAD
            BlockMap::iterator mi = mapBlockIndex.find(blockHash);
            if (mi != mapBlockIndex.end() && (*mi).second) {
                if ((*mi).second->nHeight < chainActive.Height() - 24) {
                    LogPrint("masternode","CMasternodePing::CheckAndUpdate - Masternode %s block hash %s is too old\n", vin.prevout.hash.ToString(), blockHash.ToString());
                    // Do nothing here (no Masternode update, no mnping relay)
                    // Let this node to be visible but fail to accept mnping

                    return false;
                }
            } else {
                if (fDebug) LogPrint("masternode","CMasternodePing::CheckAndUpdate - Masternode %s block hash %s is unknown\n", vin.prevout.hash.ToString(), blockHash.ToString());
                // maybe we stuck so we shouldn't ban this node, just fail to accept it
                // TODO: or should we also request this block?

                return false;
            }

            pmn->lastPing = *this;

            //mnodeman.mapSeenMasternodeBroadcast.lastPing is probably outdated, so we'll update it
            CMasternodeBroadcast mnb(*pmn);
            uint256 hash = mnb.GetHash();
=======
            // ping have passed the basic checks, can be updated now
            mnodeman.mapSeenMasternodePing.emplace(GetHash(), *this);

            // SetLastPing locks masternode cs. Be careful with the lock ordering.
            pmn->SetLastPing(*this);

            //mnodeman.mapSeenMasternodeBroadcast.lastPing is probably outdated, so we'll update it
            CMasternodeBroadcast mnb(*pmn);
            const uint256& hash = mnb.GetHash();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (mnodeman.mapSeenMasternodeBroadcast.count(hash)) {
                mnodeman.mapSeenMasternodeBroadcast[hash].lastPing = *this;
            }

<<<<<<< HEAD
            pmn->Check(true);
            if (!pmn->IsEnabled()) return false;

            LogPrint("masternode", "CMasternodePing::CheckAndUpdate - Masternode ping accepted, vin: %s\n", vin.prevout.hash.ToString());
=======
            if (!pmn->IsEnabled()) return false;

            LogPrint(BCLog::MNPING, "%s: Masternode ping accepted, vin: %s\n", __func__, vin.prevout.hash.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            Relay();
            return true;
        }
<<<<<<< HEAD
        LogPrint("masternode", "CMasternodePing::CheckAndUpdate - Masternode ping arrived too early, vin: %s\n", vin.prevout.hash.ToString());
        return false;
    }
    LogPrint("masternode", "CMasternodePing::CheckAndUpdate - Couldn't find compatible Masternode entry, vin: %s\n", vin.prevout.hash.ToString());
=======
        LogPrint(BCLog::MNPING, "%s: Masternode ping arrived too early, vin: %s\n", __func__, vin.prevout.hash.ToString());
        //nDos = 1; //disable, this is happening frequently and causing banned peers
        return false;
    }
    LogPrint(BCLog::MNPING, "%s: Couldn't find compatible Masternode entry, vin: %s\n", __func__, vin.prevout.hash.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return false;
}

void CMasternodePing::Relay()
{
    CInv inv(MSG_MASTERNODE_PING, GetHash());
<<<<<<< HEAD
    RelayInv(inv);
=======
    g_connman->RelayInv(inv);
}

MasternodeRef MakeMasternodeRefForDMN(const CDeterministicMNCPtr& dmn)
{
    // create legacy masternode for DMN
    int refHeight = std::max(dmn->pdmnState->nRegisteredHeight, dmn->pdmnState->nPoSeRevivedHeight);
    const CBlockIndex* pindex = WITH_LOCK(cs_main, return mapBlockIndex.at(chainActive[refHeight]->GetBlockHash()); );
    return std::make_shared<CMasternode>(CMasternode(dmn, pindex->GetBlockTime(), pindex->GetBlockHash()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
