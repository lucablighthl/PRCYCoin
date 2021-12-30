// Copyright (c) 2009-2010 Satoshi Nakamoto
<<<<<<< HEAD
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2009-2021 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2021 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet/walletdb.h"

<<<<<<< HEAD
#include "base58.h"
#include "protocol.h"
#include "serialize.h"
#include "sync.h"
#include "util.h"
#include "utiltime.h"
#include "wallet/wallet.h"

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <fstream>

using namespace boost;
using namespace std;

static uint64_t nAccountingEntryNumber = 0;

//
// CWalletDB
//

bool CWalletDB::AppendStealthAccountList(const std::string& accountName) {
    std::string currentList;
    if (!ReadStealthAccountList(currentList)) {
        currentList = accountName;
    } else {
        currentList = currentList + "," + accountName;
        nWalletDBUpdated++;
        Erase(std::string("accountlist"));
    }
    nWalletDBUpdated++;
    return Write(std::string("accountlist"), currentList);
}

bool CWalletDB::ReadStealthAccountList(std::string& accountList) {
    return Read(std::string("accountlist"), accountList);
}

bool CWalletDB::WriteName(const string& strAddress, const string& strName)
{
    nWalletDBUpdated++;
    return Write(make_pair(string("name"), strAddress), strName);
}

bool CWalletDB::EraseName(const string& strAddress)
{
    // This should only be used for sending addresses, never for receiving addresses,
    // receiving addresses must always have an address book entry if they're not change return.
    nWalletDBUpdated++;
    return Erase(make_pair(string("name"), strAddress));
}

bool CWalletDB::WritePurpose(const string& strAddress, const string& strPurpose)
{
    nWalletDBUpdated++;
    return Write(make_pair(string("purpose"), strAddress), strPurpose);
}

bool CWalletDB::ErasePurpose(const string& strPurpose)
{
    nWalletDBUpdated++;
    return Erase(make_pair(string("purpose"), strPurpose));
}

bool CWalletDB::WriteTx(uint256 hash, const CWalletTx& wtx)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("tx"), hash), wtx);
}

bool CWalletDB::EraseTx(uint256 hash)
{
    nWalletDBUpdated++;
    return Erase(std::make_pair(std::string("tx"), hash));
}

bool CWalletDB::WriteKey(const CPubKey& vchPubKey, const CPrivKey& vchPrivKey, const CKeyMetadata& keyMeta)
{
    nWalletDBUpdated++;

    if (!Write(std::make_pair(std::string("keymeta"), vchPubKey),
            keyMeta, false))
        return false;
=======
#include "fs.h"

#include "key_io.h"
#include "protocol.h"
#include "reverse_iterate.h"
#include "sapling/key_io_sapling.h"
#include "serialize.h"
#include "sync.h"
#include "util/system.h"
#include "utiltime.h"
#include "wallet/wallet.h"
#include "wallet/walletutil.h"

#include <atomic>

#include <boost/thread.hpp>

namespace DBKeys {
    const std::string BESTBLOCK_NOMERKLE{"bestblock_nomerkle"};
    const std::string BESTBLOCK{"bestblock"};
    const std::string CRYPTED_KEY{"ckey"};
    const std::string CSCRIPT{"cscript"};
    const std::string DEFAULTKEY{"defaultkey"};
    const std::string DESTDATA{"destdata"};
    const std::string HDCHAIN{"hdchain"};
    const std::string KEYMETA{"keymeta"};
    const std::string KEY{"key"};
    const std::string MASTER_KEY{"mkey"};
    const std::string MINVERSION{"minversion"};
    const std::string NAME{"name"};
    const std::string ORDERPOSNEXT{"orderposnext"};
    const std::string POOL{"pool"};
    const std::string PURPOSE{"purpose"};
    const std::string TX{"tx"};
    const std::string VERSION{"version"};
    const std::string WATCHS{"watchs"};

    // Sapling
    const std::string SAP_KEYMETA{"sapzkeymeta"};
    const std::string SAP_KEY{"sapzkey"};
    const std::string SAP_KEY_CRIPTED{"csapzkey"};
    const std::string SAP_ADDR{"sapzaddr"};
    const std::string SAP_COMMON_OVK{"commonovk"};
    const std::string SAP_HDCHAIN{"hdchain_sap"};
    const std::string SAP_WITNESS_CACHE_SIZE{"witnesscachesize"};

    // Wallet custom settings
    const std::string AUTOCOMBINE{"autocombinesettings"};
    const std::string STAKE_SPLIT_THRESHOLD{"stakeSplitThreshold"};
    const std::string USE_CUSTOM_FEE{"fUseCustomFee"};
    const std::string CUSTOM_FEE_VALUE{"nCustomFee"};

} // namespace DBKeys


//
// WalletBatch
//

bool WalletBatch::WriteName(const std::string& strAddress, const std::string& strName)
{
    return WriteIC(std::make_pair(std::string(DBKeys::NAME), strAddress), strName);
}

bool WalletBatch::EraseName(const std::string& strAddress)
{
    // This should only be used for sending addresses, never for receiving addresses,
    // receiving addresses must always have an address book entry if they're not change return.
    return EraseIC(std::make_pair(std::string(DBKeys::NAME), strAddress));
}

bool WalletBatch::WritePurpose(const std::string& strAddress, const std::string& strPurpose)
{
    return WriteIC(std::make_pair(std::string(DBKeys::PURPOSE), strAddress), strPurpose);
}

bool WalletBatch::ErasePurpose(const std::string& strPurpose)
{
    return EraseIC(std::make_pair(std::string(DBKeys::PURPOSE), strPurpose));
}

bool WalletBatch::WriteTx(const CWalletTx& wtx)
{
    return WriteIC(std::make_pair(std::string(DBKeys::TX), wtx.GetHash()), wtx);
}

bool WalletBatch::EraseTx(uint256 hash)
{
    return EraseIC(std::make_pair(std::string(DBKeys::TX), hash));
}

bool WalletBatch::WriteKey(const CPubKey& vchPubKey, const CPrivKey& vchPrivKey, const CKeyMetadata& keyMeta)
{
    if (!WriteIC(std::make_pair(std::string(DBKeys::KEYMETA), vchPubKey), keyMeta, false)) {
        return false;
    }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    // hash pubkey/privkey to accelerate wallet load
    std::vector<unsigned char> vchKey;
    vchKey.reserve(vchPubKey.size() + vchPrivKey.size());
    vchKey.insert(vchKey.end(), vchPubKey.begin(), vchPubKey.end());
    vchKey.insert(vchKey.end(), vchPrivKey.begin(), vchPrivKey.end());

<<<<<<< HEAD
    return Write(std::make_pair(std::string("key"), vchPubKey), std::make_pair(vchPrivKey, Hash(vchKey.begin(), vchKey.end())), false);
}

bool CWalletDB::WriteCryptedKey(const CPubKey& vchPubKey,
    const std::vector<unsigned char>& vchCryptedSecret,
    const CKeyMetadata& keyMeta)
{
    const bool fEraseUnencryptedKey = true;
    nWalletDBUpdated++;

    if (!Write(std::make_pair(std::string("keymeta"), vchPubKey),
            keyMeta))
        return false;

    if (!Write(std::make_pair(std::string("ckey"), vchPubKey), vchCryptedSecret, false))
        return false;
    if (fEraseUnencryptedKey) {
        Erase(std::make_pair(std::string("key"), vchPubKey));
        Erase(std::make_pair(std::string("wkey"), vchPubKey));
    }
    return true;
}

bool CWalletDB::WriteMasterKey(unsigned int nID, const CMasterKey& kMasterKey)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("mkey"), nID), kMasterKey, true);
}

bool CWalletDB::WriteCScript(const uint160& hash, const CScript& redeemScript)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("cscript"), hash), redeemScript, false);
}

bool CWalletDB::WriteWatchOnly(const CScript& dest)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("watchs"), dest), '1');
}

bool CWalletDB::EraseWatchOnly(const CScript& dest)
{
    nWalletDBUpdated++;
    return Erase(std::make_pair(std::string("watchs"), dest));
}

bool CWalletDB::WriteMultiSig(const CScript& dest)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("multisig"), dest), '1');
}

bool CWalletDB::EraseMultiSig(const CScript& dest)
{
    nWalletDBUpdated++;
    return Erase(std::make_pair(std::string("multisig"), dest));
}

bool CWalletDB::WriteReserveAmount(const double& amount)
{
    nWalletDBUpdated++;
    return Write(std::string("reservebalance"), amount);
}

bool CWalletDB::ReadReserveAmount(double& amount)
{
    return Read(std::string("reservebalance"), amount);
}

bool CWalletDB::WriteBestBlock(const CBlockLocator& locator)
{
    nWalletDBUpdated++;
    return Write(std::string("bestblock"), locator);
}

bool CWalletDB::ReadBestBlock(CBlockLocator& locator)
{
    return Read(std::string("bestblock"), locator);
}

bool CWalletDB::WriteOrderPosNext(int64_t nOrderPosNext)
{
    nWalletDBUpdated++;
    return Write(std::string("orderposnext"), nOrderPosNext);
}

// presstab HyperStake
bool CWalletDB::WriteStakeSplitThreshold(uint64_t nStakeSplitThreshold)
{
    nWalletDBUpdated++;
    return Write(std::string("stakeSplitThreshold"), nStakeSplitThreshold);
}

//presstab HyperStake
bool CWalletDB::WriteMultiSend(std::vector<std::pair<std::string, int> > vMultiSend)
{
    nWalletDBUpdated++;
    bool ret = true;
    for (unsigned int i = 0; i < vMultiSend.size(); i++) {
        std::pair<std::string, int> pMultiSend;
        pMultiSend = vMultiSend[i];
        if (!Write(std::make_pair(std::string("multisend"), i), pMultiSend, true))
            ret = false;
    }
    return ret;
}
//presstab HyperStake
bool CWalletDB::EraseMultiSend(std::vector<std::pair<std::string, int> > vMultiSend)
{
    nWalletDBUpdated++;
    bool ret = true;
    for (unsigned int i = 0; i < vMultiSend.size(); i++) {
        std::pair<std::string, int> pMultiSend;
        pMultiSend = vMultiSend[i];
        if (!Erase(std::make_pair(std::string("multisend"), i)))
            ret = false;
    }
    return ret;
}
//presstab HyperStake
bool CWalletDB::WriteMSettings(bool fMultiSendStake, bool fMultiSendMasternode, int nLastMultiSendHeight)
{
    nWalletDBUpdated++;
    std::pair<bool, bool> enabledMS(fMultiSendStake, fMultiSendMasternode);
    std::pair<std::pair<bool, bool>, int> pSettings(enabledMS, nLastMultiSendHeight);

    return Write(std::string("msettingsv2"), pSettings, true);
}
//presstab HyperStake
bool CWalletDB::WriteMSDisabledAddresses(std::vector<std::string> vDisabledAddresses)
{
    nWalletDBUpdated++;
    bool ret = true;
    for (unsigned int i = 0; i < vDisabledAddresses.size(); i++) {
        if (!Write(std::make_pair(std::string("mdisabled"), i), vDisabledAddresses[i]))
            ret = false;
    }
    return ret;
}
//presstab HyperStake
bool CWalletDB::EraseMSDisabledAddresses(std::vector<std::string> vDisabledAddresses)
{
    nWalletDBUpdated++;
    bool ret = true;
    for (unsigned int i = 0; i < vDisabledAddresses.size(); i++) {
        if (!Erase(std::make_pair(std::string("mdisabled"), i)))
            ret = false;
    }
    return ret;
}
bool CWalletDB::WriteAutoCombineSettings(bool fEnable, CAmount nCombineThreshold)
{
    nWalletDBUpdated++;
    std::pair<bool, CAmount> pSettings;
    pSettings.first = fEnable;
    pSettings.second = nCombineThreshold;
    return Write(std::string("autocombinesettings"), pSettings, true);
}

bool CWalletDB::WriteDefaultKey(const CPubKey& vchPubKey)
{
    nWalletDBUpdated++;
    return Write(std::string("defaultkey"), vchPubKey);
}

bool CWalletDB::ReadPool(int64_t nPool, CKeyPool& keypool)
{
    return Read(std::make_pair(std::string("pool"), nPool), keypool);
}

bool CWalletDB::WritePool(int64_t nPool, const CKeyPool& keypool)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("pool"), nPool), keypool);
}

bool CWalletDB::ErasePool(int64_t nPool)
{
    nWalletDBUpdated++;
    return Erase(std::make_pair(std::string("pool"), nPool));
}

bool CWalletDB::WriteMinVersion(int nVersion)
{
    return Write(std::string("minversion"), nVersion);
}

bool CWalletDB::WriteStakingStatus(bool status) {
    return Write(std::string("stakingstatus"), status);
}

bool CWalletDB::ReadStakingStatus() {
    bool status;
    if (!Read(std::string("stakingstatus"), status)) {
        return false;
    }
    return status;
}

bool CWalletDB::WriteScannedBlockHeight(int height)
{
    return Write(std::string("scannedblockheight"), height);
}
bool CWalletDB::ReadScannedBlockHeight(int& height)
{
    return Read(std::string("scannedblockheight"), height);
}

bool CWalletDB::Write2FA(bool status)
{
    return Write(std::string("2fa"), status);
}
bool CWalletDB::Read2FA()
{
    bool status;
    if (!Read(std::string("2fa"), status)) {
        return false;
    }
    return status;
}

bool CWalletDB::Write2FASecret(std::string secret)
{
    return Write(std::string("2fasecret"), secret);
}
std::string CWalletDB::Read2FASecret()
{
    std::string secret;
    if (!Read(std::string("2fasecret"), secret))
        return "";
    return secret;
}

bool CWalletDB::Write2FAPeriod(int period)
{
    return Write(std::string("2faperiod"), period);
}
int CWalletDB::Read2FAPeriod()
{
    int period;
    if (!Read(std::string("2faperiod"), period))
        return 0;
    return period;
}

bool CWalletDB::Write2FALastTime(uint64_t lastTime)
{
    return Write(std::string("2falasttime"), lastTime);
}
uint64_t CWalletDB::Read2FALastTime()
{
    uint64_t lastTime;
    if (!Read(std::string("2falasttime"), lastTime))
        return 0;
    return lastTime;
}

bool CWalletDB::ReadAccount(const string& strAccount, CAccount& account)
{
    account.SetNull();
    return Read(make_pair(string("acc"), strAccount), account);
}

bool CWalletDB::WriteAutoConsolidateSettingTime(uint32_t settingTime)
{
    return Write(std::string("autoconsolidatetime"), settingTime);
}

uint32_t CWalletDB::ReadAutoConsolidateSettingTime()
{
    uint32_t settingTime = 0;
    if (!Read(std::string("autoconsolidatetime"), settingTime)) {
        return 0;
    }
    return settingTime;
}


bool CWalletDB::WriteAccount(const string& strAccount, const CAccount& account)
{
    return Write(make_pair(string("acc"), strAccount), account);
}

bool CWalletDB::ReadStealthAccount(const std::string& strAccount, CStealthAccount& account)
{
    if (strAccount == "masteraccount") {
        return ReadAccount("spendaccount", account.spendAccount) && ReadAccount("viewaccount", account.viewAccount);
    }
    return ReadAccount(strAccount + "spend", account.spendAccount) && ReadAccount(strAccount + "view", account.viewAccount);
}

bool CWalletDB::WriteStealthAccount(const std::string& strAccount, const CStealthAccount& account) {
    if (strAccount == "masteraccount") {
        return WriteAccount("spendaccount", account.spendAccount) && WriteAccount("viewaccount", account.viewAccount);
    }
    return WriteAccount(strAccount + "spend", account.spendAccount) && WriteAccount(strAccount + "view", account.viewAccount);
}

bool CWalletDB::WriteAccountingEntry(const uint64_t nAccEntryNum, const CAccountingEntry& acentry)
{
    return Write(std::make_pair(std::string("acentry"), std::make_pair(acentry.strAccount, nAccEntryNum)), acentry);
}

bool CWalletDB::WriteAccountingEntry_Backend(const CAccountingEntry& acentry)
{
    return WriteAccountingEntry(++nAccountingEntryNumber, acentry);
}

CAmount CWalletDB::GetAccountCreditDebit(const string& strAccount)
{
    list<CAccountingEntry> entries;
    ListAccountCreditDebit(strAccount, entries);

    CAmount nCreditDebit = 0;
    for (const CAccountingEntry& entry : entries)
        nCreditDebit += entry.nCreditDebit;

    return nCreditDebit;
}

void CWalletDB::ListAccountCreditDebit(const string& strAccount, list<CAccountingEntry>& entries)
{
    bool fAllAccounts = (strAccount == "*");

    Dbc* pcursor = GetCursor();
    if (!pcursor)
        throw runtime_error("CWalletDB::ListAccountCreditDebit() : cannot create DB cursor");
    unsigned int fFlags = DB_SET_RANGE;
    while (true) {
        // Read next record
        CDataStream ssKey(SER_DISK, CLIENT_VERSION);
        if (fFlags == DB_SET_RANGE)
            ssKey << std::make_pair(std::string("acentry"), std::make_pair((fAllAccounts ? string("") : strAccount), uint64_t(0)));
        CDataStream ssValue(SER_DISK, CLIENT_VERSION);
        int ret = ReadAtCursor(pcursor, ssKey, ssValue, fFlags);
        fFlags = DB_NEXT;
        if (ret == DB_NOTFOUND)
            break;
        else if (ret != 0) {
            pcursor->close();
            throw runtime_error("CWalletDB::ListAccountCreditDebit() : error scanning DB");
        }

        // Unserialize
        string strType;
        ssKey >> strType;
        if (strType != "acentry")
            break;
        CAccountingEntry acentry;
        ssKey >> acentry.strAccount;
        if (!fAllAccounts && acentry.strAccount != strAccount)
            break;

        ssValue >> acentry;
        ssKey >> acentry.nEntryNo;
        entries.push_back(acentry);
    }

    pcursor->close();
}

DBErrors CWalletDB::ReorderTransactions(CWallet* pwallet)
=======
    return WriteIC(std::make_pair(std::string(DBKeys::KEY), vchPubKey), std::make_pair(vchPrivKey, Hash(vchKey.begin(), vchKey.end())), false);
}

bool WalletBatch::WriteCryptedKey(const CPubKey& vchPubKey,
                                const std::vector<unsigned char>& vchCryptedSecret,
                                const CKeyMetadata& keyMeta)
{
    const bool fEraseUnencryptedKey = true;

    if (!WriteIC(std::make_pair(std::string(DBKeys::KEYMETA), vchPubKey), keyMeta)) {
        return false;
    }

    if (!WriteIC(std::make_pair(std::string(DBKeys::CRYPTED_KEY), vchPubKey), vchCryptedSecret, false)) {
        return false;
    }
    if (fEraseUnencryptedKey) {
        EraseIC(std::make_pair(std::string(DBKeys::KEY), vchPubKey));
    }

    return true;
}

bool WalletBatch::WriteSaplingZKey(const libzcash::SaplingIncomingViewingKey &ivk,
                                 const libzcash::SaplingExtendedSpendingKey &key,
                                 const CKeyMetadata &keyMeta)
{
    if (!WriteIC(std::make_pair(std::string(DBKeys::SAP_KEYMETA), ivk), keyMeta)) {
        return false;
    }

    return WriteIC(std::make_pair(std::string(DBKeys::SAP_KEY), ivk), key, false);
}

bool WalletBatch::WriteSaplingPaymentAddress(const libzcash::SaplingPaymentAddress &addr,
                                           const libzcash::SaplingIncomingViewingKey &ivk)
{
    return WriteIC(std::make_pair(std::string(DBKeys::SAP_ADDR), addr), ivk, false);
}

bool WalletBatch::WriteCryptedSaplingZKey(const libzcash::SaplingExtendedFullViewingKey &extfvk,
                                        const std::vector<unsigned char>& vchCryptedSecret,
                                        const CKeyMetadata &keyMeta)
{
    const bool fEraseUnencryptedKey = true;
    auto ivk = extfvk.fvk.in_viewing_key();

    if (!WriteIC(std::make_pair(std::string(DBKeys::SAP_KEYMETA), ivk), keyMeta)) {
        return false;
    }

    if (!WriteIC(std::make_pair(std::string(DBKeys::SAP_KEY_CRIPTED), ivk),
                 std::make_pair(extfvk, vchCryptedSecret), false)) {
        return false;
    }

    if (fEraseUnencryptedKey) {
        EraseIC(std::make_pair(std::string(DBKeys::SAP_KEY), ivk));
    }
    return true;
}

bool WalletBatch::WriteSaplingCommonOVK(const uint256& ovk)
{
    return WriteIC(std::string(DBKeys::SAP_COMMON_OVK), ovk);
}

bool WalletBatch::ReadSaplingCommonOVK(uint256& ovkRet)
{
    return m_batch.Read(std::string(DBKeys::SAP_COMMON_OVK), ovkRet);
}

bool WalletBatch::WriteWitnessCacheSize(int64_t nWitnessCacheSize)
{
    return WriteIC(std::string(DBKeys::SAP_WITNESS_CACHE_SIZE), nWitnessCacheSize);
}

bool WalletBatch::WriteMasterKey(unsigned int nID, const CMasterKey& kMasterKey)
{
    return WriteIC(std::make_pair(std::string(DBKeys::MASTER_KEY), nID), kMasterKey, true);
}

bool WalletBatch::WriteCScript(const uint160& hash, const CScript& redeemScript)
{
    return WriteIC(std::make_pair(std::string(DBKeys::CSCRIPT), hash), redeemScript, false);
}

bool WalletBatch::WriteWatchOnly(const CScript& dest)
{
    return WriteIC(std::make_pair(std::string(DBKeys::WATCHS), dest), '1');
}

bool WalletBatch::EraseWatchOnly(const CScript& dest)
{
    return EraseIC(std::make_pair(std::string(DBKeys::WATCHS), dest));
}

bool WalletBatch::WriteBestBlock(const CBlockLocator& locator)
{
    WriteIC(std::string(DBKeys::BESTBLOCK), CBlockLocator()); // Write empty block locator so versions that require a merkle branch automatically rescan
    return WriteIC(std::string(DBKeys::BESTBLOCK_NOMERKLE), locator);
}

bool WalletBatch::ReadBestBlock(CBlockLocator& locator)
{
    if (m_batch.Read(std::string(DBKeys::BESTBLOCK), locator) && !locator.vHave.empty()) {
        return true;
    }
    return m_batch.Read(std::string(DBKeys::BESTBLOCK_NOMERKLE), locator);
}

bool WalletBatch::WriteOrderPosNext(int64_t nOrderPosNext)
{
    return WriteIC(std::string(DBKeys::ORDERPOSNEXT), nOrderPosNext);
}

bool WalletBatch::WriteStakeSplitThreshold(const CAmount& nStakeSplitThreshold)
{
    return WriteIC(std::string(DBKeys::STAKE_SPLIT_THRESHOLD), nStakeSplitThreshold);
}

bool WalletBatch::WriteUseCustomFee(bool fUse)
{
    return WriteIC(std::string(DBKeys::USE_CUSTOM_FEE), fUse);
}

bool WalletBatch::WriteCustomFeeValue(const CAmount& nFee)
{
    return WriteIC(std::string(DBKeys::CUSTOM_FEE_VALUE), nFee);
}

bool WalletBatch::WriteAutoCombineSettings(bool fEnable, CAmount nCombineThreshold)
{
    std::pair<bool, CAmount> pSettings;
    pSettings.first = fEnable;
    pSettings.second = nCombineThreshold;
    return WriteIC(std::string(DBKeys::AUTOCOMBINE), pSettings, true);
}

bool WalletBatch::ReadPool(int64_t nPool, CKeyPool& keypool)
{
    return m_batch.Read(std::make_pair(std::string(DBKeys::POOL), nPool), keypool);
}

bool WalletBatch::WritePool(int64_t nPool, const CKeyPool& keypool)
{
    return WriteIC(std::make_pair(std::string(DBKeys::POOL), nPool), keypool);
}

bool WalletBatch::ErasePool(int64_t nPool)
{
    return EraseIC(std::make_pair(std::string(DBKeys::POOL), nPool));
}

bool WalletBatch::WriteMinVersion(int nVersion)
{
    return WriteIC(std::string(DBKeys::MINVERSION), nVersion);
}

bool WalletBatch::WriteHDChain(const CHDChain& chain)
{
    std::string key = chain.chainType == HDChain::ChainCounterType::Sapling ?
                        DBKeys::SAP_HDCHAIN : DBKeys::HDCHAIN;
    return WriteIC(key, chain);
}

DBErrors WalletBatch::ReorderTransactions(CWallet* pwallet)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    LOCK(pwallet->cs_wallet);
    // Old wallets didn't have any defined order for transactions
    // Probably a bad idea to change the output of this

<<<<<<< HEAD
    // First: get all CWalletTx and CAccountingEntry into a sorted-by-time multimap.
    typedef pair<CWalletTx*, CAccountingEntry*> TxPair;
    typedef multimap<int64_t, TxPair> TxItems;
    TxItems txByTime;

    for (map<uint256, CWalletTx>::iterator it = pwallet->mapWallet.begin(); it != pwallet->mapWallet.end(); ++it) {
        CWalletTx* wtx = &((*it).second);
        txByTime.insert(make_pair(wtx->nTimeReceived, TxPair(wtx, (CAccountingEntry*)0)));
    }
    list<CAccountingEntry> acentries;
    ListAccountCreditDebit("", acentries);
    for (CAccountingEntry& entry : acentries) {
        txByTime.insert(make_pair(entry.nTime, TxPair((CWalletTx*)0, &entry)));
=======
    // First: get all CWalletTx into a sorted-by-time multimap.
    typedef std::multimap<int64_t, CWalletTx*> TxItems;
    TxItems txByTime;

    for (auto it = pwallet->mapWallet.begin(); it != pwallet->mapWallet.end(); ++it) {
        CWalletTx* wtx = &((*it).second);
        txByTime.insert(std::make_pair(wtx->nTimeReceived, wtx));
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    int64_t& nOrderPosNext = pwallet->nOrderPosNext;
    nOrderPosNext = 0;
    std::vector<int64_t> nOrderPosOffsets;
    for (TxItems::iterator it = txByTime.begin(); it != txByTime.end(); ++it) {
<<<<<<< HEAD
        CWalletTx* const pwtx = (*it).second.first;
        CAccountingEntry* const pacentry = (*it).second.second;
        int64_t& nOrderPos = (pwtx != 0) ? pwtx->nOrderPos : pacentry->nOrderPos;
=======
        CWalletTx *const pwtx = (*it).second;
        int64_t& nOrderPos = pwtx->nOrderPos;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

        if (nOrderPos == -1) {
            nOrderPos = nOrderPosNext++;
            nOrderPosOffsets.push_back(nOrderPos);

<<<<<<< HEAD
            if (pwtx) {
                if (!WriteTx(pwtx->GetHash(), *pwtx))
                    return DB_LOAD_FAIL;
            } else if (!WriteAccountingEntry(pacentry->nEntryNo, *pacentry))
                return DB_LOAD_FAIL;
=======
            if (!WriteTx(*pwtx)) return DB_LOAD_FAIL;

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        } else {
            int64_t nOrderPosOff = 0;
            for (const int64_t& nOffsetStart : nOrderPosOffsets) {
                if (nOrderPos >= nOffsetStart)
                    ++nOrderPosOff;
            }
            nOrderPos += nOrderPosOff;
            nOrderPosNext = std::max(nOrderPosNext, nOrderPos + 1);

<<<<<<< HEAD
            if (!nOrderPosOff)
                continue;

            // Since we're changing the order, write it back
            if (pwtx) {
                if (!WriteTx(pwtx->GetHash(), *pwtx))
                    return DB_LOAD_FAIL;
            } else if (!WriteAccountingEntry(pacentry->nEntryNo, *pacentry))
                return DB_LOAD_FAIL;
=======
            if (!nOrderPosOff) continue;

            // Since we're changing the order, write it back
            if (!WriteTx(*pwtx)) return DB_LOAD_FAIL;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    }
    WriteOrderPosNext(nOrderPosNext);

    return DB_LOAD_OK;
}

class CWalletScanState
{
public:
    unsigned int nKeys;
    unsigned int nCKeys;
    unsigned int nKeyMeta;
<<<<<<< HEAD
    bool fIsEncrypted;
    bool fAnyUnordered;
    int nFileVersion;
    vector<uint256> vWalletUpgrade;

    CWalletScanState()
    {
        nKeys = nCKeys = nKeyMeta = 0;
=======
    unsigned int nZKeys;
    unsigned int nZKeyMeta;
    unsigned int nSapZAddrs;
    bool fIsEncrypted;
    bool fAnyUnordered;
    int nFileVersion;
    std::vector<uint256> vWalletUpgrade;

    CWalletScanState()
    {
        nKeys = nCKeys = nKeyMeta = nZKeys = nZKeyMeta = nSapZAddrs = 0;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        fIsEncrypted = false;
        fAnyUnordered = false;
        nFileVersion = 0;
    }
};

<<<<<<< HEAD
bool ReadKeyValue(CWallet* pwallet, CDataStream& ssKey, CDataStream& ssValue, CWalletScanState& wss, string& strType, string& strErr)
=======
bool ReadKeyValue(CWallet* pwallet, CDataStream& ssKey, CDataStream& ssValue, CWalletScanState& wss, std::string& strType, std::string& strErr)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
{
    try {
        // Unserialize
        // Taking advantage of the fact that pair serialization
        // is just the two items serialized one after the other
        ssKey >> strType;
<<<<<<< HEAD
        if (strType == "name") {
            string strAddress;
            ssKey >> strAddress;
            ssValue >> pwallet->mapAddressBook[CBitcoinAddress(strAddress).Get()].name;
        } else if (strType == "purpose") {
            string strAddress;
            ssKey >> strAddress;
            ssValue >> pwallet->mapAddressBook[CBitcoinAddress(strAddress).Get()].purpose;
        } else if (strType == "tx") {
            uint256 hash;
            ssKey >> hash;
            CWalletTx wtx;
=======
        if (strType == DBKeys::NAME) {
            std::string strAddress;
            ssKey >> strAddress;
            std::string strName;
            ssValue >> strName;
            pwallet->LoadAddressBookName(Standard::DecodeDestination(strAddress), strName);
        } else if (strType == DBKeys::PURPOSE) {
            std::string strAddress;
            ssKey >> strAddress;
            std::string strPurpose;
            ssValue >> strPurpose;
            pwallet->LoadAddressBookPurpose(Standard::DecodeDestination(strAddress), strPurpose);
        } else if (strType == DBKeys::TX) {
            uint256 hash;
            ssKey >> hash;
            CWalletTx wtx(nullptr /* pwallet */, MakeTransactionRef());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            ssValue >> wtx;
            if (wtx.GetHash() != hash)
                return false;

<<<<<<< HEAD
            if (wtx.nOrderPos == -1)
                wss.fAnyUnordered = true;

            pwallet->AddToWallet(wtx, true);
        } else if (strType == "acentry") {
            string strAccount;
            ssKey >> strAccount;
            uint64_t nNumber;
            ssKey >> nNumber;
            if (nNumber > nAccountingEntryNumber)
                nAccountingEntryNumber = nNumber;

            if (!wss.fAnyUnordered) {
                CAccountingEntry acentry;
                ssValue >> acentry;
                if (acentry.nOrderPos == -1)
                    wss.fAnyUnordered = true;
            }
        } else if (strType == "watchs") {
=======
            // Undo serialize changes in 31600
            if (31404 <= wtx.fTimeReceivedIsTxTime && wtx.fTimeReceivedIsTxTime <= 31703) {
                if (!ssValue.empty()) {
                    char fTmp;
                    char fUnused;
                    std::string unused_string;
                    ssValue >> fTmp >> fUnused >> unused_string;
                    strErr = strprintf("LoadWallet() upgrading tx ver=%d %d %s",
                        wtx.fTimeReceivedIsTxTime, fTmp, hash.ToString());
                    wtx.fTimeReceivedIsTxTime = fTmp;
                } else {
                    strErr = strprintf("LoadWallet() repairing tx ver=%d %s", wtx.fTimeReceivedIsTxTime, hash.ToString());
                    wtx.fTimeReceivedIsTxTime = 0;
                }
                wss.vWalletUpgrade.push_back(hash);
            }

            if (wtx.nOrderPos == -1)
                wss.fAnyUnordered = true;

            pwallet->LoadToWallet(wtx);
        } else if (strType == DBKeys::WATCHS) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            CScript script;
            ssKey >> script;
            char fYes;
            ssValue >> fYes;
            if (fYes == '1')
                pwallet->LoadWatchOnly(script);

            // Watch-only addresses have no birthday information for now,
            // so set the wallet birthday to the beginning of time.
            pwallet->nTimeFirstKey = 1;
<<<<<<< HEAD
        } else if (strType == "key" || strType == "wkey") {
=======
        } else if (strType == DBKeys::KEY) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            CPubKey vchPubKey;
            ssKey >> vchPubKey;
            if (!vchPubKey.IsValid()) {
                strErr = "Error reading wallet database: CPubKey corrupt";
                return false;
            }
            CKey key;
            CPrivKey pkey;
<<<<<<< HEAD
            uint256 hash = 0;

            if (strType == "key") {
                wss.nKeys++;
                ssValue >> pkey;
            } else {
                CWalletKey wkey;
                ssValue >> wkey;
                pkey = wkey.vchPrivKey;
            }
=======
            uint256 hash;
            wss.nKeys++;
            ssValue >> pkey;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

            // Old wallets store keys as "key" [pubkey] => [privkey]
            // ... which was slow for wallets with lots of keys, because the public key is re-derived from the private key
            // using EC operations as a checksum.
            // Newer wallets store keys as "key"[pubkey] => [privkey][hash(pubkey,privkey)], which is much faster while
            // remaining backwards-compatible.
            try {
                ssValue >> hash;
            } catch (...) {
            }

            bool fSkipCheck = false;

<<<<<<< HEAD
            if (hash != 0) {
=======
            if (!hash.IsNull()) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                // hash pubkey/privkey to accelerate wallet load
                std::vector<unsigned char> vchKey;
                vchKey.reserve(vchPubKey.size() + pkey.size());
                vchKey.insert(vchKey.end(), vchPubKey.begin(), vchPubKey.end());
                vchKey.insert(vchKey.end(), pkey.begin(), pkey.end());

                if (Hash(vchKey.begin(), vchKey.end()) != hash) {
                    strErr = "Error reading wallet database: CPubKey/CPrivKey corrupt";
                    return false;
                }

                fSkipCheck = true;
            }

            if (!key.Load(pkey, vchPubKey, fSkipCheck)) {
                strErr = "Error reading wallet database: CPrivKey corrupt";
                return false;
            }
            if (!pwallet->LoadKey(key, vchPubKey)) {
                strErr = "Error reading wallet database: LoadKey failed";
                return false;
            }
<<<<<<< HEAD
        } else if (strType == "mkey") {
=======
        } else if (strType == DBKeys::MASTER_KEY) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            unsigned int nID;
            ssKey >> nID;
            CMasterKey kMasterKey;
            ssValue >> kMasterKey;
            if (pwallet->mapMasterKeys.count(nID) != 0) {
                strErr = strprintf("Error reading wallet database: duplicate CMasterKey id %u", nID);
                return false;
            }
            pwallet->mapMasterKeys[nID] = kMasterKey;
            if (pwallet->nMasterKeyMaxID < nID)
                pwallet->nMasterKeyMaxID = nID;
<<<<<<< HEAD
        } else if (strType == "ckey") {
            vector<unsigned char> vchPubKey;
            ssKey >> vchPubKey;
            vector<unsigned char> vchPrivKey;
=======
        } else if (strType == DBKeys::CRYPTED_KEY) {
            CPubKey vchPubKey;
            ssKey >> vchPubKey;
            std::vector<unsigned char> vchPrivKey;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            ssValue >> vchPrivKey;
            wss.nCKeys++;

            if (!pwallet->LoadCryptedKey(vchPubKey, vchPrivKey)) {
                strErr = "Error reading wallet database: LoadCryptedKey failed";
                return false;
            }
            wss.fIsEncrypted = true;
<<<<<<< HEAD
        } else if (strType == "keymeta") {
=======
        } else if (strType == DBKeys::KEYMETA) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            CPubKey vchPubKey;
            ssKey >> vchPubKey;
            CKeyMetadata keyMeta;
            ssValue >> keyMeta;
            wss.nKeyMeta++;

            pwallet->LoadKeyMetadata(vchPubKey, keyMeta);

            // find earliest key creation time, as wallet birthday
            if (!pwallet->nTimeFirstKey ||
                (keyMeta.nCreateTime < pwallet->nTimeFirstKey))
                pwallet->nTimeFirstKey = keyMeta.nCreateTime;
<<<<<<< HEAD
        } else if (strType == "defaultkey") {
            ssValue >> pwallet->vchDefaultKey;
        } else if (strType == "pool") {
=======
        } else if (strType == DBKeys::DEFAULTKEY) {
            // We don't want or need the default key, but if there is one set,
            // we want to make sure that it is valid so that we can detect corruption
            CPubKey vchPubKey;
            ssValue >> vchPubKey;
            if (!vchPubKey.IsValid()) {
                strErr = "Error reading wallet database: Default Key corrupt";
                return false;
            }
        } else if (strType == DBKeys::POOL) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            int64_t nIndex;
            ssKey >> nIndex;
            CKeyPool keypool;
            ssValue >> keypool;
<<<<<<< HEAD
            pwallet->setKeyPool.insert(nIndex);

            // If no metadata exists yet, create a default with the pool key's
            // creation time. Note that this may be overwritten by actually
            // stored metadata for that key later, which is fine.
            CKeyID keyid = keypool.vchPubKey.GetID();
            if (pwallet->mapKeyMetadata.count(keyid) == 0)
                pwallet->mapKeyMetadata[keyid] = CKeyMetadata(keypool.nTime);
        } else if (strType == "version") {
            ssValue >> wss.nFileVersion;
            if (wss.nFileVersion == 10300)
                wss.nFileVersion = 300;
        } else if (strType == "cscript") {
=======
            pwallet->GetScriptPubKeyMan()->LoadKeyPool(nIndex, keypool);
        } else if (strType == DBKeys::VERSION) {
            ssValue >> wss.nFileVersion;
            if (wss.nFileVersion == 10300)
                wss.nFileVersion = 300;
        } else if (strType == DBKeys::CSCRIPT) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            uint160 hash;
            ssKey >> hash;
            CScript script;
            ssValue >> script;
            if (!pwallet->LoadCScript(script)) {
                strErr = "Error reading wallet database: LoadCScript failed";
                return false;
            }
<<<<<<< HEAD
        } else if (strType == "orderposnext") {
            ssValue >> pwallet->nOrderPosNext;
        } else if (strType == "stakeSplitThreshold") //presstab HyperStake
        {
            ssValue >> pwallet->nStakeSplitThreshold;
        } else if (strType == "multisend") //presstab HyperStake
        {
            unsigned int i;
            ssKey >> i;
            std::pair<std::string, int> pMultiSend;
            ssValue >> pMultiSend;
            if (CBitcoinAddress(pMultiSend.first).IsValid()) {
                pwallet->vMultiSend.push_back(pMultiSend);
            }
        } else if (strType == "msettingsv2") //presstab HyperStake
        {
            std::pair<std::pair<bool, bool>, int> pSettings;
            ssValue >> pSettings;
            pwallet->fMultiSendStake = pSettings.first.first;
            pwallet->fMultiSendMasternodeReward = pSettings.first.second;
            pwallet->nLastMultiSendHeight = pSettings.second;
        } else if (strType == "mdisabled") //presstab HyperStake
        {
            std::string strDisabledAddress;
            ssValue >> strDisabledAddress;
            pwallet->vDisabledAddresses.push_back(strDisabledAddress);
        } else if (strType == "autocombinesettings") {
            std::pair<bool, CAmount> pSettings;
            ssValue >> pSettings;
            pwallet->fCombineDust = true;//pSettings.first;
            pwallet->nAutoCombineThreshold = 150;//pSettings.second;
        } else if (strType == "destdata") {
=======
        } else if (strType == DBKeys::ORDERPOSNEXT) {
            ssValue >> pwallet->nOrderPosNext;
        } else if (strType == DBKeys::STAKE_SPLIT_THRESHOLD) {
            ssValue >> pwallet->nStakeSplitThreshold;
            // originally saved as integer
            if (pwallet->nStakeSplitThreshold < COIN)
                pwallet->nStakeSplitThreshold *= COIN;
        } else if (strType == DBKeys::USE_CUSTOM_FEE) {
            ssValue >> pwallet->fUseCustomFee;
        } else if (strType == DBKeys::CUSTOM_FEE_VALUE) {
            ssValue >> pwallet->nCustomFee;
        } else if (strType == DBKeys::AUTOCOMBINE) {
            std::pair<bool, CAmount> pSettings;
            ssValue >> pSettings;
            pwallet->fCombineDust = pSettings.first;
            pwallet->nAutoCombineThreshold = pSettings.second;
            // originally saved as integer
            if (pwallet->nAutoCombineThreshold < COIN)
                pwallet->nAutoCombineThreshold *= COIN;
        } else if (strType == DBKeys::DESTDATA) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            std::string strAddress, strKey, strValue;
            ssKey >> strAddress;
            ssKey >> strKey;
            ssValue >> strValue;
<<<<<<< HEAD
            if (!pwallet->LoadDestData(CBitcoinAddress(strAddress).Get(), strKey, strValue)) {
                strErr = "Error reading wallet database: LoadDestData failed";
                return false;
            }
        } else if (strType == "hdchain") {
            CHDChain chain;
            ssValue >> chain;
            if (!pwallet->SetHDChain(chain, true))
            {
                strErr = "Error reading wallet database: SetHDChain failed";
                return false;
            }
        }
        else if (strType == "chdchain")
        {
            CHDChain chain;
            ssValue >> chain;
            if (!pwallet->SetCryptedHDChain(chain, true))
            {
                strErr = "Error reading wallet database: SetHDCryptedChain failed";
                return false;
            }
        }
        else if (strType == "hdpubkey")
        {
            CPubKey vchPubKey;
            ssKey >> vchPubKey;

            CHDPubKey hdPubKey;
            ssValue >> hdPubKey;

            if(vchPubKey != hdPubKey.extPubKey.pubkey)
            {
                strErr = "Error reading wallet database: CHDPubKey corrupt";
                return false;
            }
            if (!pwallet->LoadHDPubKey(hdPubKey))
            {
                strErr = "Error reading wallet database: LoadHDPubKey failed";
                return false;
            }
=======
            if (!pwallet->LoadDestData(DecodeDestination(strAddress), strKey, strValue)) {
                strErr = "Error reading wallet database: LoadDestData failed";
                return false;
            }
        } else if (strType == DBKeys::HDCHAIN) { // Regular key chain counter
            CHDChain chain;
            ssValue >> chain;
            pwallet->GetScriptPubKeyMan()->SetHDChain(chain, true);
        } else if (strType == DBKeys::SAP_HDCHAIN) {
            CHDChain chain;
            ssValue >> chain;
            pwallet->GetSaplingScriptPubKeyMan()->SetHDChain(chain, true);
        } else if (strType == DBKeys::SAP_KEY) {
            libzcash::SaplingIncomingViewingKey ivk;
            ssKey >> ivk;
            libzcash::SaplingExtendedSpendingKey key;
            ssValue >> key;
            if (!pwallet->LoadSaplingZKey(key)) {
                strErr = "Error reading wallet database: LoadSaplingZKey failed";
                return false;
            }
            //add checks for integrity
            wss.nZKeys++;
        } else if (strType == DBKeys::SAP_COMMON_OVK) {
            uint256 ovk;
            ssValue >> ovk;
            pwallet->GetSaplingScriptPubKeyMan()->setCommonOVK(ovk);
        } else if (strType == DBKeys::SAP_KEY_CRIPTED) {
            libzcash::SaplingIncomingViewingKey ivk;
            ssKey >> ivk;
            libzcash::SaplingExtendedFullViewingKey extfvk;
            ssValue >> extfvk;
            std::vector<unsigned char> vchCryptedSecret;
            ssValue >> vchCryptedSecret;
            wss.nCKeys++;

            if (!pwallet->LoadCryptedSaplingZKey(extfvk, vchCryptedSecret)) {
                strErr = "Error reading wallet database: LoadCryptedSaplingZKey failed";
                return false;
            }
            wss.fIsEncrypted = true;
        } else if (strType == DBKeys::SAP_KEYMETA) {
            libzcash::SaplingIncomingViewingKey ivk;
            ssKey >> ivk;
            CKeyMetadata keyMeta;
            ssValue >> keyMeta;

            wss.nZKeyMeta++;

            pwallet->LoadSaplingZKeyMetadata(ivk, keyMeta);
        } else if (strType == DBKeys::SAP_ADDR) {
            libzcash::SaplingPaymentAddress addr;
            ssKey >> addr;
            libzcash::SaplingIncomingViewingKey ivk;
            ssValue >> ivk;

            wss.nSapZAddrs++;

            if (!pwallet->LoadSaplingPaymentAddress(addr, ivk)) {
                strErr = "Error reading wallet database: LoadSaplingPaymentAddress failed";
                return false;
            }
        } else if (strType == DBKeys::SAP_WITNESS_CACHE_SIZE) {
            ssValue >> pwallet->GetSaplingScriptPubKeyMan()->nWitnessCacheSize;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
    } catch (...) {
        return false;
    }
    return true;
}

<<<<<<< HEAD
static bool IsKeyType(string strType)
{
    return (strType == "key" || strType == "wkey" ||
            strType == "mkey" || strType == "ckey");
}

DBErrors CWalletDB::LoadWallet(CWallet* pwallet)
{
    pwallet->vchDefaultKey = CPubKey();
=======
bool WalletBatch::IsKeyType(const std::string& strType)
{
    return (strType == DBKeys::KEY ||
            strType == DBKeys::MASTER_KEY || strType == DBKeys::CRYPTED_KEY ||
            strType == DBKeys::SAP_KEY || strType == DBKeys::SAP_KEY_CRIPTED);
}

DBErrors WalletBatch::LoadWallet(CWallet* pwallet)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CWalletScanState wss;
    bool fNoncriticalErrors = false;
    DBErrors result = DB_LOAD_OK;

    LOCK(pwallet->cs_wallet);
    try {
        int nMinVersion = 0;
<<<<<<< HEAD
        if (Read((string) "minversion", nMinVersion)) {
            if (nMinVersion > CLIENT_VERSION)
                return DB_TOO_NEW;
=======
        if (m_batch.Read((std::string) DBKeys::MINVERSION, nMinVersion)) {
            if (nMinVersion > CLIENT_VERSION) {
                return DB_TOO_NEW;
            }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            pwallet->LoadMinVersion(nMinVersion);
        }

        // Get cursor
<<<<<<< HEAD
        Dbc* pcursor = GetCursor();
=======
        Dbc* pcursor = m_batch.GetCursor();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!pcursor) {
            LogPrintf("Error getting wallet database cursor\n");
            return DB_CORRUPT;
        }

        while (true) {
            // Read next record
            CDataStream ssKey(SER_DISK, CLIENT_VERSION);
            CDataStream ssValue(SER_DISK, CLIENT_VERSION);
<<<<<<< HEAD
            int ret = ReadAtCursor(pcursor, ssKey, ssValue);
            if (ret == DB_NOTFOUND)
                break;
            else if (ret != 0) {
=======
            int ret = m_batch.ReadAtCursor(pcursor, ssKey, ssValue);
            if (ret == DB_NOTFOUND) {
                break;
            } else if (ret != 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                LogPrintf("Error reading next record from wallet database\n");
                return DB_CORRUPT;
            }

            // Try to be tolerant of single corrupt records:
<<<<<<< HEAD
            string strType, strErr;
            if (!ReadKeyValue(pwallet, ssKey, ssValue, wss, strType, strErr)) {
                // losing keys is considered a catastrophic error, anything else
                // we assume the user can live with:
                if (IsKeyType(strType))
                    result = DB_CORRUPT;
                else {
                    // Leave other errors alone, if we try to fix them we might make things worse.
                    fNoncriticalErrors = true; // ... but do warn the user there is something wrong.
                    if (strType == "tx")
                        // Rescan if there is a bad transaction record:
                        SoftSetBoolArg("-rescan", true);
=======
            std::string strType, strErr;
            if (!ReadKeyValue(pwallet, ssKey, ssValue, wss, strType, strErr)) {
                // losing keys is considered a catastrophic error, anything else
                // we assume the user can live with:
                if (IsKeyType(strType) || strType == DBKeys::DEFAULTKEY) {
                    result = DB_CORRUPT;
                } else {
                    // Leave other errors alone, if we try to fix them we might make things worse.
                    fNoncriticalErrors = true; // ... but do warn the user there is something wrong.
                    if (strType == DBKeys::TX)
                        // Rescan if there is a bad transaction record:
                        gArgs.SoftSetBoolArg("-rescan", true);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                }
            }
            if (!strErr.empty())
                LogPrintf("%s\n", strErr);
        }
        pcursor->close();
    } catch (const boost::thread_interrupted&) {
        throw;
    } catch (...) {
        result = DB_CORRUPT;
    }

    if (fNoncriticalErrors && result == DB_LOAD_OK)
        result = DB_NONCRITICAL_ERROR;

    // Any wallet corruption at all: skip any rewriting or
    // upgrading, we don't want to make it worse.
    if (result != DB_LOAD_OK)
        return result;

    LogPrintf("nFileVersion = %d\n", wss.nFileVersion);

    LogPrintf("Keys: %u plaintext, %u encrypted, %u w/ metadata, %u total\n",
        wss.nKeys, wss.nCKeys, wss.nKeyMeta, wss.nKeys + wss.nCKeys);

<<<<<<< HEAD
=======
    LogPrintf("ZKeys: %u plaintext, -- encrypted, %u w/metadata, %u total\n",
              wss.nZKeys, wss.nZKeyMeta, wss.nZKeys + 0);

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    // nTimeFirstKey is only reliable if all keys have metadata
    if ((wss.nKeys + wss.nCKeys) != wss.nKeyMeta)
        pwallet->nTimeFirstKey = 1; // 0 would be considered 'no value'

<<<<<<< HEAD
    for (uint256 hash : wss.vWalletUpgrade)
        WriteTx(hash, pwallet->mapWallet[hash]);

    // Rewrite encrypted wallets of versions 0.4.0 and 0.5.0rc:
    if (wss.fIsEncrypted && (wss.nFileVersion == 40000 || wss.nFileVersion == 50000))
        return DB_NEED_REWRITE;

    if (wss.nFileVersion < CLIENT_VERSION) // Update
        WriteVersion(CLIENT_VERSION);

    if (wss.fAnyUnordered)
        result = ReorderTransactions(pwallet);

    pwallet->laccentries.clear();
    ListAccountCreditDebit("*", pwallet->laccentries);
    for(CAccountingEntry& entry : pwallet->laccentries) {
        pwallet->wtxOrdered.insert(make_pair(entry.nOrderPos, CWallet::TxPair((CWalletTx*)0, &entry)));
=======
    for (const uint256& hash : wss.vWalletUpgrade) {
        WriteTx(pwallet->mapWallet.at(hash));
    }

    // Rewrite encrypted wallets of versions 0.4.0 and 0.5.0rc:
    if (wss.fIsEncrypted && (wss.nFileVersion == 40000 || wss.nFileVersion == 50000)) {
        return DB_NEED_REWRITE;
    }

    if (wss.nFileVersion < CLIENT_VERSION) { // Update
        WriteVersion(CLIENT_VERSION);
    }

    if (wss.fAnyUnordered) {
        result = ReorderTransactions(pwallet);
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return result;
}

<<<<<<< HEAD
DBErrors CWalletDB::FindWalletTx(CWallet* pwallet, vector<uint256>& vTxHash, vector<CWalletTx>& vWtx)
{
    pwallet->vchDefaultKey = CPubKey();
=======
DBErrors WalletBatch::FindWalletTx(CWallet* pwallet, std::vector<uint256>& vTxHash, std::vector<CWalletTx>& vWtx)
{
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    bool fNoncriticalErrors = false;
    DBErrors result = DB_LOAD_OK;

    try {
        LOCK(pwallet->cs_wallet);
        int nMinVersion = 0;
<<<<<<< HEAD
        if (Read((string) "minversion", nMinVersion)) {
            if (nMinVersion > CLIENT_VERSION)
                return DB_TOO_NEW;
=======
        if (m_batch.Read((std::string) DBKeys::MINVERSION, nMinVersion)) {
            if (nMinVersion > CLIENT_VERSION) {
                return DB_TOO_NEW;
            }
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            pwallet->LoadMinVersion(nMinVersion);
        }

        // Get cursor
<<<<<<< HEAD
        Dbc* pcursor = GetCursor();
=======
        Dbc* pcursor = m_batch.GetCursor();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        if (!pcursor) {
            LogPrintf("Error getting wallet database cursor\n");
            return DB_CORRUPT;
        }

        while (true) {
            // Read next record
            CDataStream ssKey(SER_DISK, CLIENT_VERSION);
            CDataStream ssValue(SER_DISK, CLIENT_VERSION);
<<<<<<< HEAD
            int ret = ReadAtCursor(pcursor, ssKey, ssValue);
            if (ret == DB_NOTFOUND)
                break;
            else if (ret != 0) {
=======
            int ret = m_batch.ReadAtCursor(pcursor, ssKey, ssValue);
            if (ret == DB_NOTFOUND) {
                break;
            } else if (ret != 0) {
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                LogPrintf("Error reading next record from wallet database\n");
                return DB_CORRUPT;
            }

<<<<<<< HEAD
            string strType;
            ssKey >> strType;
            if (strType == "tx") {
                uint256 hash;
                ssKey >> hash;

                CWalletTx wtx;
=======
            std::string strType;
            ssKey >> strType;
            if (strType == DBKeys::TX) {
                uint256 hash;
                ssKey >> hash;

                CWalletTx wtx(nullptr /* pwallet */, MakeTransactionRef());
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                ssValue >> wtx;

                vTxHash.push_back(hash);
                vWtx.push_back(wtx);
            }
        }
        pcursor->close();
    } catch (const boost::thread_interrupted&) {
        throw;
    } catch (...) {
        result = DB_CORRUPT;
    }

    if (fNoncriticalErrors && result == DB_LOAD_OK)
        result = DB_NONCRITICAL_ERROR;

    return result;
}

<<<<<<< HEAD
DBErrors CWalletDB::ZapWalletTx(CWallet* pwallet, vector<CWalletTx>& vWtx)
{
    // build list of wallet TXs
    vector<uint256> vTxHash;
    DBErrors err = FindWalletTx(pwallet, vTxHash, vWtx);
    if (err != DB_LOAD_OK)
        return err;

    // erase each wallet TX
    for (uint256& hash : vTxHash) {
        if (!EraseTx(hash))
            return DB_CORRUPT;
=======
DBErrors WalletBatch::ZapWalletTx(CWallet* pwallet, std::vector<CWalletTx>& vWtx)
{
    // build list of wallet TXs
    std::vector<uint256> vTxHash;
    DBErrors err = FindWalletTx(pwallet, vTxHash, vWtx);
    if (err != DB_LOAD_OK) {
        return err;
    }

    // erase each wallet TX
    for (uint256& hash : vTxHash) {
        if (!EraseTx(hash)) return DB_CORRUPT;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    }

    return DB_LOAD_OK;
}

<<<<<<< HEAD
void ThreadFlushWalletDB(const string& strFile)
{
    // Make this thread recognisable as the wallet flushing thread
    util::ThreadRename("prcycoin-wallet");

    static bool fOneThread;
    if (fOneThread)
        return;
    fOneThread = true;
    if (!GetBoolArg("-flushwallet", true))
        return;

    unsigned int nLastSeen = nWalletDBUpdated;
    unsigned int nLastFlushed = nWalletDBUpdated;
    int64_t nLastWalletUpdate = GetTime();
    while (true) {
        MilliSleep(500);

        if (nLastSeen != nWalletDBUpdated) {
            nLastSeen = nWalletDBUpdated;
            nLastWalletUpdate = GetTime();
        }

        if (nLastFlushed != nWalletDBUpdated && GetTime() - nLastWalletUpdate >= 2) {
            TRY_LOCK(bitdb.cs_db, lockDb);
            if (lockDb) {
                // Don't do this if any databases are in use
                int nRefCount = 0;
                map<string, int>::iterator mi = bitdb.mapFileUseCount.begin();
                while (mi != bitdb.mapFileUseCount.end()) {
                    nRefCount += (*mi).second;
                    mi++;
                }

                if (nRefCount == 0) {
                    boost::this_thread::interruption_point();
                    map<string, int>::iterator mi = bitdb.mapFileUseCount.find(strFile);
                    if (mi != bitdb.mapFileUseCount.end()) {
                        LogPrint("db", "Flushing wallet.dat\n");
                        nLastFlushed = nWalletDBUpdated;
                        int64_t nStart = GetTimeMillis();

                        // Flush wallet.dat so it's self contained
                        bitdb.CloseDb(strFile);
                        bitdb.CheckpointLSN(strFile);

                        bitdb.mapFileUseCount.erase(mi++);
                        LogPrint("db", "Flushed wallet.dat %dms\n", GetTimeMillis() - nStart);
                    }
                }
            }
        }
    }
}

bool BackupWallet(const CWallet& wallet, const filesystem::path& strDest, bool fEnableCustom)
{
    filesystem::path pathCustom;
    filesystem::path pathWithFile;
    if (!wallet.fFileBacked) {
        return false;
    } else if(fEnableCustom) {
        pathWithFile = GetArg("-backuppath", "");
        if(!pathWithFile.empty()) {
            if(!pathWithFile.has_extension()) {
                pathCustom = pathWithFile;
                filesystem::create_directories(pathCustom);
                if(access(pathCustom.string().data(), W_OK) != 0) {
                    string msg = strprintf("Error: failed to backup wallet to %s - Access denied\n", pathCustom.string());
                    LogPrintf(msg.data());
                    pathCustom = "";
                    wallet.NotifyWalletBacked(false, msg);
                } else {
                    pathWithFile /= wallet.GetUniqueWalletBackupName();
                }
            } else {
                pathCustom = pathWithFile.parent_path();
            }
        }
    }

    while (true) {
        {
            LOCK(bitdb.cs_db);
            if (!bitdb.mapFileUseCount.count(wallet.strWalletFile) || bitdb.mapFileUseCount[wallet.strWalletFile] == 0) {
                // Flush log data to the dat file
                bitdb.CloseDb(wallet.strWalletFile);
                bitdb.CheckpointLSN(wallet.strWalletFile);
                bitdb.mapFileUseCount.erase(wallet.strWalletFile);

                // Copy wallet.dat
                filesystem::path pathDest(strDest);
                filesystem::path pathSrc = GetDataDir() / wallet.strWalletFile;
                if (is_directory(pathDest)) {
                    if(!exists(pathDest)) create_directory(pathDest);
                    pathDest /= wallet.strWalletFile;
                }
                bool defaultPath = AttemptBackupWallet(wallet, pathSrc.string(), pathDest.string());

                if(defaultPath && !pathCustom.empty()) {
                    string strThreshold = GetArg("-custombackupthreshold", "");
                    int nThreshold = 0;
                    if (strThreshold != "") {
                        nThreshold = atoi(strThreshold);

                        typedef std::multimap<std::time_t, filesystem::path> folder_set_t;
                        folder_set_t folderSet;
                        filesystem::directory_iterator end_iter;

                        pathCustom.make_preferred();
                        // Build map of backup files for current(!) wallet sorted by last write time

                        filesystem::path currentFile;
                        for (filesystem::directory_iterator dir_iter(pathCustom); dir_iter != end_iter; ++dir_iter) {
                            // Only check regular files
                            if (filesystem::is_regular_file(dir_iter->status())) {
                                currentFile = dir_iter->path().filename();
                                // Only add the backups for the current wallet, e.g. wallet.dat.*
                                if (dir_iter->path().stem().string() == wallet.strWalletFile) {
                                    folderSet.insert(folder_set_t::value_type(filesystem::last_write_time(dir_iter->path()), *dir_iter));
                                }
                            }
                        }

                        int counter = 0;
                        for (auto entry : folderSet) {
                            counter++;
                            if(entry.second == pathWithFile) {
                                pathWithFile += "(1)";
                            }
                        }

                        if (counter >= nThreshold) {
                            std::time_t oldestBackup = 0;
                            for(auto entry : folderSet) {
                                if(oldestBackup == 0 || entry.first < oldestBackup) {
                                    oldestBackup = entry.first;
                                }
                            }

                            try {
                                auto entry = folderSet.find(oldestBackup);
                                if (entry != folderSet.end()) {
                                    filesystem::remove(entry->second);
                                    LogPrintf("Old backup deleted: %s\n", (*entry).second);
                                }
                            } catch (filesystem::filesystem_error& error) {
                                LogPrintf("Failed to delete backup %s\n", error.what());
                            }
                        }
                    }
                    AttemptBackupWallet(wallet, pathSrc.string(), pathWithFile.string());
                }

                return defaultPath;
            }
        }
        MilliSleep(100);
    }
    return false;
}

bool AttemptBackupWallet(const CWallet& wallet, const filesystem::path& pathSrc, const filesystem::path& pathDest)
{
    bool retStatus;
    try {
#if BOOST_VERSION >= 105800 /* BOOST_LIB_VERSION 1_58 */
        filesystem::copy_file(pathSrc, pathDest, filesystem::copy_option::overwrite_if_exists);
#else
        std::ifstream src(pathSrc,  std::ios::binary | std::ios::in);
        std::ofstream dst(pathDest, std::ios::binary | std::ios::out | std::ios::trunc);
        dst << src.rdbuf();
        dst.flush();
        src.close();
        dst.close();
#endif
        LogPrintf("copied wallet.dat to %s\n", pathDest.string());
        retStatus = true;
    } catch (const filesystem::filesystem_error& e) {
        retStatus = false;
        LogPrintf("error copying wallet.dat to %s - %s\n", pathDest, e.what());
    }
    wallet.NotifyWalletBacked(retStatus, pathDest.string());
    return retStatus;
}

//
// Try to (very carefully!) recover wallet.dat if there is a problem.
//
bool CWalletDB::Recover(CDBEnv& dbenv, std::string filename, bool fOnlyKeys)
{
    // Recovery procedure:
    // move wallet.dat to wallet.timestamp.bak
    // Call Salvage with fAggressive=true to
    // get as much data as possible.
    // Rewrite salvaged data to wallet.dat
    // Set -rescan so any missing transactions will be
    // found.
    int64_t now = GetTime();
    std::string newFilename = strprintf("wallet.%d.bak", now);

    int result = dbenv.dbenv.dbrename(NULL, filename.c_str(), NULL,
        newFilename.c_str(), DB_AUTO_COMMIT);
    if (result == 0)
        LogPrintf("Renamed %s to %s\n", filename, newFilename);
    else {
        LogPrintf("Failed to rename %s to %s\n", filename, newFilename);
        return false;
    }

    std::vector<CDBEnv::KeyValPair> salvagedData;
    bool allOK = dbenv.Salvage(newFilename, true, salvagedData);
    if (salvagedData.empty()) {
        LogPrintf("Salvage(aggressive) found no records in %s.\n", newFilename);
        return false;
    }
    LogPrintf("Salvage(aggressive) found %u records\n", salvagedData.size());

    bool fSuccess = allOK;
    boost::scoped_ptr<Db> pdbCopy(new Db(&dbenv.dbenv, 0));
    int ret = pdbCopy->open(NULL, // Txn pointer
        filename.c_str(),         // Filename
        "main",                   // Logical db name
        DB_BTREE,                 // Database type
        DB_CREATE,                // Flags
        0);
    if (ret > 0) {
        LogPrintf("Cannot create database file %s\n", filename);
        return false;
    }
    CWallet dummyWallet;
    CWalletScanState wss;

    DbTxn* ptxn = dbenv.TxnBegin();
    for (CDBEnv::KeyValPair& row : salvagedData) {
        if (fOnlyKeys) {
            CDataStream ssKey(row.first, SER_DISK, CLIENT_VERSION);
            CDataStream ssValue(row.second, SER_DISK, CLIENT_VERSION);
            string strType, strErr;
            bool fReadOK = ReadKeyValue(&dummyWallet, ssKey, ssValue,
                wss, strType, strErr);
            if (!IsKeyType(strType))
                continue;
            if (!fReadOK) {
                LogPrintf("WARNING: CWalletDB::Recover skipping %s: %s\n", strType, strErr);
                continue;
            }
        }
        Dbt datKey(&row.first[0], row.first.size());
        Dbt datValue(&row.second[0], row.second.size());
        int ret2 = pdbCopy->put(ptxn, &datKey, &datValue, DB_NOOVERWRITE);
        if (ret2 > 0)
            fSuccess = false;
    }
    ptxn->commit(0);
    pdbCopy->close(0);

    return fSuccess;
}

bool CWalletDB::Recover(CDBEnv& dbenv, std::string filename)
{
    return CWalletDB::Recover(dbenv, filename, false);
}

bool CWalletDB::WriteDestData(const std::string& address, const std::string& key, const std::string& value)
{
    nWalletDBUpdated++;
    return Write(std::make_pair(std::string("destdata"), std::make_pair(address, key)), value);
}

bool CWalletDB::WriteTxPrivateKey(const std::string& outpointKey, const std::string& k)
{
    return Write(std::make_pair(std::string("txpriv"), outpointKey), k);
}

bool CWalletDB::ReadTxPrivateKey(const std::string& outpointKey, std::string& k)
{
    return Read(std::make_pair(std::string("txpriv"), outpointKey), k);
}

bool CWalletDB::WriteKeyImage(const std::string& outpointKey, const CKeyImage& k)
{
    return Write(std::make_pair(std::string("outpointkeyimage"), outpointKey), k);
}
bool CWalletDB::ReadKeyImage(const std::string& outpointKey, CKeyImage& k)
{
    return Read(std::make_pair(std::string("outpointkeyimage"), outpointKey), k);
}


bool CWalletDB::EraseDestData(const std::string& address, const std::string& key)
{
    nWalletDBUpdated++;
    return Erase(std::make_pair(std::string("destdata"), std::make_pair(address, key)));
}

bool CWalletDB::WriteHDChain(const CHDChain& chain)
{
    nWalletDBUpdated++;
    return Write(std::string("hdchain"), chain);
}

bool CWalletDB::WriteCryptedHDChain(const CHDChain& chain)
{
    nWalletDBUpdated++;

    if (!Write(std::string("chdchain"), chain))
        return false;

    Erase(std::string("hdchain"));

    return true;
}


bool CWalletDB::WriteHDPubKey(const CHDPubKey& hdPubKey, const CKeyMetadata& keyMeta)
{
    nWalletDBUpdated++;

    if (!Write(std::make_pair(std::string("keymeta"), hdPubKey.extPubKey.pubkey), keyMeta, false))
        return false;

    return Write(std::make_pair(std::string("hdpubkey"), hdPubKey.extPubKey.pubkey), hdPubKey, false);
}

// Just get the Serial Numbers
std::list<CBigNum> CWalletDB::ListMintedCoinsSerial()
{
    std::list<CBigNum> listPubCoin;
    
    return listPubCoin;
}

// Just get the Serial Numbers
std::list<CBigNum> CWalletDB::ListSpentCoinsSerial()
{
    std::list<CBigNum> listPubCoin;
    
    return listPubCoin;
}


=======
void MaybeCompactWalletDB()
{
    static std::atomic<bool> fOneThread;
    if (fOneThread.exchange(true)) {
        return;
    }
    if (!gArgs.GetBoolArg("-flushwallet", DEFAULT_FLUSHWALLET)) {
        return;
    }

    for (CWalletRef pwallet : vpwallets) {
        WalletDatabase& dbh = pwallet->GetDBHandle();

        unsigned int nUpdateCounter = dbh.nUpdateCounter;
        if (dbh.nLastSeen != nUpdateCounter) {
            dbh.nLastSeen = nUpdateCounter;
            dbh.nLastWalletUpdate = GetTime();
        }

        if (dbh.nLastFlushed != nUpdateCounter && GetTime() - dbh.nLastWalletUpdate >= 2) {
            if (BerkeleyBatch::PeriodicFlush(dbh)) {
                dbh.nLastFlushed = nUpdateCounter;
            }
        }
    }

    fOneThread = false;
}

typedef std::multimap<std::time_t, fs::path> folder_set_t;
static folder_set_t buildBackupsMapSortedByLastWrite(const std::string& strWalletFile, const fs::path& backupsDir)
{
    folder_set_t folder_set;
    fs::directory_iterator end_iter;
    // Build map of backup files for current(!) wallet sorted by last write time
    for (fs::directory_iterator dir_iter(backupsDir); dir_iter != end_iter; ++dir_iter) {
        // Only check regular files
        if (fs::is_regular_file(dir_iter->status())) {
            // Only add the backups for the current wallet, e.g. wallet.dat.*
            if(dir_iter->path().stem().string() == strWalletFile) {
                folder_set.insert(folder_set_t::value_type(fs::last_write_time(dir_iter->path()), *dir_iter));
            }
        }
    }
    return folder_set;
}

static bool cleanWalletBackups(folder_set_t& folder_set, int nWalletBackups, std::string& strBackupWarning)
{
    // Loop backward through backup files and keep the N newest ones (1 <= N <= 10)
    int counter = 0;
    for (const std::pair<const std::time_t, fs::path>& file : reverse_iterate(folder_set)) {
        counter++;
        if (counter > nWalletBackups) {
            // More than nWalletBackups backups: delete oldest one(s)
            try {
                fs::remove(file.second);
                LogPrintf("Old backup deleted: %s\n", file.second);
            } catch (fs::filesystem_error &error) {
                strBackupWarning = strprintf(_("Failed to delete backup, error: %s"), error.what());
                LogPrintf("%s\n", strBackupWarning);
                return false;
            }
        }
    }
    return true;
}

bool AutoBackupWallet(CWallet& wallet, std::string& strBackupWarning, std::string& strBackupError)
{
    strBackupWarning = strBackupError = "";
    int nWalletBackups = std::max(0, std::min(10, (int)gArgs.GetArg("-createwalletbackups", DEFAULT_CREATEWALLETBACKUPS)));
    if (nWalletBackups == 0) {
        LogPrintf("Automatic wallet backups are disabled!\n");
        return false;
    }

    fs::path backupsDir = GetDataDir() / "backups";
    backupsDir.make_preferred();
    TryCreateDirectories(backupsDir);
    std::string strWalletFile = wallet.GetUniqueWalletBackupName();
    fs::path backupFile = backupsDir / strWalletFile;
    backupFile.make_preferred();
    if (fs::exists(backupFile)) {
        LogPrintf("%s\n", _("Failed to create backup, file already exists! This could happen if you restarted wallet in less than 60 seconds. You can continue if you are ok with this."));
        return false;
    }

    // Try to backup
    if (!wallet.BackupWallet(backupFile.string())) {
        strBackupError = "Failed to backup wallet";
        return false; // error is logged internally
    }

    // Keep only 0 < nWalletBackups <= 10 backups, including the new one of course
    folder_set_t folder_set = buildBackupsMapSortedByLastWrite(backupFile.stem().string(), backupsDir);
    return cleanWalletBackups(folder_set, nWalletBackups, strBackupWarning);
}

//
// Try to (very carefully!) recover wallet file if there is a problem.
//
bool WalletBatch::Recover(const fs::path& wallet_path, void *callbackDataIn, bool (*recoverKVcallback)(void* callbackData, CDataStream ssKey, CDataStream ssValue), std::string& out_backup_filename)
{
    return BerkeleyBatch::Recover(wallet_path, callbackDataIn, recoverKVcallback, out_backup_filename);
}

bool WalletBatch::Recover(const fs::path& wallet_path, std::string& out_backup_filename)
{
    // recover without a key filter callback
    // results in recovering all record types
    return WalletBatch::Recover(wallet_path, nullptr, nullptr, out_backup_filename);
}

bool WalletBatch::RecoverKeysOnlyFilter(void *callbackData, CDataStream ssKey, CDataStream ssValue)
{
    CWallet *dummyWallet = reinterpret_cast<CWallet*>(callbackData);
    CWalletScanState dummyWss;
    std::string strType, strErr;
    bool fReadOK;
    {
        // Required in LoadKeyMetadata():
        LOCK(dummyWallet->cs_wallet);
        fReadOK = ReadKeyValue(dummyWallet, ssKey, ssValue,
                               dummyWss, strType, strErr);
    }
    if (!IsKeyType(strType) && strType != DBKeys::HDCHAIN)
        return false;
    if (!fReadOK)
    {
        LogPrintf("WARNING: WalletBatch::Recover skipping %s: %s\n", strType, strErr);
        return false;
    }

    return true;
}

bool WalletBatch::VerifyEnvironment(const fs::path& wallet_path, std::string& errorStr)
{
    return BerkeleyBatch::VerifyEnvironment(wallet_path, errorStr);
}

bool WalletBatch::VerifyDatabaseFile(const fs::path& wallet_path, std::string& warningStr, std::string& errorStr)
{
    return BerkeleyBatch::VerifyDatabaseFile(wallet_path, warningStr, errorStr, WalletBatch::Recover);
}

bool WalletBatch::WriteDestData(const std::string& address, const std::string& key, const std::string& value)
{
    return WriteIC(std::make_pair(std::string(DBKeys::DESTDATA), std::make_pair(address, key)), value);
}

bool WalletBatch::EraseDestData(const std::string& address, const std::string& key)
{
    return EraseIC(std::make_pair(std::string(DBKeys::DESTDATA), std::make_pair(address, key)));
}

bool WalletBatch::TxnBegin()
{
    return m_batch.TxnBegin();
}

bool WalletBatch::TxnCommit()
{
    return m_batch.TxnCommit();
}

bool WalletBatch::TxnAbort()
{
    return m_batch.TxnAbort();
}

bool WalletBatch::ReadVersion(int& nVersion)
{
    return m_batch.ReadVersion(nVersion);
}

bool WalletBatch::WriteVersion(int nVersion)
{
    return m_batch.WriteVersion(nVersion);
}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
