Release Process
====================

<<<<<<< HEAD
Before every release candidate:

* Update translations - see [translation_process.md](https://github.com/PRCYCoin/PRCYCoin/blob/master/doc/translation_process.md#synchronising-translations).
* Update release candidate version in `configure.ac` (`CLIENT_VERSION_RC`)

Before every minor and major release:
=======
## Branch updates

### Before every release candidate

* Update translations (ping Fuzzbawls on Discord) see [translation_process.md](https://github.com/PIVX-Project/PIVX/blob/master/doc/translation_process.md#synchronising-translations).
* Update manpages, see [gen-manpages.sh](https://github.com/pivx-project/pivx/blob/master/contrib/devtools/README.md#gen-manpagessh).
* Update release candidate version in `configure.ac` (`CLIENT_VERSION_RC`)

### Before every major and minor release
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

* Update version in `configure.ac` (don't forget to set `CLIENT_VERSION_IS_RELEASE` to `true`) (don't forget to set `CLIENT_VERSION_RC` to `0`)
* Write release notes (see below)

<<<<<<< HEAD
Before every major release:

* Update hardcoded [seeds](/contrib/seeds/README.md), see [this pull request](https://github.com/bitcoin/bitcoin/pull/7415) for an example.
* Update [`BLOCK_CHAIN_SIZE`](/src/qt/intro.cpp) to the current size plus some overhead.
* Update `src/chainparams.cpp` with statistics about the transaction count and rate.
* Update version of `contrib/gitian-descriptors/*.yml`: usually one'd want to do this on master after branching off the release - but be sure to at least do it before a new major release

### First time / New builders

If you're using the automated script (found in [contrib/gitian-build.sh](/contrib/gitian-build.sh)), then at this point you should run it with the "--setup" command. Otherwise ignore this.
=======
### Before every major release

* Update hardcoded [seeds](/contrib/seeds/README.md), see [this pull request](https://github.com/bitcoin/bitcoin/pull/7415) for an example.
* Update [`BLOCK_CHAIN_SIZE` and `TESTNET_BLOCK_CHAIN_SIZE`](/src/qt/intro.cpp) to the current size plus some overhead for each respective network.
* Update `src/chainparams.cpp` with statistics about the transaction count and rate.
* On both the master branch and the new release branch:
  - update `CLIENT_VERSION_MINOR` in [`configure.ac`](../configure.ac)
* On the new release branch in [`configure.ac`](../configure.ac):
  - set `CLIENT_VERSION_REVISION` to `0`
  - set `CLIENT_VERSION_IS_RELEASE` to `true`


#### After branch-off (on master)

- Update the version of `contrib/gitian-descriptors/*.yml`.

#### After branch-off (on the major release branch)

- Update the versions and the link to the release notes draft in `doc/release-notes.md`.

#### Before final release

- Merge the release notes into the branch.
- Ensure the "Needs release note" label is removed from all relevant pull requests and issues.


## Building

### First time / New builders

If you're using the automated script (found in [contrib/gitian-build.py](/contrib/gitian-build.py)), then at this point you should run it with the "--setup" command. Otherwise ignore this.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Check out the source code in the following directory hierarchy.

    cd /path/to/your/toplevel/build
<<<<<<< HEAD
    git clone https://github.com/prcycoin-project/gitian.sigs.git
    git clone https://github.com/prcycoin-project/prcycoin-detached-sigs.git
    git clone https://github.com/devrandom/gitian-builder.git
    git clone https://github.com/prcycoin-project/prcycoin.git

### PRCY maintainers/release engineers, suggestion for writing release notes
=======
    git clone https://github.com/pivx-project/gitian.sigs.git
    git clone https://github.com/pivx-project/pivx-detached-sigs.git
    git clone https://github.com/devrandom/gitian-builder.git
    git clone https://github.com/pivx-project/pivx.git

### PIVX maintainers/release engineers, suggestion for writing release notes
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Write release notes. git shortlog helps a lot, for example:

    git shortlog --no-merges v(current version, e.g. 0.7.2)..v(new version, e.g. 0.8.0)


Generate list of authors:

<<<<<<< HEAD
    git log --format='%aN' "$*" | sort -ui | sed -e 's/^/- /'

Tag version (or release candidate) in git
=======
    git log --format='- %aN' v(current version, e.g. 3.2.2)..v(new version, e.g. 3.2.3) | sort -fiu

Tag the version (or release candidate) in git:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    git tag -s v(new version, e.g. 0.8.0)

### Setup and perform Gitian builds

<<<<<<< HEAD
If you're using the automated script (found in [contrib/gitian-build.sh](/contrib/gitian-build.sh)), then at this point you should run it with the "--build" command. Otherwise ignore this.

Setup Gitian descriptors:

    pushd ./prcycoin
=======
If you're using the automated script (found in [contrib/gitian-build.py](/contrib/gitian-build.py)), then at this point you should run it with the "--build" command. Otherwise ignore this.

Setup Gitian descriptors:

    pushd ./pivx
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    export SIGNER=(your Gitian key, ie bluematt, sipa, etc)
    export VERSION=(new version, e.g. 0.8.0)
    git fetch
    git checkout v${VERSION}
    popd

Ensure your gitian.sigs are up-to-date if you wish to gverify your builds against other Gitian signatures.

    pushd ./gitian.sigs
    git pull
    popd

Ensure gitian-builder is up-to-date:

    pushd ./gitian-builder
    git pull
    popd

### Fetch and create inputs: (first time, or when dependency versions change)

    pushd ./gitian-builder
    mkdir -p inputs
<<<<<<< HEAD
    wget -P inputs https://bitcoincore.org/cfields/osslsigncode-Backports-to-1.7.1.patch
    wget -P inputs http://downloads.sourceforge.net/project/osslsigncode/osslsigncode/osslsigncode-1.7.1.tar.gz
    popd

Create the OS X SDK tarball, see the [OS X readme](README_osx.md) for details, and copy it into the inputs directory.

### Optional: Seed the Gitian sources cache and offline git repositories

By default, Gitian will fetch source files as needed. To cache them ahead of time:

    pushd ./gitian-builder
    make -C ../prcycoin/depends download SOURCES_PATH=`pwd`/cache/common
=======
    wget -O osslsigncode-2.0.tar.gz -P inputs https://github.com/mtrojnar/osslsigncode/archive/2.0.tar.gz
    echo '5a60e0a4b3e0b4d655317b2f12a810211c50242138322b16e7e01c6fbb89d92f inputs/osslsigncode-2.0.tar.gz' | sha256sum -c
    popd

Create the macOS SDK tarball, see the [macOS build instructions](build-osx.md#deterministic-macos-dmg-notes) for details, and copy it into the inputs directory.

### Optional: Seed the Gitian sources cache and offline git repositories

NOTE: Gitian is sometimes unable to download files. If you have errors, try the step below.

By default, Gitian will fetch source files as needed. To cache them ahead of time, make sure you have checked out the tag you want to build in pivx, then:

    pushd ./gitian-builder
    make -C ../pivx/depends download SOURCES_PATH=`pwd`/cache/common
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    popd

Only missing files will be fetched, so this is safe to re-run for each build.

NOTE: Offline builds must use the --url flag to ensure Gitian fetches only from local URLs. For example:

    pushd ./gitian-builder
<<<<<<< HEAD
    ./bin/gbuild --url prcycoin=/path/to/prcycoin,signature=/path/to/sigs {rest of arguments}
=======
    ./bin/gbuild --url pivx=/path/to/pivx,signature=/path/to/sigs {rest of arguments}
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    popd

The gbuild invocations below <b>DO NOT DO THIS</b> by default.

<<<<<<< HEAD
### Build and sign PRCY for Linux, Windows, and OS X:

    pushd ./gitian-builder
    ./bin/gbuild --memory 3000 --commit prcycoin=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/prcycoin-*.tar.gz build/out/src/prcycoin-*.tar.gz ../

    ./bin/gbuild --memory 3000 --commit prcycoin=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-win-unsigned --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/prcycoin-*-win-unsigned.tar.gz inputs/prcycoin-win-unsigned.tar.gz
    mv build/out/prcycoin-*.zip build/out/prcycoin-*.exe ../

    ./bin/gbuild --memory 3000 --commit prcycoin=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/prcycoin-*-osx-unsigned.tar.gz inputs/prcycoin-osx-unsigned.tar.gz
    mv build/out/prcycoin-*.tar.gz build/out/prcycoin-*.dmg ../

    ./bin/gbuild --memory 3000 --commit prcycoin=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-aarch64.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-linux --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-aarch64.yml
    mv build/out/prcycoin-*.tar.gz build/out/src/prcycoin-*.tar.gz ../
=======
### Build and sign PIVX Core for Linux, Windows, and macOS:

    pushd ./gitian-builder
    ./bin/gbuild --num-make 2 --memory 3000 --commit pivx=v${VERSION} ../pivx/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-linux --destination ../gitian.sigs/ ../pivx/contrib/gitian-descriptors/gitian-linux.yml
    mv build/out/pivx-*.tar.gz build/out/src/pivx-*.tar.gz ../

    ./bin/gbuild --num-make 2 --memory 3000 --commit pivx=v${VERSION} ../pivx/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-win-unsigned --destination ../gitian.sigs/ ../pivx/contrib/gitian-descriptors/gitian-win.yml
    mv build/out/pivx-*-win-unsigned.tar.gz inputs/pivx-win-unsigned.tar.gz
    mv build/out/pivx-*.zip build/out/pivx-*.exe ../

    ./bin/gbuild --num-make 2 --memory 3000 --commit pivx=v${VERSION} ../pivx/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-osx-unsigned --destination ../gitian.sigs/ ../pivx/contrib/gitian-descriptors/gitian-osx.yml
    mv build/out/pivx-*-osx-unsigned.tar.gz inputs/pivx-osx-unsigned.tar.gz
    mv build/out/pivx-*.tar.gz build/out/pivx-*.dmg ../
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    popd

Build output expected:

<<<<<<< HEAD
  1. source tarball (`prcycoin-${VERSION}.tar.gz`)
  2. linux 32-bit and 64-bit dist tarballs (`prcycoin-${VERSION}-linux[32|64].tar.gz`)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (`prcycoin-${VERSION}-win[32|64]-setup-unsigned.exe`, `prcycoin-${VERSION}-win[32|64].zip`)
  4. OS X unsigned installer and dist tarball (`prcycoin-${VERSION}-osx-unsigned.dmg`, `prcycoin-${VERSION}-osx64.tar.gz`)
=======
  1. source tarball (`pivx-${VERSION}.tar.gz`)
  2. linux 32-bit and 64-bit dist tarballs (`pivx-${VERSION}-linux[32|64].tar.gz`)
  3. windows 32-bit and 64-bit unsigned installers and dist zips (`pivx-${VERSION}-win[32|64]-setup-unsigned.exe`, `pivx-${VERSION}-win[32|64].zip`)
  4. macOS unsigned installer and dist tarball (`pivx-${VERSION}-osx-unsigned.dmg`, `pivx-${VERSION}-osx64.tar.gz`)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  5. Gitian signatures (in `gitian.sigs/${VERSION}-<linux|{win,osx}-unsigned>/(your Gitian key)/`)

### Verify other gitian builders signatures to your own. (Optional)

Add other gitian builders keys to your gpg keyring, and/or refresh keys.

<<<<<<< HEAD
    gpg --import prcycoin/contrib/gitian-keys/*.pgp
=======
    gpg --import pivx/contrib/gitian-keys/*.pgp
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    gpg --refresh-keys

Verify the signatures

    pushd ./gitian-builder
<<<<<<< HEAD
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-linux ../prcycoin/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-unsigned ../prcycoin/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-unsigned ../prcycoin/contrib/gitian-descriptors/gitian-osx.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-aarch64 ../prcycoin/contrib/gitian-descriptors/gitian-aarch64.yml
=======
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-linux ../pivx/contrib/gitian-descriptors/gitian-linux.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-unsigned ../pivx/contrib/gitian-descriptors/gitian-win.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-unsigned ../pivx/contrib/gitian-descriptors/gitian-osx.yml
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    popd

### Next steps:

Commit your signature to gitian.sigs:

    pushd gitian.sigs
<<<<<<< HEAD
    git add ${VERSION}-linux/${SIGNER}
    git add ${VERSION}-win-unsigned/${SIGNER}
    git add ${VERSION}-osx-unsigned/${SIGNER}
    git add ${VERSION}-aarch64/${SIGNER}
    git commit -a
    git push  # Assuming you can push to the gitian.sigs tree
    popd

Codesigner only: Create Windows/OS X detached signatures:
- Only one person handles codesigning. Everyone else should skip to the next step.
- Only once the Windows/OS X builds each have 3 matching signatures may they be signed with their respective release keys.

Codesigner only: Sign the osx binary:

    transfer prcycoin-osx-unsigned.tar.gz to osx for signing
    tar xf prcycoin-osx-unsigned.tar.gz
=======
    git add ${VERSION}-linux/"${SIGNER}"
    git add ${VERSION}-win-unsigned/"${SIGNER}"
    git add ${VERSION}-osx-unsigned/"${SIGNER}"
    git commit -m "Add ${VERSION} unsigned sigs for ${SIGNER}"
    git push  # Assuming you can push to the gitian.sigs tree
    popd

Codesigner only: Create Windows/macOS detached signatures:
- Only one person handles codesigning. Everyone else should skip to the next step.
- Only once the Windows/macOS builds each have 3 matching signatures may they be signed with their respective release keys.

Codesigner only: Sign the macOS binary:

    transfer pivx-osx-unsigned.tar.gz to macOS for signing
    tar xf pivx-osx-unsigned.tar.gz
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ./detached-sig-create.sh -s "Key ID"
    Enter the keychain password and authorize the signature
    Move signature-osx.tar.gz back to the gitian host

Codesigner only: Sign the windows binaries:

<<<<<<< HEAD
    tar xf prcycoin-win-unsigned.tar.gz
=======
    tar xf pivx-win-unsigned.tar.gz
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ./detached-sig-create.sh -key /path/to/codesign.key
    Enter the passphrase for the key when prompted
    signature-win.tar.gz will be created

Codesigner only: Commit the detached codesign payloads:

<<<<<<< HEAD
    cd ~/prcycoin-detached-sigs
=======
    cd ~/pivx-detached-sigs
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    checkout the appropriate branch for this release series
    rm -rf *
    tar xf signature-osx.tar.gz
    tar xf signature-win.tar.gz
    git add -a
    git commit -m "point to ${VERSION}"
    git tag -s v${VERSION} HEAD
    git push the current branch and new tag

<<<<<<< HEAD
Non-codesigners: wait for Windows/OS X detached signatures:

- Once the Windows/OS X builds each have 3 matching signatures, they will be signed with their respective release keys.
- Detached signatures will then be committed to the [prcycoin-detached-sigs](https://github.com/PRCYcoin-Project/prcycoin-detached-sigs) repository, which can be combined with the unsigned apps to create signed binaries.

Create (and optionally verify) the signed OS X binary:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-signed ../prcycoin/contrib/gitian-descriptors/gitian-osx-signer.yml
    mv build/out/prcycoin-osx-signed.dmg ../prcycoin-${VERSION}-osx.dmg
=======
Non-codesigners: wait for Windows/macOS detached signatures:

- Once the Windows/macOS builds each have 3 matching signatures, they will be signed with their respective release keys.
- Detached signatures will then be committed to the [pivx-detached-sigs](https://github.com/pivx-Project/pivx-detached-sigs) repository, which can be combined with the unsigned apps to create signed binaries.

Create (and optionally verify) the signed macOS binary:

    pushd ./gitian-builder
    ./bin/gbuild -i --commit signature=v${VERSION} ../pivx/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-osx-signed --destination ../gitian.sigs/ ../pivx/contrib/gitian-descriptors/gitian-osx-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-osx-signed ../pivx/contrib/gitian-descriptors/gitian-osx-signer.yml
    mv build/out/pivx-osx-signed.dmg ../pivx-${VERSION}-osx.dmg
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    popd

Create (and optionally verify) the signed Windows binaries:

    pushd ./gitian-builder
<<<<<<< HEAD
    ./bin/gbuild -i --commit signature=v${VERSION} ../prcycoin/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gsign --signer $SIGNER --release ${VERSION}-win-signed --destination ../gitian.sigs/ ../prcycoin/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-signed ../prcycoin/contrib/gitian-descriptors/gitian-win-signer.yml
    mv build/out/prcycoin-*win64-setup.exe ../prcycoin-${VERSION}-win64-setup.exe
    mv build/out/prcycoin-*win32-setup.exe ../prcycoin-${VERSION}-win32-setup.exe
    popd

Commit your signature for the signed OS X/Windows binaries:

    pushd gitian.sigs
    git add ${VERSION}-osx-signed/${SIGNER}
    git add ${VERSION}-win-signed/${SIGNER}
    git commit -a
=======
    ./bin/gbuild -i --commit signature=v${VERSION} ../pivx/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gsign --signer "$SIGNER" --release ${VERSION}-win-signed --destination ../gitian.sigs/ ../pivx/contrib/gitian-descriptors/gitian-win-signer.yml
    ./bin/gverify -v -d ../gitian.sigs/ -r ${VERSION}-win-signed ../pivx/contrib/gitian-descriptors/gitian-win-signer.yml
    mv build/out/pivx-*win64-setup.exe ../pivx-${VERSION}-win64-setup.exe
    popd

Commit your signature for the signed macOS/Windows binaries:

    pushd gitian.sigs
    git add ${VERSION}-osx-signed/"${SIGNER}"
    git add ${VERSION}-win-signed/"${SIGNER}"
    git commit -m "Add ${SIGNER} ${VERSION} signed binaries signatures"
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    git push  # Assuming you can push to the gitian.sigs tree
    popd

### After 3 or more people have gitian-built and their results match:

- Create `SHA256SUMS.asc` for the builds, and GPG-sign it:

```bash
sha256sum * > SHA256SUMS
```

The list of files should be:
```
<<<<<<< HEAD
prcycoin-${VERSION}-aarch64-linux-gnu.tar.gz
prcycoin-${VERSION}-arm-linux-gnueabihf.tar.gz
prcycoin-${VERSION}-i686-pc-linux-gnu.tar.gz
prcycoin-${VERSION}-x86_64-linux-gnu.tar.gz
prcycoin-${VERSION}-osx64.tar.gz
prcycoin-${VERSION}-osx.dmg
prcycoin-${VERSION}.tar.gz
prcycoin-${VERSION}-win32-setup.exe
prcycoin-${VERSION}-win32.zip
prcycoin-${VERSION}-win64-setup.exe
prcycoin-${VERSION}-win64.zip
=======
pivx-${VERSION}-aarch64-linux-gnu.tar.gz
pivx-${VERSION}-arm-linux-gnueabihf.tar.gz
pivx-${VERSION}-i686-pc-linux-gnu.tar.gz
pivx-${VERSION}-riscv64-linux-gnu.tar.gz
pivx-${VERSION}-x86_64-linux-gnu.tar.gz
pivx-${VERSION}-osx64.tar.gz
pivx-${VERSION}-osx.dmg
pivx-${VERSION}.tar.gz
pivx-${VERSION}-win64-setup.exe
pivx-${VERSION}-win64.zip
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
```
The `*-debug*` files generated by the gitian build contain debug symbols
for troubleshooting by developers. It is assumed that anyone that is interested
in debugging can run gitian to generate the files for themselves. To avoid
end-user confusion about which file to pick, as well as save storage
<<<<<<< HEAD
space *do not upload these to the prcycoin.com server*.
=======
space *do not upload these to github*.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

- GPG-sign it, delete the unsigned file:
```
gpg --digest-algo sha256 --clearsign SHA256SUMS # outputs SHA256SUMS.asc
rm SHA256SUMS
```
(the digest algorithm is forced to sha256 to avoid confusion of the `Hash:` header that GPG adds with the SHA256 used for the files)
Note: check that SHA256SUMS itself doesn't end up in SHA256SUMS, which is a spurious/nonsensical entry.

- Upload zips and installers, as well as `SHA256SUMS.asc` from last step, to the GitHub release (see below)

- Announce the release:

  - bitcointalk announcement thread

<<<<<<< HEAD
  - Optionally twitter, reddit /r/prcycoin, ... but this will usually sort out itself

  - Archive release notes for the new version to `doc/release-notes/` (branch `master` and branch of the release)

  - Create a [new GitHub release](https://github.com/PRCYcoin-Project/PRCYcoin/releases/new) with a link to the archived release notes.
=======
  - Optionally twitter, reddit /r/pivx, ... but this will usually sort out itself

  - Archive release notes for the new version to `doc/release-notes/` (branch `master` and branch of the release)

  - Create a [new GitHub release](https://github.com/PIVX-Project/PIVX/releases/new) with a link to the archived release notes.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

  - Celebrate
