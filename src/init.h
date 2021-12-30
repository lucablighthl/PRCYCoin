// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
<<<<<<< HEAD
=======
// Copyright (c) 2017-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_INIT_H
#define BITCOIN_INIT_H

#include <string>
<<<<<<< HEAD
=======

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
class CScheduler;
class CWallet;

namespace boost
{
class thread_group;
} // namespace boost

<<<<<<< HEAD
extern CWallet* pwalletMain;

void StartShutdown();
bool ShutdownRequested();

/** Interrupt threads */
void Interrupt();
void Shutdown();
void PrepareShutdown();
=======
/** Interrupt threads */
void Interrupt();
void Shutdown();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
//!Initialize the logging infrastructure
void InitLogging();
//!Parameter interaction: change current parameters depending on various rules
void InitParameterInteraction();
<<<<<<< HEAD
bool AppInit2(bool isDaemon);

/** Initialize PRCY: Basic context setup.
=======

/** Initialize PIVX core: Basic context setup.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
 *  @note This can be done before daemonization. Do not call Shutdown() if this function fails.
 *  @pre Parameters should be parsed and config file should be read.
 */
bool AppInitBasicSetup();
<<<<<<< HEAD
=======
/**
 * Initialization: parameter interaction.
 * @note This can be done before daemonization.
 * @pre Parameters should be parsed and config file should be read, AppInitBasicSetup should have been called.
 */
bool AppInitParameterInteraction();
/**
 * Initialization sanity checks: ecc init, sanity checks, dir lock.
 * @note This can be done before daemonization.
 * @pre Parameters should be parsed and config file should be read, AppInitParameterInteraction should have been called.
 */
bool AppInitSanityChecks();
/**
 * Bitcoin core main initialization.
 * @note This should only be done after daemonization.
 * @pre Parameters should be parsed and config file should be read, AppInitSanityChecks should have been called.
 */
bool AppInitMain();
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

/** The help message mode determines what help message to show */
enum HelpMessageMode {
    HMM_BITCOIND,
    HMM_BITCOIN_QT
};

/** Help for options shared between UI and daemon (for -help) */
std::string HelpMessage(HelpMessageMode mode);
/** Returns licensing information (for -version) */
std::string LicenseInfo();

#endif // BITCOIN_INIT_H
