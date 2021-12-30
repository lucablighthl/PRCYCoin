// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zmqnotificationinterface.h"
#include "zmqpublishnotifier.h"

#include "version.h"
<<<<<<< HEAD
#include "main.h"
#include "streams.h"
#include "util.h"

void zmqError(const char *str)
{
    LogPrint("zmq", "zmq: Error: %s, errno=%s\n", str, zmq_strerror(errno));
=======
#include "streams.h"
#include "util/system.h"

void zmqError(const char *str)
{
    LogPrint(BCLog::ZMQ, "Error: %s, errno=%s\n", str, zmq_strerror(errno));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CZMQNotificationInterface::CZMQNotificationInterface() : pcontext(NULL)
{
}

CZMQNotificationInterface::~CZMQNotificationInterface()
{
    Shutdown();

    for (std::list<CZMQAbstractNotifier*>::iterator i=notifiers.begin(); i!=notifiers.end(); ++i)
    {
        delete *i;
    }
}

<<<<<<< HEAD
CZMQNotificationInterface* CZMQNotificationInterface::CreateWithArguments(const std::map<std::string, std::string> &args)
=======
CZMQNotificationInterface* CZMQNotificationInterface::Create()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    CZMQNotificationInterface* notificationInterface = NULL;
    std::map<std::string, CZMQNotifierFactory> factories;
    std::list<CZMQAbstractNotifier*> notifiers;

    factories["pubhashblock"] = CZMQAbstractNotifier::Create<CZMQPublishHashBlockNotifier>;
    factories["pubhashtx"] = CZMQAbstractNotifier::Create<CZMQPublishHashTransactionNotifier>;
<<<<<<< HEAD
    factories["pubhashtxlock"] = CZMQAbstractNotifier::Create<CZMQPublishHashTransactionLockNotifier>;
    factories["pubrawblock"] = CZMQAbstractNotifier::Create<CZMQPublishRawBlockNotifier>;
    factories["pubrawtx"] = CZMQAbstractNotifier::Create<CZMQPublishRawTransactionNotifier>;
    factories["pubrawtxlock"] = CZMQAbstractNotifier::Create<CZMQPublishRawTransactionLockNotifier>;

    for (std::map<std::string, CZMQNotifierFactory>::const_iterator i=factories.begin(); i!=factories.end(); ++i)
    {
        std::map<std::string, std::string>::const_iterator j = args.find("-zmq" + i->first);
        if (j!=args.end())
        {
            CZMQNotifierFactory factory = i->second;
            std::string address = j->second;
            CZMQAbstractNotifier *notifier = factory();
            notifier->SetType(i->first);
=======
    factories["pubrawblock"] = CZMQAbstractNotifier::Create<CZMQPublishRawBlockNotifier>;
    factories["pubrawtx"] = CZMQAbstractNotifier::Create<CZMQPublishRawTransactionNotifier>;

    for (const auto& entry : factories)
    {
        std::string arg("-zmq" + entry.first);
        if (gArgs.IsArgSet(arg))
        {
            CZMQNotifierFactory factory = entry.second;
            std::string address = gArgs.GetArg(arg, "");
            CZMQAbstractNotifier *notifier = factory();
            notifier->SetType(entry.first);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            notifier->SetAddress(address);
            notifiers.push_back(notifier);
        }
    }

    if (!notifiers.empty())
    {
        notificationInterface = new CZMQNotificationInterface();
        notificationInterface->notifiers = notifiers;

        if (!notificationInterface->Initialize())
        {
            delete notificationInterface;
            notificationInterface = NULL;
        }
    }

    return notificationInterface;
}

// Called at startup to conditionally set up ZMQ socket(s)
bool CZMQNotificationInterface::Initialize()
{
<<<<<<< HEAD
    LogPrint("zmq", "zmq: Initialize notification interface\n");
=======
    LogPrint(BCLog::ZMQ, "Initialize notification interface\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(!pcontext);

    pcontext = zmq_init(1);

    if (!pcontext)
    {
        zmqError("Unable to initialize context");
        return false;
    }

    std::list<CZMQAbstractNotifier*>::iterator i=notifiers.begin();
    for (; i!=notifiers.end(); ++i)
    {
        CZMQAbstractNotifier *notifier = *i;
        if (notifier->Initialize(pcontext))
        {
<<<<<<< HEAD
            LogPrint("zmq", "  Notifier %s ready (address = %s)\n", notifier->GetType(), notifier->GetAddress());
        }
        else
        {
            LogPrint("zmq", "  Notifier %s failed (address = %s)\n", notifier->GetType(), notifier->GetAddress());
=======
            LogPrint(BCLog::ZMQ, "Notifier %s ready (address = %s)\n", notifier->GetType(), notifier->GetAddress());
        }
        else
        {
            LogPrint(BCLog::ZMQ, "Notifier %s failed (address = %s)\n", notifier->GetType(), notifier->GetAddress());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            break;
        }
    }

    if (i!=notifiers.end())
    {
        return false;
    }

    return true;
}

// Called during shutdown sequence
void CZMQNotificationInterface::Shutdown()
{
<<<<<<< HEAD
    LogPrint("zmq", "zmq: Shutdown notification interface\n");
=======
    LogPrint(BCLog::ZMQ, "Shutdown notification interface\n");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (pcontext)
    {
        for (std::list<CZMQAbstractNotifier*>::iterator i=notifiers.begin(); i!=notifiers.end(); ++i)
        {
            CZMQAbstractNotifier *notifier = *i;
<<<<<<< HEAD
            LogPrint("zmq", "   Shutdown notifier %s at %s\n", notifier->GetType(), notifier->GetAddress());
=======
            LogPrint(BCLog::ZMQ, "Shutdown notifier %s at %s\n", notifier->GetType(), notifier->GetAddress());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            notifier->Shutdown();
        }
        zmq_ctx_destroy(pcontext);

        pcontext = 0;
    }
}

<<<<<<< HEAD
void CZMQNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindex)
{
    for (std::list<CZMQAbstractNotifier*>::iterator i = notifiers.begin(); i!=notifiers.end(); )
    {
        CZMQAbstractNotifier *notifier = *i;
        if (notifier->NotifyBlock(pindex))
=======
void CZMQNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload)
{
    if (fInitialDownload || pindexNew == pindexFork) // In IBD or blocks were disconnected without any new ones
        return;

    for (std::list<CZMQAbstractNotifier*>::iterator i = notifiers.begin(); i!=notifiers.end(); )
    {
        CZMQAbstractNotifier *notifier = *i;
        if (notifier->NotifyBlock(pindexNew))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        {
            i++;
        }
        else
        {
            notifier->Shutdown();
            i = notifiers.erase(i);
        }
    }
}

<<<<<<< HEAD
void CZMQNotificationInterface::SyncTransaction(const CTransaction &tx, const CBlock *pblock)
{
=======
void CZMQNotificationInterface::TransactionAddedToMempool(const CTransactionRef& ptx)
{
    // Used by BlockConnected and BlockDisconnected as well, because they're
    // all the same external callback.
    const CTransaction& tx = *ptx;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (std::list<CZMQAbstractNotifier*>::iterator i = notifiers.begin(); i!=notifiers.end(); )
    {
        CZMQAbstractNotifier *notifier = *i;
        if (notifier->NotifyTransaction(tx))
        {
            i++;
        }
        else
        {
            notifier->Shutdown();
            i = notifiers.erase(i);
        }
    }
}

<<<<<<< HEAD
void CZMQNotificationInterface::NotifyTransactionLock(const CTransaction &tx)
{
    for (std::list<CZMQAbstractNotifier*>::iterator i = notifiers.begin(); i!=notifiers.end(); )
    {
        CZMQAbstractNotifier *notifier = *i;
        if (notifier->NotifyTransactionLock(tx))
        {
            i++;
        }
        else
        {
            notifier->Shutdown();
            i = notifiers.erase(i);
        }
=======
void CZMQNotificationInterface::BlockConnected(const std::shared_ptr<const CBlock>& pblock, const CBlockIndex* pindexConnected)
{
    for (const CTransactionRef& ptx : pblock->vtx) {
        // Do a normal notify for each transaction added in the block
        TransactionAddedToMempool(ptx);
    }
}

void CZMQNotificationInterface::BlockDisconnected(const std::shared_ptr<const CBlock>& pblock, const uint256& blockHash, int nBlockHeight, int64_t blockTime)
{
    for (const CTransactionRef& ptx : pblock->vtx) {
        // Do a normal notify for each transaction removed in block disconnection
        TransactionAddedToMempool(ptx);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}
