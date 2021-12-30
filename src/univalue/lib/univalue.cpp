// Copyright 2014 BitPay Inc.
// Copyright 2015 Bitcoin Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <stdint.h>
<<<<<<< HEAD
#include <errno.h>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include "univalue.h"

namespace 
{
static bool ParsePrechecks(const std::string& str)
{
    if (str.empty()) // No empty string allowed
        return false;
    if (str.size() >= 1 && (json_isspace(str[0]) || json_isspace(str[str.size()-1]))) // No padding allowed
        return false;
    if (str.size() != strlen(str.c_str())) // No embedded NUL characters allowed
        return false;
    return true;
}

bool ParseInt32(const std::string& str, int32_t *out)
{
    if (!ParsePrechecks(str))
        return false;
    char *endp = NULL;
    errno = 0; // strtol will not set errno if valid
    long int n = strtol(str.c_str(), &endp, 10);
    if(out) *out = (int32_t)n;
    // Note that strtol returns a *long int*, so even if strtol doesn't report a over/underflow
    // we still have to check that the returned value is within the range of an *int32_t*. On 64-bit
    // platforms the size of these types may be different.
    return endp && *endp == 0 && !errno &&
        n >= std::numeric_limits<int32_t>::min() &&
        n <= std::numeric_limits<int32_t>::max();
}

bool ParseInt64(const std::string& str, int64_t *out)
{
    if (!ParsePrechecks(str))
        return false;
    char *endp = NULL;
    errno = 0; // strtoll will not set errno if valid
    long long int n = strtoll(str.c_str(), &endp, 10);
    if(out) *out = (int64_t)n;
    // Note that strtoll returns a *long long int*, so even if strtol doesn't report a over/underflow
    // we still have to check that the returned value is within the range of an *int64_t*.
    return endp && *endp == 0 && !errno &&
        n >= std::numeric_limits<int64_t>::min() &&
        n <= std::numeric_limits<int64_t>::max();
}

bool ParseDouble(const std::string& str, double *out)
{
    if (!ParsePrechecks(str))
        return false;
    if (str.size() >= 2 && str[0] == '0' && str[1] == 'x') // No hexadecimal floats allowed
        return false;
    std::istringstream text(str);
    text.imbue(std::locale::classic());
    double result;
    text >> result;
    if(out) *out = result;
    return text.eof() && !text.fail();
}
}

using namespace std;

=======
#include <iomanip>
#include <sstream>
#include <stdlib.h>

#include "univalue.h"

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
const UniValue NullUniValue;

void UniValue::clear()
{
    typ = VNULL;
    val.clear();
    keys.clear();
    values.clear();
}

bool UniValue::setNull()
{
    clear();
    return true;
}

bool UniValue::setBool(bool val_)
{
    clear();
    typ = VBOOL;
    if (val_)
        val = "1";
    return true;
}

<<<<<<< HEAD
static bool validNumStr(const string& s)
{
    string tokenVal;
=======
static bool validNumStr(const std::string& s)
{
    std::string tokenVal;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    unsigned int consumed;
    enum jtokentype tt = getJsonToken(tokenVal, consumed, s.data(), s.data() + s.size());
    return (tt == JTOK_NUMBER);
}

<<<<<<< HEAD
bool UniValue::setNumStr(const string& val_)
=======
bool UniValue::setNumStr(const std::string& val_)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    if (!validNumStr(val_))
        return false;

    clear();
    typ = VNUM;
    val = val_;
    return true;
}

bool UniValue::setInt(uint64_t val_)
{
<<<<<<< HEAD
    ostringstream oss;
=======
    std::ostringstream oss;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    oss << val_;

    return setNumStr(oss.str());
}

bool UniValue::setInt(int64_t val_)
{
<<<<<<< HEAD
    ostringstream oss;
=======
    std::ostringstream oss;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    oss << val_;

    return setNumStr(oss.str());
}

bool UniValue::setFloat(double val_)
{
<<<<<<< HEAD
    ostringstream oss;
=======
    std::ostringstream oss;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    oss << std::setprecision(16) << val_;

    bool ret = setNumStr(oss.str());
    typ = VNUM;
    return ret;
}

<<<<<<< HEAD
bool UniValue::setStr(const string& val_)
=======
bool UniValue::setStr(const std::string& val_)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    clear();
    typ = VSTR;
    val = val_;
    return true;
}

bool UniValue::setArray()
{
    clear();
    typ = VARR;
    return true;
}

bool UniValue::setObject()
{
    clear();
    typ = VOBJ;
    return true;
}

bool UniValue::push_back(const UniValue& val_)
{
    if (typ != VARR)
        return false;

    values.push_back(val_);
    return true;
}

bool UniValue::push_backV(const std::vector<UniValue>& vec)
{
    if (typ != VARR)
        return false;

    values.insert(values.end(), vec.begin(), vec.end());

    return true;
}

<<<<<<< HEAD
=======
void UniValue::__pushKV(const std::string& key, const UniValue& val_)
{
    keys.push_back(key);
    values.push_back(val_);
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
bool UniValue::pushKV(const std::string& key, const UniValue& val_)
{
    if (typ != VOBJ)
        return false;

<<<<<<< HEAD
    keys.push_back(key);
    values.push_back(val_);
=======
    size_t idx;
    if (findKey(key, idx))
        values[idx] = val_;
    else
        __pushKV(key, val_);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    return true;
}

bool UniValue::pushKVs(const UniValue& obj)
{
    if (typ != VOBJ || obj.typ != VOBJ)
        return false;

<<<<<<< HEAD
    for (unsigned int i = 0; i < obj.keys.size(); i++) {
        keys.push_back(obj.keys[i]);
        values.push_back(obj.values.at(i));
    }
=======
    for (size_t i = 0; i < obj.keys.size(); i++)
        __pushKV(obj.keys[i], obj.values.at(i));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    return true;
}

<<<<<<< HEAD
=======
void UniValue::getObjMap(std::map<std::string,UniValue>& kv) const
{
    if (typ != VOBJ)
        return;

    kv.clear();
    for (size_t i = 0; i < keys.size(); i++)
        kv[keys[i]] = values[i];
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
bool UniValue::findKey(const std::string& key, size_t& retIdx) const
{
    for (size_t i = 0; i < keys.size(); i++) {
        if (keys[i] == key) {
            retIdx = i;
            return true;
        }
    }

    return false;
}

<<<<<<< HEAD
bool UniValue::checkObject(const std::map<std::string,UniValue::VType>& t)
{
=======
bool UniValue::checkObject(const std::map<std::string,UniValue::VType>& t) const
{
    if (typ != VOBJ)
        return false;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    for (std::map<std::string,UniValue::VType>::const_iterator it = t.begin();
         it != t.end(); ++it) {
        size_t idx = 0;
        if (!findKey(it->first, idx))
            return false;

        if (values.at(idx).getType() != it->second)
            return false;
    }

    return true;
}

const UniValue& UniValue::operator[](const std::string& key) const
{
    if (typ != VOBJ)
        return NullUniValue;

    size_t index = 0;
    if (!findKey(key, index))
        return NullUniValue;

    return values.at(index);
}

const UniValue& UniValue::operator[](size_t index) const
{
    if (typ != VOBJ && typ != VARR)
        return NullUniValue;
    if (index >= values.size())
        return NullUniValue;

    return values.at(index);
}

const char *uvTypeName(UniValue::VType t)
{
    switch (t) {
    case UniValue::VNULL: return "null";
    case UniValue::VBOOL: return "bool";
    case UniValue::VOBJ: return "object";
    case UniValue::VARR: return "array";
    case UniValue::VSTR: return "string";
    case UniValue::VNUM: return "number";
    }

    // not reached
    return NULL;
}

const UniValue& find_value(const UniValue& obj, const std::string& name)
{
    for (unsigned int i = 0; i < obj.keys.size(); i++)
        if (obj.keys[i] == name)
            return obj.values.at(i);

    return NullUniValue;
}

<<<<<<< HEAD
const std::vector<std::string>& UniValue::getKeys() const
{
    if (typ != VOBJ)
        throw std::runtime_error("JSON value is not an object as expected");
    return keys;
}

const std::vector<UniValue>& UniValue::getValues() const
{
    if (typ != VOBJ && typ != VARR)
        throw std::runtime_error("JSON value is not an object or array as expected");
    return values;
}

bool UniValue::get_bool() const
{
    if (typ != VBOOL)
        throw std::runtime_error("JSON value is not a boolean as expected");
    return getBool();
}

const std::string& UniValue::get_str() const
{
    if (typ != VSTR)
        throw std::runtime_error("JSON value is not a string as expected");
    return getValStr();
}

int UniValue::get_int() const
{
    if (typ != VNUM)
        throw std::runtime_error("JSON value is not an integer as expected");
    int32_t retval;
    if (!ParseInt32(getValStr(), &retval))
        throw std::runtime_error("JSON integer out of range");
    return retval;
}

int64_t UniValue::get_int64() const
{
    if (typ != VNUM)
        throw std::runtime_error("JSON value is not an integer as expected");
    int64_t retval;
    if (!ParseInt64(getValStr(), &retval))
        throw std::runtime_error("JSON integer out of range");
    return retval;
}

double UniValue::get_real() const
{
    if (typ != VNUM)
        throw std::runtime_error("JSON value is not a number as expected");
    double retval;
    if (!ParseDouble(getValStr(), &retval))
        throw std::runtime_error("JSON double out of range");
    return retval;
}

const UniValue& UniValue::get_obj() const
{
    if (typ != VOBJ)
        throw std::runtime_error("JSON value is not an object as expected");
    return *this;
}

const UniValue& UniValue::get_array() const
{
    if (typ != VARR)
        throw std::runtime_error("JSON value is not an array as expected");
    return *this;
}

=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
