// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Copyright (c) 2017-2020 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#include "script/standard.h"

#include "pubkey.h"
#include "script/script.h"
<<<<<<< HEAD
#include "util.h"
#include "utilstrencodings.h"

using namespace std;

typedef vector<unsigned char> valtype;

unsigned nMaxDatacarrierBytes = MAX_OP_RETURN_RELAY;

CScriptID::CScriptID(const CScript &in) : uint160(Hash160(in.begin(), in.end())) {}

const char *GetTxnOutputType(txnouttype t) {
    switch (t) {
        case TX_NONSTANDARD:
            return "nonstandard";
        case TX_PUBKEY:
            return "pubkey";
        case TX_PUBKEYHASH:
            return "pubkeyhash";
        case TX_SCRIPTHASH:
            return "scripthash";
        case TX_MULTISIG:
            return "multisig";
        case TX_NULL_DATA:
            return "nulldata";
=======

typedef std::vector<unsigned char> valtype;

unsigned nMaxDatacarrierBytes = MAX_OP_RETURN_RELAY;

CScriptID::CScriptID(const CScript& in) : uint160(Hash160(in.begin(), in.end())) {}

const char* GetTxnOutputType(txnouttype t)
{
    switch (t)
    {
    case TX_NONSTANDARD: return "nonstandard";
    case TX_PUBKEY: return "pubkey";
    case TX_PUBKEYHASH: return "pubkeyhash";
    case TX_SCRIPTHASH: return "scripthash";
    case TX_MULTISIG: return "multisig";
    case TX_COLDSTAKE: return "coldstake";
    case TX_NULL_DATA: return "nulldata";
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
    return NULL;
}

<<<<<<< HEAD
/**
 * Return public keys or hashes from scriptPubKey, for 'standard' transaction types.
 */
bool Solver(const CScript &scriptPubKey, txnouttype &typeRet, vector <vector<unsigned char>> &vSolutionsRet) {
    // Templates
    static multimap <txnouttype, CScript> mTemplates;
    if (mTemplates.empty()) {
        // Standard tx, sender provides pubkey, receiver adds signature
        mTemplates.insert(make_pair(TX_PUBKEY, CScript() << OP_PUBKEY << OP_CHECKSIG));

        // Bitcoin address tx, sender provides hash of pubkey, receiver provides signature and pubkey
        mTemplates.insert(make_pair(TX_PUBKEYHASH, CScript() << OP_DUP << OP_HASH160 << OP_PUBKEYHASH << OP_EQUALVERIFY
                                                             << OP_CHECKSIG));

        // Sender provides N pubkeys, receivers provides M signatures
        mTemplates.insert(make_pair(TX_MULTISIG,
                                    CScript() << OP_SMALLINTEGER << OP_PUBKEYS << OP_SMALLINTEGER << OP_CHECKMULTISIG));
    }

    // Shortcut for pay-to-script-hash, which are more constrained than the other types:
    // it is always OP_HASH160 20 [20 byte hash] OP_EQUAL
    if (scriptPubKey.IsPayToScriptHash()) {
        typeRet = TX_SCRIPTHASH;
        vector<unsigned char> hashBytes(scriptPubKey.begin() + 2, scriptPubKey.begin() + 22);
=======
static bool MatchPayToPubkey(const CScript& script, valtype& pubkey)
{
    if (script.size() == CPubKey::PUBLIC_KEY_SIZE + 2 && script[0] == CPubKey::PUBLIC_KEY_SIZE && script.back() == OP_CHECKSIG) {
        pubkey = valtype(script.begin() + 1, script.begin() + CPubKey::PUBLIC_KEY_SIZE + 1);
        return CPubKey::ValidSize(pubkey);
    }
    if (script.size() == CPubKey::COMPRESSED_PUBLIC_KEY_SIZE + 2 && script[0] == CPubKey::COMPRESSED_PUBLIC_KEY_SIZE && script.back() == OP_CHECKSIG) {
        pubkey = valtype(script.begin() + 1, script.begin() + CPubKey::COMPRESSED_PUBLIC_KEY_SIZE + 1);
        return CPubKey::ValidSize(pubkey);
    }
    return false;
}

static bool MatchPayToPubkeyHash(const CScript& script, valtype& pubkeyhash)
{
    if (script.size() == 25 && script[0] == OP_DUP && script[1] == OP_HASH160 && script[2] == 20 && script[23] == OP_EQUALVERIFY && script[24] == OP_CHECKSIG) {
        pubkeyhash = valtype(script.begin () + 3, script.begin() + 23);
        return true;
    }
    return false;
}

static bool MatchPayToColdStaking(const CScript& script, valtype& stakerPubKeyHash, valtype& ownerPubKeyHash)
{
    if (script.IsPayToColdStaking()) {
        stakerPubKeyHash = valtype(script.begin () + 6, script.begin() + 26);
        ownerPubKeyHash = valtype(script.begin () + 28, script.begin() + 48);
        return true;
    }
    return false;
}

/** Test for "small positive integer" script opcodes - OP_1 through OP_16. */
static constexpr bool IsSmallInteger(opcodetype opcode)
{
    return opcode >= OP_1 && opcode <= OP_16;
}

static bool MatchMultisig(const CScript& script, unsigned int& required, std::vector<valtype>& pubkeys)
{
    opcodetype opcode;
    valtype data;
    CScript::const_iterator it = script.begin();
    if (script.size() < 1 || script.back() != OP_CHECKMULTISIG) return false;

    if (!script.GetOp(it, opcode, data) || !IsSmallInteger(opcode)) return false;
    required = CScript::DecodeOP_N(opcode);
    while (script.GetOp(it, opcode, data) && CPubKey::ValidSize(data)) {
        pubkeys.emplace_back(std::move(data));
    }
    if (!IsSmallInteger(opcode)) return false;
    unsigned int keys = CScript::DecodeOP_N(opcode);
    if (pubkeys.size() != keys || keys < required) return false;
    return (it + 1 == script.end());
}

bool Solver(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<std::vector<unsigned char> >& vSolutionsRet)
{
    if (scriptPubKey.empty())
        return false;

    vSolutionsRet.clear();

    // Shortcut for pay-to-script-hash, which are more constrained than the other types:
    // it is always OP_HASH160 20 [20 byte hash] OP_EQUAL
    if (scriptPubKey.IsPayToScriptHash())
    {
        typeRet = TX_SCRIPTHASH;
        std::vector<unsigned char> hashBytes(scriptPubKey.begin()+2, scriptPubKey.begin()+22);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        vSolutionsRet.push_back(hashBytes);
        return true;
    }

    // Provably prunable, data-carrying output
    //
    // So long as script passes the IsUnspendable() test and all but the first
    // byte passes the IsPushOnly() test we don't care what exactly is in the
    // script.
<<<<<<< HEAD
    if (scriptPubKey.size() >= 1 && scriptPubKey[0] == OP_RETURN && scriptPubKey.IsPushOnly(scriptPubKey.begin() + 1)) {
=======
    if (scriptPubKey.size() >= 1 && scriptPubKey[0] == OP_RETURN && scriptPubKey.IsPushOnly(scriptPubKey.begin()+1)) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        typeRet = TX_NULL_DATA;
        return true;
    }

<<<<<<< HEAD
    // Scan templates
    const CScript &script1 = scriptPubKey;
    for (const PAIRTYPE(txnouttype, CScript)&tplate : mTemplates)
    {
        const CScript &script2 = tplate.second;
        vSolutionsRet.clear();

        opcodetype opcode1, opcode2;
        vector<unsigned char> vch1, vch2;

        // Compare
        CScript::const_iterator pc1 = script1.begin();
        CScript::const_iterator pc2 = script2.begin();
        while (true) {
            if (pc1 == script1.end() && pc2 == script2.end()) {
                // Found a match
                typeRet = tplate.first;
                if (typeRet == TX_MULTISIG) {
                    // Additional checks for TX_MULTISIG:
                    unsigned char m = vSolutionsRet.front()[0];
                    unsigned char n = vSolutionsRet.back()[0];
                    if (m < 1 || n < 1 || m > n || vSolutionsRet.size() - 2 != n)
                        return false;
                }
                return true;
            }
            if (!script1.GetOp(pc1, opcode1, vch1))
                break;
            if (!script2.GetOp(pc2, opcode2, vch2))
                break;

            // Template matching opcodes:
            if (opcode2 == OP_PUBKEYS) {
                while (vch1.size() >= 33 && vch1.size() <= 65) {
                    vSolutionsRet.push_back(vch1);
                    if (!script1.GetOp(pc1, opcode1, vch1))
                        break;
                }
                if (!script2.GetOp(pc2, opcode2, vch2))
                    break;
                // Normal situation is to fall through
                // to other if/else statements
            }

            if (opcode2 == OP_PUBKEY) {
                if (vch1.size() < 33 || vch1.size() > 65)
                    break;
                vSolutionsRet.push_back(vch1);
            } else if (opcode2 == OP_PUBKEYHASH) {
                if (vch1.size() != sizeof(uint160))
                    break;
                vSolutionsRet.push_back(vch1);
            } else if (opcode2 == OP_SMALLINTEGER) {   // Single-byte small integer pushed onto vSolutions
                if (opcode1 == OP_0 ||
                    (opcode1 >= OP_1 && opcode1 <= OP_16)) {
                    char n = (char) CScript::DecodeOP_N(opcode1);
                    vSolutionsRet.push_back(valtype(1, n));
                } else
                    break;
            } else if (opcode1 != opcode2 || vch1 != vch2) {
                // Others must match exactly
                break;
            }
        }
    }

    vSolutionsRet.clear();
    typeRet = TX_NONSTANDARD;
    return false;
}

int ScriptSigArgsExpected(txnouttype t, const std::vector <std::vector<unsigned char>> &vSolutions) {
    switch (t) {
        case TX_NONSTANDARD:
        case TX_NULL_DATA:
        case TX_PUBKEY:
            return 1;
        case TX_PUBKEYHASH:
            return 2;
        case TX_MULTISIG:
            if (vSolutions.size() < 1 || vSolutions[0].size() < 1)
                return -1;
            return vSolutions[0][0] + 1;
        case TX_SCRIPTHASH:
            return 1; // doesn't include args needed by the script
    }
    return -1;
}

bool IsStandard(const CScript &scriptPubKey, txnouttype &whichType) {
    vector <valtype> vSolutions;
    if (!Solver(scriptPubKey, whichType, vSolutions))
        return false;

    if (whichType == TX_MULTISIG) {
        unsigned char m = vSolutions.front()[0];
        unsigned char n = vSolutions.back()[0];
        // Support up to x-of-3 multisig txns as standard
        if (n < 1 || n > 3)
            return false;
        if (m < 1 || m > n)
            return false;
    } else if (whichType == TX_NULL_DATA &&
               (!GetBoolArg("-datacarrier", true) || scriptPubKey.size() > nMaxDatacarrierBytes))
        return false;

    return whichType != TX_NONSTANDARD;
}

bool ExtractPubKey(const CScript &scriptPubKey, CPubKey& out) {
    vector <valtype> vSolutions;
    txnouttype whichType;
    if (!Solver(scriptPubKey, whichType, vSolutions))
        return false;

    if (whichType == TX_PUBKEY) {
        CPubKey pubKey(vSolutions[0]);
        out.Set(pubKey.begin(), pubKey.end());
        return true;
    }
    return false;
}

bool ExtractDestination(const CScript &scriptPubKey, CTxDestination &addressRet) {
    vector <valtype> vSolutions;
=======
    std::vector<unsigned char> data;
    if (MatchPayToPubkey(scriptPubKey, data)) {
        typeRet = TX_PUBKEY;
        vSolutionsRet.push_back(std::move(data));
        return true;
    }

    if (MatchPayToPubkeyHash(scriptPubKey, data)) {
        typeRet = TX_PUBKEYHASH;
        vSolutionsRet.push_back(std::move(data));
        return true;
    }

    std::vector<unsigned char> data1;
    if (MatchPayToColdStaking(scriptPubKey, data, data1)) {
        typeRet = TX_COLDSTAKE;
        vSolutionsRet.push_back(std::move(data));
        vSolutionsRet.push_back(std::move(data1));
        return true;
    }

    unsigned int required;
    std::vector<std::vector<unsigned char>> keys;
    if (MatchMultisig(scriptPubKey, required, keys)) {
        typeRet = TX_MULTISIG;
        vSolutionsRet.push_back({static_cast<unsigned char>(required)}); // safe as required is in range 1..16
        vSolutionsRet.insert(vSolutionsRet.end(), keys.begin(), keys.end());
        vSolutionsRet.push_back({static_cast<unsigned char>(keys.size())}); // safe as size is in range 1..16
        return true;
    }

    vSolutionsRet.clear();
    typeRet = TX_NONSTANDARD;
    return false;
}

bool ExtractDestination(const CScript& scriptPubKey, CTxDestination& addressRet, bool fColdStake)
{
    std::vector<valtype> vSolutions;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    txnouttype whichType;
    if (!Solver(scriptPubKey, whichType, vSolutions))
        return false;

    if (whichType == TX_PUBKEY) {
        CPubKey pubKey(vSolutions[0]);
        if (!pubKey.IsValid())
            return false;

        addressRet = pubKey.GetID();
        return true;
<<<<<<< HEAD
    } else if (whichType == TX_PUBKEYHASH) {
        addressRet = CKeyID(uint160(vSolutions[0]));
        return true;
    } else if (whichType == TX_SCRIPTHASH) {
        addressRet = CScriptID(uint160(vSolutions[0]));
        return true;
=======

    } else if (whichType == TX_PUBKEYHASH) {
        addressRet = CKeyID(uint160(vSolutions[0]));
        return true;

    } else if (whichType == TX_SCRIPTHASH) {
        addressRet = CScriptID(uint160(vSolutions[0]));
        return true;
    } else if (whichType == TX_COLDSTAKE) {
        addressRet = CKeyID(uint160(vSolutions[!fColdStake]));
        return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }
    // Multisig txns have more than one address...
    return false;
}

<<<<<<< HEAD
bool ExtractDestinations(const CScript &scriptPubKey, txnouttype &typeRet, vector <CTxDestination> &addressRet,
                         int &nRequiredRet) {
    addressRet.clear();
    typeRet = TX_NONSTANDARD;
    vector <valtype> vSolutions;
    if (!Solver(scriptPubKey, typeRet, vSolutions))
        return false;
    if (typeRet == TX_NULL_DATA) {
=======
bool ExtractDestinations(const CScript& scriptPubKey, txnouttype& typeRet, std::vector<CTxDestination>& addressRet, int& nRequiredRet)
{
    addressRet.clear();
    typeRet = TX_NONSTANDARD;
    std::vector<valtype> vSolutions;
    if (!Solver(scriptPubKey, typeRet, vSolutions))
        return false;
    if (typeRet == TX_NULL_DATA){
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        // This is data, not addresses
        return false;
    }

<<<<<<< HEAD
    if (typeRet == TX_MULTISIG) {
        nRequiredRet = vSolutions.front()[0];
        for (unsigned int i = 1; i < vSolutions.size() - 1; i++) {
=======
    if (typeRet == TX_MULTISIG)
    {
        nRequiredRet = vSolutions.front()[0];
        for (unsigned int i = 1; i < vSolutions.size()-1; i++)
        {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            CPubKey pubKey(vSolutions[i]);
            if (!pubKey.IsValid())
                continue;

            CTxDestination address = pubKey.GetID();
            addressRet.push_back(address);
        }

        if (addressRet.empty())
            return false;
<<<<<<< HEAD
    } else {
        nRequiredRet = 1;
        CTxDestination address;
        if (!ExtractDestination(scriptPubKey, address))
            return false;
=======

    } else if (typeRet == TX_COLDSTAKE)
    {
        if (vSolutions.size() < 2)
            return false;
        nRequiredRet = 2;
        addressRet.push_back(CKeyID(uint160(vSolutions[0])));
        addressRet.push_back(CKeyID(uint160(vSolutions[1])));
        return true;

    } else
    {
        nRequiredRet = 1;
        CTxDestination address;
        if (!ExtractDestination(scriptPubKey, address))
           return false;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        addressRet.push_back(address);
    }

    return true;
}

<<<<<<< HEAD
namespace {
    class CScriptVisitor : public boost::static_visitor<bool> {
    private:
        CScript *script;
    public:
        CScriptVisitor(CScript *scriptin) { script = scriptin; }

        bool operator()(const CNoDestination &dest) const {
            script->clear();
            return false;
        }

        bool operator()(const CKeyID &keyID) const {
            script->clear();
            *script << OP_DUP << OP_HASH160 << ToByteVector(keyID) << OP_EQUALVERIFY << OP_CHECKSIG;
            return true;
        }

        bool operator()(const CScriptID &scriptID) const {
            script->clear();
            *script << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
            return true;
        }
    };
}

CScript GetScriptForDestination(const CTxDestination &dest) {
=======
namespace
{
class CScriptVisitor : public boost::static_visitor<bool>
{
private:
    CScript *script;
public:
    CScriptVisitor(CScript *scriptin) { script = scriptin; }

    bool operator()(const CNoDestination &dest) const {
        script->clear();
        return false;
    }

    bool operator()(const CKeyID &keyID) const {
        script->clear();
        *script << OP_DUP << OP_HASH160 << ToByteVector(keyID) << OP_EQUALVERIFY << OP_CHECKSIG;
        return true;
    }

    bool operator()(const CScriptID &scriptID) const {
        script->clear();
        *script << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
        return true;
    }
};
}

CScript GetScriptForDestination(const CTxDestination& dest)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CScript script;

    boost::apply_visitor(CScriptVisitor(&script), dest);
    return script;
}

<<<<<<< HEAD
CScript GetScriptForDestination(const CPubKey &pubkey) {
    CScript scriptPubKey = CScript() << ToByteVector(pubkey) << OP_CHECKSIG;
    //boost::apply_visitor(CScriptVisitor(&script), dest);
    return scriptPubKey;
}

CScript GetScriptForMultisig(int nRequired, const std::vector <CPubKey> &keys) {
    CScript script;

    script << CScript::EncodeOP_N(nRequired);
    for (const CPubKey &key : keys)
    script << ToByteVector(key);
    script << CScript::EncodeOP_N(keys.size()) << OP_CHECKMULTISIG;
    return script;
}
=======
CScript GetScriptForRawPubKey(const CPubKey& pubKey)
{
    return CScript() << std::vector<unsigned char>(pubKey.begin(), pubKey.end()) << OP_CHECKSIG;
}

CScript GetScriptForStakeDelegation(const CKeyID& stakingKey, const CKeyID& spendingKey)
{
    CScript script;
    script << OP_DUP << OP_HASH160 << OP_ROT <<
            OP_IF << OP_CHECKCOLDSTAKEVERIFY << ToByteVector(stakingKey) <<
            OP_ELSE << ToByteVector(spendingKey) << OP_ENDIF <<
            OP_EQUALVERIFY << OP_CHECKSIG;
    return script;
}

CScript GetScriptForStakeDelegationLOF(const CKeyID& stakingKey, const CKeyID& spendingKey)
{
    CScript script;
    script << OP_DUP << OP_HASH160 << OP_ROT <<
            OP_IF << OP_CHECKCOLDSTAKEVERIFY_LOF << ToByteVector(stakingKey) <<
            OP_ELSE << ToByteVector(spendingKey) << OP_ENDIF <<
            OP_EQUALVERIFY << OP_CHECKSIG;
    return script;
}

CScript GetScriptForMultisig(int nRequired, const std::vector<CPubKey>& keys)
{
    CScript script;

    script << CScript::EncodeOP_N(nRequired);
    for (const CPubKey& key : keys)
        script << ToByteVector(key);
    script << CScript::EncodeOP_N(keys.size()) << OP_CHECKMULTISIG;
    return script;
}

CScript GetScriptForOpReturn(const uint256& message)
{
    CScript script;
    script << OP_RETURN << ToByteVector(message);
    return script;
}

bool IsValidDestination(const CTxDestination& dest) {
    return dest.which() != 0;
}

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
