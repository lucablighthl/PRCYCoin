// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "core_io.h"

#include "primitives/block.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "serialize.h"
#include "streams.h"
<<<<<<< HEAD
#include <univalue.h>
#include "util.h"
=======
#include "util/system.h"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include "utilstrencodings.h"
#include "version.h"

#include <boost/algorithm/string/classification.hpp>
<<<<<<< HEAD
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/assign/list_of.hpp>

using namespace boost;
using namespace boost::algorithm;
using namespace std;
=======
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/split.hpp>

#include <univalue.h>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

CScript ParseScript(std::string s)
{
    CScript result;

<<<<<<< HEAD
    static map<string, opcodetype> mapOpNames;

    vector<string> words;
    split(words, s, is_any_of(" \t\n"), token_compress_on);
=======
    static std::map<std::string, opcodetype> mapOpNames;

    if (mapOpNames.empty()) {
        for (int op = 0; op <= OP_ZEROCOINSPEND; op++) {
            // Allow OP_RESERVED to get into mapOpNames
            if (op < OP_NOP && op != OP_RESERVED)
                continue;

            const char* name = GetOpName(static_cast<opcodetype>(op));
            if (strcmp(name, "OP_UNKNOWN") == 0)
                continue;
            std::string strName(name);
            mapOpNames[strName] = static_cast<opcodetype>(op);
            // Convenience: OP_ADD and just ADD are both recognized:
            boost::algorithm::replace_first(strName, "OP_", "");
            mapOpNames[strName] = static_cast<opcodetype>(op);
        }
    }

    std::vector<std::string> words;
    boost::algorithm::split(words, s, boost::algorithm::is_any_of(" \t\n"), boost::algorithm::token_compress_on);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    for (std::vector<std::string>::const_iterator w = words.begin(); w != words.end(); ++w) {
        if (w->empty()) {
            // Empty string, ignore. (boost::split given '' will return one word)
<<<<<<< HEAD
        } else if (all(*w, is_digit()) ||
                   (starts_with(*w, "-") && all(string(w->begin() + 1, w->end()), is_digit()))) {
            // Number
            int64_t n = atoi64(*w);
            result << n;
        } else if (starts_with(*w, "0x") && (w->begin() + 2 != w->end()) && IsHex(string(w->begin() + 2, w->end()))) {
            // Raw hex data, inserted NOT pushed onto stack:
            std::vector<unsigned char> raw = ParseHex(string(w->begin() + 2, w->end()));
            result.insert(result.end(), raw.begin(), raw.end());
        } else if (w->size() >= 2 && starts_with(*w, "'") && ends_with(*w, "'")) {
=======
        } else if (std::all_of(w->begin(), w->end(), ::IsDigit) ||
                   (w->front() == '-' && w->size() > 1 && std::all_of(w->begin()+1, w->end(), ::IsDigit))) {
            // Number
            int64_t n = atoi64(*w);
            result << n;
        } else if (w->substr(0,2) == "0x" && w->size() > 2 && IsHex(std::string(w->begin()+2, w->end()))) {
            // Raw hex data, inserted NOT pushed onto stack:
            std::vector<unsigned char> raw = ParseHex(std::string(w->begin() + 2, w->end()));
            result.insert(result.end(), raw.begin(), raw.end());
        } else if (w->size() >= 2 && w->front() == '\'' && w->back() == '\'') {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            // Single-quoted string, pushed as data. NOTE: this is poor-man's
            // parsing, spaces/tabs/newlines in single-quoted strings won't work.
            std::vector<unsigned char> value(w->begin() + 1, w->end() - 1);
            result << value;
        } else if (mapOpNames.count(*w)) {
            // opcode, e.g. OP_ADD or ADD:
            result << mapOpNames[*w];
        } else {
<<<<<<< HEAD
            throw runtime_error("script parse error");
=======
            throw std::runtime_error("script parse error");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }

    return result;
}

<<<<<<< HEAD
bool DecodeHexTx(CTransaction& tx, const std::string& strHexTx)
=======
bool DecodeHexTx(CMutableTransaction& tx, const std::string& strHexTx)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (!IsHex(strHexTx))
        return false;

<<<<<<< HEAD
    vector<unsigned char> txData(ParseHex(strHexTx));
=======
    std::vector<unsigned char> txData(ParseHex(strHexTx));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CDataStream ssData(txData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssData >> tx;
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

bool DecodeHexBlk(CBlock& block, const std::string& strHexBlk)
{
    if (!IsHex(strHexBlk))
        return false;

    std::vector<unsigned char> blockData(ParseHex(strHexBlk));
    CDataStream ssBlock(blockData, SER_NETWORK, PROTOCOL_VERSION);
    try {
        ssBlock >> block;
    } catch (const std::exception&) {
        return false;
    }

    return true;
}

<<<<<<< HEAD
uint256 ParseHashUV(const UniValue& v, const string& strName)
{
    string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    return ParseHashStr(strHex, strName); // Note: ParseHashStr("") throws a runtime_error
=======
uint256 ParseHashUV(const UniValue& v, const std::string& strName)
{
    std::string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    return ParseHashStr(strHex, strName); // Note: ParseHashStr("") throws a std::runtime_error
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

uint256 ParseHashStr(const std::string& strHex, const std::string& strName)
{
    if (!IsHex(strHex)) // Note: IsHex("") is false
<<<<<<< HEAD
        throw runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");
=======
        throw std::runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    uint256 result;
    result.SetHex(strHex);
    return result;
}

<<<<<<< HEAD
vector<unsigned char> ParseHexUV(const UniValue& v, const string& strName)
{
    string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    if (!IsHex(strHex))
        throw runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");
=======
bool ParseHashStr(const std::string& strReq, uint256& v)
{
    if (!IsHex(strReq) || (strReq.size() != 64))
        return false;

    v.SetHex(strReq);
    return true;
}

std::vector<unsigned char> ParseHexUV(const UniValue& v, const std::string& strName)
{
    std::string strHex;
    if (v.isStr())
        strHex = v.getValStr();
    if (!IsHex(strHex))
        throw std::runtime_error(strName + " must be hexadecimal string (not '" + strHex + "')");
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return ParseHex(strHex);
}
