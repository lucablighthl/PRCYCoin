// Copyright (c) 2011-2013 The Bitcoin developers
<<<<<<< HEAD
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include "transactionfilterproxy.h"

#include "transactionrecord.h"
#include "transactiontablemodel.h"

#include <cstdlib>

<<<<<<< HEAD
#include <QDateTime>

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Earliest date that can be represented (far in the past)
const QDateTime TransactionFilterProxy::MIN_DATE = QDateTime::fromTime_t(0);
// Last date that can be represented (far in the future)
const QDateTime TransactionFilterProxy::MAX_DATE = QDateTime::fromTime_t(0xFFFFFFFF);

TransactionFilterProxy::TransactionFilterProxy(QObject* parent) : QSortFilterProxyModel(parent),
                                                                  dateFrom(MIN_DATE),
                                                                  dateTo(MAX_DATE),
<<<<<<< HEAD
                                                                  addrPrefix(),
                                                                  typeFilter(COMMON_TYPES),
=======
                                                                  typeFilter(ALL_TYPES),
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                                                                  watchOnlyFilter(WatchOnlyFilter_All),
                                                                  minAmount(0),
                                                                  limitRows(-1),
                                                                  showInactive(true),
<<<<<<< HEAD
                                                                  fHideOrphans(false)
=======
                                                                  fHideOrphans(true)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
}

bool TransactionFilterProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

<<<<<<< HEAD
    int type = index.data(TransactionTableModel::TypeRole).toInt();
    QDateTime datetime = index.data(TransactionTableModel::DateRole).toDateTime();
    bool involvesWatchAddress = index.data(TransactionTableModel::WatchonlyRole).toBool();
    QString address = index.data(TransactionTableModel::AddressRole).toString();
    QString label = index.data(TransactionTableModel::LabelRole).toString();
    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());
    int status = index.data(TransactionTableModel::StatusRole).toInt();

    if (!showInactive && status == TransactionStatus::Conflicted)
        return false;
    if (fHideOrphans && isOrphan(status, type))
        return false;
    if (!(TYPE(type) & typeFilter))
        return false;
=======
    int status = index.data(TransactionTableModel::StatusRole).toInt();
    if (!showInactive && status == TransactionStatus::Conflicted)
        return false;

    int type = index.data(TransactionTableModel::TypeRole).toInt();
    if (fHideOrphans && isOrphan(status, type)) return false;
    if (!(bool)(TYPE(type) & typeFilter)) return false;

    bool involvesWatchAddress = index.data(TransactionTableModel::WatchonlyRole).toBool();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (involvesWatchAddress && watchOnlyFilter == WatchOnlyFilter_No)
        return false;
    if (!involvesWatchAddress && watchOnlyFilter == WatchOnlyFilter_Yes)
        return false;
<<<<<<< HEAD
    if (datetime < dateFrom || datetime > dateTo)
        return false;
    if (!address.contains(addrPrefix, Qt::CaseInsensitive) && !label.contains(addrPrefix, Qt::CaseInsensitive))
        return false;
=======

    QDateTime datetime = index.data(TransactionTableModel::DateRole).toDateTime();
    if (datetime < dateFrom || datetime > dateTo)
        return false;

    qint64 amount = llabs(index.data(TransactionTableModel::AmountRole).toLongLong());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    if (amount < minAmount)
        return false;

    return true;
}

void TransactionFilterProxy::setDateRange(const QDateTime& from, const QDateTime& to)
{
<<<<<<< HEAD
=======
    if (from == this->dateFrom && to == this->dateTo)
        return; // No need to set the range.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    this->dateFrom = from;
    this->dateTo = to;
    invalidateFilter();
}

<<<<<<< HEAD
void TransactionFilterProxy::setHideOrphans(bool fHide)
{
    this->fHideOrphans = fHide;
    invalidateFilter();
}

void TransactionFilterProxy::setAddressPrefix(const QString& addrPrefix)
{
    this->addrPrefix = addrPrefix;
    invalidateFilter();
}

void TransactionFilterProxy::setTypeFilter(quint32 modes)
{
=======
void TransactionFilterProxy::setTypeFilter(quint32 modes)
{
    if (typeFilter == modes) return;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    this->typeFilter = modes;
    invalidateFilter();
}

void TransactionFilterProxy::setMinAmount(const CAmount& minimum)
{
    this->minAmount = minimum;
    invalidateFilter();
}

void TransactionFilterProxy::setWatchOnlyFilter(WatchOnlyFilter filter)
{
    this->watchOnlyFilter = filter;
    invalidateFilter();
}

void TransactionFilterProxy::setLimit(int limit)
{
    this->limitRows = limit;
}

void TransactionFilterProxy::setShowInactive(bool showInactive)
{
    this->showInactive = showInactive;
    invalidateFilter();
}

<<<<<<< HEAD
=======
void TransactionFilterProxy::setHideOrphans(bool fHide)
{
    this->fHideOrphans = fHide;
    invalidateFilter();
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
int TransactionFilterProxy::rowCount(const QModelIndex& parent) const
{
    if (limitRows != -1) {
        return std::min(QSortFilterProxyModel::rowCount(parent), limitRows);
    } else {
        return QSortFilterProxyModel::rowCount(parent);
    }
}

bool TransactionFilterProxy::isOrphan(const int status, const int type)
{
<<<<<<< HEAD
    return ( (type == TransactionRecord::Generated || type == TransactionRecord::StakeMint || type == TransactionRecord::MNReward)
=======
    return ( (type == TransactionRecord::Generated || type == TransactionRecord::StakeMint ||
            type == TransactionRecord::StakeZPIV || type == TransactionRecord::MNReward || type == TransactionRecord::BudgetPayment)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            && (status == TransactionStatus::Conflicted || status == TransactionStatus::NotAccepted) );
}
