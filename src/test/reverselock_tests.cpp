// Copyright (c) 2015 The Bitcoin Core developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "reverselock.h"
#include <boost/bind.hpp>
=======
// Copyright (c) 2017-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "sync.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>
#include <boost/test/unit_test.hpp>

<<<<<<< HEAD
#ifdef DISABLE_PASSED_TEST
BOOST_AUTO_TEST_SUITE(reverselock_tests)
BOOST_AUTO_TEST_CASE(reverselock_basics)
        {
                boost::mutex mutex;
                boost::unique_lock<boost::mutex> lock(mutex);
                BOOST_CHECK(lock.owns_lock());
                {
                    reverse_lock <boost::unique_lock<boost::mutex>> rlock(lock);
                    BOOST_CHECK(!lock.owns_lock());
                }
                BOOST_CHECK(lock.owns_lock());
        }
BOOST_AUTO_TEST_CASE(reverselock_errors)
        {
                boost::mutex mutex;
                boost::unique_lock<boost::mutex> lock(mutex);
                // Make sure trying to reverse lock an unlocked lock fails
                lock.unlock();
                BOOST_CHECK(!lock.owns_lock());
                bool failed = false;
                try {
            reverse_lock <boost::unique_lock<boost::mutex>> rlock(lock);
        } catch(...) {
            failed = true;
        }
                BOOST_CHECK(failed);
                BOOST_CHECK(!lock.owns_lock());
                // Locking the original lock after it has been taken by a reverse lock
                // makes no sense. Ensure that the original lock no longer owns the lock
                // after giving it to a reverse one.
                lock.lock();
                BOOST_CHECK(lock.owns_lock());
                {
                    reverse_lock <boost::unique_lock<boost::mutex>> rlock(lock);
                    BOOST_CHECK(!lock.owns_lock());
                }
                BOOST_CHECK(failed);
                BOOST_CHECK(lock.owns_lock());
        }

BOOST_AUTO_TEST_SUITE_END()
#endif
=======
BOOST_AUTO_TEST_SUITE(reverselock_tests)

BOOST_AUTO_TEST_CASE(reverselock_basics)
{
    Mutex mutex;
    WAIT_LOCK(mutex, lock);

    BOOST_CHECK(lock.owns_lock());
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
    }
    BOOST_CHECK(lock.owns_lock());
}

BOOST_AUTO_TEST_CASE(reverselock_multiple)
{
    Mutex mutex2;
    Mutex mutex;
    WAIT_LOCK(mutex2, lock2);
    WAIT_LOCK(mutex, lock);

    // Make sure undoing two locks succeeds
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
        REVERSE_LOCK(lock2);
        BOOST_CHECK(!lock2.owns_lock());
    }
    BOOST_CHECK(lock.owns_lock());
    BOOST_CHECK(lock2.owns_lock());
}

BOOST_AUTO_TEST_CASE(reverselock_errors)
{
    Mutex mutex2;
    Mutex mutex;
    WAIT_LOCK(mutex2, lock2);
    WAIT_LOCK(mutex, lock);

#ifdef DEBUG_LOCKORDER
    // Make sure trying to reverse lock a previous lock fails
    try {
        REVERSE_LOCK(lock2);
        BOOST_CHECK(false); // REVERSE_LOCK(lock2) succeeded
    } catch(...) { }
    BOOST_CHECK(lock2.owns_lock());
#endif

    // Make sure trying to reverse lock an unlocked lock fails
    lock.unlock();

    BOOST_CHECK(!lock.owns_lock());

    bool failed = false;
    try {
        REVERSE_LOCK(lock);
    } catch(...) {
        failed = true;
    }

    BOOST_CHECK(failed);
    BOOST_CHECK(!lock.owns_lock());

    // Locking the original lock after it has been taken by a reverse lock
    // makes no sense. Ensure that the original lock no longer owns the lock
    // after giving it to a reverse one.

    lock.lock();
    BOOST_CHECK(lock.owns_lock());
    {
        REVERSE_LOCK(lock);
        BOOST_CHECK(!lock.owns_lock());
    }

    BOOST_CHECK(failed);
    BOOST_CHECK(lock.owns_lock());
}

BOOST_AUTO_TEST_SUITE_END()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
