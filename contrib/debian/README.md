
Debian
====================
<<<<<<< HEAD
This directory contains files used to package prcycoind/prcycoin-qt
for Debian-based Linux systems. If you compile prcycoind/prcycoin-qt yourself, there are some useful files here.

## prcycoin: URI support ##


prcycoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install prcycoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your prcycoinqt binary to `/usr/bin`
and the `../../share/pixmaps/prcycoin128.png` to `/usr/share/pixmaps`

prcycoin-qt.protocol (KDE)
=======
This directory contains files used to package pivxd/pivx-qt
for Debian-based Linux systems. If you compile pivxd/pivx-qt yourself, there are some useful files here.

## pivx: URI support ##


pivx-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install pivx-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your pivx-qt binary to `/usr/bin`
and the `../../share/pixmaps/pivx128.png` to `/usr/share/pixmaps`

pivx-qt.protocol (KDE)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

