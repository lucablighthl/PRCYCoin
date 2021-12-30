UNIX BUILD NOTES
====================
<<<<<<< HEAD
Some notes on how to build PRCY in Unix.

Note
---------------------
Always use absolute paths to configure and compile prcycoin and the dependencies,
for example, when specifying the the path of the dependency:

	../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX

Here BDB_PREFIX must absolute path - it is defined using $(pwd) which ensures
=======
Some notes on how to build PIVX Core in Unix.

Note
---------------------
Always use absolute paths to configure and compile PIVX Core and the dependencies,
For example, when specifying the path of the dependency:

	../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX

Here BDB_PREFIX must be an absolute path - it is defined using $(pwd) which ensures
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
the usage of the absolute path.

To Build
---------------------

```bash
./autogen.sh
./configure
make
make install # optional
```

<<<<<<< HEAD
This will build prcycoin-qt as well if the dependencies are met.
=======
This will build pivx-qt as well, if the dependencies are met.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Dependencies
---------------------

These dependencies are required:

<<<<<<< HEAD
 Library     | Purpose          | Description
 ------------|------------------|----------------------
 libssl      | SSL Support      | Secure communications
 libboost    | Boost            | C++ Library
 libevent    | Events           | Asynchronous event notification
=======
 Library     | Purpose            | Description
 ------------|--------------------|----------------------
 libboost    | Utility            | Library for threading, data structures, etc
 libevent    | Networking         | OS independent asynchronous networking
 libgmp      | Bignum Arithmetic  | Precision arithmetic
 libsodium   | Sapling Crypto     | A modern, portable, easy to use crypto library
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Optional dependencies:

 Library     | Purpose          | Description
 ------------|------------------|----------------------
 miniupnpc   | UPnP Support     | Firewall-jumping support
<<<<<<< HEAD
 libdb4.8    | Berkeley DB      | Wallet storage (only needed when wallet enabled)
 qt          | GUI              | GUI toolkit (only needed when GUI enabled)
 protobuf    | Payments in GUI  | Data interchange format used for payment protocol (only needed when GUI enabled)
 libqrencode | QR codes in GUI  | Optional for generating QR codes (only needed when GUI enabled)
 univalue    | Utility          | JSON parsing and encoding (bundled version will be used unless --with-system-univalue passed to configure)

For the versions used in the release, see [release-process.md](release-process.md) under *Fetch and build inputs*.

System requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1 GB of
memory available when compiling PRCYcoin. With 512MB of memory or less
compilation will take much longer due to swap thrashing.

Dependency Build Instructions: Ubuntu & Debian
----------------------------------------------
Build requirements:

	sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev libevent-dev

For Ubuntu 12.04 and later or Debian 7 and later libboost-all-dev has to be installed:

	sudo apt-get install libboost-all-dev

 db4.8 packages are available [here](https://launchpad.net/~bitcoin/+archive/bitcoin).
 You can add the repository using the following command:

        sudo add-apt-repository ppa:bitcoin/bitcoin
        sudo apt-get update

 Ubuntu 12.04 and later have packages for libdb5.1-dev and libdb5.1++-dev,
 but using these will break binary wallet compatibility, and is not recommended.

For other Debian & Ubuntu (with ppa):

	sudo apt-get install libdb4.8-dev libdb4.8++-dev

Optional:

	sudo apt-get install libminiupnpc-dev (see --with-miniupnpc and --enable-upnp-default)

Dependencies for the GUI: Ubuntu & Debian
-----------------------------------------

If you want to build PRCYcoin-Qt, make sure that the required packages for Qt development
are installed. Qt 5 is necessary to build the GUI.
If both Qt 4 and Qt 5 are installed, Qt 5 will be used.
To build without GUI pass `--without-gui`.

For Qt 5 you need the following:

    sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler

libqrencode (optional) can be installed with:

    sudo apt-get install libqrencode-dev

Once these are installed, they will be found by configure and a prcycoin-qt executable will be
built by default.

Notes
-----
The release is built with GCC and then "strip prcycoind" to strip the debug
=======
 libnatpmp   | NAT-PMP Support  | Firewall-jumping support
 libdb4.8    | Berkeley DB      | Wallet storage (only needed when wallet enabled)
 qt          | GUI              | GUI toolkit (only needed when GUI enabled)
 univalue    | Utility          | JSON parsing and encoding (bundled version will be used unless --with-system-univalue passed to configure)
 libzmq3     | ZMQ notification | Optional, allows generating ZMQ notifications (requires ZMQ version >= 4.0.0)

For the versions used, see [dependencies.md](dependencies.md)

Memory Requirements
--------------------

C++ compilers are memory-hungry. It is recommended to have at least 1.5 GB of
memory available when compiling PIVX Core. On systems with less, gcc can be
tuned to conserve memory with additional CXXFLAGS:


    ./configure CXXFLAGS="--param ggc-min-expand=1 --param ggc-min-heapsize=32768"


## Linux Distribution Specific Instructions

### Ubuntu & Debian

#### Dependency Build Instructions

Build requirements:

    sudo apt-get install build-essential libtool bsdmainutils autotools-dev autoconf pkg-config automake python3

Now, you can either build from self-compiled [depends](/depends/README.md) or install the required dependencies:

    sudo apt-get install libgmp-dev libevent-dev libboost-all-dev libsodium-dev cargo

BerkeleyDB is required for the wallet.

 **For Ubuntu only:** db4.8 packages are available [here](https://launchpad.net/~pivx/+archive/pivx).
 You can add the repository using the following command:

    sudo apt-get install software-properties-common
    sudo add-apt-repository ppa:pivx/pivx
    sudo apt-get update
    sudo apt-get install libdb4.8-dev libdb4.8++-dev

Ubuntu and Debian have their own libdb-dev and libdb++-dev packages, but these will install
BerkeleyDB 5.1 or later. This will break binary wallet compatibility with the distributed executables, which
are based on BerkeleyDB 4.8. If you do not care about wallet compatibility,
pass `--with-incompatible-bdb` to configure.

Otherwise, you can build from self-compiled `depends` (see above).

To build PIVX Core without wallet, see [*Disable-wallet mode*](/doc/build-unix.md#disable-wallet-mode)


Optional port mapping libraries (see: `--with-miniupnpc`, and `--enable-upnp-default`, `--with-natpmp`, `--enable-natpmp-default`):

    sudo apt install libminiupnpc-dev libnatpmp-dev

ZMQ dependencies (provides ZMQ API):

    sudo apt-get install libzmq3-dev

GUI dependencies:

If you want to build pivx-qt, make sure that the required packages for Qt development
are installed. Qt 5 is necessary to build the GUI.
To build without GUI pass `--without-gui`.

To build with Qt 5 you need the following:

    sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 libqt5svg5-dev libqt5charts5-dev qttools5-dev qttools5-dev-tools libqrencode-dev

**Note:** Ubuntu versions prior to Bionic (18.04), and Debian version prior to Buster, do not have the `libqt5charts5-dev` package. If you are compiling on one of these older versions, you will need to omit `libqt5charts5-dev` from the above command.

Once these are installed, they will be found by configure and a pivx-qt executable will be
built by default.


### Fedora

#### Dependency Build Instructions

Build requirements:

    sudo dnf install which gcc-c++ libtool make autoconf automake libevent-devel boost-devel libdb4-devel libdb4-cxx-devel gmp-devel libsodium-devel cargo python3

Optional:

    sudo dnf install miniupnpc-devel libnatpmp-devel zeromq-devel

To build with Qt 5 you need the following:

    sudo dnf install qt5-qttools-devel qt5-qtbase-devel qt5-qtsvg-devel qt5-qtcharts-devel qrencode-devel

Notes
-----
The release is built with GCC and then "strip pivxd" to strip the debug
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
symbols, which reduces the executable size by about 90%.


miniupnpc
---------

[miniupnpc](http://miniupnp.free.fr/) may be used for UPnP port mapping.  It can be downloaded from [here](
http://miniupnp.tuxfamily.org/files/).  UPnP support is compiled in and
<<<<<<< HEAD
turned off by default.  See the configure options for upnp behavior desired:

	--without-miniupnpc      No UPnP support miniupnp not required
	--disable-upnp-default   (the default) UPnP support turned off by default at runtime
	--enable-upnp-default    UPnP support turned on by default at runtime

To build:

Before you start building, please make sure that your compiler supports C++14.

	tar -xzvf miniupnpc-1.6.tar.gz
	cd miniupnpc-1.6
	make
	sudo su
	make install
=======
turned off by default.  See the configure options for UPnp behavior desired:

	--without-miniupnpc      No UPnP support, miniupnp not required
	--disable-upnp-default   (the default) UPnP support turned off by default at runtime
	--enable-upnp-default    UPnP support turned on by default at runtime

libnatpmp
---------

[libnatpmp](https://miniupnp.tuxfamily.org/libnatpmp.html) may be used for NAT-PMP port mapping. It can be downloaded
from [here](https://miniupnp.tuxfamily.org/files/). NAT-PMP support is compiled in and
turned off by default. See the configure options for NAT-PMP behavior desired:

	--without-natpmp          No NAT-PMP support, libnatpmp not required
	--disable-natpmp-default  (the default) NAT-PMP support turned off by default at runtime
	--enable-natpmp-default   NAT-PMP support turned on by default at runtime

To build:

    tar -xzvf miniupnpc-1.6.tar.gz
    cd miniupnpc-1.6
    make
    sudo su
    make install
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e


Berkeley DB
-----------
<<<<<<< HEAD
It is recommended to use Berkeley DB 4.8. If you have to build it yourself:

```bash
PRCYcoin_ROOT=$(pwd)

# Pick some path to install BDB to, here we create a directory within the prcycoin directory
BDB_PREFIX="${PRCYcoin_ROOT}/db4"
mkdir -p $BDB_PREFIX

# Fetch the source and verify that it is not tampered with
wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
echo '12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef  db-4.8.30.NC.tar.gz' | sha256sum -c
# -> db-4.8.30.NC.tar.gz: OK
tar -xzvf db-4.8.30.NC.tar.gz

# Build the library and install to our prefix
cd db-4.8.30.NC/build_unix/
#  Note: Do a static build so that it can be embedded into the exectuable, instead of having to find a .so at runtime
../dist/configure --enable-cxx --disable-shared --with-pic --prefix=$BDB_PREFIX
make install

# Configure PRCY to use our own-built instance of BDB
cd $PRCYcoin_ROOT
./configure (other args...) LDFLAGS="-L${BDB_PREFIX}/lib/" CPPFLAGS="-I${BDB_PREFIX}/include/"
```

**Note**: You only need Berkeley DB if the wallet is enabled (see the section *Disable-Wallet mode* below).
=======
It is recommended to use Berkeley DB 4.8. If you have to build it yourself,
you can use [the installation script included in contrib/](/contrib/install_db4.sh)
like so:

```shell
./contrib/install_db4.sh `pwd`
```

from the root of the repository.

**Note**: You only need Berkeley DB if the wallet is enabled (see [*Disable-wallet mode*](/doc/build-unix.md#disable-wallet-mode)).
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

Boost
-----
If you need to build Boost yourself:

	sudo su
	./bootstrap.sh
	./bjam install


Security
--------
<<<<<<< HEAD
To help make your PRCY installation more secure by making certain attacks impossible to
=======
To help make your PIVX Core installation more secure by making certain attacks impossible to
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
exploit even if a vulnerability is found, binaries are hardened by default.
This can be disabled with:

Hardening Flags:

	./configure --enable-hardening
	./configure --disable-hardening


Hardening enables the following features:
<<<<<<< HEAD

* Position Independent Executable
    Build position independent code to take advantage of Address Space Layout Randomization
    offered by some kernels. An attacker who is able to cause execution of code at an arbitrary
    memory location is thwarted if he doesn't know where anything useful is located.
    The stack and heap are randomly located by default but this allows the code section to be
    randomly located as well.

    On an Amd64 processor where a library was not compiled with -fPIC, this will cause an error
=======
* _Position Independent Executable_: Build position independent code to take advantage of Address Space Layout Randomization
    offered by some kernels. Attackers who can cause execution of code at an arbitrary memory
    location are thwarted if they don't know where anything useful is located.
    The stack and heap are randomly located by default, but this allows the code section to be
    randomly located as well.

    On an AMD64 processor where a library was not compiled with -fPIC, this will cause an error
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    such as: "relocation R_X86_64_32 against `......' can not be used when making a shared object;"

    To test that you have built PIE executable, install scanelf, part of paxutils, and use:

<<<<<<< HEAD
    	scanelf -e ./prcycoind

    The output should contain:
     TYPE
    ET_DYN

* Non-executable Stack
    If the stack is executable then trivial stack based buffer overflow exploits are possible if
    vulnerable buffers are found. By default, prcycoin should be built with a non-executable stack
=======
    	scanelf -e ./pivxd

    The output should contain:

     TYPE
    ET_DYN

* _Non-executable Stack_: If the stack is executable then trivial stack-based buffer overflow exploits are possible if
    vulnerable buffers are found. By default, PIVX Core should be built with a non-executable stack
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    but if one of the libraries it uses asks for an executable stack or someone makes a mistake
    and uses a compiler extension which requires an executable stack, it will silently build an
    executable without the non-executable stack protection.

    To verify that the stack is non-executable after compiling use:
<<<<<<< HEAD
    `scanelf -e ./prcycoind`

    the output should contain:
=======
    `scanelf -e ./pivxd`

    The output should contain:
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
	STK/REL/PTL
	RW- R-- RW-

    The STK RW- means that the stack is readable and writeable but not executable.
<<<<<<< HEAD
=======

Disable-wallet mode
--------------------
**Note:** This functionality is not yet completely implemented, and compilation using the below option will currently fail.

When the intention is to run only a P2P node without a wallet, PIVX Core may be compiled in
disable-wallet mode with:

    ./configure --disable-wallet

In this case there is no dependency on Berkeley DB 4.8.


Additional Configure Flags
--------------------------
A list of additional configure flags can be displayed with:

    ./configure --help


ARM Cross-compilation
-------------------
These steps can be performed on, for example, an Ubuntu VM. The depends system
will also work on other Linux distributions, however the commands for
installing the toolchain will be different.

Make sure you install the build requirements mentioned above.
Then, install the toolchain and curl:

    sudo apt-get install g++-arm-linux-gnueabihf curl

To build executables for ARM:

    cd depends
    make HOST=arm-linux-gnueabihf NO_QT=1
    cd ..
    ./autogen.sh
    ./configure --prefix=$PWD/depends/arm-linux-gnueabihf --enable-glibc-back-compat --enable-reduce-exports LDFLAGS=-static-libstdc++
    make


For further documentation on the depends system see [README.md](../depends/README.md) in the depends directory.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
