// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
#define BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H

#include "validationinterface.h"
#include <string>
#include <map>
<<<<<<< HEAD
=======
#include <list>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

class CBlockIndex;
class CZMQAbstractNotifier;

class CZMQNotificationInterface : public CValidationInterface
{
public:
    virtual ~CZMQNotificationInterface();

<<<<<<< HEAD
    static CZMQNotificationInterface* CreateWithArguments(const std::map<std::string, std::string> &args);
=======
    static CZMQNotificationInterface* Create();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

protected:
    bool Initialize();
    void Shutdown();

    // CValidationInterface
<<<<<<< HEAD
    void SyncTransaction(const CTransaction &tx, const CBlock *pblock);
    void UpdatedBlockTip(const CBlockIndex *pindex);
    void NotifyTransactionLock(const CTransaction &tx);
=======
    void TransactionAddedToMempool(const CTransactionRef& tx) override;
    void BlockConnected(const std::shared_ptr<const CBlock>& pblock, const CBlockIndex* pindexConnected) override;
    void BlockDisconnected(const std::shared_ptr<const CBlock>& pblock, const uint256& blockHash, int nBlockHeight, int64_t blockTime) override;
    void UpdatedBlockTip(const CBlockIndex *pindexNew, const CBlockIndex *pindexFork, bool fInitialDownload) override;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

private:
    CZMQNotificationInterface();

    void *pcontext;
    std::list<CZMQAbstractNotifier*> notifiers;
};

#endif // BITCOIN_ZMQ_ZMQNOTIFICATIONINTERFACE_H
