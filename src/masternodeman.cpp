// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternodeman.h"
<<<<<<< HEAD
#include "activemasternode.h"
#include "addrman.h"
#include "masternode.h"
#include "obfuscation.h"
#include "util.h"
#include <boost/filesystem.hpp>
=======

#include "addrman.h"
#include "evo/deterministicmns.h"
#include "fs.h"
#include "masternode-payments.h"
#include "masternode-sync.h"
#include "masternode.h"
#include "messagesigner.h"
#include "netbase.h"
#include "netmessagemaker.h"
#include "shutdown.h"
#include "spork.h"
#include "validation.h"

#include <boost/thread/thread.hpp>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#define MN_WINNER_MINIMUM_AGE 8000    // Age in seconds. This should be > MASTERNODE_REMOVAL_SECONDS to avoid misconfigured new nodes in the list.

/** Masternode manager */
CMasternodeMan mnodeman;
<<<<<<< HEAD

struct CompareLastPaid {
    bool operator()(const pair<int64_t, CTxIn>& t1,
        const pair<int64_t, CTxIn>& t2) const
    {
        return t1.first < t2.first;
    }
};

struct CompareScoreTxIn {
    bool operator()(const pair<int64_t, CTxIn>& t1,
        const pair<int64_t, CTxIn>& t2) const
    {
        return t1.first < t2.first;
    }
};

struct CompareScoreMN {
    bool operator()(const pair<int64_t, CMasternode>& t1,
        const pair<int64_t, CMasternode>& t2) const
=======
/** Keep track of the active Masternode */
CActiveMasternode activeMasternode;

struct CompareScoreMN {
    template <typename T>
    bool operator()(const std::pair<int64_t, T>& t1,
        const std::pair<int64_t, T>& t2) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        return t1.first < t2.first;
    }
};

//
// CMasternodeDB
//

<<<<<<< HEAD
=======
static const int MASTERNODE_DB_VERSION_BIP155 = 2;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
CMasternodeDB::CMasternodeDB()
{
    pathMN = GetDataDir() / "mncache.dat";
    strMagicMessage = "MasternodeCache";
}

bool CMasternodeDB::Write(const CMasternodeMan& mnodemanToSave)
{
    int64_t nStart = GetTimeMillis();
<<<<<<< HEAD

    // serialize, checksum data up to that point, then append checksum
    CDataStream ssMasternodes(SER_DISK, CLIENT_VERSION);
    ssMasternodes << strMagicMessage;                   // masternode cache file specific magic message
    ssMasternodes << FLATDATA(Params().MessageStart()); // network specific magic number
=======
    const auto& params = Params();

    // serialize, checksum data up to that point, then append checksum
    // Always done in the latest format.
    CDataStream ssMasternodes(SER_DISK, CLIENT_VERSION | ADDRV2_FORMAT);
    ssMasternodes << MASTERNODE_DB_VERSION_BIP155;
    ssMasternodes << strMagicMessage;                   // masternode cache file specific magic message
    ssMasternodes << params.MessageStart(); // network specific magic number
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ssMasternodes << mnodemanToSave;
    uint256 hash = Hash(ssMasternodes.begin(), ssMasternodes.end());
    ssMasternodes << hash;

    // open output file, and associate with CAutoFile
<<<<<<< HEAD
    FILE* file = fopen(pathMN.string().c_str(), "wb");
=======
    FILE* file = fsbridge::fopen(pathMN, "wb");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CAutoFile fileout(file, SER_DISK, CLIENT_VERSION);
    if (fileout.IsNull())
        return error("%s : Failed to open file %s", __func__, pathMN.string());

    // Write and commit header, data
    try {
        fileout << ssMasternodes;
    } catch (const std::exception& e) {
        return error("%s : Serialize or I/O error - %s", __func__, e.what());
    }
<<<<<<< HEAD

    fileout.fclose();

    LogPrint("masternode","Written info to mncache.dat  %dms\n", GetTimeMillis() - nStart);
    LogPrint("masternode","  %s\n", mnodemanToSave.ToString());
=======
    //    FileCommit(fileout);
    fileout.fclose();

    LogPrint(BCLog::MASTERNODE,"Written info to mncache.dat  %dms\n", GetTimeMillis() - nStart);
    LogPrint(BCLog::MASTERNODE,"  %s\n", mnodemanToSave.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return true;
}

<<<<<<< HEAD
CMasternodeDB::ReadResult CMasternodeDB::Read(CMasternodeMan& mnodemanToLoad, bool fDryRun)
{
    int64_t nStart = GetTimeMillis();
    // open input file, and associate with CAutoFile
    FILE* file = fopen(pathMN.string().c_str(), "rb");
=======
CMasternodeDB::ReadResult CMasternodeDB::Read(CMasternodeMan& mnodemanToLoad)
{
    int64_t nStart = GetTimeMillis();
    // open input file, and associate with CAutoFile
    FILE* file = fsbridge::fopen(pathMN, "rb");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CAutoFile filein(file, SER_DISK, CLIENT_VERSION);
    if (filein.IsNull()) {
        error("%s : Failed to open file %s", __func__, pathMN.string());
        return FileError;
    }

    // use file size to size memory buffer
<<<<<<< HEAD
    int fileSize = boost::filesystem::file_size(pathMN);
=======
    int fileSize = fs::file_size(pathMN);
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

<<<<<<< HEAD
    CDataStream ssMasternodes(vchData, SER_DISK, CLIENT_VERSION);
=======
    const auto& params = Params();
    // serialize, checksum data up to that point, then append checksum
    CDataStream ssMasternodes(vchData, SER_DISK,  CLIENT_VERSION);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // verify stored checksum matches input data
    uint256 hashTmp = Hash(ssMasternodes.begin(), ssMasternodes.end());
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
        ssMasternodes >> version;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        ssMasternodes >> strMagicMessageTmp;

        // ... verify the message matches predefined one
        if (strMagicMessage != strMagicMessageTmp) {
            error("%s : Invalid masternode cache magic message", __func__);
            return IncorrectMagicMessage;
        }

        // de-serialize file header (network specific magic number) and ..
<<<<<<< HEAD
        ssMasternodes >> FLATDATA(pchMsgTmp);

        // ... verify the network matches ours
        if (memcmp(pchMsgTmp, Params().MessageStart(), sizeof(pchMsgTmp))) {
            error("%s : Invalid network magic number", __func__);
            return IncorrectMagicNumber;
        }
        // de-serialize data into CMasternodeMan object
        ssMasternodes >> mnodemanToLoad;
=======
        std::vector<unsigned char> pchMsgTmp(4);
        ssMasternodes >> MakeSpan(pchMsgTmp);

        // ... verify the network matches ours
        if (memcmp(pchMsgTmp.data(), params.MessageStart(), pchMsgTmp.size()) != 0) {
            error("%s : Invalid network magic number", __func__);
            return IncorrectMagicNumber;
        }
        // de-serialize data into CMasternodeMan object.
        if (version == MASTERNODE_DB_VERSION_BIP155) {
            OverrideStream<CDataStream> s(&ssMasternodes, ssMasternodes.GetType(), ssMasternodes.GetVersion() | ADDRV2_FORMAT);
            s >> mnodemanToLoad;
        } else {
            // Old format
            ssMasternodes >> mnodemanToLoad;
        }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    } catch (const std::exception& e) {
        mnodemanToLoad.Clear();
        error("%s : Deserialize or I/O error - %s", __func__, e.what());
        return IncorrectFormat;
    }

<<<<<<< HEAD
    LogPrint("masternode","Loaded info from mncache.dat  %dms\n", GetTimeMillis() - nStart);
    LogPrint("masternode","  %s\n", mnodemanToLoad.ToString());
    if (!fDryRun) {
        LogPrint("masternode","Masternode manager - cleaning....\n");
        mnodemanToLoad.CheckAndRemove(true);
        LogPrint("masternode","Masternode manager - result:\n");
        LogPrint("masternode","  %s\n", mnodemanToLoad.ToString());
    }
=======
    LogPrint(BCLog::MASTERNODE,"Loaded info from mncache.dat (dbversion=%d) %dms\n", version, GetTimeMillis() - nStart);
    LogPrint(BCLog::MASTERNODE,"  %s\n", mnodemanToLoad.ToString());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return Ok;
}

void DumpMasternodes()
{
    int64_t nStart = GetTimeMillis();

    CMasternodeDB mndb;
<<<<<<< HEAD
    CMasternodeMan tempMnodeman;

    LogPrint("masternode","Verifying mncache.dat format...\n");
    CMasternodeDB::ReadResult readResult = mndb.Read(tempMnodeman, true);
    // there was an error and it was not an error on file opening => do not proceed
    if (readResult == CMasternodeDB::FileError)
        LogPrint("masternode","Missing masternode cache file - mncache.dat, will try to recreate\n");
    else if (readResult != CMasternodeDB::Ok) {
        LogPrint("masternode","Error reading mncache.dat: ");
        if (readResult == CMasternodeDB::IncorrectFormat)
            LogPrint("masternode","magic is ok but data has invalid format, will try to recreate\n");
        else {
            LogPrint("masternode","file format is unknown or invalid, please fix it manually\n");
            return;
        }
    }
    LogPrint("masternode","Writting info to mncache.dat...\n");
    mndb.Write(mnodeman);

    LogPrint("masternode","Masternode dump finished  %dms\n", GetTimeMillis() - nStart);
}

CMasternodeMan::CMasternodeMan()
{
    nDsqCount = 0;
}

bool CMasternodeMan::Add(CMasternode& mn)
{
    LOCK(cs);

    if (!mn.IsEnabled())
        return false;

    CMasternode* pmn = Find(mn.vin);
    if (pmn == NULL) {
        LogPrint("masternode", "CMasternodeMan: Adding new Masternode %s - %i now\n", mn.vin.prevout.hash.ToString(), size() + 1);
        vMasternodes.push_back(mn);
=======
    LogPrint(BCLog::MASTERNODE,"Writing info to mncache.dat...\n");
    mndb.Write(mnodeman);

    LogPrint(BCLog::MASTERNODE,"Masternode dump finished  %dms\n", GetTimeMillis() - nStart);
}

CMasternodeMan::CMasternodeMan():
        cvLastBlockHashes(CACHED_BLOCK_HASHES, UINT256_ZERO),
        nDsqCount(0)
{}

bool CMasternodeMan::Add(CMasternode& mn)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        return false;
    }

    if (deterministicMNManager->GetListAtChainTip().HasMNByCollateral(mn.vin.prevout)) {
        LogPrint(BCLog::MASTERNODE, "ERROR: Not Adding Masternode %s as the collateral is already registered with a DMN\n",
                mn.vin.prevout.ToString());
        return false;
    }

    LOCK(cs);

    if (!mn.IsAvailableState())
        return false;

    const auto& it = mapMasternodes.find(mn.vin.prevout);
    if (it == mapMasternodes.end()) {
        LogPrint(BCLog::MASTERNODE, "Adding new Masternode %s\n", mn.vin.prevout.ToString());
        mapMasternodes.emplace(mn.vin.prevout, std::make_shared<CMasternode>(mn));
        LogPrint(BCLog::MASTERNODE, "Masternode added. New total count: %d\n", mapMasternodes.size());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return true;
    }

    return false;
}

<<<<<<< HEAD
void CMasternodeMan::AskForMN(CNode* pnode, CTxIn& vin)
{
=======
void CMasternodeMan::AskForMN(CNode* pnode, const CTxIn& vin)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        return;
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    std::map<COutPoint, int64_t>::iterator i = mWeAskedForMasternodeListEntry.find(vin.prevout);
    if (i != mWeAskedForMasternodeListEntry.end()) {
        int64_t t = (*i).second;
        if (GetTime() < t) return; // we've asked recently
    }

    // ask for the mnb info once from the node that sent mnp

<<<<<<< HEAD
    LogPrint("masternode", "\nCMasternodeMan::AskForMN - Asking node for missing entry, vin: %s\n", vin.prevout.hash.ToString());
    std::string stl(vin.masternodeStealthAddress.begin(), vin.masternodeStealthAddress.end());
    LogPrint("masternode", "\nCMasternodeMan::AskForMN - stealth masternode address = %s\n", stl);
    pnode->PushMessage("dseg", vin);
    int64_t askAgain = GetTime() + MASTERNODE_MIN_MNP_SECONDS;
    mWeAskedForMasternodeListEntry[vin.prevout] = askAgain;
}

void CMasternodeMan::Check()
{
    LOCK(cs);

    for (CMasternode& mn : vMasternodes) {
        mn.Check();
    }
}

void CMasternodeMan::CheckAndRemove(bool forceExpiredRemoval)
{
    Check();

    LOCK(cs);

    //remove inactive and outdated
    vector<CMasternode>::iterator it = vMasternodes.begin();
    while (it != vMasternodes.end()) {
        if ((*it).activeState == CMasternode::MASTERNODE_REMOVE ||
            (*it).activeState == CMasternode::MASTERNODE_VIN_SPENT ||
            (forceExpiredRemoval && (*it).activeState == CMasternode::MASTERNODE_EXPIRED) ||
            (*it).protocolVersion < masternodePayments.GetMinMasternodePaymentsProto()) {
            LogPrint("masternode", "CMasternodeMan: Removing inactive Masternode %s - %i now,mapSeenMasternodeBroadcast.size=%d\n", (*it).vin.prevout.hash.ToString(), size() - 1, mapSeenMasternodeBroadcast.size());

            //erase all of the broadcasts we've seen from this vin
            // -- if we missed a few pings and the node was removed, this will allow is to get it back without them
            //    sending a brand new mnb
            map<uint256, CMasternodeBroadcast>::iterator it3 = mapSeenMasternodeBroadcast.begin();
            while (it3 != mapSeenMasternodeBroadcast.end()) {
                if ((*it3).second.vin == (*it).vin) {
                    masternodeSync.mapSeenSyncMNB.erase((*it3).first);
                    mapSeenMasternodeBroadcast.erase(it3++);
=======
    LogPrint(BCLog::MASTERNODE, "CMasternodeMan::AskForMN - Asking node for missing entry, vin: %s\n", vin.prevout.hash.ToString());
    g_connman->PushMessage(pnode, CNetMsgMaker(pnode->GetSendVersion()).Make(NetMsgType::GETMNLIST, vin));
    int64_t askAgain = GetTime() + MasternodeMinPingSeconds();
    mWeAskedForMasternodeListEntry[vin.prevout] = askAgain;
}

int CMasternodeMan::CheckAndRemove(bool forceExpiredRemoval)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        LogPrint(BCLog::MASTERNODE, "Removing all legacy mn due to SPORK 21\n");
        Clear();
        return 0;
    }

    LOCK(cs);

    //remove inactive and outdated (or replaced by DMN)
    auto it = mapMasternodes.begin();
    while (it != mapMasternodes.end()) {
        MasternodeRef& mn = it->second;
        auto activeState = mn->GetActiveState();
        if (activeState == CMasternode::MASTERNODE_REMOVE ||
            activeState == CMasternode::MASTERNODE_VIN_SPENT ||
            (forceExpiredRemoval && activeState == CMasternode::MASTERNODE_EXPIRED) ||
            mn->protocolVersion < ActiveProtocol()) {
            LogPrint(BCLog::MASTERNODE, "Removing inactive (legacy) Masternode %s\n", it->first.ToString());
            //erase all of the broadcasts we've seen from this vin
            // -- if we missed a few pings and the node was removed, this will allow is to get it back without them
            //    sending a brand new mnb
            std::map<uint256, CMasternodeBroadcast>::iterator it3 = mapSeenMasternodeBroadcast.begin();
            while (it3 != mapSeenMasternodeBroadcast.end()) {
                if (it3->second.vin.prevout == it->first) {
                    masternodeSync.mapSeenSyncMNB.erase((*it3).first);
                    it3 = mapSeenMasternodeBroadcast.erase(it3);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                } else {
                    ++it3;
                }
            }

            // allow us to ask for this masternode again if we see another ping
<<<<<<< HEAD
            map<COutPoint, int64_t>::iterator it2 = mWeAskedForMasternodeListEntry.begin();
            while (it2 != mWeAskedForMasternodeListEntry.end()) {
                if ((*it2).first == (*it).vin.prevout) {
                    mWeAskedForMasternodeListEntry.erase(it2++);
=======
            std::map<COutPoint, int64_t>::iterator it2 = mWeAskedForMasternodeListEntry.begin();
            while (it2 != mWeAskedForMasternodeListEntry.end()) {
                if (it2->first == it->first) {
                    it2 = mWeAskedForMasternodeListEntry.erase(it2);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                } else {
                    ++it2;
                }
            }

<<<<<<< HEAD
            it = vMasternodes.erase(it);
=======
            // clean MN pings right away.
            auto itPing = mapSeenMasternodePing.begin();
            while (itPing != mapSeenMasternodePing.end()) {
                if (itPing->second.GetVin().prevout == it->first) {
                    itPing = mapSeenMasternodePing.erase(itPing);
                } else {
                    ++itPing;
                }
            }

            it = mapMasternodes.erase(it);
            LogPrint(BCLog::MASTERNODE, "Masternode removed.\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it;
        }
    }
<<<<<<< HEAD

    // check who's asked for the Masternode list
    map<CNetAddr, int64_t>::iterator it1 = mAskedUsForMasternodeList.begin();
    while (it1 != mAskedUsForMasternodeList.end()) {
        if ((*it1).second < GetTime()) {
            mAskedUsForMasternodeList.erase(it1++);
=======
    LogPrint(BCLog::MASTERNODE, "New total masternode count: %d\n", mapMasternodes.size());

    // check who's asked for the Masternode list
    std::map<CNetAddr, int64_t>::iterator it1 = mAskedUsForMasternodeList.begin();
    while (it1 != mAskedUsForMasternodeList.end()) {
        if ((*it1).second < GetTime()) {
            it1 = mAskedUsForMasternodeList.erase(it1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it1;
        }
    }

    // check who we asked for the Masternode list
    it1 = mWeAskedForMasternodeList.begin();
    while (it1 != mWeAskedForMasternodeList.end()) {
        if ((*it1).second < GetTime()) {
<<<<<<< HEAD
            mWeAskedForMasternodeList.erase(it1++);
=======
            it1 = mWeAskedForMasternodeList.erase(it1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it1;
        }
    }

    // check which Masternodes we've asked for
<<<<<<< HEAD
    map<COutPoint, int64_t>::iterator it2 = mWeAskedForMasternodeListEntry.begin();
    while (it2 != mWeAskedForMasternodeListEntry.end()) {
        if ((*it2).second < GetTime()) {
            mWeAskedForMasternodeListEntry.erase(it2++);
=======
    std::map<COutPoint, int64_t>::iterator it2 = mWeAskedForMasternodeListEntry.begin();
    while (it2 != mWeAskedForMasternodeListEntry.end()) {
        if ((*it2).second < GetTime()) {
            it2 = mWeAskedForMasternodeListEntry.erase(it2);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it2;
        }
    }

    // remove expired mapSeenMasternodeBroadcast
<<<<<<< HEAD
    map<uint256, CMasternodeBroadcast>::iterator it3 = mapSeenMasternodeBroadcast.begin();
    while (it3 != mapSeenMasternodeBroadcast.end()) {
        if ((*it3).second.lastPing.sigTime < GetTime() - (MASTERNODE_REMOVAL_SECONDS * 2)) {
            mapSeenMasternodeBroadcast.erase(it3++);
            masternodeSync.mapSeenSyncMNB.erase((*it3).second.GetHash());
=======
    std::map<uint256, CMasternodeBroadcast>::iterator it3 = mapSeenMasternodeBroadcast.begin();
    while (it3 != mapSeenMasternodeBroadcast.end()) {
        if ((*it3).second.lastPing.sigTime < GetTime() - (MasternodeRemovalSeconds() * 2)) {
            masternodeSync.mapSeenSyncMNB.erase((*it3).second.GetHash());
            it3 = mapSeenMasternodeBroadcast.erase(it3);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it3;
        }
    }

    // remove expired mapSeenMasternodePing
<<<<<<< HEAD
    map<uint256, CMasternodePing>::iterator it4 = mapSeenMasternodePing.begin();
    while (it4 != mapSeenMasternodePing.end()) {
        if ((*it4).second.sigTime < GetTime() - (MASTERNODE_REMOVAL_SECONDS * 2)) {
            mapSeenMasternodePing.erase(it4++);
=======
    std::map<uint256, CMasternodePing>::iterator it4 = mapSeenMasternodePing.begin();
    while (it4 != mapSeenMasternodePing.end()) {
        if ((*it4).second.sigTime < GetTime() - (MasternodeRemovalSeconds() * 2)) {
            it4 = mapSeenMasternodePing.erase(it4);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            ++it4;
        }
    }
<<<<<<< HEAD
=======

    return mapMasternodes.size();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void CMasternodeMan::Clear()
{
    LOCK(cs);
<<<<<<< HEAD
    vMasternodes.clear();
=======
    mapMasternodes.clear();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    mAskedUsForMasternodeList.clear();
    mWeAskedForMasternodeList.clear();
    mWeAskedForMasternodeListEntry.clear();
    mapSeenMasternodeBroadcast.clear();
    mapSeenMasternodePing.clear();
    nDsqCount = 0;
}

<<<<<<< HEAD
int CMasternodeMan::stable_size ()
{
    int nStable_size = 0;
    int nMinProtocol = ActiveProtocol();
    int64_t nMasternode_Min_Age = MN_WINNER_MINIMUM_AGE;
    int64_t nMasternode_Age = 0;

    for (CMasternode& mn : vMasternodes) {
        if (mn.protocolVersion < nMinProtocol) {
            continue; // Skip obsolete versions
        }
        mn.Check ();
        if (!mn.IsEnabled ())
            continue; // Skip not-enabled masternodes

        nStable_size++;
    }

    return nStable_size;
}

int CMasternodeMan::CountEnabled(int protocolVersion)
{
    int i = 0;
    protocolVersion = protocolVersion == -1 ? masternodePayments.GetMinMasternodePaymentsProto() : protocolVersion;

    for (CMasternode& mn : vMasternodes) {
        mn.Check();
        if (mn.protocolVersion < protocolVersion || !mn.IsEnabled()) continue;
        i++;
    }

    return i;
}

void CMasternodeMan::CountNetworks(int protocolVersion, int& ipv4, int& ipv6, int& onion)
{
    protocolVersion = protocolVersion == -1 ? masternodePayments.GetMinMasternodePaymentsProto() : protocolVersion;

    for (CMasternode& mn : vMasternodes) {
        mn.Check();
        std::string strHost;
        int port;
        SplitHostPort(mn.addr.ToString(), port, strHost);
        CNetAddr node = CNetAddr(strHost);
        int nNetwork = node.GetNetwork();
        switch (nNetwork) {
            case 1 :
                ipv4++;
                break;
            case 2 :
                ipv6++;
                break;
            case 3 :
                onion++;
                break;
        }
    }
}

void CMasternodeMan::DsegUpdate(CNode* pnode)
{
    LOCK(cs);

    if (Params().NetworkID() == CBaseChainParams::MAIN) {
=======
static void CountNetwork(const CService& addr, int& ipv4, int& ipv6, int& onion)
{
    std::string strHost;
    int port;
    SplitHostPort(addr.ToString(), port, strHost);
    CNetAddr node;
    LookupHost(strHost, node, false);
    switch(node.GetNetwork()) {
        case NET_IPV4:
            ipv4++;
            break;
        case NET_IPV6:
            ipv6++;
            break;
        case NET_ONION:
            onion++;
            break;
        default:
            break;
    }
}

CMasternodeMan::MNsInfo CMasternodeMan::getMNsInfo() const
{
    MNsInfo info;
    int nMinProtocol = ActiveProtocol();
    bool spork_8_active = sporkManager.IsSporkActive(SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT);

    // legacy masternodes
    {
        LOCK(cs);
        for (const auto& it : mapMasternodes) {
            const MasternodeRef& mn = it.second;
            info.total++;
            CountNetwork(mn->addr, info.ipv4, info.ipv6, info.onion);
            if (mn->protocolVersion < nMinProtocol || !mn->IsEnabled()) {
                continue;
            }
            info.enabledSize++;
            // Eligible for payments
            if (spork_8_active && (GetAdjustedTime() - mn->sigTime < MN_WINNER_MINIMUM_AGE)) {
                continue; // Skip masternodes younger than (default) 8000 sec (MUST be > MASTERNODE_REMOVAL_SECONDS)
            }
            info.stableSize++;
        }
    }

    // deterministic masternodes
    if (deterministicMNManager->IsDIP3Enforced()) {
        auto mnList = deterministicMNManager->GetListAtChainTip();
        mnList.ForEachMN(false, [&](const CDeterministicMNCPtr& dmn) {
            info.total++;
            CountNetwork(dmn->pdmnState->addr, info.ipv4, info.ipv6, info.onion);
            if (!dmn->IsPoSeBanned()) {
                info.enabledSize++;
                info.stableSize++;
            }
        });
    }

    return info;
}

int CMasternodeMan::CountEnabled(bool only_legacy) const
{
    int count_enabled = 0;
    int protocolVersion = ActiveProtocol();

    {
        LOCK(cs);
        for (const auto& it : mapMasternodes) {
            const MasternodeRef& mn = it.second;
            if (mn->protocolVersion < protocolVersion || !mn->IsEnabled()) continue;
            count_enabled++;
        }
    }

    if (!only_legacy && deterministicMNManager->IsDIP3Enforced()) {
        count_enabled += deterministicMNManager->GetListAtChainTip().GetValidMNsCount();
    }

    return count_enabled;
}

bool CMasternodeMan::RequestMnList(CNode* pnode)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        return false;
    }

    LOCK(cs);
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!(pnode->addr.IsRFC1918() || pnode->addr.IsLocal())) {
            std::map<CNetAddr, int64_t>::iterator it = mWeAskedForMasternodeList.find(pnode->addr);
            if (it != mWeAskedForMasternodeList.end()) {
                if (GetTime() < (*it).second) {
<<<<<<< HEAD
                    LogPrint("masternode", "dseg - we already asked peer %i for the list; skipping...\n", pnode->GetId());
                    return;
=======
                    LogPrint(BCLog::MASTERNODE, "dseg - we already asked peer %i for the list; skipping...\n", pnode->GetId());
                    return false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                }
            }
        }
    }

<<<<<<< HEAD
    pnode->PushMessage("dseg", CTxIn());
    int64_t askAgain = GetTime() + MASTERNODES_DSEG_SECONDS;
    mWeAskedForMasternodeList[pnode->addr] = askAgain;
}

CMasternode* CMasternodeMan::Find(const CScript& payee)
{
    LOCK(cs);
    CScript payee2;

    for (CMasternode& mn : vMasternodes) {
        payee2 = GetScriptForDestination(mn.pubKeyCollateralAddress);
        if (payee2 == payee)
            return &mn;
    }
    return NULL;
}

CMasternode* CMasternodeMan::Find(const CTxIn& vin)
{
    LOCK(cs);

    for (CMasternode& mn : vMasternodes) {
        if (mn.vin.prevout == vin.prevout)
            return &mn;
    }
    return NULL;
}


=======
    g_connman->PushMessage(pnode, CNetMsgMaker(pnode->GetSendVersion()).Make(NetMsgType::GETMNLIST, CTxIn()));
    int64_t askAgain = GetTime() + MASTERNODES_REQUEST_SECONDS;
    mWeAskedForMasternodeList[pnode->addr] = askAgain;
    return true;
}

CMasternode* CMasternodeMan::Find(const COutPoint& collateralOut)
{
    LOCK(cs);
    auto it = mapMasternodes.find(collateralOut);
    return it != mapMasternodes.end() ? it->second.get() : nullptr;
}

const CMasternode* CMasternodeMan::Find(const COutPoint& collateralOut) const
{
    LOCK(cs);
    auto const& it = mapMasternodes.find(collateralOut);
    return it != mapMasternodes.end() ? it->second.get() : nullptr;
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
CMasternode* CMasternodeMan::Find(const CPubKey& pubKeyMasternode)
{
    LOCK(cs);

<<<<<<< HEAD
    for (CMasternode& mn : vMasternodes) {
        if (mn.pubKeyMasternode == pubKeyMasternode)
            return &mn;
    }
    return NULL;
}

//
// Deterministically select the oldest/best masternode to pay on the network
//
CMasternode* CMasternodeMan::GetNextMasternodeInQueueForPayment(int nBlockHeight, bool fFilterSigTime, int& nCount)
{
    LOCK(cs);

    CMasternode* pBestMasternode = NULL;
    std::vector<pair<int64_t, CTxIn> > vecMasternodeLastPaid;

    /*
        Make a vector with all of the last paid times
    */

    int nMnCount = CountEnabled();
    for (CMasternode& mn : vMasternodes) {
        mn.Check();
        if (!mn.IsEnabled()) continue;

        // //check protocol version
        if (mn.protocolVersion < masternodePayments.GetMinMasternodePaymentsProto()) continue;

        //it's in the list (up to 8 entries ahead of current block to allow propagation) -- so let's skip it
        if (masternodePayments.IsScheduled(mn, nBlockHeight)) continue;

        //it's too new, wait for a cycle
        if (fFilterSigTime && mn.sigTime + (nMnCount * 2.6 * 60) > GetAdjustedTime()) continue;

        //make sure it has as many confirmations as there are masternodes
        if (mn.GetMasternodeInputAge() < nMnCount) continue;

        vecMasternodeLastPaid.push_back(make_pair(mn.SecondsSincePayment(), mn.vin));
=======
    for (auto& it : mapMasternodes) {
        MasternodeRef& mn = it.second;
        if (mn->pubKeyMasternode == pubKeyMasternode)
            return mn.get();
    }
    return nullptr;
}

void CMasternodeMan::CheckSpentCollaterals(const std::vector<CTransactionRef>& vtx)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        return;
    }

    LOCK(cs);
    for (const auto& tx : vtx) {
        for (const auto& in : tx->vin) {
            auto it = mapMasternodes.find(in.prevout);
            if (it != mapMasternodes.end()) {
                it->second->SetSpent();
            }
        }
    }
}

static bool canScheduleMN(bool fFilterSigTime, const MasternodeRef& mn, int minProtocol,
                          int nMnCount, int nBlockHeight)
{
    // check protocol version
    if (mn->protocolVersion < minProtocol) return false;

    // it's in the list (up to 8 entries ahead of current block to allow propagation) -- so let's skip it
    if (masternodePayments.IsScheduled(*mn, nBlockHeight)) return false;

    // it's too new, wait for a cycle
    if (fFilterSigTime && mn->sigTime + (nMnCount * 2.6 * 60) > GetAdjustedTime()) return false;

    // make sure it has as many confirmations as there are masternodes
    if (pcoinsTip->GetCoinDepthAtHeight(mn->vin.prevout, nBlockHeight) < nMnCount) return false;

    return true;
}

//
// Deterministically select the oldest/best masternode to pay on the network
//
MasternodeRef CMasternodeMan::GetNextMasternodeInQueueForPayment(int nBlockHeight, bool fFilterSigTime, int& nCount, const CBlockIndex* pChainTip) const
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        LogPrintf("%s: ERROR - called after legacy system disabled\n", __func__);
        return nullptr;
    }

    AssertLockNotHeld(cs_main);
    const CBlockIndex* BlockReading = (pChainTip == nullptr ? GetChainTip() : pChainTip);
    if (!BlockReading) return nullptr;

    MasternodeRef pBestMasternode = nullptr;
    std::vector<std::pair<int64_t, MasternodeRef> > vecMasternodeLastPaid;

    /*
        Make a vector with all of the last paid times
    */
    int minProtocol = ActiveProtocol();
    int count_enabled = CountEnabled();
    {
        LOCK(cs);
        for (const auto& it : mapMasternodes) {
            if (!it.second->IsEnabled()) continue;
            if (canScheduleMN(fFilterSigTime, it.second, minProtocol, count_enabled, nBlockHeight)) {
                vecMasternodeLastPaid.emplace_back(SecondsSincePayment(it.second, count_enabled, BlockReading), it.second);
            }
        }
    }
    // Add deterministic masternodes to the vector
    if (deterministicMNManager->IsDIP3Enforced()) {
        CDeterministicMNList mnList = deterministicMNManager->GetListAtChainTip();
        mnList.ForEachMN(true, [&](const CDeterministicMNCPtr& dmn) {
            const MasternodeRef mn = MakeMasternodeRefForDMN(dmn);
            if (canScheduleMN(fFilterSigTime, mn, minProtocol, count_enabled, nBlockHeight)) {
                vecMasternodeLastPaid.emplace_back(SecondsSincePayment(mn, count_enabled, BlockReading), mn);
            }
        });
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    nCount = (int)vecMasternodeLastPaid.size();

    //when the network is in the process of upgrading, don't penalize nodes that recently restarted
<<<<<<< HEAD
    if (fFilterSigTime && nCount < nMnCount / 3) return GetNextMasternodeInQueueForPayment(nBlockHeight, false, nCount);

    // Sort them high to low
    sort(vecMasternodeLastPaid.rbegin(), vecMasternodeLastPaid.rend(), CompareLastPaid());
=======
    if (fFilterSigTime && nCount < count_enabled / 3) return GetNextMasternodeInQueueForPayment(nBlockHeight, false, nCount, BlockReading);

    // Sort them high to low
    sort(vecMasternodeLastPaid.rbegin(), vecMasternodeLastPaid.rend(), CompareScoreMN());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // Look at 1/10 of the oldest nodes (by last payment), calculate their scores and pay the best one
    //  -- This doesn't look at who is being paid in the +8-10 blocks, allowing for double payments very rarely
    //  -- 1/100 payments should be a double payment on mainnet - (1/(3000/10))*2
    //  -- (chance per block * chances before IsScheduled will fire)
<<<<<<< HEAD
    int nTenthNetwork = CountEnabled() / 10;
    int nCountTenth = 0;
    uint256 nHigh = 0;
    for (PAIRTYPE(int64_t, CTxIn) & s : vecMasternodeLastPaid) {
        CMasternode* pmn = Find(s.second);
        if (!pmn) break;

        uint256 n = pmn->CalculateScore(1, nBlockHeight - 100);
=======
    int nTenthNetwork = count_enabled / 10;
    int nCountTenth = 0;
    arith_uint256 nHigh = ARITH_UINT256_ZERO;
    const uint256& hash = GetHashAtHeight(nBlockHeight - 101);
    for (const auto& s: vecMasternodeLastPaid) {
        const MasternodeRef pmn = s.second;
        if (!pmn) break;

        const arith_uint256& n = pmn->CalculateScore(hash);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (n > nHigh) {
            nHigh = n;
            pBestMasternode = pmn;
        }
        nCountTenth++;
        if (nCountTenth >= nTenthNetwork) break;
    }
    return pBestMasternode;
}

<<<<<<< HEAD
CMasternode* CMasternodeMan::FindRandomNotInVec(std::vector<CTxIn>& vecToExclude, int protocolVersion)
{
    LOCK(cs);

    protocolVersion = protocolVersion == -1 ? masternodePayments.GetMinMasternodePaymentsProto() : protocolVersion;

    int nCountEnabled = CountEnabled(protocolVersion);
    LogPrint("masternode", "CMasternodeMan::FindRandomNotInVec - nCountEnabled - vecToExclude.size() %d\n", nCountEnabled - vecToExclude.size());
    if (nCountEnabled - vecToExclude.size() < 1) return NULL;

    int rand = GetRandInt(nCountEnabled - vecToExclude.size());
    LogPrint("masternode", "CMasternodeMan::FindRandomNotInVec - rand %d\n", rand);
    bool found;

    for (CMasternode& mn : vMasternodes) {
        if (mn.protocolVersion < protocolVersion || !mn.IsEnabled()) continue;
        found = false;
        for (CTxIn& usedVin : vecToExclude) {
            if (mn.vin.prevout == usedVin.prevout) {
                found = true;
                break;
            }
        }
        if (found) continue;
        if (--rand < 1) {
            return &mn;
        }
    }

    return NULL;
}

CMasternode* CMasternodeMan::GetCurrentMasterNode(int mod, int64_t nBlockHeight, int minProtocol)
{
    int64_t score = 0;
    CMasternode* winner = NULL;

    // scan for winner
    for (CMasternode& mn : vMasternodes) {
        mn.Check();
        if (mn.protocolVersion < minProtocol || !mn.IsEnabled()) continue;

        // calculate the score for each Masternode
        uint256 n = mn.CalculateScore(mod, nBlockHeight);
        int64_t n2 = n.GetCompact(false);

        // determine the winner
        if (n2 > score) {
            score = n2;
            winner = &mn;
        }
    }

    return winner;
}

int CMasternodeMan::GetMasternodeRank(const CTxIn& vin, int64_t nBlockHeight, int minProtocol, bool fOnlyActive)
{
    std::vector<pair<int64_t, CTxIn> > vecMasternodeScores;
    int64_t nMasternode_Min_Age = MN_WINNER_MINIMUM_AGE;
    int64_t nMasternode_Age = 0;

    //make sure we know about this block
    uint256 hash = 0;
    if (!GetBlockHash(hash, nBlockHeight)) return -1;

    // scan for winner
    for (CMasternode& mn : vMasternodes) {
        if (mn.protocolVersion < minProtocol) {
            LogPrint("masternode","Skipping Masternode with obsolete version %d\n", mn.protocolVersion);
            continue;                                                       // Skip obsolete versions
        }

        if (fOnlyActive) {
            mn.Check();
            if (!mn.IsEnabled()) continue;
        }
        uint256 n = mn.CalculateScore(1, nBlockHeight);
        int64_t n2 = n.GetCompact(false);

        vecMasternodeScores.push_back(make_pair(n2, mn.vin));
    }

    sort(vecMasternodeScores.rbegin(), vecMasternodeScores.rend(), CompareScoreTxIn());

    int rank = 0;
    for (PAIRTYPE(int64_t, CTxIn) & s : vecMasternodeScores) {
=======
MasternodeRef CMasternodeMan::GetCurrentMasterNode(const uint256& hash) const
{
    int minProtocol = ActiveProtocol();
    int64_t score = 0;
    MasternodeRef winner = nullptr;

    // scan for winner
    for (const auto& it : mapMasternodes) {
        const MasternodeRef& mn = it.second;
        if (mn->protocolVersion < minProtocol || !mn->IsEnabled()) continue;
        // calculate the score of the masternode
        const int64_t n = mn->CalculateScore(hash).GetCompact(false);
        // determine the winner
        if (n > score) {
            score = n;
            winner = mn;
        }
    }

    // scan also dmns
    if (deterministicMNManager->IsDIP3Enforced()) {
        auto mnList = deterministicMNManager->GetListAtChainTip();
        mnList.ForEachMN(true, [&](const CDeterministicMNCPtr& dmn) {
            const MasternodeRef mn = MakeMasternodeRefForDMN(dmn);
            // calculate the score of the masternode
            const int64_t n = mn->CalculateScore(hash).GetCompact(false);
            // determine the winner
            if (n > score) {
                score = n;
                winner = mn;
            }
        });
    }

    return winner;
}

std::vector<std::pair<MasternodeRef, int>> CMasternodeMan::GetMnScores(int nLast) const
{
    std::vector<std::pair<MasternodeRef, int>> ret;
    int nChainHeight = GetBestHeight();
    if (nChainHeight < 0) return ret;

    for (int nHeight = nChainHeight - nLast; nHeight < nChainHeight + 20; nHeight++) {
        const uint256& hash = GetHashAtHeight(nHeight - 101);
        MasternodeRef winner = GetCurrentMasterNode(hash);
        if (winner) {
            ret.emplace_back(winner, nHeight);
        }
    }
    return ret;
}

int CMasternodeMan::GetMasternodeRank(const CTxIn& vin, int64_t nBlockHeight) const
{
    const uint256& hash = GetHashAtHeight(nBlockHeight - 1);
    // height outside range
    if (hash == UINT256_ZERO) return -1;

    // scan for winner
    int minProtocol = ActiveProtocol();
    std::vector<std::pair<int64_t, CTxIn> > vecMasternodeScores;
    {
        LOCK(cs);
        for (const auto& it : mapMasternodes) {
            const MasternodeRef& mn = it.second;
            if (!mn->IsEnabled()) {
                continue; // Skip not enabled
            }
            if (mn->protocolVersion < minProtocol) {
                LogPrint(BCLog::MASTERNODE,"Skipping Masternode with obsolete version %d\n", mn->protocolVersion);
                continue; // Skip obsolete versions
            }
            if (sporkManager.IsSporkActive(SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT) &&
                    GetAdjustedTime() - mn->sigTime < MN_WINNER_MINIMUM_AGE) {
                continue; // Skip masternodes younger than (default) 1 hour
            }
            vecMasternodeScores.emplace_back(mn->CalculateScore(hash).GetCompact(false), mn->vin);
        }
    }

    // scan also dmns
    if (deterministicMNManager->IsDIP3Enforced()) {
        auto mnList = deterministicMNManager->GetListAtChainTip();
        mnList.ForEachMN(true, [&](const CDeterministicMNCPtr& dmn) {
            const MasternodeRef mn = MakeMasternodeRefForDMN(dmn);
            vecMasternodeScores.emplace_back(mn->CalculateScore(hash).GetCompact(false), mn->vin);
        });
    }

    sort(vecMasternodeScores.rbegin(), vecMasternodeScores.rend(), CompareScoreMN());

    int rank = 0;
    for (std::pair<int64_t, CTxIn> & s : vecMasternodeScores) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        rank++;
        if (s.second.prevout == vin.prevout) {
            return rank;
        }
    }

    return -1;
}

<<<<<<< HEAD
std::vector<pair<int, CMasternode> > CMasternodeMan::GetMasternodeRanks(int64_t nBlockHeight, int minProtocol)
{
    std::vector<pair<int64_t, CMasternode> > vecMasternodeScores;
    std::vector<pair<int, CMasternode> > vecMasternodeRanks;

    //make sure we know about this block
    uint256 hash = 0;
    if (!GetBlockHash(hash, nBlockHeight)) return vecMasternodeRanks;

    // scan for winner
    for (CMasternode& mn : vMasternodes) {
        mn.Check();

        if (mn.protocolVersion < minProtocol) continue;

        if (!mn.IsEnabled()) {
            vecMasternodeScores.push_back(make_pair(9999, mn));
            continue;
        }

        uint256 n = mn.CalculateScore(1, nBlockHeight);
        int64_t n2 = n.GetCompact(false);

        vecMasternodeScores.push_back(make_pair(n2, mn));
    }

    sort(vecMasternodeScores.rbegin(), vecMasternodeScores.rend(), CompareScoreMN());

    int rank = 0;
    for (PAIRTYPE(int64_t, CMasternode) & s : vecMasternodeScores) {
        rank++;
        vecMasternodeRanks.push_back(make_pair(rank, s.second));
    }

    return vecMasternodeRanks;
}

CMasternode* CMasternodeMan::GetMasternodeByRank(int nRank, int64_t nBlockHeight, int minProtocol, bool fOnlyActive)
{
    std::vector<pair<int64_t, CTxIn> > vecMasternodeScores;

    // scan for winner
    for (CMasternode& mn : vMasternodes) {
        if (mn.protocolVersion < minProtocol) continue;
        if (fOnlyActive) {
            mn.Check();
            if (!mn.IsEnabled()) continue;
        }

        uint256 n = mn.CalculateScore(1, nBlockHeight);
        int64_t n2 = n.GetCompact(false);

        vecMasternodeScores.push_back(make_pair(n2, mn.vin));
    }

    sort(vecMasternodeScores.rbegin(), vecMasternodeScores.rend(), CompareScoreTxIn());

    int rank = 0;
    for (PAIRTYPE(int64_t, CTxIn) & s : vecMasternodeScores) {
        rank++;
        if (rank == nRank) {
            return Find(s.second);
        }
    }

    return NULL;
}

void CMasternodeMan::ProcessMasternodeConnections()
{
    //we don't care about this for regtest
    if (Params().NetworkID() == CBaseChainParams::REGTEST) return;

    LOCK(cs_vNodes);
    for (CNode* pnode : vNodes) {
        if (pnode->fObfuScationMaster) {
            if (obfuScationPool.pSubmittedToMasternode != NULL && pnode->addr == obfuScationPool.pSubmittedToMasternode->addr) continue;
            LogPrint("masternode","Closing Masternode connection peer=%i \n", pnode->GetId());
            pnode->fObfuScationMaster = false;
            pnode->Release();
        }
    }
}

void CMasternodeMan::ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv)
{
    if (fLiteMode) return; //disable all Obfuscation/Masternode related functionality
    if (!masternodeSync.IsBlockchainSynced()) return;

    LOCK(cs_process_message);
    if (strCommand == "mnb") { //Masternode Broadcast
        CMasternodeBroadcast mnb;
        vRecv >> mnb;

        if (mapSeenMasternodeBroadcast.count(mnb.GetHash())) { //seen
            masternodeSync.AddedMasternodeList(mnb.GetHash());
            return;
        }
        mapSeenMasternodeBroadcast.insert(make_pair(mnb.GetHash(), mnb));

        int nDoS = 0;
        if (!mnb.CheckAndUpdate(nDoS)) {
            if (nDoS > 0)
                Misbehaving(pfrom->GetId(), nDoS);

            //failed
            return;
        }

        // make sure the vout that was signed is related to the transaction that spawned the Masternode
        //  - this is expensive, so it's only done once per Masternode
        if (!obfuScationSigner.IsVinAssociatedWithPubkey(mnb.vin, mnb.pubKeyCollateralAddress)) {
            LogPrintf("CMasternodeMan::ProcessMessage() : mnb - Got mismatched pubkey and vin\n");
            Misbehaving(pfrom->GetId(), 33);
            return;
        }

        // make sure it's still unspent
        //  - this is checked later by .check() in many places and by ThreadCheckObfuScationPool()
        if (mnb.CheckInputsAndAdd(nDoS)) {
            // use this as a peer
            addrman.Add(CAddress(mnb.addr), pfrom->addr, 2 * 60 * 60);
            masternodeSync.AddedMasternodeList(mnb.GetHash());
        } else {
            LogPrint("masternode","mnb - Rejected Masternode entry %s\n", mnb.vin.prevout.hash.ToString());

            if (nDoS > 0)
                Misbehaving(pfrom->GetId(), nDoS);
        }
    }

    else if (strCommand == "mnp") { //Masternode Ping
        CMasternodePing mnp;
        vRecv >> mnp;

        LogPrint("masternode", "mnp - Masternode ping, vin: %s\n", mnp.vin.prevout.hash.ToString());

        if (mapSeenMasternodePing.count(mnp.GetHash())) return; //seen
        mapSeenMasternodePing.insert(make_pair(mnp.GetHash(), mnp));

        int nDoS = 0;
        if (mnp.CheckAndUpdate(nDoS)) return;

        if (nDoS > 0) {
            // if anything significant failed, mark that node
            Misbehaving(pfrom->GetId(), nDoS);
        } else {
            // if nothing significant failed, search existing Masternode list
            CMasternode* pmn = Find(mnp.vin);
            // if it's known, don't ask for the mnb, just return
            if (pmn != NULL) return;
        }

        // something significant is broken or mn is unknown,
        // we might have to ask for a masternode entry once
        AskForMN(pfrom, mnp.vin);

    } else if (strCommand == "dseg") { //Get Masternode list or specific entry

        CTxIn vin;
        vRecv >> vin;
        if (vin == CTxIn()) { //only should ask for this once
            //local network
            bool isLocal = (pfrom->addr.IsRFC1918() || pfrom->addr.IsLocal());

            if (!isLocal && Params().NetworkID() == CBaseChainParams::MAIN) {
                std::map<CNetAddr, int64_t>::iterator i = mAskedUsForMasternodeList.find(pfrom->addr);
                if (i != mAskedUsForMasternodeList.end()) {
                    int64_t t = (*i).second;
                    if (GetTime() < t) {
                        LogPrintf("CMasternodeMan::ProcessMessage() : dseg - peer already asked me for the list\n");
                        Misbehaving(pfrom->GetId(), 34);
                        return;
                    }
                }
                int64_t askAgain = GetTime() + MASTERNODES_DSEG_SECONDS;
                mAskedUsForMasternodeList[pfrom->addr] = askAgain;
            }
        } //else, asking for a specific node which is ok


        int nInvCount = 0;
        for (CMasternode& mn : vMasternodes) {
            if (mn.addr.IsRFC1918()) continue; //local network

            if (mn.IsEnabled()) {
                LogPrint("masternode", "dseg - Sending Masternode entry - %s \n", mn.vin.prevout.hash.ToString());
                if (vin == CTxIn() || vin == mn.vin) {
                    CMasternodeBroadcast mnb = CMasternodeBroadcast(mn);
                    uint256 hash = mnb.GetHash();
                    pfrom->PushInventory(CInv(MSG_MASTERNODE_ANNOUNCE, hash));
                    nInvCount++;

                    if (!mapSeenMasternodeBroadcast.count(hash)) mapSeenMasternodeBroadcast.insert(make_pair(hash, mnb));

                    if (vin == mn.vin) {
                        LogPrint("masternode", "\ndseg - Sent 1 Masternode entry to peer %i\n", pfrom->GetId());
                        return;
                    }
                }
            }
        }

        if (vin == CTxIn()) {
            pfrom->PushMessage("ssc", MASTERNODE_SYNC_LIST, nInvCount);
            LogPrint("masternode", "\ndseg - Sent %d Masternode entries to peer %i\n", nInvCount, pfrom->GetId());
        }
    }
    /*
     * IT'S SAFE TO REMOVE THIS IN FURTHER VERSIONS
     * AFTER MIGRATION TO V12 IS DONE
     */

    // Light version for OLD MASSTERNODES - fake pings, no self-activation
    else if (strCommand == "dsee") { //ObfuScation Election Entry
        if (!masternodeSync.IsBlockchainSynced()) return;
        CTxIn vin;
        CService addr;
        CPubKey pubkey;
        CPubKey pubkey2;
        vector<unsigned char> vchSig;
        int64_t sigTime;
        int count;
        int current;
        int64_t lastUpdated;
        int protocolVersion;
        CScript donationAddress;
        int donationPercentage;
        std::string strMessage;

        vRecv >> vin >> addr >> vchSig >> sigTime >> pubkey >> pubkey2 >> count >> current >> lastUpdated >> protocolVersion >> donationAddress >> donationPercentage;
        // make sure signature isn't in the future (past is OK)
        if (sigTime > GetAdjustedTime() + 60 * 60) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - Signature rejected, too far into the future %s\n", vin.prevout.hash.ToString());
            Misbehaving(pfrom->GetId(), 1);
            return;
        }

        if (!VerifyShnorrKeyImageTxIn(vin, GetTxInSignatureHash(vin))) {
            LogPrint("masternode","dsee - Shnorr Signature rejected: %s\n", vin.prevout.hash.ToString());
            return;
        }

        //verify the value if 5K PRCY
        COutPoint prevout = vin.prevout;
        CTransaction prev;
        uint256 bh;
        if (!GetTransaction(prevout.hash, prev, bh, true)) {
            LogPrint("masternode","dsee - failed to read transaction hash %s\n", vin.prevout.hash.ToString());
            return;
        }

        CTxOut out = prev.vout[prevout.n];
        CPubKey sharedSec(vin.encryptionKey.begin(), vin.encryptionKey.end());
        CKey mask;
        CAmount amount;
        ECDHInfo::Decode(out.maskValue.mask.begin(), out.maskValue.amount.begin(), sharedSec, mask, amount);

        std::vector<unsigned char> commitment;
        CWallet::CreateCommitment(mask.begin(), amount, commitment);
        if (commitment != out.commitment) {
            LogPrint("masternode","dsee - decoded masternode collateralization not match %s\n", vin.prevout.hash.ToString());
            return;
        }

        if (amount != Params().MNCollateralAmt()) {
            LogPrint("masternode","dsee - masternode collateralization not equal to 5K %s\n", vin.prevout.hash.ToString());
            return;
        }

        std::string vchPubKey(pubkey.begin(), pubkey.end());
        std::string vchPubKey2(pubkey2.begin(), pubkey2.end());
        std::string ss = addr.ToString();
        CDataStream ser(SER_NETWORK, protocolVersion);
        ser << ss << sigTime << pubkey << pubkey2 << protocolVersion;
        /*strMessage = Hash(BEGIN(ss), END(ss),
                        BEGIN(sigTime), END(sigTime),
                        pubkey.begin(), pubkey.end(),
                        pubkey2.begin(), pubkey2.end(),
                        BEGIN(protocolVersion), END(protocolVersion)).GetHex();*/
        strMessage = HexStr(ser.begin(), ser.end());

        if (protocolVersion < masternodePayments.GetMinMasternodePaymentsProto()) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - ignoring outdated Masternode %s protocol version %d < %d\n", vin.prevout.hash.ToString(), protocolVersion, masternodePayments.GetMinMasternodePaymentsProto());
            Misbehaving(pfrom->GetId(), 1);
            return;
        }

        CScript pubkeyScript;
        pubkeyScript = GetScriptForDestination(pubkey);

        if ((pubkey.IsCompressed() && pubkeyScript.size() != 35) || (!pubkey.IsCompressed() && pubkeyScript.size() != 67)) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - pubkey the wrong size\n");
            Misbehaving(pfrom->GetId(), 100);
            return;
        }

        CScript pubkeyScript2;
        pubkeyScript2 = GetScriptForDestination(pubkey2);
        if ((pubkey2.IsCompressed() && pubkeyScript2.size() != 35) || (!pubkey2.IsCompressed() && pubkeyScript2.size() != 67)) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - pubkey2 the wrong size\n");
            Misbehaving(pfrom->GetId(), 100);
            return;
        }

        if (!vin.scriptSig.empty()) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - Ignore Not Empty ScriptSig %s\n", vin.prevout.hash.ToString());
            Misbehaving(pfrom->GetId(), 100);
            return;
        }

        std::string errorMessage = "";
        CScript sc = GetScriptForDestination(pubkey);
        if (!obfuScationSigner.VerifyMessage(pubkey, vchSig, strMessage, errorMessage)) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - Got bad Masternode address signature\n");;
            Misbehaving(pfrom->GetId(), 100);
            return;
        }

        if (Params().NetworkID() == CBaseChainParams::MAIN) {
            if (addr.GetPort() != 59682) return;
        } else if (addr.GetPort() == 59682)
            return;

        //search existing Masternode list, this is where we update existing Masternodes with new dsee broadcasts
        CMasternode* pmn = this->Find(vin);
        if (pmn != NULL) {
            // count == -1 when it's a new entry
            //   e.g. We don't want the entry relayed/time updated when we're syncing the list
            // mn.pubkey = pubkey, IsVinAssociatedWithPubkey is validated once below,
            //   after that they just need to match
            if (count == -1 && pmn->pubKeyCollateralAddress == pubkey && (GetAdjustedTime() - pmn->nLastDsee > MASTERNODE_MIN_MNB_SECONDS)) {
                if (pmn->protocolVersion > GETHEADERS_VERSION && sigTime - pmn->lastPing.sigTime < MASTERNODE_MIN_MNB_SECONDS) {
                    return;
                }
                if (pmn->nLastDsee < sigTime) { //take the newest entry
                    LogPrint("masternode", "masternode: dsee - Got updated entry for %s\n", vin.prevout.hash.ToString());
                    if (pmn->protocolVersion < GETHEADERS_VERSION) {
                        pmn->pubKeyMasternode = pubkey2;
                        pmn->sigTime = sigTime;
                        pmn->sig = vchSig;
                        pmn->protocolVersion = protocolVersion;
                        pmn->addr = addr;
                        //fake ping
                        pmn->lastPing = CMasternodePing(vin);
                        pmn->vin.masternodeStealthAddress = vin.masternodeStealthAddress;
                    }
                    pmn->nLastDsee = sigTime;
                    pmn->Check();
                    if (pmn->IsEnabled()) {
                        TRY_LOCK(cs_vNodes, lockNodes);
                        if (!lockNodes) return;
                        for (CNode* pnode : vNodes)
                            if (pnode->nVersion >= masternodePayments.GetMinMasternodePaymentsProto())
                                pnode->PushMessage("dsee", vin, addr, vchSig, sigTime, pubkey, pubkey2, count, current, lastUpdated, protocolVersion, donationAddress, donationPercentage);
                    }
                }
            }

            return;
        }
        static std::map<COutPoint, CPubKey> mapSeenDsee;
        if (mapSeenDsee.count(vin.prevout) && mapSeenDsee[vin.prevout] == pubkey) {
            LogPrint("masternode", "dsee - already seen this vin %s\n", vin.prevout.ToString());
            return;
        }
        mapSeenDsee.insert(make_pair(vin.prevout, pubkey));
        // make sure the vout that was signed is related to the transaction that spawned the Masternode
        //  - this is expensive, so it's only done once per Masternode
        if (!obfuScationSigner.IsVinAssociatedWithPubkey(vin, pubkey)) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dsee - Got mismatched pubkey and vin\n");
            Misbehaving(pfrom->GetId(), 100);
            return;
        }


        LogPrint("masternode", "dsee - Got NEW OLD Masternode entry %s\n", vin.prevout.hash.ToString());

        // make sure it's still unspent
        //  - this is checked later by .check() in many places and by ThreadCheckObfuScationPool()

        CValidationState state;

        bool fAcceptable = !IsKeyImageSpend1(vin.keyImage.GetHex(), uint256());

        if (fAcceptable) {
            if (GetInputAge(vin) < MASTERNODE_MIN_CONFIRMATIONS) {
                LogPrintf("CMasternodeMan::ProcessMessage() : dsee - Input must have least %d confirmations\n", MASTERNODE_MIN_CONFIRMATIONS);
                Misbehaving(pfrom->GetId(), 20);
                return;
            }

            // use this as a peer
            addrman.Add(CAddress(addr), pfrom->addr, 2 * 60 * 60);
            // add Masternode
            CMasternode mn = CMasternode();
            mn.addr = addr;
            mn.vin = vin;
            mn.pubKeyCollateralAddress = pubkey;
            mn.sig = vchSig;
            mn.sigTime = sigTime;
            mn.pubKeyMasternode = pubkey2;
            mn.protocolVersion = protocolVersion;
            // fake ping
            mn.lastPing = CMasternodePing(vin);
            mn.Check(true);
            // add v11 masternodes, v12 should be added by mnb only
            LogPrint("masternode", "dsee - Accepted OLD Masternode entry %i %i\n", count, current);
            Add(mn);

            if (mn.IsEnabled()) {
                TRY_LOCK(cs_vNodes, lockNodes);
                if (!lockNodes) return;
                for (CNode* pnode : vNodes)
                    if (pnode->nVersion >= masternodePayments.GetMinMasternodePaymentsProto())
                        pnode->PushMessage("dsee", vin, addr, vchSig, sigTime, pubkey, pubkey2, count, current, lastUpdated, protocolVersion, donationAddress, donationPercentage);
            }
        } else {
            LogPrint("masternode","dsee - Rejected Masternode entry %s\n", vin.prevout.hash.ToString());

            int nDoS = 0;
            if (state.IsInvalid(nDoS)) {
                LogPrint("masternode","dsee - %i %s was not accepted into the memory pool\n",
                    pfrom->GetId(), pfrom->cleanSubVer.c_str());
                if (nDoS > 0)
                    Misbehaving(pfrom->GetId(), nDoS);
            }
        }
    }

    else if (strCommand == "dseep") { //ObfuScation Election Entry Ping

        CTxIn vin;
        vector<unsigned char> vchSig;
        int64_t sigTime;
        bool stop;
        vRecv >> vin >> vchSig >> sigTime >> stop;

        if (!VerifyShnorrKeyImageTxIn(vin, GetTxInSignatureHash(vin))) {
            LogPrintf("dsee - Shnorr Signature rejected: %s\n", vin.prevout.hash.ToString());
            return;
        }

        if (sigTime > GetAdjustedTime() + 60 * 60) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dseep - Signature rejected, too far into the future %s\n", vin.prevout.hash.ToString());
            Misbehaving(pfrom->GetId(), 1);
            return;
        }

        if (sigTime <= GetAdjustedTime() - 60 * 60) {
            LogPrintf("CMasternodeMan::ProcessMessage() : dseep - Signature rejected, too far into the past %s - %d %d \n", vin.prevout.hash.ToString(), sigTime, GetAdjustedTime());
            Misbehaving(pfrom->GetId(), 1);
            return;
        }

        std::map<COutPoint, int64_t>::iterator i = mWeAskedForMasternodeListEntry.find(vin.prevout);
        if (i != mWeAskedForMasternodeListEntry.end()) {
            int64_t t = (*i).second;
            if (GetTime() < t) return; // we've asked recently
        }

        // see if we have this Masternode
        CMasternode* pmn = this->Find(vin);
        if (pmn != NULL && pmn->protocolVersion >= masternodePayments.GetMinMasternodePaymentsProto()) {
            // take this only if it's newer
            if (sigTime - pmn->nLastDseep > MASTERNODE_MIN_MNP_SECONDS) {
                std::string ss = pmn->addr.ToString();
                HEX_DATA_STREAM_PROTOCOL(PROTOCOL_VERSION) << pmn->addr.ToString() << sigTime << stop;
                std::string strMessage = HEX_STR(ser);

                std::string errorMessage = "";
                if (!obfuScationSigner.VerifyMessage(pmn->pubKeyMasternode, vchSig, strMessage, errorMessage)) {
                    LogPrint("masternode", "\ndseep - Got bad Masternode address signature %s \n", vin.prevout.hash.ToString());
                    return;
                }

                // fake ping for v11 masternodes, ignore for v12
                if (pmn->protocolVersion < GETHEADERS_VERSION) pmn->lastPing = CMasternodePing(vin);
                pmn->nLastDseep = sigTime;
                pmn->Check();
                if (pmn->IsEnabled()) {
                    TRY_LOCK(cs_vNodes, lockNodes);
                    if (!lockNodes) return;
                    LogPrint("masternode", "\ndseep - relaying %s \n", vin.prevout.hash.ToString());
                    for (CNode* pnode : vNodes)
                        if (pnode->nVersion >= masternodePayments.GetMinMasternodePaymentsProto())
                            pnode->PushMessage("dseep", vin, vchSig, sigTime, stop);
                }
            }
            return;
        }

        AskForMN(pfrom, vin);
    }

    /*
     * END OF "REMOVE"
     */
}

void CMasternodeMan::Remove(CTxIn vin)
{
    LOCK(cs);

    vector<CMasternode>::iterator it = vMasternodes.begin();
    while (it != vMasternodes.end()) {
        if ((*it).vin == vin) {
            LogPrint("masternode", "CMasternodeMan: Removing Masternode %s - %i now\n", (*it).vin.prevout.hash.ToString(), size() - 1);
            vMasternodes.erase(it);
            break;
        }
        ++it;
    }
}

void CMasternodeMan::UpdateMasternodeList(CMasternodeBroadcast mnb)
{
    LOCK(cs);
    mapSeenMasternodePing.insert(std::make_pair(mnb.lastPing.GetHash(), mnb.lastPing));
    mapSeenMasternodeBroadcast.insert(std::make_pair(mnb.GetHash(), mnb));

    LogPrint("masternode","CMasternodeMan::UpdateMasternodeList -- masternode=%s\n", mnb.vin.prevout.ToStringShort());

    CMasternode* pmn = Find(mnb.vin);
    if (pmn == NULL) {
        CMasternode mn(mnb);
        if (Add(mn)) {
            masternodeSync.AddedMasternodeList(mnb.GetHash());
        }
    } else if (pmn->UpdateFromNewBroadcast(mnb)) {
        masternodeSync.AddedMasternodeList(mnb.GetHash());
    }
}

std::string CMasternodeMan::ToString() const
{
    std::ostringstream info;

    info << "Masternodes: " << (int)vMasternodes.size() << ", peers who asked us for Masternode list: " << (int)mAskedUsForMasternodeList.size() << ", peers we asked for Masternode list: " << (int)mWeAskedForMasternodeList.size() << ", entries in Masternode list we asked for: " << (int)mWeAskedForMasternodeListEntry.size() << ", nDsqCount: " << (int)nDsqCount;

    return info.str();
=======
std::vector<std::pair<int64_t, MasternodeRef>> CMasternodeMan::GetMasternodeRanks(int nBlockHeight) const
{
    std::vector<std::pair<int64_t, MasternodeRef>> vecMasternodeScores;
    const uint256& hash = GetHashAtHeight(nBlockHeight - 1);
    // height outside range
    if (hash == UINT256_ZERO) return vecMasternodeScores;
    {
        LOCK(cs);
        // scan for winner
        for (const auto& it : mapMasternodes) {
            const MasternodeRef mn = it.second;
            const uint32_t score = mn->IsEnabled() ? mn->CalculateScore(hash).GetCompact(false) : 9999;

            vecMasternodeScores.emplace_back(score, mn);
        }
    }
    // scan also dmns
    if (deterministicMNManager->IsDIP3Enforced()) {
        auto mnList = deterministicMNManager->GetListAtChainTip();
        mnList.ForEachMN(false, [&](const CDeterministicMNCPtr& dmn) {
            const MasternodeRef mn = MakeMasternodeRefForDMN(dmn);
            const uint32_t score = dmn->IsPoSeBanned() ? 9999 : mn->CalculateScore(hash).GetCompact(false);

            vecMasternodeScores.emplace_back(score, mn);
        });
    }
    sort(vecMasternodeScores.rbegin(), vecMasternodeScores.rend(), CompareScoreMN());
    return vecMasternodeScores;
}

bool CMasternodeMan::CheckInputs(CMasternodeBroadcast& mnb, int nChainHeight, int& nDoS)
{
    // incorrect ping or its sigTime
    if(mnb.lastPing.IsNull() || !mnb.lastPing.CheckAndUpdate(nDoS, false, true)) {
        return false;
    }

    // search existing Masternode list
    CMasternode* pmn = Find(mnb.vin.prevout);
    if (pmn != nullptr) {
        // nothing to do here if we already know about this masternode and it's enabled
        if (pmn->IsEnabled()) return true;
        // if it's not enabled, remove old MN first and continue
        else
            mnodeman.Remove(pmn->vin.prevout);
    }

    const Coin& collateralUtxo = pcoinsTip->AccessCoin(mnb.vin.prevout);
    if (collateralUtxo.IsSpent()) {
        LogPrint(BCLog::MASTERNODE,"mnb - vin %s spent\n", mnb.vin.prevout.ToString());
        return false;
    }

    // Check collateral value
    if (collateralUtxo.out.nValue != Params().GetConsensus().nMNCollateralAmt) {
        LogPrint(BCLog::MASTERNODE,"mnb - invalid amount for mnb collateral %s\n", mnb.vin.prevout.ToString());
        nDoS = 33;
        return false;
    }

    // Check collateral association with mnb pubkey
    CScript payee = GetScriptForDestination(mnb.pubKeyCollateralAddress.GetID());
    if (collateralUtxo.out.scriptPubKey != payee) {
        LogPrint(BCLog::MASTERNODE,"mnb - collateral %s not associated with mnb pubkey\n", mnb.vin.prevout.ToString());
        nDoS = 33;
        return false;
    }

    LogPrint(BCLog::MASTERNODE, "mnb - Accepted Masternode entry\n");
    const int utxoHeight = (int) collateralUtxo.nHeight;
    int collateralUtxoDepth = nChainHeight - utxoHeight + 1;
    if (collateralUtxoDepth < MasternodeCollateralMinConf()) {
        LogPrint(BCLog::MASTERNODE,"mnb - Input must have at least %d confirmations\n", MasternodeCollateralMinConf());
        // maybe we miss few blocks, let this mnb to be checked again later
        mapSeenMasternodeBroadcast.erase(mnb.GetHash());
        masternodeSync.mapSeenSyncMNB.erase(mnb.GetHash());
        return false;
    }

    // verify that sig time is legit in past
    // should be at least not earlier than block when 1000 PIV tx got MASTERNODE_MIN_CONFIRMATIONS
    CBlockIndex* pConfIndex = WITH_LOCK(cs_main, return chainActive[utxoHeight + MasternodeCollateralMinConf() - 1]); // block where tx got MASTERNODE_MIN_CONFIRMATIONS
    if (pConfIndex->GetBlockTime() > mnb.sigTime) {
        LogPrint(BCLog::MASTERNODE,"mnb - Bad sigTime %d for Masternode %s (%i conf block is at %d)\n",
                 mnb.sigTime, mnb.vin.prevout.hash.ToString(), MasternodeCollateralMinConf(), pConfIndex->GetBlockTime());
        return false;
    }

    // Good input
    return true;
}

int CMasternodeMan::ProcessMNBroadcast(CNode* pfrom, CMasternodeBroadcast& mnb)
{
    const uint256& mnbHash = mnb.GetHash();
    if (mapSeenMasternodeBroadcast.count(mnbHash)) { //seen
        masternodeSync.AddedMasternodeList(mnbHash);
        return 0;
    }

    int chainHeight = GetBestHeight();
    int nDoS = 0;
    if (!mnb.CheckAndUpdate(nDoS)) {
        return nDoS;
    }

    // make sure it's still unspent
    if (!CheckInputs(mnb, chainHeight, nDoS)) {
        return nDoS; // error set internally
    }

    // now that did the mnb checks, can add it.
    mapSeenMasternodeBroadcast.emplace(mnbHash, mnb);

    // All checks performed, add it
    LogPrint(BCLog::MASTERNODE,"%s - Got NEW Masternode entry - %s - %lli \n", __func__,
             mnb.vin.prevout.hash.ToString(), mnb.sigTime);
    CMasternode mn(mnb);
    if (!Add(mn)) {
        LogPrint(BCLog::MASTERNODE, "%s - Rejected Masternode entry %s\n", __func__,
                 mnb.vin.prevout.hash.ToString());
        return 0;
    }

    // if it matches our MN pubkey, then we've been remotely activated
    if (mnb.pubKeyMasternode == activeMasternode.pubKeyMasternode && mnb.protocolVersion == PROTOCOL_VERSION) {
        activeMasternode.EnableHotColdMasterNode(mnb.vin, mnb.addr);
    }

    // Relay only if we are synchronized and if the mnb address is not local.
    // Makes no sense to relay MNBs to the peers from where we are syncing them.
    bool isLocal = (mnb.addr.IsRFC1918() || mnb.addr.IsLocal()) && !Params().IsRegTestNet();
    if (!isLocal && masternodeSync.IsSynced()) mnb.Relay();

    // Add it as a peer
    g_connman->AddNewAddress(CAddress(mnb.addr, NODE_NETWORK), pfrom->addr, 2 * 60 * 60);

    // Update sync status
    masternodeSync.AddedMasternodeList(mnbHash);

    // All good
    return 0;
}

int CMasternodeMan::ProcessMNPing(CNode* pfrom, CMasternodePing& mnp)
{
    const uint256& mnpHash = mnp.GetHash();
    if (mapSeenMasternodePing.count(mnpHash)) return 0; //seen

    int nDoS = 0;
    if (mnp.CheckAndUpdate(nDoS)) return 0;

    if (nDoS > 0) {
        // if anything significant failed, mark that node
        return nDoS;
    } else {
        // if nothing significant failed, search existing Masternode list
        CMasternode* pmn = Find(mnp.vin.prevout);
        // if it's known, don't ask for the mnb, just return
        if (pmn != NULL) return 0;
    }

    // something significant is broken or mn is unknown,
    // we might have to ask for the mn entry (while we aren't syncing).
    if (masternodeSync.IsSynced()) {
        AskForMN(pfrom, mnp.vin);
    }

    // All good
    return 0;
}

void CMasternodeMan::BroadcastInvMN(CMasternode* mn, CNode* pfrom)
{
    CMasternodeBroadcast mnb = CMasternodeBroadcast(*mn);
    const uint256& hash = mnb.GetHash();
    pfrom->PushInventory(CInv(MSG_MASTERNODE_ANNOUNCE, hash));

    // Add to mapSeenMasternodeBroadcast in case that isn't there for some reason.
    if (!mapSeenMasternodeBroadcast.count(hash)) mapSeenMasternodeBroadcast.emplace(hash, mnb);
}

int CMasternodeMan::ProcessGetMNList(CNode* pfrom, CTxIn& vin)
{
    // Single MN request
    if (!vin.IsNull()) {
        CMasternode* mn = Find(vin.prevout);
        if (!mn || !mn->IsEnabled()) return 0; // Nothing to return.

        // Relay the MN.
        BroadcastInvMN(mn, pfrom);
        LogPrint(BCLog::MASTERNODE, "dseg - Sent 1 Masternode entry to peer %i\n", pfrom->GetId());
        return 0;
    }

    // Check if the node asked for mn list sync before.
    bool isLocal = (pfrom->addr.IsRFC1918() || pfrom->addr.IsLocal());
    if (!isLocal) {
        auto itAskedUsMNList = mAskedUsForMasternodeList.find(pfrom->addr);
        if (itAskedUsMNList != mAskedUsForMasternodeList.end()) {
            int64_t t = (*itAskedUsMNList).second;
            if (GetTime() < t) {
                LogPrintf("CMasternodeMan::ProcessMessage() : dseg - peer already asked me for the list\n");
                return 20;
            }
        }
        int64_t askAgain = GetTime() + MASTERNODES_REQUEST_SECONDS;
        mAskedUsForMasternodeList[pfrom->addr] = askAgain;
    }

    int nInvCount = 0;
    {
        LOCK(cs);
        for (auto& it : mapMasternodes) {
            MasternodeRef& mn = it.second;
            if (mn->addr.IsRFC1918()) continue; //local network
            if (mn->IsEnabled()) {
                LogPrint(BCLog::MASTERNODE, "dseg - Sending Masternode entry - %s \n", mn->vin.prevout.hash.ToString());
                BroadcastInvMN(mn.get(), pfrom);
                nInvCount++;
            }
        }
    }

    g_connman->PushMessage(pfrom, CNetMsgMaker(pfrom->GetSendVersion()).Make(NetMsgType::SYNCSTATUSCOUNT, MASTERNODE_SYNC_LIST, nInvCount));
    LogPrint(BCLog::MASTERNODE, "dseg - Sent %d Masternode entries to peer %i\n", nInvCount, pfrom->GetId());

    // All good
    return 0;
}

bool CMasternodeMan::ProcessMessage(CNode* pfrom, std::string& strCommand, CDataStream& vRecv, int& dosScore)
{
    dosScore = ProcessMessageInner(pfrom, strCommand, vRecv);
    return dosScore == 0;
}

int CMasternodeMan::ProcessMessageInner(CNode* pfrom, std::string& strCommand, CDataStream& vRecv)
{
    if (!masternodeSync.IsBlockchainSynced()) return 0;

    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        LogPrint(BCLog::MASTERNODE, "%s: skip obsolete message %s\n", __func__, strCommand);
        return 0;
    }

    LOCK(cs_process_message);

    if (strCommand == NetMsgType::MNBROADCAST) {
        CMasternodeBroadcast mnb;
        vRecv >> mnb;
        {
            // Clear inv request
            LOCK(cs_main);
            g_connman->RemoveAskFor(mnb.GetHash(), MSG_MASTERNODE_ANNOUNCE);
        }
        return ProcessMNBroadcast(pfrom, mnb);

    } else if (strCommand == NetMsgType::MNBROADCAST2) {
        CMasternodeBroadcast mnb;
        OverrideStream<CDataStream> s(&vRecv, vRecv.GetType(), vRecv.GetVersion() | ADDRV2_FORMAT);
        s >> mnb;
        {
            // Clear inv request
            LOCK(cs_main);
            g_connman->RemoveAskFor(mnb.GetHash(), MSG_MASTERNODE_ANNOUNCE);
        }

        // For now, let's not process mnb2 with pre-BIP155 node addr format.
        if (mnb.addr.IsAddrV1Compatible()) {
            LogPrint(BCLog::MASTERNODE, "%s: mnb2 with pre-BIP155 node addr format rejected\n", __func__);
            return 30;
        }

        return ProcessMNBroadcast(pfrom, mnb);

    } else if (strCommand == NetMsgType::MNPING) {
        //Masternode Ping
        CMasternodePing mnp;
        vRecv >> mnp;
        LogPrint(BCLog::MNPING, "mnp - Masternode ping, vin: %s\n", mnp.vin.prevout.hash.ToString());
        {
            // Clear inv request
            LOCK(cs_main);
            g_connman->RemoveAskFor(mnp.GetHash(), MSG_MASTERNODE_PING);
        }
        return ProcessMNPing(pfrom, mnp);

    } else if (strCommand == NetMsgType::GETMNLIST) {
        //Get Masternode list or specific entry
        CTxIn vin;
        vRecv >> vin;
        return ProcessGetMNList(pfrom, vin);
    }
    // Nothing to report
    return 0;
}

void CMasternodeMan::Remove(const COutPoint& collateralOut)
{
    LOCK(cs);
    const auto it = mapMasternodes.find(collateralOut);
    if (it != mapMasternodes.end()) {
        mapMasternodes.erase(it);
    }
}

void CMasternodeMan::UpdateMasternodeList(CMasternodeBroadcast& mnb)
{
    // Skip after legacy obsolete. !TODO: remove when transition to DMN is complete
    if (deterministicMNManager->LegacyMNObsolete()) {
        LogPrint(BCLog::MASTERNODE, "Removing all legacy mn due to SPORK 21\n");
        return;
    }

    mapSeenMasternodePing.emplace(mnb.lastPing.GetHash(), mnb.lastPing);
    mapSeenMasternodeBroadcast.emplace(mnb.GetHash(), mnb);
    masternodeSync.AddedMasternodeList(mnb.GetHash());

    LogPrint(BCLog::MASTERNODE,"%s -- masternode=%s\n", __func__, mnb.vin.prevout.ToString());

    CMasternode* pmn = Find(mnb.vin.prevout);
    if (pmn == NULL) {
        CMasternode mn(mnb);
        Add(mn);
    } else {
        pmn->UpdateFromNewBroadcast(mnb);
    }
}

int64_t CMasternodeMan::SecondsSincePayment(const MasternodeRef& mn, int count_enabled, const CBlockIndex* BlockReading) const
{
    int64_t sec = (GetAdjustedTime() - GetLastPaid(mn, count_enabled, BlockReading));
    int64_t month = 60 * 60 * 24 * 30;
    if (sec < month) return sec; //if it's less than 30 days, give seconds

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << mn->vin;
    ss << mn->sigTime;
    const arith_uint256& hash = UintToArith256(ss.GetHash());

    // return some deterministic value for unknown/unpaid but force it to be more than 30 days old
    return month + hash.GetCompact(false);
}

int64_t CMasternodeMan::GetLastPaid(const MasternodeRef& mn, int count_enabled, const CBlockIndex* BlockReading) const
{
    if (BlockReading == nullptr) return false;

    const CScript& mnpayee = mn->GetPayeeScript();

    CHashWriter ss(SER_GETHASH, PROTOCOL_VERSION);
    ss << mn->vin;
    ss << mn->sigTime;
    const uint256& hash = ss.GetHash();

    // use a deterministic offset to break a tie -- 2.5 minutes
    int64_t nOffset = UintToArith256(hash).GetCompact(false) % 150;

    int max_depth = count_enabled * 1.25;
    for (int n = 0; n < max_depth; n++) {
        const auto& it = masternodePayments.mapMasternodeBlocks.find(BlockReading->nHeight);
        if (it != masternodePayments.mapMasternodeBlocks.end()) {
            // Search for this payee, with at least 2 votes. This will aid in consensus
            // allowing the network to converge on the same payees quickly, then keep the same schedule.
            if (it->second.HasPayeeWithVotes(mnpayee, 2))
                return BlockReading->nTime + nOffset;
        }
        BlockReading = BlockReading->pprev;

        if (BlockReading == nullptr || BlockReading->nHeight <= 0) {
            break;
        }
    }

    return 0;
}

std::string CMasternodeMan::ToString() const
{
    std::ostringstream info;
    info << "Masternodes: " << (int)mapMasternodes.size()
         << ", peers who asked us for Masternode list: " << (int)mAskedUsForMasternodeList.size()
         << ", peers we asked for Masternode list: " << (int)mWeAskedForMasternodeList.size()
         << ", entries in Masternode list we asked for: " << (int)mWeAskedForMasternodeListEntry.size();
    return info.str();
}

void CMasternodeMan::CacheBlockHash(const CBlockIndex* pindex)
{
    cvLastBlockHashes.Set(pindex->nHeight, pindex->GetBlockHash());
}

void CMasternodeMan::UncacheBlockHash(const CBlockIndex* pindex)
{
    cvLastBlockHashes.Set(pindex->nHeight, UINT256_ZERO);
}

uint256 CMasternodeMan::GetHashAtHeight(int nHeight) const
{
    // return zero if outside bounds
    if (nHeight < 0) {
        LogPrint(BCLog::MASTERNODE, "%s: Negative height. Returning 0\n",  __func__);
        return UINT256_ZERO;
    }
    int nCurrentHeight = GetBestHeight();
    if (nHeight > nCurrentHeight) {
        LogPrint(BCLog::MASTERNODE, "%s: height %d over current height %d. Returning 0\n",
                __func__, nHeight, nCurrentHeight);
        return UINT256_ZERO;
    }

    if (nHeight > nCurrentHeight - (int) CACHED_BLOCK_HASHES) {
        // Use cached hash
        return cvLastBlockHashes.Get(nHeight);
    } else {
        // Use chainActive
        LOCK(cs_main);
        return chainActive[nHeight]->GetBlockHash();
    }
}

bool CMasternodeMan::IsWithinDepth(const uint256& nHash, int depth) const
{
    // Sanity checks
    if (nHash.IsNull()) {
        return error("%s: Called with null hash\n", __func__);
    }
    if (depth < 0 || (unsigned) depth >= CACHED_BLOCK_HASHES) {
        return error("%s: Invalid depth %d. Cached block hashes: %d\n", __func__, depth, CACHED_BLOCK_HASHES);
    }
    // Check last depth blocks to find one with matching hash
    const int nCurrentHeight = GetBestHeight();
    int nStopHeight = std::max(0, nCurrentHeight - depth);
    for (int i = nCurrentHeight; i >= nStopHeight; i--) {
        if (GetHashAtHeight(i) == nHash)
            return true;
    }
    return false;
}

void ThreadCheckMasternodes()
{
    // Make this thread recognisable as the wallet flushing thread
    util::ThreadRename("pivx-masternodeman");
    LogPrintf("Masternodes thread started\n");

    unsigned int c = 0;

    try {
        // first clean up stale masternode payments data
        masternodePayments.CleanPaymentList(mnodeman.CheckAndRemove(), mnodeman.GetBestHeight());

        // Startup-only, clean any stored seen MN broadcast with an invalid service that
        // could have been invalidly stored on a previous release
        auto itSeenMNB = mnodeman.mapSeenMasternodeBroadcast.begin();
        while (itSeenMNB != mnodeman.mapSeenMasternodeBroadcast.end()) {
            if (!itSeenMNB->second.addr.IsValid()) {
                itSeenMNB = mnodeman.mapSeenMasternodeBroadcast.erase(itSeenMNB);
            } else {
                itSeenMNB++;
            }
        }

        while (true) {

            if (ShutdownRequested()) {
                break;
            }

            MilliSleep(1000);
            boost::this_thread::interruption_point();

            // try to sync from all available nodes, one step at a time
            masternodeSync.Process();

            if (masternodeSync.IsBlockchainSynced()) {
                c++;

                // check if we should activate or ping every few minutes,
                // start right after sync is considered to be done
                if (c % (MasternodePingSeconds()/2) == 0)
                    activeMasternode.ManageStatus();

                if (c % (MasternodePingSeconds()/5) == 0) {
                    masternodePayments.CleanPaymentList(mnodeman.CheckAndRemove(), mnodeman.GetBestHeight());
                }
            }
        }
    } catch (boost::thread_interrupted&) {
        // nothing, thread interrupted.
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
