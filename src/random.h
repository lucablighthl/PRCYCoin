// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
=======
// Copyright (c) 2009-2018 The Bitcoin developers
// Copyright (c) 2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_RANDOM_H
#define BITCOIN_RANDOM_H

#include "crypto/chacha20.h"
#include "crypto/common.h"
#include "uint256.h"

#include <stdint.h>
<<<<<<< HEAD

/* Seed OpenSSL PRNG with additional entropy data */
void RandAddSeed();

/**
 * Functions to gather random data via the OpenSSL PRNG
 */
void GetRandBytes(unsigned char* buf, int num);
uint64_t GetRand(uint64_t nMax);
int GetRandInt(int nMax);
uint256 GetRandHash();

/**
 * Add a little bit of randomness to the output of GetStrongRangBytes.
 * This sleeps for a millisecond, so should only be called when there is
 * no other work to be done.
 */
void RandAddSeedSleep();

/**
 * Function to gather random data from multiple sources, failing whenever any
 * of those source fail to provide a result.
 */
void GetStrongRandBytes(unsigned char* buf, int num);

/**
 * Fast randomness source. This is seeded once with secure random data, but
 * is completely deterministic and insecure after that.
=======
#include <limits>

/**
 * Overall design of the RNG and entropy sources.
 *
 * We maintain a single global 256-bit RNG state for all high-quality randomness.
 * The following (classes of) functions interact with that state by mixing in new
 * entropy, and optionally extracting random output from it:
 *
 * - The GetRand*() class of functions, as well as construction of FastRandomContext objects,
 *   perform 'fast' seeding, consisting of mixing in:
 *   - A stack pointer (indirectly committing to calling thread and call stack)
 *   - A high-precision timestamp (rdtsc when available, c++ high_resolution_clock otherwise)
 *   - 64 bits from the hardware RNG (rdrand) when available.
 *   These entropy sources are very fast, and only designed to protect against situations
 *   where a VM state restore/copy results in multiple systems with the same randomness.
 *   FastRandomContext on the other hand does not protect against this once created, but
 *   is even faster (and acceptable to use inside tight loops).
 *
 * - The GetStrongRand*() class of function perform 'slow' seeding, including everything
 *   that fast seeding includes, but additionally:
 *   - OS entropy (/dev/urandom, getrandom(), ...). The application will terminate if
 *     this entropy source fails.
 *   - Another high-precision timestamp (indirectly committing to a benchmark of all the
 *     previous sources).
 *   These entropy sources are slower, but designed to make sure the RNG state contains
 *   fresh data that is unpredictable to attackers.
 *
 * - RandAddPeriodic() seeds everything that fast seeding includes, but additionally:
 *   - A high-precision timestamp
 *   - Dynamic environment data (performance monitoring, ...)
 *   - Strengthen the entropy for 10 ms using repeated SHA512.
 *   This is run once every minute.
 *
 * On first use of the RNG (regardless of what function is called first), all entropy
 * sources used in the 'slow' seeder are included, but also:
 * - 256 bits from the hardware RNG (rdseed or rdrand) when available.
 * - Dynamic environment data (performance monitoring, ...)
 * - Static environment data
 * - Strengthen the entropy for 100 ms using repeated SHA512.
 *
 * When mixing in new entropy, H = SHA512(entropy || old_rng_state) is computed, and
 * (up to) the first 32 bytes of H are produced as output, while the last 32 bytes
 * become the new RNG state.
*/

/**
 * Generate random data via the internal PRNG.
 *
 * These functions are designed to be fast (sub microsecond), but do not necessarily
 * meaningfully add entropy to the PRNG state.
 *
 * Thread-safe.
 */
void GetRandBytes(unsigned char* buf, int num) noexcept;
uint64_t GetRand(uint64_t nMax) noexcept;
int GetRandInt(int nMax) noexcept;
uint256 GetRandHash() noexcept;

/**
 * Gather entropy from various sources, feed it into the internal PRNG, and
 * generate random data using it.
 *
 * This function will cause failure whenever the OS RNG fails.
 *
 * Thread-safe.
 */
void GetStrongRandBytes(unsigned char* buf, int num) noexcept;

/**
 * Gather entropy from various expensive sources, and feed them to the PRNG state.
 *
 * Thread-safe.
 */
void RandAddPeriodic() noexcept;

/**
 * Gathers entropy from the low bits of the time at which events occur. Should
 * be called with a uint32_t describing the event at the time an event occurs.
 *
 * Thread-safe.
 */
void RandAddEvent(const uint32_t event_info) noexcept;

/**
 * Fast randomness source. This is seeded once with secure random data, but
 * is completely deterministic and does not gather more entropy after that.
 *
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 * This class is not thread-safe.
 */
class FastRandomContext {
private:
<<<<<<< HEAD
    bool requires_seed;
    ChaCha20 rng;

    unsigned char bytebuf[64];
    int bytebuf_size;

    uint64_t bitbuf;
    int bitbuf_size;
=======
    bool requires_seed{false};
    ChaCha20 rng;

    unsigned char bytebuf[64];
    int bytebuf_size{0};

    uint64_t bitbuf{0};
    int bitbuf_size{0};
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    void RandomSeed();

    void FillByteBuffer()
    {
        if (requires_seed) {
            RandomSeed();
        }
<<<<<<< HEAD
        rng.Output(bytebuf, sizeof(bytebuf));
=======
        rng.Keystream(bytebuf, sizeof(bytebuf));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        bytebuf_size = sizeof(bytebuf);
    }

    void FillBitBuffer()
    {
        bitbuf = rand64();
        bitbuf_size = 64;
    }

public:
<<<<<<< HEAD
    explicit FastRandomContext(bool fDeterministic = false);

    /** Initialize with explicit seed (only for testing) */
    explicit FastRandomContext(const uint256& seed);
=======
    explicit FastRandomContext(bool fDeterministic = false) noexcept;

    /** Initialize with explicit seed (only for testing) */
    explicit FastRandomContext(const uint256& seed) noexcept;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // Do not permit copying a FastRandomContext (move it, or create a new one to get reseeded).
    FastRandomContext(const FastRandomContext&) = delete;
    FastRandomContext(FastRandomContext&&) = delete;
    FastRandomContext& operator=(const FastRandomContext&) = delete;

    /** Move a FastRandomContext. If the original one is used again, it will be reseeded. */
    FastRandomContext& operator=(FastRandomContext&& from) noexcept;

    /** Generate a random 64-bit integer. */
<<<<<<< HEAD
    uint64_t rand64()
=======
    uint64_t rand64() noexcept
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        if (bytebuf_size < 8) FillByteBuffer();
        uint64_t ret = ReadLE64(bytebuf + 64 - bytebuf_size);
        bytebuf_size -= 8;
        return ret;
    }

    /** Generate a random (bits)-bit integer. */
<<<<<<< HEAD
    uint64_t randbits(int bits) {
=======
    uint64_t randbits(int bits) noexcept {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (bits == 0) {
            return 0;
        } else if (bits > 32) {
            return rand64() >> (64 - bits);
        } else {
            if (bitbuf_size < bits) FillBitBuffer();
            uint64_t ret = bitbuf & (~(uint64_t)0 >> (64 - bits));
            bitbuf >>= bits;
            bitbuf_size -= bits;
            return ret;
        }
    }

    /** Generate a random integer in the range [0..range). */
<<<<<<< HEAD
    uint64_t randrange(uint64_t range)
=======
    uint64_t randrange(uint64_t range) noexcept
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    {
        --range;
        int bits = CountBits(range);
        while (true) {
            uint64_t ret = randbits(bits);
            if (ret <= range) return ret;
        }
    }

    /** Generate random bytes. */
    std::vector<unsigned char> randbytes(size_t len);

<<<<<<< HEAD
    /** Generate a random 32-bit integer. */
    uint32_t rand32() { return randbits(32); }

    /** generate a random uint256. */
    uint256 rand256();

    /** Generate a random boolean. */
    bool randbool() { return randbits(1); }
};

=======
    /** Generate a random 16-bit integer. */
    uint16_t rand16() { return randbits(16); }

    /** Generate a random 32-bit integer. */
    uint32_t rand32() noexcept { return randbits(32); }

    /** generate a random uint256. */
    uint256 rand256() noexcept;

    /** Generate a random boolean. */
    bool randbool() noexcept { return randbits(1); }

    // Compatibility with the C++11 UniformRandomBitGenerator concept
    typedef uint64_t result_type;
    static constexpr uint64_t min() { return 0; }
    static constexpr uint64_t max() { return std::numeric_limits<uint64_t>::max(); }
    inline uint64_t operator()() noexcept { return rand64(); }
};

/** More efficient than using std::shuffle on a FastRandomContext.
 *
 * This is more efficient as std::shuffle will consume entropy in groups of
 * 64 bits at the time and throw away most.
 *
 * This also works around a bug in libstdc++ std::shuffle that may cause
 * type::operator=(type&&) to be invoked on itself, which the library's
 * debug mode detects and panics on. This is a known issue, see
 * https://stackoverflow.com/questions/22915325/avoiding-self-assignment-in-stdshuffle
 */
template<typename I, typename R>
void Shuffle(I first, I last, R&& rng)
{
    while (first != last) {
        size_t j = rng.randrange(last - first);
        if (j) {
            using std::swap;
            swap(*first, *(first + j));
        }
        ++first;
    }
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
/* Number of random bytes returned by GetOSRand.
 * When changing this constant make sure to change all call sites, and make
 * sure that the underlying OS APIs for all platforms support the number.
 * (many cap out at 256 bytes).
 */
static const ssize_t NUM_OS_RANDOM_BYTES = 32;

/** Get 32 bytes of system entropy. Do not use this in application code: use
 * GetStrongRandBytes instead.
 */
void GetOSRand(unsigned char *ent32);

/** Check that OS randomness is available and returning the requested number
 * of bytes.
 */
bool Random_SanityCheck();

<<<<<<< HEAD
/** Initialize the RNG. */
=======
/**
 * Initialize global RNG state and log any CPU features that are used.
 *
 * Calling this function is optional. RNG state will be initialized when first
 * needed if it is not called.
 */
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
void RandomInit();

#endif // BITCOIN_RANDOM_H
