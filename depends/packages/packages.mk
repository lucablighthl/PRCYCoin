<<<<<<< HEAD
packages:=boost openssl libevent

qt_native_packages = native_protobuf
qt_packages = protobuf zlib

qrencode_packages = qrencode

qt_linux_packages:=qt expat libxcb xcb_proto libXau xproto freetype fontconfig

qt_android_packages=qt
=======
packages:=boost libevent gmp $(zcash_packages) libsodium
native_packages := native_rust

qt_packages = qrencode zlib

qt_linux_packages:=qt expat dbus libxcb xcb_proto libXau xproto freetype fontconfig

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
qt_darwin_packages=qt
qt_mingw32_packages=qt

wallet_packages=bdb

zmq_packages=zeromq

upnp_packages=miniupnpc
<<<<<<< HEAD

darwin_native_packages = native_biplist native_ds_store native_mac_alias
=======
natpmp_packages=libnatpmp

darwin_native_packages = native_ds_store native_mac_alias

$(host_arch)_$(host_os)_native_packages += native_b2
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

ifneq ($(build_os),darwin)
darwin_native_packages += native_cctools native_cdrkit native_libdmg-hfsplus
endif
