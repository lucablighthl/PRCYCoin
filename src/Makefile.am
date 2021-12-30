# Copyright (c) 2013-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
DIST_SUBDIRS = secp256k1 secp256k1-mw univalue
AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS)
=======
DIST_SUBDIRS = secp256k1 univalue chiabls

AM_LDFLAGS = $(PTHREAD_CFLAGS) $(LIBTOOL_LDFLAGS) $(HARDENED_LDFLAGS) $(GPROF_LDFLAGS) $(SANITIZER_LDFLAGS)
AM_CXXFLAGS = $(DEBUG_CXXFLAGS) $(HARDENED_CXXFLAGS) $(WARN_CXXFLAGS) $(NOWARN_CXXFLAGS) $(ERROR_CXXFLAGS) $(GPROF_CXXFLAGS) $(SANITIZER_CXXFLAGS)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
AM_CPPFLAGS = $(DEBUG_CPPFLAGS) $(HARDENED_CPPFLAGS)
AM_LIBTOOLFLAGS = --preserve-dup-deps
EXTRA_LIBRARIES =

if EMBEDDED_UNIVALUE
LIBUNIVALUE = univalue/libunivalue.la

$(LIBUNIVALUE): $(wildcard univalue/lib/*) $(wildcard univalue/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
else
LIBUNIVALUE = $(UNIVALUE_LIBS)
endif

BITCOIN_CONFIG_INCLUDES=-I$(builddir)/config
<<<<<<< HEAD
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BITCOIN_CONFIG_INCLUDES) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS) $(CRYPTO_CFLAGS) $(SSL_CFLAGS)

BITCOIN_INCLUDES += -I$(srcdir)/secp256k1/include
BITCOIN_INCLUDES += -I$(srcdir)/secp256k1-mw/include
BITCOIN_INCLUDES += -I$(srcdir)/zxcvbn
=======
BITCOIN_INCLUDES=-I$(builddir) -I$(builddir)/obj $(BDB_CPPFLAGS) $(BOOST_CPPFLAGS) $(LEVELDB_CPPFLAGS)

BITCOIN_INCLUDES += -I$(srcdir)/rust/include
BITCOIN_INCLUDES += -I$(srcdir)/secp256k1/include
BITCOIN_INCLUDES += -I$(srcdir)/chiabls/src
BITCOIN_INCLUDES += -I$(srcdir)/chiabls/contrib/relic/include
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
BITCOIN_INCLUDES += $(UNIVALUE_CFLAGS)

LIBBITCOIN_SERVER=libbitcoin_server.a
LIBBITCOIN_COMMON=libbitcoin_common.a
LIBBITCOIN_CLI=libbitcoin_cli.a
LIBBITCOIN_UTIL=libbitcoin_util.a
LIBBITCOIN_CRYPTO=crypto/libbitcoin_crypto.a
<<<<<<< HEAD
LIBBITCOINQT=qt/libbitcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBSECP256K1_2=secp256k1-mw/libsecp256k1_2.la
LIBBITCOIN_ZXCVBN=libbitcoin_zxcvbn.a
=======
LIBBITCOIN_ZEROCOIN=libzerocoin/libbitcoin_zerocoin.a
LIBBITCOINQT=qt/libbitcoinqt.a
LIBSECP256K1=secp256k1/libsecp256k1.la
LIBSAPLING=libsapling.a
LIBCHIABLS=chiabls/libchiabls.la
if ENABLE_ONLINE_RUST
LIBRUSTZCASH=$(top_builddir)/target/release/librustzcash.a
else
LIBRUSTZCASH=$(top_builddir)/target/$(RUST_TARGET)/release/librustzcash.a
endif
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

if ENABLE_ZMQ
LIBBITCOIN_ZMQ=libbitcoin_zmq.a
endif
if BUILD_BITCOIN_LIBS
LIBBITCOINCONSENSUS=libbitcoinconsensus.la
endif
if ENABLE_WALLET
LIBBITCOIN_WALLET=libbitcoin_wallet.a
endif

<<<<<<< HEAD
$(LIBSECP256K1): $(wildcard secp256k1/src/*) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)
$(LIBSECP256K1_2): $(wildcard secp256k1-mw/src/*) $(wildcard secp256k1-mw/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

=======
BLS_LIBS = $(LIBCHIABLS)

RUST_ENV_VARS = RUSTC="$(RUSTC)" TERM=dumb
RUST_BUILD_OPTS = --release

if ENABLE_ONLINE_RUST
# Ensure that .cargo/config does not exist
CARGO_CONFIGURED = $(top_srcdir)/.cargo/.configured-for-online
$(CARGO_CONFIGURED):
	$(AM_V_at)rm -f $(top_srcdir)/.cargo/.configured-for-offline $(top_srcdir)/.cargo/config
	$(AM_V_at)touch $@

else
# Enable dependency vendoring
RUST_BUILD_OPTS += --locked --offline
if HAVE_RUST_TARGET
RUST_BUILD_OPTS += --target $(RUST_TARGET)
endif

CARGO_CONFIGURED = $(top_srcdir)/.cargo/.configured-for-offline
$(CARGO_CONFIGURED): $(top_srcdir)/.cargo/config.offline
	$(AM_V_at)rm -f $(top_srcdir)/.cargo/.configured-for-online
	$(AM_V_at)cp $< $(top_srcdir)/.cargo/config
	$(AM_V_at)echo "directory = \"$(RUST_VENDORED_SOURCES)\"" >> $(top_srcdir)/.cargo/config
	$(AM_V_at)touch $@
endif

cargo-build: $(CARGO_CONFIGURED)
	$(RUST_ENV_VARS) $(CARGO) build $(RUST_BUILD_OPTS) --manifest-path $(top_srcdir)/Cargo.toml

$(LIBRUSTZCASH): cargo-build

$(LIBSECP256K1): $(wildcard secp256k1/src/*.h) $(wildcard secp256k1/src/*.c) $(wildcard secp256k1/include/*)
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C $(@D) $(@F)

$(LIBCHIABLS):
	$(AM_V_at)$(MAKE) $(AM_MAKEFLAGS) -C chiabls

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
# Make is not made aware of per-object dependencies to avoid limiting building parallelization
# But to build the less dependent modules first, we manually select their order here:
EXTRA_LIBRARIES += \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_COMMON) \
<<<<<<< HEAD
=======
  $(LIBBITCOIN_ZEROCOIN) \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_CLI) \
  $(LIBBITCOIN_WALLET) \
  $(LIBBITCOIN_ZMQ) \
<<<<<<< HEAD
  $(LIBBITCOIN_ZXCVBN)
=======
  $(LIBSAPLING)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

lib_LTLIBRARIES = $(LIBBITCOINCONSENSUS)

bin_PROGRAMS =
noinst_PROGRAMS =
TESTS =
<<<<<<< HEAD

if BUILD_BITCOIND
  bin_PROGRAMS += prcycoind
endif

if BUILD_BITCOIN_UTILS
  bin_PROGRAMS += prcycoin-cli prcycoin-tx
endif

.PHONY: FORCE
# prcycoin core #
BITCOIN_CORE_H = \
  activemasternode.h \
  addrman.h \
  allocators.h \
  amount.h \
  base58.h \
  bignum.h \
  bip38.h \
  bip39.h \
  bip39_english.h \
  ecdhutil.h \
  enum.h \
  hdchain.h \
  bloom.h \
=======
BENCHMARKS =

if BUILD_BITCOIND
  bin_PROGRAMS += pivxd
endif

if BUILD_BITCOIN_UTILS
  bin_PROGRAMS += pivx-cli pivx-tx
endif

LIBSAPLING_H = \
  sapling/sapling.h \
  bech32.h \
  sapling/key_io_sapling.h \
  sapling/sapling_core_write.h \
  sapling/sapling_util.h \
  sapling/prf.h \
  sapling/noteencryption.h \
  sapling/address.h \
  sapling/note.h \
  sapling/zip32.h \
  sapling/saplingscriptpubkeyman.h \
  sapling/incrementalmerkletree.h \
  sapling/sapling_transaction.h \
  sapling/transaction_builder.h \
  sapling/sapling_operation.h

.PHONY: FORCE cargo-build check-symbols check-security
# pivx core #
BITCOIN_CORE_H = \
  activemasternode.h \
  addrdb.h \
  addrman.h \
  attributes.h \
  arith_uint256.h \
  amount.h \
  base58.h \
  bip38.h \
  bloom.h \
  blocksignature.h \
  bls/bls_ies.h \
  bls/bls_worker.h \
  bls/bls_wrapper.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  chain.h \
  chainparams.h \
  chainparamsbase.h \
  chainparamsseeds.h \
  checkpoints.h \
  checkqueue.h \
  clientversion.h \
  coincontrol.h \
  coins.h \
  compat.h \
<<<<<<< HEAD
  compat/sanity.h \
  compressor.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  crypter.h \
  obfuscation.h \
  obfuscation-relay.h \
  wallet/db.h \
  eccryptoverify.h \
  ecwrapper.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  init.h \
  invalid.h \
  kernel.h \
  swifttx.h \
  key.h \
  keystore.h \
  leveldbwrapper.h \
  limitedmap.h \
  main.h \
  masternode.h \
  masternode-payments.h \
  masternode-budget.h \
=======
  compat/byteswap.h \
  compat/cpuid.h \
  compat/endian.h \
  compat/sanity.h \
  compressor.h \
  consensus/consensus.h \
  consensus/merkle.h \
  consensus/validation.h \
  consensus/tx_verify.h \
  consensus/zerocoin_verify.h \
  consensus/params.h \
  consensus/upgrades.h \
  primitives/block.h \
  primitives/transaction.h \
  core_io.h \
  cuckoocache.h \
  crypter.h \
  ctpl.h \
  cyclingvector.h \
  evo/deterministicmns.h \
  evo/evodb.h \
  evo/evonotificationinterface.h \
  evo/providertx.h \
  evo/specialtx_validation.h \
  llmq/quorums_blockprocessor.h \
  llmq/quorums_commitment.h \
  llmq/quorums_init.h \
  llmq/quorums_utils.h \
  addressbook.h \
  wallet/db.h \
  flatfile.h \
  fs.h \
  hash.h \
  httprpc.h \
  httpserver.h \
  indirectmap.h \
  init.h \
  tiertwo/init.h \
  interfaces/handler.h \
  interfaces/wallet.h \
  invalid.h \
  invalid_outpoints.json.h \
  legacy/stakemodifier.h \
  kernel.h \
  key.h \
  key_io.h \
  keystore.h \
  dbwrapper.h \
  limitedmap.h \
  logging.h \
  legacy/validation_zerocoin_legacy.h \
  sapling/sapling_validation.h \
  budget/budgetdb.h \
  budget/budgetmanager.h \
  budget/budgetproposal.h \
  budget/budgetvote.h \
  budget/finalizedbudget.h \
  budget/finalizedbudgetvote.h \
  budget/budgetutil.h \
  mapport.h \
  memusage.h \
  masternode.h \
  masternode-payments.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  masternode-sync.h \
  masternodeman.h \
  masternodeconfig.h \
  merkleblock.h \
<<<<<<< HEAD
  miner.h \
  mruset.h \
  netbase.h \
  net.h \
  noui.h \
  poa.h \
  protocol.h \
  pubkey.h \
  random.h \
  reverselock.h \
  reverse_iterate.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/server.h \
  scheduler.h \
  script/interpreter.h \
=======
  messagesigner.h \
  blockassembler.h \
  miner.h \
  moneysupply.h \
  net.h \
  net_processing.h \
  netaddress.h \
  netbase.h \
  netmessagemaker.h \
  noui.h \
  policy/feerate.h \
  policy/fees.h \
  policy/policy.h \
  optional.h \
  operationresult.h \
  pow.h \
  prevector.h \
  protocol.h \
  pubkey.h \
  random.h \
  randomenv.h \
  reverse_iterate.h \
  rpc/client.h \
  rpc/protocol.h \
  rpc/register.h \
  rpc/server.h \
  saltedhasher.h \
  scheduler.h \
  script/interpreter.h \
  script/keyorigin.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  script/script.h \
  script/sigcache.h \
  script/sign.h \
  script/standard.h \
  script/script_error.h \
  serialize.h \
<<<<<<< HEAD
  streams.h \
  support/cleanse.h \
  sync.h \
  threadsafety.h \
=======
  shutdown.h \
  span.h \
  spork.h \
  sporkdb.h \
  sporkid.h \
  stakeinput.h \
  script/ismine.h \
  streams.h \
  support/allocators/mt_pooled_secure.h \
  support/allocators/pooled_secure.h \
  support/allocators/secure.h \
  support/allocators/zeroafterfree.h \
  support/cleanse.h \
  support/events.h \
  support/lockedpool.h \
  sync.h \
  threadsafety.h \
  threadinterrupt.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  timedata.h \
  tinyformat.h \
  torcontrol.h \
  txdb.h \
  txmempool.h \
  guiinterface.h \
<<<<<<< HEAD
  uint256.h \
  undo.h \
  util.h \
  util/macros.h \
  util/threadnames.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  validationinterface.h \
  version.h \
  wallet/wallet.h \
  wallet/wallet_ismine.h \
  wallet/walletdb.h \
=======
  guiinterfaceutil.h \
  uint256.h \
  undo.h \
  util/asmap.h \
  util/blockstatecatcher.h \
  util/system.h \
  util/macros.h \
  util/string.h \
  util/threadnames.h \
  util/validation.h \
  utilstrencodings.h \
  utilmoneystr.h \
  utiltime.h \
  util/vector.h \
  validation.h \
  validationinterface.h \
  version.h \
  wallet/hdchain.h \
  wallet/rpcwallet.h \
  wallet/scriptpubkeyman.h \
  destination_io.h \
  wallet/fees.h \
  wallet/init.h \
  wallet/wallet.h \
  wallet/walletdb.h \
  wallet/walletutil.h \
  warnings.h \
  zpivchain.h \
  zpiv/zpos.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  zmq/zmqabstractnotifier.h \
  zmq/zmqconfig.h \
  zmq/zmqnotificationinterface.h \
  zmq/zmqpublishnotifier.h

obj/build.h: FORCE
	@$(MKDIR_P) $(builddir)/obj
	@$(top_srcdir)/share/genbuild.sh "$(abs_top_builddir)/src/obj/build.h" \
	  "$(abs_top_srcdir)"
libbitcoin_util_a-clientversion.$(OBJEXT): obj/build.h

<<<<<<< HEAD
# server: shared between prcycoind and prcycoin-qt
libbitcoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbitcoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_server_a_SOURCES = \
  addrman.cpp \
  bloom.cpp \
  chain.cpp \
  checkpoints.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  leveldbwrapper.cpp \
  main.cpp \
  merkleblock.cpp \
  miner.cpp \
  net.cpp \
  noui.cpp \
  poa.cpp \
=======
# server: shared between pivxd and pivx-qt
libbitcoin_server_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(MINIUPNPC_CPPFLAGS) $(NATPMP_CPPFLAGS) $(EVENT_CFLAGS) $(EVENT_PTHREADS_CFLAGS)
libbitcoin_server_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_server_a_SOURCES = \
  addrdb.cpp \
  addrman.cpp \
  bloom.cpp \
  blocksignature.cpp \
  bls/bls_ies.cpp \
  bls/bls_worker.cpp \
  bls/bls_wrapper.cpp \
  chain.cpp \
  checkpoints.cpp \
  consensus/params.cpp \
  consensus/tx_verify.cpp \
  flatfile.cpp \
  consensus/zerocoin_verify.cpp \
  evo/deterministicmns.cpp \
  evo/evodb.cpp \
  evo/evonotificationinterface.cpp \
  evo/providertx.cpp \
  evo/specialtx_validation.cpp \
  llmq/quorums_blockprocessor.cpp \
  llmq/quorums_commitment.cpp \
  llmq/quorums_init.cpp \
  llmq/quorums_utils.cpp \
  httprpc.cpp \
  httpserver.cpp \
  init.cpp \
  tiertwo/init.cpp \
  dbwrapper.cpp \
  legacy/validation_zerocoin_legacy.cpp \
  sapling/sapling_validation.cpp \
  merkleblock.cpp \
  blockassembler.cpp \
  mapport.cpp \
  miner.cpp \
  net.cpp \
  net_processing.cpp \
  noui.cpp \
  policy/fees.cpp \
  policy/policy.cpp \
  pow.cpp \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  rest.cpp \
  rpc/blockchain.cpp \
  rpc/masternode.cpp \
  rpc/budget.cpp \
  rpc/mining.cpp \
  rpc/misc.cpp \
  rpc/net.cpp \
  rpc/rawtransaction.cpp \
<<<<<<< HEAD
  rpc/server.cpp \
  script/sigcache.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  txmempool.cpp \
  validationinterface.cpp \
  $(BITCOIN_CORE_H)
=======
  rpc/rpcevo.cpp \
  rpc/server.cpp \
  script/sigcache.cpp \
  script/ismine.cpp \
  shutdown.cpp \
  sporkdb.cpp \
  timedata.cpp \
  torcontrol.cpp \
  txdb.cpp \
  sapling/sapling_txdb.cpp \
  txmempool.cpp \
  validation.cpp \
  validationinterface.cpp \
  zpivchain.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBSAPLING_H)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

if ENABLE_ZMQ
libbitcoin_zmq_a_CPPFLAGS = $(BITCOIN_INCLUDES) $(ZMQ_CFLAGS)
libbitcoin_zmq_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_zmq_a_SOURCES = \
  zmq/zmqabstractnotifier.cpp \
  zmq/zmqnotificationinterface.cpp \
  zmq/zmqpublishnotifier.cpp
endif

<<<<<<< HEAD
libbitcoin_zxcvbn_a_CPPFLAGS = $(BITCOIN_INCLUDES)
libbitcoin_zxcvbn_a_CXXFLAGS = $(AM_CXXFLAGS)
libbitcoin_zxcvbn_a_SOURCES = \
  zxcvbn/_frequency_lists.cpp \
  zxcvbn/adjacency_graphs.cpp \
  zxcvbn/feedback.cpp \
  zxcvbn/frequency_lists.cpp \
  zxcvbn/matching.cpp \
  zxcvbn/scoring.cpp \
  zxcvbn/time_estimates.cpp \
  zxcvbn/util.cpp \
  zxcvbn/zxcvbn.cpp \
  zxcvbn/adjacency_graphs.hpp \
  zxcvbn/common.hpp \
  zxcvbn/feedback.hpp \
  zxcvbn/frequency_lists_common.hpp \
  zxcvbn/_frequency_lists.hpp \
  zxcvbn/frequency_lists.hpp \
  zxcvbn/matching.hpp \
  zxcvbn/optional.hpp \
  zxcvbn/scoring.hpp \
  zxcvbn/time_estimates.hpp \
  zxcvbn/util.hpp \
  zxcvbn/zxcvbn.hpp \
  zxcvbn/zxcvbn.h


# wallet: shared between prcycoind and prcycoin-qt, but only linked
# when wallet enabled

libbitcoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_wallet_a_SOURCES = \
  activemasternode.cpp \
  bip38.cpp \
  obfuscation.cpp \
  obfuscation-relay.cpp \
  wallet/db.cpp \
  crypter.cpp \
  swifttx.cpp \
  masternode.cpp \
  masternode-budget.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  kernel.cpp \
  wallet/wallet.cpp \
  wallet/wallet_ismine.cpp \
  wallet/walletdb.cpp \
  $(BITCOIN_CORE_H)

# crypto primitives library
crypto_libbitcoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS)
crypto_libbitcoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
crypto_libbitcoin_crypto_a_SOURCES = \
=======
# wallet: shared between pivxd and pivx-qt, but only linked
# when wallet enabled
libbitcoin_wallet_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_wallet_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_wallet_a_SOURCES = \
  bip38.cpp \
  interfaces/wallet.cpp \
  addressbook.cpp \
  crypter.cpp \
  legacy/stakemodifier.cpp \
  kernel.cpp \
  wallet/db.cpp \
  wallet/fees.cpp \
  wallet/init.cpp \
  wallet/rpcdump.cpp \
  wallet/rpcwallet.cpp \
  wallet/hdchain.cpp \
  wallet/scriptpubkeyman.cpp \
  destination_io.cpp \
  wallet/wallet.cpp \
  wallet/walletdb.cpp \
  wallet/walletutil.cpp \
  stakeinput.cpp \
  zpiv/zpos.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBSAPLING_H)

# crypto primitives library
crypto_libbitcoin_crypto_a_CPPFLAGS = $(AM_CPPFLAGS) $(PIC_FLAGS)
crypto_libbitcoin_crypto_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIC_FLAGS)
crypto_libbitcoin_crypto_a_SOURCES = \
  crypto/aes.cpp \
  crypto/aes.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/chacha20.h \
  crypto/chacha20.cpp \
  crypto/hmac_sha256.cpp \
  crypto/rfc6979_hmac_sha256.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/ripemd160.cpp \
  crypto/aes_helper.c \
  crypto/blake.c \
  crypto/bmw.c \
  crypto/groestl.c \
  crypto/jh.c \
  crypto/keccak.c \
  crypto/skein.c \
  crypto/common.h \
  crypto/sha256.h \
<<<<<<< HEAD
  crypto/sha512.h \
=======
  crypto/sha3.h \
  crypto/sha3.cpp \
  crypto/sha512.h \
  crypto/siphash.cpp \
  crypto/siphash.h \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  crypto/hmac_sha256.h \
  crypto/rfc6979_hmac_sha256.h \
  crypto/hmac_sha512.h \
  crypto/scrypt.h \
  crypto/sha1.h \
  crypto/ripemd160.h \
  crypto/sph_blake.h \
  crypto/sph_bmw.h \
  crypto/sph_groestl.h \
  crypto/sph_jh.h \
  crypto/sph_keccak.h \
  crypto/sph_skein.h \
  crypto/sph_types.h

<<<<<<< HEAD
# common: shared between prcycoind, and prcycoin-qt and non-server tools
libbitcoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_common_a_SOURCES = \
  allocators.cpp \
  amount.cpp \
  base58.cpp \
  bip38.cpp \
  bip39.cpp \
  hdchain.cpp \
  chainparams.cpp \
  coins.cpp \
  compressor.cpp \
=======
# libzerocoin library
libzerocoin_libbitcoin_zerocoin_a_CPPFLAGS = $(AM_CPPFLAGS) $(BOOST_CPPFLAGS)
libzerocoin_libbitcoin_zerocoin_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libzerocoin_libbitcoin_zerocoin_a_SOURCES = \
  libzerocoin/bignum.h \
  libzerocoin/Coin.h \
  libzerocoin/CoinRandomnessSchnorrSignature.h \
  libzerocoin/CoinSpend.h \
  libzerocoin/Commitment.h \
  libzerocoin/Denominations.h \
  libzerocoin/ParamGeneration.h \
  libzerocoin/Params.h \
  libzerocoin/SpendType.h \
  libzerocoin/ZerocoinDefines.h \
  zpiv/zpivmodule.h \
  libzerocoin/bignum.cpp \
  libzerocoin/Coin.cpp \
  libzerocoin/CoinRandomnessSchnorrSignature.cpp \
  libzerocoin/CoinSpend.cpp \
  libzerocoin/Denominations.cpp \
  libzerocoin/ParamGeneration.cpp \
  libzerocoin/Params.cpp \
  zpiv/zpivmodule.cpp

# common: shared between pivxd, and pivx-qt and non-server tools
libbitcoin_common_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_common_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_common_a_SOURCES = \
  activemasternode.cpp \
  base58.cpp \
  bip38.cpp \
  budget/budgetdb.cpp \
  budget/budgetmanager.cpp \
  budget/budgetproposal.cpp \
  budget/budgetvote.cpp \
  budget/finalizedbudget.cpp \
  budget/finalizedbudgetvote.cpp \
  budget/budgetutil.cpp \
  chainparams.cpp \
  consensus/upgrades.cpp \
  coins.cpp \
  compressor.cpp \
  consensus/merkle.cpp \
  key_io.cpp \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  primitives/block.cpp \
  primitives/transaction.cpp \
  core_read.cpp \
  core_write.cpp \
<<<<<<< HEAD
  eccryptoverify.cpp \
  ecwrapper.cpp \
  hash.cpp \
  key.cpp \
  keystore.cpp \
  netbase.cpp \
  protocol.cpp \
  pubkey.cpp \
=======
  hash.cpp \
  invalid.cpp \
  key.cpp \
  keystore.cpp \
  masternode.cpp \
  masternode-payments.cpp \
  masternode-sync.cpp \
  masternodeconfig.cpp \
  masternodeman.cpp \
  messagesigner.cpp \
  netaddress.cpp \
  netbase.cpp \
  policy/feerate.cpp \
  protocol.cpp \
  pubkey.cpp \
  saltedhasher.cpp \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  scheduler.cpp \
  script/interpreter.cpp \
  script/script.cpp \
  script/sign.cpp \
  script/standard.cpp \
<<<<<<< HEAD
  script/script_error.cpp \
  $(BITCOIN_CORE_H)
=======
  tiertwo_networksync.cpp \
  warnings.cpp \
  script/script_error.cpp \
  spork.cpp \
  sporkdb.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBSAPLING_H)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

# util: shared between all executables.
# This library *must* be included to make sure that the glibc
# backward-compatibility objects and their sanity checks are linked.
libbitcoin_util_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_util_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_util_a_SOURCES = \
<<<<<<< HEAD
  allocators.cpp \
  compat/strnlen.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  random.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  sync.cpp \
  uint256.cpp \
  util.cpp \
  utilstrencodings.cpp \
  util/threadnames.cpp \
  utilmoneystr.cpp \
  utiltime.cpp \
  $(BITCOIN_CORE_H)
=======
  arith_uint256.cpp \
  chainparamsbase.cpp \
  clientversion.cpp \
  compat/glibc_sanity.cpp \
  compat/glibcxx_sanity.cpp \
  compat/strnlen.cpp \
  fs.cpp \
  interfaces/handler.cpp \
  logging.cpp \
  random.cpp \
  randomenv.cpp \
  rpc/protocol.cpp \
  support/cleanse.cpp \
  support/lockedpool.cpp \
  sync.cpp \
  threadinterrupt.cpp \
  util/asmap.cpp \
  uint256.cpp \
  util/system.cpp \
  utilmoneystr.cpp \
  util/threadnames.cpp \
  utilstrencodings.cpp \
  util/string.cpp \
  util/validation.cpp \
  utiltime.cpp \
  $(BITCOIN_CORE_H) \
  $(LIBSAPLING_H)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

if GLIBC_BACK_COMPAT
libbitcoin_util_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)
endif

<<<<<<< HEAD
# cli: shared between prcycoin-cli and prcycoin-qt
=======
# sapling protocol primitives #
libsapling_a_CPPFLAGS = $(AM_CPPFLAGS) $(PIC_FLAGS) $(BITCOIN_INCLUDES)
libsapling_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libsapling_a_SOURCES = \
  bech32.cpp \
  sapling/sapling_util.cpp \
  sapling/key_io_sapling.cpp \
  sapling/sapling_core_write.cpp \
  sapling/prf.cpp \
  sapling/noteencryption.cpp \
  sapling/address.cpp \
  sapling/note.cpp \
  sapling/zip32.cpp \
  sapling/crypter_sapling.cpp \
  sapling/saplingscriptpubkeyman.cpp \
  sapling/incrementalmerkletree.cpp \
  sapling/transaction_builder.cpp \
  sapling/sapling_operation.cpp

if GLIBC_BACK_COMPAT
libsapling_a_SOURCES += compat/glibc_compat.cpp
AM_LDFLAGS += $(COMPAT_LDFLAGS)
endif

# cli: shared between pivx-cli and pivx-qt
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
libbitcoin_cli_a_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
libbitcoin_cli_a_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
libbitcoin_cli_a_SOURCES = \
  rpc/client.cpp \
<<<<<<< HEAD
  $(BITCOIN_CORE_H)
=======
  $(BITCOIN_CORE_H) \
  $(LIBSAPLING_H)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

nodist_libbitcoin_util_a_SOURCES = $(srcdir)/obj/build.h
#

<<<<<<< HEAD
# prcycoind binary #
prcycoind_SOURCES = prcycoind.cpp
prcycoind_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
prcycoind_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
prcycoind_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
prcycoind_SOURCES += prcycoind-res.rc
endif
prcycoind_LDADD = \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_WALLET) \
  $(LIBBITCOIN_ZMQ) \
  $(LIBBITCOIN_CRYPTO) \
=======
# pivxd binary #
pivxd_SOURCES = pivxd.cpp
pivxd_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
pivxd_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pivxd_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pivxd_SOURCES += pivxd-res.rc
endif
pivxd_LDADD = \
  $(LIBBITCOIN_SERVER) \
  $(LIBBITCOIN_WALLET) \
  $(LIBBITCOIN_COMMON) \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_ZEROCOIN) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_ZMQ) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBSAPLING) \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  $(LIBLEVELDB) \
  $(LIBLEVELDB_SSE42) \
  $(LIBMEMENV) \
  $(LIBSECP256K1) \
<<<<<<< HEAD
  $(LIBSECP256K1_2)

prcycoind_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(MINIUPNPC_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS)

# prcycoin-cli binary #
prcycoin_cli_SOURCES = prcycoin-cli.cpp
prcycoin_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(EVENT_CFLAGS)
prcycoin_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
prcycoin_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
prcycoin_cli_SOURCES += prcycoin-cli-res.rc
endif

prcycoin_cli_LDADD = \
=======
  $(LIBRUSTZCASH) \
  $(LIBZCASH_LIBS)

pivxd_LDADD += $(BOOST_LIBS) $(BDB_LIBS) $(MINIUPNPC_LIBS) $(NATPMP_LIBS) $(EVENT_PTHREADS_LIBS) $(EVENT_LIBS) $(ZMQ_LIBS) $(BLS_LIBS)

# pivx-cli binary #
pivx_cli_SOURCES = pivx-cli.cpp
pivx_cli_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES) $(EVENT_CFLAGS)
pivx_cli_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pivx_cli_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pivx_cli_SOURCES += pivx-cli-res.rc
endif

pivx_cli_LDADD = \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  $(LIBBITCOIN_CLI) \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_CRYPTO) \
<<<<<<< HEAD
  $(LIBSECP256K1) \
  $(LIBSECP256K1_2)

prcycoin_cli_SOURCES = \
  prcycoin-cli.cpp

prcycoin_cli_LDADD += $(BOOST_LIBS) $(SSL_LIBS) $(CRYPTO_LIBS) $(EVENT_LIBS)
#

# prcycoin-tx binary #
prcycoin_tx_SOURCES = prcycoin-tx.cpp
prcycoin_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
prcycoin_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
prcycoin_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
prcycoin_tx_SOURCES += prcycoin-tx-res.rc
endif

prcycoin_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(LIBSECP256K1_2)

prcycoin_tx_LDADD += $(BOOST_LIBS) $(CRYPTO_LIBS)
=======
  $(LIBSAPLING) \
  $(LIBRUSTZCASH) \
  $(LIBZCASH_LIBS)

pivx_cli_LDADD += $(BOOST_LIBS) $(EVENT_LIBS) $(BLS_LIBS)
#

# pivx-tx binary #
pivx_tx_SOURCES = pivx-tx.cpp
pivx_tx_CPPFLAGS = $(AM_CPPFLAGS) $(BITCOIN_INCLUDES)
pivx_tx_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)
pivx_tx_LDFLAGS = $(RELDFLAGS) $(AM_LDFLAGS) $(LIBTOOL_APP_LDFLAGS)

if TARGET_WINDOWS
pivx_tx_SOURCES += pivx-tx-res.rc
endif

pivx_tx_LDADD = \
  $(LIBUNIVALUE) \
  $(LIBBITCOIN_COMMON) \
  $(LIBBITCOIN_ZEROCOIN) \
  $(LIBBITCOIN_UTIL) \
  $(LIBBITCOIN_CRYPTO) \
  $(LIBSECP256K1) \
  $(LIBSAPLING) \
  $(LIBRUSTZCASH) \
  $(LIBZCASH_LIBS)

pivx_tx_LDADD += $(BOOST_LIBS) $(BLS_LIBS)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#

# bitcoinconsensus library #
if BUILD_BITCOIN_LIBS
include_HEADERS = script/bitcoinconsensus.h
libbitcoinconsensus_la_SOURCES = \
<<<<<<< HEAD
  allocators.cpp \
=======
  arith_uint256.cpp \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  primitives/transaction.cpp \
  crypto/hmac_sha512.cpp \
  crypto/scrypt.cpp \
  crypto/sha1.cpp \
  crypto/sha256.cpp \
  crypto/sha512.cpp \
  crypto/ripemd160.cpp \
<<<<<<< HEAD
  eccryptoverify.cpp \
  ecwrapper.cpp \
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  hash.cpp \
  pubkey.cpp \
  script/script.cpp \
  script/interpreter.cpp \
  script/bitcoinconsensus.cpp \
  uint256.cpp \
  utilstrencodings.cpp

if GLIBC_BACK_COMPAT
  libbitcoinconsensus_la_SOURCES += compat/glibc_compat.cpp
endif

libbitcoinconsensus_la_LDFLAGS = $(AM_LDFLAGS) -no-undefined $(RELDFLAGS)
<<<<<<< HEAD
libbitcoinconsensus_la_LIBADD = $(CRYPTO_LIBS) $(BOOST_LIBS)
libbitcoinconsensus_la_CPPFLAGS = $(CRYPTO_CFLAGS) -I$(builddir)/obj -DBUILD_BITCOIN_INTERNAL
if USE_LIBSECP256K1
libbitcoinconsensus_la_LIBADD += secp256k1/libsecp256k1.la
libbitcoinconsensus_la_LIBADD += secp256k1-mw/libsecp256k1_2.la
endif
endif

CLEANFILES = $(EXTRA_LIBRARIES)
CLEANFILES += leveldb/libleveldb.a leveldb/libmemenv.a
CLEANFILES += *.gcda *.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += obj/build.h

EXTRA_DIST = leveldb

config/prcycoin-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/prcycoin-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/prcycoin-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/prcycoin-config.h.in
	
clean-local:
	-$(MAKE) -C leveldb clean
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C secp256k1-mw clean
	-$(MAKE) -C univalue clean
	rm -f leveldb/*/*.gcno leveldb/helpers/memenv/*.gcno
	-rm -f config.h
=======
libbitcoinconsensus_la_LIBADD = $(LIBSECP256K1) $(BLS_LIBS)
libbitcoinconsensus_la_CPPFLAGS = $(AM_CPPFLAGS) -I$(builddir)/obj -I$(srcdir)/secp256k1/include -DBUILD_BITCOIN_INTERNAL
libbitcoinconsensus_la_CXXFLAGS = $(AM_CXXFLAGS) $(PIE_FLAGS)

endif
#

CLEANFILES = $(EXTRA_LIBRARIES)

CLEANFILES += *.gcda *.gcno
CLEANFILES += budget/*.gcda budget/*.gcno
CLEANFILES += bls/*.gcda bls/*.gcno
CLEANFILES += compat/*.gcda compat/*.gcno
CLEANFILES += consensus/*.gcda consensus/*.gcno
CLEANFILES += crc32c/src/*.gcda crc32c/src/*.gcno
CLEANFILES += crypto/*.gcda crypto/*.gcno
CLEANFILES += evo/*.gcda evo/*.gcno
CLEANFILES += interfaces/*.gcda interfaces/*.gcno
CLEANFILES += legacy/*.gcda legacy/*.gcno
CLEANFILES += libzerocoin/*.gcda libzerocoin/*.gcno
CLEANFILES += policy/*.gcda policy/*.gcno
CLEANFILES += primitives/*.gcda primitives/*.gcno
CLEANFILES += rpc/*.gcda rpc/*.gcno
CLEANFILES += sapling/*.gcda sapling/*.gcno
CLEANFILES += script/*.gcda script/*.gcno
CLEANFILES += support/*.gcda support/*.gcno
CLEANFILES += test/librust/*.gcda test/librust/*.gcno
CLEANFILES += tiertwo/*.gcda tiertwo/*.gcno
CLEANFILES += univalue/*.gcda univalue/*.gcno
CLEANFILES += util/*.gcda util/*.gcno
CLEANFILES += wallet/*.gcda wallet/*.gcno
CLEANFILES += wallet/test/*.gcda wallet/test/*.gcno
CLEANFILES += zmq/*.gcda zmq/*.gcno
CLEANFILES += zpiv/*.gcda zpiv/*.gcno
CLEANFILES += obj/build.h

CTAES_DIST =  crypto/ctaes/bench.c
CTAES_DIST += crypto/ctaes/ctaes.c
CTAES_DIST += crypto/ctaes/ctaes.h
CTAES_DIST += crypto/ctaes/README.md
CTAES_DIST += crypto/ctaes/test.c

IMMER_DIST = immer

EXTRA_DIST = $(CTAES_DIST) rust
EXTRA_DIST += $(IMMER_DIST)


config/pivx-config.h: config/stamp-h1
	@$(MAKE) -C $(top_builddir) $(subdir)/$(@)
config/stamp-h1: $(top_srcdir)/$(subdir)/config/pivx-config.h.in $(top_builddir)/config.status
	$(AM_V_at)$(MAKE) -C $(top_builddir) $(subdir)/$(@)
$(top_srcdir)/$(subdir)/config/pivx-config.h.in:  $(am__configure_deps)
	$(AM_V_at)$(MAKE) -C $(top_srcdir) $(subdir)/config/pivx-config.h.in

clean-local:
	-$(MAKE) -C secp256k1 clean
	-$(MAKE) -C univalue clean
	-$(MAKE) -C chiabls clean
	-rm -f leveldb/*/*.gcda leveldb/*/*.gcno leveldb/helpers/memenv/*.gcda leveldb/helpers/memenv/*.gcno
	-rm -f config.h
	-rm -rf test/__pycache__
	-rm -rf ../target
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

.rc.o:
	@test -f $(WINDRES)
	## FIXME: How to get the appropriate modulename_CPPFLAGS in here?
	$(AM_V_GEN) $(WINDRES) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(CPPFLAGS) -DWINDRES_PREPROC -i $< -o $@

<<<<<<< HEAD
%.pb.cc %.pb.h: %.proto
	@test -f $(PROTOC)
	$(AM_V_GEN) $(PROTOC) --cpp_out=$(@D) --proto_path=$(<D) $<

if EMBEDDED_LEVELDB
=======
check-symbols: $(bin_PROGRAMS)
if GLIBC_BACK_COMPAT
	@echo "Checking glibc back compat..."
	$(AM_V_at) READELF=$(READELF) CPPFILT=$(CPPFILT) $(PYTHON) $(top_srcdir)/contrib/devtools/symbol-check.py < $(bin_PROGRAMS)
endif

check-security: $(bin_PROGRAMS)
if HARDEN
	@echo "Checking binary security..."
	$(AM_V_at) READELF=$(READELF) OBJDUMP=$(OBJDUMP) $(PYTHON) $(top_srcdir)/contrib/devtools/security-check.py < $(bin_PROGRAMS)
endif

if EMBEDDED_LEVELDB
include Makefile.crc32c.include
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
include Makefile.leveldb.include
endif

if ENABLE_TESTS
include Makefile.test.include
endif

<<<<<<< HEAD
=======
if ENABLE_BENCH
include Makefile.bench.include
endif

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
if ENABLE_QT
include Makefile.qt.include
endif

if ENABLE_QT_TESTS
include Makefile.qttest.include
endif
