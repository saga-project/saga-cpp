##### http://autoconf-archive.cryp.to/ax_boost.html
#
# SYNOPSIS
#
#   AX_SAGA_CHECK_BOOST([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the Boost C++ libraries of a particular version (or newer)
#   we forward the tests to the official boost.m4 macros.

m4_include([boost.m4])

AC_DEFUN([AX_SAGA_CHECK_BOOST_VERSION],
[
  HAVE_BOOST=no

  # first find boost
  BOOST_REQUIRE($1)

  # then find boost version
  if test "x$BOOST_VERSION_HPP" != "x"; then
    BOOST_VERSION_NATIVE=`grep '^#define BOOST_VERSION' $BOOST_VERSION_HPP | cut -f 3 -d ' '`
    BOOST_VERSION_MAJ=`echo $BOOST_VERSION_NATIVE | cut -c 1  `
    BOOST_VERSION_MIN=`echo $BOOST_VERSION_NATIVE | cut -c 2-4`
    BOOST_VERSION_SUB=`echo $BOOST_VERSION_NATIVE | cut -c 5-6`

    # get rid of strange newlines in the native string
    BOOST_VERSION_NATIVE="$BOOST_VERSION_MAJ$BOOST_VERSION_MIN$BOOST_VERSION_SUB"

    BOOST_VERSION_MAJ=`expr $BOOST_VERSION_MAJ : "0*\(..*\)"`
    BOOST_VERSION_MIN=`expr $BOOST_VERSION_MIN : "0*\(..*\)"`
    BOOST_VERSION_SUB=`expr $BOOST_VERSION_SUB : "0*\(..*\)"`

    export BOOST_VERSION_NATIVE
    export BOOST_VERSION_MAJ
    export BOOST_VERSION_MIN
    export BOOST_VERSION_SUB

    BOOST_VERSION="$BOOST_VERSION_MAJ.$BOOST_VERSION_MIN.$BOOST_VERSION_SUB"

    HAVE_BOOST=yes

    # AC_MSG_NOTICE(BOOST_VERSION_MAJ: $BOOST_VERSION_MAJ)
    # AC_MSG_NOTICE(BOOST_VERSION_MIN: $BOOST_VERSION_MIN)
    # AC_MSG_NOTICE(BOOST_VERSION_SUB: $BOOST_VERSION_SUB)
  fi

  AC_SUBST(BOOST_VERSION)
  AC_SUBST(BOOST_VERSION_NATIVE)
  AC_SUBST(BOOST_VERSION_MAJ)
  AC_SUBST(BOOST_VERSION_MIN)
  AC_SUBST(BOOST_VERSION_SUB)
])


AC_DEFUN([AX_SAGA_CHECK_BOOST],
[
  AC_ARG_VAR([BOOST_LOCATION],[BOOST installation directory])

  # boost checks require libdl, for whatever reason.
  AX_SAGA_CHECK_LIBDL()
  
  AX_SAGA_CHECK_BOOST_VERSION($1)

  BOOST_HEADERS="$BOOST_HEADER_VERSION"

  if test "x$LD_LIBRARY_PATH" = "x"; then
    LD_LIBRARY_PATH=$BOOST_LOCATION/lib
  else
    LD_LIBRARY_PATH=$BOOST_LOCATION/lib:$LD_LIBRARY_PATH
  fi

  export LD_LIBRARY_PATH

  BOOST_HAVE_ASIO=no
  BOOST_HAVE_UUID=no
  BOOST_HAVE_LOG=no
  BOOST_HAVE_SYSTEM=no
  BOOST_HAVE_FILESYSTEM=no

  if test "$BOOST_VERSION_MAJ" -ge "1"; then
    if test "$BOOST_VERSION_MAJ" -eq "1"; then
      if test "$BOOST_VERSION_MIN" -ge "35"; then
        BOOST_HAVE_ASIO=yes
        BOOST_HAVE_SYSTEM=yes
      fi
    else
      BOOST_HAVE_ASIO=yes
      BOOST_HAVE_SYSTEM=yes
    fi
  fi


  # Configuration for Boost.Asio
  if test "x$BOOST_HAVE_ASIO" == "xyes"; then
    BOOST_ASIO
    
    if test "x$BOOST_HAVE_LIB_ASIO" == "xyes"; then
      SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_ASIO"
    fi
  fi


  if test "$BOOST_VERSION_MAJ" -ge "1"; then
    if test "$BOOST_VERSION_MAJ" -eq "1"; then
      if test "$BOOST_VERSION_MIN" -ge "42"; then
        BOOST_HAVE_UUID=yes
      fi
    else
      BOOST_HAVE_UUID=yes
    fi
  fi


  # Configuration for Boost.Asio
  if test "x$BOOST_HAVE_UUID" == "xyes"; then
    BOOST_UUID
  fi

  if test "x$BOOST_HAVE_LIB_UUID" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_UUID"
  fi


  # if boost version is > 1.35, find libboost_system
  if test "x$BOOST_HAVE_SYSTEM" = "xyes"; then
    BOOST_SYSTEM
    if test "x$BOOST_HAVE_LIB_SYSTEM" = "xyes"; then
      BOOST_LIBS="$BOOST_LIBS $BOOST_SYSTEM_LIBS"
      BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_SYSTEM_S_LIBS"
    else
      AC_MSG_ERROR([could not find boost system])
    fi
  fi

  # boost > 1.35 requires to link against system when linking against
  # filesystem.  Linux manages quite well on itself, but MacOS... *sigh*
  # anyway, if we don't find it, we have our own copy in external which 
  # does not need boost_system

  if test "x$BOOST_HAVE_SYSTEM" = "xyes"; then
    SAGA_SAVE_LDFLAGS=$LDFLAGS
    LDFLAGS="$LDFLAGS $BOOST_SYSTEM_LDFLAGS $BOOST_SYSTEM_LIBS"
    export LDFLAGS
    export SAGA_SAVE_LDFLAGS
  fi

  if test "x$BOOST_HAVE_LIB_SYSTEM" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_SYSTEM"
  fi



  # test for various libraries, and crate CPPFLAGS and LDFLAGS
  BOOST_THREADS
  if test "x$BOOST_HAVE_LIB_THREAD" = "xyes"; then
    BOOST_LDFLAGS="$BOOST_THREAD_LDFLAGS"
    BOOST_LIBS="$BOOST_LIBS $BOOST_THREAD_LIBS"
    BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_THREAD_S_LIBS"
  else
    AC_MSG_ERROR([could not find boost thread])
  fi

  if test "x$BOOST_HAVE_LIB_THREADS" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_THREADS"
  fi




  BOOST_FILESYSTEM
  if test "x$BOOST_HAVE_LIB_FILESYSTEM" = "xyes"; then
    BOOST_HAVE_FILESYSTEM=yes
    BOOST_LIBS="$BOOST_LIBS $BOOST_FILESYSTEM_LIBS"
    BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_FILESYSTEM_S_LIBS"
    BOOST_FILESYSTEM_VERSION=$BOOST_VERSION_NATIVE
  else
    BOOST_HAVE_FILESYSTEM=no
    AC_MSG_WARN([could not find boost filesystem])
  fi
  LDFLAGS=$SAGA_SAVE_LDFLAGS

  # for < 1.35, we ignore the system boost file system, and use our own.
  if test "$BOOST_VERSION_MAJ" -eq "1"; then
    if test "$BOOST_VERSION_MIN" -lt "35"; then
      BOOST_HAVE_FILESYSTEM=no
      AC_MSG_WARN([using internal boost filesystem for boost < 1.35])
      # set version to 1.34.1
      BOOST_FILESYSTEM_VERSION=103500
    fi
  fi

  if test "x$BOOST_HAVE_LIB_FILESYSTEM" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_FILESYSTEM"
  fi



  BOOST_REGEX
  if test "x$BOOST_HAVE_LIB_REGEX" = "xyes"; then
    BOOST_LIBS="$BOOST_LIBS $BOOST_REGEX_LIBS"
    BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_REGEX_S_LIBS"
  else
    AC_MSG_ERROR([could not find boost regex])
  fi

  if test "x$BOOST_HAVE_LIB_REGEX" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_REGEX"
  fi



  BOOST_SERIALIZATION
  if test "x$BOOST_HAVE_LIB_SERIALIZATION" = "xyes"; then
    BOOST_LIBS="$BOOST_LIBS $BOOST_SERIALIZATION_LIBS"
    BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_SERIALIZATION_S_LIBS"
  else
    AC_MSG_ERROR([could not find boost serialization])
  fi

  if test "x$BOOST_HAVE_LIB_SERIALIZATION" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_SERIALIZATION"
  fi



  BOOST_IOSTREAMS
  if test "x$BOOST_HAVE_LIB_IOSTREAMS" = "xyes"; then
    BOOST_LIBS="$BOOST_LIBS $BOOST_IOSTREAMS_LIBS"
    BOOST_S_LIBS="$BOOST_S_LIBS $BOOST_IOSTREAMS_S_LIBS"
    BOOST_HAVE_IOSTREAMS=yes
    BOOST_IOSTREAMS_VERSION=$BOOST_VERSION_NATIVE
  else
    AC_MSG_WARN([$BOOST_HAVE_IOSTREAMS: could not find boost iostreams - using internal version])
    BOOST_HAVE_IOSTREAMS=no
    BOOST_IOSTREAMS_VERSION=104000
  fi

  if test "x$BOOST_HAVE_LIB_IOSTREAMS" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_IOSTREAMS"
  fi



  BOOST_TEST
  if test "x$BOOST_HAVE_LIB_UNIT_TEST_FRAMEWORK" = "xyes"; then
    BOOST_HAVE_TEST=yes
  else
    AC_MSG_WARN([could not find boost unit test framework])
  fi

  if test "x$BOOST_HAVE_LIB_TEST" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_TEST"
  fi



  BOOST_PROGRAM_OPTIONS
  if test "x$BOOST_HAVE_LIB_PROGRAM_OPTIONS" = "xyes"; then
    BOOST_HAVE_PROGRAM_OPTIONS=yes
  else
    AC_MSG_WARN([could not find boost program options])
  fi


  if test "x$BOOST_HAVE_LIB_PROGRAM_OPTIONS" != "xyes"; then
    # we need program options for the tests (test helper)
    BOOST_HAVE_TEST=no
    AC_MSG_WARN([cannot use boost unit test framework w/o boost program options])
  fi

  if test "x$BOOST_HAVE_LIB_PROGRAM_OPTIONS" == "xyes"; then
    SAGA_BOOST_DEP_FILES="$SAGA_BOOST_DEP_FILES $BOOST_HEADER_PROGRAM_OPTIONS"
  fi



  AC_MSG_CHECKING(for boost location)
  AC_MSG_RESULT([$BOOST_LOCATION])

  AC_MSG_CHECKING(for boost major version)
  AC_MSG_RESULT([$BOOST_VERSION_MAJ])

  AC_MSG_CHECKING(for boost minor version)
  AC_MSG_RESULT([$BOOST_VERSION_MIN])

  AC_MSG_CHECKING(for boost sub version)
  AC_MSG_RESULT([$BOOST_VERSION_SUB])

  AC_MSG_CHECKING(for boost version)
  AC_MSG_RESULT([$BOOST_VERSION])

  AC_MSG_CHECKING(for boost CPPFLAGS)
  AC_MSG_RESULT([$BOOST_CPPFLAGS])

  AC_MSG_CHECKING(for boost LDFLAGS)
  AC_MSG_RESULT([$BOOST_LDFLAGS])

  AC_MSG_CHECKING(for boost LIBS)
  AC_MSG_RESULT([$BOOST_LIBS])

  AC_MSG_CHECKING(for boost static LIBS)
  AC_MSG_RESULT([$BOOST_S_LIBS])


  AC_SUBST(HAVE_BOOST)
  AC_SUBST(BOOST_HAVE_ASIO)
  AC_SUBST(BOOST_HAVE_UUID)
  AC_SUBST(BOOST_HAVE_TEST)
  AC_SUBST(BOOST_HAVE_LOG)
  AC_SUBST(BOOST_HAVE_SYSTEM)
  AC_SUBST(BOOST_HAVE_FILESYSTEM)
  AC_SUBST(BOOST_FILESYSTEM_VERSION)
  AC_SUBST(BOOST_HAVE_IOSTREAMS)
  AC_SUBST(BOOST_IOSTREAMS_VERSION)
  AC_SUBST(BOOST_HAVE_PROGRAM_OPTIONS)
  AC_SUBST(BOOST_CPPFLAGS)
  AC_SUBST(BOOST_LDFLAGS)
  AC_SUBST(BOOST_LIBS)
  AC_SUBST(BOOST_S_LIBS)
  AC_SUBST(BOOST_LOCATION)
  AC_SUBST(BOOST_VERSION)
  AC_SUBST(BOOST_IOSTREAM_VERSION)

  AC_DEFINE_UNQUOTED([SAGA_BOOST_FILESYSTEM_VERSION], [$BOOST_FILESYSTEM_VERSION])
  AC_DEFINE_UNQUOTED([SAGA_BOOST_IOSTREAMS_VERSION],  [$BOOST_IOSTREAMS_VERSION])
])

