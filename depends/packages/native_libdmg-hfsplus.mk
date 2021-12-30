package=native_libdmg-hfsplus
<<<<<<< HEAD
$(package)_version=0.1
$(package)_download_path=https://github.com/theuni/libdmg-hfsplus/archive
$(package)_file_name=libdmg-hfsplus-v$($(package)_version).tar.gz
$(package)_sha256_hash=6569a02eb31c2827080d7d59001869ea14484c281efab0ae7f2b86af5c3120b3
$(package)_build_subdir=build

define $(package)_preprocess_cmds
=======
$(package)_version=7ac55ec64c96f7800d9818ce64c79670e7f02b67
$(package)_download_path=https://github.com/planetbeing/libdmg-hfsplus/archive
$(package)_file_name=$($(package)_version).tar.gz
$(package)_sha256_hash=56fbdc48ec110966342f0ecddd6f8f89202f4143ed2a3336e42bbf88f940850c
$(package)_build_subdir=build
$(package)_patches=remove-libcrypto-dependency.patch

define $(package)_preprocess_cmds
  patch -p1 < $($(package)_patch_dir)/remove-libcrypto-dependency.patch && \
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
  mkdir build
endef

define $(package)_config_cmds
<<<<<<< HEAD
  cmake -DCMAKE_INSTALL_PREFIX:PATH=$(build_prefix)/bin -DCMAKE_C_FLAGS="-Wl,--build-id=none" ..
=======
  cmake -DCMAKE_INSTALL_PREFIX:PATH=$(build_prefix) ..
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
endef

define $(package)_build_cmds
  $(MAKE) -C dmg
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) -C dmg install
endef
