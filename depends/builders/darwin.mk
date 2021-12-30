<<<<<<< HEAD
build_darwin_CC:=$(shell xcrun -f clang)
build_darwin_CXX:=$(shell xcrun -f clang++)
=======
build_darwin_CC:=$(shell xcrun -f clang) -isysroot$(shell xcrun --show-sdk-path)
build_darwin_CXX:=$(shell xcrun -f clang++) -isysroot$(shell xcrun --show-sdk-path)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
build_darwin_AR:=$(shell xcrun -f ar)
build_darwin_RANLIB:=$(shell xcrun -f ranlib)
build_darwin_STRIP:=$(shell xcrun -f strip)
build_darwin_OTOOL:=$(shell xcrun -f otool)
build_darwin_NM:=$(shell xcrun -f nm)
build_darwin_INSTALL_NAME_TOOL:=$(shell xcrun -f install_name_tool)
build_darwin_SHA256SUM=shasum -a 256
build_darwin_DOWNLOAD=curl --location --fail --connect-timeout $(DOWNLOAD_CONNECT_TIMEOUT) --retry $(DOWNLOAD_RETRIES) -o

#darwin host on darwin builder. overrides darwin host preferences.
<<<<<<< HEAD
darwin_CC=$(shell xcrun -f clang) -mmacosx-version-min=$(OSX_MIN_VERSION) --sysroot $(shell xcrun --show-sdk-path)
darwin_CXX:=$(shell xcrun -f clang++) -mmacosx-version-min=$(OSX_MIN_VERSION) -stdlib=libc++ --sysroot $(shell xcrun --show-sdk-path)
=======
darwin_CC=$(shell xcrun -f clang) -mmacosx-version-min=$(OSX_MIN_VERSION) -isysroot$(shell xcrun --show-sdk-path)
darwin_CXX:=$(shell xcrun -f clang++) -mmacosx-version-min=$(OSX_MIN_VERSION) -stdlib=libc++ -isysroot$(shell xcrun --show-sdk-path)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
darwin_AR:=$(shell xcrun -f ar)
darwin_RANLIB:=$(shell xcrun -f ranlib)
darwin_STRIP:=$(shell xcrun -f strip)
darwin_LIBTOOL:=$(shell xcrun -f libtool)
darwin_OTOOL:=$(shell xcrun -f otool)
darwin_NM:=$(shell xcrun -f nm)
darwin_INSTALL_NAME_TOOL:=$(shell xcrun -f install_name_tool)
<<<<<<< HEAD
=======
darwin_native_binutils=
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
darwin_native_toolchain=
