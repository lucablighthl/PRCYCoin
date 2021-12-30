<<<<<<< HEAD
// Copyright (c) 2012-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "util.h"
=======
// Copyright (c) 2012-2016 The Bitcoin Core developers
// Copyright (c) 2017-2019 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "util/system.h"
#include "test/test_pivx.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

<<<<<<< HEAD
#ifdef DISABLE_PASSED_TEST
BOOST_AUTO_TEST_SUITE(getarg_tests)
=======
BOOST_FIXTURE_TEST_SUITE(getarg_tests, BasicTestingSetup)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

static void ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    if (strArg.size())
      boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    for (std::string& s : vecArg)
        vecChar.push_back(s.c_str());

<<<<<<< HEAD
    ParseParameters(vecChar.size(), &vecChar[0]);
=======
    gArgs.ParseParameters(vecChar.size(), vecChar.data());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-foo");
<<<<<<< HEAD
    BOOST_CHECK(GetBoolArg("-foo", false));
    BOOST_CHECK(GetBoolArg("-foo", true));

    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-fooo", false));
    BOOST_CHECK(GetBoolArg("-fooo", true));

    ResetArgs("-foo=0");
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));

    ResetArgs("-foo=1");
    BOOST_CHECK(GetBoolArg("-foo", false));
    BOOST_CHECK(GetBoolArg("-foo", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nofoo");
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));

    ResetArgs("-nofoo=1");
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));

    ResetArgs("-foo -nofoo");  // -nofoo should win
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));

    ResetArgs("-foo=1 -nofoo=1");  // -nofoo should win
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));

    ResetArgs("-foo=0 -nofoo=0");  // -nofoo=0 should win
    BOOST_CHECK(GetBoolArg("-foo", false));
    BOOST_CHECK(GetBoolArg("-foo", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--foo=1");
    BOOST_CHECK(GetBoolArg("-foo", false));
    BOOST_CHECK(GetBoolArg("-foo", true));

    ResetArgs("--nofoo=1");
    BOOST_CHECK(!GetBoolArg("-foo", false));
    BOOST_CHECK(!GetBoolArg("-foo", true));
=======
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));

    BOOST_CHECK(!gArgs.GetBoolArg("-fo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-fo", true));

    BOOST_CHECK(!gArgs.GetBoolArg("-fooo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-fooo", true));

    ResetArgs("-foo=0");
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));

    ResetArgs("-foo=1");
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-nofoo");
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));

    ResetArgs("-nofoo=1");
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));

    ResetArgs("-foo -nofoo");  // -nofoo should win
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));

    ResetArgs("-foo=1 -nofoo=1");  // -nofoo should win
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));

    ResetArgs("-foo=0 -nofoo=0");  // -nofoo=0 should win
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--foo=1");
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));

    ResetArgs("--nofoo=1");
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
<<<<<<< HEAD
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-foo", "eleven"), "eleven");

    ResetArgs("-foo -bar");
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-foo", "eleven"), "");

    ResetArgs("-foo=");
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-foo", "eleven"), "");

    ResetArgs("-foo=11");
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-foo", "eleven"), "11");

    ResetArgs("-foo=eleven");
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-foo", "eleven"), "eleven");
=======
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", "eleven"), "eleven");

    ResetArgs("-foo -bar");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", "eleven"), "");

    ResetArgs("-foo=");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", "eleven"), "");

    ResetArgs("-foo=11");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "11");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", "eleven"), "11");

    ResetArgs("-foo=eleven");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "eleven");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", "eleven"), "eleven");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
<<<<<<< HEAD
    BOOST_CHECK_EQUAL(GetArg("-foo", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-foo", 0), 0);

    ResetArgs("-foo -bar");
    BOOST_CHECK_EQUAL(GetArg("-foo", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-foo=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-foo", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-foo=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-foo", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
=======
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", 11), 11);
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", 0), 0);

    ResetArgs("-foo -bar");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", 11), 0);
    BOOST_CHECK_EQUAL(gArgs.GetArg("-bar", 11), 0);

    ResetArgs("-foo=11 -bar=12");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", 0), 11);
    BOOST_CHECK_EQUAL(gArgs.GetArg("-bar", 11), 12);

    ResetArgs("-foo=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", 1), 0);
    BOOST_CHECK_EQUAL(gArgs.GetArg("-bar", 11), 0);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--foo");
<<<<<<< HEAD
    BOOST_CHECK_EQUAL(GetBoolArg("-foo", false), true);

    ResetArgs("--foo=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-foo", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
=======
    BOOST_CHECK_EQUAL(gArgs.GetBoolArg("-foo", false), true);

    ResetArgs("--foo=verbose --bar=1");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-foo", ""), "verbose");
    BOOST_CHECK_EQUAL(gArgs.GetArg("-bar", 0), 1);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-nofoo");
<<<<<<< HEAD
    BOOST_CHECK(!GetBoolArg("-foo", true));
    BOOST_CHECK(!GetBoolArg("-foo", false));

    ResetArgs("-nofoo=1");
    BOOST_CHECK(!GetBoolArg("-foo", true));
    BOOST_CHECK(!GetBoolArg("-foo", false));

    ResetArgs("-nofoo=0");
    BOOST_CHECK(GetBoolArg("-foo", true));
    BOOST_CHECK(GetBoolArg("-foo", false));

    ResetArgs("-foo --nofoo"); // --nofoo should win
    BOOST_CHECK(!GetBoolArg("-foo", true));
    BOOST_CHECK(!GetBoolArg("-foo", false));

    ResetArgs("-nofoo -foo"); // foo always wins:
    BOOST_CHECK(GetBoolArg("-foo", true));
    BOOST_CHECK(GetBoolArg("-foo", false));
}

BOOST_AUTO_TEST_SUITE_END()
#endif
=======
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));

    ResetArgs("-nofoo=1");
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));

    ResetArgs("-nofoo=0");
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));

    ResetArgs("-foo --nofoo"); // --nofoo should win
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", true));
    BOOST_CHECK(!gArgs.GetBoolArg("-foo", false));

    ResetArgs("-nofoo -foo"); // foo always wins:
    BOOST_CHECK(gArgs.GetBoolArg("-foo", true));
    BOOST_CHECK(gArgs.GetBoolArg("-foo", false));
}

BOOST_AUTO_TEST_SUITE_END()
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
