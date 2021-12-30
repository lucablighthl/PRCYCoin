// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "bitcoinunits.h"
#include "chainparams.h"
<<<<<<< HEAD
=======
#include "policy/feerate.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "primitives/transaction.h"

#include <QSettings>
#include <QStringList>

<<<<<<< HEAD
=======
#include <iostream>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
BitcoinUnits::BitcoinUnits(QObject* parent) : QAbstractListModel(parent),
                                              unitlist(availableUnits())
{
}

QList<BitcoinUnits::Unit> BitcoinUnits::availableUnits()
{
    QList<BitcoinUnits::Unit> unitlist;
<<<<<<< HEAD
    unitlist.append(PRCY);
    unitlist.append(mPRCY);
    unitlist.append(uPRCY);
=======
    unitlist.append(PIV);
    unitlist.append(mPIV);
    unitlist.append(uPIV);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return unitlist;
}

bool BitcoinUnits::valid(int unit)
{
    switch (unit) {
<<<<<<< HEAD
    case PRCY:
    case mPRCY:
    case uPRCY:
=======
    case PIV:
    case mPIV:
    case uPIV:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return true;
    default:
        return false;
    }
}

QString BitcoinUnits::id(int unit)
{
    switch (unit) {
<<<<<<< HEAD
    case PRCY:
        return QString("prcycoin");
    case mPRCY:
        return QString("mprcycoin");
    case uPRCY:
        return QString::fromUtf8("uprcycoin");
=======
    case PIV:
        return QString("pivx");
    case mPIV:
        return QString("mpivx");
    case uPIV:
        return QString::fromUtf8("upivx");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    default:
        return QString("???");
    }
}

<<<<<<< HEAD
QString BitcoinUnits::name(int unit)
{
    if (Params().NetworkID() == CBaseChainParams::MAIN) {
        switch (unit) {
        case PRCY:
            return QString("PRCY");
        case mPRCY:
            return QString("mPRCY");
        case uPRCY:
            return QString::fromUtf8("μPRCY");
=======
QString BitcoinUnits::name(int unit, bool isZpiv)
{
    const QString CURR_UNIT = QString(CURRENCY_UNIT.c_str());
    QString z = "";
    if(isZpiv) z = "z";
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        switch (unit) {
        case PIV:
            return z + CURR_UNIT;
        case mPIV:
            return z + QString("m") + CURR_UNIT;
        case uPIV:
            return z + QString::fromUtf8("μ") + CURR_UNIT;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        default:
            return QString("???");
        }
    } else {
        switch (unit) {
<<<<<<< HEAD
        case PRCY:
            return QString("tPRCY");
        case mPRCY:
            return QString("mtPRCY");
        case uPRCY:
            return QString::fromUtf8("μtPRCY");
=======
        case PIV:
            return z + QString("t") + CURR_UNIT;
        case mPIV:
            return z + QString("mt") + CURR_UNIT;
        case uPIV:
            return z + QString::fromUtf8("μt") + CURR_UNIT;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        default:
            return QString("???");
        }
    }
}

QString BitcoinUnits::description(int unit)
{
<<<<<<< HEAD
    if (Params().NetworkID() == CBaseChainParams::MAIN) {
        switch (unit) {
        case PRCY:
            return QString("PRCY");
        case mPRCY:
            return QString("Milli-PRCY (1 / 1" THIN_SP_UTF8 "000)");
        case uPRCY:
            return QString("Micro-PRCY (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
=======
    const QString CURR_UNIT = QString(CURRENCY_UNIT.c_str());
    if (Params().NetworkIDString() == CBaseChainParams::MAIN) {
        switch (unit) {
        case PIV:
            return CURR_UNIT;
        case mPIV:
            return QString("Milli-") + CURR_UNIT + QString(" (1 / 1" THIN_SP_UTF8 "000)");
        case uPIV:
            return QString("Micro-") + CURR_UNIT + QString(" (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        default:
            return QString("???");
        }
    } else {
        switch (unit) {
<<<<<<< HEAD
        case PRCY:
            return QString("TestPRCYs");
        case mPRCY:
            return QString("Milli-TestPRCY (1 / 1" THIN_SP_UTF8 "000)");
        case uPRCY:
            return QString("Micro-TestPRCY (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
=======
        case PIV:
            return QString("Test") + CURR_UNIT;
        case mPIV:
            return QString("Milli-Test") + CURR_UNIT + QString(" (1 / 1" THIN_SP_UTF8 "000)");
        case uPIV:
            return QString("Micro-Test") + CURR_UNIT + QString(" (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        default:
            return QString("???");
        }
    }
}

qint64 BitcoinUnits::factor(int unit)
{
    switch (unit) {
<<<<<<< HEAD
    case PRCY:
        return 100000000;
    case mPRCY:
        return 100000;
    case uPRCY:
=======
    case PIV:
        return 100000000;
    case mPIV:
        return 100000;
    case uPIV:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return 100;
    default:
        return 100000000;
    }
}

int BitcoinUnits::decimals(int unit)
{
    switch (unit) {
<<<<<<< HEAD
    case PRCY:
        return 8;
    case mPRCY:
        return 5;
    case uPRCY:
=======
    case PIV:
        return 8;
    case mPIV:
        return 5;
    case uPIV:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return 2;
    default:
        return 0;
    }
}

<<<<<<< HEAD
QString BitcoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if (!valid(unit))
        return QString(); // Refuse to format invalid unit
=======
QString BitcoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators, bool cleanRemainderZeros)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if (!valid(unit)){
        return QString(); // Refuse to format invalid unit
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');

    if (num_decimals <= 0)
        return quotient_str;

<<<<<<< HEAD
=======
    if(cleanRemainderZeros) {
        // Clean remainder
        QString cleanRemainder = remainder_str;
        for (int i = (remainder_str.length() - 1); i > 1; i--) {
            if (remainder_str.at(i) == QChar('0')) {
                cleanRemainder = cleanRemainder.left(cleanRemainder.lastIndexOf("0"));
            } else
                break;
        }
        return quotient_str + QString(".") + cleanRemainder;
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return quotient_str + QString(".") + remainder_str;
}


// TODO: Review all remaining calls to BitcoinUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: BtcoinUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString BitcoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString BitcoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
<<<<<<< HEAD
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString BitcoinUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
=======
    str.replace(QChar(THIN_SP_CP), QString(COMMA_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

QString BitcoinUnits::floorWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators, bool cleanRemainderZeros, bool isZPIV)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    QSettings settings;
    int digits = settings.value("digits").toInt();

<<<<<<< HEAD
    QString result = format(unit, amount, plussign, separators);
    if (decimals(unit) > digits) result.chop(decimals(unit) - digits);

    return result + QString(" ") + name(unit);
}

QString BitcoinUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(floorWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
=======
    QString result = format(unit, amount, plussign, separators, cleanRemainderZeros);
    if(decimals(unit) > digits) {
        if (!cleanRemainderZeros) {
            result.chop(decimals(unit) - digits);
        } else {
            int lenght = result.mid(result.indexOf("."), result.length() - 1).length() - 1;
            if (lenght > digits) {
                result.chop(lenght - digits);
            }
        }
    }

    return result + QString(" ") + name(unit, isZPIV);
}

QString BitcoinUnits::floorHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators, bool cleanRemainderZeros, bool isZPIV)
{
    QString str(floorWithUnit(unit, amount, plussign, separators, cleanRemainderZeros, isZPIV));
    str.replace(QChar(THIN_SP_CP), QString(COMMA_HTML));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}

bool BitcoinUnits::parse(int unit, const QString& value, CAmount* val_out)
{
    if (!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
<<<<<<< HEAD
    QStringList parts = removeSpaces(value).split(".");
=======
    QStringList parts = removeSpaces(value).replace(",", ".").split(".");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    if (parts.size() > 2) {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if (parts.size() > 1) {
        decimals = parts[1];
    }
    if (decimals.size() > num_decimals) {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if (str.size() > 18) {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if (val_out) {
        *val_out = retvalue;
    }
    return ok;
}

QString BitcoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (BitcoinUnits::valid(unit)) {
        amountTitle += " (" + BitcoinUnits::name(unit) + ")";
    }
    return amountTitle;
}

int BitcoinUnits::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant BitcoinUnits::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if (row >= 0 && row < unitlist.size()) {
        Unit unit = unitlist.at(row);
        switch (role) {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount BitcoinUnits::maxMoney()
{
<<<<<<< HEAD
    return Params().MaxMoneyOut();
=======
    return Params().GetConsensus().nMaxMoneyOut;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}
