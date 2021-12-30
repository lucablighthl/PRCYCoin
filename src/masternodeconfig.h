// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
=======
// Copyright (c) 2015-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#ifndef SRC_MASTERNODECONFIG_H_
#define SRC_MASTERNODECONFIG_H_

<<<<<<< HEAD
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

=======
#include "sync.h"
#include <string>
#include <vector>

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class CMasternodeConfig;
extern CMasternodeConfig masternodeConfig;

class CMasternodeConfig
{
public:
    class CMasternodeEntry
    {
    private:
        std::string alias;
        std::string ip;
        std::string privKey;
        std::string txHash;
        std::string outputIndex;

    public:
<<<<<<< HEAD
        CMasternodeEntry(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex)
        {
            this->alias = alias;
            this->ip = ip;
            this->privKey = privKey;
            this->txHash = txHash;
            this->outputIndex = outputIndex;
        }

        const std::string& getAlias() const
        {
            return alias;
        }

        void setAlias(const std::string& alias)
        {
            this->alias = alias;
        }

        const std::string& getOutputIndex() const
        {
            return outputIndex;
        }

        bool castOutputIndex(int& n);

        void setOutputIndex(const std::string& outputIndex)
        {
            this->outputIndex = outputIndex;
        }

        const std::string& getPrivKey() const
        {
            return privKey;
        }

        void setPrivKey(const std::string& privKey)
        {
            this->privKey = privKey;
        }

        const std::string& getTxHash() const
        {
            return txHash;
        }

        void setTxHash(const std::string& txHash)
        {
            this->txHash = txHash;
        }

        const std::string& getIp() const
        {
            return ip;
        }

        void setIp(const std::string& ip)
        {
            this->ip = ip;
        }
    };

    CMasternodeConfig()
    {
        entries = std::vector<CMasternodeEntry>();
    }

    void clear();
    bool read(std::string& strErr);
    void add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex);

    std::vector<CMasternodeEntry>& getEntries()
    {
        return entries;
    }

    int getCount()
    {
        int c = -1;
        for (CMasternodeEntry e : entries) {
            if (e.getAlias() != "") c++;
=======
        CMasternodeEntry(std::string& _alias, std::string& _ip, std::string& _privKey, std::string& _txHash, std::string& _outputIndex) :
            alias(_alias), ip(_ip), privKey(_privKey), txHash(_txHash), outputIndex(_outputIndex) { }

        const std::string& getAlias() const { return alias; }
        const std::string& getOutputIndex() const { return outputIndex; }
        bool castOutputIndex(int& n) const;
        const std::string& getPrivKey() const { return privKey; }
        const std::string& getTxHash() const { return txHash; }
        const std::string& getIp() const { return ip; }
    };

    CMasternodeConfig() { entries = std::vector<CMasternodeEntry>(); }

    void clear() { LOCK(cs_entries); entries.clear(); }
    bool read(std::string& strErr);
    CMasternodeConfig::CMasternodeEntry* add(std::string alias, std::string ip, std::string privKey, std::string txHash, std::string outputIndex);
    void remove(std::string alias);

    std::vector<CMasternodeEntry> getEntries() { LOCK(cs_entries); return entries; }

    int getCount()
    {
        LOCK(cs_entries);
        int c = -1;
        for (const auto& e : entries) {
            if (!e.getAlias().empty()) c++;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        }
        return c;
    }

private:
    std::vector<CMasternodeEntry> entries;
<<<<<<< HEAD
=======
    Mutex cs_entries;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
};


#endif /* SRC_MASTERNODECONFIG_H_ */
