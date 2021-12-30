// Copyright 2014 BitPay Inc.
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iomanip>
#include <sstream>
#include <stdio.h>
#include "univalue.h"
#include "univalue_escapes.h"

<<<<<<< HEAD
using namespace std;

static string json_escape(const string& inS)
{
    string outS;
=======
static std::string json_escape(const std::string& inS)
{
    std::string outS;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    outS.reserve(inS.size() * 2);

    for (unsigned int i = 0; i < inS.size(); i++) {
        unsigned char ch = inS[i];
        const char *escStr = escapes[ch];

        if (escStr)
            outS += escStr;
        else
            outS += ch;
    }

    return outS;
}

<<<<<<< HEAD
string UniValue::write(unsigned int prettyIndent,
                       unsigned int indentLevel) const
{
    string s;
=======
std::string UniValue::write(unsigned int prettyIndent,
                            unsigned int indentLevel) const
{
    std::string s;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    s.reserve(1024);

    unsigned int modIndent = indentLevel;
    if (modIndent == 0)
        modIndent = 1;

    switch (typ) {
    case VNULL:
        s += "null";
        break;
    case VOBJ:
        writeObject(prettyIndent, modIndent, s);
        break;
    case VARR:
        writeArray(prettyIndent, modIndent, s);
        break;
    case VSTR:
        s += "\"" + json_escape(val) + "\"";
        break;
    case VNUM:
        s += val;
        break;
    case VBOOL:
        s += (val == "1" ? "true" : "false");
        break;
    }

    return s;
}

<<<<<<< HEAD
static void indentStr(unsigned int prettyIndent, unsigned int indentLevel, string& s)
=======
static void indentStr(unsigned int prettyIndent, unsigned int indentLevel, std::string& s)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    s.append(prettyIndent * indentLevel, ' ');
}

<<<<<<< HEAD
void UniValue::writeArray(unsigned int prettyIndent, unsigned int indentLevel, string& s) const
=======
void UniValue::writeArray(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    s += "[";
    if (prettyIndent)
        s += "\n";

    for (unsigned int i = 0; i < values.size(); i++) {
        if (prettyIndent)
            indentStr(prettyIndent, indentLevel, s);
        s += values[i].write(prettyIndent, indentLevel + 1);
        if (i != (values.size() - 1)) {
            s += ",";
        }
        if (prettyIndent)
            s += "\n";
    }

    if (prettyIndent)
        indentStr(prettyIndent, indentLevel - 1, s);
    s += "]";
}

<<<<<<< HEAD
void UniValue::writeObject(unsigned int prettyIndent, unsigned int indentLevel, string& s) const
=======
void UniValue::writeObject(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    s += "{";
    if (prettyIndent)
        s += "\n";

    for (unsigned int i = 0; i < keys.size(); i++) {
        if (prettyIndent)
            indentStr(prettyIndent, indentLevel, s);
        s += "\"" + json_escape(keys[i]) + "\":";
        if (prettyIndent)
            s += " ";
        s += values.at(i).write(prettyIndent, indentLevel + 1);
        if (i != (values.size() - 1))
            s += ",";
        if (prettyIndent)
            s += "\n";
    }

    if (prettyIndent)
        indentStr(prettyIndent, indentLevel - 1, s);
    s += "}";
}

