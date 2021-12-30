<<<<<<< HEAD
// Copyright (c) 2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "timedata.h"

#include "netbase.h"
#include "sync.h"
#include "guiinterface.h"
#include "util.h"
#include "utilstrencodings.h"

using namespace std;
=======
// Copyright (c) 2014-2017 The Bitcoin developers
// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/pivx-config.h"
#endif

#include "timedata.h"

#include "chainparams.h"
#include "guiinterface.h"
#include "netaddress.h"
#include "sync.h"
#include "util/system.h"
#include "warnings.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static RecursiveMutex cs_nTimeOffset;
static int64_t nTimeOffset = 0;

/**
 * "Never go to sea with two chronometers; take one or three."
 * Our three time sources are:
 *  - System clock
 *  - Median of other nodes clocks
 *  - The user (asking the user to fix the system clock if the first two disagree)
 */
int64_t GetTimeOffset()
{
    LOCK(cs_nTimeOffset);
    return nTimeOffset;
}

int64_t GetAdjustedTime()
{
    return GetTime() + GetTimeOffset();
}

<<<<<<< HEAD
static int64_t abs64(int64_t n)
{
    return (n >= 0 ? n : -n);
}

void AddTimeData(const CNetAddr& ip, int64_t nOffsetSample)
{
    LOCK(cs_nTimeOffset);
    // Ignore duplicates
    static set<CNetAddr> setKnown;
    if (!setKnown.insert(ip).second)
        return;

    // Add data
    static CMedianFilter<int64_t> vTimeOffsets(200, 0);
=======
#define BITCOIN_TIMEDATA_MAX_SAMPLES 200

void AddTimeData(const CNetAddr& ip, int64_t nOffsetSample, int nOffsetLimit)
{
    LOCK(cs_nTimeOffset);
    // Ignore duplicates (Except on regtest where all nodes have the same ip)
    static std::set<CNetAddr> setKnown;
    if (setKnown.size() == BITCOIN_TIMEDATA_MAX_SAMPLES)
        return;
    if (!Params().IsRegTestNet() && !setKnown.insert(ip).second)
        return;

    // Add data
    static CMedianFilter<int64_t> vTimeOffsets(BITCOIN_TIMEDATA_MAX_SAMPLES, 0);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    vTimeOffsets.input(nOffsetSample);
    LogPrintf("Added time data, samples %d, offset %+d (%+d minutes)\n", vTimeOffsets.size(), nOffsetSample, nOffsetSample / 60);

    // There is a known issue here (see issue #4521):
    //
    // - The structure vTimeOffsets contains up to 200 elements, after which
    // any new element added to it will not increase its size, replacing the
    // oldest element.
    //
    // - The condition to update nTimeOffset includes checking whether the
    // number of elements in vTimeOffsets is odd, which will never happen after
    // there are 200 elements.
    //
    // But in this case the 'bug' is protective against some attacks, and may
    // actually explain why we've never seen attacks which manipulate the
    // clock offset.
    //
    // So we should hold off on fixing this and clean it up as part of
    // a timing cleanup that strengthens it in a number of other ways.
    //
    if (vTimeOffsets.size() >= 5 && vTimeOffsets.size() % 2 == 1) {
        int64_t nMedian = vTimeOffsets.median();
        std::vector<int64_t> vSorted = vTimeOffsets.sorted();
        // Only let other nodes change our time by so much
<<<<<<< HEAD
        if (abs64(nMedian) < 70 * 60) {
            nTimeOffset = nMedian;
        } else {
            nTimeOffset = 0;

            static bool fDone;
            if (!fDone) {
                // If nobody has a time different than ours but within 5 minutes of ours, give a warning
                bool fMatch = false;
                for (int64_t nOffset : vSorted)
                    if (nOffset != 0 && abs64(nOffset) < 5 * 60)
                        fMatch = true;

                if (!fMatch) {
                    fDone = true;
                    string strMessage = _("Warning: Please check that your computer's date and time are correct! If your clock is wrong PRCY will not work properly.");
                    strMiscWarning = strMessage;
                    LogPrintf("*** %s\n", strMessage);
                    uiInterface.ThreadSafeMessageBox(strMessage, "", CClientUIInterface::MSG_WARNING);
                }
            }
        }
        if (fDebug) {
=======
        if (abs64(nMedian) < nOffsetLimit) {
            nTimeOffset = nMedian;
            SetMiscWarning("");
        } else {
            nTimeOffset = (nMedian > 0 ? 1 : -1) * nOffsetLimit;
            std::string strMessage = strprintf(_("Warning: Please check that your computer's date and time are correct! If your clock is wrong %s will not work properly."), PACKAGE_NAME);
            SetMiscWarning(strMessage);
            LogPrintf("*** %s\n", strMessage);
            uiInterface.ThreadSafeMessageBox(strMessage, "", CClientUIInterface::MSG_ERROR);
        }
        if (!gArgs.GetBoolArg("-shrinkdebugfile", g_logger->DefaultShrinkDebugFile())) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            for (int64_t n : vSorted)
                LogPrintf("%+d  ", n);
            LogPrintf("|  ");
        }
<<<<<<< HEAD
        LogPrintf("nTimeOffset = %+d  (%+d minutes)\n", nTimeOffset, nTimeOffset / 60);
    }
}
=======
        LogPrintf("nTimeOffset = %+d\n", nTimeOffset);
    }
}

// Time Protocol V2
// Timestamp for time protocol V2: slot duration 15 seconds
int64_t GetTimeSlot(const int64_t nTime)
{
    const int slotLen = Params().GetConsensus().nTimeSlotLength;
    return (nTime / slotLen) * slotLen;
}

int64_t GetCurrentTimeSlot()
{
    return GetTimeSlot(GetAdjustedTime());
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
