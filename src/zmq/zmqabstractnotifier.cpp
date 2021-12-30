// Copyright (c) 2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "zmqabstractnotifier.h"
<<<<<<< HEAD
#include "util.h"
=======
#include "util/system.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e


CZMQAbstractNotifier::~CZMQAbstractNotifier()
{
    assert(!psocket);
}

bool CZMQAbstractNotifier::NotifyBlock(const CBlockIndex * /*CBlockIndex*/)
{
    return true;
}

bool CZMQAbstractNotifier::NotifyTransaction(const CTransaction &/*transaction*/)
{
    return true;
}

<<<<<<< HEAD
bool CZMQAbstractNotifier::NotifyTransactionLock(const CTransaction &/*transaction*/)
{
    return true;
}
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
