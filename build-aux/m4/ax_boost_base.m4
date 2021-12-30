# ===========================================================================
<<<<<<< HEAD
#      https://www.gnu.org/software/autoconf-archive/ax_boost_base.html
=======
#       http://www.gnu.org/software/autoconf-archive/ax_boost_base.html
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
# ===========================================================================
#
# SYNOPSIS
#
#   AX_BOOST_BASE([MINIMUM-VERSION], [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   Test for the Boost C++ libraries of a particular version (or newer)
#
<<<<<<< HEAD
#   If no path to the installed boost library is given the macro searchs
#   under /usr, /usr/local, /opt and /opt/local and evaluates the
=======
#   If no path to the installed boost library is given the macro search
#   under /usr, /usr/local, /opt, /opt/local, and /opt/homebrew and evaluates the
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
#   $BOOST_ROOT environment variable. Further documentation is available at
#   <http://randspringer.de/boost/index.html>.
#
#   This macro calls:
#
#     AC_SUBST(BOOST_CPPFLAGS) / AC_SUBST(BOOST_LDFLAGS)
#
#   And sets:
#
#     HAVE_BOOST
#
# LICENSE
#
#   Copyright (c) 2008 Thomas Porschberg <thomas@randspringer.de>
#   Copyright (c) 2009 Peter Adolphs
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

<<<<<<< HEAD
#serial 49

# example boost program (need to pass version)
m4_define([_AX_BOOST_BASE_PROGRAM],
          [AC_LANG_PROGRAM([[
#include <boost/version.hpp>
]],[[
(void) ((void)sizeof(char[1 - 2*!!((BOOST_VERSION) < ($1))]));
]])])
=======
#serial 27
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

AC_DEFUN([AX_BOOST_BASE],
[
AC_ARG_WITH([boost],
  [AS_HELP_STRING([--with-boost@<:@=ARG@:>@],
    [use Boost library from a standard location (ARG=yes),
     from the specified location (ARG=<path>),
     or disable it (ARG=no)
     @<:@ARG=yes@:>@ ])],
    [
<<<<<<< HEAD
     AS_CASE([$withval],
       [no],[want_boost="no";_AX_BOOST_BASE_boost_path=""],
       [yes],[want_boost="yes";_AX_BOOST_BASE_boost_path=""],
       [want_boost="yes";_AX_BOOST_BASE_boost_path="$withval"])
=======
    if test "$withval" = "no"; then
        want_boost="no"
    elif test "$withval" = "yes"; then
        want_boost="yes"
        ac_boost_path=""
    else
        want_boost="yes"
        ac_boost_path="$withval"
    fi
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    ],
    [want_boost="yes"])


AC_ARG_WITH([boost-libdir],
<<<<<<< HEAD
  [AS_HELP_STRING([--with-boost-libdir=LIB_DIR],
    [Force given directory for boost libraries.
     Note that this will override library path detection,
     so use this parameter only if default library detection fails
     and you know exactly where your boost libraries are located.])],
  [
   AS_IF([test -d "$withval"],
         [_AX_BOOST_BASE_boost_lib_path="$withval"],
    [AC_MSG_ERROR([--with-boost-libdir expected directory name])])
  ],
  [_AX_BOOST_BASE_boost_lib_path=""])

BOOST_LDFLAGS=""
BOOST_CPPFLAGS=""
AS_IF([test "x$want_boost" = "xyes"],
      [_AX_BOOST_BASE_RUNDETECT([$1],[$2],[$3])])
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LDFLAGS)
])


# convert a version string in $2 to numeric and affect to polymorphic var $1
AC_DEFUN([_AX_BOOST_BASE_TONUMERICVERSION],[
  AS_IF([test "x$2" = "x"],[_AX_BOOST_BASE_TONUMERICVERSION_req="1.20.0"],[_AX_BOOST_BASE_TONUMERICVERSION_req="$2"])
  _AX_BOOST_BASE_TONUMERICVERSION_req_shorten=`expr $_AX_BOOST_BASE_TONUMERICVERSION_req : '\([[0-9]]*\.[[0-9]]*\)'`
  _AX_BOOST_BASE_TONUMERICVERSION_req_major=`expr $_AX_BOOST_BASE_TONUMERICVERSION_req : '\([[0-9]]*\)'`
  AS_IF([test "x$_AX_BOOST_BASE_TONUMERICVERSION_req_major" = "x"],
        [AC_MSG_ERROR([You should at least specify libboost major version])])
  _AX_BOOST_BASE_TONUMERICVERSION_req_minor=`expr $_AX_BOOST_BASE_TONUMERICVERSION_req : '[[0-9]]*\.\([[0-9]]*\)'`
  AS_IF([test "x$_AX_BOOST_BASE_TONUMERICVERSION_req_minor" = "x"],
        [_AX_BOOST_BASE_TONUMERICVERSION_req_minor="0"])
  _AX_BOOST_BASE_TONUMERICVERSION_req_sub_minor=`expr $_AX_BOOST_BASE_TONUMERICVERSION_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
  AS_IF([test "X$_AX_BOOST_BASE_TONUMERICVERSION_req_sub_minor" = "X"],
        [_AX_BOOST_BASE_TONUMERICVERSION_req_sub_minor="0"])
  _AX_BOOST_BASE_TONUMERICVERSION_RET=`expr $_AX_BOOST_BASE_TONUMERICVERSION_req_major \* 100000 \+  $_AX_BOOST_BASE_TONUMERICVERSION_req_minor \* 100 \+ $_AX_BOOST_BASE_TONUMERICVERSION_req_sub_minor`
  AS_VAR_SET($1,$_AX_BOOST_BASE_TONUMERICVERSION_RET)
])

dnl Run the detection of boost should be run only if $want_boost
AC_DEFUN([_AX_BOOST_BASE_RUNDETECT],[
    _AX_BOOST_BASE_TONUMERICVERSION(WANT_BOOST_VERSION,[$1])
    succeeded=no


    AC_REQUIRE([AC_CANONICAL_HOST])
=======
        AS_HELP_STRING([--with-boost-libdir=LIB_DIR],
        [Force given directory for boost libraries. Note that this will override library path detection, so use this parameter only if default library detection fails and you know exactly where your boost libraries are located.]),
        [
        if test -d "$withval"
        then
                ac_boost_lib_path="$withval"
        else
                AC_MSG_ERROR(--with-boost-libdir expected directory name)
        fi
        ],
        [ac_boost_lib_path=""]
)

if test "x$want_boost" = "xyes"; then
    boost_lib_version_req=ifelse([$1], ,1.20.0,$1)
    boost_lib_version_req_shorten=`expr $boost_lib_version_req : '\([[0-9]]*\.[[0-9]]*\)'`
    boost_lib_version_req_major=`expr $boost_lib_version_req : '\([[0-9]]*\)'`
    boost_lib_version_req_minor=`expr $boost_lib_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
    boost_lib_version_req_sub_minor=`expr $boost_lib_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
    if test "x$boost_lib_version_req_sub_minor" = "x" ; then
        boost_lib_version_req_sub_minor="0"
        fi
    WANT_BOOST_VERSION=`expr $boost_lib_version_req_major \* 100000 \+  $boost_lib_version_req_minor \* 100 \+ $boost_lib_version_req_sub_minor`
    AC_MSG_CHECKING(for boostlib >= $boost_lib_version_req)
    succeeded=no

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    dnl On 64-bit systems check for system libraries in both lib64 and lib.
    dnl The former is specified by FHS, but e.g. Debian does not adhere to
    dnl this (as it rises problems for generic multi-arch support).
    dnl The last entry in the list is chosen by default when no libraries
    dnl are found, e.g. when only header-only libraries are installed!
<<<<<<< HEAD
    AS_CASE([${host_cpu}],
      [x86_64],[libsubdirs="lib64 libx32 lib lib64"],
      [mips*64*],[libsubdirs="lib64 lib32 lib lib64"],
      [ppc64|powerpc64|s390x|sparc64|aarch64|ppc64le|powerpc64le|riscv64],[libsubdirs="lib64 lib lib64"],
      [libsubdirs="lib"]
    )
=======
    libsubdirs="lib"
    ax_arch=`uname -m`
    case $ax_arch in
      x86_64)
        libsubdirs="lib64 libx32 lib lib64"
        ;;
      ppc64|s390x|sparc64|aarch64|ppc64le)
        libsubdirs="lib64 lib lib64"
        ;;
    esac
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    dnl allow for real multi-arch paths e.g. /usr/lib/x86_64-linux-gnu. Give
    dnl them priority over the other paths since, if libs are found there, they
    dnl are almost assuredly the ones desired.
<<<<<<< HEAD
    AS_CASE([${host_cpu}],
      [i?86],[multiarch_libsubdir="lib/i386-${host_os}"],
      [multiarch_libsubdir="lib/${host_cpu}-${host_os}"]
    )
=======
    AC_REQUIRE([AC_CANONICAL_HOST])
    libsubdirs="lib/${host_cpu}-${host_os} $libsubdirs"

    case ${host_cpu} in
      i?86)
        libsubdirs="lib/i386-${host_os} $libsubdirs"
        ;;
    esac
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

    dnl some arches may advertise a cpu type that doesn't line up with their
    dnl prefix's cpu type. For example, uname may report armv7l while libs are
    dnl installed to /usr/lib/arm-linux-gnueabihf. Try getting the compiler's
    dnl value for an extra chance of finding the correct path.
    libsubdirs="lib/`$CXX -dumpmachine 2>/dev/null` $libsubdirs"

    dnl first we check the system location for boost libraries
<<<<<<< HEAD
    dnl this location is chosen if boost libraries are installed with the --layout=system option
    dnl or if you install boost with RPM
    AS_IF([test "x$_AX_BOOST_BASE_boost_path" != "x"],[
        AC_MSG_CHECKING([for boostlib >= $1 ($WANT_BOOST_VERSION) includes in "$_AX_BOOST_BASE_boost_path/include"])
         AS_IF([test -d "$_AX_BOOST_BASE_boost_path/include" && test -r "$_AX_BOOST_BASE_boost_path/include"],[
           AC_MSG_RESULT([yes])
           BOOST_CPPFLAGS="-I$_AX_BOOST_BASE_boost_path/include"
           for _AX_BOOST_BASE_boost_path_tmp in $multiarch_libsubdir $libsubdirs; do
                AC_MSG_CHECKING([for boostlib >= $1 ($WANT_BOOST_VERSION) lib path in "$_AX_BOOST_BASE_boost_path/$_AX_BOOST_BASE_boost_path_tmp"])
                AS_IF([test -d "$_AX_BOOST_BASE_boost_path/$_AX_BOOST_BASE_boost_path_tmp" && test -r "$_AX_BOOST_BASE_boost_path/$_AX_BOOST_BASE_boost_path_tmp" ],[
                        AC_MSG_RESULT([yes])
                        BOOST_LDFLAGS="-L$_AX_BOOST_BASE_boost_path/$_AX_BOOST_BASE_boost_path_tmp";
                        break;
                ],
      [AC_MSG_RESULT([no])])
           done],[
      AC_MSG_RESULT([no])])
    ],[
        if test X"$cross_compiling" = Xyes; then
            search_libsubdirs=$multiarch_libsubdir
        else
            search_libsubdirs="$multiarch_libsubdir $libsubdirs"
        fi
        for _AX_BOOST_BASE_boost_path_tmp in /usr /usr/local /opt /opt/local ; do
            if test -d "$_AX_BOOST_BASE_boost_path_tmp/include/boost" && test -r "$_AX_BOOST_BASE_boost_path_tmp/include/boost" ; then
                for libsubdir in $search_libsubdirs ; do
                    if ls "$_AX_BOOST_BASE_boost_path_tmp/$libsubdir/libboost_"* >/dev/null 2>&1 ; then break; fi
                done
                BOOST_LDFLAGS="-L$_AX_BOOST_BASE_boost_path_tmp/$libsubdir"
                BOOST_CPPFLAGS="-I$_AX_BOOST_BASE_boost_path_tmp/include"
                break;
            fi
        done
    ])

    dnl overwrite ld flags if we have required special directory with
    dnl --with-boost-libdir parameter
    AS_IF([test "x$_AX_BOOST_BASE_boost_lib_path" != "x"],
          [BOOST_LDFLAGS="-L$_AX_BOOST_BASE_boost_lib_path"])

    AC_MSG_CHECKING([for boostlib >= $1 ($WANT_BOOST_VERSION)])
=======
    dnl this location ist chosen if boost libraries are installed with the --layout=system option
    dnl or if you install boost with RPM
    if test "$ac_boost_path" != ""; then
        BOOST_CPPFLAGS="-I$ac_boost_path/include"
        for ac_boost_path_tmp in $libsubdirs; do
                if test -d "$ac_boost_path"/"$ac_boost_path_tmp" ; then
                        BOOST_LDFLAGS="-L$ac_boost_path/$ac_boost_path_tmp"
                        break
                fi
        done
    elif test "$cross_compiling" != yes; then
        for ac_boost_path_tmp in /usr /usr/local /opt /opt/local /opt/homebrew ; do
            if test -d "$ac_boost_path_tmp/include/boost" && test -r "$ac_boost_path_tmp/include/boost"; then
                for libsubdir in $libsubdirs ; do
                    if ls "$ac_boost_path_tmp/$libsubdir/libboost_"* >/dev/null 2>&1 ; then break; fi
                done
                BOOST_LDFLAGS="-L$ac_boost_path_tmp/$libsubdir"
                BOOST_CPPFLAGS="-I$ac_boost_path_tmp/include"
                break;
            fi
        done
    fi

    dnl overwrite ld flags if we have required special directory with
    dnl --with-boost-libdir parameter
    if test "$ac_boost_lib_path" != ""; then
       BOOST_LDFLAGS="-L$ac_boost_lib_path"
    fi

>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
    CPPFLAGS_SAVED="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
    export CPPFLAGS

    LDFLAGS_SAVED="$LDFLAGS"
    LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
    export LDFLAGS

    AC_REQUIRE([AC_PROG_CXX])
    AC_LANG_PUSH(C++)
<<<<<<< HEAD
        AC_COMPILE_IFELSE([_AX_BOOST_BASE_PROGRAM($WANT_BOOST_VERSION)],[
=======
        AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    @%:@include <boost/version.hpp>
    ]], [[
    #if BOOST_VERSION >= $WANT_BOOST_VERSION
    // Everything is okay
    #else
    #  error Boost version is too old
    #endif
    ]])],[
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        AC_MSG_RESULT(yes)
    succeeded=yes
    found_system=yes
        ],[
        ])
    AC_LANG_POP([C++])



    dnl if we found no boost with system layout we search for boost libraries
    dnl built and installed without the --layout=system option or for a staged(not installed) version
<<<<<<< HEAD
    if test "x$succeeded" != "xyes" ; then
        CPPFLAGS="$CPPFLAGS_SAVED"
        LDFLAGS="$LDFLAGS_SAVED"
        BOOST_CPPFLAGS=
        if test -z "$_AX_BOOST_BASE_boost_lib_path" ; then
            BOOST_LDFLAGS=
        fi
        _version=0
        if test -n "$_AX_BOOST_BASE_boost_path" ; then
            if test -d "$_AX_BOOST_BASE_boost_path" && test -r "$_AX_BOOST_BASE_boost_path"; then
                for i in `ls -d $_AX_BOOST_BASE_boost_path/include/boost-* 2>/dev/null`; do
                    _version_tmp=`echo $i | sed "s#$_AX_BOOST_BASE_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                    V_CHECK=`expr $_version_tmp \> $_version`
                    if test "x$V_CHECK" = "x1" ; then
                        _version=$_version_tmp
                    fi
                    VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
                    BOOST_CPPFLAGS="-I$_AX_BOOST_BASE_boost_path/include/boost-$VERSION_UNDERSCORE"
                done
                dnl if nothing found search for layout used in Windows distributions
                if test -z "$BOOST_CPPFLAGS"; then
                    if test -d "$_AX_BOOST_BASE_boost_path/boost" && test -r "$_AX_BOOST_BASE_boost_path/boost"; then
                        BOOST_CPPFLAGS="-I$_AX_BOOST_BASE_boost_path"
                    fi
                fi
                dnl if we found something and BOOST_LDFLAGS was unset before
                dnl (because "$_AX_BOOST_BASE_boost_lib_path" = ""), set it here.
                if test -n "$BOOST_CPPFLAGS" && test -z "$BOOST_LDFLAGS"; then
                    for libsubdir in $libsubdirs ; do
                        if ls "$_AX_BOOST_BASE_boost_path/$libsubdir/libboost_"* >/dev/null 2>&1 ; then break; fi
                    done
                    BOOST_LDFLAGS="-L$_AX_BOOST_BASE_boost_path/$libsubdir"
                fi
            fi
        else
            if test "x$cross_compiling" != "xyes" ; then
                for _AX_BOOST_BASE_boost_path in /usr /usr/local /opt /opt/local ; do
                    if test -d "$_AX_BOOST_BASE_boost_path" && test -r "$_AX_BOOST_BASE_boost_path" ; then
                        for i in `ls -d $_AX_BOOST_BASE_boost_path/include/boost-* 2>/dev/null`; do
                            _version_tmp=`echo $i | sed "s#$_AX_BOOST_BASE_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                            V_CHECK=`expr $_version_tmp \> $_version`
                            if test "x$V_CHECK" = "x1" ; then
                                _version=$_version_tmp
                                best_path=$_AX_BOOST_BASE_boost_path
=======
    if test "x$succeeded" != "xyes"; then
        CPPFLAGS="$CPPFLAGS_SAVED"
        LDFLAGS="$LDFLAGS_SAVED"
        BOOST_CPPFLAGS=
        BOOST_LDFLAGS=
        _version=0
        if test "$ac_boost_path" != ""; then
            if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
                for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                    _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                    V_CHECK=`expr $_version_tmp \> $_version`
                    if test "$V_CHECK" = "1" ; then
                        _version=$_version_tmp
                    fi
                    VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
                    BOOST_CPPFLAGS="-I$ac_boost_path/include/boost-$VERSION_UNDERSCORE"
                done
                dnl if nothing found search for layout used in Windows distributions
                if test -z "$BOOST_CPPFLAGS"; then
                    if test -d "$ac_boost_path/boost" && test -r "$ac_boost_path/boost"; then
                        BOOST_CPPFLAGS="-I$ac_boost_path"
                    fi
                fi
            fi
        else
            if test "$cross_compiling" != yes; then
                for ac_boost_path in /usr /usr/local /opt /opt/local /opt/homebrew ; do
                    if test -d "$ac_boost_path" && test -r "$ac_boost_path"; then
                        for i in `ls -d $ac_boost_path/include/boost-* 2>/dev/null`; do
                            _version_tmp=`echo $i | sed "s#$ac_boost_path##" | sed 's/\/include\/boost-//' | sed 's/_/./'`
                            V_CHECK=`expr $_version_tmp \> $_version`
                            if test "$V_CHECK" = "1" ; then
                                _version=$_version_tmp
                                best_path=$ac_boost_path
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                            fi
                        done
                    fi
                done

                VERSION_UNDERSCORE=`echo $_version | sed 's/\./_/'`
                BOOST_CPPFLAGS="-I$best_path/include/boost-$VERSION_UNDERSCORE"
<<<<<<< HEAD
                if test -z "$_AX_BOOST_BASE_boost_lib_path" ; then
=======
                if test "$ac_boost_lib_path" = ""; then
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                    for libsubdir in $libsubdirs ; do
                        if ls "$best_path/$libsubdir/libboost_"* >/dev/null 2>&1 ; then break; fi
                    done
                    BOOST_LDFLAGS="-L$best_path/$libsubdir"
                fi
            fi

<<<<<<< HEAD
            if test -n "$BOOST_ROOT" ; then
=======
            if test "x$BOOST_ROOT" != "x"; then
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                for libsubdir in $libsubdirs ; do
                    if ls "$BOOST_ROOT/stage/$libsubdir/libboost_"* >/dev/null 2>&1 ; then break; fi
                done
                if test -d "$BOOST_ROOT" && test -r "$BOOST_ROOT" && test -d "$BOOST_ROOT/stage/$libsubdir" && test -r "$BOOST_ROOT/stage/$libsubdir"; then
                    version_dir=`expr //$BOOST_ROOT : '.*/\(.*\)'`
                    stage_version=`echo $version_dir | sed 's/boost_//' | sed 's/_/./g'`
                        stage_version_shorten=`expr $stage_version : '\([[0-9]]*\.[[0-9]]*\)'`
                    V_CHECK=`expr $stage_version_shorten \>\= $_version`
<<<<<<< HEAD
                    if test "x$V_CHECK" = "x1" && test -z "$_AX_BOOST_BASE_boost_lib_path" ; then
=======
                    if test "$V_CHECK" = "1" -a "$ac_boost_lib_path" = "" ; then
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
                        AC_MSG_NOTICE(We will use a staged boost library from $BOOST_ROOT)
                        BOOST_CPPFLAGS="-I$BOOST_ROOT"
                        BOOST_LDFLAGS="-L$BOOST_ROOT/stage/$libsubdir"
                    fi
                fi
            fi
        fi

        CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
        export CPPFLAGS
        LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
        export LDFLAGS

        AC_LANG_PUSH(C++)
<<<<<<< HEAD
            AC_COMPILE_IFELSE([_AX_BOOST_BASE_PROGRAM($WANT_BOOST_VERSION)],[
=======
            AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
        @%:@include <boost/version.hpp>
        ]], [[
        #if BOOST_VERSION >= $WANT_BOOST_VERSION
        // Everything is okay
        #else
        #  error Boost version is too old
        #endif
        ]])],[
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
            AC_MSG_RESULT(yes)
        succeeded=yes
        found_system=yes
            ],[
            ])
        AC_LANG_POP([C++])
    fi

<<<<<<< HEAD
    if test "x$succeeded" != "xyes" ; then
        if test "x$_version" = "x0" ; then
            AC_MSG_NOTICE([[We could not detect the boost libraries (version $1 or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
=======
    if test "$succeeded" != "yes" ; then
        if test "$_version" = "0" ; then
            AC_MSG_NOTICE([[We could not detect the boost libraries (version $boost_lib_version_req_shorten or higher). If you have a staged boost library (still not installed) please specify \$BOOST_ROOT in your environment and do not give a PATH to --with-boost option.  If you are sure you have boost installed, then check your version number looking in <boost/version.hpp>. See http://randspringer.de/boost for more documentation.]])
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        else
            AC_MSG_NOTICE([Your boost libraries seems to old (version $_version).])
        fi
        # execute ACTION-IF-NOT-FOUND (if present):
        ifelse([$3], , :, [$3])
    else
<<<<<<< HEAD
=======
        AC_SUBST(BOOST_CPPFLAGS)
        AC_SUBST(BOOST_LDFLAGS)
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e
        AC_DEFINE(HAVE_BOOST,,[define if the Boost library is available])
        # execute ACTION-IF-FOUND (if present):
        ifelse([$2], , :, [$2])
    fi

    CPPFLAGS="$CPPFLAGS_SAVED"
    LDFLAGS="$LDFLAGS_SAVED"
<<<<<<< HEAD
=======
fi
>>>>>>> 6ed103f204953728b4b97b6363e44051b274582e

])
