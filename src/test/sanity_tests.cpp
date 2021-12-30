// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "compat/sanity.h"
#include "key.h"
<<<<<<< HEAD

#include <boost/test/unit_test.hpp>
#ifdef DISABLE_PASSED_TEST
BOOST_AUTO_TEST_SUITE(sanity_tests)
=======
#include "test_pivx.h"

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(sanity_tests, TestingSetup)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

BOOST_AUTO_TEST_CASE(basic_sanity)
{
  BOOST_CHECK_MESSAGE(glibc_sanity_test() == true, "libc sanity test");
  BOOST_CHECK_MESSAGE(glibcxx_sanity_test() == true, "stdlib sanity test");
<<<<<<< HEAD
  BOOST_CHECK_MESSAGE(ECC_InitSanityCheck() == true, "openssl ECC test");
}

BOOST_AUTO_TEST_SUITE_END()
#endif
=======
  BOOST_CHECK_MESSAGE(ECC_InitSanityCheck() == true, "secp256k1 sanity test");
}

BOOST_AUTO_TEST_SUITE_END()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
