// Copyright (c) 2011-2014 The Bitcoin developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "walletmodel.h"

<<<<<<< HEAD
#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "recentrequeststablemodel.h"
#include "transactionrecord.h"
#include "transactiontablemodel.h"
#include "init.h" // for ShutdownRequested(). Future: move to an interface wrapper

#include "base58.h"
#include "wallet/db.h"
#include "keystore.h"
#include "main.h"
#include "miner.h"
#include "sync.h"
#include "guiinterface.h"
#include "wallet/wallet.h"
#include "wallet/walletdb.h" // for BackupWallet
#include <stdint.h>
#include <regex>
#include <QDebug>
#include <QLocale>
#include <QSet>
#include <QTimer>
#include <QtCore>
#include <QtMath>
#include <stdint.h>
#include <QTextStream>

using namespace std;

WalletModel::WalletModel(CWallet* wallet, OptionsModel* optionsModel, QObject* parent) : QObject(parent), wallet(wallet), optionsModel(optionsModel), addressTableModel(0),
                                                                                         transactionTableModel(0),
                                                                                         recentRequestsTableModel(0),
                                                                                         cachedBalance(0), cachedUnconfirmedBalance(0), spendableBalance(0), cachedImmatureBalance(0), cachedWatchOnlyBalance(0),
                                                                                         cachedWatchUnconfBalance(0), cachedWatchImmatureBalance(0),
                                                                                         cachedEncryptionStatus(Unencrypted),
                                                                                         cachedNumBlocks(0), cachedTxLocks(0),
                                                                                         txTableModel(0)

=======
#include "budget/budgetproposal.h"
#include "interfaces/handler.h"
#include "sapling/key_io_sapling.h"
#include "sapling/sapling_operation.h"
#include "sapling/transaction_builder.h"
#include "shutdown.h"
#include "spork.h"
#include "wallet/fees.h"

#include "qt/addresstablemodel.h"
#include "qt/clientmodel.h"
#include "qt/guiconstants.h"
#include "qt/optionsmodel.h"
#include "qt/transactiontablemodel.h"
#include "qt/walletmodeltransaction.h"

#include <stdint.h>
#include <iostream>

#include <QtConcurrent/QtConcurrent>
#include <QSet>
#include <QTimer>
#include <utility>

// Util function
template <typename T>
static std::string toHexStr(const T& obj)
{
    CDataStream ss(SER_DISK, CLIENT_VERSION);
    ss << obj;
    return HexStr(ss);
}

WalletModel::WalletModel(CWallet* wallet, OptionsModel* optionsModel, QObject* parent) : QObject(parent), wallet(wallet), walletWrapper(*wallet),
                                                                                         optionsModel(optionsModel),
                                                                                         cachedEncryptionStatus(Unencrypted),
                                                                                         cachedNumBlocks(0)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    fHaveWatchOnly = wallet->HaveWatchOnly();
    fForceCheckBalanceChanged = false;

    addressTableModel = new AddressTableModel(wallet, this);
    transactionTableModel = new TransactionTableModel(wallet, this);
<<<<<<< HEAD
    recentRequestsTableModel = new RecentRequestsTableModel(wallet, this);

    // This timer will be fired repeatedly to update the balance
    pollTimer = new QTimer(this);
    connect(pollTimer, SIGNAL(timeout()), this, SLOT(pollBalanceChanged()));
    pollTimer->start(MODEL_UPDATE_DELAY);

    subscribeToCoreSignals();
}

=======
}

void WalletModel::init()
{
    transactionTableModel->init();
    // This timer will be fired repeatedly to update the balance
    pollTimer = new QTimer(this);
    connect(pollTimer, &QTimer::timeout, this, &WalletModel::pollBalanceChanged);
    pollTimer->start(MODEL_UPDATE_DELAY * 5);
    subscribeToCoreSignals();
}

WalletModel::~WalletModel()
{
    unsubscribeFromCoreSignals();
}

void WalletModel::resetWalletOptions(QSettings& settings)
{
    setWalletStakeSplitThreshold(CWallet::DEFAULT_STAKE_SPLIT_THRESHOLD);
    setWalletCustomFee(false, DEFAULT_TRANSACTION_FEE);
    optionsModel->setWalletDefaultOptions(settings, false);
}

bool WalletModel::isTestNetwork() const
{
    return Params().IsTestnet();
}

bool WalletModel::isRegTestNetwork() const
{
    return Params().IsRegTestNet();
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
bool WalletModel::isShutdownRequested()
{
    return ShutdownRequested();
}

<<<<<<< HEAD
WalletModel::~WalletModel()
{
    unsubscribeFromCoreSignals();
}

CAmount WalletModel::getBalance(const CCoinControl* coinControl) const
{
    if (coinControl) {

        {   
            return wallet->GetBalance();
        }
    }

    return wallet->GetBalance();
}

CAmount WalletModel::getUnconfirmedBalance() const
{
    return wallet->GetUnconfirmedBalance();
}

CAmount WalletModel::getSpendableBalance() const 
{
    return wallet->GetSpendableBalance();
}

CAmount WalletModel::getImmatureBalance() const
{
    return wallet->GetImmatureBalance();
=======
bool WalletModel::isColdStakingNetworkelyEnabled() const
{
    return !sporkManager.IsSporkActive(SPORK_19_COLDSTAKING_MAINTENANCE);
}

bool WalletModel::isSaplingInMaintenance() const
{
    return sporkManager.IsSporkActive(SPORK_20_SAPLING_MAINTENANCE);
}

bool WalletModel::isV6Enforced() const
{
    return Params().GetConsensus().NetworkUpgradeActive(cachedNumBlocks, Consensus::UPGRADE_V6_0);
}

bool WalletModel::isStakingStatusActive() const
{
    return wallet && wallet->pStakerStatus && wallet->pStakerStatus->IsActive();
}

bool WalletModel::isHDEnabled() const
{
    return wallet->IsHDEnabled();
}

bool WalletModel::isSaplingWalletEnabled() const
{
    return wallet->IsSaplingUpgradeEnabled();
}

bool WalletModel::upgradeWallet(std::string& upgradeError)
{
    // This action must be performed in a separate thread and not the main one.
    LOCK2(cs_main, wallet->cs_wallet);

    // Get version
    int prev_version = wallet->GetVersion();
    // Upgrade wallet's version
    wallet->SetMinVersion(FEATURE_LATEST);
    wallet->SetMaxVersion(FEATURE_LATEST);

    // Upgrade to HD
    return wallet->Upgrade(upgradeError, prev_version);
}

QString WalletModel::getWalletPath()
{
    return QString::fromStdString(wallet->GetPathToDBFile().string());
}

CAmount WalletModel::getBalance(const CCoinControl* coinControl, bool fIncludeDelegated, bool fUnlockedOnly, bool fIncludeShielded) const
{
    if (coinControl) {
        CAmount nBalance = 0;
        CWallet::AvailableCoinsFilter coinsFilter;
        coinsFilter.fIncludeDelegated = fIncludeDelegated;
        std::vector<COutput> vCoins;
        wallet->AvailableCoins(&vCoins, coinControl, coinsFilter);
        for (const COutput& out : vCoins) {
            bool fSkip = fUnlockedOnly && isLockedCoin(out.tx->GetHash(), out.i);
            if (out.fSpendable && !fSkip)
                nBalance += out.tx->tx->vout[out.i].nValue;
        }

        return nBalance;
    }

    return wallet->GetAvailableBalance(fIncludeDelegated, fIncludeShielded) - (fUnlockedOnly ? wallet->GetLockedCoins() : CAmount(0));
}

CAmount WalletModel::getUnlockedBalance(const CCoinControl* coinControl, bool fIncludeDelegated, bool fIncludeShielded) const
{
    return getBalance(coinControl, fIncludeDelegated, true, fIncludeShielded);
}

CAmount WalletModel::getMinColdStakingAmount() const
{
    return MIN_COLDSTAKING_AMOUNT;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CAmount WalletModel::getLockedBalance() const
{
    return wallet->GetLockedCoins();
}

bool WalletModel::haveWatchOnly() const
{
    return fHaveWatchOnly;
}

<<<<<<< HEAD
CAmount WalletModel::getWatchBalance() const
{
    return wallet->GetWatchOnlyBalance();
}

CAmount WalletModel::getWatchUnconfirmedBalance() const
{
    return wallet->GetUnconfirmedWatchOnlyBalance();
}

CAmount WalletModel::getWatchImmatureBalance() const
{
    return wallet->GetImmatureWatchOnlyBalance();
=======
CAmount WalletModel::getDelegatedBalance() const
{
    return wallet->GetDelegatedBalance();
}

bool WalletModel::isColdStaking() const
{
    // TODO: Complete me..
    return false;
}

void WalletModel::getAvailableP2CSCoins(std::vector<COutput>& vCoins) const
{
    return wallet->GetAvailableP2CSCoins(vCoins);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void WalletModel::updateStatus()
{
<<<<<<< HEAD
=======
    if (!wallet) return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    EncryptionStatus newEncryptionStatus = getEncryptionStatus();

    if (cachedEncryptionStatus != newEncryptionStatus)
        Q_EMIT encryptionStatusChanged(newEncryptionStatus);
}

<<<<<<< HEAD
bool IsImportingOrReindexing() {
    return fImporting || fReindex;
}

void WalletModel::pollBalanceChanged()
{
    if (wallet->walletUnlockCountStatus == 1) {
        Q_EMIT WalletUnlocked();
        wallet->walletUnlockCountStatus++;
    }

    // Wait a little bit more when the wallet is reindexing and/or importing, no need to lock cs_main so often.
    if (IsImportingOrReindexing()) {
        static uint8_t waitLonger = 0;
        waitLonger++;
        if (waitLonger < 10) // 10 seconds
=======
bool WalletModel::isWalletUnlocked() const
{
    if (!wallet) return false;
    EncryptionStatus status = getEncryptionStatus();
    return (status == Unencrypted || status == Unlocked);
}

bool WalletModel::isWalletLocked(bool fFullUnlocked) const
{
    if (!wallet) return false;
    EncryptionStatus status = getEncryptionStatus();
    return (status == Locked || (!fFullUnlocked && status == UnlockedForStaking));
}

static bool IsImportingOrReindexing()
{
    return fImporting || fReindex;
}

std::atomic<bool> processingBalance{false};

bool WalletModel::processBalanceChangeInternal()
{
    int chainHeight = getLastBlockProcessedNum();
    const uint256& blockHash = getLastBlockProcessed();

    // Avoid recomputing wallet balances unless a tx changed or BlockTip notification was received.
    // Extra note: This needs to be done before and after the update task trigger and execution because, as it runs concurrently,
    // there is no guarantee that the threadpool will execute the task right away.
    if (!fForceCheckBalanceChanged && m_cached_best_block_hash == blockHash) return false;

    // Try to get lock only if needed
    TRY_LOCK(wallet->cs_wallet, lockWallet);
    if (!lockWallet) return false;

    setfForceCheckBalanceChanged(false);

    // Balance and number of transactions might have changed
    setCacheNumBlocks(chainHeight);
    setCacheBlockHash(blockHash);
    checkBalanceChanged(getBalances());
    QMetaObject::invokeMethod(this, "updateTxModelData", Qt::QueuedConnection);
    QMetaObject::invokeMethod(this, "pollFinished", Qt::QueuedConnection);

    // Address in receive tab may have been used
    Q_EMIT notifyReceiveAddressChanged();
    return true;
}

static void processBalanceChange(WalletModel* walletModel)
{
    if (!walletModel || !walletModel->processBalanceChangeInternal()) {
        processingBalance = false;
    }
}

void WalletModel::pollBalanceChanged()
{
    if (processingBalance || !m_client_model) return;

    // Wait a little bit more when the wallet is reindexing and/or importing, no need to lock cs_main so often.
    if (IsImportingOrReindexing() || m_client_model->inInitialBlockDownload()) {
        static uint8_t waitLonger = 0;
        waitLonger++;
        if (waitLonger < 6) // 30 seconds
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return;
        waitLonger = 0;
    }

<<<<<<< HEAD
    // Get required locks upfront. This avoids the GUI from getting stuck on
    // periodical polls if the core is holding the locks for a longer time -
    // for example, during a wallet rescan.
    TRY_LOCK(cs_main, lockMain);
    if (!lockMain)
        return;
    TRY_LOCK(wallet->cs_wallet, lockWallet);
    if (!lockWallet)
        return;

    int chainHeight = chainActive.Height();
    if (fForceCheckBalanceChanged || chainHeight != cachedNumBlocks) {
        fForceCheckBalanceChanged = false;

        // Balance and number of transactions might have changed
        cachedNumBlocks = chainHeight;

        checkBalanceChanged();
        if (transactionTableModel) {
            transactionTableModel->updateConfirmations();
        }
    } else {
        checkBalanceChanged();
=======
    // Don't continue processing if the chain tip time is less than the first
    // key creation time as there is no need to iterate over the transaction
    // table model in this case.
    int64_t blockTime = clientModel().getLastBlockProcessedTime();
    if (blockTime < getCreationTime())
        return;

    // Avoid recomputing wallet balances unless a tx changed or
    // BlockTip notification was received.
    if (!fForceCheckBalanceChanged && m_cached_best_block_hash == getLastBlockProcessed()) return;

    processingBalance = true;
    pollFuture = QtConcurrent::run(processBalanceChange, this);
}

void WalletModel::updateTxModelData()
{
    if (transactionTableModel) {
        transactionTableModel->updateConfirmations();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

void WalletModel::emitBalanceChanged()
{
    // Force update of UI elements even when no values have changed
<<<<<<< HEAD
    if (cachedBalance == 0 && !checkBalanceChanged())
        return;
    
    Q_EMIT balanceChanged(cachedBalance, cachedUnconfirmedBalance, cachedImmatureBalance,
        cachedWatchOnlyBalance, cachedWatchUnconfBalance, cachedWatchImmatureBalance);
}

bool WalletModel::checkBalanceChanged()
{
    TRY_LOCK(cs_main, lockMain);
    if (!lockMain) return false;
    TRY_LOCK(pwalletMain->cs_wallet, lockWallet);
    if (!lockWallet) return false;
    CAmount newBalance = getBalance();
    CAmount newUnconfirmedBalance = getUnconfirmedBalance();
    CAmount newImmatureBalance = getImmatureBalance();
    CAmount newSpendableBalance = newBalance - newImmatureBalance;
    static bool stkEnabled = false;
    static bool walletLocked = pwalletMain->IsLocked();
    CAmount newWatchOnlyBalance = 0;
    CAmount newWatchUnconfBalance = 0;
    CAmount newWatchImmatureBalance = 0;
    if (haveWatchOnly()) {
        newWatchOnlyBalance = getWatchBalance();
        newWatchUnconfBalance = getWatchUnconfirmedBalance();
        newWatchImmatureBalance = getWatchImmatureBalance();
    }

    if (walletLocked != pwalletMain->IsLocked() || 
        (stkEnabled != (nLastCoinStakeSearchInterval > 0)) || 
        newSpendableBalance != spendableBalance || 
        cachedBalance != newBalance || 
        cachedUnconfirmedBalance != newUnconfirmedBalance || 
        cachedImmatureBalance != newImmatureBalance ||
        cachedWatchOnlyBalance != newWatchOnlyBalance || 
        cachedWatchUnconfBalance != newWatchUnconfBalance || 
        cachedWatchImmatureBalance != newWatchImmatureBalance ||
        cachedTxLocks != nCompleteTXLocks) {
        cachedBalance = newBalance;
        cachedUnconfirmedBalance = newUnconfirmedBalance;
        cachedImmatureBalance = newImmatureBalance;
        spendableBalance = newSpendableBalance;
        cachedTxLocks = nCompleteTXLocks;
        cachedWatchOnlyBalance = newWatchOnlyBalance;
        cachedWatchUnconfBalance = newWatchUnconfBalance;
        cachedWatchImmatureBalance = newWatchImmatureBalance;
        stkEnabled = (nLastCoinStakeSearchInterval > 0);
        walletLocked = pwalletMain->IsLocked();
        Q_EMIT balanceChanged(newBalance, newUnconfirmedBalance, newImmatureBalance,
            newWatchOnlyBalance, newWatchUnconfBalance, newWatchImmatureBalance);
        return true;
    }

    return false;
=======
   Q_EMIT balanceChanged(getBalances());
}

void WalletModel::checkBalanceChanged(const interfaces::WalletBalances& newBalance)
{
    if (newBalance.balanceChanged(m_cached_balances)) {
        m_cached_balances = newBalance;
        QMetaObject::invokeMethod(this, "balanceNotify", Qt::QueuedConnection);
    }
}

void WalletModel::balanceNotify()
{
    Q_EMIT balanceChanged(m_cached_balances);
}

void WalletModel::pollFinished()
{
    processingBalance = false;
}

void WalletModel::stop()
{
    if (pollFuture.isRunning()) {
        pollFuture.cancel();
        pollFuture.setPaused(true);
    }
}

void WalletModel::setWalletDefaultFee(CAmount fee)
{
    payTxFee = CFeeRate(fee);
}

bool WalletModel::hasWalletCustomFee()
{
    LOCK(wallet->cs_wallet);
    return wallet->fUseCustomFee;
}

bool WalletModel::getWalletCustomFee(CAmount& nFeeRet)
{
    LOCK(wallet->cs_wallet);
    nFeeRet = wallet->nCustomFee;
    return wallet->fUseCustomFee;
}

void WalletModel::setWalletCustomFee(bool fUseCustomFee, const CAmount nFee)
{
    LOCK(wallet->cs_wallet);
    WalletBatch db(wallet->GetDBHandle());
    if (wallet->fUseCustomFee != fUseCustomFee) {
        wallet->fUseCustomFee = fUseCustomFee;
        db.WriteUseCustomFee(fUseCustomFee);
    }
    if (wallet->nCustomFee != nFee) {
        wallet->nCustomFee = nFee;
        db.WriteCustomFeeValue(nFee);
    }
}

void WalletModel::setWalletStakeSplitThreshold(const CAmount nStakeSplitThreshold)
{
    wallet->SetStakeSplitThreshold(nStakeSplitThreshold);
}

CAmount WalletModel::getWalletStakeSplitThreshold() const
{
    return wallet->GetStakeSplitThreshold();
}

/* returns default minimum value for stake split threshold as doulbe */
double WalletModel::getSSTMinimum() const
{
    return static_cast<double>(CWallet::minStakeSplitThreshold) / COIN;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void WalletModel::updateTransaction()
{
    // Balance and number of transactions might have changed
    fForceCheckBalanceChanged = true;
}

void WalletModel::updateAddressBook(const QString& address, const QString& label, bool isMine, const QString& purpose, int status)
{
<<<<<<< HEAD
    if (addressTableModel)
        addressTableModel->updateEntry(address, label, isMine, purpose, status);
}
void WalletModel::updateAddressBook(const QString& pubCoin, const QString& isUsed, int status)
{
    if (addressTableModel)
        addressTableModel->updateEntry(pubCoin, isUsed, status);
}


=======
    try {
        if (addressTableModel)
            addressTableModel->updateEntry(address, label, isMine, purpose, status);
    } catch (...) {
        std::cout << "Exception updateAddressBook" << std::endl;
    }
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void WalletModel::updateWatchOnlyFlag(bool fHaveWatchonly)
{
    fHaveWatchOnly = fHaveWatchonly;
    Q_EMIT notifyWatchonlyChanged(fHaveWatchonly);
}

bool WalletModel::validateAddress(const QString& address)
{
<<<<<<< HEAD
    CBitcoinAddress addressParsed(address.toStdString());
    bool valid = (regex_match(address.toStdString(), regex("[a-zA-z0-9]+")))&&(address.length()==99||address.length()==110);
    return valid||addressParsed.IsValid();
}

WalletModel::SendCoinsReturn WalletModel::prepareTransaction(WalletModelTransaction& transaction, const CCoinControl* coinControl)
{
    CAmount total = 0;
    QList<SendCoinsRecipient> recipients = transaction.getRecipients();
    std::vector<std::pair<CScript, CAmount> > vecSend;
=======
    // Only regular base58 addresses and shielded addresses accepted here
    bool isStaking = false;
    CWDestination dest = Standard::DecodeDestination(address.toStdString(), isStaking);
    const auto regDest = boost::get<CTxDestination>(&dest);
    if (regDest && IsValidDestination(*regDest) && isStaking) return false;
    return Standard::IsValidDestination(dest);
}

bool WalletModel::validateAddress(const QString& address, bool fStaking)
{
    return IsValidDestinationString(address.toStdString(), fStaking);
}

bool WalletModel::validateAddress(const QString& address, bool fStaking, bool& isShielded)
{
    bool isStaking = false;
    CWDestination dest = Standard::DecodeDestination(address.toStdString(), isStaking);
    if (IsShieldedDestination(dest)) {
        isShielded = true;
        return true;
    }
    return Standard::IsValidDestination(dest) && (isStaking == fStaking);
}

bool WalletModel::updateAddressBookLabels(const CWDestination& dest, const std::string& strName, const std::string& strPurpose)
{
    auto optAdd = wallet->GetAddressBookEntry(dest);
    // Check if we have a new address or an updated label
    if (!optAdd) {
        return wallet->SetAddressBook(dest, strName, strPurpose);
    } else if (optAdd->name != strName) {
        return wallet->SetAddressBook(dest, strName, ""); // "" means don't change purpose
    }
    return false;
}

bool WalletModel::addKeys(const CKey& key, const CPubKey& pubkey, WalletRescanReserver& reserver)
{
    {
        LOCK(wallet->cs_wallet);
        wallet->mapKeyMetadata[pubkey.GetID()].nCreateTime = 1;

        if (!wallet->AddKeyPubKey(key, pubkey)) {
            return false;
        }

        // whenever a key is imported, we need to scan the whole chain
        wallet->nTimeFirstKey = 1; // 0 would be considered 'no value'
    }
    CBlockIndex* pindexGenesis = WITH_LOCK(cs_main, return chainActive.Genesis(); );
    wallet->ScanForWalletTransactions(pindexGenesis, nullptr, reserver, true);
    return true;
}

WalletModel::SendCoinsReturn WalletModel::prepareTransaction(WalletModelTransaction* transaction, const CCoinControl* coinControl, bool fIncludeDelegations)
{
    CAmount total = 0;
    QList<SendCoinsRecipient> recipients = transaction->getRecipients();
    std::vector<CRecipient> vecSend;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (recipients.empty()) {
        return OK;
    }

<<<<<<< HEAD
    if (isAnonymizeOnlyUnlocked()) {
        return AnonymizeOnlyUnlocked;
=======
    if (isStakingOnlyUnlocked()) {
        return StakingOnlyUnlocked;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    QSet<QString> setAddress; // Used to detect duplicates
    int nAddresses = 0;

    // Pre-check input data for validity
<<<<<<< HEAD
   Q_FOREACH (const SendCoinsRecipient& rcp, recipients) {
        if (rcp.paymentRequest.IsInitialized()) { // PaymentRequest...
            CAmount subtotal = 0;
            const payments::PaymentDetails& details = rcp.paymentRequest.getDetails();
            for (int i = 0; i < details.outputs_size(); i++) {
                const payments::Output& out = details.outputs(i);
                if (out.amount() <= 0) continue;
                subtotal += out.amount();
                const unsigned char* scriptStr = (const unsigned char*)out.script().data();
                CScript scriptPubKey(scriptStr, scriptStr + out.script().size());
                vecSend.push_back(std::pair<CScript, CAmount>(scriptPubKey, out.amount()));
            }
            if (subtotal <= 0) {
                return InvalidAmount;
            }
            total += subtotal;
        } else { // User-entered prcycoin address / amount:
            if (!validateAddress(rcp.address)) {
=======
    for (const SendCoinsRecipient& rcp : recipients) {
        { // User-entered pivx address / amount:
            if (!validateAddress(rcp.address, rcp.isP2CS)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                return InvalidAddress;
            }
            if (rcp.amount <= 0) {
                return InvalidAmount;
            }
            setAddress.insert(rcp.address);
            ++nAddresses;

<<<<<<< HEAD
            CScript scriptPubKey = GetScriptForDestination(CBitcoinAddress(rcp.address.toStdString()).Get());
            vecSend.push_back(std::pair<CScript, CAmount>(scriptPubKey, rcp.amount));
=======
            CScript scriptPubKey;
            CTxDestination out = DecodeDestination(rcp.address.toStdString());

            if (rcp.isP2CS) {
                Destination ownerAdd;
                if (rcp.ownerAddress.isEmpty()) {
                    // Create new internal owner address
                    auto res = getNewAddress();
                    if (!res) return CannotCreateInternalAddress;
                    ownerAdd = *res.getObjResult();
                } else {
                    ownerAdd = Destination(DecodeDestination(rcp.ownerAddress.toStdString()), false);
                }

                const CKeyID* stakerId = boost::get<CKeyID>(&out);
                const CKeyID* ownerId = ownerAdd.getKeyID();
                if (!stakerId || !ownerId) {
                    return InvalidAddress;
                }

                scriptPubKey = isV6Enforced() ? GetScriptForStakeDelegation(*stakerId, *ownerId)
                                              : GetScriptForStakeDelegationLOF(*stakerId, *ownerId);
            } else {
                // Regular P2PK or P2PKH
                scriptPubKey = GetScriptForDestination(out);
            }
            vecSend.emplace_back(scriptPubKey, rcp.amount, rcp.fSubtractFee);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            total += rcp.amount;
        }
    }
    if (setAddress.size() != nAddresses) {
        return DuplicateAddress;
    }

<<<<<<< HEAD
    CAmount nBalance = getBalance(coinControl);

    if (total > nBalance) {
=======
    CAmount nSpendableBalance = getUnlockedBalance(coinControl, fIncludeDelegations);

    if (total > nSpendableBalance) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return AmountExceedsBalance;
    }

    {
        LOCK2(cs_main, wallet->cs_wallet);

<<<<<<< HEAD
        transaction.newPossibleKeyChange(wallet);
        CAmount nFeeRequired = 0;
        std::string strFailReason;

        CWalletTx* newTx = transaction.getTransaction();
        CReserveKey* keyChange = transaction.getPossibleKeyChange();

        bool fCreated = wallet->CreateTransaction(vecSend, *newTx, *keyChange, nFeeRequired, strFailReason, coinControl, recipients[0].inputType, recipients[0].useSwiftTX);
        transaction.setTransactionFee(nFeeRequired);

        if (!fCreated) {
            if ((total + nFeeRequired) > nBalance) {
                return SendCoinsReturn(AmountWithFeeExceedsBalance);
            }
            Q_EMIT message(tr("Send Coins"), QString::fromStdString(strFailReason),
                CClientUIInterface::MSG_ERROR);
=======
        CReserveKey* keyChange = transaction->newPossibleKeyChange(wallet);
        CAmount nFeeRequired = 0;
        int nChangePosInOut = -1;
        std::string strFailReason;

        bool fCreated = wallet->CreateTransaction(vecSend,
                                                  transaction->getTransaction(),
                                                  *keyChange,
                                                  nFeeRequired,
                                                  nChangePosInOut,
                                                  strFailReason,
                                                  coinControl,
                                                  true,
                                                  0,
                                                  fIncludeDelegations,
                                                  &transaction->fIsStakeDelegationVoided);
        transaction->setTransactionFee(nFeeRequired);

        if (!fCreated) {
            if ((total + nFeeRequired) > nSpendableBalance) {
                return SendCoinsReturn(AmountWithFeeExceedsBalance);
            }

            Q_EMIT message(tr("Send Coins"), tr("Transaction creation failed!\n%1").arg(
                    strFailReason == "Transaction too large" ?
                            tr("The size of the transaction is too big.\nSelect fewer inputs with coin control.") :
                            QString::fromStdString(strFailReason)),
                    CClientUIInterface::MSG_ERROR);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return TransactionCreationFailed;
        }

        // reject insane fee
<<<<<<< HEAD
        if (nFeeRequired > ::minRelayTxFee.GetFee(transaction.getTransactionSize()) * 10000)
=======
        if (nFeeRequired > ::minRelayTxFee.GetFee(transaction->getTransactionSize()) * 10000)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            return InsaneFee;
    }

    return SendCoinsReturn(OK);
}

WalletModel::SendCoinsReturn WalletModel::sendCoins(WalletModelTransaction& transaction)
{
<<<<<<< HEAD
    QByteArray transaction_array; /* store serialized transaction */

    std::string stealthAddr = transaction.getRecipients()[0].address.toStdString();
    CAmount nValue = transaction.getRecipients()[0].amount;
    CWalletTx wtxNew;

    if (wallet->SendToStealthAddress(stealthAddr, nValue, wtxNew,false))
        return SendCoinsReturn(OK);

    return SendCoinsReturn(TransactionCommitFailed);
=======
    bool fColdStakingActive = isColdStakingNetworkelyEnabled();

    // Double check the tx before doing anything
    CTransactionRef& newTx = transaction.getTransaction();
    CValidationState state;
    if (!CheckTransaction(*newTx, state, fColdStakingActive)) {
        return TransactionCheckFailed;
    }

    QByteArray transaction_array; /* store serialized transaction */

    {
        LOCK2(cs_main, wallet->cs_wallet);

        CReserveKey* keyChange = transaction.getPossibleKeyChange();
        const CWallet::CommitResult& res = wallet->CommitTransaction(newTx, keyChange, g_connman.get());
        if (res.status != CWallet::CommitStatus::OK) {
            return SendCoinsReturn(res);
        }

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << *newTx;
        transaction_array.append(&(ssTx[0]), ssTx.size());
    }

    // Add addresses / update labels that we've sent to to the address book,
    // and emit coinsSent signal for each recipient
    for (const SendCoinsRecipient& rcp : transaction.getRecipients()) {
        // Don't touch the address book when we have a payment request
        {
            bool isStaking = false;
            bool isShielded = false;
            auto address = Standard::DecodeDestination(rcp.address.toStdString(), isStaking, isShielded);
            std::string purpose = isShielded ? AddressBook::AddressBookPurpose::SHIELDED_SEND :
                                  isStaking ? AddressBook::AddressBookPurpose::COLD_STAKING_SEND : AddressBook::AddressBookPurpose::SEND;
            std::string strLabel = rcp.label.toStdString();
            updateAddressBookLabels(address, strLabel, purpose);
        }
        Q_EMIT coinsSent(wallet, rcp, transaction_array);
    }
    emitBalanceChanged(); // update balance immediately, otherwise there could be a short noticeable delay until pollBalanceChanged hits

    return SendCoinsReturn(OK);
}

OperationResult WalletModel::PrepareShieldedTransaction(WalletModelTransaction* modelTransaction,
                                                        bool fromTransparent,
                                                        const CCoinControl* coinControl)
{
    // Load shieldedAddrRecipients.
    std::vector<SendManyRecipient> recipients;
    for (const auto& recipient : modelTransaction->getRecipients()) {
        bool fSubtractFeeFromAmount = recipient.fSubtractFee;
        if (recipient.isShieldedAddr) {
            auto pa = KeyIO::DecodeSaplingPaymentAddress(recipient.address.toStdString());
            if (!pa) return errorOut("Error, invalid shielded address");
            recipients.emplace_back(*pa, recipient.amount, recipient.message.toStdString(), fSubtractFeeFromAmount);
        } else {
            auto dest = DecodeDestination(recipient.address.toStdString());
            if (!IsValidDestination(dest)) return errorOut("Error, invalid transparent address");
            recipients.emplace_back(dest, recipient.amount, fSubtractFeeFromAmount);
        }
    }

    // Now check the transaction size
    auto opResult = CheckTransactionSize(recipients, true);
    if (!opResult) return opResult;

    // Create the operation
    SaplingOperation operation(Params().GetConsensus(), wallet);
    auto operationResult = operation.setRecipients(recipients)
             ->setTransparentKeyChange(modelTransaction->getPossibleKeyChange())
             ->setSelectTransparentCoins(fromTransparent)
             ->setSelectShieldedCoins(!fromTransparent)
             ->setCoinControl(coinControl)
             ->setMinDepth(fromTransparent ? 1 : 5)
             ->build();

    if (!operationResult) {
        return operationResult;
    }

    // load the transaction and key change (if needed)
    CTransactionRef& txRef = modelTransaction->getTransaction();
    txRef = MakeTransactionRef(operation.getFinalTx());
    modelTransaction->setTransactionFee(operation.getFee()); // in the future, fee will be dynamically calculated.
    return operationResult;
}

OperationResult WalletModel::createAndSendProposalFeeTx(CBudgetProposal& proposal)
{
    CTransactionRef wtx;
    const uint256& nHash = proposal.GetHash();
    CReserveKey keyChange(wallet);
    if (!wallet->CreateBudgetFeeTX(wtx, nHash, keyChange, BUDGET_FEE_TX_OLD)) { // 50 PIV collateral for proposal
        return {false , "Error making fee transaction for proposal. Please check your wallet balance."};
    }

    // send the tx to the network
    mapValue_t extraValues;
    extraValues.emplace("proposal", toHexStr(proposal));
    const CWallet::CommitResult& res = wallet->CommitTransaction(wtx, &keyChange, g_connman.get(), &extraValues);
    if (res.status != CWallet::CommitStatus::OK) {
        return {false, strprintf("Cannot commit proposal fee transaction: %s", res.ToString())};
    }
    // Everything went fine, set the fee tx hash
    proposal.SetFeeTxHash(wtx->GetHash());
    return {true};
}

const CWalletTx* WalletModel::getTx(uint256 id)
{
    return wallet->GetWalletTx(id);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

OptionsModel* WalletModel::getOptionsModel()
{
    return optionsModel;
}

AddressTableModel* WalletModel::getAddressTableModel()
{
    return addressTableModel;
}

TransactionTableModel* WalletModel::getTransactionTableModel()
{
    return transactionTableModel;
}

<<<<<<< HEAD
RecentRequestsTableModel* WalletModel::getRecentRequestsTableModel()
{
    return recentRequestsTableModel;
}

WalletModel::EncryptionStatus WalletModel::getEncryptionStatus() const
{
    if (!wallet->IsLocked()) {
        return Unencrypted;
    } else if (wallet->fWalletUnlockAnonymizeOnly) {
        return UnlockedForAnonymizationOnly;
=======
WalletModel::EncryptionStatus WalletModel::getEncryptionStatus() const
{
    if (!wallet) throw std::runtime_error("Error, cannot get encryption status. Wallet doesn't exist");
    if (!wallet->IsCrypted()) {
        return Unencrypted;
    } else if (wallet->fWalletUnlockStaking) {
        return UnlockedForStaking;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    } else if (wallet->IsLocked()) {
        return Locked;
    } else {
        return Unlocked;
    }
<<<<<<< HEAD
=======

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool WalletModel::setWalletEncrypted(bool encrypted, const SecureString& passphrase)
{
    if (encrypted) {
        // Encrypt
        return wallet->EncryptWallet(passphrase);
    } else {
        // Decrypt -- TODO; not supported yet
        return false;
    }
}

<<<<<<< HEAD
bool WalletModel::setWalletLocked(bool locked, const SecureString& passPhrase, bool anonymizeOnly)
{
    if (locked) {
        // Lock
        wallet->fWalletUnlockAnonymizeOnly = false;
        return wallet->Lock();
    } else {
        // Unlock
        return wallet->Unlock(passPhrase, anonymizeOnly);
=======
bool WalletModel::setWalletLocked(bool locked, const SecureString& passPhrase, bool stakingOnly)
{
    if (locked) {
        // Lock
        wallet->fWalletUnlockStaking = false;
        return wallet->Lock();
    } else {
        // Unlock
        return wallet->Unlock(passPhrase, stakingOnly);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

bool WalletModel::lockForStakingOnly(const SecureString& passPhrase)
{
    if (!wallet->IsLocked()) {
<<<<<<< HEAD
        wallet->fWalletUnlockAnonymizeOnly = true;
=======
        wallet->fWalletUnlockStaking = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return true;
    } else {
        setWalletLocked(false, passPhrase, true);
    }
    return false;
}

<<<<<<< HEAD
bool WalletModel::isAnonymizeOnlyUnlocked()
{
    return wallet->fWalletUnlockAnonymizeOnly;
=======
bool WalletModel::isStakingOnlyUnlocked()
{
    return wallet->fWalletUnlockStaking;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool WalletModel::changePassphrase(const SecureString& oldPass, const SecureString& newPass)
{
    bool retval;
    {
        LOCK(wallet->cs_wallet);
        wallet->Lock(); // Make sure wallet is locked before attempting pass change
        retval = wallet->ChangeWalletPassphrase(oldPass, newPass);
    }
    return retval;
}

bool WalletModel::backupWallet(const QString& filename)
{
<<<<<<< HEAD
    //attempt regular backup
    if(!BackupWallet(*wallet, filename.toLocal8Bit().data())) {
=======
    // Attempt regular backup
    if (!wallet->BackupWallet(filename.toLocal8Bit().data())) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return error("ERROR: Failed to backup wallet!");
    }

    return true;
}

<<<<<<< HEAD
// Handlers for core signals
static void NotifyKeyStoreStatusChanged(WalletModel* walletmodel, CCryptoKeyStore* wallet)
=======

// Handlers for core signals
static void NotifyKeyStoreStatusChanged(WalletModel* walletmodel)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    qDebug() << "NotifyKeyStoreStatusChanged";
    QMetaObject::invokeMethod(walletmodel, "updateStatus", Qt::QueuedConnection);
}

<<<<<<< HEAD
static void NotifyAddressBookChanged(WalletModel* walletmodel, CWallet* wallet, const CTxDestination& address, const std::string& label, bool isMine, const std::string& purpose, ChangeType status)
{
    QString strAddress = QString::fromStdString(CBitcoinAddress(address).ToString());
=======
static void NotifyAddressBookChanged(WalletModel* walletmodel, CWallet* wallet, const CWDestination& address, const std::string& label, bool isMine, const std::string& purpose, ChangeType status)
{
    QString strAddress = QString::fromStdString(wallet->ParseIntoAddress(address, purpose));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QString strLabel = QString::fromStdString(label);
    QString strPurpose = QString::fromStdString(purpose);

    qDebug() << "NotifyAddressBookChanged : " + strAddress + " " + strLabel + " isMine=" + QString::number(isMine) + " purpose=" + strPurpose + " status=" + QString::number(status);
    QMetaObject::invokeMethod(walletmodel, "updateAddressBook", Qt::QueuedConnection,
        Q_ARG(QString, strAddress),
        Q_ARG(QString, strLabel),
        Q_ARG(bool, isMine),
        Q_ARG(QString, strPurpose),
        Q_ARG(int, status));
}

// queue notifications to show a non freezing progress dialog e.g. for rescan
static bool fQueueNotifications = false;
static std::vector<std::pair<uint256, ChangeType> > vQueueNotifications;
static void NotifyTransactionChanged(WalletModel* walletmodel, CWallet* wallet, const uint256& hash, ChangeType status)
{
    if (fQueueNotifications) {
<<<<<<< HEAD
        vQueueNotifications.push_back(make_pair(hash, status));
=======
        vQueueNotifications.emplace_back(hash, status);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return;
    }

    QString strHash = QString::fromStdString(hash.GetHex());

    qDebug() << "NotifyTransactionChanged : " + strHash + " status= " + QString::number(status);
<<<<<<< HEAD
    QMetaObject::invokeMethod(walletmodel, "updateTransaction", Qt::QueuedConnection
    );
=======
    QMetaObject::invokeMethod(walletmodel, "updateTransaction", Qt::QueuedConnection /*,
                              Q_ARG(QString, strHash),
                              Q_ARG(int, status)*/);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

static void ShowProgress(WalletModel* walletmodel, const std::string& title, int nProgress)
{
    // emits signal "showProgress"
    QMetaObject::invokeMethod(walletmodel, "showProgress", Qt::QueuedConnection,
        Q_ARG(QString, QString::fromStdString(title)),
        Q_ARG(int, nProgress));
}

static void NotifyWatchonlyChanged(WalletModel* walletmodel, bool fHaveWatchonly)
{
    QMetaObject::invokeMethod(walletmodel, "updateWatchOnlyFlag", Qt::QueuedConnection,
        Q_ARG(bool, fHaveWatchonly));
}

<<<<<<< HEAD
static void NotifyWalletBacked(WalletModel* model, const bool& fSuccess, const string& filename)
{
    std::string message;
    string title = "Backup ";
    CClientUIInterface::MessageBoxFlags method;

    if (fSuccess) {
=======
static void NotifySSTChanged(WalletModel* walletmodel, const CAmount stakeSplitThreshold)
{
    const double val = static_cast<double>(stakeSplitThreshold) / COIN;
    Q_EMIT walletmodel->notifySSTChanged(val);
}

static void NotifyWalletBacked(WalletModel* model, const bool fSuccess, const std::string& filename)
{
    std::string message;
    std::string title = "Backup ";
    CClientUIInterface::MessageBoxFlags method;

    if (fSuccess) {
        message = "The wallet data was successfully saved to ";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        title += "Successful: ";
        method = CClientUIInterface::MessageBoxFlags::MSG_INFORMATION;
    } else {
        message = "There was an error trying to save the wallet data to ";
        title += "Failed: ";
        method = CClientUIInterface::MessageBoxFlags::MSG_ERROR;
    }

    message += _(filename.data());

<<<<<<< HEAD

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    QMetaObject::invokeMethod(model, "message", Qt::QueuedConnection,
                              Q_ARG(QString, QString::fromStdString(title)),
                              Q_ARG(QString, QString::fromStdString(message)),
                              Q_ARG(unsigned int, (unsigned int)method));
}

void WalletModel::subscribeToCoreSignals()
{
    // Connect signals to wallet
<<<<<<< HEAD
    wallet->NotifyStatusChanged.connect(boost::bind(&NotifyKeyStoreStatusChanged, this, _1));
    wallet->NotifyAddressBookChanged.connect(boost::bind(NotifyAddressBookChanged, this, _1, _2, _3, _4, _5, _6));
    wallet->NotifyTransactionChanged.connect(boost::bind(NotifyTransactionChanged, this, _1, _2, _3));
    wallet->ShowProgress.connect(boost::bind(ShowProgress, this, _1, _2));
    wallet->NotifyWatchonlyChanged.connect(boost::bind(NotifyWatchonlyChanged, this, _1));
    wallet->NotifyWalletBacked.connect(boost::bind(NotifyWalletBacked, this, _1, _2));
=======
    m_handler_notify_status_changed = interfaces::MakeHandler(wallet->NotifyStatusChanged.connect(std::bind(&NotifyKeyStoreStatusChanged, this)));
    m_handler_notify_addressbook_changed = interfaces::MakeHandler(wallet->NotifyAddressBookChanged.connect(std::bind(NotifyAddressBookChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6)));
    m_handler_notify_sst_changed = interfaces::MakeHandler(wallet->NotifySSTChanged.connect(std::bind(NotifySSTChanged, this, std::placeholders::_1)));
    m_handler_notify_transaction_changed = interfaces::MakeHandler(wallet->NotifyTransactionChanged.connect(std::bind(NotifyTransactionChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
    m_handler_show_progress = interfaces::MakeHandler(wallet->ShowProgress.connect(std::bind(ShowProgress, this, std::placeholders::_1, std::placeholders::_2)));
    m_handler_notify_watch_only_changed = interfaces::MakeHandler(wallet->NotifyWatchonlyChanged.connect(std::bind(NotifyWatchonlyChanged, this, std::placeholders::_1)));
    m_handler_notify_walletbacked = interfaces::MakeHandler(wallet->NotifyWalletBacked.connect(std::bind(NotifyWalletBacked, this, std::placeholders::_1, std::placeholders::_2)));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void WalletModel::unsubscribeFromCoreSignals()
{
    // Disconnect signals from wallet
<<<<<<< HEAD
    wallet->NotifyStatusChanged.disconnect(boost::bind(&NotifyKeyStoreStatusChanged, this, _1));
    wallet->NotifyAddressBookChanged.disconnect(boost::bind(NotifyAddressBookChanged, this, _1, _2, _3, _4, _5, _6));
    wallet->NotifyTransactionChanged.disconnect(boost::bind(NotifyTransactionChanged, this, _1, _2, _3));
    wallet->ShowProgress.disconnect(boost::bind(ShowProgress, this, _1, _2));
    wallet->NotifyWatchonlyChanged.disconnect(boost::bind(NotifyWatchonlyChanged, this, _1));
    wallet->NotifyWalletBacked.disconnect(boost::bind(NotifyWalletBacked, this, _1, _2));
}

// WalletModel::UnlockContext implementation
WalletModel::UnlockContext WalletModel::requestUnlock(AskPassphraseDialog::Context context, bool relock)
{
    bool was_locked = getEncryptionStatus() == Locked;

    if (!was_locked && isAnonymizeOnlyUnlocked()) {
        setWalletLocked(true);
        wallet->fWalletUnlockAnonymizeOnly = false;
        was_locked = getEncryptionStatus() == Locked;
    }

    if (was_locked) {
        // Request UI to unlock wallet
        Q_EMIT requireUnlock(context);
    }
    // If wallet is still locked, unlock was failed or cancelled, mark context as invalid
    bool valid = getEncryptionStatus() != Locked;

    return UnlockContext(valid, relock);
}

WalletModel::UnlockContext::UnlockContext(bool valid, bool relock) : valid(valid), relock(relock)
=======
    m_handler_notify_status_changed->disconnect();
    m_handler_notify_addressbook_changed->disconnect();
    m_handler_notify_sst_changed->disconnect();
    m_handler_notify_transaction_changed->disconnect();
    m_handler_show_progress->disconnect();
    m_handler_notify_watch_only_changed->disconnect();
    m_handler_notify_walletbacked->disconnect();
}

// WalletModel::UnlockContext implementation
WalletModel::UnlockContext WalletModel::requestUnlock()
{
    const WalletModel::EncryptionStatus status_before = getEncryptionStatus();
    if (status_before == Locked || status_before == UnlockedForStaking)
    {
        // Request UI to unlock wallet
        Q_EMIT requireUnlock();
    }
    // If wallet is still locked, unlock was failed or cancelled, mark context as invalid
    bool valid = isWalletUnlocked();

    return UnlockContext(this, valid, status_before);
}

WalletModel::UnlockContext::UnlockContext(WalletModel *_wallet, bool _valid, const WalletModel::EncryptionStatus& status_before):
        wallet(_wallet),
        valid(_valid),
        was_status(status_before),
        relock(status_before == Locked || status_before == UnlockedForStaking)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
}

WalletModel::UnlockContext::~UnlockContext()
{
<<<<<<< HEAD
}

CWallet* WalletModel::getCWallet()
{
    return this->wallet;
}

void WalletModel::UnlockContext::CopyFrom(const UnlockContext& rhs)
=======
    if (valid && relock && wallet) {
        if (was_status == Locked) wallet->setWalletLocked(true);
        else if (was_status == UnlockedForStaking) wallet->lockForStakingOnly();
        wallet->updateStatus();
    }
}

void WalletModel::UnlockContext::CopyFrom(UnlockContext&& rhs)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // Transfer context; old object no longer relocks wallet
    *this = rhs;
    rhs.relock = false;
}

bool WalletModel::getPubKey(const CKeyID& address, CPubKey& vchPubKeyOut) const
{
    return wallet->GetPubKey(address, vchPubKeyOut);
}

<<<<<<< HEAD
// returns a list of COutputs from COutPoints
void WalletModel::getOutputs(const std::vector<COutPoint>& vOutpoints, std::vector<COutput>& vOutputs)
{
    LOCK2(cs_main, wallet->cs_wallet);
    for (const COutPoint& outpoint : vOutpoints) {
        if (!wallet->mapWallet.count(outpoint.hash)) continue;
        int nDepth = wallet->mapWallet[outpoint.hash].GetDepthInMainChain();
        if (nDepth < 0) continue;
        COutput out(&wallet->mapWallet[outpoint.hash], outpoint.n, nDepth, true);
        vOutputs.push_back(out);
    }
}

bool WalletModel::isSpent(const COutPoint& outpoint) const
{
    LOCK2(cs_main, wallet->cs_wallet);
    return wallet->IsSpent(outpoint.hash, outpoint.n);
}

// AvailableCoins + LockedCoins grouped by wallet address (put change in one group with wallet address)
void WalletModel::listCoins(std::map<QString, std::vector<COutput> >& mapCoins) const
{
    std::vector<COutput> vCoins;
    wallet->AvailableCoins(vCoins);

    LOCK2(cs_main, wallet->cs_wallet); // ListLockedCoins, mapWallet
    std::vector<COutPoint> vLockedCoins;
    wallet->ListLockedCoins(vLockedCoins);

    // add locked coins
    for (const COutPoint& outpoint : vLockedCoins) {
        if (!wallet->mapWallet.count(outpoint.hash)) continue;
        int nDepth = wallet->mapWallet[outpoint.hash].GetDepthInMainChain();
        if (nDepth < 0) continue;
        COutput out(&wallet->mapWallet[outpoint.hash], outpoint.n, nDepth, true);
        if (outpoint.n < out.tx->vout.size() && wallet->IsMine(out.tx->vout[outpoint.n]) == ISMINE_SPENDABLE)
            vCoins.push_back(out);
    }

    for (const COutput& out : vCoins) {
        COutput cout = out;

        while (wallet->IsChange(cout.tx->vout[cout.i]) && cout.tx->vin.size() > 0 && wallet->IsMine(cout.tx->vin[0])) {
            if (!wallet->mapWallet.count(wallet->findMyOutPoint(cout.tx->vin[0]).hash)) break;
            cout = COutput(&wallet->mapWallet[wallet->findMyOutPoint(cout.tx->vin[0]).hash], wallet->findMyOutPoint(cout.tx->vin[0]).n, 0, true);
        }

        CTxDestination address;
        if (!out.fSpendable || !ExtractDestination(cout.tx->vout[cout.i].scriptPubKey, address))
            continue;
        mapCoins[QString::fromStdString(CBitcoinAddress(address).ToString())].push_back(out);
=======
int64_t WalletModel::getCreationTime() const {
    return wallet->nTimeFirstKey;
}

int64_t WalletModel::getKeyCreationTime(const CPubKey& key)
{
    return wallet->GetKeyCreationTime(key);
}

int64_t WalletModel::getKeyCreationTime(const CTxDestination& address)
{
    if (this->isMine(address)) {
        return wallet->GetKeyCreationTime(address);
    }
    return 0;
}

int64_t WalletModel::getKeyCreationTime(const std::string& address)
{
    return wallet->GetKeyCreationTime(Standard::DecodeDestination(address));
}

int64_t WalletModel::getKeyCreationTime(const libzcash::SaplingPaymentAddress& address)
{
    if (this->isMine(address)) {
        return wallet->GetKeyCreationTime(address);
    }
    return 0;
}

CallResult<Destination> WalletModel::getNewAddress(const std::string& label) const
{
    auto res = wallet->getNewAddress(label);
    return res ? CallResult<Destination>(Destination(*res.getObjResult(), false)) :
           CallResult<Destination>(res.getError());
}

CallResult<Destination> WalletModel::getNewStakingAddress(const std::string& label) const
{
    auto res = wallet->getNewStakingAddress(label);
    return res ? CallResult<Destination>(Destination(*res.getObjResult(), true)) :
           CallResult<Destination>(res.getError());
}

CallResult<Destination> WalletModel::getNewShieldedAddress(std::string strLabel)
{
    return CallResult<Destination>(Destination(wallet->GenerateNewSaplingZKey(std::move(strLabel))));
}

bool WalletModel::whitelistAddressFromColdStaking(const QString &addressStr)
{
    return updateAddressBookPurpose(addressStr, AddressBook::AddressBookPurpose::DELEGATOR);
}

bool WalletModel::blacklistAddressFromColdStaking(const QString &addressStr)
{
    return updateAddressBookPurpose(addressStr, AddressBook::AddressBookPurpose::DELEGABLE);
}

bool WalletModel::updateAddressBookPurpose(const QString &addressStr, const std::string& purpose)
{
    bool isStaking = false;
    CTxDestination address = DecodeDestination(addressStr.toStdString(), isStaking);
    if (isStaking)
        return error("Invalid PIVX address, cold staking address");
    CKeyID keyID;
    if (!getKeyId(address, keyID))
        return false;
    return wallet->SetAddressBook(keyID, getLabelForAddress(address), purpose);
}

bool WalletModel::getKeyId(const CTxDestination& address, CKeyID& keyID)
{
    if (!IsValidDestination(address))
        return error("Invalid PIVX address");

    const CKeyID* inKeyID = boost::get<CKeyID>(&address);
    if (!inKeyID)
        return error("Unable to get KeyID from PIVX address");

    keyID = *inKeyID;
    return true;
}

std::string WalletModel::getLabelForAddress(const CTxDestination& address)
{
    std::string label = "";
    label = wallet->GetNameForAddressBookEntry(address);
    return label;
}

QString WalletModel::getSaplingAddressString(const CWalletTx* wtx, const SaplingOutPoint& op) const
{
    Optional<libzcash::SaplingPaymentAddress> opAddr =
            wallet->GetSaplingScriptPubKeyMan()->GetOutPointAddress(*wtx, op);
    if (!opAddr) {
        return QString();
    }
    QString ret = QString::fromStdString(Standard::EncodeDestination(*opAddr));
    return ret.left(18) + "..." + ret.right(18);
}

// returns a COutPoint of 10000 PIV if found
bool WalletModel::getMNCollateralCandidate(COutPoint& outPoint)
{
    CWallet::AvailableCoinsFilter coinsFilter;
    coinsFilter.fIncludeDelegated = false;
    coinsFilter.nMaxOutValue = Params().GetConsensus().nMNCollateralAmt;
    coinsFilter.nMinOutValue = coinsFilter.nMaxOutValue;
    coinsFilter.fIncludeLocked = true;
    std::vector<COutput> vCoins;
    wallet->AvailableCoins(&vCoins, nullptr, coinsFilter);
    for (const COutput& out : vCoins) {
        // skip locked collaterals
        if (!isLockedCoin(out.tx->GetHash(), out.i)) {
            outPoint = COutPoint(out.tx->GetHash(), out.i);
            return true;
        }
    }
    return false;
}

// Depth of a wallet transaction or -1 if not found
int WalletModel::getWalletTxDepth(const uint256& txHash) const
{
    const CWalletTx *walletTx = wallet->GetWalletTx(txHash);
    if (!walletTx) {
        return -1;
    }
    LOCK(wallet->cs_wallet);
    return walletTx->GetDepthInMainChain();
}

bool WalletModel::isSpent(const COutPoint& outpoint) const
{
    LOCK(wallet->cs_wallet);
    return wallet->IsSpent(outpoint.hash, outpoint.n);
}

void WalletModel::listCoins(std::map<ListCoinsKey, std::vector<ListCoinsValue>>& mapCoins, bool fTransparent) const
{
    if (fTransparent) {
        listCoins(mapCoins);
    } else {
        listAvailableNotes(mapCoins);
    }
}

void WalletModel::listAvailableNotes(std::map<ListCoinsKey, std::vector<ListCoinsValue>>& mapCoins) const
{
    for (const auto& it: wallet->ListNotes()) {
        const ListCoinsKey key{QString::fromStdString(KeyIO::EncodePaymentAddress(it.first)), false, nullopt};

        for (const SaplingNoteEntry& note : it.second) {
            mapCoins[key].emplace_back(note.op.hash,
                                       (int)note.op.n,
                                       (CAmount)note.note.value(),
                                       0,
                                       note.confirmations);
        }
    }
}

// AvailableCoins + LockedCoins grouped by wallet address (put change in one group with wallet address)
void WalletModel::listCoins(std::map<ListCoinsKey, std::vector<ListCoinsValue>>& mapCoins) const
{
    for (const auto& it: wallet->ListCoins()) {
        const std::pair<CTxDestination, Optional<CTxDestination>>& addresses = it.first;
        const std::vector<COutput>& coins = it.second;

        const QString& address = QString::fromStdString(EncodeDestination(addresses.first));
        const Optional<QString>& stakerAddr = addresses.second == nullopt ? nullopt : Optional<QString>(
                QString::fromStdString(EncodeDestination(*addresses.second, CChainParams::STAKING_ADDRESS)));
        // P2CS cannot be "change"
        const bool isChange = stakerAddr == nullopt ? wallet->IsChange(addresses.first) : false;

        const ListCoinsKey key{address, isChange, stakerAddr};

        for (const COutput& out: coins) {
            mapCoins[key].emplace_back(out.tx->GetHash(),
                                       out.i,
                                       out.tx->tx->vout[out.i].nValue,
                                       out.tx->GetTxTime(),
                                       out.nDepth);
        }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

bool WalletModel::isLockedCoin(uint256 hash, unsigned int n) const
{
<<<<<<< HEAD
    LOCK2(cs_main, wallet->cs_wallet);   
=======
    LOCK(wallet->cs_wallet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return wallet->IsLockedCoin(hash, n);
}

void WalletModel::lockCoin(COutPoint& output)
<<<<<<< HEAD
{   
    LOCK2(cs_main, wallet->cs_wallet);
=======
{
    LOCK(wallet->cs_wallet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    wallet->LockCoin(output);
}

void WalletModel::unlockCoin(COutPoint& output)
{
<<<<<<< HEAD
    LOCK2(cs_main, wallet->cs_wallet);
    wallet->UnlockCoin(output);
}

void WalletModel::listLockedCoins(std::vector<COutPoint>& vOutpts)
{
    LOCK2(cs_main, wallet->cs_wallet);
    wallet->ListLockedCoins(vOutpts);
=======
    LOCK(wallet->cs_wallet);
    wallet->UnlockCoin(output);
}

std::set<COutPoint> WalletModel::listLockedCoins()
{
    LOCK(wallet->cs_wallet);
    return wallet->ListLockedCoins();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void WalletModel::loadReceiveRequests(std::vector<std::string>& vReceiveRequests)
{
<<<<<<< HEAD
    LOCK(wallet->cs_wallet);
    for (const PAIRTYPE(CTxDestination, CAddressBookData) & item : wallet->mapAddressBook)
        for (const PAIRTYPE(std::string, std::string) & item2 : item.second.destdata)
            if (item2.first.size() > 2 && item2.first.substr(0, 2) == "rr") // receive request
                vReceiveRequests.push_back(item2.second);
=======
    vReceiveRequests = wallet->GetDestValues("rr"); // receive request
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool WalletModel::saveReceiveRequest(const std::string& sAddress, const int64_t nId, const std::string& sRequest)
{
<<<<<<< HEAD
    CTxDestination dest = CBitcoinAddress(sAddress).Get();
=======
    CTxDestination dest = DecodeDestination(sAddress);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    std::stringstream ss;
    ss << nId;
    std::string key = "rr" + ss.str(); // "rr" prefix = "receive request" in destdata

    LOCK(wallet->cs_wallet);
    if (sRequest.empty())
        return wallet->EraseDestData(dest, key);
    else
        return wallet->AddDestData(dest, key, sRequest);
}

<<<<<<< HEAD
bool WalletModel::isMine(CBitcoinAddress address)
{
    return IsMine(*wallet, address.Get());
}

StakingStatusError WalletModel::getStakingStatusError(QString& error)
{
    /* {
        bool fMintable = pwalletMain->MintableCoins();
        CAmount balance = pwalletMain->GetSpendableBalance();
        if (!fMintable || nReserveBalance > balance) {
            if (balance < Params().MinimumStakeAmount()) {
                error = "\nBalance is under the minimum 400,000 staking threshold.\nPlease send more PRCY to this wallet.\n";
                return StakingStatusError::STAKING_OK;
            }
            if (nReserveBalance > balance || (balance > nReserveBalance && balance - nReserveBalance < Params().MinimumStakeAmount())) {
                error = "Reserve balance is too high.\nPlease lower it in order to turn staking on.";
                return StakingStatusError::RESERVE_TOO_HIGH;
            }
            if (!fMintable) {
                if (balance > Params().MinimumStakeAmount()) {
                    //10 is to cover transaction fees
                    if (balance >= Params().MinimumStakeAmount() + 10*COIN) {
                        error = "Not enough mintable coins.\nDo you want to merge & make a sent-to-yourself transaction to make the wallet stakable?";
                        return StakingStatusError::UTXO_UNDER_THRESHOLD;
                    }
                }
            }
        }
    }*/
    return StakingStatusError::STAKING_OK;
}

void WalletModel::generateCoins(bool fGenerate, int nGenProcLimit)
{
    GeneratePrcycoins(fGenerate, pwalletMain, nGenProcLimit);
    if (false /*if regtest*/ && fGenerate) {
        //regtest generate
    } else {
        GeneratePrcycoins(fGenerate, pwalletMain, nGenProcLimit);
    }
}

QAbstractTableModel* WalletModel::getTxTableModel()
{
    if (!txTableModel) {
        return NULL;
    } else
        return txTableModel;
}

namespace WalletUtil
{
std::map<QString, QString> getTx(CWallet* wallet, uint256 hash)
{
    return getTx(wallet, *wallet->GetWalletTx(hash));
}

vector<std::map<QString, QString> > getTXs(CWallet* wallet)
{
    vector<std::map<QString, QString> > txs;
    if (!wallet || wallet->IsLocked()) return txs;
    std::map<uint256, CWalletTx> txMap = wallet->mapWallet;
    {
        LOCK2(cs_main, wallet->cs_wallet);
        for (std::map<uint256, CWalletTx>::iterator tx = txMap.begin(); tx != txMap.end(); ++tx) {
            if (tx->second.GetDepthInMainChain() > 0) {
                txs.push_back(getTx(wallet, tx->second));
            }
        }
    }

    return txs;
}

std::map<QString, QString> getTx(CWallet* wallet, CWalletTx tx)
{

    // get stx amount
    CAmount totalamount = CAmount(0);
    CAmount totalIn = 0;
    if (wallet && !wallet->IsLocked()) {
        for (CTxIn in: tx.vin) {
            COutPoint prevout = wallet->findMyOutPoint(in);
            map<uint256, CWalletTx>::const_iterator mi = wallet->mapWallet.find(prevout.hash);
            if (mi != wallet->mapWallet.end()) {
                const CWalletTx& prev = (*mi).second;
                if (prevout.n < prev.vout.size()) {
                    if (wallet->IsMine(prev.vout[prevout.n])) {
                        CAmount decodedAmount = 0;
                        CKey blind;
                        pwalletMain->RevealTxOutAmount(prev, prev.vout[prevout.n], decodedAmount, blind);
                        totalIn += decodedAmount;
                    }
                }
            }
        }
    }
    CAmount firstOut = 0;
    if (wallet && !wallet->IsLocked()) {
        for (CTxOut out: tx.vout){
            CAmount vamount;
            CKey blind;
            if (wallet->IsMine(out) && wallet->RevealTxOutAmount(tx,out,vamount, blind)) {
                if (vamount != 0 && firstOut == 0) {
                    firstOut = vamount;
                }
                totalamount+=vamount;   //this is the total output
            }
        }
    }
    QList<TransactionRecord> decomposedTx = TransactionRecord::decomposeTransaction(wallet, tx);
    std::string txHash = tx.GetHash().GetHex();
    QList<QString> addressBook = getAddressBookData(wallet);
    std::map<QString, QString> txData;
    
    if (tx.hashBlock != 0) {
        BlockMap::iterator mi = mapBlockIndex.find(tx.hashBlock);
        if (mi != mapBlockIndex.end() && (*mi).second) {
            CBlockIndex* pindex = (*mi).second;
            if (chainActive.Contains(pindex))
                txData["confirmations"] = QString::number(1 + chainActive.Height() - pindex->nHeight);
            else
                txData["confirmations"] = QString::number(0);
        }
    }

    for (TransactionRecord TxRecord : decomposedTx) {
        txData["date"] = QString(GUIUtil::dateTimeStr(TxRecord.time));
        // if address is in book, use data from book, else use data from transaction
        txData["address"]=""; 
//        for (QString addressBookEntry : addressBook)
//            if (addressBookEntry.contains(TxRecord.address.c_str())) {
//                txData["address"] = addressBookEntry;
//                wallet->addrToTxHashMap[addressBookEntry.toStdString()] = txHash;
//            }
        if (!txData["address"].length()) {
            txData["address"] = QString(TxRecord.address.c_str());
            wallet->addrToTxHashMap[TxRecord.address] = txHash;
        }

        txData["amount"] = BitcoinUnits::format(0, totalamount); //absolute value of total amount
        txData["id"] = QString(TxRecord.hash.GetHex().c_str());
        // parse transaction type
        switch (TxRecord.type) {
        case 1:
            txData["type"] = QString("Mined");
            txData["amount"] = BitcoinUnits::format(0, totalamount - totalIn); //absolute value of total amount
            return txData;
            break;
        case TransactionRecord::SendToSelf:
            txData["type"] = QString("Payment to yourself");
            txData["amount"] = BitcoinUnits::format(0, TxRecord.debit); //absolute value of total amount
            return txData;
            break;
        case TransactionRecord::SendToAddress:
        case TransactionRecord::SendToOther:
            txData["type"] = QString("Sent");
            txData["amount"] = BitcoinUnits::format(0, totalIn - totalamount - tx.nTxFee); //absolute value of total amount
            return txData;
            break;
        case 0:
        case TransactionRecord::RecvWithAddress:
        case TransactionRecord::RecvFromOther:
        case TransactionRecord::RecvWithObfuscation:
            txData["type"] = QString("Received");
            break;
        case 2:
            txData["type"] = QString("Minted");
            txData["amount"] = BitcoinUnits::format(0,  totalamount - totalIn); //absolute value of total amount
            break;
        case TransactionRecord::MNReward:
            txData["type"] = QString("Masternode");
            txData["amount"] = BitcoinUnits::format(0,  TxRecord.credit); //absolute value of total amount
            break;     
        default:
            txData["type"] = QString("Payment");
            //txData["type"] = QString("Unknown");
        }
    }
    return txData;
}

QList<QString> getAddressBookData(CWallet* wallet)
{
    std::map<CTxDestination, CAddressBookData> mapAddressBook = pwalletMain->mapAddressBook;
    QList<QString> AddressBookData;
    for (std::map<CTxDestination, CAddressBookData>::iterator address = mapAddressBook.begin(); address != mapAddressBook.end(); ++address) {
        QString desc = address->second.name.c_str();
        QString addressHash = CBitcoinAddress(address->first).ToString().c_str();
        if (desc.length())
            AddressBookData.push_front(desc + " | " + addressHash);
        else
            AddressBookData.push_front(addressHash);
       
    }
    return AddressBookData;
}

} // namespace WalletUtil
=======
bool WalletModel::isMine(const CWDestination& address)
{
    return IsMine(*wallet, address);
}

bool WalletModel::isMine(const QString& addressStr)
{
    return IsMine(*wallet, DecodeDestination(addressStr.toStdString()));
}

bool WalletModel::IsShieldedDestination(const CWDestination& address)
{
    return boost::get<libzcash::SaplingPaymentAddress>(&address);
}

bool WalletModel::isUsed(CTxDestination address)
{
    return wallet->IsUsed(address);
}

Optional<QString> WalletModel::getShieldedAddressFromSpendDesc(const uint256& txHash, int index)
{
    Optional<libzcash::SaplingPaymentAddress> opAddr = wallet->GetSaplingScriptPubKeyMan()->GetAddressFromInputIfPossible(txHash, index);
    return opAddr ? Optional<QString>(QString::fromStdString(KeyIO::EncodePaymentAddress(*opAddr))) : nullopt;
}

void WalletModel::setClientModel(ClientModel* client_model)
{
    m_client_model = client_model;
}

uint256 WalletModel::getLastBlockProcessed() const
{
    return m_client_model ? m_client_model->getLastBlockProcessed() : UINT256_ZERO;
}

int WalletModel::getLastBlockProcessedNum() const
{
    return m_client_model ? m_client_model->getLastBlockProcessedHeight() : 0;
}

CAmount WalletModel::getNetMinFee()
{   // future: unify minimum required fee.
    return GetRequiredFee(1000);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
