// Copyright (c) 2012-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
<<<<<<< HEAD
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2020 The DAPS Project developers
=======
// Copyright (c) 2015-2020 The PIVX developers
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VERSION_H
#define BITCOIN_VERSION_H

/**
 * network protocol versioning
 */

<<<<<<< HEAD
static const int PROTOCOL_VERSION = 70915;
=======
static const int PROTOCOL_VERSION = 70924;
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209;

<<<<<<< HEAD
//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 70077;

//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION_BEFORE_ENFORCEMENT = 70915;
static const int MIN_PEER_PROTO_VERSION_AFTER_ENFORCEMENT = 70915;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31402;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 60000;

//! "mempool" command, enhanced "getdata" behavior starts with this version
static const int MEMPOOL_GD_VERSION = 60002;

//! "filter*" commands are disabled without NODE_BLOOM after and including this version
static const int NO_BLOOM_VERSION = 70005;

=======
//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION_BEFORE_ENFORCEMENT = 70922;
static const int MIN_PEER_PROTO_VERSION_AFTER_ENFORCEMENT = 70923;

//! Version where BIP155 was introduced
static const int MIN_BIP155_PROTOCOL_VERSION = 70923;

// Make sure that none of the values above collide with
// `ADDRV2_FORMAT`.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

#endif // BITCOIN_VERSION_H
