// Copyright (c) 2011-2014 The Bitcoin Core developers
<<<<<<< HEAD
=======
// Copyright (c) 2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for denial-of-service detection/prevention code
//

<<<<<<< HEAD


#include "keystore.h"
#include "main.h"
#include "net.h"
#include "poa.h"
#include "script/sign.h"
#include "serialize.h"
#include "util.h"

#include <stdint.h>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/test/unit_test.hpp>

// Tests this internal-to-main.cpp method:
extern bool AddOrphanTx(const CTransaction& tx, NodeId peer);
extern void EraseOrphansFor(NodeId peer);
extern unsigned int LimitOrphanTxSize(unsigned int nMaxOrphans);
struct COrphanTx {
    CTransaction tx;
    NodeId fromPeer;
};
extern std::map<uint256, COrphanTx> mapOrphanTransactions;
extern std::map<uint256, std::set<uint256> > mapOrphanTransactionsByPrev;
=======
#include "test/test_pivx.h"

#include "arith_uint256.h"
#include "keystore.h"
#include "net_processing.h"
#include "net.h"
#include "pubkey.h"
#include "pow.h"
#include "script/sign.h"
#include "serialize.h"
#include "util/system.h"
#include "validation.h"

#include <stdint.h>

#include <boost/test/unit_test.hpp>

// Tests this internal-to-validation.cpp method:
extern bool AddOrphanTx(const CTransactionRef& tx, NodeId peer);
extern void EraseOrphansFor(NodeId peer);
extern unsigned int LimitOrphanTxSize(unsigned int nMaxOrphans);
struct COrphanTx {
    CTransactionRef tx;
    NodeId fromPeer;
    int64_t nTimeExpire;
};
extern RecursiveMutex g_cs_orphans;
extern std::map<uint256, COrphanTx> mapOrphanTransactions GUARDED_BY(g_cs_orphans);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

CService ip(uint32_t i)
{
    struct in_addr s;
    s.s_addr = i;
    return CService(CNetAddr(s), Params().GetDefaultPort());
}

<<<<<<< HEAD
#ifdef DISABLE_FAILED_TEST
BOOST_AUTO_TEST_SUITE(DoS_tests)

BOOST_AUTO_TEST_CASE(DoS_banning)
{
    CNode::ClearBanned();
    CAddress addr1(ip(0xa0b0c001));
    CNode dummyNode1(INVALID_SOCKET, addr1, "", true);
    dummyNode1.nVersion = 1;
    Misbehaving(dummyNode1.GetId(), 100); // Should get banned
    SendMessages(&dummyNode1, false);
    BOOST_CHECK(CNode::IsBanned(addr1));
    BOOST_CHECK(!CNode::IsBanned(ip(0xa0b0c001|0x0000ff00))); // Different IP, not banned

    CAddress addr2(ip(0xa0b0c002));
    CNode dummyNode2(INVALID_SOCKET, addr2, "", true);
    dummyNode2.nVersion = 1;
    Misbehaving(dummyNode2.GetId(), 50);
    SendMessages(&dummyNode2, false);
    BOOST_CHECK(!CNode::IsBanned(addr2)); // 2 not banned yet...
    BOOST_CHECK(CNode::IsBanned(addr1));  // ... but 1 still should be
    Misbehaving(dummyNode2.GetId(), 50);
    SendMessages(&dummyNode2, false);
    BOOST_CHECK(CNode::IsBanned(addr2));
=======
static NodeId id = 0;

BOOST_FIXTURE_TEST_SUITE(DoS_tests, TestingSetup)

void misbehave(NodeId id, int value) {
    LOCK(cs_main);
    Misbehaving(id, value); // Should get banned
}

BOOST_AUTO_TEST_CASE(DoS_banning)
{
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    CAddress addr1(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode1(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr1, 0, 0, "", true);
    dummyNode1.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode1);
    dummyNode1.nVersion = 1;
    dummyNode1.fSuccessfullyConnected = true;
    misbehave(dummyNode1.GetId(), 100); // Should get banned
    {
        LOCK2(cs_main, dummyNode1.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode1, interruptDummy);
    }
    BOOST_CHECK(connman->IsBanned(addr1));
    BOOST_CHECK(!connman->IsBanned(ip(0xa0b0c001|0x0000ff00))); // Different IP, not banned

    CAddress addr2(ip(0xa0b0c002), NODE_NONE);
    CNode dummyNode2(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr2, 1, 1, "", true);
    dummyNode2.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode2);
    dummyNode2.nVersion = 1;
    dummyNode2.fSuccessfullyConnected = true;
    misbehave(dummyNode2.GetId(), 50);
    {
        LOCK2(cs_main, dummyNode2.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode2, interruptDummy);
    }
    BOOST_CHECK(!connman->IsBanned(addr2)); // 2 not banned yet...
    BOOST_CHECK(connman->IsBanned(addr1));  // ... but 1 still should be
    misbehave(dummyNode2.GetId(), 50);
    {
        LOCK2(cs_main, dummyNode2.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode2, interruptDummy);
    }
    BOOST_CHECK(connman->IsBanned(addr2));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

BOOST_AUTO_TEST_CASE(DoS_banscore)
{
<<<<<<< HEAD
    CNode::ClearBanned();
    mapArgs["-banscore"] = "111"; // because 11 is my favorite number
    CAddress addr1(ip(0xa0b0c001));
    CNode dummyNode1(INVALID_SOCKET, addr1, "", true);
    dummyNode1.nVersion = 1;
    Misbehaving(dummyNode1.GetId(), 100);
    SendMessages(&dummyNode1, false);
    BOOST_CHECK(!CNode::IsBanned(addr1));
    Misbehaving(dummyNode1.GetId(), 10);
    SendMessages(&dummyNode1, false);
    BOOST_CHECK(!CNode::IsBanned(addr1));
    Misbehaving(dummyNode1.GetId(), 1);
    SendMessages(&dummyNode1, false);
    BOOST_CHECK(CNode::IsBanned(addr1));
    mapArgs.erase("-banscore");
=======
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    gArgs.ForceSetArg("-banscore", "111"); // because 11 is my favorite number
    CAddress addr1(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode1(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr1, 3, 1, "", true);
    dummyNode1.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode1);
    dummyNode1.nVersion = 1;
    dummyNode1.fSuccessfullyConnected = true;
    misbehave(dummyNode1.GetId(), 100);
    {
        LOCK2(cs_main, dummyNode1.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode1, interruptDummy);
    }
    BOOST_CHECK(!connman->IsBanned(addr1));
    misbehave(dummyNode1.GetId(), 10);
    {
        LOCK2(cs_main, dummyNode1.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode1, interruptDummy);
    }
    BOOST_CHECK(!connman->IsBanned(addr1));
    misbehave(dummyNode1.GetId(), 1);
    {
        LOCK2(cs_main, dummyNode1.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode1, interruptDummy);
    }
    BOOST_CHECK(connman->IsBanned(addr1));
    gArgs.ForceSetArg("-banscore", std::to_string(DEFAULT_BANSCORE_THRESHOLD));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

BOOST_AUTO_TEST_CASE(DoS_bantime)
{
<<<<<<< HEAD
    CNode::ClearBanned();
    int64_t nStartTime = GetTime();
    SetMockTime(nStartTime); // Overrides future calls to GetTime()

    CAddress addr(ip(0xa0b0c001));
    CNode dummyNode(INVALID_SOCKET, addr, "", true);
    dummyNode.nVersion = 1;

    Misbehaving(dummyNode.GetId(), 100);
    SendMessages(&dummyNode, false);
    BOOST_CHECK(CNode::IsBanned(addr));

    SetMockTime(nStartTime+60*60);
    BOOST_CHECK(CNode::IsBanned(addr));

    SetMockTime(nStartTime+60*60*24+1);
    BOOST_CHECK(!CNode::IsBanned(addr));
}

CTransaction RandomOrphan()
{
    std::map<uint256, COrphanTx>::iterator it;
    it = mapOrphanTransactions.lower_bound(insecure_rand256());
=======
    std::atomic<bool> interruptDummy(false);

    connman->ClearBanned();
    int64_t nStartTime = GetTime();
    SetMockTime(nStartTime); // Overrides future calls to GetTime()

    CAddress addr(ip(0xa0b0c001), NODE_NONE);
    CNode dummyNode(id++, NODE_NETWORK, 0, INVALID_SOCKET, addr, 4, 4, "", true);
    dummyNode.SetSendVersion(PROTOCOL_VERSION);
    peerLogic->InitializeNode(&dummyNode);
    dummyNode.nVersion = 1;
    dummyNode.fSuccessfullyConnected = true;

    misbehave(dummyNode.GetId(), 100);
    {
        LOCK2(cs_main, dummyNode.cs_sendProcessing);
        peerLogic->SendMessages(&dummyNode, interruptDummy);
    }
    BOOST_CHECK(connman->IsBanned(addr));

    SetMockTime(nStartTime+60*60);
    BOOST_CHECK(connman->IsBanned(addr));

    SetMockTime(nStartTime+60*60*24+1);
    BOOST_CHECK(!connman->IsBanned(addr));
}

CTransactionRef RandomOrphan()
{
    std::map<uint256, COrphanTx>::iterator it;
    LOCK2(cs_main, g_cs_orphans);
    it = mapOrphanTransactions.lower_bound(InsecureRand256());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (it == mapOrphanTransactions.end())
        it = mapOrphanTransactions.begin();
    return it->second.tx;
}

<<<<<<< HEAD
BOOST_AUTO_TEST_CASE(DoS_mapOrphans)
{
    CKey key;
    key.MakeNewKey(true);
=======
static void MakeNewKeyWithFastRandomContext(CKey& key)
{
    std::vector<unsigned char> keydata;
    keydata = insecure_rand_ctx.randbytes(32);
    key.Set(keydata.data(), keydata.data() + keydata.size(), /*fCompressedIn*/ true);
    assert(key.IsValid());
}

BOOST_AUTO_TEST_CASE(DoS_mapOrphans)
{
    // This test had non-deterministic coverage due to
    // randomly selected seeds.
    // This seed is chosen so that all branches of the function
    // ecdsa_signature_parse_der_lax are executed during this test.
    // Specifically branches that run only when an ECDSA
    // signature's R and S values have leading zeros.
    insecure_rand_ctx = FastRandomContext(ArithToUint256(arith_uint256(33)));

    CKey key;
    MakeNewKeyWithFastRandomContext(key);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CBasicKeyStore keystore;
    keystore.AddKey(key);

    // 50 orphan transactions:
    for (int i = 0; i < 50; i++)
    {
        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
<<<<<<< HEAD
        tx.vin[0].prevout.hash = insecure_rand256();
=======
        tx.vin[0].prevout.hash = InsecureRand256();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        tx.vin[0].scriptSig << OP_1;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());

<<<<<<< HEAD
        AddOrphanTx(tx, i);
=======
        AddOrphanTx(MakeTransactionRef(tx), i);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // ... and 50 that depend on other orphans:
    for (int i = 0; i < 50; i++)
    {
<<<<<<< HEAD
        CTransaction txPrev = RandomOrphan();
=======
        CTransactionRef txPrev = RandomOrphan();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        CMutableTransaction tx;
        tx.vin.resize(1);
        tx.vin[0].prevout.n = 0;
<<<<<<< HEAD
        tx.vin[0].prevout.hash = txPrev.GetHash();
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());
        SignSignature(keystore, txPrev, tx, 0);

        AddOrphanTx(tx, i);
=======
        tx.vin[0].prevout.hash = txPrev->GetHash();
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());
        SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL);

        AddOrphanTx(MakeTransactionRef(tx), i);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    // This really-big orphan should be ignored:
    for (int i = 0; i < 10; i++)
    {
<<<<<<< HEAD
        CTransaction txPrev = RandomOrphan();
=======
        CTransactionRef txPrev = RandomOrphan();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        CMutableTransaction tx;
        tx.vout.resize(1);
        tx.vout[0].nValue = 1*CENT;
        tx.vout[0].scriptPubKey = GetScriptForDestination(key.GetPubKey().GetID());
<<<<<<< HEAD
        tx.vin.resize(500);
        for (unsigned int j = 0; j < tx.vin.size(); j++)
        {
            tx.vin[j].prevout.n = j;
            tx.vin[j].prevout.hash = txPrev.GetHash();
        }
        SignSignature(keystore, txPrev, tx, 0);
=======
        tx.vin.resize(2777);
        for (unsigned int j = 0; j < tx.vin.size(); j++)
        {
            tx.vin[j].prevout.n = j;
            tx.vin[j].prevout.hash = txPrev->GetHash();
        }
        SignSignature(keystore, *txPrev, tx, 0, SIGHASH_ALL);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        // Re-use same signature for other inputs
        // (they don't have to be valid for this test)
        for (unsigned int j = 1; j < tx.vin.size(); j++)
            tx.vin[j].scriptSig = tx.vin[0].scriptSig;

<<<<<<< HEAD
        BOOST_CHECK(!AddOrphanTx(tx, i));
    }

=======
        BOOST_CHECK(!AddOrphanTx(MakeTransactionRef(tx), i));
    }

    LOCK2(cs_main, g_cs_orphans);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // Test EraseOrphansFor:
    for (NodeId i = 0; i < 3; i++)
    {
        size_t sizeBefore = mapOrphanTransactions.size();
        EraseOrphansFor(i);
        BOOST_CHECK(mapOrphanTransactions.size() < sizeBefore);
    }

    // Test LimitOrphanTxSize() function:
    LimitOrphanTxSize(40);
    BOOST_CHECK(mapOrphanTransactions.size() <= 40);
    LimitOrphanTxSize(10);
    BOOST_CHECK(mapOrphanTransactions.size() <= 10);
    LimitOrphanTxSize(0);
    BOOST_CHECK(mapOrphanTransactions.empty());
<<<<<<< HEAD
    BOOST_CHECK(mapOrphanTransactionsByPrev.empty());
}

BOOST_AUTO_TEST_SUITE_END()
#endif
=======
}

BOOST_AUTO_TEST_SUITE_END()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
