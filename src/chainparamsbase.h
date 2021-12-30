<<<<<<< HEAD
// Copyright (c) 2014 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMSBASE_H
#define BITCOIN_CHAINPARAMSBASE_H

=======
// Copyright (c) 2014-2021 The Bitcoin developers
// Copyright (c) 2017-2021 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef PIVX_CHAINPARAMSBASE_H
#define PIVX_CHAINPARAMSBASE_H

#include <memory>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#include <string>
#include <vector>

/**
<<<<<<< HEAD
 * CBaseChainParams defines the base parameters (shared between prcycoin-cli and prcycoind)
 * of a given instance of the Prcycoin system.
=======
 * CBaseChainParams defines the base parameters (shared between pivx-cli and pivxd)
 * of a given instance of the Pivx system.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 */
class CBaseChainParams
{
public:
<<<<<<< HEAD
    enum Network {
        MAIN,
        TESTNET,
        REGTEST,

        MAX_NETWORK_TYPES
    };
=======
    ///@{
    /** Chain name strings */
    static const std::string MAIN;
    static const std::string TESTNET;
    static const std::string REGTEST;
    ///@}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    const std::string& DataDir() const { return strDataDir; }
    int RPCPort() const { return nRPCPort; }

<<<<<<< HEAD
protected:
    CBaseChainParams() {}

    int nRPCPort;
    std::string strDataDir;
    Network networkID;
};

/**
 * Return the currently selected parameters. This won't change after app startup
 * outside of the unit tests.
 */
const CBaseChainParams& BaseParams();

/** Sets the params returned by Params() to those for the given network. */
void SelectBaseParams(CBaseChainParams::Network network);

/**
 * Looks for -regtest or -testnet and returns the appropriate Network ID.
 * Returns MAX_NETWORK_TYPES if an invalid combination is given.
 */
CBaseChainParams::Network NetworkIdFromCommandLine();

/**
 * Calls NetworkIdFromCommandLine() and then calls SelectParams as appropriate.
 * Returns false if an invalid combination is given.
 */
bool SelectBaseParamsFromCommandLine();

/**
 * Return true if SelectBaseParamsFromCommandLine() has been called to select
 * a network.
 */
bool AreBaseParamsConfigured();

#endif // BITCOIN_CHAINPARAMSBASE_H
=======
    CBaseChainParams() = delete;
    CBaseChainParams(const std::string& data_dir, int rpc_port) : nRPCPort(rpc_port), strDataDir(data_dir) {}

private:
    int nRPCPort;
    std::string strDataDir;
};

/**
 * Creates and returns a std::unique_ptr<CBaseChainParams> of the chosen chain.
 * @returns a CBaseChainParams* of the chosen chain.
 * @throws a std::runtime_error if the chain is not supported.
 */
std::unique_ptr<CBaseChainParams> CreateBaseChainParams(const std::string& chain);

/**
 * Append the help messages for the chainparams options to the
 * parameter string.
 */
void AppendParamsHelpMessages(std::string& strUsage, bool debugHelp=true);

/**
 * Return the currently selected parameters. This won't change after app startup
 * startup, except for unit tests.
 */
const CBaseChainParams& BaseParams();

/** Sets the params returned by Params() to those for the given network. */
void SelectBaseParams(const std::string& chain);

/**
 * Returns the appropriate chain name from the program arguments.
 * @return CBaseChainParams::MAX_NETWORK_TYPES if an invalid combination is given. CBaseChainParams::MAIN by default.
 */
std::string ChainNameFromCommandLine();

#endif // PIVX_CHAINPARAMSBASE_H
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
