<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
=======
// Copyright (c) 2009-2017 The Bitcoin developers
// Copyright (c) 2017-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "key.h"

<<<<<<< HEAD
#include "crypto/hmac_sha512.h"
#include "crypto/rfc6979_hmac_sha256.h"
#include "eccryptoverify.h"
#include "pubkey.h"
#include "random.h"

#include "ecwrapper.h"
#include <secp256k1.h>

//! anonymous namespace
namespace
{
class CSecp256k1Init
{
public:
    CSecp256k1Init()
    {
        secp256k1_start(SECP256K1_START_SIGN | SECP256K1_START_VERIFY);
    }
    ~CSecp256k1Init()
    {
        secp256k1_stop();
    }
};
static CSecp256k1Init instance_of_csecp256k1;

} // anon namespace

bool CKey::Check(const unsigned char* vch)
{
    return eccrypto::Check(vch);
=======
#include "crypto/common.h"
#include "crypto/hmac_sha512.h"
#include "random.h"

#include <secp256k1.h>
#include <secp256k1_recovery.h>

static secp256k1_context* secp256k1_context_sign = nullptr;

/** These functions are taken from the libsecp256k1 distribution and are very ugly. */

/**
 * This parses a format loosely based on a DER encoding of the ECPrivateKey type from
 * section C.4 of SEC 1 <http://www.secg.org/sec1-v2.pdf>, with the following caveats:
 *
 * * The octet-length of the SEQUENCE must be encoded as 1 or 2 octets. It is not
 *   required to be encoded as one octet if it is less than 256, as DER would require.
 * * The octet-length of the SEQUENCE must not be greater than the remaining
 *   length of the key encoding, but need not match it (i.e. the encoding may contain
 *   junk after the encoded SEQUENCE).
 * * The privateKey OCTET STRING is zero-filled on the left to 32 octets.
 * * Anything after the encoding of the privateKey OCTET STRING is ignored, whether
 *   or not it is validly encoded DER.
 *
 * out32 must point to an output buffer of length at least 32 bytes.
 */
static int ec_privkey_import_der(const secp256k1_context* ctx, unsigned char *out32, const unsigned char *privkey, size_t privkeylen) {
    const unsigned char *end = privkey + privkeylen;
    memset(out32, 0, 32);
    /* sequence header */
    if (end - privkey < 1 || *privkey != 0x30u) {
        return 0;
    }
    privkey++;
    /* sequence length constructor */
    if (end - privkey < 1 || !(*privkey & 0x80u)) {
        return 0;
    }
    ptrdiff_t lenb = *privkey & ~0x80u; privkey++;
    if (lenb < 1 || lenb > 2) {
        return 0;
    }
    if (end - privkey < lenb) {
        return 0;
    }
    /* sequence length */
    ptrdiff_t len = privkey[lenb-1] | (lenb > 1 ? privkey[lenb-2] << 8 : 0u);
    privkey += lenb;
    if (end - privkey < len) {
        return 0;
    }
    /* sequence element 0: version number (=1) */
    if (end - privkey < 3 || privkey[0] != 0x02u || privkey[1] != 0x01u || privkey[2] != 0x01u) {
        return 0;
    }
    privkey += 3;
    /* sequence element 1: octet string, up to 32 bytes */
    if (end - privkey < 2 || privkey[0] != 0x04u) {
        return 0;
    }
    ptrdiff_t oslen = privkey[1];
    privkey += 2;
    if (oslen > 32 || end - privkey < oslen) {
        return 0;
    }
    memcpy(out32 + (32 - oslen), privkey, oslen);
    if (!secp256k1_ec_seckey_verify(ctx, out32)) {
        memset(out32, 0, 32);
        return 0;
    }
    return 1;
}

/**
 * This serializes to a DER encoding of the ECPrivateKey type from section C.4 of SEC 1
 * <http://www.secg.org/sec1-v2.pdf>. The optional parameters and publicKey fields are
 * included.
 *
 * privkey must point to an output buffer of length at least CKey::PRIVATE_KEY_SIZE bytes.
 * privkeylen must initially be set to the size of the privkey buffer. Upon return it
 * will be set to the number of bytes used in the buffer.
 * key32 must point to a 32-byte raw private key.
 */
static int ec_privkey_export_der(const secp256k1_context *ctx, unsigned char *privkey, size_t *privkeylen, const unsigned char *key32, int compressed) {
    assert(*privkeylen >= CKey::PRIVATE_KEY_SIZE);
    secp256k1_pubkey pubkey;
    size_t pubkeylen = 0;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, key32)) {
        *privkeylen = 0;
        return 0;
    }
    if (compressed) {
        static const unsigned char begin[] = {
            0x30,0x81,0xD3,0x02,0x01,0x01,0x04,0x20
        };
        static const unsigned char middle[] = {
            0xA0,0x81,0x85,0x30,0x81,0x82,0x02,0x01,0x01,0x30,0x2C,0x06,0x07,0x2A,0x86,0x48,
            0xCE,0x3D,0x01,0x01,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFE,0xFF,0xFF,0xFC,0x2F,0x30,0x06,0x04,0x01,0x00,0x04,0x01,0x07,0x04,
            0x21,0x02,0x79,0xBE,0x66,0x7E,0xF9,0xDC,0xBB,0xAC,0x55,0xA0,0x62,0x95,0xCE,0x87,
            0x0B,0x07,0x02,0x9B,0xFC,0xDB,0x2D,0xCE,0x28,0xD9,0x59,0xF2,0x81,0x5B,0x16,0xF8,
            0x17,0x98,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFE,0xBA,0xAE,0xDC,0xE6,0xAF,0x48,0xA0,0x3B,0xBF,0xD2,0x5E,
            0x8C,0xD0,0x36,0x41,0x41,0x02,0x01,0x01,0xA1,0x24,0x03,0x22,0x00
        };
        unsigned char *ptr = privkey;
        memcpy(ptr, begin, sizeof(begin)); ptr += sizeof(begin);
        memcpy(ptr, key32, 32); ptr += 32;
        memcpy(ptr, middle, sizeof(middle)); ptr += sizeof(middle);
        pubkeylen = CPubKey::COMPRESSED_PUBLIC_KEY_SIZE;
        secp256k1_ec_pubkey_serialize(ctx, ptr, &pubkeylen, &pubkey, SECP256K1_EC_COMPRESSED);
        ptr += pubkeylen;
        *privkeylen = ptr - privkey;
        assert(*privkeylen == CKey::COMPRESSED_PRIVATE_KEY_SIZE);
    } else {
        static const unsigned char begin[] = {
            0x30,0x82,0x01,0x13,0x02,0x01,0x01,0x04,0x20
        };
        static const unsigned char middle[] = {
            0xA0,0x81,0xA5,0x30,0x81,0xA2,0x02,0x01,0x01,0x30,0x2C,0x06,0x07,0x2A,0x86,0x48,
            0xCE,0x3D,0x01,0x01,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFE,0xFF,0xFF,0xFC,0x2F,0x30,0x06,0x04,0x01,0x00,0x04,0x01,0x07,0x04,
            0x41,0x04,0x79,0xBE,0x66,0x7E,0xF9,0xDC,0xBB,0xAC,0x55,0xA0,0x62,0x95,0xCE,0x87,
            0x0B,0x07,0x02,0x9B,0xFC,0xDB,0x2D,0xCE,0x28,0xD9,0x59,0xF2,0x81,0x5B,0x16,0xF8,
            0x17,0x98,0x48,0x3A,0xDA,0x77,0x26,0xA3,0xC4,0x65,0x5D,0xA4,0xFB,0xFC,0x0E,0x11,
            0x08,0xA8,0xFD,0x17,0xB4,0x48,0xA6,0x85,0x54,0x19,0x9C,0x47,0xD0,0x8F,0xFB,0x10,
            0xD4,0xB8,0x02,0x21,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
            0xFF,0xFF,0xFF,0xFF,0xFE,0xBA,0xAE,0xDC,0xE6,0xAF,0x48,0xA0,0x3B,0xBF,0xD2,0x5E,
            0x8C,0xD0,0x36,0x41,0x41,0x02,0x01,0x01,0xA1,0x44,0x03,0x42,0x00
        };
        unsigned char *ptr = privkey;
        memcpy(ptr, begin, sizeof(begin)); ptr += sizeof(begin);
        memcpy(ptr, key32, 32); ptr += 32;
        memcpy(ptr, middle, sizeof(middle)); ptr += sizeof(middle);
        pubkeylen = CPubKey::PUBLIC_KEY_SIZE;
        secp256k1_ec_pubkey_serialize(ctx, ptr, &pubkeylen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
        ptr += pubkeylen;
        *privkeylen = ptr - privkey;
        assert(*privkeylen == CKey::PRIVATE_KEY_SIZE);
    }
    return 1;
}

bool CKey::Check(const unsigned char* vch)
{
    return secp256k1_ec_seckey_verify(secp256k1_context_sign, vch);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

void CKey::MakeNewKey(bool fCompressedIn)
{
    do {
<<<<<<< HEAD
        GetStrongRandBytes(vch, sizeof(vch));
    } while (!Check(vch));
=======
        GetStrongRandBytes(keydata.data(), keydata.size());
    } while (!Check(keydata.data()));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    fValid = true;
    fCompressed = fCompressedIn;
}

<<<<<<< HEAD
bool CKey::SetPrivKey(const CPrivKey& privkey, bool fCompressedIn)
{
    if (!secp256k1_ec_privkey_import((unsigned char*)begin(), &privkey[0], privkey.size()))
        return false;
    fCompressed = fCompressedIn;
    fValid = true;
    return true;
}

uint256 CKey::GetPrivKey_256()
{
    void* key = &vch;
    uint256* key_256 = (uint256*)key;

    return *key_256;
=======
uint256 CKey::GetPrivKey_256()
{
    void* key = keydata.data();
    return *(uint256*)key;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

CPrivKey CKey::GetPrivKey() const
{
    assert(fValid);
    CPrivKey privkey;
<<<<<<< HEAD
    int privkeylen, ret;
    privkey.resize(279);
    privkeylen = 279;
    ret = secp256k1_ec_privkey_export(begin(), (unsigned char*)&privkey[0], &privkeylen, fCompressed);
=======
    size_t privkeylen;
    privkey.resize(PRIVATE_KEY_SIZE);
    privkeylen = PRIVATE_KEY_SIZE;
    int ret = ec_privkey_export_der(secp256k1_context_sign, (unsigned char*)privkey.data(), &privkeylen, begin(), fCompressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(ret);
    privkey.resize(privkeylen);
    return privkey;
}

CPubKey CKey::GetPubKey() const
{
    assert(fValid);
<<<<<<< HEAD
    CPubKey result;
    int clen = 65;
    int ret = secp256k1_ec_pubkey_create((unsigned char*)result.begin(), &clen, begin(), fCompressed);
    assert((int)result.size() == clen);
    assert(ret);
=======
    secp256k1_pubkey pubkey;
    size_t clen = CPubKey::PUBLIC_KEY_SIZE;
    CPubKey result;
    int ret = secp256k1_ec_pubkey_create(secp256k1_context_sign, &pubkey, begin());
    assert(ret);
    secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char*)result.begin(), &clen, &pubkey, fCompressed ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED);
    assert(result.size() == clen);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(result.IsValid());
    return result;
}

bool CKey::Sign(const uint256& hash, std::vector<unsigned char>& vchSig, uint32_t test_case) const
{
    if (!fValid)
        return false;
<<<<<<< HEAD
    vchSig.resize(72);
    RFC6979_HMAC_SHA256 prng(begin(), 32, (unsigned char*)&hash, 32);
    do {
        uint256 nonce;
        prng.Generate((unsigned char*)&nonce, 32);
        nonce += test_case;
        int nSigLen = 72;
        int ret = secp256k1_ecdsa_sign((const unsigned char*)&hash, 32, (unsigned char*)&vchSig[0], &nSigLen, begin(), (unsigned char*)&nonce);
        nonce = 0;
        if (ret) {
            vchSig.resize(nSigLen);
            return true;
        }
    } while (true);
=======
    vchSig.resize(CPubKey::SIGNATURE_SIZE);
    size_t nSigLen = CPubKey::SIGNATURE_SIZE;
    unsigned char extra_entropy[32] = {0};
    WriteLE32(extra_entropy, test_case);
    secp256k1_ecdsa_signature sig;
    int ret = secp256k1_ecdsa_sign(secp256k1_context_sign, &sig, hash.begin(), begin(), secp256k1_nonce_function_rfc6979, test_case ? extra_entropy : nullptr);
    assert(ret);
    secp256k1_ecdsa_signature_serialize_der(secp256k1_context_sign, (unsigned char*)vchSig.data(), &nSigLen, &sig);
    vchSig.resize(nSigLen);
    return true;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool CKey::VerifyPubKey(const CPubKey& pubkey) const
{
    if (pubkey.IsCompressed() != fCompressed) {
        return false;
    }
    unsigned char rnd[8];
    std::string str = "Bitcoin key verification\n";
    GetRandBytes(rnd, sizeof(rnd));
    uint256 hash;
<<<<<<< HEAD
    CHash256().Write((unsigned char*)str.data(), str.size()).Write(rnd, sizeof(rnd)).Finalize((unsigned char*)&hash);
=======
    CHash256().Write((unsigned char*)str.data(), str.size()).Write(rnd, sizeof(rnd)).Finalize(hash.begin());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    std::vector<unsigned char> vchSig;
    Sign(hash, vchSig);
    return pubkey.Verify(hash, vchSig);
}

bool CKey::SignCompact(const uint256& hash, std::vector<unsigned char>& vchSig) const
{
    if (!fValid)
        return false;
<<<<<<< HEAD
    vchSig.resize(65);
    int rec = -1;
    RFC6979_HMAC_SHA256 prng(begin(), 32, (unsigned char*)&hash, 32);
    do {
        uint256 nonce;
        prng.Generate((unsigned char*)&nonce, 32);
        int ret = secp256k1_ecdsa_sign_compact((const unsigned char*)&hash, 32, &vchSig[1], begin(), (unsigned char*)&nonce, &rec);
        nonce = 0;
        if (ret)
            break;
    } while (true);
=======
    vchSig.resize(CPubKey::COMPACT_SIGNATURE_SIZE);
    int rec = -1;
    secp256k1_ecdsa_recoverable_signature sig;
    int ret = secp256k1_ecdsa_sign_recoverable(secp256k1_context_sign, &sig, hash.begin(), begin(), secp256k1_nonce_function_rfc6979, nullptr);
    assert(ret);
    secp256k1_ecdsa_recoverable_signature_serialize_compact(secp256k1_context_sign, (unsigned char*)&vchSig[1], &rec, &sig);
    assert(ret);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    assert(rec != -1);
    vchSig[0] = 27 + rec + (fCompressed ? 4 : 0);
    return true;
}

<<<<<<< HEAD
bool CKey::Load(CPrivKey& privkey, CPubKey& vchPubKey, bool fSkipCheck = false)
{
    if (!secp256k1_ec_privkey_import((unsigned char*)begin(), &privkey[0], privkey.size()))
=======
bool CKey::Load(const CPrivKey& privkey, const CPubKey& vchPubKey, bool fSkipCheck = false)
{
    if (!ec_privkey_import_der(secp256k1_context_sign, (unsigned char*)begin(), privkey.data(), privkey.size()))
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        return false;
    fCompressed = vchPubKey.IsCompressed();
    fValid = true;

    if (fSkipCheck)
        return true;

    return VerifyPubKey(vchPubKey);
}

<<<<<<< HEAD
bool CKey::Derive(CKey& keyChild, unsigned char ccChild[32], unsigned int nChild, const unsigned char cc[32]) const
{
    assert(IsValid());
    assert(IsCompressed());
    unsigned char out[64];
    LockObject(out);
    if ((nChild >> 31) == 0) {
        CPubKey pubkey = GetPubKey();
        assert(pubkey.begin() + 33 == pubkey.end());
        BIP32Hash(cc, nChild, *pubkey.begin(), pubkey.begin() + 1, out);
    } else {
        assert(begin() + 32 == end());
        BIP32Hash(cc, nChild, 0, begin(), out);
    }
    memcpy(ccChild, out + 32, 32);
    memcpy((unsigned char*)keyChild.begin(), begin(), 32);
    bool ret = secp256k1_ec_privkey_tweak_add((unsigned char*)keyChild.begin(), out);
    UnlockObject(out);
=======
bool CKey::Derive(CKey& keyChild, ChainCode &ccChild, unsigned int nChild, const ChainCode& cc) const
{
    assert(IsValid());
    assert(IsCompressed());
    std::vector<unsigned char, secure_allocator<unsigned char>> vout(64);
    if ((nChild >> 31) == 0) {
        CPubKey pubkey = GetPubKey();
        assert(pubkey.size() == CPubKey::COMPRESSED_PUBLIC_KEY_SIZE);
        BIP32Hash(cc, nChild, *pubkey.begin(), pubkey.begin()+1, vout.data());
    } else {
        assert(size() == 32);
        BIP32Hash(cc, nChild, 0, begin(), vout.data());
    }
    memcpy(ccChild.begin(), vout.data()+32, 32);
    memcpy((unsigned char*)keyChild.begin(), begin(), 32);
    bool ret = secp256k1_ec_privkey_tweak_add(secp256k1_context_sign, (unsigned char*)keyChild.begin(), vout.data());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    keyChild.fCompressed = true;
    keyChild.fValid = ret;
    return ret;
}

<<<<<<< HEAD
bool CExtKey::Derive(CExtKey& out, unsigned int nChild) const
=======
bool CExtKey::Derive(CExtKey& out, unsigned int _nChild) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    out.nDepth = nDepth + 1;
    CKeyID id = key.GetPubKey().GetID();
    memcpy(&out.vchFingerprint[0], &id, 4);
<<<<<<< HEAD
    out.nChild = nChild;
    return key.Derive(out.key, out.vchChainCode, nChild, vchChainCode);
}

void CExtKey::SetMaster(const unsigned char* seed, unsigned int nSeedLen)
{
    static const unsigned char hashkey[] = {'p', 'r', 'c', 'y', 'c', 'o', 'i', 'n', ' ', 's', 'e', 'e', 'd'};
    unsigned char out[64];
    LockObject(out);
    CHMAC_SHA512(hashkey, sizeof(hashkey)).Write(seed, nSeedLen).Finalize(out);
    key.Set(&out[0], &out[32], true);
    memcpy(vchChainCode, &out[32], 32);
    UnlockObject(out);
=======
    out.nChild = _nChild;
    return key.Derive(out.key, out.chaincode, _nChild, chaincode);
}

void CExtKey::SetSeed(const unsigned char* seed, unsigned int nSeedLen)
{
    static const unsigned char hashkey[] = {'B', 'i', 't', 'c', 'o', 'i', 'n', ' ', 's', 'e', 'e', 'd'};
    std::vector<unsigned char, secure_allocator<unsigned char>> vout(64);
    CHMAC_SHA512(hashkey, sizeof(hashkey)).Write(seed, nSeedLen).Finalize(vout.data());
    key.Set(vout.data(), vout.data() + 32, true);
    memcpy(chaincode.begin(), vout.data() + 32, 32);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    nDepth = 0;
    nChild = 0;
    memset(vchFingerprint, 0, sizeof(vchFingerprint));
}

CExtPubKey CExtKey::Neuter() const
{
    CExtPubKey ret;
    ret.nDepth = nDepth;
    memcpy(&ret.vchFingerprint[0], &vchFingerprint[0], 4);
    ret.nChild = nChild;
    ret.pubkey = key.GetPubKey();
<<<<<<< HEAD
    memcpy(&ret.vchChainCode[0], &vchChainCode[0], 32);
    return ret;
}

void CExtKey::Encode(unsigned char code[74]) const
=======
    ret.chaincode = chaincode;
    return ret;
}

void CExtKey::Encode(unsigned char code[BIP32_EXTKEY_SIZE]) const
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    code[0] = nDepth;
    memcpy(code + 1, vchFingerprint, 4);
    code[5] = (nChild >> 24) & 0xFF;
    code[6] = (nChild >> 16) & 0xFF;
    code[7] = (nChild >> 8) & 0xFF;
    code[8] = (nChild >> 0) & 0xFF;
<<<<<<< HEAD
    memcpy(code + 9, vchChainCode, 32);
=======
    memcpy(code + 9, chaincode.begin(), 32);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    code[41] = 0;
    assert(key.size() == 32);
    memcpy(code + 42, key.begin(), 32);
}

<<<<<<< HEAD
void CExtKey::Decode(const unsigned char code[74])
=======
void CExtKey::Decode(const unsigned char code[BIP32_EXTKEY_SIZE])
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    nDepth = code[0];
    memcpy(vchFingerprint, code + 1, 4);
    nChild = (code[5] << 24) | (code[6] << 16) | (code[7] << 8) | code[8];
<<<<<<< HEAD
    memcpy(vchChainCode, code + 9, 32);
    key.Set(code + 42, code + 74, true);
=======
    memcpy(chaincode.begin(), code + 9, 32);
    key.Set(code + 42, code + BIP32_EXTKEY_SIZE, true);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
}

bool ECC_InitSanityCheck()
{
<<<<<<< HEAD
#if !defined(USE_SECP256K1)
    if (!CECKey::SanityCheck()) {
        return false;
    }
#endif
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CKey key;
    key.MakeNewKey(true);
    CPubKey pubkey = key.GetPubKey();
    return key.VerifyPubKey(pubkey);
}
<<<<<<< HEAD
=======

void ECC_Start() {
    assert(secp256k1_context_sign == nullptr);

    secp256k1_context *ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);
    assert(ctx != nullptr);

    {
        // Pass in a random blinding seed to the secp256k1 context.
        std::vector<unsigned char, secure_allocator<unsigned char>> vseed(32);
        GetRandBytes(vseed.data(), 32);
        bool ret = secp256k1_context_randomize(ctx, vseed.data());
        assert(ret);
    }

    secp256k1_context_sign = ctx;
}

void ECC_Stop() {
    secp256k1_context *ctx = secp256k1_context_sign;
    secp256k1_context_sign = nullptr;

    if (ctx) {
        secp256k1_context_destroy(ctx);
    }
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
