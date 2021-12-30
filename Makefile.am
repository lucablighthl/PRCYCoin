# Copyright (c) 2013-2016 The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

<<<<<<< HEAD
ACLOCAL_AMFLAGS = -I build-aux/m4
SUBDIRS = src
=======
# Pattern rule to print variables, e.g. make print-top_srcdir
print-%:
	@echo $* = $($*)

ACLOCAL_AMFLAGS = -I build-aux/m4
SUBDIRS = src
if ENABLE_MAN
SUBDIRS += doc/man
endif
SUBDIRS += params
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
.PHONY: deploy FORCE

export PYTHONPATH

if BUILD_BITCOIN_LIBS
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libbitcoinconsensus.pc
endif

BITCOIND_BIN=$(top_builddir)/src/$(BITCOIN_DAEMON_NAME)$(EXEEXT)
BITCOIN_QT_BIN=$(top_builddir)/src/qt/$(BITCOIN_GUI_NAME)$(EXEEXT)
BITCOIN_CLI_BIN=$(top_builddir)/src/$(BITCOIN_CLI_NAME)$(EXEEXT)
<<<<<<< HEAD
BITCOIN_TX_BIN=$(top_builddir)/src/$(BITCOIN_TX_NAME)$(EXEEXT)
BITCOIN_WIN_INSTALLER=$(PACKAGE)-$(PACKAGE_VERSION)-win$(WINDOWS_BITS)-setup$(EXEEXT)
=======
BITCOIN_WIN_INSTALLER=$(PACKAGE)-$(PACKAGE_VERSION)-win64-setup$(EXEEXT)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

empty :=
space := $(empty) $(empty)

<<<<<<< HEAD
OSX_APP=PRCYcoin-Qt.app
=======
OSX_APP=PIVX-Qt.app
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
OSX_VOLNAME = $(subst $(space),-,$(PACKAGE_NAME))
OSX_DMG = $(OSX_VOLNAME).dmg
OSX_BACKGROUND_SVG=background.svg
OSX_BACKGROUND_IMAGE=background.tiff
OSX_BACKGROUND_IMAGE_DPIS=36 72
<<<<<<< HEAD
OSX_DSSTORE_GEN=$(top_srcdir)/contrib/macdeploy/custom_dsstore.py
OSX_DEPLOY_SCRIPT=$(top_srcdir)/contrib/macdeploy/macdeployqtplus
OSX_FANCY_PLIST=$(top_srcdir)/contrib/macdeploy/fancy.plist
OSX_INSTALLER_ICONS=$(top_srcdir)/src/qt/res/icons/bitcoin.icns
OSX_PLIST=$(top_builddir)/share/qt/Info.plist #not installed
OSX_QT_TRANSLATIONS = ar,bg,ca,cs,da,de,es,fa,fi,fr,gd,gl,he,hu,it,ja,ko,lt,lv,pl,pt,ru,sk,sl,sv,uk,zh_CN,zh_TW

DIST_DOCS = $(wildcard doc/*.md) $(wildcard doc/release-notes/*.md)

WINDOWS_PACKAGING = $(top_srcdir)/share/pixmaps/bitcoin.ico \
=======
OSX_DEPLOY_SCRIPT=$(top_srcdir)/contrib/macdeploy/macdeployqtplus
OSX_INSTALLER_ICONS=$(top_srcdir)/src/qt/res/icons/bitcoin.icns
OSX_PLIST=$(top_builddir)/share/qt/Info.plist #not installed

DIST_DOCS = \
  README.md \
  $(wildcard doc/*.md) \
  $(wildcard doc/release-notes/*.md)
DIST_CONTRIB = $(top_srcdir)/contrib/pivx-cli.bash-completion \
	       $(top_srcdir)/contrib/pivx-tx.bash-completion \
	       $(top_srcdir)/contrib/pivxd.bash-completion \
           $(top_srcdir)/contrib/init \
           $(top_srcdir)/contrib/install_db4.sh

DIST_SHARE = \
           $(top_srcdir)/share/genbuild.sh \
           $(top_srcdir)/share/rpcauth

DIST_CARGO = \
           $(top_srcdir)/.cargo/config.offline \
           $(top_srcdir)/Cargo.lock \
           $(top_srcdir)/Cargo.toml \
           $(top_srcdir)/rust-toolchain \
           $(top_srcdir)/params/install-params.sh

BIN_CHECKS=$(top_srcdir)/contrib/devtools/symbol-check.py \
           $(top_srcdir)/contrib/devtools/security-check.py

WINDOWS_PACKAGING = $(top_srcdir)/share/pixmaps/pivx.ico \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  $(top_srcdir)/share/pixmaps/nsis-header.bmp \
  $(top_srcdir)/share/pixmaps/nsis-wizard.bmp \
  $(top_srcdir)/doc/README_windows.txt

<<<<<<< HEAD
OSX_PACKAGING = $(OSX_DEPLOY_SCRIPT) $(OSX_FANCY_PLIST) $(OSX_INSTALLER_ICONS) \
  $(top_srcdir)/contrib/macdeploy/$(OSX_BACKGROUND_SVG) \
  $(OSX_DSSTORE_GEN) \
  $(top_srcdir)/contrib/macdeploy/detached-sig-apply.sh \
  $(top_srcdir)/contrib/macdeploy/detached-sig-create.sh

COVERAGE_INFO = baseline_filtered_combined.info baseline.info block_test.info \
  leveldb_baseline.info test_prcycoin_filtered.info total_coverage.info \
  baseline_filtered.info block_test_filtered.info \
  leveldb_baseline_filtered.info test_prcycoin_coverage.info test_prcycoin.info

dist-hook:
	-$(MAKE) -C $(top_distdir)/src/leveldb clean
	-$(MAKE) -C $(top_distdir)/src/secp256k1 distclean
	-$(GIT) archive --format=tar HEAD -- src/clientversion.cpp | $(AMTAR) -C $(top_distdir) -xf -

distcheck-hook:
	$(MKDIR_P) $(top_distdir)/_build/src/leveldb
	cp -rf $(top_srcdir)/src/leveldb/* $(top_distdir)/_build/src/leveldb/
	-$(MAKE) -C $(top_distdir)/_build/src/leveldb clean

distcleancheck:
	@:

=======
LINUX_PACKAGING = $(top_srcdir)/share/pixmaps/pivx16.xpm \
  $(top_srcdir)/share/pixmaps/pivx32.xpm \
  $(top_srcdir)/share/pixmaps/pivx128.png

OSX_PACKAGING = $(OSX_DEPLOY_SCRIPT) $(OSX_INSTALLER_ICONS) \
  $(top_srcdir)/contrib/macdeploy/$(OSX_BACKGROUND_SVG) \
  $(top_srcdir)/contrib/macdeploy/detached-sig-apply.sh \
  $(top_srcdir)/contrib/macdeploy/detached-sig-create.sh

SPEND_PARAMS = $(top_srcdir)/params/sapling-spend.params
OUTPUT_PARAMS = $(top_srcdir)/params/sapling-output.params

COVERAGE_INFO = baseline.info \
  test_pivx_filtered.info total_coverage.info \
  baseline_filtered.info functional_test.info functional_test_filtered.info \
  test_pivx_coverage.info test_pivx.info

dist-hook:
	-$(GIT) archive --format=tar HEAD -- src/clientversion.cpp | $(AMTAR) -C $(top_distdir) -xf -

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
$(BITCOIN_WIN_INSTALLER): all-recursive
	$(MKDIR_P) $(top_builddir)/release
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM) $(BITCOIND_BIN) $(top_builddir)/release
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM) $(BITCOIN_QT_BIN) $(top_builddir)/release
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM) $(BITCOIN_CLI_BIN) $(top_builddir)/release
<<<<<<< HEAD
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM) $(BITCOIN_TX_BIN) $(top_builddir)/release
=======
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
	@test -f $(MAKENSIS) && $(MAKENSIS) -V2 $(top_builddir)/share/setup.nsi || \
	  echo error: could not build $@
	@echo built $@

$(OSX_APP)/Contents/PkgInfo:
	$(MKDIR_P) $(@D)
	@echo "APPL????" > $@

$(OSX_APP)/Contents/Resources/empty.lproj:
	$(MKDIR_P) $(@D)
	@touch $@

$(OSX_APP)/Contents/Info.plist: $(OSX_PLIST)
	$(MKDIR_P) $(@D)
	$(INSTALL_DATA) $< $@

$(OSX_APP)/Contents/Resources/bitcoin.icns: $(OSX_INSTALLER_ICONS)
	$(MKDIR_P) $(@D)
	$(INSTALL_DATA) $< $@

<<<<<<< HEAD
$(OSX_APP)/Contents/MacOS/PRCYcoin-Qt: $(BITCOIN_QT_BIN)
	$(MKDIR_P) $(@D)
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM)  $< $@
=======
$(OSX_APP)/Contents/MacOS/PIVX-Qt: all-recursive
	$(MKDIR_P) $(@D)
	STRIPPROG="$(STRIP)" $(INSTALL_STRIP_PROGRAM)  $(BITCOIN_QT_BIN) $@
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

$(OSX_APP)/Contents/Resources/Base.lproj/InfoPlist.strings:
	$(MKDIR_P) $(@D)
	echo '{	CFBundleDisplayName = "$(PACKAGE_NAME)"; CFBundleName = "$(PACKAGE_NAME)"; }' > $@

<<<<<<< HEAD
OSX_APP_BUILT=$(OSX_APP)/Contents/PkgInfo $(OSX_APP)/Contents/Resources/empty.lproj \
  $(OSX_APP)/Contents/Resources/bitcoin.icns $(OSX_APP)/Contents/Info.plist \
  $(OSX_APP)/Contents/MacOS/PRCYcoin-Qt $(OSX_APP)/Contents/Resources/Base.lproj/InfoPlist.strings
=======
$(OSX_APP)/Contents/Resources/sapling-spend.params: $(SPEND_PARAMS)
	$(MKDIR_P) $(@D)
	$(INSTALL_DATA) $< $@

$(OSX_APP)/Contents/Resources/sapling-output.params: $(OUTPUT_PARAMS)
	$(MKDIR_P) $(@D)
	$(INSTALL_DATA) $< $@

OSX_APP_BUILT=$(OSX_APP)/Contents/PkgInfo $(OSX_APP)/Contents/Resources/empty.lproj \
  $(OSX_APP)/Contents/Resources/bitcoin.icns $(OSX_APP)/Contents/Info.plist \
  $(OSX_APP)/Contents/MacOS/PIVX-Qt $(OSX_APP)/Contents/Resources/Base.lproj/InfoPlist.strings \
  $(OSX_APP)/Contents/Resources/sapling-spend.params $(OSX_APP)/Contents/Resources/sapling-output.params
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

osx_volname:
	echo $(OSX_VOLNAME) >$@

if BUILD_DARWIN
$(OSX_DMG): $(OSX_APP_BUILT) $(OSX_PACKAGING) $(OSX_BACKGROUND_IMAGE)
<<<<<<< HEAD
	$(PYTHON) $(OSX_DEPLOY_SCRIPT) $(OSX_APP) -add-qt-tr $(OSX_QT_TRANSLATIONS) -translations-dir=$(QT_TRANSLATION_DIR) -dmg -fancy $(OSX_FANCY_PLIST) -verbose 2 -volname $(OSX_VOLNAME)
=======
	$(PYTHON) $(OSX_DEPLOY_SCRIPT) $(OSX_APP) $(OSX_VOLNAME) -translations-dir=$(QT_TRANSLATION_DIR) -dmg
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

$(OSX_BACKGROUND_IMAGE).png: contrib/macdeploy/$(OSX_BACKGROUND_SVG)
	sed 's/PACKAGE_NAME/$(PACKAGE_NAME)/' < "$<" | $(RSVG_CONVERT) -f png -d 36 -p 36 -o $@
$(OSX_BACKGROUND_IMAGE)@2x.png: contrib/macdeploy/$(OSX_BACKGROUND_SVG)
	sed 's/PACKAGE_NAME/$(PACKAGE_NAME)/' < "$<" | $(RSVG_CONVERT) -f png -d 72 -p 72 -o $@
$(OSX_BACKGROUND_IMAGE): $(OSX_BACKGROUND_IMAGE).png $(OSX_BACKGROUND_IMAGE)@2x.png
	tiffutil -cathidpicheck $^ -out $@

deploydir: $(OSX_DMG)
else
APP_DIST_DIR=$(top_builddir)/dist
APP_DIST_EXTRAS=$(APP_DIST_DIR)/.background/$(OSX_BACKGROUND_IMAGE) $(APP_DIST_DIR)/.DS_Store $(APP_DIST_DIR)/Applications

$(APP_DIST_DIR)/Applications:
	@rm -f $@
	@cd $(@D); $(LN_S) /Applications $(@F)

<<<<<<< HEAD
$(APP_DIST_EXTRAS): $(APP_DIST_DIR)/$(OSX_APP)/Contents/MacOS/PRCYcoin-Qt
=======
$(APP_DIST_EXTRAS): $(APP_DIST_DIR)/$(OSX_APP)/Contents/MacOS/PIVX-Qt
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

$(OSX_DMG): $(APP_DIST_EXTRAS)
	$(GENISOIMAGE) -no-cache-inodes -D -l -probe -V "$(OSX_VOLNAME)" -no-pad -r -dir-mode 0755 -apple -o $@ dist

dpi%.$(OSX_BACKGROUND_IMAGE): contrib/macdeploy/$(OSX_BACKGROUND_SVG)
	sed 's/PACKAGE_NAME/$(PACKAGE_NAME)/' < "$<" | $(RSVG_CONVERT) -f png -d $* -p $* | $(IMAGEMAGICK_CONVERT) - $@
OSX_BACKGROUND_IMAGE_DPIFILES := $(foreach dpi,$(OSX_BACKGROUND_IMAGE_DPIS),dpi$(dpi).$(OSX_BACKGROUND_IMAGE))
$(APP_DIST_DIR)/.background/$(OSX_BACKGROUND_IMAGE): $(OSX_BACKGROUND_IMAGE_DPIFILES)
	$(MKDIR_P) $(@D)
	$(TIFFCP) -c none $(OSX_BACKGROUND_IMAGE_DPIFILES) $@

<<<<<<< HEAD
$(APP_DIST_DIR)/.DS_Store: $(OSX_DSSTORE_GEN)
	$(PYTHON) $< "$@" "$(OSX_VOLNAME)"

$(APP_DIST_DIR)/$(OSX_APP)/Contents/MacOS/PRCYcoin-Qt: $(OSX_APP_BUILT) $(OSX_PACKAGING)
	INSTALLNAMETOOL=$(INSTALLNAMETOOL)  OTOOL=$(OTOOL) STRIP=$(STRIP) $(PYTHON) $(OSX_DEPLOY_SCRIPT) $(OSX_APP) -translations-dir=$(QT_TRANSLATION_DIR) -add-qt-tr $(OSX_QT_TRANSLATIONS) -verbose 2
=======
$(APP_DIST_DIR)/$(OSX_APP)/Contents/MacOS/PIVX-Qt: $(OSX_APP_BUILT) $(OSX_PACKAGING)
	INSTALLNAMETOOL=$(INSTALLNAMETOOL) OTOOL=$(OTOOL) STRIP=$(STRIP) $(PYTHON) $(OSX_DEPLOY_SCRIPT) $(OSX_APP) $(OSX_VOLNAME) -translations-dir=$(QT_TRANSLATION_DIR)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

deploydir: $(APP_DIST_EXTRAS)
endif

if TARGET_DARWIN
appbundle: $(OSX_APP_BUILT)
deploy: $(OSX_DMG)
endif
if TARGET_WINDOWS
deploy: $(BITCOIN_WIN_INSTALLER)
endif

$(BITCOIN_QT_BIN): FORCE
	$(MAKE) -C src qt/$(@F)

$(BITCOIND_BIN): FORCE
	$(MAKE) -C src $(@F)

$(BITCOIN_CLI_BIN): FORCE
	$(MAKE) -C src $(@F)

<<<<<<< HEAD
$(BITCOIN_TX_BIN): FORCE
	$(MAKE) -C src $(@F)

if USE_LCOV
=======
if USE_LCOV
LCOV_FILTER_PATTERN=-p "/usr/include/" -p "/usr/lib/" -p "src/leveldb/" -p "src/crc32c/" -p "src/univalue" -p "src/secp256k1"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

baseline.info:
	$(LCOV) -c -i -d $(abs_builddir)/src -o $@

baseline_filtered.info: baseline.info
<<<<<<< HEAD
	$(LCOV) -r $< "/usr/include/*" -o $@

leveldb_baseline.info: baseline_filtered.info
	$(LCOV) -c -i -d $(abs_builddir)/src/leveldb -b $(abs_builddir)/src/leveldb -o $@

leveldb_baseline_filtered.info: leveldb_baseline.info
	$(LCOV) -r $< "/usr/include/*" -o $@

baseline_filtered_combined.info: leveldb_baseline_filtered.info baseline_filtered.info
	$(LCOV) -a leveldb_baseline_filtered.info -a baseline_filtered.info -o $@

test_prcycoin.info: baseline_filtered_combined.info
	$(MAKE) -C src/ check
	$(LCOV) -c -d $(abs_builddir)/src -t test_prcycoin -o $@
	$(LCOV) -z -d $(abs_builddir)/src
	$(LCOV) -z -d $(abs_builddir)/src/leveldb

test_prcycoin_filtered.info: test_prcycoin.info
	$(LCOV) -r $< "/usr/include/*" -o $@

block_test.info: test_prcycoin_filtered.info
	$(MKDIR_P) qa/tmp
	-@TIMEOUT=15 qa/pull-tester/run-bitcoind-for-test.sh $(JAVA) -jar $(JAVA_COMPARISON_TOOL) qa/tmp/compTool 0
	$(LCOV) -c -d $(abs_builddir)/src --t BitcoinJBlockTest -o $@
	$(LCOV) -z -d $(abs_builddir)/src
	$(LCOV) -z -d $(abs_builddir)/src/leveldb

block_test_filtered.info: block_test.info
	$(LCOV) -r $< "/usr/include/*" -o $@

test_prcycoin_coverage.info: baseline_filtered_combined.info test_prcycoin_filtered.info
	$(LCOV) -a baseline_filtered.info -a leveldb_baseline_filtered.info -a test_prcycoin_filtered.info -o $@

total_coverage.info:  baseline_filtered_combined.info test_prcycoin_filtered.info block_test_filtered.info
	$(LCOV) -a baseline_filtered.info -a leveldb_baseline_filtered.info -a test_prcycoin_filtered.info -a block_test_filtered.info -o $@ | $(GREP) "\%" | $(AWK) '{ print substr($$3,2,50) "/" $$5 }' > coverage_percent.txt

test_prcycoin.coverage/.dirstamp:  test_prcycoin_coverage.info
	$(GENHTML) -s $< -o $(@D)
	@touch $@

total.coverage/.dirstamp: total_coverage.info
	$(GENHTML) -s $< -o $(@D)
	@touch $@

cov: test_prcycoin.coverage/.dirstamp total.coverage/.dirstamp
=======
	$(abs_builddir)/contrib/filter-lcov.py $(LCOV_FILTER_PATTERN) $< $@
	$(LCOV) -a $@ $(LCOV_OPTS) -o $@

test_pivx.info: baseline_filtered.info
	$(MAKE) -C src/ check
	$(LCOV) -c $(LCOV_OPTS) -d $(abs_builddir)/src -t test_pivx -o $@
	$(LCOV) -z $(LCOV_OPTS) -d $(abs_builddir)/src

test_pivx_filtered.info: test_pivx.info
	$(abs_builddir)/contrib/filter-lcov.py $(LCOV_FILTER_PATTERN) $< $@
	$(LCOV) -a $@ $(LCOV_OPTS) -o $@

functional_test.info: test_pivx_filtered.info
	-@TIMEOUT=15 test/functional/test_runner.py $(EXTENDED_FUNCTIONAL_TESTS)
	$(LCOV) -c $(LCOV_OPTS) -d $(abs_builddir)/src --t functional-tests -o $@
	$(LCOV) -z $(LCOV_OPTS) -d $(abs_builddir)/src

functional_test_filtered.info: functional_test.info
	$(abs_builddir)/contrib/filter-lcov.py $(LCOV_FILTER_PATTERN) $< $@
	$(LCOV) -a $@ $(LCOV_OPTS) -o $@

test_pivx_coverage.info: baseline_filtered.info test_pivx_filtered.info
	$(LCOV) -a $(LCOV_OPTS) baseline_filtered.info -a test_pivx_filtered.info -o $@

total_coverage.info: test_pivx_filtered.info functional_test_filtered.info
	$(LCOV) -a $(LCOV_OPTS) baseline_filtered.info -a test_pivx_filtered.info -a functional_test_filtered.info -o $@ | $(GREP) "\%" | $(AWK) '{ print substr($$3,2,50) "/" $$5 }' > coverage_percent.txt

test_pivx.coverage/.dirstamp:  test_pivx_coverage.info
	$(GENHTML) -s $(LCOV_OPTS) $< -o $(@D)
	@touch $@

total.coverage/.dirstamp: total_coverage.info
	$(GENHTML) -s $(LCOV_OPTS) $< -o $(@D)
	@touch $@

cov: test_pivx.coverage/.dirstamp total.coverage/.dirstamp
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

endif

dist_noinst_SCRIPTS = autogen.sh

<<<<<<< HEAD
EXTRA_DIST = $(top_srcdir)/share/genbuild.sh qa/pull-tester/rpc-tests.sh qa/pull-tester/run-bitcoin-cli qa/rpc-tests $(DIST_DOCS) $(WINDOWS_PACKAGING) $(OSX_PACKAGING)
=======
EXTRA_DIST = $(DIST_SHARE) $(DIST_CONTRIB) $(DIST_DOCS) $(DIST_CARGO) $(WINDOWS_PACKAGING) $(LINUX_PACKAGING) $(OSX_PACKAGING) $(BIN_CHECKS)

EXTRA_DIST += \
    test/functional \
    test/fuzz

EXTRA_DIST += \
    test/util/bitcoin-util-test.py \
    test/util/data/bitcoin-util-test.json \
    test/util/data/blanktxv1.hex \
    test/util/data/blanktxv1.json \
    test/util/data/tt-delin1-out.hex \
    test/util/data/tt-delin1-out.json \
    test/util/data/tt-delout1-out.hex \
    test/util/data/tt-delout1-out.json \
    test/util/data/tt-locktime317000-out.hex \
    test/util/data/tt-locktime317000-out.json \
    test/util/data/tx394b54bb.hex \
    test/util/data/txcreate1.hex \
    test/util/data/txcreate1.json \
    test/util/data/txcreate2.hex \
    test/util/data/txcreate2.json \
    test/util/data/txcreatescript1.hex \
    test/util/data/txcreatescript1.json \
    test/util/data/txcreatesign.hex \
    test/util/data/txcreatesign.json \
    test/util/data/pre_hd_wallet.dat \
    test/util/rpcauth-test.py
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

CLEANFILES = $(OSX_DMG) $(BITCOIN_WIN_INSTALLER)

.INTERMEDIATE: $(COVERAGE_INFO)

<<<<<<< HEAD
clean-local:
	rm -rf test_prcycoin.coverage/ total.coverage/ $(OSX_APP)
	rm -rf osx_volname dist/ dpi36.background.tiff dpi72.background.tiff
=======
DISTCHECK_CONFIGURE_FLAGS = --enable-man

doc/doxygen/.stamp: doc/Doxyfile FORCE
	$(MKDIR_P) $(@D)
	$(DOXYGEN) $^
	$(AM_V_at) touch $@

if HAVE_DOXYGEN
docs: doc/doxygen/.stamp
else
docs:
	@echo "error: doxygen not found"
endif

clean-docs:
	rm -rf doc/doxygen

clean-local: clean-docs
	rm -rf coverage_percent.txt test_pivx.coverage/ total.coverage/ test/tmp/ cache/ $(OSX_APP)
	rm -rf test/functional/__pycache__ test/functional/test_framework/__pycache__ test/cache share/rpcauth/__pycache__

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
