<<<<<<< HEAD
PRCY version 1.0.3 is now available from:

  <https://github.com/PRCYCoin/PRCYCoin/releases>

This is a new minor-revision version release, including various bug fixes and
performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github:

  <https://github.com/PRCYCoin/PRCYCoin/issues>
=======
(note: this is a temporary file, to be added-to by anybody, and moved to release-notes at release time)

PIVX Core version *version* is now available from:  <https://github.com/pivx-project/pivx/releases>

This is a new major version release, including various bug fixes and performance improvements, as well as updated translations.

Please report bugs using the issue tracker at github: <https://github.com/pivx-project/pivx/issues>
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e


How to Upgrade
==============

<<<<<<< HEAD
If you are running an older version, shut it down. Wait until it has completely shut down (which might take a few minutes for older versions), then run the installer (on Windows) or just copy over /Applications/PRCYcoin-Qt (on Mac) or prcycoind/prcycoin-qt (on Linux).


Compatibility
==============

PRCY is extensively tested on multiple operating systems using
the Linux kernel, macOS 10.8+, and Windows Vista and later.

Microsoft ended support for Windows XP on [April 8th, 2014](https://www.microsoft.com/en-us/WindowsForBusiness/end-of-xp-support),
No attempt is made to prevent installing or running the software on Windows XP, you
can still do so at your own risk but be aware that there are known instabilities and issues.
Please do not report issues about Windows XP to the issue tracker.

PRCY should also work on most other Unix-like systems but is not
frequently tested on them.


Notable Changes
===============

Automated Database Corruption Repair
---------------------
There have been cases of blockchain database corruption that can occur when PRCY client is not closed gracefully. The most common cases of corruption have been identified and the wallet will now automatically fix most of these corruptions. Certain corruption states are still unable to be fixed, but now provide more detailed error messages to the user as well as prompting the user to reindex their database.

More Accurate Error Messages
---------------------
Some error messages in the wallet have been too vague and done little to help developers and the support team properly identify issues. Error messages have been refined and are now more specific.

Reduction of Debug Log Spam
---------------------
Many 3rd party services have reported that their debug logs have been overloaded with messages about unknown transaction types. This log spam has been fixed.

Removal of Heavy Running Transaction Search Code
---------------------
Many areas of the block validation code use a "slow" transaction search, which searches redundantly for transactions. This "slow" search has been removed upstream in Bitcoin and is now removed in PRCYcoin. This provides a more efficient syncing process and generally better performing wallet.

Working Testnet
---------------------
Testnet is now accessible with this release of the wallet. Testnet can be accessed using the `-testnet` startup flag.

Credits
=======

Thanks to everyone who directly contributed to this release:
- Fuzzbawls
- Jon Spock
- Mrs-X
- Patrick Collins
- PeterL73
- presstab
- sonic
- whateverpal

As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/prcycoin-project-translations/).
=======
If you are running an older version, shut it down. Wait until it has completely shut down (which might take a few minutes for older versions), then run the installer (on Windows) or just copy over /Applications/PIVX-Qt (on Mac) or pivxd/pivx-qt (on Linux).

Notable Changes
==============

(Developers: add your notes here as part of your pull requests whenever possible)


### Deprecated autocombinerewards RPC Command

The `autocombinerewards` RPC command was soft-deprecated in v5.3.0 and replaced with explicit setter/getter commands `setautocombinethreshold`/`getautocombinethreshold`. PIVX Core, by default, will no longer accept the `autocombinerewards` command, returning a deprecation error, unless the `pivxd`/`pivx-qt` is started with the `-deprecatedrpc=autocombinerewards` option.

This command will be fully removed in v6.0.0.

### Shield address support for RPC label commands

The `setlabel` RPC command now supports a shield address input argument to allow users to set labels for shield addresses. Additionally, the `getaddressesbylabel` RPC command will also now return shield addresses with a matching label.

### Specify optional label for getnewshieldaddress

The `getnewshieldaddress` RPC command now takes an optional argument `label (string)` to denote the desired label for the generated address.

P2P connection management
--------------------------

- Peers manually added through the addnode option or addnode RPC now have their own
  limit of sixteen connections which does not compete with other inbound or outbound
  connection usage and is not subject to the maxconnections limitation.

- New connections to manually added peers are much faster.

*version* Change log
==============

Detailed release notes follow. This overview includes changes that affect behavior, not code moves, refactors and string updates. For convenience in locating the code changes and accompanying discussion, both the pull request and git merge commit are mentioned.

### Core Features

### Build System

### P2P Protocol and Network Code

### GUI

### RPC/REST

### Wallet

### Miscellaneous

## Credits

Thanks to everyone who directly contributed to this release:


As well as everyone that helped translating on [Transifex](https://www.transifex.com/projects/p/pivx-project-translations/), the QA team during Testing and the Node hosts supporting our Testnet.
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
