// Copyright (c) 2011-2013 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "walletmodeltransaction.h"

#include "wallet/wallet.h"

WalletModelTransaction::WalletModelTransaction(const QList<SendCoinsRecipient>& recipients) : recipients(recipients),
                                                                                              walletTransaction(0),
                                                                                              keyChange(0),
                                                                                              fee(0)
<<<<<<< HEAD
{
    walletTransaction = new CWalletTx();
}
=======
{ }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

WalletModelTransaction::~WalletModelTransaction()
{
    delete keyChange;
<<<<<<< HEAD
    delete walletTransaction;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

QList<SendCoinsRecipient> WalletModelTransaction::getRecipients()
{
    return recipients;
}

<<<<<<< HEAD
CWalletTx* WalletModelTransaction::getTransaction()
=======
CTransactionRef& WalletModelTransaction::getTransaction()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    return walletTransaction;
}

unsigned int WalletModelTransaction::getTransactionSize()
{
<<<<<<< HEAD
    return (!walletTransaction ? 0 : (::GetSerializeSize(*(CTransaction*)walletTransaction, SER_NETWORK, PROTOCOL_VERSION)));
=======
    return (!walletTransaction ? 0 : (::GetSerializeSize(*walletTransaction, PROTOCOL_VERSION)));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CAmount WalletModelTransaction::getTransactionFee()
{
    return fee;
}

void WalletModelTransaction::setTransactionFee(const CAmount& newFee)
{
    fee = newFee;
}

<<<<<<< HEAD
CAmount WalletModelTransaction::getTotalTransactionAmount()
{
    CAmount totalTransactionAmount = 0;
   Q_FOREACH (const SendCoinsRecipient& rcp, recipients) {
=======
unsigned int WalletModelTransaction::subtractFeeFromRecipents() const
{
    unsigned int count = 0;
    for (const SendCoinsRecipient& rcp : recipients) {
        if (rcp.fSubtractFee) count++;
    }
    return count;
}

CAmount WalletModelTransaction::getTotalTransactionAmount()
{
    CAmount totalTransactionAmount = 0;
    for (const SendCoinsRecipient& rcp : recipients) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        totalTransactionAmount += rcp.amount;
    }
    return totalTransactionAmount;
}

<<<<<<< HEAD
void WalletModelTransaction::newPossibleKeyChange(CWallet* wallet)
{
    keyChange = new CReserveKey(wallet);
=======
CReserveKey* WalletModelTransaction::newPossibleKeyChange(CWallet* wallet)
{
    keyChange = new CReserveKey(wallet);
    return keyChange;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CReserveKey* WalletModelTransaction::getPossibleKeyChange()
{
    return keyChange;
}
