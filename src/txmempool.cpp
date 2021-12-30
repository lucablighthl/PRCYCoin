// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2016-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "txmempool.h"

#include "clientversion.h"
<<<<<<< HEAD
#include "main.h"
#include "streams.h"
#include "util.h"
#include "utilmoneystr.h"
#include "version.h"

#include <boost/circular_buffer.hpp>

using namespace std;

CTxMemPoolEntry::CTxMemPoolEntry() : nFee(0), nTxSize(0), nModSize(0), nTime(0), dPriority(0.0)
{
    nHeight = MEMPOOL_HEIGHT;
}

CTxMemPoolEntry::CTxMemPoolEntry(const CTransaction& _tx, const CAmount& _nFee, int64_t _nTime, double _dPriority, unsigned int _nHeight) : tx(_tx), nFee(_nFee), nTime(_nTime), dPriority(_dPriority), nHeight(_nHeight)
{
    nTxSize = ::GetSerializeSize(tx, SER_NETWORK, PROTOCOL_VERSION);

    nModSize = tx.CalculateModifiedSize(nTxSize);
}

CTxMemPoolEntry::CTxMemPoolEntry(const CTxMemPoolEntry& other)
{
    *this = other;
}

double
CTxMemPoolEntry::GetPriority(unsigned int currentHeight) const
{
    return ::GetPriority(tx, currentHeight);
}

/**
 * Keep track of fee/priority for transactions confirmed within N blocks
 */
class CBlockAverage
{
private:
    boost::circular_buffer<CFeeRate> feeSamples;
    boost::circular_buffer<double> prioritySamples;

    template <typename T>
    std::vector<T> buf2vec(boost::circular_buffer<T> buf) const
    {
        std::vector<T> vec(buf.begin(), buf.end());
        return vec;
    }

public:
    CBlockAverage() : feeSamples(100), prioritySamples(100) {}

    void RecordFee(const CFeeRate& feeRate)
    {
        feeSamples.push_back(feeRate);
    }

    void RecordPriority(double priority)
    {
        prioritySamples.push_back(priority);
    }

    size_t FeeSamples() const { return feeSamples.size(); }
    size_t GetFeeSamples(std::vector<CFeeRate>& insertInto) const
    {
        for (const CFeeRate& f : feeSamples)
            insertInto.push_back(f);
        return feeSamples.size();
    }
    size_t PrioritySamples() const { return prioritySamples.size(); }
    size_t GetPrioritySamples(std::vector<double>& insertInto) const
    {
        for (double d : prioritySamples)
            insertInto.push_back(d);
        return prioritySamples.size();
    }

    /**
     * Used as belt-and-suspenders check when reading to detect
     * file corruption
     */
    static bool AreSane(const CFeeRate fee, const CFeeRate& minRelayFee)
    {
        if (fee < CFeeRate(0))
            return false;
        if (fee.GetFeePerK() > minRelayFee.GetFeePerK() * 10000)
            return false;
        return true;
    }
    static bool AreSane(const std::vector<CFeeRate>& vecFee, const CFeeRate& minRelayFee)
    {
        for (CFeeRate fee : vecFee) {
            if (!AreSane(fee, minRelayFee))
                return false;
        }
        return true;
    }
    static bool AreSane(const double priority)
    {
        return priority >= 0;
    }
    static bool AreSane(const std::vector<double> vecPriority)
    {
        for (double priority : vecPriority) {
            if (!AreSane(priority))
                return false;
        }
        return true;
    }

    void Write(CAutoFile& fileout) const
    {
        std::vector<CFeeRate> vecFee = buf2vec(feeSamples);
        fileout << vecFee;
        std::vector<double> vecPriority = buf2vec(prioritySamples);
        fileout << vecPriority;
    }

    void Read(CAutoFile& filein, const CFeeRate& minRelayFee)
    {
        std::vector<CFeeRate> vecFee;
        filein >> vecFee;
        if (AreSane(vecFee, minRelayFee))
            feeSamples.insert(feeSamples.end(), vecFee.begin(), vecFee.end());
        else
            throw runtime_error("Corrupt fee value in estimates file.");
        std::vector<double> vecPriority;
        filein >> vecPriority;
        if (AreSane(vecPriority))
            prioritySamples.insert(prioritySamples.end(), vecPriority.begin(), vecPriority.end());
        else
            throw runtime_error("Corrupt priority value in estimates file.");
        if (feeSamples.size() + prioritySamples.size() > 0)
            LogPrint("estimatefee", "Read %d fee samples and %d priority samples\n",
                feeSamples.size(), prioritySamples.size());
    }
};

class CMinerPolicyEstimator
{
private:
    /**
     * Records observed averages transactions that confirmed within one block, two blocks,
     * three blocks etc.
     */
    std::vector<CBlockAverage> history;
    std::vector<CFeeRate> sortedFeeSamples;
    std::vector<double> sortedPrioritySamples;

    int nBestSeenHeight;

    /**
     * nBlocksAgo is 0 based, i.e. transactions that confirmed in the highest seen block are
     * nBlocksAgo == 0, transactions in the block before that are nBlocksAgo == 1 etc.
     */
    void seenTxConfirm(const CFeeRate& feeRate, const CFeeRate& minRelayFee, double dPriority, int nBlocksAgo)
    {
        // Last entry records "everything else".
        int nBlocksTruncated = min(nBlocksAgo, (int)history.size() - 1);
        assert(nBlocksTruncated >= 0);

        // We need to guess why the transaction was included in a block-- either
        // because it is high-priority or because it has sufficient fees.
        bool sufficientFee = (feeRate > minRelayFee);
        bool sufficientPriority = AllowFree(dPriority);
        const char* assignedTo = "unassigned";
        if (sufficientFee && !sufficientPriority && CBlockAverage::AreSane(feeRate, minRelayFee)) {
            history[nBlocksTruncated].RecordFee(feeRate);
            assignedTo = "fee";
        } else if (sufficientPriority && !sufficientFee && CBlockAverage::AreSane(dPriority)) {
            history[nBlocksTruncated].RecordPriority(dPriority);
            assignedTo = "priority";
        } else {
            // Neither or both fee and priority sufficient to get confirmed:
            // don't know why they got confirmed.
        }
        LogPrint("estimatefee", "Seen TX confirm: %s : %s fee/%g priority, took %d blocks\n",
            assignedTo, feeRate.ToString(), dPriority, nBlocksAgo);
    }

public:
    CMinerPolicyEstimator(int nEntries) : nBestSeenHeight(0)
    {
        history.resize(nEntries);
    }

    void seenBlock(const std::vector<CTxMemPoolEntry>& entries, int nBlockHeight, const CFeeRate minRelayFee)
    {
        if (nBlockHeight <= nBestSeenHeight) {
            // Ignore side chains and re-orgs; assuming they are random
            // they don't affect the estimate.
            // And if an attacker can re-org the chain at will, then
            // you've got much bigger problems than "attacker can influence
            // transaction fees."
            return;
        }
        nBestSeenHeight = nBlockHeight;

        // Fill up the history buckets based on how long transactions took
        // to confirm.
        std::vector<std::vector<const CTxMemPoolEntry*> > entriesByConfirmations;
        entriesByConfirmations.resize(history.size());
        for (const CTxMemPoolEntry& entry : entries) {
            // How many blocks did it take for miners to include this transaction?
            int delta = nBlockHeight - entry.GetHeight();
            if (delta <= 0) {
                // Re-org made us lose height, this should only happen if we happen
                // to re-org on a difficulty transition point: very rare!
                continue;
            }
            if ((delta - 1) >= (int)history.size())
                delta = history.size(); // Last bucket is catch-all
            entriesByConfirmations.at(delta - 1).push_back(&entry);
        }
        for (size_t i = 0; i < entriesByConfirmations.size(); i++) {
            std::vector<const CTxMemPoolEntry*>& e = entriesByConfirmations.at(i);
            // Insert at most 10 random entries per bucket, otherwise a single block
            // can dominate an estimate:
            if (e.size() > 10) {
                std::random_shuffle(e.begin(), e.end());
                e.resize(10);
            }
            for (const CTxMemPoolEntry* entry : e) {
                // Fees are stored and reported as BTC-per-kb:
                CFeeRate feeRate(entry->GetFee(), entry->GetTxSize());
                double dPriority = entry->GetPriority(entry->GetHeight()); // Want priority when it went IN
                seenTxConfirm(feeRate, minRelayFee, dPriority, i);
            }
        }

        //After new samples are added, we have to clear the sorted lists,
        //so they'll be resorted the next time someone asks for an estimate
        sortedFeeSamples.clear();
        sortedPrioritySamples.clear();

        for (size_t i = 0; i < history.size(); i++) {
            if (history[i].FeeSamples() + history[i].PrioritySamples() > 0)
                LogPrint("estimatefee", "estimates: for confirming within %d blocks based on %d/%d samples, fee=%s, prio=%g\n",
                    i,
                    history[i].FeeSamples(), history[i].PrioritySamples(),
                    estimateFee(i + 1).ToString(), estimatePriority(i + 1));
        }
    }

    /**
     * Can return CFeeRate(0) if we don't have any data for that many blocks back. nBlocksToConfirm is 1 based.
     */
    CFeeRate estimateFee(int nBlocksToConfirm)
    {
        nBlocksToConfirm--;

        if (nBlocksToConfirm < 0 || nBlocksToConfirm >= (int)history.size())
            return CFeeRate(0);

        if (sortedFeeSamples.size() == 0) {
            for (size_t i = 0; i < history.size(); i++)
                history.at(i).GetFeeSamples(sortedFeeSamples);
            std::sort(sortedFeeSamples.begin(), sortedFeeSamples.end(),
                std::greater<CFeeRate>());
        }
        if (sortedFeeSamples.size() < 11) {
            // Eleven is Gavin's Favorite Number
            // ... but we also take a maximum of 10 samples per block so eleven means
            // we're getting samples from at least two different blocks
            return CFeeRate(0);
        }

        int nBucketSize = history.at(nBlocksToConfirm).FeeSamples();

        // Estimates should not increase as number of confirmations goes up,
        // but the estimates are noisy because confirmations happen discretely
        // in blocks. To smooth out the estimates, use all samples in the history
        // and use the nth highest where n is (number of samples in previous bucket +
        // half the samples in nBlocksToConfirm bucket):
        size_t nPrevSize = 0;
        for (int i = 0; i < nBlocksToConfirm; i++)
            nPrevSize += history.at(i).FeeSamples();
        size_t index = min(nPrevSize + nBucketSize / 2, sortedFeeSamples.size() - 1);
        return sortedFeeSamples[index];
    }
    double estimatePriority(int nBlocksToConfirm)
    {
        nBlocksToConfirm--;

        if (nBlocksToConfirm < 0 || nBlocksToConfirm >= (int)history.size())
            return -1;

        if (sortedPrioritySamples.size() == 0) {
            for (size_t i = 0; i < history.size(); i++)
                history.at(i).GetPrioritySamples(sortedPrioritySamples);
            std::sort(sortedPrioritySamples.begin(), sortedPrioritySamples.end(),
                std::greater<double>());
        }
        if (sortedPrioritySamples.size() < 11)
            return -1.0;

        int nBucketSize = history.at(nBlocksToConfirm).PrioritySamples();

        // Estimates should not increase as number of confirmations needed goes up,
        // but the estimates are noisy because confirmations happen discretely
        // in blocks. To smooth out the estimates, use all samples in the history
        // and use the nth highest where n is (number of samples in previous buckets +
        // half the samples in nBlocksToConfirm bucket).
        size_t nPrevSize = 0;
        for (int i = 0; i < nBlocksToConfirm; i++)
            nPrevSize += history.at(i).PrioritySamples();
        size_t index = min(nPrevSize + nBucketSize / 2, sortedPrioritySamples.size() - 1);
        return sortedPrioritySamples[index];
    }

    void Write(CAutoFile& fileout) const
    {
        fileout << nBestSeenHeight;
        fileout << history.size();
        for (const CBlockAverage& entry : history) {
            entry.Write(fileout);
        }
    }

    void Read(CAutoFile& filein, const CFeeRate& minRelayFee)
    {
        int nFileBestSeenHeight;
        filein >> nFileBestSeenHeight;
        size_t numEntries;
        filein >> numEntries;
        if (numEntries <= 0 || numEntries > 10000)
            throw runtime_error("Corrupt estimates file. Must have between 1 and 10k entries.");

        std::vector<CBlockAverage> fileHistory;

        for (size_t i = 0; i < numEntries; i++) {
            CBlockAverage entry;
            entry.Read(filein, minRelayFee);
            fileHistory.push_back(entry);
        }

        // Now that we've processed the entire fee estimate data file and not
        // thrown any errors, we can copy it to our history
        nBestSeenHeight = nFileBestSeenHeight;
        history = fileHistory;
        assert(history.size() > 0);
    }
};


CTxMemPool::CTxMemPool(const CFeeRate& _minRelayFee) : nTransactionsUpdated(0),
                                                       minRelayFee(_minRelayFee)
{
    // Sanity checks off by default for performance, because otherwise
    // accepting transactions becomes O(N^2) where N is the number
    // of transactions in the pool
    fSanityCheck = false;

    // 25 blocks is a compromise between using a lot of disk/memory and
    // trying to give accurate estimates to people who might be willing
    // to wait a day or two to save a fraction of a penny in fees.
    // Confirmation times for very-low-fee transactions that take more
    // than an hour or three to confirm are highly variable.
    minerPolicyEstimator = new CMinerPolicyEstimator(25);
=======
#include "bls/bls_wrapper.h"
#include "evo/deterministicmns.h"
#include "evo/specialtx_validation.h"
#include "evo/providertx.h"
#include "policy/fees.h"
#include "reverse_iterate.h"
#include "streams.h"
#include "timedata.h"
#include "util/system.h"
#include "utilmoneystr.h"
#include "utiltime.h"
#include "version.h"
#include "validation.h"
#include "validationinterface.h"


CTxMemPoolEntry::CTxMemPoolEntry(const CTransactionRef& _tx, const CAmount& _nFee,
                                 int64_t _nTime, unsigned int _entryHeight,
                                 bool _spendsCoinbaseOrCoinstake, unsigned int _sigOps) :
     tx(MakeTransactionRef(_tx)), nFee(_nFee), nTime(_nTime), entryHeight(_entryHeight),
     spendsCoinbaseOrCoinstake(_spendsCoinbaseOrCoinstake), sigOpCount(_sigOps)
{
    nTxSize = ::GetSerializeSize(*_tx, PROTOCOL_VERSION);
    nUsageSize = _tx->DynamicMemoryUsage();
    hasZerocoins = _tx->ContainsZerocoins();
    m_isShielded = _tx->IsShieldedTx();

    nCountWithDescendants = 1;
    nSizeWithDescendants = nTxSize;
    nModFeesWithDescendants = nFee;

    feeDelta = 0;

    nCountWithAncestors = 1;
    nSizeWithAncestors = nTxSize;
    nModFeesWithAncestors = nFee;
    nSigOpCountWithAncestors = sigOpCount;
}

void CTxMemPoolEntry::UpdateFeeDelta(int64_t newFeeDelta)
{
    nModFeesWithDescendants += newFeeDelta - feeDelta;
    nModFeesWithAncestors += newFeeDelta - feeDelta;
    feeDelta = newFeeDelta;
}

// Update the given tx for any in-mempool descendants.
// Assumes that setMemPoolChildren is correct for the given tx and all
// descendants.
void CTxMemPool::UpdateForDescendants(txiter updateIt, cacheMap &cachedDescendants, const std::set<uint256> &setExclude)
{
    setEntries stageEntries, setAllDescendants;
    stageEntries = GetMemPoolChildren(updateIt);

    while (!stageEntries.empty()) {
        const txiter cit = *stageEntries.begin();
        setAllDescendants.insert(cit);
        stageEntries.erase(cit);
        const setEntries &setChildren = GetMemPoolChildren(cit);
        for (const txiter& childEntry : setChildren) {
            cacheMap::iterator cacheIt = cachedDescendants.find(childEntry);
            if (cacheIt != cachedDescendants.end()) {
                // We've already calculated this one, just add the entries for this set
                // but don't traverse again.
                for (const txiter& cacheEntry : cacheIt->second) {
                    setAllDescendants.insert(cacheEntry);
                }
            } else if (!setAllDescendants.count(childEntry)) {
                // Schedule for later processing
                stageEntries.insert(childEntry);
            }
        }
    }
    // setAllDescendants now contains all in-mempool descendants of updateIt.
    // Update and add to cached descendant map
    int64_t modifySize = 0;
    CAmount modifyFee = 0;
    int64_t modifyCount = 0;
    for (const txiter& cit : setAllDescendants) {
        if (!setExclude.count(cit->GetTx().GetHash())) {
            modifySize += cit->GetTxSize();
            modifyFee += cit->GetModifiedFee();
            modifyCount++;
            cachedDescendants[updateIt].insert(cit);
            // Update ancestor state for each descendant
            mapTx.modify(cit, update_ancestor_state(updateIt->GetTxSize(), updateIt->GetModifiedFee(), 1, updateIt->GetSigOpCount()));
        }
    }
    mapTx.modify(updateIt, update_descendant_state(modifySize, modifyFee, modifyCount));
}

// vHashesToUpdate is the set of transaction hashes from a disconnected block
// which has been re-added to the mempool.
// for each entry, look for descendants that are outside hashesToUpdate, and
// add fee/size information for such descendants to the parent.
// for each such descendant, also update the ancestor state to include the parent.
void CTxMemPool::UpdateTransactionsFromBlock(const std::vector<uint256> &vHashesToUpdate)
{
    LOCK(cs);
    // For each entry in vHashesToUpdate, store the set of in-mempool, but not
    // in-vHashesToUpdate transactions, so that we don't have to recalculate
    // descendants when we come across a previously seen entry.
    cacheMap mapMemPoolDescendantsToUpdate;

    // Use a set for lookups into vHashesToUpdate (these entries are already
    // accounted for in the state of their ancestors)
    std::set<uint256> setAlreadyIncluded(vHashesToUpdate.begin(), vHashesToUpdate.end());

    // Iterate in reverse, so that whenever we are looking at at a transaction
    // we are sure that all in-mempool descendants have already been processed.
    // This maximizes the benefit of the descendant cache and guarantees that
    // setMemPoolChildren will be updated, an assumption made in
    // UpdateForDescendants.
    for (const uint256 &hash : reverse_iterate(vHashesToUpdate)) {
        // we cache the in-mempool children to avoid duplicate updates
        setEntries setChildren;
        // calculate children from mapNextTx
        txiter it = mapTx.find(hash);
        if (it == mapTx.end()) {
            continue;
        }
        auto iter = mapNextTx.lower_bound(COutPoint(hash, 0));
        // First calculate the children, and update setMemPoolChildren to
        // include them, and update their setMemPoolParents to include this tx.
        for (; iter != mapNextTx.end() && iter->first->hash == hash; ++iter) {
            const uint256 &childHash = iter->second->GetHash();
            txiter childIter = mapTx.find(childHash);
            assert(childIter != mapTx.end());
            // We can skip updating entries we've encountered before or that
            // are in the block (which are already accounted for).
            if (setChildren.insert(childIter).second && !setAlreadyIncluded.count(childHash)) {
                UpdateChild(it, childIter, true);
                UpdateParent(childIter, it, true);
            }
        }
        UpdateForDescendants(it, mapMemPoolDescendantsToUpdate, setAlreadyIncluded);
    }
}

bool CTxMemPool::CalculateMemPoolAncestors(const CTxMemPoolEntry &entry, setEntries &setAncestors, uint64_t limitAncestorCount, uint64_t limitAncestorSize, uint64_t limitDescendantCount, uint64_t limitDescendantSize, std::string &errString, bool fSearchForParents /* = true */) const
{
    setEntries parentHashes;
    const auto &tx = entry.GetSharedTx();

    if (fSearchForParents) {
        // Get parents of this transaction that are in the mempool
        // GetMemPoolParents() is only valid for entries in the mempool, so we
        // iterate mapTx to find parents.
        for (unsigned int i = 0; i < tx->vin.size(); i++) {
            txiter piter = mapTx.find(tx->vin[i].prevout.hash);
            if (piter != mapTx.end()) {
                parentHashes.insert(piter);
                if (parentHashes.size() + 1 > limitAncestorCount) {
                    errString = strprintf("too many unconfirmed parents [limit: %u]", limitAncestorCount);
                    return false;
                }
            }
        }
    } else {
        // If we're not searching for parents, we require this to be an
        // entry in the mempool already.
        txiter it = mapTx.iterator_to(entry);
        parentHashes = GetMemPoolParents(it);
    }

    size_t totalSizeWithAncestors = entry.GetTxSize();

    while (!parentHashes.empty()) {
        txiter stageit = *parentHashes.begin();

        setAncestors.insert(stageit);
        parentHashes.erase(stageit);
        totalSizeWithAncestors += stageit->GetTxSize();

        if (stageit->GetSizeWithDescendants() + entry.GetTxSize() > limitDescendantSize) {
            errString = strprintf("exceeds descendant size limit for tx %s [limit: %u]", stageit->GetTx().GetHash().ToString(), limitDescendantSize);
            return false;
        } else if (stageit->GetCountWithDescendants() + 1 > limitDescendantCount) {
            errString = strprintf("too many descendants for tx %s [limit: %u]", stageit->GetTx().GetHash().ToString(), limitDescendantCount);
            return false;
        } else if (totalSizeWithAncestors > limitAncestorSize) {
            errString = strprintf("exceeds ancestor size limit [limit: %u]", limitAncestorSize);
            return false;
        }

        const setEntries & setMemPoolParents = GetMemPoolParents(stageit);
        for (const txiter& phash : setMemPoolParents) {
            // If this is a new ancestor, add it.
            if (setAncestors.count(phash) == 0) {
                parentHashes.insert(phash);
            }
            if (parentHashes.size() + setAncestors.size() + 1 > limitAncestorCount) {
                errString = strprintf("too many unconfirmed ancestors [limit: %u]", limitAncestorCount);
                return false;
            }
        }
    }

    return true;
}

void CTxMemPool::UpdateAncestorsOf(bool add, txiter it, setEntries &setAncestors)
{
    setEntries parentIters = GetMemPoolParents(it);
    // add or remove this tx as a child of each parent
    for (const txiter& piter : parentIters) {
        UpdateChild(piter, it, add);
    }
    const int64_t updateCount = (add ? 1 : -1);
    const int64_t updateSize = updateCount * it->GetTxSize();
    const CAmount updateFee = updateCount * it->GetModifiedFee();
    for (const txiter& ancestorIt : setAncestors) {
        mapTx.modify(ancestorIt, update_descendant_state(updateSize, updateFee, updateCount));
    }
}

void CTxMemPool::UpdateEntryForAncestors(txiter it, const setEntries &setAncestors)
{
    int64_t updateCount = setAncestors.size();
    int64_t updateSize = 0;
    CAmount updateFee = 0;
    int updateSigOps = 0;
    for (const txiter& ancestorIt : setAncestors) {
        updateSize += ancestorIt->GetTxSize();
        updateFee += ancestorIt->GetModifiedFee();
        updateSigOps += ancestorIt->GetSigOpCount();
    }
    mapTx.modify(it, update_ancestor_state(updateSize, updateFee, updateCount, updateSigOps));
}

void CTxMemPool::UpdateChildrenForRemoval(txiter it)
{
    const setEntries &setMemPoolChildren = GetMemPoolChildren(it);
    for (const txiter& updateIt : setMemPoolChildren) {
        UpdateParent(updateIt, it, false);
    }
}

void CTxMemPool::UpdateForRemoveFromMempool(const setEntries &entriesToRemove, bool updateDescendants)
{
    // For each entry, walk back all ancestors and decrement size associated with this
    // transaction
    const uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
    if (updateDescendants) {
        // updateDescendants should be true whenever we're not recursively
        // removing a tx and all its descendants, eg when a transaction is
        // confirmed in a block.
        // Here we only update statistics and not data in mapLinks (which
        // we need to preserve until we're finished with all operations that
        // need to traverse the mempool).
        for (const txiter& removeIt : entriesToRemove) {
            setEntries setDescendants;
            CalculateDescendants(removeIt, setDescendants);
            setDescendants.erase(removeIt); // don't update state for self
            int64_t modifySize = -((int64_t)removeIt->GetTxSize());
            CAmount modifyFee = -removeIt->GetModifiedFee();
            int modifySigOps = -removeIt->GetSigOpCount();
            for (const txiter& dit : setDescendants) {
                mapTx.modify(dit, update_ancestor_state(modifySize, modifyFee, -1, modifySigOps));
            }
        }
    }
    for (const txiter& removeIt : entriesToRemove) {
        setEntries setAncestors;
        const CTxMemPoolEntry &entry = *removeIt;
        std::string dummy;
        // Since this is a tx that is already in the mempool, we can call CMPA
        // with fSearchForParents = false.  If the mempool is in a consistent
        // state, then using true or false should both be correct, though false
        // should be a bit faster.
        // However, if we happen to be in the middle of processing a reorg, then
        // the mempool can be in an inconsistent state.  In this case, the set
        // of ancestors reachable via mapLinks will be the same as the set of
        // ancestors whose packages include this transaction, because when we
        // add a new transaction to the mempool in addUnchecked(), we assume it
        // has no children, and in the case of a reorg where that assumption is
        // false, the in-mempool children aren't linked to the in-block tx's
        // until UpdateTransactionsFromBlock() is called.
        // So if we're being called during a reorg, ie before
        // UpdateTransactionsFromBlock() has been called, then mapLinks[] will
        // differ from the set of mempool parents we'd calculate by searching,
        // and it's important that we use the mapLinks[] notion of ancestor
        // transactions as the set of things to update for removal.
        CalculateMemPoolAncestors(entry, setAncestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy, false);
        // Note that UpdateAncestorsOf severs the child links that point to
        // removeIt in the entries for the parents of removeIt.
        UpdateAncestorsOf(false, removeIt, setAncestors);
    }
    // After updating all the ancestor sizes, we can now sever the link between each
    // transaction being removed and any mempool children (ie, update setMemPoolParents
    // for each direct child of a transaction being removed).
    for (const txiter& removeIt : entriesToRemove) {
        UpdateChildrenForRemoval(removeIt);
    }
}

void CTxMemPoolEntry::UpdateDescendantState(int64_t modifySize, CAmount modifyFee, int64_t modifyCount)
{
    nSizeWithDescendants += modifySize;
    assert(int64_t(nSizeWithDescendants) > 0);
    nModFeesWithDescendants += modifyFee;
    nCountWithDescendants += modifyCount;
    assert(int64_t(nCountWithDescendants) > 0);
}

void CTxMemPoolEntry::UpdateAncestorState(int64_t modifySize, CAmount modifyFee, int64_t modifyCount, int modifySigOps)
{
    nSizeWithAncestors += modifySize;
    assert(int64_t(nSizeWithAncestors) > 0);
    nModFeesWithAncestors += modifyFee;
    nCountWithAncestors += modifyCount;
    assert(int64_t(nCountWithAncestors) > 0);
    nSigOpCountWithAncestors += modifySigOps;
    assert(int(nSigOpCountWithAncestors) >= 0);
}

CTxMemPool::CTxMemPool(const CFeeRate& _minReasonableRelayFee) :
        nTransactionsUpdated(0)
{
    _clear();   // lock-free clear

    // Sanity checks off by default for performance, because otherwise
    // accepting transactions becomes O(N^2) where N is the number
    // of transactions in the pool
    nCheckFrequency = 0;

    minerPolicyEstimator = new CBlockPolicyEstimator(_minReasonableRelayFee);
    minReasonableRelayFee = _minReasonableRelayFee;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CTxMemPool::~CTxMemPool()
{
    delete minerPolicyEstimator;
}

<<<<<<< HEAD
void CTxMemPool::pruneSpent(const uint256& hashTx, CCoins& coins)
{
    LOCK(cs);

    std::map<COutPoint, CInPoint>::iterator it = mapNextTx.lower_bound(COutPoint(hashTx, 0));

    // iterate over all COutPoints in mapNextTx whose hash equals the provided hashTx
    while (it != mapNextTx.end() && it->first.hash == hashTx) {
        coins.Spend(it->first.n); // and remove those outputs from coins
        it++;
    }
=======
bool CTxMemPool::isSpent(const COutPoint& outpoint)
{
    LOCK(cs);
    return mapNextTx.count(outpoint);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

unsigned int CTxMemPool::GetTransactionsUpdated() const
{
    LOCK(cs);
    return nTransactionsUpdated;
}

void CTxMemPool::AddTransactionsUpdated(unsigned int n)
{
    LOCK(cs);
    nTransactionsUpdated += n;
}

<<<<<<< HEAD

bool CTxMemPool::addUnchecked(const uint256& hash, const CTxMemPoolEntry& entry)
{
    // Add to memory pool without checking anything.
    // Used by main.cpp AcceptToMemoryPool(), which DOES do
    // all the appropriate checks.
    LOCK(cs);
    {
        mapTx[hash] = entry;
        const CTransaction& tx = mapTx[hash].GetTx();
        {
            if (tx.IsCoinStake()) {
                for (unsigned int i = 0; i < tx.vin.size(); i++)
                    mapNextTx[tx.vin[i].prevout] = CInPoint(&tx, i);
            }
        }
        nTransactionsUpdated++;
        totalTxSize += entry.GetTxSize();
    }
    return true;
}


void CTxMemPool::remove(const CTransaction& origTx, std::list<CTransaction>& removed, bool fRecursive)
=======
void CTxMemPool::addUncheckedSpecialTx(const CTransaction& tx)
{
    if (!tx.IsSpecialTx()) return;

    // Invalid special txes never get this far because transactions should be
    // fully checked by AcceptToMemoryPool() at this point, so we just assume that
    // everything is fine here.
    const uint256& txid = tx.GetHash();
    switch(tx.nType) {
        case CTransaction::TxType::PROREG: {
            ProRegPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            if (!pl.collateralOutpoint.hash.IsNull()) {
                mapProTxRefs.emplace(txid, pl.collateralOutpoint.hash);
                mapProTxCollaterals.emplace(pl.collateralOutpoint, txid);
            }
            mapProTxAddresses.emplace(pl.addr, txid);
            mapProTxPubKeyIDs.emplace(pl.keyIDOwner, txid);
            mapProTxBlsPubKeyHashes.emplace(pl.pubKeyOperator.GetHash(), txid);
            break;
        }

        case CTransaction::TxType::PROUPSERV: {
            ProUpServPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            mapProTxRefs.emplace(pl.proTxHash, txid);
            mapProTxAddresses.emplace(pl.addr, txid);
            break;
        }

        case CTransaction::TxType::PROUPREG: {
            ProUpRegPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            mapProTxRefs.emplace(pl.proTxHash, txid);
            mapProTxBlsPubKeyHashes.emplace(pl.pubKeyOperator.GetHash(), txid);
            break;
        }

        case CTransaction::TxType::PROUPREV: {
            ProUpRevPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            mapProTxRefs.emplace(pl.proTxHash, txid);
            break;
        }

    }
}

bool CTxMemPool::addUnchecked(const uint256& hash, const CTxMemPoolEntry &entry, setEntries &setAncestors, bool validFeeEstimate)
{
    // Add to memory pool without checking anything.
    // Used by AcceptToMemoryPool(), which DOES do all the appropriate checks.
    LOCK(cs);
    indexed_transaction_set::iterator newit = mapTx.insert(entry).first;
    mapLinks.insert(make_pair(newit, TxLinks()));

    // Update transaction for any feeDelta created by PrioritiseTransaction
    // TODO: refactor so that the fee delta is calculated before inserting
    // into mapTx.
    std::map<uint256, CAmount>::const_iterator pos = mapDeltas.find(hash);
    if (pos != mapDeltas.end()) {
        const CAmount &delta = pos->second;
        if (delta) {
            mapTx.modify(newit, update_fee_delta(delta));
        }
    }

    // Update cachedInnerUsage to include contained transaction's usage.
    // (When we update the entry for in-mempool parents, memory usage will be
    // further updated.)
    cachedInnerUsage += entry.DynamicMemoryUsage();

    const CTransaction& tx = newit->GetTx();
    std::set<uint256> setParentTransactions;
    if(!tx.HasZerocoinSpendInputs()) {
        for (unsigned int i = 0; i < tx.vin.size(); i++) {
            mapNextTx.insert(std::make_pair(&tx.vin[i].prevout, newit->GetSharedTx()));
            setParentTransactions.insert(tx.vin[i].prevout.hash);
        }
    }
    // Don't bother worrying about child transactions of this one.
    // Normal case of a new transaction arriving is that there can't be any
    // children, because such children would be orphans.
    // An exception to that is if a transaction enters that used to be in a block.
    // In that case, our disconnect block logic will call UpdateTransactionsFromBlock
    // to clean up the mess we're leaving here.

    // Update ancestors with information about this tx
    for (const uint256& phash : setParentTransactions) {
        txiter pit = mapTx.find(phash);
        if (pit != mapTx.end()) {
            UpdateParent(newit, pit, true);
        }
    }
    UpdateAncestorsOf(true, newit, setAncestors);
    UpdateEntryForAncestors(newit, setAncestors);

    // Save spent nullifiers
    if (tx.IsShieldedTx()) {
        for (const SpendDescription& sd : tx.sapData->vShieldedSpend) {
            mapSaplingNullifiers[sd.nullifier] = newit->GetSharedTx();
        }
    }

    nTransactionsUpdated++;
    totalTxSize += entry.GetTxSize();
    minerPolicyEstimator->processTransaction(entry, validFeeEstimate);

    addUncheckedSpecialTx(tx);

    return true;
}

void CTxMemPool::removeUncheckedSpecialTx(const CTransaction& tx)
{
    if (!tx.IsSpecialTx()) return;

    auto eraseProTxRef = [&](const uint256& proTxHash, const uint256& txHash) {
        auto its = mapProTxRefs.equal_range(proTxHash);
        for (auto it = its.first; it != its.second;) {
            if (it->second == txHash) {
                it = mapProTxRefs.erase(it);
            } else {
                ++it;
            }
        }
    };

    const uint256& txid = tx.GetHash();
    switch(tx.nType) {
        case CTransaction::TxType::PROREG: {
            ProRegPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            if (!pl.collateralOutpoint.IsNull()) {
                eraseProTxRef(txid, pl.collateralOutpoint.hash);
            }
            mapProTxCollaterals.erase(pl.collateralOutpoint);
            mapProTxAddresses.erase(pl.addr);
            mapProTxPubKeyIDs.erase(pl.keyIDOwner);
            mapProTxBlsPubKeyHashes.erase(pl.pubKeyOperator.GetHash());
            break;
        }

        case CTransaction::TxType::PROUPSERV: {
            ProUpServPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            eraseProTxRef(pl.proTxHash, txid);
            mapProTxAddresses.erase(pl.addr);
            break;
        }

        case CTransaction::TxType::PROUPREG: {
            ProUpRegPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            eraseProTxRef(pl.proTxHash, txid);
            mapProTxBlsPubKeyHashes.erase(pl.pubKeyOperator.GetHash());
            break;
        }

        case CTransaction::TxType::PROUPREV: {
            ProUpRevPL pl;
            bool ok = GetTxPayload(tx, pl);
            assert(ok);
            eraseProTxRef(pl.proTxHash, txid);
            break;
        }

    }
}

void CTxMemPool::removeUnchecked(txiter it, MemPoolRemovalReason reason)
{
    if (reason != MemPoolRemovalReason::BLOCK) {
        // Notify clients that a transaction has been removed from the mempool
        // for any reason except being included in a block. Clients interested
        // in transactions included in blocks can subscribe to the BlockConnected
        // notification.
        GetMainSignals().TransactionRemovedFromMempool(it->GetSharedTx(), reason);
    }

    AssertLockHeld(cs);
    const CTransaction& tx = it->GetTx();
    for (const CTxIn& txin : tx.vin)
        mapNextTx.erase(txin.prevout);
    // Remove spent nullifiers
    if (tx.IsShieldedTx()) {
        for (const SpendDescription& sd : tx.sapData->vShieldedSpend) {
            mapSaplingNullifiers.erase(sd.nullifier);
        }
    }

    removeUncheckedSpecialTx(tx);

    totalTxSize -= it->GetTxSize();
    cachedInnerUsage -= it->DynamicMemoryUsage();
    cachedInnerUsage -= memusage::DynamicUsage(mapLinks[it].parents) + memusage::DynamicUsage(mapLinks[it].children);
    mapLinks.erase(it);
    mapTx.erase(it);
    nTransactionsUpdated++;
    minerPolicyEstimator->removeTx(tx.GetHash());
}

// Calculates descendants of entry that are not already in setDescendants, and adds to
// setDescendants. Assumes entryit is already a tx in the mempool and setMemPoolChildren
// is correct for tx and all descendants.
// Also assumes that if an entry is in setDescendants already, then all
// in-mempool descendants of it are already in setDescendants as well, so that we
// can save time by not iterating over those entries.
void CTxMemPool::CalculateDescendants(txiter entryit, setEntries &setDescendants)
{
    setEntries stage;
    if (setDescendants.count(entryit) == 0) {
        stage.insert(entryit);
    }
    // Traverse down the children of entry, only adding children that are not
    // accounted for in setDescendants already (because those children have either
    // already been walked, or will be walked in this iteration).
    while (!stage.empty()) {
        txiter it = *stage.begin();
        setDescendants.insert(it);
        stage.erase(it);

        const setEntries &setChildren = GetMemPoolChildren(it);
        for (const txiter& childiter : setChildren) {
            if (!setDescendants.count(childiter)) {
                stage.insert(childiter);
            }
        }
    }
}

void CTxMemPool::removeRecursive(const CTransaction& origTx, MemPoolRemovalReason reason)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // Remove transaction from memory pool
    {
        LOCK(cs);
<<<<<<< HEAD
        std::deque<uint256> txToRemove;
        txToRemove.push_back(origTx.GetHash());
        if (fRecursive && !mapTx.count(origTx.GetHash())) {
            // If recursively removing but origTx isn't in the mempool
=======
        setEntries txToRemove;
        txiter origit = mapTx.find(origTx.GetHash());
        if (origit != mapTx.end()) {
            txToRemove.insert(origit);
        } else {
            // When recursively removing but origTx isn't in the mempool
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            // be sure to remove any children that are in the pool. This can
            // happen during chain re-orgs if origTx isn't re-accepted into
            // the mempool for any reason.
            for (unsigned int i = 0; i < origTx.vout.size(); i++) {
<<<<<<< HEAD
                std::map<COutPoint, CInPoint>::iterator it = mapNextTx.find(COutPoint(origTx.GetHash(), i));
                if (it == mapNextTx.end())
                    continue;
                txToRemove.push_back(it->second.ptx->GetHash());
            }
        }
        while (!txToRemove.empty()) {
            uint256 hash = txToRemove.front();
            txToRemove.pop_front();
            if (!mapTx.count(hash))
                continue;
            const CTransaction& tx = mapTx[hash].GetTx();
            if (fRecursive) {
                for (unsigned int i = 0; i < tx.vout.size(); i++) {
                    std::map<COutPoint, CInPoint>::iterator it = mapNextTx.find(COutPoint(hash, i));
                    if (it == mapNextTx.end())
                        continue;
                    txToRemove.push_back(it->second.ptx->GetHash());
                }
            }
            for (const CTxIn& txin : tx.vin)
                mapNextTx.erase(txin.prevout);

            removed.push_back(tx);
            totalTxSize -= mapTx[hash].GetTxSize();
            mapTx.erase(hash);
            nTransactionsUpdated++;
        }
    }
}

void CTxMemPool::removeCoinbaseSpends(const CCoinsViewCache* pcoins, unsigned int nMemPoolHeight)
{
    // Remove transactions spending a coinbase which are now immature
    LOCK(cs);
    list<CTransaction> transactionsToRemove;
    for (std::map<uint256, CTxMemPoolEntry>::const_iterator it = mapTx.begin(); it != mapTx.end(); it++) {
        const CTransaction& tx = it->second.GetTx();
        for (const CTxIn& txin : tx.vin) {
            std::map<uint256, CTxMemPoolEntry>::const_iterator it2 = mapTx.find(txin.prevout.hash);
            if (it2 != mapTx.end())
                continue;
            const CCoins* coins = pcoins->AccessCoins(txin.prevout.hash);
            if (fSanityCheck) assert(coins);
            if (!coins || ((coins->IsCoinBase() || coins->IsCoinStake()) && nMemPoolHeight - coins->nHeight < (unsigned)Params().COINBASE_MATURITY())) {
                transactionsToRemove.push_back(tx);
=======
                auto it = mapNextTx.find(COutPoint(origTx.GetHash(), i));
                if (it == mapNextTx.end())
                    continue;
                txiter nextit = mapTx.find(it->second->GetHash());
                assert(nextit != mapTx.end());
                txToRemove.insert(nextit);
            }
        }
        setEntries setAllRemoves;
        for (const txiter& it : txToRemove) {
            CalculateDescendants(it, setAllRemoves);
        }

        RemoveStaged(setAllRemoves, false, reason);
    }
}

void CTxMemPool::removeForReorg(const CCoinsViewCache *pcoins, unsigned int nMemPoolHeight, int flags)
{
    AssertLockHeld(cs_main);
    // Remove transactions spending a coinbase which are now immature and no-longer-final transactions
    LOCK(cs);
    setEntries txToRemove;
    for (indexed_transaction_set::const_iterator it = mapTx.begin(); it != mapTx.end(); it++) {
        const CTransactionRef& tx = it->GetSharedTx();
        if (!CheckFinalTx(tx, flags)) {
            txToRemove.insert(it);
        } else if (it->GetSpendsCoinbaseOrCoinstake()) {
            for (const CTxIn& txin : tx->vin) {
                indexed_transaction_set::const_iterator it2 = mapTx.find(txin.prevout.hash);
                if (it2 != mapTx.end())
                    continue;
                const Coin &coin = pcoins->AccessCoin(txin.prevout);
                if (nCheckFrequency != 0) assert(!coin.IsSpent());
                if (coin.IsSpent() || ((coin.IsCoinBase() || coin.IsCoinStake()) && ((signed long)nMemPoolHeight) - coin.nHeight < (signed long)Params().GetConsensus().nCoinbaseMaturity)) {
                    txToRemove.insert(it);
                    break;
                }
            }
        }
    }
    setEntries setAllRemoves;
    for (txiter it : txToRemove) {
        CalculateDescendants(it, setAllRemoves);
    }
    RemoveStaged(setAllRemoves, false, MemPoolRemovalReason::REORG);
}

void CTxMemPool::removeWithAnchor(const uint256& invalidRoot)
{

    // If a block is disconnected from the tip, and the root changed,
    // we must invalidate transactions from the mempool which spend
    // from that root -- almost as though they were spending coinbases
    // which are no longer valid to spend due to coinbase maturity.
    LOCK(cs);
    std::list<CTransaction> transactionsToRemove;
    for (indexed_transaction_set::const_iterator it = mapTx.begin(); it != mapTx.end(); it++) {
        const CTransaction& tx = it->GetTx();
        if (!tx.IsShieldedTx()) continue;
        for (const auto& sd : tx.sapData->vShieldedSpend) {
            if (sd.anchor == invalidRoot) {
                transactionsToRemove.emplace_back(tx);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                break;
            }
        }
    }
    for (const CTransaction& tx : transactionsToRemove) {
<<<<<<< HEAD
        list<CTransaction> removed;
        remove(tx, removed, true);
    }
}

void CTxMemPool::removeConflicts(const CTransaction& tx, std::list<CTransaction>& removed)
{
    // Remove transactions which depend on inputs of tx, recursively
    list<CTransaction> result;
    LOCK(cs);
    for (const CTxIn& txin : tx.vin) {
        std::map<COutPoint, CInPoint>::iterator it = mapNextTx.find(txin.prevout);
        if (it != mapNextTx.end()) {
            const CTransaction& txConflict = *it->second.ptx;
            if (txConflict != tx) {
                remove(txConflict, removed, true);
            }
        }
=======
        removeRecursive(tx);
    }
}

void CTxMemPool::removeConflicts(const CTransaction& tx)
{
    // Remove transactions which depend on inputs of tx, recursively
    std::list<CTransaction> result;
    LOCK(cs);
    for (const CTxIn& txin : tx.vin) {
        auto it = mapNextTx.find(txin.prevout);
        if (it != mapNextTx.end()) {
            const CTransaction& txConflict = *it->second;
            if (txConflict != tx) {
                removeRecursive(txConflict, MemPoolRemovalReason::CONFLICT);
                ClearPrioritisation(txConflict.GetHash());
            }
        }
    }
    // Remove txes with conflicting nullifier
    if (tx.IsShieldedTx()) {
        for (const SpendDescription& sd : tx.sapData->vShieldedSpend) {
            const auto& it = mapSaplingNullifiers.find(sd.nullifier);
            if (it != mapSaplingNullifiers.end()) {
                const CTransaction& txConflict = *it->second;
                if (txConflict != tx) {
                    removeRecursive(txConflict, MemPoolRemovalReason::CONFLICT);
                    ClearPrioritisation(txConflict.GetHash());
                }
            }
        }
    }
}

void CTxMemPool::removeProTxPubKeyConflicts(const CTransaction& tx, const CKeyID& keyId)
{
    if (mapProTxPubKeyIDs.count(keyId)) {
        const uint256& conflictHash = mapProTxPubKeyIDs.at(keyId);
        if (conflictHash != tx.GetHash() && mapTx.count(conflictHash)) {
            removeRecursive(mapTx.find(conflictHash)->GetTx(), MemPoolRemovalReason::CONFLICT);
        }
    }
}

void CTxMemPool::removeProTxPubKeyConflicts(const CTransaction& tx, const CBLSPublicKey& pubKey)
{
    if (mapProTxBlsPubKeyHashes.count(pubKey.GetHash())) {
        const uint256& conflictHash = mapProTxBlsPubKeyHashes.at(pubKey.GetHash());
        if (conflictHash != tx.GetHash() && mapTx.count(conflictHash)) {
            removeRecursive(mapTx.find(conflictHash)->GetTx(), MemPoolRemovalReason::CONFLICT);
        }
    }
}

void CTxMemPool::removeProTxCollateralConflicts(const CTransaction &tx, const COutPoint &collateralOutpoint)
{
    if (mapProTxCollaterals.count(collateralOutpoint)) {
        const uint256& conflictHash = mapProTxCollaterals.at(collateralOutpoint);
        if (conflictHash != tx.GetHash() && mapTx.count(conflictHash)) {
            removeRecursive(mapTx.find(conflictHash)->GetTx(), MemPoolRemovalReason::CONFLICT);
        }
    }
}

void CTxMemPool::removeProTxReferences(const uint256& proTxHash, MemPoolRemovalReason reason)
{
    // Remove TXs that refer to a certain MN
    while (true) {
        auto it = mapProTxRefs.find(proTxHash);
        if (it == mapProTxRefs.end()) {
            break;
        }
        auto conflictIt = mapTx.find(it->second);
        if (conflictIt != mapTx.end()) {
            removeRecursive(conflictIt->GetTx(), reason);
        } else {
            // Should not happen as we track referencing TXs in addUnchecked/removeUnchecked.
            // But lets be on the safe side and not run into an endless loop...
            LogPrint(BCLog::MEMPOOL, "%s: ERROR: found invalid TX ref in mapProTxRefs, proTxHash=%s, txHash=%s\n", __func__, proTxHash.ToString(), it->second.ToString());
            mapProTxRefs.erase(it);
        }
    }
}

void CTxMemPool::removeProTxSpentCollateralConflicts(const CTransaction &tx)
{
    auto mnList = deterministicMNManager->GetListAtChainTip();
    for (const auto& in : tx.vin) {
        auto collateralIt = mapProTxCollaterals.find(in.prevout);
        if (collateralIt != mapProTxCollaterals.end()) {
            // These are not yet mined ProRegTxs
            removeProTxReferences(collateralIt->second, MemPoolRemovalReason::CONFLICT);
        }
        auto dmn = mnList.GetMNByCollateral(in.prevout);
        if (dmn) {
            // These are updates refering to a mined ProRegTx
            removeProTxReferences(dmn->proTxHash, MemPoolRemovalReason::CONFLICT);
        }
    }
}

void CTxMemPool::removeProTxConflicts(const CTransaction &tx)
{
    removeProTxSpentCollateralConflicts(tx);

    if (!tx.IsSpecialTx()) return;

    const uint256& txid = tx.GetHash();
    switch(tx.nType) {
        case CTransaction::TxType::PROREG: {
            ProRegPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return;
            }
            if (mapProTxAddresses.count(pl.addr)) {
                const uint256& conflictHash = mapProTxAddresses.at(pl.addr);
                if (conflictHash != txid && mapTx.count(conflictHash)) {
                    removeRecursive(mapTx.find(conflictHash)->GetTx(), MemPoolRemovalReason::CONFLICT);
                }
            }
            removeProTxPubKeyConflicts(tx, pl.keyIDOwner);
            removeProTxPubKeyConflicts(tx, pl.pubKeyOperator);
            if (!pl.collateralOutpoint.hash.IsNull()) {
                removeProTxCollateralConflicts(tx, pl.collateralOutpoint);
            }
            break;
        }

        case CTransaction::TxType::PROUPSERV: {
            ProUpServPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return;
            }
            if (mapProTxAddresses.count(pl.addr)) {
                const uint256& conflictHash = mapProTxAddresses.at(pl.addr);
                if (conflictHash != txid && mapTx.count(conflictHash)) {
                    removeRecursive(mapTx.find(conflictHash)->GetTx(), MemPoolRemovalReason::CONFLICT);
                }
            }
            break;
        }

        case CTransaction::TxType::PROUPREG: {
            ProUpRegPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return;
            }
            removeProTxPubKeyConflicts(tx, pl.pubKeyOperator);
            break;
        }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
}

/**
 * Called when a block is connected. Removes from mempool and updates the miner fee estimator.
 */
<<<<<<< HEAD
void CTxMemPool::removeForBlock(const std::vector<CTransaction>& vtx, unsigned int nBlockHeight, std::list<CTransaction>& conflicts)
{
    LOCK(cs);
    std::vector<CTxMemPoolEntry> entries;
    for (const CTransaction& tx : vtx) {
        uint256 hash = tx.GetHash();
        if (mapTx.count(hash))
            entries.push_back(mapTx[hash]);
    }
    minerPolicyEstimator->seenBlock(entries, nBlockHeight, minRelayFee);
    for (const CTransaction& tx : vtx) {
        std::list<CTransaction> dummy;
        remove(tx, dummy, false);
        removeConflicts(tx, conflicts);
        ClearPrioritisation(tx.GetHash());
    }
}


void CTxMemPool::clear()
{
    LOCK(cs);
    mapTx.clear();
    mapNextTx.clear();
    totalTxSize = 0;
    ++nTransactionsUpdated;
}

void CTxMemPool::check(const CCoinsViewCache* pcoins) const
{
    if (!fSanityCheck)
        return;

    LogPrint("mempool", "Checking mempool with %u transactions and %u inputs\n", (unsigned int)mapTx.size(), (unsigned int)mapNextTx.size());

    uint64_t checkTotal = 0;
=======
void CTxMemPool::removeForBlock(const std::vector<CTransactionRef>& vtx, unsigned int nBlockHeight)
{
    LOCK(cs);
    std::vector<const CTxMemPoolEntry*> entries;
    for (const auto& tx : vtx) {
        uint256 hash = tx->GetHash();
        indexed_transaction_set::iterator i = mapTx.find(hash);
        if (i != mapTx.end())
            entries.push_back(&*i);
    }
    // Before the txs in the new block have been removed from the mempool, update policy estimates
    minerPolicyEstimator->processBlock(nBlockHeight, entries);
    for (const auto& tx : vtx) {
        txiter it = mapTx.find(tx->GetHash());
        if (it != mapTx.end()) {
            setEntries stage;
            stage.insert(it);
            RemoveStaged(stage, true, MemPoolRemovalReason::BLOCK);
        }
        removeConflicts(*tx);
        removeProTxConflicts(*tx);
        ClearPrioritisation(tx->GetHash());
    }
    lastRollingFeeUpdate = GetTime();
    blockSinceLastRollingFeeBump = true;
}


void CTxMemPool::_clear()
{
    mapLinks.clear();
    mapTx.clear();
    mapNextTx.clear();
    mapProTxAddresses.clear();
    mapProTxPubKeyIDs.clear();
    totalTxSize = 0;
    cachedInnerUsage = 0;
    lastRollingFeeUpdate = GetTime();
    blockSinceLastRollingFeeBump = false;
    rollingMinimumFeeRate = 0;
    ++nTransactionsUpdated;
}

void CTxMemPool::clear()
{
    LOCK(cs);
    _clear();
}

void CTxMemPool::check(const CCoinsViewCache* pcoins) const
{
    if (nCheckFrequency == 0)
        return;

    if (GetRand(std::numeric_limits<uint32_t>::max()) >= nCheckFrequency)
        return;

    LogPrint(BCLog::MEMPOOL, "Checking mempool with %u transactions and %u inputs\n", (unsigned int)mapTx.size(), (unsigned int)mapNextTx.size());

    uint64_t checkTotal = 0;
    uint64_t innerUsage = 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    CCoinsViewCache mempoolDuplicate(const_cast<CCoinsViewCache*>(pcoins));

    LOCK(cs);
<<<<<<< HEAD
    list<const CTxMemPoolEntry*> waitingOnDependants;
    for (std::map<uint256, CTxMemPoolEntry>::const_iterator it = mapTx.begin(); it != mapTx.end(); it++) {
        unsigned int i = 0;
        checkTotal += it->second.GetTxSize();
        const CTransaction& tx = it->second.GetTx();
        bool fDependsWait = false;
        for (const CTxIn& txin : tx.vin) {
            // Check that every mempool transaction's inputs refer to available coins, or other mempool tx's.
            std::map<uint256, CTxMemPoolEntry>::const_iterator it2 = mapTx.find(txin.prevout.hash);
            if (it2 != mapTx.end()) {
                const CTransaction& tx2 = it2->second.GetTx();
                assert(tx2.vout.size() > txin.prevout.n && !tx2.vout[txin.prevout.n].IsNull());
                fDependsWait = true;
            } else {
                const CCoins* coins = pcoins->AccessCoins(txin.prevout.hash);
                assert(coins && coins->IsAvailable(txin.prevout.n));
            }
            // Check whether its inputs are marked in mapNextTx.
            std::map<COutPoint, CInPoint>::const_iterator it3 = mapNextTx.find(txin.prevout);
            assert(it3 != mapNextTx.end());
            assert(it3->second.ptx == &tx);
            assert(it3->second.n == i);
            i++;
        }
        if (fDependsWait)
            waitingOnDependants.push_back(&it->second);
        else {
            CValidationState state;
            CTxUndo undo;
            assert(CheckInputs(tx, state, mempoolDuplicate, false, 0, false, NULL));
            UpdateCoins(tx, state, mempoolDuplicate, undo, 1000000);
        }
    }
=======
    std::list<const CTxMemPoolEntry*> waitingOnDependants;
    for (indexed_transaction_set::const_iterator it = mapTx.begin(); it != mapTx.end(); it++) {
        unsigned int i = 0;
        checkTotal += it->GetTxSize();
        innerUsage += it->DynamicMemoryUsage();
        const CTransaction& tx = it->GetTx();
        txlinksMap::const_iterator linksiter = mapLinks.find(it);
        assert(linksiter != mapLinks.end());
        const TxLinks &links = linksiter->second;
        innerUsage += memusage::DynamicUsage(links.parents) + memusage::DynamicUsage(links.children);
        bool fDependsWait = false;
        setEntries setParentCheck;
        int64_t parentSizes = 0;
        unsigned int parentSigOpCount = 0;
        for (const CTxIn& txin : tx.vin) {
            // Check that every mempool transaction's inputs refer to available coins, or other mempool tx's.
            indexed_transaction_set::const_iterator it2 = mapTx.find(txin.prevout.hash);
            if (it2 != mapTx.end()) {
                const CTransaction& tx2 = it2->GetTx();
                assert(tx2.vout.size() > txin.prevout.n && !tx2.vout[txin.prevout.n].IsNull());
                fDependsWait = true;
                if (setParentCheck.insert(it2).second) {
                    parentSizes += it2->GetTxSize();
                    parentSigOpCount += it2->GetSigOpCount();
                }
            } else {
                assert(pcoins->HaveCoin(txin.prevout));
            }
            // Check whether its inputs are marked in mapNextTx.
            auto it3 = mapNextTx.find(txin.prevout);
            assert(it3 != mapNextTx.end());
            assert(it3->first == &txin.prevout);
            assert(*it3->second == tx);
            i++;
        }
        // sapling txes
        if (tx.IsShieldedTx()) {
            for (const SpendDescription& sd : tx.sapData->vShieldedSpend) {
                SaplingMerkleTree tree;
                assert(pcoins->GetSaplingAnchorAt(sd.anchor, tree));
                assert(!pcoins->GetNullifier(sd.nullifier));
            }
        }
        assert(setParentCheck == GetMemPoolParents(it));
        // Verify ancestor state is correct.
        setEntries setAncestors;
        uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
        std::string dummy;
        CalculateMemPoolAncestors(*it, setAncestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy);
        uint64_t nCountCheck = setAncestors.size() + 1;
        uint64_t nSizeCheck = it->GetTxSize();
        CAmount nFeesCheck = it->GetModifiedFee();
        unsigned int nSigOpCheck = it->GetSigOpCount();

        for (const txiter& ancestorIt : setAncestors) {
            nSizeCheck += ancestorIt->GetTxSize();
            nFeesCheck += ancestorIt->GetModifiedFee();
            nSigOpCheck += ancestorIt->GetSigOpCount();
        }

        assert(it->GetCountWithAncestors() == nCountCheck);
        assert(it->GetSizeWithAncestors() == nSizeCheck);
        assert(it->GetSigOpCountWithAncestors() == nSigOpCheck);
        assert(it->GetModFeesWithAncestors() == nFeesCheck);

        // Check children against mapNextTx
        CTxMemPool::setEntries setChildrenCheck;
        auto iter = mapNextTx.lower_bound(COutPoint(it->GetTx().GetHash(), 0));
        int64_t childSizes = 0;
        for (; iter != mapNextTx.end() && iter->first->hash == tx.GetHash(); ++iter) {
            txiter childit = mapTx.find(iter->second->GetHash());
            assert(childit != mapTx.end()); // mapNextTx points to in-mempool transactions
            if (setChildrenCheck.insert(childit).second) {
                childSizes += childit->GetTxSize();
            }
        }
        assert(setChildrenCheck == GetMemPoolChildren(it));
        // Also check to make sure size is greater than sum with immediate children.
        // just a sanity check, not definitive that this calc is correct...
        assert(it->GetSizeWithDescendants() >= (uint64_t)(childSizes + it->GetTxSize()));


        if (fDependsWait)
            waitingOnDependants.push_back(&(*it));
        else {
            CValidationState state;
            PrecomputedTransactionData precomTxData(tx);
            assert(CheckInputs(tx, state, mempoolDuplicate, false, 0, false, precomTxData, NULL));
            UpdateCoins(tx, mempoolDuplicate, 1000000);
        }
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned int stepsSinceLastRemove = 0;
    while (!waitingOnDependants.empty()) {
        const CTxMemPoolEntry* entry = waitingOnDependants.front();
        waitingOnDependants.pop_front();
        CValidationState state;
<<<<<<< HEAD
        if (!CheckHaveInputs(mempoolDuplicate, entry->GetTx())) {
=======
        if (!mempoolDuplicate.HaveInputs(entry->GetTx())) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            waitingOnDependants.push_back(entry);
            stepsSinceLastRemove++;
            assert(stepsSinceLastRemove < waitingOnDependants.size());
        } else {
<<<<<<< HEAD
            assert(CheckInputs(entry->GetTx(), state, mempoolDuplicate, false, 0, false, NULL));
            CTxUndo undo;
            UpdateCoins(entry->GetTx(), state, mempoolDuplicate, undo, 1000000);
            stepsSinceLastRemove = 0;
        }
    }
    for (std::map<COutPoint, CInPoint>::const_iterator it = mapNextTx.begin(); it != mapNextTx.end(); it++) {
        uint256 hash = it->second.ptx->GetHash();
        map<uint256, CTxMemPoolEntry>::const_iterator it2 = mapTx.find(hash);
        const CTransaction& tx = it2->second.GetTx();
        assert(it2 != mapTx.end());
        assert(&tx == it->second.ptx);
        assert(tx.vin.size() > it->second.n);
        assert(it->first == it->second.ptx->vin[it->second.n].prevout);
    }

    assert(totalTxSize == checkTotal);
}

void CTxMemPool::queryHashes(vector<uint256>& vtxid)
{
    vtxid.clear();

    LOCK(cs);
    vtxid.reserve(mapTx.size());
    for (map<uint256, CTxMemPoolEntry>::iterator mi = mapTx.begin(); mi != mapTx.end(); ++mi)
        vtxid.push_back((*mi).first);
}

bool CTxMemPool::lookup(uint256 hash, CTransaction& result) const
{
    LOCK(cs);
    map<uint256, CTxMemPoolEntry>::const_iterator i = mapTx.find(hash);
    if (i == mapTx.end()) return false;
    result = i->second.GetTx();
    return true;
=======
            PrecomputedTransactionData precomTxData(entry->GetTx());
            assert(CheckInputs(entry->GetTx(), state, mempoolDuplicate, false, 0, false, precomTxData, NULL));
            UpdateCoins(entry->GetTx(), mempoolDuplicate, 1000000);
            stepsSinceLastRemove = 0;
        }
    }
    for (auto it = mapNextTx.cbegin(); it != mapNextTx.cend(); it++) {
        const uint256& hash = it->second->GetHash();
        indexed_transaction_set::const_iterator it2 = mapTx.find(hash);
        const CTransactionRef& tx = it2->GetSharedTx();
        assert(it2 != mapTx.end());
        assert(tx == it->second);
    }

    // Consistency check for sapling nullifiers
    checkNullifiers();

    assert(totalTxSize == checkTotal);
    assert(innerUsage == cachedInnerUsage);
}

void CTxMemPool::checkNullifiers() const
{
    for (const auto& it : mapSaplingNullifiers) {
        const uint256& hash = it.second->GetHash();
        const auto& findTx = mapTx.find(hash);
        assert(findTx != mapTx.end());
        const CTransactionRef& tx = findTx->GetSharedTx();
        assert(*tx == *it.second);
    }
}

bool CTxMemPool::CompareDepthAndScore(const uint256& hasha, const uint256& hashb)
{
    LOCK(cs);
    indexed_transaction_set::const_iterator i = mapTx.find(hasha);
    if (i == mapTx.end()) return false;
    indexed_transaction_set::const_iterator j = mapTx.find(hashb);
    if (j == mapTx.end()) return true;
    uint64_t counta = i->GetCountWithAncestors();
    uint64_t countb = j->GetCountWithAncestors();
    if (counta == countb) {
        return CompareTxMemPoolEntryByScore()(*i, *j);
    }
    return counta < countb;
}

namespace {
    class DepthAndScoreComparator
    {
    public:
        bool operator()(const CTxMemPool::indexed_transaction_set::const_iterator& a, const CTxMemPool::indexed_transaction_set::const_iterator& b) const
        {
            uint64_t counta = a->GetCountWithAncestors();
            uint64_t countb = b->GetCountWithAncestors();
            if (counta == countb) {
                return CompareTxMemPoolEntryByScore()(*a, *b);
            }
            return counta < countb;
        }
    };
}

std::vector<CTxMemPool::indexed_transaction_set::const_iterator> CTxMemPool::GetSortedDepthAndScore() const
{
    std::vector<indexed_transaction_set::const_iterator> iters;
    AssertLockHeld(cs);

    iters.reserve(mapTx.size());

    for (indexed_transaction_set::iterator mi = mapTx.begin(); mi != mapTx.end(); ++mi) {
        iters.push_back(mi);
    }
    std::sort(iters.begin(), iters.end(), DepthAndScoreComparator());
    return iters;
}

void CTxMemPool::queryHashes(std::vector<uint256>& vtxid)
{
    LOCK(cs);
    auto iters = GetSortedDepthAndScore();

    vtxid.clear();
    vtxid.reserve(mapTx.size());

    for (auto it : iters) {
        vtxid.emplace_back(it->GetTx().GetHash());
    }
}

static TxMempoolInfo GetInfo(CTxMemPool::indexed_transaction_set::const_iterator it) {
    return TxMempoolInfo{it->GetSharedTx(), it->GetTime(), CFeeRate(it->GetFee(), it->GetTxSize()), it->GetModifiedFee() - it->GetFee()};
}

std::vector<TxMempoolInfo> CTxMemPool::infoAll() const
{
    LOCK(cs);
    auto iters = GetSortedDepthAndScore();

    std::vector<TxMempoolInfo> ret;
    ret.reserve(mapTx.size());
    for (auto it : iters) {
        ret.emplace_back(GetInfo(it));
    }

    return ret;
}

void CTxMemPool::getTransactions(std::set<uint256>& setTxid)
{
    setTxid.clear();

    LOCK(cs);
    for (indexed_transaction_set::iterator mi = mapTx.begin(); mi != mapTx.end(); ++mi)
        setTxid.insert(mi->GetTx().GetHash());
}

std::shared_ptr<const CTransaction> CTxMemPool::get(const uint256& hash) const
{
    LOCK(cs);
    indexed_transaction_set::const_iterator i = mapTx.find(hash);
    if (i == mapTx.end())
        return nullptr;
    return i->GetSharedTx();
}

TxMempoolInfo CTxMemPool::info(const uint256& hash) const
{
    LOCK(cs);
    indexed_transaction_set::const_iterator i = mapTx.find(hash);
    if (i == mapTx.end())
        return TxMempoolInfo();
    return GetInfo(i);
}

bool CTxMemPool::existsProviderTxConflict(const CTransaction &tx) const
{
    if (!tx.IsSpecialTx()) return false;

    LOCK(cs);

    switch(tx.nType) {
        case CTransaction::TxType::PROREG: {
            ProRegPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return true; // i.e. can't decode payload == conflict
            }
            if (mapProTxAddresses.count(pl.addr) || mapProTxPubKeyIDs.count(pl.keyIDOwner) ||
                    mapProTxBlsPubKeyHashes.count(pl.pubKeyOperator.GetHash())) {
                return true;
            }
            if (!pl.collateralOutpoint.hash.IsNull()) {
                if (mapProTxCollaterals.count(pl.collateralOutpoint)) {
                    // there is another ProRegTx that refers to the same collateral
                    return true;
                }
                if (mapNextTx.count(pl.collateralOutpoint)) {
                    // there is another tx that spends the collateral
                    return true;
                }
            }
            return false;
        }

        case CTransaction::TxType::PROUPSERV: {
            ProUpServPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return true; // i.e. can't decode payload == conflict
            }
            auto it = mapProTxAddresses.find(pl.addr);
            return it != mapProTxAddresses.end() && it->second != pl.proTxHash;
        }

        case CTransaction::TxType::PROUPREG: {
            ProUpRegPL pl;
            if (!GetTxPayload(tx, pl)) {
                LogPrint(BCLog::MEMPOOL, "%s: ERROR: Invalid transaction payload, tx: %s", __func__, tx.ToString());
                return true; // i.e. can't decode payload == conflict
            }
            auto it = mapProTxBlsPubKeyHashes.find(pl.pubKeyOperator.GetHash());
            return it != mapProTxBlsPubKeyHashes.end() && it->second != pl.proTxHash;
        }

    }
    return false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CFeeRate CTxMemPool::estimateFee(int nBlocks) const
{
    LOCK(cs);
    return minerPolicyEstimator->estimateFee(nBlocks);
}
<<<<<<< HEAD
double CTxMemPool::estimatePriority(int nBlocks) const
{
    LOCK(cs);
    return minerPolicyEstimator->estimatePriority(nBlocks);
=======

CFeeRate CTxMemPool::estimateSmartFee(int nBlocks, int *answerFoundAtBlocks) const
{
    LOCK(cs);
    return minerPolicyEstimator->estimateSmartFee(nBlocks, answerFoundAtBlocks, *this);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool CTxMemPool::WriteFeeEstimates(CAutoFile& fileout) const
{
    try {
        LOCK(cs);
<<<<<<< HEAD
        fileout << 120000;         // version required to read: 0.12.00 or later
        fileout << CLIENT_VERSION; // version that wrote the file
=======
        fileout << 4029900;         // version required to read: 4.2.99
        fileout << CLIENT_VERSION;  // version that wrote the file
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        minerPolicyEstimator->Write(fileout);
    } catch (const std::exception&) {
        LogPrintf("CTxMemPool::WriteFeeEstimates() : unable to write policy estimator data (non-fatal)");
        return false;
    }
    return true;
}

bool CTxMemPool::ReadFeeEstimates(CAutoFile& filein)
{
    try {
        int nVersionRequired, nVersionThatWrote;
        filein >> nVersionRequired >> nVersionThatWrote;
        if (nVersionRequired > CLIENT_VERSION)
            return error("CTxMemPool::ReadFeeEstimates() : up-version (%d) fee estimate file", nVersionRequired);

        LOCK(cs);
<<<<<<< HEAD
        minerPolicyEstimator->Read(filein, minRelayFee);
=======
        minerPolicyEstimator->Read(filein, nVersionThatWrote);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    } catch (const std::exception&) {
        LogPrintf("CTxMemPool::ReadFeeEstimates() : unable to read policy estimator data (non-fatal)");
        return false;
    }
    return true;
}

<<<<<<< HEAD
void CTxMemPool::PrioritiseTransaction(const uint256 hash, const string strHash, double dPriorityDelta, const CAmount& nFeeDelta)
{
    {
        LOCK(cs);
        std::pair<double, CAmount>& deltas = mapDeltas[hash];
        deltas.first += dPriorityDelta;
        deltas.second += nFeeDelta;
    }
    LogPrintf("PrioritiseTransaction: %s priority += %f, fee += %d\n", strHash, dPriorityDelta, FormatMoney(nFeeDelta));
}

void CTxMemPool::ApplyDeltas(const uint256 hash, double& dPriorityDelta, CAmount& nFeeDelta)
{
    LOCK(cs);
    std::map<uint256, std::pair<double, CAmount> >::iterator pos = mapDeltas.find(hash);
    if (pos == mapDeltas.end())
        return;
    const std::pair<double, CAmount>& deltas = pos->second;
    dPriorityDelta += deltas.first;
    nFeeDelta += deltas.second;
=======
void CTxMemPool::PrioritiseTransaction(const uint256& hash, const CAmount& nFeeDelta)
{
    {
        LOCK(cs);
        CAmount &delta = mapDeltas[hash];
        delta += nFeeDelta;
        txiter it = mapTx.find(hash);
        if (it != mapTx.end()) {
            mapTx.modify(it, update_fee_delta(delta));
            // Now update all ancestors' modified fees with descendants
            setEntries setAncestors;
            uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
            std::string dummy;
            CalculateMemPoolAncestors(*it, setAncestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy, false);
            for (const txiter& ancestorIt : setAncestors) {
                mapTx.modify(ancestorIt, update_descendant_state(0, nFeeDelta, 0));
            }
        }
    }
    LogPrintf("PrioritiseTransaction: %s feerate += %s\n", hash.ToString(), FormatMoney(nFeeDelta));
}

void CTxMemPool::ApplyDelta(const uint256& hash, CAmount& nFeeDelta) const
{
    LOCK(cs);
    std::map<uint256, CAmount>::const_iterator pos = mapDeltas.find(hash);
    if (pos == mapDeltas.end())
        return;
    const CAmount &delta = pos->second;
    nFeeDelta += delta;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void CTxMemPool::ClearPrioritisation(const uint256 hash)
{
    LOCK(cs);
    mapDeltas.erase(hash);
}

<<<<<<< HEAD

CCoinsViewMemPool::CCoinsViewMemPool(CCoinsView* baseIn, CTxMemPool& mempoolIn) : CCoinsViewBacked(baseIn), mempool(mempoolIn) {}

bool CCoinsViewMemPool::GetCoins(const uint256& txid, CCoins& coins) const
=======
bool CTxMemPool::nullifierExists(const uint256& nullifier) const
{
    LOCK(cs);
    return mapSaplingNullifiers.count(nullifier);
}

bool CTxMemPool::HasNoInputsOf(const CTransaction &tx) const
{
    if (tx.HasZerocoinSpendInputs())
        return true;
    for (unsigned int i = 0; i < tx.vin.size(); i++)
        if (exists(tx.vin[i].prevout.hash))
            return false;
    return true;
}


CCoinsViewMemPool::CCoinsViewMemPool(CCoinsView* baseIn, CTxMemPool& mempoolIn) : CCoinsViewBacked(baseIn), mempool(mempoolIn) {}

bool CCoinsViewMemPool::GetCoin(const COutPoint& outpoint, Coin& coin) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    // If an entry in the mempool exists, always return that one, as it's guaranteed to never
    // conflict with the underlying cache, and it cannot have pruned entries (as it contains full)
    // transactions. First checking the underlying cache risks returning a pruned entry instead.
<<<<<<< HEAD
    CTransaction tx;
    if (mempool.lookup(txid, tx)) {
        coins = CCoins(tx, MEMPOOL_HEIGHT);
        return true;
    }
    return (base->GetCoins(txid, coins) && !coins.IsPruned());
}

bool CCoinsViewMemPool::HaveCoins(const uint256& txid) const
{
    return mempool.exists(txid) || base->HaveCoins(txid);
}
=======
    CTransactionRef ptx = mempool.get(outpoint.hash);
    if (ptx) {
        if (outpoint.n < ptx->vout.size()) {
            coin = Coin(ptx->vout[outpoint.n], MEMPOOL_HEIGHT, false, false);
            return true;
        } else {
            return false;
        }
    }
    return (base->GetCoin(outpoint, coin) && !coin.IsSpent());
}

bool CCoinsViewMemPool::HaveCoin(const COutPoint& outpoint) const
{
    return mempool.exists(outpoint) || base->HaveCoin(outpoint);
}

bool CCoinsViewMemPool::GetNullifier(const uint256& nullifier) const
{
    return mempool.nullifierExists(nullifier) || base->GetNullifier(nullifier);
}

size_t CTxMemPool::DynamicMemoryUsage() const
{
    LOCK(cs);
    // Estimate the overhead of mapTx to be 15 pointers + an allocation, as no exact formula for
    // boost::multi_index_contained is implemented.
    return memusage::MallocUsage(sizeof(CTxMemPoolEntry) + 15 * sizeof(void*)) * mapTx.size() +
            memusage::DynamicUsage(mapNextTx) +
            memusage::DynamicUsage(mapDeltas) +
            memusage::DynamicUsage(mapLinks) +
            cachedInnerUsage +
            memusage::DynamicUsage(mapSaplingNullifiers);
}

void CTxMemPool::RemoveStaged(setEntries &stage, bool updateDescendants, MemPoolRemovalReason reason)
{
    AssertLockHeld(cs);
    UpdateForRemoveFromMempool(stage, updateDescendants);
    for (const txiter& it : stage) {
        removeUnchecked(it, reason);
    }
}

int CTxMemPool::Expire(int64_t time)
{
    LOCK(cs);
    indexed_transaction_set::index<entry_time>::type::iterator it = mapTx.get<entry_time>().begin();
    setEntries toremove;
    while (it != mapTx.get<entry_time>().end() && it->GetTime() < time) {
        toremove.insert(mapTx.project<0>(it));
        it++;
    }
    setEntries stage;
    for (const txiter& removeit : toremove) {
        CalculateDescendants(removeit, stage);
    }
    RemoveStaged(stage, false, MemPoolRemovalReason::EXPIRY);
    return stage.size();
}

bool CTxMemPool::addUnchecked(const uint256&hash, const CTxMemPoolEntry &entry, bool validFeeEstimate)
{
    LOCK(cs);
    setEntries setAncestors;
    uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
    std::string dummy;
    CalculateMemPoolAncestors(entry, setAncestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy);
    return addUnchecked(hash, entry, setAncestors, validFeeEstimate);
}

void CTxMemPool::UpdateChild(txiter entry, txiter child, bool add)
{
    setEntries s;
    if (add && mapLinks[entry].children.insert(child).second) {
        cachedInnerUsage += memusage::IncrementalDynamicUsage(s);
    } else if (!add && mapLinks[entry].children.erase(child)) {
        cachedInnerUsage -= memusage::IncrementalDynamicUsage(s);
    }
}

void CTxMemPool::UpdateParent(txiter entry, txiter parent, bool add)
{
    setEntries s;
    if (add && mapLinks[entry].parents.insert(parent).second) {
        cachedInnerUsage += memusage::IncrementalDynamicUsage(s);
    } else if (!add && mapLinks[entry].parents.erase(parent)) {
        cachedInnerUsage -= memusage::IncrementalDynamicUsage(s);
    }
}

const CTxMemPool::setEntries & CTxMemPool::GetMemPoolParents(txiter entry) const
{
    assert (entry != mapTx.end());
    txlinksMap::const_iterator it = mapLinks.find(entry);
    assert(it != mapLinks.end());
    return it->second.parents;
}

const CTxMemPool::setEntries & CTxMemPool::GetMemPoolChildren(txiter entry) const
{
    assert (entry != mapTx.end());
    txlinksMap::const_iterator it = mapLinks.find(entry);
    assert(it != mapLinks.end());
    return it->second.children;
}

CFeeRate CTxMemPool::GetMinFee(size_t sizelimit) const
{
    LOCK(cs);
    if (!blockSinceLastRollingFeeBump || rollingMinimumFeeRate == 0)
        return CFeeRate(rollingMinimumFeeRate);

    int64_t time = GetTime();
    if (time > lastRollingFeeUpdate + 10) {
        double halflife = ROLLING_FEE_HALFLIFE;
        if (DynamicMemoryUsage() < sizelimit / 4)
            halflife /= 4;
        else if (DynamicMemoryUsage() < sizelimit / 2)
            halflife /= 2;

        rollingMinimumFeeRate = rollingMinimumFeeRate / pow(2.0, (time - lastRollingFeeUpdate) / halflife);
        lastRollingFeeUpdate = time;

        if (rollingMinimumFeeRate < (double)minReasonableRelayFee.GetFeePerK() / 2) {
            rollingMinimumFeeRate = 0;
            return CFeeRate(0);
        }
    }
    return std::max(CFeeRate(rollingMinimumFeeRate), minReasonableRelayFee);
}

void CTxMemPool::trackPackageRemoved(const CFeeRate& rate)
{
    AssertLockHeld(cs);
    if (rate.GetFeePerK() > rollingMinimumFeeRate) {
        rollingMinimumFeeRate = rate.GetFeePerK();
        blockSinceLastRollingFeeBump = false;
    }
}

void CTxMemPool::TrimToSize(size_t sizelimit, std::vector<COutPoint>* pvNoSpendsRemaining)
{
    LOCK(cs);
    unsigned nTxnRemoved = 0;
    CFeeRate maxFeeRateRemoved(0);
    while (DynamicMemoryUsage() > sizelimit) {
        indexed_transaction_set::index<descendant_score>::type::iterator it = mapTx.get<descendant_score>().begin();

        // We set the new mempool min fee to the feerate of the removed set, plus the
        // "minimum reasonable fee rate" (ie some value under which we consider txn
        // to have 0 fee). This way, we don't allow txn to enter mempool with feerate
        // equal to txn which were removed with no block in between.
        CFeeRate removed(it->GetModFeesWithDescendants(), it->GetSizeWithDescendants());
        removed += minReasonableRelayFee;
        trackPackageRemoved(removed);
        maxFeeRateRemoved = std::max(maxFeeRateRemoved, removed);

        setEntries stage;
        CalculateDescendants(mapTx.project<0>(it), stage);
        nTxnRemoved += stage.size();

        std::vector<CTransaction> txn;
        if (pvNoSpendsRemaining) {
            txn.reserve(stage.size());
            for (txiter it: stage)
                txn.push_back(it->GetTx());
        }
        RemoveStaged(stage, false, MemPoolRemovalReason::SIZELIMIT);
        if (pvNoSpendsRemaining) {
            for (const CTransaction& tx: txn) {
                for (const CTxIn& txin: tx.vin) {
                    if (exists(txin.prevout.hash)) continue;
                    if (!mapNextTx.count(txin.prevout)) {
                        pvNoSpendsRemaining->push_back(txin.prevout);
                    }
                }
            }
        }
    }

    if (maxFeeRateRemoved > CFeeRate(0))
        LogPrint(BCLog::MEMPOOL, "Removed %u txn, rolling minimum fee bumped to %s\n", nTxnRemoved, maxFeeRateRemoved.ToString());
}

bool CTxMemPool::TransactionWithinChainLimit(const uint256& txid, size_t chainLimit) const {
    LOCK(cs);
    auto it = mapTx.find(txid);
    return it == mapTx.end() || (it->GetCountWithAncestors() < chainLimit &&
       it->GetCountWithDescendants() < chainLimit);
}

bool CTxMemPool::IsLoaded() const
{
    LOCK(cs);
    return m_is_loaded;
}

void CTxMemPool::SetIsLoaded(bool loaded)
{
    LOCK(cs);
    m_is_loaded = loaded;
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
