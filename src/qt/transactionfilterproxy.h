// Copyright (c) 2011-2013 The Bitcoin developers
<<<<<<< HEAD
// Distributed under the MIT/X11 software license, see the accompanying
=======
// Copyright (c) 2017-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_TRANSACTIONFILTERPROXY_H
#define BITCOIN_QT_TRANSACTIONFILTERPROXY_H

#include "amount.h"

#include <QDateTime>
#include <QSortFilterProxyModel>

/** Filter the transaction list according to pre-specified rules. */
class TransactionFilterProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit TransactionFilterProxy(QObject* parent = 0);

    /** Earliest date that can be represented (far in the past) */
    static const QDateTime MIN_DATE;
    /** Last date that can be represented (far in the future) */
    static const QDateTime MAX_DATE;
    /** Type filter bit field (all types) */
    static const quint32 ALL_TYPES = 0xFFFFFFFF;
<<<<<<< HEAD
    /** Type filter bit field (all types but Obfuscation-SPAM ... enum 0-13 are common) */
    static const quint32 COMMON_TYPES = 0x000003FFF;
=======
    /** Type filter bit field (all types but Obfuscation-SPAM ... enum 0-14 are common) */
    static const quint32 COMMON_TYPES = 0x0005FFFF;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    static quint32 TYPE(int type) { return 1 << type; }

    enum WatchOnlyFilter {
        WatchOnlyFilter_All,
        WatchOnlyFilter_Yes,
        WatchOnlyFilter_No
    };

    void setDateRange(const QDateTime& from, const QDateTime& to);
<<<<<<< HEAD
    void setAddressPrefix(const QString& addrPrefix);
=======
    void clearDateRange() {
        if (dateFrom != MIN_DATE || dateTo == MAX_DATE)
            setDateRange(MIN_DATE, MAX_DATE);
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    /**
      @note Type filter takes a bit field created with TYPE() or ALL_TYPES
     */
    void setTypeFilter(quint32 modes);
    void setMinAmount(const CAmount& minimum);
    void setWatchOnlyFilter(WatchOnlyFilter filter);

    /** Set maximum number of rows returned, -1 if unlimited. */
    void setLimit(int limit);

    /** Set whether to show conflicted transactions. */
    void setShowInactive(bool showInactive);

    /** Set whether to hide orphan stakes. */
    void setHideOrphans(bool fHide);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    static bool isOrphan(const int status, const int type);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

private:
    QDateTime dateFrom;
    QDateTime dateTo;
<<<<<<< HEAD
    QString addrPrefix;
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    quint32 typeFilter;
    WatchOnlyFilter watchOnlyFilter;
    CAmount minAmount;
    int limitRows;
    bool showInactive;
<<<<<<< HEAD
    bool fHideOrphans;
=======
    bool fHideOrphans = true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_QT_TRANSACTIONFILTERPROXY_H
