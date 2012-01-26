#
# SYNOPSIS
#
#   AX_SAGA_CHECK_XMLRPC([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the XMLRPC libraries of a particular version (or newer)
#
#   If no path to the installed xmlrpc library is given,
#   the macro searchs under /usr, /usr/local, /opt and
#   /usr/local/package/xmlrpc-*
#
#   This macro calls:
#
#     AC_SUBST(HAVE_XMLRPC)
#     AC_SUBST(XMLRPC_LOCATION)
#     AC_SUBST(XMLRPC_CPPFLAGS) 
#     AC_SUBST(XMLRPC_LDFLAGS)
#     AC_SUBST(XMLRPC_S_LIBS)
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

AC_DEFUN([AX_SAGA_CHECK_XMLRPC],
[
  AC_ARG_VAR([XMLRPC_LOCATION],[XMLRPC installation directory])

  #AC_MSG_NOTICE([XMLRPC_LOCATION: $XMLRPC_LOCATION])

  HAVE_XMLRPC=no

  tmp_location=""
  AC_ARG_WITH([xmlrpc],
              AS_HELP_STRING([--with-xmlrpc=DIR],
              [use xmlrpc (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_xmlrpc="no"
              elif test "$withval" = "yes"; then
                want_xmlrpc="yes"
                tmp_location=""
              else
                want_xmlrpc="yes"
                tmp_location="$withval"
              fi
              ],
              [want_xmlrpc="yes"])

  # use XMLRPC_LOCATION if avaialble, and if not 
  # overwritten by --with-xmlrpc=<dir>

  if test "x$want_xmlrpc" = "xyes"; then
    
    packages=`ls /usr/local/package/xmlrpc-* 2>>/dev/null`
    
    for tmp_path in $tmp_location $XMLRPC_LOCATION /usr /usr/local /opt /opt/local $packages; do
      
      AC_MSG_CHECKING(for xmlrpc lib in $tmp_path)

      have_libname_1=`ls $tmp_path/lib/libxmlrpc++.* 2>/dev/null`
      have_libname_2=`ls $tmp_path/lib/libXmlRpc.*   2>/dev/null`

      if test "x$have_libname_1" = "x"; then
        if test "x$have_libname_2" = "x"; then
          AC_MSG_RESULT(no)
          continue
        else
          libname="XmlRpc"
        fi
      else
        libname="xmlrpc++"
      fi

      AC_MSG_RESULT($libname)


      AC_MSG_CHECKING(for xmlrpc header in $tmp_path)

      have_headername_1=`ls $tmp_path/include/XmlRpcCpp.h 2>/dev/null`
      have_headername_2=`ls $tmp_path/include/XmlRpc.h   2>/dev/null`ZZZZ

      if test "x$have_headername_1" = "x"; then
        if test "x$have_headername_2" = "x"; then
          AC_MSG_RESULT(no)
          continue
        else
          headername="XmlRpc"
        fi
      else
        headername="XmlRpcCpp"
      fi

      AC_MSG_RESULT($headername)

      saved_cppflags=$CPPFLAGS
      saved_ldflags=$LDFLAGS

      XMLRPC_PATH=$tmp_path
      XMLRPC_LDFLAGS="-L$tmp_path/lib/ -l$libname"
      XMLRPC_CPPFLAGS="-I$tmp_path/include/"

      CPPFLAGS="$CPPFLAGS $XMLRPC_CPPFLAGS"
      export CPPFLAGS

      LDFLAGS="$LDFLAGS $XMLRPC_LDFLAGS"
      export LDFLAGS

      AC_MSG_CHECKING(xmlrpc linkage)

      AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <$headername.h>]],
                                      [[
                                        XmlRpc::XmlRpcClient c ("localhost", 1234, NULL, false);
                                        return (0);
                                      ]])],
                                      link_xmlrpc="yes",
                                      link_xmlrpc="no")
            
      if test "x$link_xmlrpc" = "xyes"; then

        XMLRPC_NEEDS_BOOL=1

      else
      
        AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <$headername.h>]],
                                        [[
                                          XmlRpc::XmlRpcClient c ("localhost", 1234, NULL);
                                          return (0);
                                        ]])],
                                        link_xmlrpc="yes",
                                        link_xmlrpc="no")
            
        if test "x$link_xmlrpc" = "xyes"; then

          XMLRPC_NEEDS_BOOL=0

        else

          AC_MSG_RESULT(no)
          LDFLAGS=$saved_ldflags
          CPPFLAGS=$saved_cppflags
        
        fi

      fi


      if test "x$link_xmlrpc" = "xyes"; then

        AC_MSG_RESULT(yes)

        SAGA_XMLRPC_DEP_FILES="$tmp_path/include/$headername.h"

        AC_MSG_CHECKING(for static lib xmlrpc)
        if test -e "$tmp_path/lib/lib$libname.a"; then
          XMLRPC_S_LIBS="$tmp_path/lib/lib$libname.a"
          AC_MSG_RESULT([$XMLRPC_S_LIBS])
        else
          AC_MSG_RESULT([no])
        fi

        XMLRPC_VERSION=`ls $tmp_path/lib/lib$libname.so.*.*.* 2>/dev/null | sed -e 's|$tmp_path/lib/lib$libname.so.||'`
        export XMLRPC_VERSION

        XMLRPC_LOCATION=$tmp_path
        HAVE_XMLRPC=yes

        XMLRPC_HEADER=$headername

        export HAVE_XMLRPC

        AC_SUBST(HAVE_XMLRPC)
        AC_SUBST(XMLRPC_LOCATION)
        AC_SUBST(XMLRPC_VERSION)
        AC_SUBST(XMLRPC_CPPFLAGS)
        AC_SUBST(XMLRPC_LDFLAGS)
        AC_SUBST(XMLRPC_HEADER)
        AC_SUBST(XMLRPC_S_LIBS)
        AC_SUBST(XMLRPC_NEEDS_BOOL)
        
        AC_DEFINE_UNQUOTED([SAGA_XMLRPC_NEEDS_BOOL], ["$XMLRPC_NEEDS_BOOL"])
        AC_DEFINE_UNQUOTED([SAGA_XMLRPC_HEADER],     ["$XMLRPC_HEADER"])

        #if test "$tmp_location" != "" && \
        #   test "$tmp_location" != "$tmp_path" ; then
        #  AC_MSG_WARN([XMLRPC found:
        #               not in $tmp_location
        #               but in $tmp_path])
        #fi

        break;

      fi # link ok

    done # foreach path


    #if test "$HAVE_XMLRPC" == "no" ; then
    #  XMLRPC_LOCATION=""
    #  AC_MSG_WARN(XMLRPC not found)
    #fi

  fi # want_xmlrpc

])

