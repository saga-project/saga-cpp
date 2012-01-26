#
# SYNOPSIS
#
#   AX_SAGA_CHECK_POSTGRESQL([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the POSTGRESQL libraries of a particular version (or newer)
#
#   If no path to the installed postgresql library is given,
#   the macro searchs under /usr, /usr/local, /opt, /usr/local/pgsql
#   and /usr/local/package/postgresql-*
#
#   This macro calls:
#
#     AC_SUBST(HAVE_POSTGRESQL)
#     AC_SUBST(POSTGRESQL_LOCATION)
#     AC_SUBST(POSTGRESQL_CPPFLAGS) 
#     AC_SUBST(POSTGRESQL_LDFLAGS)
#     AC_SUBST(POSTGRESQL_S_LIBS)
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

AC_DEFUN([AX_SAGA_CHECK_POSTGRESQL],
[
  AC_ARG_VAR([POSTGRESQL_LOCATION],[POSTGRESQL installation directory])

  HAVE_POSTGRESQL=no

  tmp_location=""
  AC_ARG_WITH([postgresql],
              AS_HELP_STRING([--with-postgresql=DIR],
              [use postgresql (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_postgresql="no"
              elif test "$withval" = "yes"; then
                want_postgresql="yes"
                tmp_location=""
              else
                want_postgresql="yes"
                tmp_location="$withval"
              fi
              ],
              [want_postgresql="yes"])

  # use POSTGRESQL_LOCATION if avaialble, and if not 
  # overwritten by --with-postgresql=<dir>

  if test "x$want_postgresql" = "xyes"; then
    
    packages=`ls /usr/local/package/postgresql-* 2>>/dev/null`
    
    for tmp_path in $tmp_location $POSTGRESQL_LOCATION /usr /usr/local /usr/local/pgsql /opt /opt/local $packages; do
 
      AC_MSG_CHECKING(for postgresql in $tmp_path)

      have_something=`ls $tmp_path/lib{64,}/libpq* 2>/dev/null`

      if test "x$have_something" = "x"; then
      
        have_something=`ls $tmp_path/lib{64,}/postgresql*/libpq* 2>/dev/null`
        
        if test "x$have_something" = "x"; then
          AC_MSG_RESULT(no)
          continue
        else
          subdir=`echo $have_something | head -1 | rev | cut -f 2 -d '/' | rev`
          tmp_libdir=`echo $have_something | head -1 | rev | cut -f 3 -d '/' | rev`
        fi
      else
        tmp_libdir=`echo $have_something | head -1 | rev | cut -f 2 -d '/' | rev`
      fi

      saved_cppflags=$CPPFLAGS
      saved_ldflags=$LDFLAGS
      saved_ldlibpath=$LD_LIBRARY_PATH
      saved_dyldlibpath=$DYLD_LIBRARY_PATH


      POSTGRESQL_PATH=$tmp_path
      POSTGRESQL_LDFLAGS="-L$tmp_path/$tmp_libdir/$subdir -lpq"
      POSTGRESQL_CPPFLAGS="-I$tmp_path/include/$subdir -I$tmp_path/include/$subdir/pgsql -I$tmp_path/include/$subdir/postgresql"

      CPPFLAGS="$CPPFLAGS $POSTGRESQL_CPPFLAGS"
      export CPPFLAGS

      LDFLAGS="$LDFLAGS $POSTGRESQL_LDFLAGS"
      export LDFLAGS

      LD_LIBRARY_PATH="$tmp_path/$tmp_libdir/$subdir:$LD_LIBRARY_PATH"
      DYLD_LIBRARY_PATH="$tmp_path/$tmp_libdir/$subdir:$DYLD_LIBRARY_PATH"

      AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <libpq-fe.h>]],
                                      [[
                                        PGconn* connection = PQconnectdb ("testdb");
                                        return (0);
                                      ]])],
                                      link_postgresql="yes",
                                      link_postgresql="no")
            
      if test "x$link_postgresql" = "xno"; then
      
        AC_MSG_RESULT(no)

        LDFLAGS=$saved_ldflags
        CPPFLAGS=$saved_cppflags
        LD_LIBRARY_PATH=$saved_ldlibpath
        DYLD_LIBRARY_PATH=$saved_dyldlibpath
        POSTGRESQL_LOCATION=""
      
      else

        AC_MSG_RESULT(yes)

        if test -e "$tmp_path/include/$subdir/postgresql/libpq-fe.h"; then
          POSTGRESQL_CPPFLAGS="-I$tmp_path/include/$subdir/postgresql"
          SAGA_POSTGRESQL_DEP_FILES="$tmp_path/include/$subdir/postgresql/libpq-fe.h"
        fi

        if test -e "$tmp_path/include/$subdir/psql/libpq-fe.h"; then
          POSTGRESQL_CPPFLAGS="-I$tmp_path/include/$subdir/psql"
          SAGA_POSTGRESQL_DEP_FILES="$tmp_path/include/$subdir/psql/libpq-fe.h"
        fi

        if test -e "$tmp_path/include/$subdir/libpq-fe.h"; then
          POSTGRESQL_CPPFLAGS="-I$tmp_path/include/$subdir"
          SAGA_POSTGRESQL_DEP_FILES="$tmp_path/include/$subdir/libpq-fe.h"
        fi

        POSTGRESQL_LOCATION=$tmp_path

        AC_MSG_CHECKING(for postgresql version)
        AC_LANG_PUSH(C++)
        AC_RUN_IFELSE(
            [AC_LANG_PROGRAM([[#include <pg_config.h>
                               #include <fstream>]], 
                             [[std::fstream out;
                               out.open ("conftest.out", std::fstream::out);
                               out << PG_VERSION << std::endl;
                               out.close ();
                               return 0; ]])],
            [POSTGRESQL_VERSION=`cat conftest.out`],
            [POSTGRESQL_VERSION=""])

        if test "$POSTGRESQL_VERSION" = ""; then
          AC_MSG_RESULT([unknown])
        else
          # check for version > 8.x
          PQ_VERSION_MAJ=`echo $POSTGRESQL_VERSION | cut -f 1 -d .`
          PQ_VERSION_MIN=`echo $POSTGRESQL_VERSION | cut -f 2 -d .`
          PQ_VERSION_SUB=`echo $POSTGRESQL_VERSION | cut -f 3 -d .`

          if test "$PQ_VERSION_MAJ" -lt "8"; then
            HAVE_POSTGRESQL=no
            POSTGRESQL_VERSION="$POSTGRESQL_VERSION - too old"
            AC_MSG_RESULT([$POSTGRESQL_VERSION - older than 8.0])
            AC_DEFINE(SOCI_PGSQL_NOPARAMS,  1)
            AC_DEFINE(SOCI_PGSQL_NOPREPARE, 1)
          else
            HAVE_POSTGRESQL=yes
            AC_MSG_RESULT([$POSTGRESQL_VERSION - newer than 8.0])
          fi
        fi

        AC_MSG_CHECKING(for static lib postgresql)
        if test -e "$tmp_path/$tmp_libdir/$subdir/libpq.a"; then
          POSTGRESQL_S_LIBS="$tmp_path/$tmp_libdir/$subdir/libpq.a"
          AC_MSG_RESULT([$POSTGRESQL_S_LIBS])
        else
          AC_MSG_RESULT([no])
        fi

        export HAVE_POSTGRESQL
        AC_SUBST(HAVE_POSTGRESQL)
        AC_SUBST(POSTGRESQL_VERSION)
        AC_SUBST(POSTGRESQL_LOCATION)
        AC_SUBST(POSTGRESQL_CPPFLAGS)
        AC_SUBST(POSTGRESQL_LDFLAGS)
        AC_SUBST(POSTGRESQL_S_LIBS)
        
        #if test "$tmp_location" != "" && \
        #   test "$tmp_location" != "$tmp_path" ; then
        #  AC_MSG_WARN([POSTGRESQL found:
        #               not in $tmp_location
        #               but in $tmp_path])
        #fi

        break;

      fi # link ok

    done # foreach path


    #if test "$HAVE_POSTGRESQL" == "no" ; then
    #  POSTGRESQL_LOCATION=""
    #  AC_MSG_WARN(POSTGRESQL not found)
    #fi

  fi # want_postgresql

])

