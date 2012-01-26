
m4_include([config/saga_boost.m4])
m4_include([config/saga_ldl.m4])

AC_DEFUN([AX_SAGA_CHECK_BASE],
[
  if test "x$SAGA_CORE_CONFIGURE" == "x"; then
    AC_MSG_ERROR([AX-SAGA_CHECK_BASE should *only* be called from saga_core's main configure!])
  fi

  CONFDIR=`pwd`/config/
  export CONFDIR
  AC_SUBST(CONFDIR)

  AC_MSG_CHECKING([SAGA Version])
  # get version
  test -f PACKAGE && SAGA_VERSION=` grep -e '^VERSION:'  PACKAGE | sed -e 's/^VERSION: *//'`
  test -f PACKAGE && SAGA_REVISION=`grep -e '^REVISION:' PACKAGE | cut -f 2 -d '$' | cut -f 2 -d ' '`
  
  # svn info supercedes the revision info from the PACKAGE file
  svnrevision=`svn info 2>&1 | grep Revision | cut -f 2 -d ' '`

  if ! test "$svnrevision" = ""; then
    SAGA_REVISION=$svnrevision
  fi

  SAGA_VERSION_MAJOR=`echo $SAGA_VERSION    | cut -f 1 -d . | cut -f 1 -d '-'`
  SAGA_VERSION_MINOR=`echo $SAGA_VERSION    | cut -f 2 -d . | cut -f 1 -d '-'`
  SAGA_VERSION_SUBMINOR=`echo $SAGA_VERSION | cut -f 3 -d . | cut -f 1 -d '-'`

  if test "$SAGA_VERSION_SUBMINOR" == ""; then
    SAGA_VERSION_SUBMINOR=0
  fi

  SAGA_VERSION_FULL="0x"

  if test `expr $SAGA_VERSION_MAJOR : '.*'` == 1; then
    SAGA_VERSION_FULL="${SAGA_VERSION_FULL}0$SAGA_VERSION_MAJOR"
  else
    SAGA_VERSION_FULL="$SAGA_VERSION_FULL$SAGA_VERSION_MAJOR"
  fi

  if test `expr $SAGA_VERSION_MINOR : '.*'` == 1; then
    SAGA_VERSION_FULL="${SAGA_VERSION_FULL}0$SAGA_VERSION_MINOR"
  else
    SAGA_VERSION_FULL="$SAGA_VERSION_FULL$SAGA_VERSION_MINOR"
  fi

  if test `expr $SAGA_VERSION_SUBMINOR : '.*'` == 1; then
    SAGA_VERSION_FULL="${SAGA_VERSION_FULL}0$SAGA_VERSION_SUBMINOR"
  else
    SAGA_VERSION_FULL="$SAGA_VERSION_FULL$SAGA_VERSION_SUBMINOR"
  fi

  SAGA_VERSION_DATE=`date "+%Y%m%d"`

  AC_SUBST(SAGA_VERSION)
  AC_SUBST(SAGA_REVISION)
  AC_SUBST(SAGA_VERSION_MAJOR)
  AC_SUBST(SAGA_VERSION_MINOR)
  AC_SUBST(SAGA_VERSION_SUBMINOR)
  AC_SUBST(SAGA_VERSION_FULL)
  AC_SUBST(SAGA_VERSION_DATE)

  AC_DEFINE_UNQUOTED([SAGA_VERSION]         , ["$SAGA_VERSION"])
  AC_DEFINE_UNQUOTED([SAGA_REVISION]        , [$SAGA_REVISION])
  AC_DEFINE_UNQUOTED([SAGA_VERSION_MAJOR]   , [$SAGA_VERSION_MAJOR])
  AC_DEFINE_UNQUOTED([SAGA_VERSION_MINOR]   , [$SAGA_VERSION_MINOR])
  AC_DEFINE_UNQUOTED([SAGA_VERSION_SUBMINOR], [$SAGA_VERSION_SUBMINOR])
  AC_DEFINE_UNQUOTED([SAGA_VERSION_FULL]    , [$SAGA_VERSION_FULL])
  AC_DEFINE_UNQUOTED([SAGA_VERSION_DATE]    , [$SAGA_VERSION_DATE])

  AC_MSG_RESULT([$SAGA_VERSION (r$SAGA_REVISION)])


  AC_MSG_CHECKING([operating system type])
  SAGA_OSTYPE="SAGA_"`uname -s | tr 'a-z' 'A-Z'`

  if test "x$SAGA_OSTYPE" = "x"; then
    SAGA_OSTYPE=UNKNOWN
  fi

  if test "x$SAGA_OSTYPE" = "xSAGA_DARWIN"; then
    SAGA_OSTYPE=SAGA_APPLE
  fi

  SAGA_ARCH=`uname -m`
  SAGA_OSDIST=unknown

  if test "x$SAGA_OSTYPE" = "xSAGA_LINUX"; then
    if test -e /etc/lsb-release; then
      SAGA_OSDIST=`grep DISTRIB_ID /etc/lsb-release | cut -f 2 -d '='`
    elif test -e /etc/redhat-release; then
      SAGA_OSDIST=redhat
    fi
  fi


  AC_MSG_RESULT([$SAGA_OSTYPE - $SAGA_OSDIST])
  AC_SUBST(SAGA_OSTYPE)
  AC_SUBST(SAGA_OSDIST)
  AC_SUBST(SAGA_ARCH)
  AC_DEFINE_UNQUOTED([SAGA_OSTYPE],  [$SAGA_OSTYPE])
  AC_DEFINE_UNQUOTED([SAGA_OSDIST],  [$SAGA_OSDIST])
  AC_DEFINE_UNQUOTED([SAGA_ARCH],    [$SAGA_ARCH])
  AC_DEFINE_UNQUOTED([$SAGA_OSTYPE], [1])
  export SAGA_OSTYPE
  export SAGA_ARCH

  LANG=C
  export LANG

  AC_LANG(C++)
  AC_REQUIRE_CPP
  
  AC_PROG_CC
  AC_PROG_CXX
  AC_PROG_CXXCPP

  # check compiler version - works only for gcc/g++
  CPP_VERSION=`$CXXCPP -v 2>&1 | grep '^gcc version' | cut -f 3 -d ' '`
  CXX_VERSION=`$CXX -v 2>&1 | grep '^gcc version' | cut -f 3 -d ' '`
  CC_VERSION=`$CC   -v 2>&1 | grep '^gcc version' | cut -f 3 -d ' '`
  LD_VERSION="$CXX_VERSION"


  if test "x$CXX_VERSION" != "x"; then
 
    AC_MSG_CHECKING([gcc version >= 3.4])

    cxx_version_major=`echo $CXX_VERSION | cut -f 1 -d '.'`
    cxx_version_minor=`echo $CXX_VERSION | cut -f 2 -d '.'`

    cxx_version_ok=1

    if test "0$cxx_version_major" -lt "04"; then
      if test "0$cxx_version_major" -lt "03"; then
        $cxx_version_ok=0
      else
        # for v3.x
        if test "0$cxx_version_minor" -lt "04"; then
          $cxx_version_ok=0
        fi
      fi
    fi
        
    if test "$cxx_version_ok" -eq "0"; then 
      AC_MSG_FAILURE([Sorry, SAGA does not build with gcc prior to version 3.4 (found $cxx_version_major.$cxx_version_minor)])
    fi
      
    AC_MSG_RESULT([$CXX_VERSION - ok])

    # version is ok
    export CPP_VERSION
    export CXX_VERSION
    export CC_VERSION
    export LD_VERSION

    AC_SUBST(CPP_VERSION)
    AC_SUBST(CXX_VERSION)
    AC_SUBST(CC_VERSION)
    AC_SUBST(LD_VERSION)

    # check if the compiler supports the visibility flag
    echo "int main () {return 0;}" | $CXX -fvisibility=default -E -c - >& /dev/null
    if test "x$?" = "x0"; then
      SAGA_GCC_VISIBILITY="-fvisibility=default"
      AC_DEFINE([SAGA_GCC_HAVE_VISIBILITY], [1], [gcc understands visibility pragmas])
    else
      SAGA_GCC_VISIBILITY=""
    fi

    AC_SUBST(SAGA_GCC_VISIBILITY)

    # instead of trying to find out if -Wl,--no-as-needed is actually 
    # required, or, even worse, trying to get the linker command 
    # line args in order, we simply check if that flag is supported, 
    # and use it if it is.  That will usually only be the case for 
    # newer gcc versions.A
    ldflags_save=$LDFLAGS
    LDFLAGS="$LDFLAGS -Wl,--no-as-needed -lm"
    AC_MSG_CHECKING([gcc supports --no-as-needed])
    AC_LINK_IFELSE([
      #include <math.h>
      int main () 
      { 
        double y = cos (0.5); 
        return 0;
      } ],
      [SAGA_LINKS_AS_NEEDED=yes],
      [SAGA_LINKS_AS_NEEDED=no])

    AC_MSG_RESULT([$SAGA_LINKS_AS_NEEDED])
    if test "x$SAGA_LINKS_AS_NEEDED" = "xyes"; then
      if test "x$ldflags_save" = "x"; then
        LDFLAGS="-Wl,--no-as-needed"
      else
        LDFLAGS="$ldflags_save -Wl,--no-as-needed"
      fi
    else
      LDFLAGS="$ldflags_save"
    fi

    # on some systems, gcc produces 64bit objects by default, on other
    # systems it does 32bit by default.  We try to detect the default
    # behavior - which has consequences for the library search path
    # we have to use for linking (lib64 vs. lib etc.)
    AC_MSG_CHECKING([default bit mode (32/64)])
    echo "int main() {return 0;}" > /tmp/conftest.cpp
    $CXX $CXXFLAGS -c -o /tmp/conftest.o /tmp/conftest.cpp
    if test -e /tmp/conftest.o; then
      CC_BIT=`file /tmp/conftest.o | grep -o -e '64-bit' -e '32-bit' | cut -f 1 -d '-'`
    else
      CC_BIT=""
    fi
    rm -f /tmp/conftest.cpp /tmp/conftest.o

    AC_MSG_RESULT([$CC_BIT])
    AC_SUBST(CC_BIT)
    export CC_BIT

  fi


  ######################################################################
  # enable debug
  AC_ARG_ENABLE([debug],
                [AS_HELP_STRING([--enable-debug],
                [build debug version (default: enabled)])],
                [],
                [enable_debug=yes])

  if test "x$enable_debug" = "xyes"; then
    SAGA_CXXFLAGS_DEBUG="($CXXFLAGS)"
    SAGA_CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-O.//g'`
    SAGA_HAVE_DEBUG=yes
    AC_DEFINE([SAGA_HAVE_DEBUG], [yes])
  else
    SAGA_CXXFLAGS_DEBUG=
    SAGA_CXXFLAGS=`echo $CXXFLAGS | sed -e 's/-g//g'`
    SAGA_HAVE_DEBUG=no
  fi

  export SAGA_CXXFLAGS
  export SAGA_CXXFLAGS_DEBUG
  AC_SUBST(SAGA_HAVE_DEBUG)


  ######################################################################
  # enable deprecated calls
  AC_ARG_ENABLE([deprecated],
                [AS_HELP_STRING([--enable-deprecated],
                [allow deprecated calls (default: enabled)])],
                [],
                [enable_deprecated=yes])

  if test "x$enable_deprecated" = "xyes"; then
    SAGA_ALLOW_DEPRECATED=yes
    AC_DEFINE([SAGA_ALLOW_DEPRECATED], [yes],
              [allow deprcated calls])
  else
    SAGA_ALLOW_DEPRECATED=no
  fi

  AC_SUBST(SAGA_ALLOW_DEPRECATED)


  ######################################################################
  # lets see if we can find googles profiling tools
  AC_ARG_WITH([google_profiler],
              [AS_HELP_STRING([--with-google-profiler=DIR],
              [link against googles libprofiler@<:@default=no@:>@])],
              [with_google_profiler=$withval],
              [with_google_profiler=no])

  SAGA_HAVE_GOOGLE_PROFILER=no

  if test "x$with_google_profiler" = "xno" ; then

    AC_MSG_CHECKING([google profiler])
    AC_MSG_RESULT([no])

  else

    ldflags_save=$LDFLAGS

    LDFLAGS="-L$with_google_profiler/lib/ $LDFLAGS"

    AC_CHECK_LIB([profiler], 
                 [main],
                 [AC_SUBST([LIBADD_GOOGLE_PROFILER], ["-L$with_google_profiler/lib/ -lprofiler"])
                  SAGA_HAVE_GOOGLE_PROFILER=yes
                  AC_DEFINE_UNQUOTED([SAGA_HAVE_GOOGLE_PROFILER], 
                                     [$SAGA_HAVE_GOOGLE_PROFILER])
                 ],
                 [if test "x$with_google_profiler" != "x"; then
                  AC_MSG_RESULT(
                       [--with-google-profiler was given, but test for profiler failed ($with_google_profiler)])
                 fi]
                )

    SAGA_PROFILER_EVAL=$with_google_profiler/bin/pprof
    AC_SUBST(SAGA_PROFILER_EVAL)

    LDFLAGS=$ldflags_save

  fi


  ######################################################################
  # lets see if we can find ccache,  for speeding up compiles
  AC_ARG_WITH([ccache],
              [AS_HELP_STRING([--with-ccache=DIR],
              [speed up repeated compiles with ccache@<:@default=no@:>@])],
              [],
              [with_ccache=$withval])

  SAGA_CCACHE=
  SAGA_HAVE_CCACHE=no

  if test "x$with_ccache" = "xno" ; then

    AC_MSG_CHECKING([ccache])
    AC_MSG_RESULT([no])

  else

    AC_PATH_PROG(SAGA_CCACHE, ccache)

    if ! test "x$SAGA_CCACHE" = "x"; then
      SAGA_HAVE_CCACHE=yes
    fi

  fi


  ######################################################################
  # lets see if we can find readline, for the saga-shell
  AC_ARG_WITH([readline],
              [AS_HELP_STRING([--with-readline=DIR],
              [support fancy command line editing @<:@default=check@:>@])],
              [],
              [with_readline=check])

  LIBADD_READLINE=

  if test "x$with_readline" != "xno" ; then
    if test "x$with_readline" != "xyes" ; then
      readline_dir=$with_readline
    fi

    cppflags_save=$CPPFLAGS
    ldflags_save=$LDFLAGS

    CPPFLAGS="-I$readline_dir/include/ $CPPFLAGS"
    LDFLAGS="-L$readline_dir/lib/ $LDFLAGS"

    AC_CHECK_HEADER([readline/readline.h], 
                    [HAVE_READLINE_HEADER=0]
                    [HAVE_READLINE_HEADER=1])

    if test "x$HAVE_READLINE_HEADER" = "x1"; then
    
      AC_CHECK_LIB([readline], [readline],
                   [AC_SUBST([LIBADD_READLINE], ["-lreadline -lncurses"])
                   SAGA_HAVE_READLINE=1
                   AC_DEFINE_UNQUOTED([SAGA_HAVE_READLINE], 
                                     [$SAGA_HAVE_READLINE])
                   ],
                   [if test "x$with_readline" != xcheck; then
                   AC_MSG_RESULT(
                       [--with-readline was given, but test for readline failed])
                   fi], 
                   [-lncurses])
    fi
      
    CPPFLAGS=$cppflags_save
    LDFLAGS=$ldflags_save
      
  fi

  if test "x$SAGA_HAVE_READLINE" = "x1" ; then
    AC_CHECK_LIB([readline], [history_list],
                 SAGA_HAVE_HISTORY_LIST=1
                 [ AC_DEFINE_UNQUOTED([SAGA_HAVE_HISTORY_LIST],
                                     [$SAGA_HAVE_HISTORY_LIST])])
  fi

  ######################################################################
  # check if static and/or dynamic libs should be built

  AC_ARG_WITH([libs],
              [AS_HELP_STRING([--with-libs=LIST],
              [define which lib types are build: static, dynamic, all (default=all)])],
              [],
              [with_libs=all])

  SAGA_BUILD_LIB_STATIC=no
  SAGA_BUILD_LIB_DYNAMIC=no

  if test "$with_libs" = "yes" ; then
    with_libs=all
  fi
  if test "$with_libs" = "dynamic,static" ; then
    with_libs=all
  fi
  if test "$with_libs" = "static,dynamic" ; then
    with_libs=all
  fi

  if test "x$with_libs" = "xall" ; then
    SAGA_BUILD_LIB_STATIC=yes
    SAGA_BUILD_LIB_DYNAMIC=yes
  fi

  if test "$with_libs" = "static" ; then
    SAGA_BUILD_LIB_STATIC=yes
  fi

  if test "$with_libs" = "dynamic" ; then
    SAGA_BUILD_LIB_DYNAMIC=yes
  fi

  if test "$SAGA_BUILD_LIB_STATIC-$SAGA_BUILD_LIB_DYNAMIC" = "no-no" ; then
    AC_MSG_ERROR([cannot sensibly build SAGA with no libs])
  fi


  AC_MSG_NOTICE([build static libs:  $SAGA_BUILD_LIB_STATIC])
  AC_MSG_NOTICE([build dynamic libs: $SAGA_BUILD_LIB_DYNAMIC])

  AC_SUBST(SAGA_BUILD_LIB_STATIC)
  AC_SUBST(SAGA_BUILD_LIB_DYNAMIC)


  ######################################################################

  SAGA_CPP=$CXXCPP
  SAGA_CXX=$CXX
  SAGA_CC=$CC
  SAGA_LD=$CXX
  
  SAGA_CPPFLAGS="-E"
  SAGA_CPPINCS=
  SAGA_CPPINCS_SYS="$CPPFLAGS"
  SAGA_CCFLAGS=$CFLAGS
  SAGA_LDFLAGS_SYS="$LDFLAGS"
  SAGA_LDFLAGS_READLINE="$LIBADD_READLINE"
  SAGA_LDFLAGS_GOOGLE_PROFILER="$LIBADD_GOOGLE_PROFILER"
  SAGA_ARFLAGS="rcs"
  
  if test "_$SAGA_CPP" = "_" ; then
    SAGA_CPP=$CXX
  fi
  
  if test "_$SAGA_LD" = "_" ; then
    SAGA_LD=$CXX
  fi
  
  AC_SUBST(SAGA_CXX)
  AC_SUBST(SAGA_CPP)
  AC_SUBST(SAGA_CC)
  AC_SUBST(SAGA_LD)
  AC_SUBST(SAGA_CCACHE)
  
  AC_SUBST(SAGA_CPPINCS)
  AC_SUBST(SAGA_CPPINCS_SYS)
  AC_SUBST(SAGA_CPPFLAGS)
  AC_SUBST(SAGA_CXXFLAGS)
  AC_SUBST(SAGA_CCFLAGS)
  AC_SUBST(SAGA_LDFLAGS_GOOGLE_PROFILER)
  AC_SUBST(SAGA_LDFLAGS_READLINE)
  AC_SUBST(SAGA_LDFLAGS_SYS)
  AC_SUBST(SAGA_ARFLAGS)
  

  AC_PATH_PROG(SAGA_AR,        ar)
  AC_PATH_PROG(SAGA_BC,        bc)
  AC_PATH_PROG(SAGA_CP,        cp)
  AC_PATH_PROG(SAGA_LN,        ln)
  AC_PATH_PROG(SAGA_LS,        ls)
  AC_PATH_PROG(SAGA_MV,        mv)
  AC_PATH_PROG(SAGA_RM,        rm)
  AC_PATH_PROG(SAGA_SH,        sh)
  AC_PATH_PROG(SAGA_TR,        tr)
  AC_PATH_PROG(SAGA_AWK,       awk)
  AC_PATH_PROG(SAGA_CAT,       cat)
  AC_PATH_PROG(SAGA_CUT,       cut)
  AC_PATH_PROG(SAGA_ENV,       env)
  AC_PATH_PROG(SAGA_PWD,       pwd)
  AC_PATH_PROG(SAGA_SED,       sed)
  AC_PATH_PROG(SAGA_SCP,       scp)
  AC_PATH_PROG(SAGA_SSH,       ssh)
  AC_PATH_PROG(SAGA_TAR,       tar)
  AC_PATH_PROG(SAGA_DATE,      date)
  AC_PATH_PROG(SAGA_EXPR,      expr)
  AC_PATH_PROG(SAGA_GREP,      grep)
  AC_PATH_PROG(SAGA_GZIP,      gzip)
  AC_PATH_PROG(SAGA_ECHO,      echo)
  AC_PATH_PROG(SAGA_FIND,      find)
  AC_PATH_PROG(SAGA_GZIP,      gzip)
  AC_PATH_PROG(SAGA_SORT,      sort)
  AC_PATH_PROG(SAGA_TEST,      test)
  AC_PATH_PROG(SAGA_TRUE,      true)
  AC_PATH_PROG(SAGA_UNIQ,      uniq)
  AC_PATH_PROG(SAGA_MKDIR,     mkdir)
  AC_PATH_PROG(SAGA_RMDIR,     rmdir)
  AC_PATH_PROG(SAGA_TOUCH,     touch)
  AC_PATH_PROG(SAGA_FALSE,     false)
  AC_PATH_PROG(SAGA_SLEEP,     sleep)
  AC_PATH_PROG(SAGA_XARGS,     xargs)
  AC_PATH_PROG(SAGA_PDFLATEX,  pdflatex)
  AC_PATH_PROG(SAGA_BIBTEX,    bibtex)
  AC_PATH_PROG(SAGA_HTLATEX,   htlatex)
  AC_PATH_PROG(SAGA_DOXYGEN,   doxygen)
  AC_PATH_PROG(SAGA_EPYDOC,    epydoc)

  AC_DEFINE_UNQUOTED([SAGA_TEST_BIN_BC],    ["$SAGA_BC"])
  AC_DEFINE_UNQUOTED([SAGA_TEST_BIN_TRUE],  ["$SAGA_TRUE"])
  AC_DEFINE_UNQUOTED([SAGA_TEST_BIN_FALSE], ["$SAGA_FALSE"])
  AC_DEFINE_UNQUOTED([SAGA_TEST_BIN_SLEEP], ["$SAGA_SLEEP"])

  # check if pwd understands -P
  AC_MSG_CHECKING(for -P flag on pwd)
  test=`cd / && $SAGA_PWD -P 2>&1`
  if test "$test" = "/"; then
    SAGA_PWD="$SAGA_PWD -P"
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
  fi



  AC_MSG_CHECKING(for /dev/null)
  if test -w /dev/null; then
    SAGA_DEVNULL=/dev/null
  else
    SAGA_DEVNULL=/tmp/remove_me.saga
  fi

  AC_SUBST(SAGA_DEVNULL)
  AC_MSG_RESULT([$SAGA_DEVNULL])


  ###########################################################
  # 
  # lets see who finds this...
  # inspired by : configure of kvirc 2.0
  # 
  AC_MSG_CHECKING(number of dimensions)
  AC_MSG_RESULT([3.5*pi (spatial) + 1 (temporal)])
  AC_MSG_CHECKING(type of universe)
  AC_MSG_RESULT([4 bosons, 3 fermions, 1 higgs - ok, standard universe])
  AC_MSG_CHECKING(if e=mc^2)
  AC_MSG_RESULT([ok])
  AC_MSG_CHECKING(validity of Maxwell laws on this machine)
  AC_MSG_RESULT([ok])
  AC_MSG_CHECKING(fundamental symmetry group)
  AC_MSG_RESULT([E8 (nice)])
  #
  ###########################################################
])


AC_DEFUN([AX_SAGA_CHECK_TYPES],
[
  old_cppflags=$CPPFLAGS
  CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"


  AC_MSG_CHECKING([for long type])
  SAGA_TYPE_SIZE=unique
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <long, int>::value); } ],
    [SAGA_TYPE_LONG=int
     AC_DEFINE([SAGA_TYPE_LONG_IS_INT], [1])])
  AC_MSG_RESULT([$SAGA_TYPE_SIZE])
 
 
  AC_MSG_CHECKING([for long long type])
  SAGA_TYPE_SIZE=unique
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <long long, int>::value); } ],
    [SAGA_TYPE_LONGLONG=int
     AC_DEFINE([SAGA_TYPE_LONGLONG_IS_INT], [1])])
  AC_MSG_RESULT([$SAGA_TYPE_SIZE])
 
 
  AC_MSG_CHECKING([for size_t type])
  SAGA_TYPE_SIZE=unique
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <size_t, int>::value); } ],
    [SAGA_TYPE_SIZE=int
     AC_DEFINE([SAGA_TYPE_SIZE_IS_INT], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <size_t, long>::value); } ],
    [SAGA_TYPE_SIZE=long
     AC_DEFINE([SAGA_TYPE_SIZE_IS_LONG], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <size_t, long long>::value); } ],
    [SAGA_TYPE_SIZE="long long"
     AC_DEFINE([SAGA_TYPE_SIZE_IS_LONGLONG], [1])])
  AC_MSG_RESULT([$SAGA_TYPE_SIZE])
 
 
  AC_MSG_CHECKING([for ssize_t type])
  SAGA_TYPE_SSIZE=unique
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <ssize_t, int>::value); } ],
    [SAGA_TYPE_SSIZE=int
     AC_DEFINE([SAGA_TYPE_SSIZE_IS_INT], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <ssize_t, long>::value); } ],
    [SAGA_TYPE_SSIZE=long
     AC_DEFINE([SAGA_TYPE_SSIZE_IS_LONG], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <ssize_t, long long>::value); } ],
    [SAGA_TYPE_SSIZE="long long"
     AC_DEFINE([SAGA_TYPE_SSIZE_IS_LONGLONG], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <ssize_t, size_t>::value); } ],
    [SAGA_TYPE_SSIZE=size_t
     AC_DEFINE([SAGA_TYPE_SSIZE_IS_SIZE], [1])])
  AC_MSG_RESULT([$SAGA_TYPE_SSIZE])
 
 
  AC_MSG_CHECKING([for off_t type])
  SAGA_TYPE_OFF=unique
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <off_t, int>::value); } ],
    [SAGA_TYPE_OFF=int
     AC_DEFINE([SAGA_TYPE_OFF_IS_INT], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <off_t, long>::value); } ],
    [SAGA_TYPE_OFF=long
     AC_DEFINE([SAGA_TYPE_OFF_IS_LONG], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <off_t, long long>::value); } ],
    [SAGA_TYPE_OFF="long long"
     AC_DEFINE([SAGA_TYPE_OFF_IS_LONGLONG], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <off_t, size_t>::value); } ],
    [SAGA_TYPE_OFF=size_t
     AC_DEFINE([SAGA_TYPE_OFF_IS_SIZE], [1])])
  AC_RUN_IFELSE([
    #include <boost/type_traits/is_same.hpp>
    int main () { return ! (boost::is_same <off_t, ssize_t>::value); } ],
    [SAGA_TYPE_OFF=ssize_t
     AC_DEFINE([SAGA_TYPE_OFF_IS_SSIZE], [1])])
  AC_MSG_RESULT([$SAGA_TYPE_OFF])


  CPPFLAGS=$old_cppflags
])


AC_DEFUN([AX_SAGA_CHECK_PACKAGES],
[
  AC_MSG_CHECKING([available packages])

  SAGA_HAVE_PACKAGE_JOB=no
  SAGA_HAVE_PACKAGE_FILESYSTEM=no
  SAGA_HAVE_PACKAGE_REPLICA=no
  SAGA_HAVE_PACKAGE_NAMESPACE=no
  SAGA_HAVE_PACKAGE_STREAM=no
  SAGA_HAVE_PACKAGE_RPC=no
  SAGA_HAVE_PACKAGE_ADVERT=no
  SAGA_HAVE_PACKAGE_SD=no
  SAGA_HAVE_PACKAGE_CPR=no
  SAGA_HAVE_PACKAGE_MESSAGES=no
  SAGA_HAVE_PACKAGE_ISN=no

  AC_ARG_WITH([packages],
              AS_HELP_STRING([--with-packages=LIST],
                             [comma separated list of SAGA API packages to build.
                              possible values: job, namespace, filesystem, replica, 
                              advert, sd, isn, all 
                              (default is all).]),
              [
                if test "$withval" = "no"; then
                  want_packages=""
                elif test "$withval" = "yes"; then
                  want_packages="all"
                else
                  want_packages=$withval
                fi
              ],
              [
                want_packages="all"
              ])

  if test "x$want_packages" = "x"; then
    AC_MSG_ERROR("It makes no sense to build SAGA withpout any API packages.")
  fi

  if test "x$want_packages" = "xall"; then
    want_packages="job,namespace,filesystem,replica,advert,sd,isn"
  fi

  while $TRUE
  do
    this_package=`echo  $want_packages |            cut -f1  -d','`
    want_packages=`echo $want_packages | grep ',' | cut -f2- -d','`

    if test "x$this_package" == "x"; then
      break
    fi

    build_this_package="$this_package "

    case "$this_package" in

      job )
          SAGA_HAVE_PACKAGE_JOB=yes
        ;;

      filesystem )
          SAGA_HAVE_PACKAGE_FILESYSTEM=yes
          SAGA_HAVE_PACKAGE_NAMESPACE=yes
        ;;

      replica )
          SAGA_HAVE_PACKAGE_REPLICA=yes
          SAGA_HAVE_PACKAGE_NAMESPACE=yes
        ;;

      namespace )
          SAGA_HAVE_PACKAGE_NAMESPACE=yes
        ;;

      advert )
          SAGA_HAVE_PACKAGE_ADVERT=yes
          SAGA_HAVE_PACKAGE_NAMESPACE=yes
        ;;

      sd )
          SAGA_HAVE_PACKAGE_SD=yes
        ;;

      isn )
          SAGA_HAVE_PACKAGE_ISN=yes
        ;;

      * )
          build_this_package=""
          AC_MSG_ERROR(unknown package type '$this_package')
        ;;

    esac

    build_packages="$build_packages$build_this_package"

  done


  if test "$SAGA_HAVE_PACKAGE_JOB" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_JOB])
    AC_DEFINE([SAGA_HAVE_PACKAGE_JOB], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_job"
  fi

  if test "$SAGA_HAVE_PACKAGE_FILESYSTEM" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_FILESYSTEM])
    AC_DEFINE([SAGA_HAVE_PACKAGE_FILESYSTEM], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_filesystem"
  fi

  if test "$SAGA_HAVE_PACKAGE_REPLICA" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_REPLICA])
    AC_DEFINE([SAGA_HAVE_PACKAGE_REPLICA], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_replica"
  fi

  if test "$SAGA_HAVE_PACKAGE_NAMESPACE" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_NAMESPACE])
    AC_DEFINE([SAGA_HAVE_PACKAGE_NAMESPACE], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_namespace"
  fi

  if test "$SAGA_HAVE_PACKAGE_STREAM" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_STREAM])
    AC_DEFINE([SAGA_HAVE_PACKAGE_STREAM], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_stream"
  fi

  if test "$SAGA_HAVE_PACKAGE_RPC" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_RPC])
    AC_DEFINE([SAGA_HAVE_PACKAGE_RPC], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_rpc"
  fi

  if test "$SAGA_HAVE_PACKAGE_ADVERT" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_ADVERT])
    AC_DEFINE([SAGA_HAVE_PACKAGE_ADVERT], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_advert"
  fi

  if test "$SAGA_HAVE_PACKAGE_SD" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_SD])
    AC_DEFINE([SAGA_HAVE_PACKAGE_SD], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_sd"
  fi

  if test "$SAGA_HAVE_PACKAGE_CPR" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_CPR])
    AC_DEFINE([SAGA_HAVE_PACKAGE_CPR], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_cpr"
  fi

  if test "$SAGA_HAVE_PACKAGE_MESSAGES" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_MESSAGES])
    AC_DEFINE([SAGA_HAVE_PACKAGE_MESSAGES], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_messages"
  fi

  if test "$SAGA_HAVE_PACKAGE_ISN" = "yes"; then
    AC_SUBST([SAGA_HAVE_PACKAGE_ISN])
    AC_DEFINE([SAGA_HAVE_PACKAGE_ISN], [1])
    SAGA_PACKAGE_LDFLAGS="$SAGA_PACKAGE_LDFLAGS -lsaga_package_isn"
  fi

  export SAGA_PACKAGE_LDFLAGS
  AC_SUBST(SAGA_PACKAGE_LDFLAGS)

  SAGA_BUILD_PACKAGES=$build_packages
  export   SAGA_BUILD_PACKAGES
  AC_SUBST(SAGA_BUILD_PACKAGES)

  export SAGA_HAVE_PACKAGE_JOB
  export SAGA_HAVE_PACKAGE_FILESYSTEM
  export SAGA_HAVE_PACKAGE_REPLICA
  export SAGA_HAVE_PACKAGE_NAMESPACE
  export SAGA_HAVE_PACKAGE_STREAM
  export SAGA_HAVE_PACKAGE_RPC
  export SAGA_HAVE_PACKAGE_ADVERT
  export SAGA_HAVE_PACKAGE_SD
  export SAGA_HAVE_PACKAGE_CPR
  export SAGA_HAVE_PACKAGE_MESSAGES
  export SAGA_HAVE_PACKAGE_ISN

  # also define the 'SAGA_HAVE_ADAPTOR_XYZ' versions, for backward compatibility
  export SAGA_HAVE_ADAPTOR_JOB=$SAGA_HAVE_PACKAGE_JOB        
  export SAGA_HAVE_ADAPTOR_FILESYSTEM=$SAGA_HAVE_PACKAGE_FILESYSTEM 
  export SAGA_HAVE_ADAPTOR_REPLICA=$SAGA_HAVE_PACKAGE_REPLICA    
  export SAGA_HAVE_ADAPTOR_NAMESPACE=$SAGA_HAVE_PACKAGE_NAMESPACE  
  export SAGA_HAVE_ADAPTOR_STREAM=$SAGA_HAVE_PACKAGE_STREAM     
  export SAGA_HAVE_ADAPTOR_RPC=$SAGA_HAVE_PACKAGE_RPC        
  export SAGA_HAVE_ADAPTOR_ADVERT=$SAGA_HAVE_PACKAGE_ADVERT     
  export SAGA_HAVE_ADAPTOR_SD=$SAGA_HAVE_PACKAGE_SD         
  export SAGA_HAVE_ADAPTOR_CPR=$SAGA_HAVE_PACKAGE_CPR        
  export SAGA_HAVE_ADAPTOR_MESSAGES=$SAGA_HAVE_PACKAGE_MESSAGES   
  export SAGA_HAVE_ADAPTOR_ISN=$SAGA_HAVE_PACKAGE_ISN        

  AC_SUBST([SAGA_HAVE_ADAPTOR_JOB])
  AC_SUBST([SAGA_HAVE_ADAPTOR_FILESYSTEM])
  AC_SUBST([SAGA_HAVE_ADAPTOR_REPLICA])
  AC_SUBST([SAGA_HAVE_ADAPTOR_NAMESPACE])
  AC_SUBST([SAGA_HAVE_ADAPTOR_STREAM])
  AC_SUBST([SAGA_HAVE_ADAPTOR_RPC])
  AC_SUBST([SAGA_HAVE_ADAPTOR_ADVERT])
  AC_SUBST([SAGA_HAVE_ADAPTOR_SD])
  AC_SUBST([SAGA_HAVE_ADAPTOR_CPR])
  AC_SUBST([SAGA_HAVE_ADAPTOR_MESSAGES])
  AC_SUBST([SAGA_HAVE_ADAPTOR_ISN])

  AC_MSG_RESULT([$build_packages])


  # we also store the engine config for later reuse, e.g. by adaptor
  # configure's
  CACHE=config/saga-config.cache

  echo ""                                                                  >  $CACHE
  echo "export SAGA_OSTYPE=$SAGA_OSTYPE"                                   >> $CACHE
  echo "export SAGA_OSDIST=$SAGA_OSDIST"                                   >> $CACHE
  echo "export SAGA_ARCH=$SAGA_ARCH"                                       >> $CACHE
  echo ""                                                                  >> $CACHE
  echo "export LDFLAGS=\"$LDFLAGS\""                                       >> $CACHE
  echo ""                                                                  >> $CACHE
  echo "export BOOST_LOCATION=$BOOST_LOCATION"                             >> $CACHE
  echo "export OPENSSL_LOCATION=$OPENSSL_LOCATION"                         >> $CACHE
  echo "export SQLITE3_LOCATION=$SQLITE3_LOCATION"                         >> $CACHE
  echo "export POSTGRESQL_LOCATION=$POSTGRESQL_LOCATION"                   >> $CACHE
  echo "export XMLRPC_LOCATION=$XMLRPC_LOCATION"                           >> $CACHE
  echo ""                                                                  >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_JOB=$SAGA_HAVE_PACKAGE_JOB"               >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_FILESYSTEM=$SAGA_HAVE_PACKAGE_FILESYSTEM" >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_REPLICA=$SAGA_HAVE_PACKAGE_REPLICA"       >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_NAMESPACE=$SAGA_HAVE_PACKAGE_NAMESPACE"   >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_STREAM=$SAGA_HAVE_PACKAGE_STREAM"         >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_RPC=$SAGA_HAVE_PACKAGE_RPC"               >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_ADVERT=$SAGA_HAVE_PACKAGE_ADVERT"         >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_SD=$SAGA_HAVE_PACKAGE_SD"                 >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_CPR=$SAGA_HAVE_PACKAGE_CPR"               >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_MESSAGES=$SAGA_HAVE_PACKAGE_MESSAGES"     >> $CACHE
  echo "export SAGA_HAVE_PACKAGE_ISN=$SAGA_HAVE_PACKAGE_ISN"               >> $CACHE
  echo ""                                                                  >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_JOB=$SAGA_HAVE_PACKAGE_JOB"               >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_FILESYSTEM=$SAGA_HAVE_PACKAGE_FILESYSTEM" >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_REPLICA=$SAGA_HAVE_PACKAGE_REPLICA"       >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_NAMESPACE=$SAGA_HAVE_PACKAGE_NAMESPACE"   >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_STREAM=$SAGA_HAVE_PACKAGE_STREAM"         >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_RPC=$SAGA_HAVE_PACKAGE_RPC"               >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_ADVERT=$SAGA_HAVE_PACKAGE_ADVERT"         >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_SD=$SAGA_HAVE_PACKAGE_SD"                 >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_CPR=$SAGA_HAVE_PACKAGE_CPR"               >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_MESSAGES=$SAGA_HAVE_PACKAGE_MESSAGES"     >> $CACHE
  echo "export SAGA_HAVE_ADAPTOR_ISN=$SAGA_HAVE_PACKAGE_ISN"               >> $CACHE
  echo ""                                                                  >> $CACHE
])

