// Copyright (c) 2012 Pieter Wuille
<<<<<<< HEAD
=======
// Copyright (c) 2012-2015 The Bitcoin developers
// Copyright (c) 2017-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_ADDRMAN_H
#define BITCOIN_ADDRMAN_H

<<<<<<< HEAD
#include "netbase.h"
=======
#if defined(HAVE_CONFIG_H)
#include "config/pivx-config.h"
#endif //HAVE_CONFIG_H

#include "clientversion.h"
#include "netaddress.h"
#include "optional.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "protocol.h"
#include "random.h"
#include "sync.h"
#include "timedata.h"
<<<<<<< HEAD
#include "util.h"

#include <map>
#include <set>
#include <stdint.h>
#include <vector>

/** 
 * Extended statistics about a CAddress 
=======
#include "tinyformat.h"
#include "util/system.h"

#include <fs.h>
#include <hash.h>
#include <iostream>
#include <map>
#include <set>
#include <stdint.h>
#include <streams.h>
#include <vector>

/**
 * Extended statistics about a CAddress
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 */
class CAddrInfo : public CAddress
{
public:
    //! last try whatsoever by us (memory only)
<<<<<<< HEAD
    int64_t nLastTry;
=======
    int64_t nLastTry{0};

    //! last counted attempt (memory only)
    int64_t nLastCountAttempt{0};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

private:
    //! where knowledge about this address first came from
    CNetAddr source;

    //! last successful connection by us
<<<<<<< HEAD
    int64_t nLastSuccess;

    //! connection attempts since last successful attempt
    int nAttempts;

    //! reference count in new sets (memory only)
    int nRefCount;

    //! in tried set? (memory only)
    bool fInTried;

    //! position in vRandom
    int nRandomPos;
=======
    int64_t nLastSuccess{0};

    //! connection attempts since last successful attempt
    int nAttempts{0};

    //! reference count in new sets (memory only)
    int nRefCount{0};

    //! in tried set? (memory only)
    bool fInTried{false};

    //! position in vRandom
    int nRandomPos{-1};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    friend class CAddrMan;

public:
<<<<<<< HEAD
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(*(CAddress*)this);
        READWRITE(source);
        READWRITE(nLastSuccess);
        READWRITE(nAttempts);
    }

    void Init()
    {
        nLastSuccess = 0;
        nLastTry = 0;
        nAttempts = 0;
        nRefCount = 0;
        fInTried = false;
        nRandomPos = -1;
=======

    SERIALIZE_METHODS(CAddrInfo, obj)
    {
        READWRITEAS(CAddress, obj);
        READWRITE(obj.source, obj.nLastSuccess, obj.nAttempts);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    CAddrInfo(const CAddress& addrIn, const CNetAddr& addrSource) : CAddress(addrIn), source(addrSource)
    {
<<<<<<< HEAD
        Init();
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    CAddrInfo() : CAddress(), source()
    {
<<<<<<< HEAD
        Init();
    }

    //! Calculate in which "tried" bucket this entry belongs
    int GetTriedBucket(const uint256& nKey) const;

    //! Calculate in which "new" bucket this entry belongs, given a certain source
    int GetNewBucket(const uint256& nKey, const CNetAddr& src) const;

    //! Calculate in which "new" bucket this entry belongs, using its default source
    int GetNewBucket(const uint256& nKey) const
    {
        return GetNewBucket(nKey, source);
=======
    }

    //! Calculate in which "tried" bucket this entry belongs
    int GetTriedBucket(const uint256& nKey, const std::vector<bool>& asmap) const;

    //! Calculate in which "new" bucket this entry belongs, given a certain source
    int GetNewBucket(const uint256& nKey, const CNetAddr& src, const std::vector<bool>& asmap) const;

    //! Calculate in which "new" bucket this entry belongs, using its default source
    int GetNewBucket(const uint256& nKey, const std::vector<bool>& asmap) const
    {
        return GetNewBucket(nKey, source, asmap);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    //! Calculate in which position of a bucket to store this entry.
    int GetBucketPosition(const uint256& nKey, bool fNew, int nBucket) const;

    //! Determine whether the statistics about this entry are bad enough so that it can just be deleted
    bool IsTerrible(int64_t nNow = GetAdjustedTime()) const;

    //! Calculate the relative chance this entry should be given when selecting nodes to connect to
    double GetChance(int64_t nNow = GetAdjustedTime()) const;
};

/** Stochastic address manager
 *
 * Design goals:
<<<<<<< HEAD
 *  * Keep the address tables in-memory, and asynchronously dump the entire to able in peers.dat.
=======
 *  * Keep the address tables in-memory, and asynchronously dump the entire table to peers.dat.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *  * Make sure no (localized) attacker can fill the entire table with his nodes/addresses.
 *
 * To that end:
 *  * Addresses are organized into buckets.
<<<<<<< HEAD
 *    * Address that have not yet been tried go into 1024 "new" buckets.
 *      * Based on the address range (/16 for IPv4) of source of the information, 64 buckets are selected at random
 *      * The actual bucket is chosen from one of these, based on the range the address itself is located.
 *      * One single address can occur in up to 8 different buckets, to increase selection chances for addresses that
=======
 *    * Addresses that have not yet been tried go into 1024 "new" buckets.
 *      * Based on the address range (/16 for IPv4) of the source of information, 64 buckets are selected at random.
 *      * The actual bucket is chosen from one of these, based on the range in which the address itself is located.
 *      * One single address can occur in up to 8 different buckets to increase selection chances for addresses that
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *        are seen frequently. The chance for increasing this multiplicity decreases exponentially.
 *      * When adding a new address to a full bucket, a randomly chosen entry (with a bias favoring less recently seen
 *        ones) is removed from it first.
 *    * Addresses of nodes that are known to be accessible go into 256 "tried" buckets.
 *      * Each address range selects at random 8 of these buckets.
 *      * The actual bucket is chosen from one of these, based on the full address.
 *      * When adding a new good address to a full bucket, a randomly chosen entry (with a bias favoring less recently
 *        tried ones) is evicted from it, back to the "new" buckets.
 *    * Bucket selection is based on cryptographic hashing, using a randomly-generated 256-bit key, which should not
 *      be observable by adversaries.
 *    * Several indexes are kept for high performance. Defining DEBUG_ADDRMAN will introduce frequent (and expensive)
 *      consistency checks for the entire data structure.
 */

//! total number of buckets for tried addresses
#define ADDRMAN_TRIED_BUCKET_COUNT_LOG2 8

//! total number of buckets for new addresses
#define ADDRMAN_NEW_BUCKET_COUNT_LOG2 10

//! maximum allowed number of entries in buckets for new and tried addresses
#define ADDRMAN_BUCKET_SIZE_LOG2 6

//! over how many buckets entries with tried addresses from a single group (/16 for IPv4) are spread
#define ADDRMAN_TRIED_BUCKETS_PER_GROUP 8

//! over how many buckets entries with new addresses originating from a single group are spread
#define ADDRMAN_NEW_BUCKETS_PER_SOURCE_GROUP 64

//! in how many buckets for entries with new addresses a single address may occur
#define ADDRMAN_NEW_BUCKETS_PER_ADDRESS 8

//! how old addresses can maximally be
#define ADDRMAN_HORIZON_DAYS 30

//! after how many failed attempts we give up on a new node
#define ADDRMAN_RETRIES 3

//! how many successive failures are allowed ...
#define ADDRMAN_MAX_FAILURES 10

//! ... in at least this many days
#define ADDRMAN_MIN_FAIL_DAYS 7

<<<<<<< HEAD
//! the maximum percentage of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX_PCT 23

//! the maximum number of nodes to return in a getaddr call
#define ADDRMAN_GETADDR_MAX 2500
=======
//! how recent a successful connection should be before we allow an address to be evicted from tried
#define ADDRMAN_REPLACEMENT_HOURS 4
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

//! Convenience
#define ADDRMAN_TRIED_BUCKET_COUNT (1 << ADDRMAN_TRIED_BUCKET_COUNT_LOG2)
#define ADDRMAN_NEW_BUCKET_COUNT (1 << ADDRMAN_NEW_BUCKET_COUNT_LOG2)
#define ADDRMAN_BUCKET_SIZE (1 << ADDRMAN_BUCKET_SIZE_LOG2)

<<<<<<< HEAD
/** 
 * Stochastical (IP) address manager 
 */
class CAddrMan
{
private:
    //! critical section to protect the inner data structures
    mutable RecursiveMutex cs;

    //! secret key to randomize bucket select with
    uint256 nKey;

    //! last used nId
    int nIdCount;

    //! table with information about all nIds
    std::map<int, CAddrInfo> mapInfo;

    //! find an nId based on its network address
    std::map<CNetAddr, int> mapAddr;

    //! randomly-ordered vector of all nIds
    std::vector<int> vRandom;

    // number of "tried" entries
    int nTried;

    //! list of "tried" buckets
    int vvTried[ADDRMAN_TRIED_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE];

    //! number of (unique) "new" entries
    int nNew;

    //! list of "new" buckets
    int vvNew[ADDRMAN_NEW_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE];

protected:
=======
//! the maximum number of tried addr collisions to store
#define ADDRMAN_SET_TRIED_COLLISION_SIZE 10

//! the maximum time we'll spend trying to resolve a tried table collision, in seconds
static const int64_t ADDRMAN_TEST_WINDOW = 40*60; // 40 minutes

/**
 * Stochastical (IP) address manager
 */
class CAddrMan
{
friend class CAddrManTest;
protected:
    //! critical section to protect the inner data structures
    mutable RecursiveMutex cs;

private:
    //! Serialization versions.
    enum Format : uint8_t {
        V0_HISTORICAL = 0,    //!< historic format, before commit e6b343d88
        V1_DETERMINISTIC = 1, //!< for pre-asmap files
        V2_ASMAP = 2,         //!< for files including asmap version
        V3_BIP155 = 3,        //!< same as V2_ASMAP plus addresses are in BIP155 format
    };

    //! The maximum format this software knows it can unserialize. Also, we always serialize
    //! in this format.
    //! The format (first byte in the serialized stream) can be higher than this and
    //! still this software may be able to unserialize the file - if the second byte
    //! (see `lowest_compatible` in `Unserialize()`) is less or equal to this.
    static constexpr Format FILE_FORMAT = Format::V3_BIP155;

    //! The initial value of a field that is incremented every time an incompatible format
    //! change is made (such that old software versions would not be able to parse and
    //! understand the new file format). This is 32 because we overtook the "key size"
    //! field which was 32 historically.
    //! @note Don't increment this. Increment `lowest_compatible` in `Serialize()` instead.
    static constexpr uint8_t INCOMPATIBILITY_BASE = 32;

    //! last used nId
    int nIdCount GUARDED_BY(cs);

    //! table with information about all nIds
    std::map<int, CAddrInfo> mapInfo GUARDED_BY(cs);

    //! find an nId based on its network address
    std::map<CNetAddr, int> mapAddr GUARDED_BY(cs);

    //! randomly-ordered vector of all nIds
    std::vector<int> vRandom GUARDED_BY(cs);

    // number of "tried" entries
    int nTried GUARDED_BY(cs);

    //! list of "tried" buckets
    int vvTried[ADDRMAN_TRIED_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE] GUARDED_BY(cs);

    //! number of (unique) "new" entries
    int nNew GUARDED_BY(cs);

    //! list of "new" buckets
    int vvNew[ADDRMAN_NEW_BUCKET_COUNT][ADDRMAN_BUCKET_SIZE] GUARDED_BY(cs);

    //! last time Good was called (memory only)
    int64_t nLastGood GUARDED_BY(cs);

    //! Holds addrs inserted into tried table that collide with existing entries. Test-before-evict discipline used to resolve these collisions.
    std::set<int> m_tried_collisions;

protected:
    //! secret key to randomize bucket select with
    uint256 nKey;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    //! Source of random numbers for randomization in inner loops
    FastRandomContext insecure_rand;

    //! Find an entry.
<<<<<<< HEAD
    CAddrInfo* Find(const CNetAddr& addr, int* pnId = NULL);

    //! find an entry, creating it if necessary.
    //! nTime and nServices of the found node are updated, if necessary.
    CAddrInfo* Create(const CAddress& addr, const CNetAddr& addrSource, int* pnId = NULL);

    //! Swap two elements in vRandom.
    void SwapRandom(unsigned int nRandomPos1, unsigned int nRandomPos2);

    //! Move an entry from the "new" table(s) to the "tried" table
    void MakeTried(CAddrInfo& info, int nId);

    //! Delete an entry. It must not be in tried, and have refcount 0.
    void Delete(int nId);

    //! Clear a position in a "new" table. This is the only place where entries are actually deleted.
    void ClearNew(int nUBucket, int nUBucketPos);

    //! Mark an entry "good", possibly moving it from "new" to "tried".
    void Good_(const CService& addr, int64_t nTime);

    //! Add an entry to the "new" table.
    bool Add_(const CAddress& addr, const CNetAddr& source, int64_t nTimePenalty);

    //! Mark an entry as attempted to connect.
    void Attempt_(const CService& addr, int64_t nTime);

    //! Select an address to connect to.
    //! nUnkBias determines how much to favor new addresses over tried ones (min=0, max=100)
    CAddrInfo Select_();

#ifdef DEBUG_ADDRMAN
    //! Perform consistency check. Returns an error code or zero.
    int Check_();
#endif

    //! Select several addresses at once.
    void GetAddr_(std::vector<CAddress>& vAddr);

    //! Mark an entry as currently-connected-to.
    void Connected_(const CService& addr, int64_t nTime);

public:
    /**
     * serialized format:
     * * version byte (currently 1)
     * * 0x20 + nKey (serialized as if it were a vector, for backward compatibility)
=======
    CAddrInfo* Find(const CNetAddr& addr, int *pnId = nullptr) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! find an entry, creating it if necessary.
    //! nTime and nServices of the found node are updated, if necessary.
    CAddrInfo* Create(const CAddress& addr, const CNetAddr& addrSource, int* pnId = nullptr) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Swap two elements in vRandom.
    void SwapRandom(unsigned int nRandomPos1, unsigned int nRandomPos2) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Move an entry from the "new" table(s) to the "tried" table
    void MakeTried(CAddrInfo& info, int nId) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Delete an entry. It must not be in tried, and have refcount 0.
    void Delete(int nId) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Clear a position in a "new" table. This is the only place where entries are actually deleted.
    void ClearNew(int nUBucket, int nUBucketPos) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Mark an entry "good", possibly moving it from "new" to "tried".
    void Good_(const CService& addr, bool test_before_evict, int64_t time) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Add an entry to the "new" table.
    bool Add_(const CAddress& addr, const CNetAddr& source, int64_t nTimePenalty) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Mark an entry as attempted to connect.
    void Attempt_(const CService& addr, bool fCountFailure, int64_t nTime) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Select an address to connect to, if newOnly is set to true, only the new table is selected from.
    CAddrInfo Select_(bool newOnly) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! See if any to-be-evicted tried table entries have been tested and if so resolve the collisions.
    void ResolveCollisions_() EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Return a random to-be-evicted tried table address.
    CAddrInfo SelectTriedCollision_() EXCLUSIVE_LOCKS_REQUIRED(cs);

#ifdef DEBUG_ADDRMAN
    //! Perform consistency check. Returns an error code or zero.
    int Check_() EXCLUSIVE_LOCKS_REQUIRED(cs);
#endif

    /**
     * Return all or many randomly selected addresses, optionally by network.
     *
     * @param[out] vAddr         Vector of randomly selected addresses from vRandom.
     * @param[in] max_addresses  Maximum number of addresses to return (0 = all).
     * @param[in] max_pct        Maximum percentage of addresses to return (0 = all).
     * @param[in] network        Select only addresses of this network (nullopt = all).
     */
    void GetAddr_(std::vector<CAddress>& vAddr, size_t max_addresses, size_t max_pct, Optional<Network> network) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Mark an entry as currently-connected-to.
    void Connected_(const CService& addr, int64_t nTime) EXCLUSIVE_LOCKS_REQUIRED(cs);

    //! Update an entry's service bits.
    void SetServices_(const CService& addr, ServiceFlags nServices) EXCLUSIVE_LOCKS_REQUIRED(cs);

public:
    // Compressed IP->ASN mapping, loaded from a file when a node starts.
    // Should be always empty if no file was provided.
    // This mapping is then used for bucketing nodes in Addrman.
    //
    // If asmap is provided, nodes will be bucketed by
    // AS they belong to, in order to make impossible for a node
    // to connect to several nodes hosted in a single AS.
    // This is done in response to Erebus attack, but also to generally
    // diversify the connections every node creates,
    // especially useful when a large fraction of nodes
    // operate under a couple of cloud providers.
    //
    // If a new asmap was provided, the existing records
    // would be re-bucketed accordingly.
    std::vector<bool> m_asmap;

    // Read asmap from provided binary file
    static std::vector<bool> DecodeAsmap(fs::path path);


    /**
     * Serialized format.
     * * format version byte (@see `Format`)
     * * lowest compatible format version byte. This is used to help old software decide
     *   whether to parse the file. For example:
     *   * PIVX Core version N knows how to parse up to format=3. If a new format=4 is
     *     introduced in version N+1 that is compatible with format=3 and it is known that
     *     version N will be able to parse it, then version N+1 will write
     *     (format=4, lowest_compatible=3) in the first two bytes of the file, and so
     *     version N will still try to parse it.
     *   * PIVX Core version N+2 introduces a new incompatible format=5. It will write
     *     (format=5, lowest_compatible=5) and so any versions that do not know how to parse
     *     format=5 will not try to read the file.
     * * nKey
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     * * nNew
     * * nTried
     * * number of "new" buckets XOR 2**30
     * * all nNew addrinfos in vvNew
     * * all nTried addrinfos in vvTried
     * * for each bucket:
     *   * number of elements
     *   * for each element: index
     *
     * 2**30 is xorred with the number of buckets to make addrman deserializer v0 detect it
     * as incompatible. This is necessary because it did not check the version number on
     * deserialization.
     *
     * Notice that vvTried, mapAddr and vVector are never encoded explicitly;
     * they are instead reconstructed from the other information.
     *
<<<<<<< HEAD
     * vvNew is serialized, but only used if ADDRMAN_UNKOWN_BUCKET_COUNT didn't change,
=======
     * vvNew is serialized, but only used if ADDRMAN_UNKNOWN_BUCKET_COUNT didn't change,
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
     * otherwise it is reconstructed as well.
     *
     * This format is more complex, but significantly smaller (at most 1.5 MiB), and supports
     * changes to the ADDRMAN_ parameters without breaking the on-disk structure.
     *
<<<<<<< HEAD
     * We don't use ADD_SERIALIZE_METHODS since the serialization and deserialization code has
     * very little in common.
     */
    template <typename Stream>
    void Serialize(Stream& s, int nType, int nVersionDummy) const
    {
        LOCK(cs);

        unsigned char nVersion = 1;
        s << nVersion;
        s << ((unsigned char)32);
=======
     * We don't use SERIALIZE_METHODS since the serialization and deserialization code has
     * very little in common.
     */
    template <typename Stream>
    void Serialize(Stream& s_) const
    {
        LOCK(cs);

        // Always serialize in the latest version (FILE_FORMAT).

        OverrideStream<Stream> s(&s_, s_.GetType(), s_.GetVersion() | ADDRV2_FORMAT);

        s << static_cast<uint8_t>(FILE_FORMAT);

        // Increment `lowest_compatible` if a newly introduced format is incompatible with
        // the previous one.
        static constexpr uint8_t lowest_compatible = Format::V3_BIP155;
        s << static_cast<uint8_t>(INCOMPATIBILITY_BASE + lowest_compatible);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        s << nKey;
        s << nNew;
        s << nTried;

        int nUBuckets = ADDRMAN_NEW_BUCKET_COUNT ^ (1 << 30);
        s << nUBuckets;
        std::map<int, int> mapUnkIds;
        int nIds = 0;
<<<<<<< HEAD
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); it++) {
            mapUnkIds[(*it).first] = nIds;
            const CAddrInfo& info = (*it).second;
=======
        for (const auto& entry : mapInfo) {
            mapUnkIds[entry.first] = nIds;
            const CAddrInfo& info = entry.second;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (info.nRefCount) {
                assert(nIds != nNew); // this means nNew was wrong, oh ow
                s << info;
                nIds++;
            }
        }
        nIds = 0;
<<<<<<< HEAD
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); it++) {
            const CAddrInfo& info = (*it).second;
=======
        for (const auto& entry : mapInfo) {
            const CAddrInfo& info = entry.second;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            if (info.fInTried) {
                assert(nIds != nTried); // this means nTried was wrong, oh ow
                s << info;
                nIds++;
            }
        }
        for (int bucket = 0; bucket < ADDRMAN_NEW_BUCKET_COUNT; bucket++) {
            int nSize = 0;
            for (int i = 0; i < ADDRMAN_BUCKET_SIZE; i++) {
                if (vvNew[bucket][i] != -1)
                    nSize++;
            }
            s << nSize;
            for (int i = 0; i < ADDRMAN_BUCKET_SIZE; i++) {
                if (vvNew[bucket][i] != -1) {
                    int nIndex = mapUnkIds[vvNew[bucket][i]];
                    s << nIndex;
                }
            }
        }
<<<<<<< HEAD
    }

    template <typename Stream>
    void Unserialize(Stream& s, int nType, int nVersionDummy)
=======
        // Store asmap version after bucket entries so that it
        // can be ignored by older clients for backward compatibility.
        uint256 asmap_version;
        if (m_asmap.size() != 0) {
            asmap_version = SerializeHash(m_asmap);
        }
        s << asmap_version;
    }

    template <typename Stream>
    void Unserialize(Stream& s_)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        LOCK(cs);

        Clear();

<<<<<<< HEAD
        unsigned char nVersion;
        s >> nVersion;
        unsigned char nKeySize;
        s >> nKeySize;
        if (nKeySize != 32) throw std::ios_base::failure("Incorrect keysize in addrman deserialization");
=======
        Format format;
        s_ >> Using<CustomUintFormatter<1>>(format);

        int stream_version = s_.GetVersion();
        if (format >= Format::V3_BIP155) {
            // Add ADDRV2_FORMAT to the version so that the CNetAddr and CAddress
            // unserialize methods know that an address in addrv2 format is coming.
            stream_version |= ADDRV2_FORMAT;
        }

        OverrideStream<Stream> s(&s_, s_.GetType(), stream_version);

        uint8_t compat;
        s >> compat;
        const uint8_t lowest_compatible = compat - INCOMPATIBILITY_BASE;
        if (lowest_compatible > FILE_FORMAT) {
            throw std::ios_base::failure(strprintf(
                "Unsupported format of addrman database: %u. It is compatible with formats >=%u, "
                "but the maximum supported by this version of %s is %u.",
                format, lowest_compatible, PACKAGE_NAME, static_cast<uint8_t>(FILE_FORMAT)));
        }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        s >> nKey;
        s >> nNew;
        s >> nTried;
        int nUBuckets = 0;
        s >> nUBuckets;
<<<<<<< HEAD
        if (nVersion != 0) {
            nUBuckets ^= (1 << 30);
        }

=======
        if (format >= Format::V1_DETERMINISTIC) {
            nUBuckets ^= (1 << 30);
        }

        if (nNew > ADDRMAN_NEW_BUCKET_COUNT * ADDRMAN_BUCKET_SIZE) {
            throw std::ios_base::failure("Corrupt CAddrMan serialization, nNew exceeds limit.");
        }

        if (nTried > ADDRMAN_TRIED_BUCKET_COUNT * ADDRMAN_BUCKET_SIZE) {
            throw std::ios_base::failure("Corrupt CAddrMan serialization, nTried exceeds limit.");
        }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        // Deserialize entries from the new table.
        for (int n = 0; n < nNew; n++) {
            CAddrInfo& info = mapInfo[n];
            s >> info;
            mapAddr[info] = n;
            info.nRandomPos = vRandom.size();
            vRandom.push_back(n);
<<<<<<< HEAD
            if (nVersion != 1 || nUBuckets != ADDRMAN_NEW_BUCKET_COUNT) {
                // In case the new table data cannot be used (nVersion unknown, or bucket count wrong),
                // immediately try to give them a reference based on their primary source address.
                int nUBucket = info.GetNewBucket(nKey);
                int nUBucketPos = info.GetBucketPosition(nKey, true, nUBucket);
                if (vvNew[nUBucket][nUBucketPos] == -1) {
                    vvNew[nUBucket][nUBucketPos] = n;
                    info.nRefCount++;
                }
            }
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        nIdCount = nNew;

        // Deserialize entries from the tried table.
        int nLost = 0;
        for (int n = 0; n < nTried; n++) {
            CAddrInfo info;
            s >> info;
<<<<<<< HEAD
            int nKBucket = info.GetTriedBucket(nKey);
=======
            int nKBucket = info.GetTriedBucket(nKey, m_asmap);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            int nKBucketPos = info.GetBucketPosition(nKey, false, nKBucket);
            if (vvTried[nKBucket][nKBucketPos] == -1) {
                info.nRandomPos = vRandom.size();
                info.fInTried = true;
                vRandom.push_back(nIdCount);
                mapInfo[nIdCount] = info;
                mapAddr[info] = nIdCount;
                vvTried[nKBucket][nKBucketPos] = nIdCount;
                nIdCount++;
            } else {
                nLost++;
            }
        }
        nTried -= nLost;

<<<<<<< HEAD
        // Deserialize positions in the new table (if possible).
=======
        // Store positions in the new table buckets to apply later (if possible).
        std::map<int, int> entryToBucket; // Represents which entry belonged to which bucket when serializing

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        for (int bucket = 0; bucket < nUBuckets; bucket++) {
            int nSize = 0;
            s >> nSize;
            for (int n = 0; n < nSize; n++) {
                int nIndex = 0;
                s >> nIndex;
                if (nIndex >= 0 && nIndex < nNew) {
<<<<<<< HEAD
                    CAddrInfo& info = mapInfo[nIndex];
                    int nUBucketPos = info.GetBucketPosition(nKey, true, bucket);
                    if (nVersion == 1 && nUBuckets == ADDRMAN_NEW_BUCKET_COUNT && vvNew[bucket][nUBucketPos] == -1 && info.nRefCount < ADDRMAN_NEW_BUCKETS_PER_ADDRESS) {
                        info.nRefCount++;
                        vvNew[bucket][nUBucketPos] = nIndex;
                    }
=======
                    entryToBucket[nIndex] = bucket;
                }
            }
        }

        uint256 supplied_asmap_version;
        if (m_asmap.size() != 0) {
            supplied_asmap_version = SerializeHash(m_asmap);
        }
        uint256 serialized_asmap_version;
        if (format >= Format::V2_ASMAP) {
            s >> serialized_asmap_version;
        }

        for (int n = 0; n < nNew; n++) {
            CAddrInfo &info = mapInfo[n];
            int bucket = entryToBucket[n];
            int nUBucketPos = info.GetBucketPosition(nKey, true, bucket);
            if (format >= Format::V2_ASMAP && nUBuckets == ADDRMAN_NEW_BUCKET_COUNT && vvNew[bucket][nUBucketPos] == -1 &&
                info.nRefCount < ADDRMAN_NEW_BUCKETS_PER_ADDRESS && serialized_asmap_version == supplied_asmap_version) {
                // Bucketing has not changed, using existing bucket positions for the new table
                vvNew[bucket][nUBucketPos] = n;
                info.nRefCount++;
            } else {
                // In case the new table data cannot be used (format unknown, bucket count wrong or new asmap),
                // try to give them a reference based on their primary source address.
                LogPrint(BCLog::ADDRMAN, "Bucketing method was updated, re-bucketing addrman entries from disk\n");
                bucket = info.GetNewBucket(nKey, m_asmap);
                nUBucketPos = info.GetBucketPosition(nKey, true, bucket);
                if (vvNew[bucket][nUBucketPos] == -1) {
                    vvNew[bucket][nUBucketPos] = n;
                    info.nRefCount++;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                }
            }
        }

        // Prune new entries with refcount 0 (as a result of collisions).
        int nLostUnk = 0;
        for (std::map<int, CAddrInfo>::const_iterator it = mapInfo.begin(); it != mapInfo.end();) {
            if (it->second.fInTried == false && it->second.nRefCount == 0) {
                std::map<int, CAddrInfo>::const_iterator itCopy = it++;
                Delete(itCopy->first);
                nLostUnk++;
            } else {
                it++;
            }
        }
        if (nLost + nLostUnk > 0) {
<<<<<<< HEAD
            LogPrint("addrman", "addrman lost %i new and %i tried addresses due to collisions\n", nLostUnk, nLost);
=======
            LogPrint(BCLog::ADDRMAN, "addrman lost %i new and %i tried addresses due to collisions\n", nLostUnk, nLost);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }

        Check();
    }

<<<<<<< HEAD
    unsigned int GetSerializeSize(int nType, int nVersion) const
    {
        return (CSizeComputer(nType, nVersion) << *this).size();
    }

    void Clear()
    {
        std::vector<int>().swap(vRandom);
        nKey = GetRandHash();
=======
    void Clear()
    {
        LOCK(cs);
        std::vector<int>().swap(vRandom);
        nKey = insecure_rand.rand256();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        for (size_t bucket = 0; bucket < ADDRMAN_NEW_BUCKET_COUNT; bucket++) {
            for (size_t entry = 0; entry < ADDRMAN_BUCKET_SIZE; entry++) {
                vvNew[bucket][entry] = -1;
            }
        }
        for (size_t bucket = 0; bucket < ADDRMAN_TRIED_BUCKET_COUNT; bucket++) {
            for (size_t entry = 0; entry < ADDRMAN_BUCKET_SIZE; entry++) {
                vvTried[bucket][entry] = -1;
            }
        }

        nIdCount = 0;
        nTried = 0;
        nNew = 0;
<<<<<<< HEAD
=======
        nLastGood = 1; //Initially at 1 so that "never" is strictly worse.
        mapInfo.clear();
        mapAddr.clear();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    CAddrMan()
    {
        Clear();
    }

    ~CAddrMan()
    {
<<<<<<< HEAD
        nKey = uint256();
    }

    //! Return the number of (unique) addresses in all tables.
    int size()
    {
=======
        nKey.SetNull();
    }

    //! Return the number of (unique) addresses in all tables.
    size_t size() const
    {
        LOCK(cs); // TODO: Cache this in an atomic to avoid this overhead
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return vRandom.size();
    }

    //! Consistency check
    void Check()
    {
#ifdef DEBUG_ADDRMAN
        {
            LOCK(cs);
            int err;
            if ((err = Check_()))
                LogPrintf("ADDRMAN CONSISTENCY CHECK FAILED!!! err=%i\n", err);
        }
#endif
    }

    //! Add a single address.
    bool Add(const CAddress& addr, const CNetAddr& source, int64_t nTimePenalty = 0)
    {
<<<<<<< HEAD
        bool fRet = false;
        {
            LOCK(cs);
            Check();
            fRet |= Add_(addr, source, nTimePenalty);
            Check();
        }
        if (fRet)
            LogPrint("addrman", "Added %s from %s: %i tried, %i new\n", addr.ToStringIPPort(), source.ToString(), nTried, nNew);
=======
        LOCK(cs);
        bool fRet = false;
        Check();
        fRet |= Add_(addr, source, nTimePenalty);
        Check();
        if (fRet) {
            LogPrint(BCLog::ADDRMAN, "Added %s from %s: %i tried, %i new\n", addr.ToStringIPPort(), source.ToString(), nTried, nNew);
        }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return fRet;
    }

    //! Add multiple addresses.
    bool Add(const std::vector<CAddress>& vAddr, const CNetAddr& source, int64_t nTimePenalty = 0)
    {
<<<<<<< HEAD
        int nAdd = 0;
        {
            LOCK(cs);
            Check();
            for (std::vector<CAddress>::const_iterator it = vAddr.begin(); it != vAddr.end(); it++)
                nAdd += Add_(*it, source, nTimePenalty) ? 1 : 0;
            Check();
        }
        if (nAdd)
            LogPrint("addrman", "Added %i addresses from %s: %i tried, %i new\n", nAdd, source.ToString(), nTried, nNew);
=======
        LOCK(cs);
        int nAdd = 0;
        Check();
        for (std::vector<CAddress>::const_iterator it = vAddr.begin(); it != vAddr.end(); it++)
            nAdd += Add_(*it, source, nTimePenalty) ? 1 : 0;
        Check();
        if (nAdd) {
            LogPrint(BCLog::ADDRMAN, "Added %i addresses from %s: %i tried, %i new\n", nAdd, source.ToString(), nTried, nNew);
        }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return nAdd > 0;
    }

    //! Mark an entry as accessible.
<<<<<<< HEAD
    void Good(const CService& addr, int64_t nTime = GetAdjustedTime())
    {
        {
            LOCK(cs);
            Check();
            Good_(addr, nTime);
            Check();
        }
    }

    //! Mark an entry as connection attempted to.
    void Attempt(const CService& addr, int64_t nTime = GetAdjustedTime())
    {
        {
            LOCK(cs);
            Check();
            Attempt_(addr, nTime);
            Check();
        }
=======
    void Good(const CService& addr, bool test_before_evict = true, int64_t nTime = GetAdjustedTime())
    {
        LOCK(cs);
        Check();
        Good_(addr, test_before_evict, nTime);
        Check();
    }

    //! Mark an entry as connection attempted to.
    void Attempt(const CService& addr, bool fCountFailure, int64_t nTime = GetAdjustedTime())
    {
        LOCK(cs);
        Check();
        Attempt_(addr, fCountFailure, nTime);
        Check();
    }

    //! See if any to-be-evicted tried table entries have been tested and if so resolve the collisions.
    void ResolveCollisions()
    {
        LOCK(cs);
        Check();
        ResolveCollisions_();
        Check();
    }

    //! Randomly select an address in tried that another address is attempting to evict.
    CAddrInfo SelectTriedCollision()
    {
        CAddrInfo ret;
        {
            LOCK(cs);
            Check();
            ret = SelectTriedCollision_();
            Check();
        }
        return ret;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    /**
     * Choose an address to connect to.
     * nUnkBias determines how much "new" entries are favored over "tried" ones (0-100).
     */
<<<<<<< HEAD
    CAddrInfo Select()
=======
    CAddrInfo Select(bool newOnly = false)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        CAddrInfo addrRet;
        {
            LOCK(cs);
            Check();
<<<<<<< HEAD
            addrRet = Select_();
=======
            addrRet = Select_(newOnly);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            Check();
        }
        return addrRet;
    }

<<<<<<< HEAD
    //! Return a bunch of addresses, selected at random.
    std::vector<CAddress> GetAddr()
=======
    /**
     * Return all or many randomly selected addresses, optionally by network.
     *
     * @param[in] max_addresses  Maximum number of addresses to return (0 = all).
     * @param[in] max_pct        Maximum percentage of addresses to return (0 = all).
     * @param[in] network        Select only addresses of this network (nullopt = all).
     */
    std::vector<CAddress> GetAddr(size_t max_addresses, size_t max_pct, Optional<Network> network)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        Check();
        std::vector<CAddress> vAddr;
        {
            LOCK(cs);
<<<<<<< HEAD
            GetAddr_(vAddr);
=======
            GetAddr_(vAddr, max_addresses, max_pct, network);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        Check();
        return vAddr;
    }

    //! Mark an entry as currently-connected-to.
    void Connected(const CService& addr, int64_t nTime = GetAdjustedTime())
    {
<<<<<<< HEAD
        {
            LOCK(cs);
            Check();
            Connected_(addr, nTime);
            Check();
        }
    }
=======
        LOCK(cs);
        Check();
        Connected_(addr, nTime);
        Check();
    }

    void SetServices(const CService& addr, ServiceFlags nServices)
    {
        LOCK(cs);
        Check();
        SetServices_(addr, nServices);
        Check();
    }

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};

#endif // BITCOIN_ADDRMAN_H
