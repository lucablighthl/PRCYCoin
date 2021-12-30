<<<<<<< HEAD
// Copyright (c) 2011-2013 The Bitcoin developers
=======
// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_TRANSACTIONRECORD_H
#define BITCOIN_QT_TRANSACTIONRECORD_H

#include "amount.h"
<<<<<<< HEAD
=======
#include "script/script.h"
#include "optional.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "uint256.h"

#include <QList>
#include <QString>

class CWallet;
class CWalletTx;

/** UI model for transaction status. The transaction status is the part of a transaction that will change over time.
 */
class TransactionStatus
{
public:
    TransactionStatus() : countsForBalance(false), sortKey(""),
<<<<<<< HEAD
                          matures_in(0), status(Offline), depth(0), open_for(0), cur_num_blocks(-1)
    {
    }

    enum Status {
        Confirmed, /**< Have 6 or more confirmations (normal tx) or fully mature (mined tx) **/
        /// Normal (sent/received) transactions
        OpenUntilDate,  /**< Transaction not yet final, waiting for date */
        OpenUntilBlock, /**< Transaction not yet final, waiting for block */
        Offline,        /**< Not sent to any other nodes **/
=======
                          matures_in(0), status(Unconfirmed), depth(0), open_for(0), cur_num_blocks(-1)
    {
    }

    enum Status : uint16_t{
        Confirmed = 0, /**< Have 6 or more confirmations (normal tx) or fully mature (mined tx) **/
        /// Normal (sent/received) transactions
        OpenUntilDate,  /**< Transaction not yet final, waiting for date */
        OpenUntilBlock, /**< Transaction not yet final, waiting for block */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        Unconfirmed,    /**< Not yet mined into a block **/
        Confirming,     /**< Confirmed, but waiting for the recommended number of confirmations **/
        Conflicted,     /**< Conflicts with other transaction or mempool **/
        /// Generated (mined) transactions
        Immature,       /**< Mined but waiting for maturity */
<<<<<<< HEAD
        MaturesWarning, /**< Transaction will likely not mature because no nodes have confirmed */
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        NotAccepted     /**< Mined but not accepted */
    };

    /// Transaction counts towards available balance
    bool countsForBalance;
    /// Sorting key based on status
    std::string sortKey;

    /** @name Generated (mined) transactions
       @{*/
    int matures_in;
    /**@}*/

    /** @name Reported status
       @{*/
    Status status;
    qint64 depth;
    qint64 open_for; /**< Timestamp if status==OpenUntilDate, otherwise number
                      of additional blocks that need to be mined before
                      finalization */
    /**@}*/

    /** Current number of blocks (to know whether cached status is still valid) */
    int cur_num_blocks;

<<<<<<< HEAD
    //** Know when to update transaction for ix locks **/
    int cur_num_ix_locks;
=======
    bool needsUpdate;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

/** UI model for a transaction. A core transaction can be represented by multiple UI transactions if it has
    multiple outputs.
 */
class TransactionRecord
{
public:
<<<<<<< HEAD
    enum Type {
        Other,
        Generated,
        StakeMint,
=======
    enum Type : uint16_t {
        Other = 0,
        Generated,
        StakeMint,
        StakeZPIV,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        SendToAddress,
        SendToOther,
        RecvWithAddress,
        MNReward,
<<<<<<< HEAD
        RecvFromOther,
        SendToSelf,
        RecvWithObfuscation,
        ObfuscationDenominate,
        ObfuscationCollateralPayment,
        ObfuscationMakeCollaterals,
        ObfuscationCreateDenominations,
        Obfuscated
=======
        BudgetPayment,
        RecvFromOther,
        SendToSelf,
        ZerocoinMint,
        ZerocoinSpend,
        RecvFromZerocoinSpend,
        ZerocoinSpend_Change_zPiv,
        ZerocoinSpend_FromMe,
        StakeDelegated, // Received cold stake (owner)
        StakeHot, // Staked via a delegated P2CS.
        P2CSDelegation, // Non-spendable P2CS, staker side.
        P2CSDelegationSent, // Non-spendable P2CS delegated utxo. (coin-owner transferred ownership to external wallet)
        P2CSDelegationSentOwner, // Spendable P2CS delegated utxo. (coin-owner)
        P2CSUnlockOwner, // Coin-owner spent the delegated utxo
        P2CSUnlockStaker, // Staker watching the owner spent the delegated utxo
        SendToShielded, // Shielded send
        RecvWithShieldedAddress, // Shielded receive
        SendToSelfShieldedAddress, // Shielded send to self
        SendToSelfShieldToTransparent, // Unshield coins to self
        SendToSelfShieldToShieldChangeAddress, // Changing coins from one shielded address to another inside the wallet.
        SendToNobody // Burned PIVs, op_return output.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    };

    /** Number of confirmation recommended for accepting a transaction */
    static const int RecommendedNumConfirmations = 6;

<<<<<<< HEAD
    TransactionRecord() : hash(), time(0), type(Other), address(""), debit(0), credit(0), idx(0)
    {
    }

    TransactionRecord(uint256 hash, qint64 time) : hash(hash), time(time), type(Other), address(""), debit(0),
                                                   credit(0), idx(0)
    {
    }

    TransactionRecord(uint256 hash, qint64 time, Type type, const std::string& address, const CAmount& debit, const CAmount& credit) : hash(hash), time(time), type(type), address(address), debit(debit), credit(credit),
                                                                                                                                       idx(0)
=======
    TransactionRecord(unsigned int size) : hash(), time(0), type(Other), address(""), debit(0), credit(0), size(size), idx(0)
    {
    }

    TransactionRecord(uint256 hash, qint64 time, unsigned int size) : hash(hash), time(time), type(Other), address(""), debit(0),
                                                   credit(0), size(size), idx(0)
    {
    }

    TransactionRecord(uint256 hash, qint64 time, unsigned int size, Type type, const std::string& address, const CAmount& debit, const CAmount& credit) : hash(hash), time(time), type(type), address(address), debit(debit), credit(credit),
                                                                                                                                       size(size), idx(0)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
    }

    /** Decompose CWallet transaction to model transaction records.
     */
    static QList<TransactionRecord> decomposeTransaction(const CWallet* wallet, const CWalletTx& wtx);

<<<<<<< HEAD
=======
    /// Helpers
    static bool decomposeCoinStake(const CWallet* wallet, const CWalletTx& wtx,
                                   const CAmount& nCredit, const CAmount& nDebit,
                                   QList<TransactionRecord>& parts);

    static bool decomposeZcSpendTx(const CWallet* wallet, const CWalletTx& wtx,
                                   const CAmount& nCredit, const CAmount& nDebit,
                                   QList<TransactionRecord>& parts);

    static bool decomposeP2CS(const CWallet* wallet, const CWalletTx& wtx,
                                    const CAmount& nCredit, const CAmount& nDebit,
                                    QList<TransactionRecord>& parts);

    static bool decomposeCreditTransaction(const CWallet* wallet, const CWalletTx& wtx,
                                    QList<TransactionRecord>& parts);

    static bool decomposeSendToSelfTransaction(const CWalletTx& wtx, const CAmount& nCredit,
                                    const CAmount& nDebit, bool involvesWatchAddress,
                                    QList<TransactionRecord>& parts, const CWallet* wallet);

    static bool decomposeDebitTransaction(const CWallet* wallet, const CWalletTx& wtx,
                                                      const CAmount& nDebit, bool involvesWatchAddress,
                                                      QList<TransactionRecord>& parts);

    static bool decomposeShieldedDebitTransaction(const CWallet* wallet, const CWalletTx& wtx, CAmount nTxFee,
                                                  bool involvesWatchAddress, QList<TransactionRecord>& parts);

    static std::string getValueOrReturnEmpty(const std::map<std::string, std::string>& mapValue, const std::string& key);
    static void loadHotOrColdStakeOrContract(const CWallet* wallet, const CWalletTx& wtx,
                                            TransactionRecord& record, bool isContract = false);
    static void loadUnlockColdStake(const CWallet* wallet, const CWalletTx& wtx, TransactionRecord& record);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /** @name Immutable transaction attributes
      @{*/
    uint256 hash;
    qint64 time;
    Type type;
    std::string address;
    CAmount debit;
    CAmount credit;
<<<<<<< HEAD
=======
    unsigned int size;
    Optional<CAmount> shieldedCredit{nullopt};
    Optional<std::string> memo{nullopt};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /**@}*/

    /** Subtransaction index, for sort key */
    int idx;

    /** Status: can change with block chain update */
    TransactionStatus status;

    /** Whether the transaction was sent/received with a watch-only address */
<<<<<<< HEAD
    bool involvesWatchAddress;

    /** Return the unique identifier for this transaction (part) */
    QString getTxID() const;
=======
    bool involvesWatchAddress{false};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    /** Return the output index of the subtransaction  */
    int getOutputIndex() const;

    /** Update status from core wallet tx.
     */
<<<<<<< HEAD
    void updateStatus(const CWalletTx& wtx);

    /** Return whether a status update is needed.
     */
    bool statusUpdateNeeded();
=======
    void updateStatus(const CWalletTx& wtx, int chainHeight);

    /** Return whether a status update is needed.
     */
    bool statusUpdateNeeded(int blockHeight) const;

    /** Return transaction status
     */
    std::string statusToString();

    /** Return true if the tx is a coinstake
     */
    bool isCoinStake() const;

    /** Return true if the tx is a MN reward */
    bool isMNReward() const;

    /** Return true if the tx is a any cold staking type tx.
     */
    bool isAnyColdStakingType() const;

    /** Return true if the tx hash is null and/or if the size is 0
     */
    bool isNull() const;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_QT_TRANSACTIONRECORD_H
