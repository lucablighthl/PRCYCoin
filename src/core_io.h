// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2019 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CORE_IO_H
#define BITCOIN_CORE_IO_H

#include <string>
#include <vector>

class CBlock;
class CScript;
class CTransaction;
<<<<<<< HEAD
class uint256;
class UniValue;
class PoSBlockSummary;

// core_read.cpp
extern CScript ParseScript(std::string s);
extern bool DecodeHexTx(CTransaction& tx, const std::string& strHexTx);
extern bool DecodeHexBlk(CBlock&, const std::string& strHexBlk);
extern uint256 ParseHashUV(const UniValue& v, const std::string& strName);
extern uint256 ParseHashStr(const std::string&, const std::string& strName);
=======
struct CMutableTransaction;
class uint256;
class UniValue;

// core_read.cpp
extern CScript ParseScript(std::string s);
extern std::string ScriptToAsmStr(const CScript& script, const bool fAttemptSighashDecode = false);
extern bool DecodeHexTx(CMutableTransaction& tx, const std::string& strHexTx);
extern bool DecodeHexBlk(CBlock&, const std::string& strHexBlk);
extern uint256 ParseHashUV(const UniValue& v, const std::string& strName);
extern uint256 ParseHashStr(const std::string&, const std::string& strName);
extern bool ParseHashStr(const std::string& strReq, uint256& v);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
extern std::vector<unsigned char> ParseHexUV(const UniValue& v, const std::string& strName);

// core_write.cpp
extern std::string FormatScript(const CScript& script);
extern std::string EncodeHexTx(const CTransaction& tx);
<<<<<<< HEAD
extern void ScriptPubKeyToUniv(const CScript& scriptPubKey,
    UniValue& out,
    bool fIncludeHex);
extern void TxToUniv(const CTransaction& tx, const uint256& hashBlock, UniValue& entry);

extern std::string EncodeHexPoSBlockSummary(const PoSBlockSummary& pos);

=======
extern void ScriptPubKeyToUniv(const CScript& scriptPubKey, UniValue& out, bool fIncludeHex);
extern void TxToUniv(const CTransaction& tx, const uint256& hashBlock, UniValue& entry);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#endif // BITCOIN_CORE_IO_H
