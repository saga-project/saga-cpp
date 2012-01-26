#
# SYNOPSIS
#
#   AX_SAGA_CHECK_SQLITE3([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the SQLITE3 libraries of a particular version (or newer)
#
#   If no path to the installed sqlite3 library is given,
#   the macro searchs under /usr, /usr/local, /opt and
#   /usr/local/package/sqlite3-*
#
#   This macro calls:
#
#     AC_SUBST(HAVE_SQLITE3)
#     AC_SUBST(SQLITE3_LOCATION)
#     AC_SUBST(SQLITE3_CPPFLAGS) 
#     AC_SUBST(SQLITE3_LDFLAGS)
#     AC_SUBST(SQLITE3_S_LIBS)
#
# LAST MODIFICATION
#
#   2007-03-04
#
# COPYLEFT
#
#   Copyright (c) 2007 Andre Merzky      <andre@merzky.net>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_SAGA_CHECK_SQLITE3],
[
  AC_ARG_VAR([SQLITE3_LOCATION],[SQLITE3 installation directory])

  HAVE_SQLITE3=no
  HAVE_SQLITE3_DTOR_TYPE=0

  tmp_location=""
  AC_ARG_WITH([sqlite3],
              AS_HELP_STRING([--with-sqlite3=DIR],
              [use sqlite3 (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_sqlite3="no"
              elif test "$withval" = "yes"; then
                want_sqlite3="yes"
                tmp_location=""
              else
                want_sqlite3="yes"
                tmp_location="$withval"
              fi
              ],
              [want_sqlite3="yes"])

  # use SQLITE3_LOCATION if avaialble, and if not 
  # overwritten by --with-sqlite3=<dir>

  if test "x$want_sqlite3" = "xyes"; then
    
    packages=`ls /usr/local/package/sqlite3-* 2>>/dev/null`
  
    which_path=`which sqlite3 | rev | cut -f 3- -d '/' | rev`
    
    for tmp_path in $tmp_location $SQLITE3_LOCATION $which_path /usr /usr/local /opt $packages; do
      
      AC_MSG_CHECKING(for sqlite3 in $tmp_path)

      if ! test -x "$tmp_path/bin/sqlite3"; then
        AC_MSG_RESULT(no)
        continue
      fi

      if test "$SAGA_OSTYPE" = "SAGA_APPLE"; then
        sqlite3_libname=`otool -L $tmp_path/bin/sqlite3 | grep libsqlite3 | cut -f 2 | cut -f 1 -d ' '`
        sqlite3_libpath=`echo $sqlite3_libname | rev | cut -f 2- -d '/' | rev`
        sqlite3_libbase=`echo $sqlite3_libname | rev | cut -f 1  -d '/' | rev | sed -e 's/^lib//' | sed -e 's/.dylib$//'`
      else
        sqlite3_libname=`ldd $tmp_path/bin/sqlite3 | grep libsqlite3 | cut -f 2 -d '>' | cut -f 2 -d ' '`
        sqlite3_libpath=`echo $sqlite3_libname | rev | cut -f 2- -d '/' | rev`
        sqlite3_libbase=`echo $sqlite3_libname | rev | cut -f 1  -d '/' | rev | sed -e 's/^lib//' | cut -f 1 -d '.'`
      fi

      saved_cppflags=$CPPFLAGS
      saved_ldflags=$LDFLAGS

      SQLITE3_PATH=$tmp_path
      SQLITE3_LDFLAGS="-L$sqlite3_libpath -l$sqlite3_libbase"
      SQLITE3_CPPFLAGS="-I$tmp_path/include/"

      CPPFLAGS="$CPPFLAGS $SQLITE3_CPPFLAGS"
      export CPPFLAGS

      LDFLAGS="$LDFLAGS $SQLITE3_LDFLAGS"
      export LDFLAGS

      AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <sqlite3.h>]],
                                      [[sqlite3 *db;
                                        int t = 1;
                                        return (sqlite3_open ("test_db", &db));
                                      ]])],
                                      link_sqlite3="yes",
                                      link_sqlite3="no")
            
      if test "x$link_sqlite3" = "xno"; then
      
        AC_MSG_RESULT(no)

        LDFLAGS=$saved_ldflags
        CPPFLAGS=$saved_cppflags
      
      else

        AC_MSG_RESULT(yes)

        SAGA_SQLITE3_DEP_FILES="$tmp_path/include/sqlite3.h"

        SQLITE3_VERSION=`$tmp_path/bin/sqlite3 -version`
        export SQLITE3_VERSION


        AC_MSG_CHECKING(for static lib sqlite3)
        if test -e "$sqlite3_libpath/libsqlite3.a"; then
          SQLITE3_S_LIBS="$sqlite3_libpath/libsqlite3.a"
          AC_MSG_RESULT([$SQLITE3_S_LIBS])
        else
          AC_MSG_RESULT([no])
        fi

        SQLITE3_LOCATION=$tmp_path
        HAVE_SQLITE3=yes

        export HAVE_SQLITE3

        AC_SUBST(HAVE_SQLITE3)
        AC_SUBST(SQLITE3_VERSION)
        AC_SUBST(SQLITE3_LOCATION)
        AC_SUBST(SQLITE3_CPPFLAGS)
        AC_SUBST(SQLITE3_LDFLAGS)
        AC_SUBST(SQLITE3_S_LIBS)
        
        if test "$tmp_location" != "" && \
           test "$tmp_location" != "$tmp_path" ; then
          AC_MSG_WARN([SQLITE3 found:
                       not in $tmp_location
                       but in $tmp_path])
        fi

        break;

      fi # link ok

    done # foreach path


    if test "$HAVE_SQLITE3" == "no" ; then
      AC_MSG_WARN(SQLITE3 not found)
    else

      # need to test for the destructor type
      AC_MSG_CHECKING([for sqlite3_destructur_type])

      AC_LINK_IFELSE([AC_LANG_PROGRAM([[
                                        namespace sqlite_api {
                                          @%:@include <sqlite3.h>
                                        }
                                      ]],
                                      [[
                                        using sqlite_api::sqlite3_destructor_type;
                                        SQLITE_STATIC;
                                        return (0);
                                      ]])],
                                      have_dtor_type="yes",
                                      have_dtor_type="no")
            
      if test "x$have_dtor_type" = "xyes"; then
        
        AC_MSG_RESULT(yes)

        HAVE_SQLITE3_DTOR_TYPE=1

      else
        
        AC_MSG_RESULT(no)

      fi

    fi

    AC_SUBST(HAVE_SQLITE3_DTOR_TYPE)

    if test "$HAVE_SQLITE3" == "no" ; then
      SQLITE3_LOCATION=""
      AC_MSG_WARN(SQLITE3 not found)
    fi

  fi # want_sqlite3

])

