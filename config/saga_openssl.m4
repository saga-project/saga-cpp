#
# SYNOPSIS
#
#   AX_SAGA_CHECK_OPENSSL([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the OPENSSL libraries of a particular version (or newer)
#
#   If no path to the installed openssl library is given,
#   the macro searchs under /usr, /usr/local, /opt and
#   /usr/local/package/openssl-*
#
#   This macro calls:
#
#     AC_SUBST(HAVE_OPENSSL)
#     AC_SUBST(OPENSSL_VERSION)
#     AC_SUBST(OPENSSL_LOCATION)
#     AC_SUBST(OPENSSL_CPPFLAGS) 
#     AC_SUBST(OPENSSL_LDFLAGS)
#     AC_SUBST(OPENSSL_S_LIBS)
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

AC_DEFUN([AX_SAGA_CHECK_OPENSSL],
[
  AC_ARG_VAR([OPENSSL_LOCATION],[OPENSSL installation directory])

  HAVE_OPENSSL=no

  tmp_location=""
  AC_ARG_WITH([openssl],
              AS_HELP_STRING([--with-openssl=DIR],
              [use openssl (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_openssl="no"
              elif test "$withval" = "yes"; then
                want_openssl="yes"
                tmp_location=""
              else
                want_openssl="yes"
                tmp_location="$withval"
              fi
              ],
              [want_openssl="yes"])

  # use OPENSSL_LOCATION if avaialble, and if not 
  # overwritten by --with-openssl=<dir>

  if test "x$want_openssl" = "xyes"; then
    
    packages=`ls /usr/local/package/openssl-* 2>>/dev/null`
    
    for tmp_path in $tmp_location $OPENSSL_LOCATION /usr /usr/local /opt $packages; do
      
      AC_MSG_CHECKING(for openssl in $tmp_path)

      saved_cppflags=$CPPFLAGS
      saved_ldflags=$LDFLAGS

      OPENSSL_PATH=$tmp_path

      # MacOS is different, for sure...
      if test "$SAGA_OSTYPE" = "SAGA_APPLE"; then
        OPENSSL_CPPFLAGS="-I$tmp_path/include/"
        OPENSSL_LDFLAGS="-L$tmp_path/lib/ -lssl -lcrypto"
      else
        OPENSSL_CPPFLAGS="-I$tmp_path/include/"
        OPENSSL_LDFLAGS="-L$tmp_path/lib/ -lssl"
      fi

      if test -e "$tmp_path/lib/libssl.a"; then
        if test "$SAGA_OSTYPE" = "$SAGA_APPLE"; then
          OPENSSL_S_LIBS="$tmp_path/lib/libssl.a $tmp_path/lib/libcrypto.a"
        else
          OPENSSL_S_LIBS="$tmp_path/lib/libssl.a"
        fi
      fi

      CPPFLAGS="$CPPFLAGS $OPENSSL_CPPFLAGS"
      export CPPFLAGS

      LDFLAGS="$LDFLAGS $OPENSSL_LDFLAGS"
      export LDFLAGS

      AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <openssl/ssl.h>]],
                                      [[
                                        SSL_library_init ();
                                        return (0);
                                      ]])],
                                      link_openssl="yes",
                                      link_openssl="no")
            
      if test "x$link_openssl" = "xno"; then
      
        AC_MSG_RESULT(no)

        LDFLAGS=$saved_ldflags
        CPPFLAGS=$saved_cppflags
        OPENSSL_LOCATION=""
      
      else

        AC_MSG_RESULT(yes)

        OPENSSL_VERSION=`ls $tmp_path/lib/libssl.so.*.*.* 2>/dev/null | sed -e "s|$tmp_path/lib/libssl.so.||"`
        export OPENSSL_VERSION

        OPENSSL_LOCATION=$tmp_path
        HAVE_OPENSSL=yes

        SAGA_OPENSSL_DEP_FILES="$OPENSSL_LOCATION/include/openssl/ssl.h"

        export HAVE_OPENSSL


        AC_SUBST(HAVE_OPENSSL)
        AC_SUBST(OPENSSL_VERSION)
        AC_SUBST(OPENSSL_LOCATION)
        AC_SUBST(OPENSSL_CPPFLAGS)
        AC_SUBST(OPENSSL_LDFLAGS)
        AC_SUBST(OPENSSL_S_LIBS)
        
        #if test "$tmp_location" != "" && \
        #   test "$tmp_location" != "$tmp_path" ; then
        #  AC_MSG_WARN([OPENSSL found:
        #               not in $tmp_location
        #               but in $tmp_path])
        #fi

        break;

      fi # link ok

    done # foreach path


    #if test "$HAVE_OPENSSL" == "no" ; then
    #  OPENSSL_LOCATION=""
    #  AC_MSG_WARN(OPENSSL not found)
    #fi

  fi # want_openssl

])

