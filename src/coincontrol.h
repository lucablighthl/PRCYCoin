// Copyright (c) 2011-2013 The Bitcoin developers
// Copyright (c) 2014-2016 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COINCONTROL_H
#define BITCOIN_COINCONTROL_H

<<<<<<< HEAD
#include "primitives/transaction.h"
#include "script/standard.h"
=======
#include "policy/feerate.h"
#include "primitives/transaction.h"
#include "script/standard.h"
#include <unordered_set>

class OutPointWrapper {
public:
    BaseOutPoint outPoint;
    CAmount value;
    bool isP2CS;

    bool operator<(const OutPointWrapper& obj2) const {
        return this->outPoint < obj2.outPoint;
    }

    bool operator==(const OutPointWrapper& obj2) const {
        return this->outPoint == obj2.outPoint;
    }
};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** Coin Control Features. */
class CCoinControl
{
public:
<<<<<<< HEAD
    CTxDestination destChange;
    CPubKey receiver;
    CKey txPriv;//for change UTXO
    bool useObfuScation;
    bool useSwiftTX;
    bool fSplitBlock;
    int nSplitBlock;
    //! If false, allows unselected inputs, but requires all selected inputs be used
    bool fAllowOtherInputs;
    //! Includes watch only addresses which match the ISMINE_WATCH_SOLVABLE criteria
    bool fAllowWatchOnly;
    //! Minimum absolute fee (not per kilobyte)
    CAmount nMinimumTotalFee;
=======
    CTxDestination destChange = CNoDestination();
    //! If false, allows unselected inputs, but requires all selected inputs be used
    bool fAllowOtherInputs;
    //! Includes watch only addresses which are solvable
    bool fAllowWatchOnly;
    //! Minimum absolute fee (not per kilobyte)
    CAmount nMinimumTotalFee;
    //! Override estimated feerate
    bool fOverrideFeeRate;
    //! Feerate to use if overrideFeeRate is true
    CFeeRate nFeeRate;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CCoinControl()
    {
        SetNull();
    }

    void SetNull()
    {
        destChange = CNoDestination();
        setSelected.clear();
<<<<<<< HEAD
        useSwiftTX = false;
        useObfuScation = false;
        fAllowOtherInputs = false;
        fAllowWatchOnly = true;
        nMinimumTotalFee = 0;
        fSplitBlock = false;
        nSplitBlock = 1;
=======
        fAllowOtherInputs = false;
        fAllowWatchOnly = false;
        nMinimumTotalFee = 0;
        nFeeRate = CFeeRate(0);
        fOverrideFeeRate = false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    bool HasSelected() const
    {
<<<<<<< HEAD
        return (setSelected.size() > 0);
    }

    bool IsSelected(const uint256& hash, unsigned int n) const
    {
        COutPoint outpt(hash, n);
        return (setSelected.count(outpt) > 0);
    }

    void Select(const COutPoint& output)
    {
        setSelected.insert(output);
    }

    void UnSelect(const COutPoint& output)
    {
        setSelected.erase(output);
=======
        return (!setSelected.empty());
    }

    bool IsSelected(const BaseOutPoint& output) const
    {
        return (setSelected.count(OutPointWrapper{output, 0, false}) > 0);
    }

    void Select(const BaseOutPoint& output, CAmount value = 0, bool isP2CS = false)
    {
        setSelected.insert(OutPointWrapper{output, value, isP2CS});
    }

    void UnSelect(const BaseOutPoint& output)
    {
        setSelected.erase(OutPointWrapper{output, 0, false});
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    void UnSelectAll()
    {
        setSelected.clear();
    }

<<<<<<< HEAD
    void ListSelected(std::vector<COutPoint>& vOutpoints)
=======
    void ListSelected(std::vector<OutPointWrapper>& vOutpoints) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        vOutpoints.assign(setSelected.begin(), setSelected.end());
    }

<<<<<<< HEAD
    unsigned int QuantitySelected()
=======
    unsigned int QuantitySelected() const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        return setSelected.size();
    }

<<<<<<< HEAD
    void SetSelection(std::set<COutPoint> setSelected)
    {
        this->setSelected.clear();
        this->setSelected = setSelected;
    }

private:
    std::set<COutPoint> setSelected;
=======
private:

    struct SimpleOutpointHash {
        size_t operator() (const OutPointWrapper& obj) const {
            return (UintToArith256(obj.outPoint.hash) + obj.outPoint.n).GetLow64();
        }
    };

    std::unordered_set<OutPointWrapper, SimpleOutpointHash> setSelected;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_COINCONTROL_H
