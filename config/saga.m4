
#############################################################
#
# find SAGA installation tree, and set some useful variables
#
#   HAVE_SAGA    
#   SAGA_LOCATION
#   SAGA_MAKE_DIR          # = $SAGA_LOCATION/share/saga/make/
#   SAGA_CONF_DIR          # = $SAGA_LOCATION/share/saga/config/
#   SAGA_INC_DIR           # = $SAGA_LOCATION/include/
#   SAGA_LIB_DIR           # = $SAGA_LOCATION/lib/
#   SAGA_BIN_DIR           # = $SAGA_LOCATION/bin/
#   SAGA_INI_DIR           # = $SAGA_LOCATION/share/saga/
#   SAGA_CONFIG            # = $SAGA_BIN_DIR/saga-config
#   SAGA_VERSION           # = `$SAGA_CONFIG --version`
#   SAGA_VERSION_MAJOR
#   SAGA_VERSION_MINOR
#   SAGA_VERSION_SUBMINOR
#   SAGA_PACKAGE_VERSION   # = `grep 'Version' PACKAGE`
#
# This macro stays backward compatible to old versions, by defining
#   
#   SAGA_MAKE_INLUDE_ROOT  # = $SAGA_MAKE_DIR
#   TMP_SAGA_LOCATION      # = $SAGA_LOCATION
#   SAGA_LIB_VERSION       # = $SAGA_PACKAGE_VERSION
#
# Thos vars are deprecated though, and may disappear in future versions.
# 
AC_DEFUN([AX_SAGA_CONFIG],
[
  AC_LANG(C++) 
  
  AC_ARG_VAR([SAGA_LOCATION],[SAGA installation root])

  HAVE_SAGA=no

  tmp_location=""
  AC_ARG_WITH([saga],
              AS_HELP_STRING([--with-saga=DIR],
              [use saga at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                AC_MSG_ERROR([SAGA is required])
              elif test "$withval" = "yes"; then
                tmp_location=""
              else
                tmp_location="$withval"
              fi
              ],
              [tmp_location=""])


  # try to find a valid SAGA_LOCATION amongst the possible candidates

  packages=`ls /usr/local/saga* /usr/local/package/saga* 2>>/dev/null`
  which_saga=`which saga-config 2>/dev/null | sed -e 's/\/bin\/saga-config//'`

  if test "$tmp_location-$SAGA_LOCATION" = "-"; then
    paths="$which_saga /usr /usr/local /opt /opt/local $packages"
  else
    paths="$tmp_location $SAGA_LOCATION"
  fi
    
  for tmp_path in $paths; do
 
    AC_MSG_CHECKING(for saga in $tmp_path)

    # try to verify the prefix via "saga-config --prefix"
    SAGA_CONFIG=$tmp_path/bin/saga-config

    tmp=`$SAGA_CONFIG --prefix 2> /dev/null`
 
    if ! test "x$tmp" = "x"; then
      AC_MSG_RESULT([yes])
      TMP_SAGA_LOCATION=$tmp_path
      break
    else
      AC_MSG_RESULT([no])
    fi

  done


  # found anything?
  if test "x$TMP_SAGA_LOCATION" = "x"; then
    AC_MSG_ERROR([Couldn't find SAGA installation.])
  else
    HAVE_SAGA=yes
  fi


  # if we have a usable SAGA_LOCATION, try to infer all required vars
  if ! test "x$TMP_SAGA_LOCATION" = "x"; then

    AC_MSG_CHECKING([saga version])
    SAGA_VERSION=`$SAGA_CONFIG --version`

    SAGA_VERSION_MAJOR=`echo $SAGA_VERSION    | cut -sf 1 -d . | cut -sf 1 -d '-'`
    SAGA_VERSION_MINOR=`echo $SAGA_VERSION    | cut -sf 2 -d . | cut -sf 1 -d '-'`
    SAGA_VERSION_SUBMINOR=`echo $SAGA_VERSION | cut -sf 3 -d . | cut -sf 1 -d '-'`

    if test "$SAGA_VERSION_SUBMINOR" == ""; then
      SAGA_VERSION_SUBMINOR=0
    fi

    AC_SUBST(SAGA_VERSION)
    AC_SUBST(SAGA_VERSION_MAJOR)
    AC_SUBST(SAGA_VERSION_MINOR)
    AC_SUBST(SAGA_VERSION_SUBMINOR)

    export SAGA_VERSION
    export SAGA_VERSION_MAJOR
    export SAGA_VERSION_MINOR
    export SAGA_VERSION_SUBMINOR

    AC_MSG_RESULT([$SAGA_VERSION_MAJOR.$SAGA_VERSION_MINOR.$SAGA_VERSION_SUBMINOR])
  fi


  # We set SAGA_PACKAGE_VERSION to SAGA_VERSION by default.  If a local PACKAGE file is
  # present though, for example for an adaptor, we use its version info instead.  

  AC_MSG_CHECKING([saga package version])
  SAGA_PACKAGE_VERSION=$SAGA_VERSION
  test -f PACKAGE && SAGA_PACKAGE_VERSION=` grep -e '^VERSION:'  PACKAGE | sed -e 's/^VERSION: *//'`
  test -f PACKAGE && SAGA_PACKAGE_REVISION=`grep -e '^REVISION:' PACKAGE | cut -sf 2 -d '$' | cut -sf 2 -d ' '`
  AC_MSG_RESULT([$SAGA_PACKAGE_VERSION (r$SAGA_PACKAGE_REVISION)])

  # svn info supercedes the revision info from the PACKAGE file
  svnrevision=`svn info 2>&1 | grep Revision | cut -sf 2 -d ' '`

  if ! test "$svnrevision" = ""; then
    SAGA_PACKAGE_REVISION=$svnrevision
  fi

  AC_SUBST(SAGA_PACKAGE_VERSION)
  export  SAGA_PACKAGE_VERSION
  AC_SUBST(SAGA_PACKAGE_REVISION)
  export  SAGA_PACKAGE_REVISION



  # we set SAGA_LOCATION, and a couple of other vars
  SAGA_LOCATION=$TMP_SAGA_LOCATION
  SAGA_MAKE_DIR=$SAGA_LOCATION/share/saga/make/
  SAGA_CONF_DIR=$SAGA_LOCATION/share/saga/config/
  SAGA_INC_DIR=$SAGA_LOCATION/include/
  SAGA_LIB_DIR=$SAGA_LOCATION/lib/
  SAGA_BIN_DIR=$SAGA_LOCATION/bin
  SAGA_INI_DIR=$SAGA_LOCATION/share/saga/
  SAGA_CONFIG=$SAGA_BIN_DIR/saga-config
  SAGA_VERSION=`$SAGA_CONFIG --version`

  SAGA_CORE_DEP_FILES="$SAGA_INC_DIR/saga/saga.hpp"

  export SAGA_LOCATION
  export SAGA_MAKE_DIR
  export SAGA_CONF_DIR
  export SAGA_INC_DIR
  export SAGA_LIB_DIR
  export SAGA_BIN_DIR
  export SAGA_INI_DIR
  export SAGA_CONFIG
  export SAGA_VERSION

  AC_SUBST(HAVE_SAGA)
  AC_SUBST(SAGA_LOCATION)
  AC_SUBST(SAGA_MAKE_DIR)
  AC_SUBST(SAGA_CONF_DIR)
  AC_SUBST(SAGA_INC_DIR)
  AC_SUBST(SAGA_LIB_DIR)
  AC_SUBST(SAGA_BIN_DIR)
  AC_SUBST(SAGA_INI_DIR)
  AC_SUBST(SAGA_CONFIG)
  AC_SUBST(SAGA_VERSION)


  # the adaptor and binding config files depend on the earlier findings of
  # SAGA's main configure.  
  SAGA_CONF="$SAGA_CONF_DIR/saga-config.cache"

  if test -e "$SAGA_CONF"; then
    source "$SAGA_CONF"
  else
    AC_MSG_ERROR([SAGA tree at $SAGA_LOCATION seems invalid: no $SAGA_CONF found.])
  fi

  # be backward compatible to old versions of saga.m4
  TMP_SAGA_LOCATION=$SAGA_LOCATION
  export TMP_SAGA_LOCATION
  AC_SUBST(TMP_SAGA_LOCATION)

  SAGA_MAKE_INCLUDE_DIR=$SAGA_MAKE_DIR
  export SAGA_MAKE_INCLUDE_DIR
  AC_SUBST(SAGA_MAKE_INCLUDE_DIR)

  SAGA_LIB_VERSION=$SAGA_PACKAGE_VERSION
  export SAGA_LIB_VERSION
  AC_SUBST(SAGA_LIB_VERSION)

  TMP_SAGA_INSTTYPE=install
  export TMP_SAGA_INSTTYPE
  AC_SUBST(TMP_SAGA_INSTTYPE)

  # by convention, SAGA components keep configure files in a config/subdir.  We
  # set a full path to that, so that make files can find the respective includes
  CONFDIR=`pwd`/config/
  export CONFDIR
  AC_SUBST(CONFDIR)
])


###########################################################
# 
# package dependencies
#
# find the debian package dependencies for a list of dependent files (arg1, arg2)
#
# arg1: list of files whose packages are added w/  version constraints
# arg2: list of files whose packages are added w/o version constraints
# 
AC_DEFUN([AX_SAGA_CHECK_DEBDEPS],
[
  depfiles=$1
  depfiles_nv=$2  

  AC_PATH_PROG(SAGA_DPKG, dpkg)

  if test "$SAGA_DPKG" = ""; then
    AC_MSG_RESULT([DPKG not found - not a debian system?])
  else

    SAGA_DEB_DEPENDENCIES=""
    first=yes
    oldpackage="NONE"

    for  depfile in $depfiles;
    do

      depfile=`echo $depfile | sed -e 's|//|/|g'`
      package=`$SAGA_DPKG -S $depfile 2>>/dev/null | cut -sf 1 -d ':'`

      if test "" = "$package"; then
        AC_MSG_RESULT([Cannot find debian package for $depfile])
        continue
      fi

      # AC_MSG_NOTICE([$depfile: $package])
       
      if test "$oldpackage" = "$package"; then
        # avoid repetetetive entries, to some extend
        continue;
      fi
    
      version=`$SAGA_DPKG -p $package 2>>/dev/null | grep -e '^Version:' | cut -sf 2 -d ' '`

      if test "" = "$version"; then
        AC_MSG_RESULT([Cannot find debian package version for $depfile])
        continue
      fi
     
      # this is what, for example, ubuntu 11.x reposrt for the package
      # openssh-client: 1:5.8p1-1ubuntu3
      # Google tells me that this contains version 5.8.x of the ssh tools, 
      # so we strip whatever is before the colon, and hope that scheme is
      # universally valid.  haha.
      version=`echo $version | sed -e "s/^.*\://g"`
      
      vmajor=`echo $version | cut -sf 1 -d '.'`
      vminor=`echo $version | cut -sf 2 -d '.'`
      vmin="$vmajor.$vminor"
    
      # boost version can only differ by sub-minor version, other packages can
      # differ by minor versions...
      boostmatch=`expr "$package" ":" ".*boost.*"`
    
      if test "0" = "$boostmatch"; then
        vmax="`expr 1 + $vmajor`.0"
      else
        vmax="$vmajor.`expr 1 + $vminor`"
      fi
    
      if ! test "$first" == "yes"; then
        SAGA_DEB_DEPENDENCIES="$SAGA_DEB_DEPENDENCIES, "
      fi
    
      SAGA_DEB_DEPENDENCIES="$SAGA_DEB_DEPENDENCIES$package (>= $vmin)"

      if ! test "." = `expr substr $vmax 1 1`; then
        SAGA_DEB_DEPENDENCIES="$SAGA_DEB_DEPENDENCIES, $package (<< $vmax)"
      fi

      first=no
      oldpackage=$package
    done
    
    for  depfile in $depfiles_nv;
    do

      depfile=`echo $depfile | sed -e 's|//|/|g'`
      package=`$SAGA_DPKG -S $depfile 2>>/dev/null | cut -sf 1 -d ':'`

      if test "" = "$package"; then
        AC_MSG_RESULT([Cannot find debian package for $depfile])
        continue
      fi

      # AC_MSG_NOTICE([$depfile: $package])
       
      if test "$oldpackage" = "$package"; then
        # avoid repetetetive entries, to some extend
        continue;
      fi
    
      if ! test "$first" == "yes"; then
        SAGA_DEB_DEPENDENCIES="$SAGA_DEB_DEPENDENCIES, "
      fi
    
      SAGA_DEB_DEPENDENCIES="$SAGA_DEB_DEPENDENCIES$package"

      first=no
      oldpackage=$package
    done
    
    AC_MSG_RESULT([$SAGA_DEB_DEPENDENCIES])


    AC_MSG_CHECKING([debian architecture])
    deb_arch=`$SAGA_DPKG --print-architecture`
    if test "$deb_arch" == ""; then
      SAGA_DEB_ARCH=$SAGA_ARCH
    else
      SAGA_DEB_ARCH=$deb_arch
    fi
    AC_MSG_RESULT([$SAGA_DEB_ARCH])


    AC_MSG_CHECKING([debian package version])
    SAGA_DEB_VERSION=$SAGA_VERSION
    test -f PACKAGE && SAGA_DEB_VERSION=`grep '^VERSION:' PACKAGE | sed -e 's/^VERSION: *//'`
    AC_MSG_RESULT([$SAGA_DEB_VERSION])

    AC_MSG_CHECKING([debian package name])
    SAGA_DEB_PACKAGE="NONE"
    test -f PACKAGE && SAGA_DEB_PACKAGE=`grep '^PACKAGE:' PACKAGE | sed -e 's/^PACKAGE: *//'`
    AC_MSG_RESULT([$SAGA_DEB_PACKAGE])

    ###########################################################
    #
    # with the thus gathered information, we can actually 
    # create a debian control file
    AC_MSG_CHECKING([debian package control file])
    SAGA_DEB_CONTROL=$SAGA_DEB_PACKAGE.deb.ctrl

    AC_MSG_RESULT([creating $SAGA_DEB_CONTROL])

    cat <<EOT >$SAGA_DEB_CONTROL

Package:        $SAGA_DEB_PACKAGE
Version:        $SAGA_DEB_VERSION
Section:        devel
Priority:       optional
Architecture:   $SAGA_DEB_ARCH
Essential:      no
Depends:        $SAGA_DEB_DEPENDENCIES
Pre-Depends: 
Recommends: 
Suggests: 
Installed-Size: ###SIZE###
Maintainer:     Andre Merzky [[andre--saga@merzky.net]]
Conflicts: 
Replaces: 
Provides: 
Description:    http://saga-project.org/
EOT
    #
    ###########################################################

    AC_SUBST([SAGA_DEB_DEPENDENCIES])
    AC_SUBST([SAGA_DEB_ARCH])
    AC_SUBST([SAGA_DEB_VERSION])
    AC_SUBST([SAGA_DEB_PACKAGE])
    AC_SUBST([SAGA_DEB_CONTROL])

  fi # !DPKG

])


###########################################################
# 
# package dependencies
#
# find the redhat package dependencies for a list of dependent files (arg1, arg2)
#
# arg1: list of files whose packages are added w/  version constraints
# arg2: list of files whose packages are added w/o version constraints
#
AC_DEFUN([AX_SAGA_CHECK_RPMDEPS],
[
  depfiles=$1
  depfiles_nv=$2

  AC_PATH_PROG(SAGA_RPM,      rpm)
  AC_PATH_PROG(SAGA_RPMBUILD, rpmbuild)
 
  AC_MSG_CHECKING([redhat package dependencies])

  if test "$SAGA_RPM" = ""; then
    AC_MSG_RESULT([RPM not found - not a redhat system?])
  elif test "$SAGA_RPMBUILD" = ""; then
    AC_MSG_RESULT([RPMBUILD not found - not a redhat system?])
  else
    SAGA_DEB_DEPENDENCIES=""
    first=yes
    oldpackage="NONE"
  
    for  depfile in $depfiles;
    do
  
      depfile=`echo $depfile | sed -e 's|//|/|g'`
      package=`$SAGA_RPM -qf $depfile 2>>/dev/null`
      check=`echo "$package" | grep ' '`
  
      if test "$check" = "$package"; then
        AC_MSG_RESULT([Cannot find redhat package for $depfile])
        continue
      fi

      package=`rpm -qi $package | grep Name | head -1 | cut -sf 2 -d ':' | cut -sf 2 -d ' '`
  
      # AC_MSG_NOTICE([$depfile: $package])
       
      if test "$oldpackage" = "$package"; then
        # avoid repetetetive entries, to some extend
        continue;
      fi
    
      version=`$SAGA_RPM -qi $package 2>>/dev/null | grep Version | cut -sf 2 -d ':'  | cut -sf 2 -d ' '`
  
      if test "" = "$version"; then
        AC_MSG_RESULT([Cannot find redhat package version for $depfile])
        continue
      fi
     
      # this is what, for example, ubuntu 11.x reports for the package
      # openssh-client: 1:5.8p1-1ubuntu3
      # Google tells me that this contains version 5.8.x of the ssh tools, 
      # so we strip whatever is before the colon, and hope that scheme is
      # universally valid.  haha.
      #
      # Oy, we also need to strip the patchlevel info, as some versions
      # of rpmbuild complain about that when evaluating the control file.
      version=`echo $version | sed -e "s/^.*\://g" | sed -e "s/p.*//g"`
      
      vmajor=`echo $version | cut -sf 1 -d '.'`
      vminor=`echo $version | cut -sf 2 -d '.'`
      vmin="$vmajor.$vminor"
    
      # boost version can only differ by sub-minor version, other packages can
      # differ by minor versions...
      boostmatch=`expr "$package" ":" ".*boost.*"`
    
      if test "0" = "$boostmatch"; then
        vmax="`expr 1 + $vmajor`.0"
      else
        vmax="$vmajor.`expr 1 + $vminor`"
      fi
    
      if ! test "$first" == "yes"; then
        SAGA_RPM_DEPENDENCIES="$SAGA_RPM_DEPENDENCIES, "
      fi
    
      SAGA_RPM_DEPENDENCIES="$SAGA_RPM_DEPENDENCIES$package >= $vmin"

      # we trust RPM's autoreq utilities to do the right thing, and do not
      # specify upper version bounds
      # 
      # if ! test "." = `expr substr $vmax 1 1`; then
      #   SAGA_RPM_DEPENDENCIES="$SAGA_RPM_DEPENDENCIES, $package < $vmax"
      # fi
  
      first=no
      oldpackage=$package
    done

    for  depfile in $depfiles_nv;
    do
  
      depfile=`echo $depfile | sed -e 's|//|/|g'`
      package=`$SAGA_RPM -qf $depfile 2>>/dev/null`
      check=`echo "$package" | grep ' '`
  
      if test "$check" = "$package"; then
        AC_MSG_RESULT([Cannot find redhat package for $depfile])
        continue
      fi

      package=`rpm -qi $package | grep Name | head -1 | cut -sf 2 -d ':' | cut -sf 2 -d ' '`
  
      # AC_MSG_NOTICE([$depfile: $package])
       
      if test "$oldpackage" = "$package"; then
        # avoid repetetetive entries, to some extend
        continue;
      fi
    
      if ! test "$first" == "yes"; then
        SAGA_RPM_DEPENDENCIES="$SAGA_RPM_DEPENDENCIES, "
      fi
    
      SAGA_RPM_DEPENDENCIES="$SAGA_RPM_DEPENDENCIES$package"
  
      first=no
      oldpackage=$package
    done

    if test "$SAGA_RPM_DEPENDENCIES" = ""; then
      all_deps=""
    else
      all_deps="Requires:  $SAGA_RPM_DEPENDENCIES"
    fi
    
    AC_MSG_RESULT([$SAGA_RPM_DEPENDENCIES])
  
  
    AC_MSG_CHECKING([redhat architecture])
    rpmarch=`uname -m`
    if test "$rpm_arch" == ""; then
      SAGA_RPM_ARCH=$SAGA_ARCH
    else
      SAGA_RPM_ARCH=$rpmarch
    fi
    AC_MSG_RESULT([$SAGA_RPM_ARCH])
  
  
    AC_MSG_CHECKING([redhat package version])
    SAGA_RPM_VERSION=$SAGA_VERSION
    SAGA_RPM_RELEASE=""
    test -f PACKAGE && SAGA_RPM_VERSION=`grep '^VERSION:' PACKAGE | sed -e 's/^VERSION: *//' | cut -f 1 -d '-'`
    test -f PACKAGE && SAGA_RPM_RELEASE=`grep '^VERSION:' PACKAGE | sed -e 's/^VERSION: *//' | cut -sf 2 -d '-'`
    AC_MSG_RESULT([$SAGA_RPM_VERSION - $SAGA_RPM_RELEASE])
  
    AC_MSG_CHECKING([redhat package license])
    test -f PACKAGE && SAGA_RPM_LICENSE=`grep '^LICENSE:' PACKAGE | sed -e 's/^LICENSE: *//'`
    AC_MSG_RESULT([$SAGA_RPM_LICENSE])

    AC_MSG_CHECKING([redhat package name])
    SAGA_RPM_PACKAGE="NONE"
    test -f PACKAGE && SAGA_RPM_PACKAGE=`grep '^PACKAGE:' PACKAGE | sed -e 's/^PACKAGE: *//'`
    AC_MSG_RESULT([$SAGA_RPM_PACKAGE])
  
    ###########################################################
    #
    # with the thus gathered information, we can actually 
    # create a redhat control file
    AC_MSG_CHECKING([redhat package control file])
    SAGA_RPM_CONTROL=$SAGA_RPM_PACKAGE.rpm.ctrl
 
    AC_MSG_RESULT([creating $SAGA_RPM_CONTROL])

    SAGA_RPM_FULLNAME="$SAGA_RPM_PACKAGE-$SAGA_RPM_VERSION-$SAGA_RPM_RELEASE"

    if test "$SAGA_RPM_RELEASE" = ""; then
      SAGA_RPM_RELEASE=0
      SAGA_RPM_FULLNAME="$SAGA_RPM_PACKAGE-$SAGA_RPM_VERSION"
    fi
      
    cat <<EOT >$SAGA_RPM_CONTROL

########################################################################
# 
# Header section 
#

Summary:   http://saga-project.org/
Name:      $SAGA_RPM_PACKAGE
Version:   $SAGA_RPM_VERSION
Release:   $SAGA_RPM_RELEASE
License:   '$SAGA_RPM_LICENSE'
Group:     Applications/Devel
Source:    $SAGA_RPM_FULLNAME.tgz
URL:       http://saga-project.org/release/

# temporary dir where the software should be compiled
Buildroot: %{_tmppath}/%{name}-root

# non-obvious software required to build the software
Autoreq:   1
$all_deps

%description
http://saga-project.org/


########################################################################
# 
# Prep section 
# 

%prep

# unpack the source and cd into the source directory
%setup -q -n $SAGA_RPM_FULLNAME

########################################################################
# 
# Build section 
#

%build

# less uses autoconf, so ./configure it w/ appropriate options
%configure
# compile the software
make 


########################################################################
# 
# Install section 
#

%install

# as sanity protection, make sure the Buildroot is empty
rm -rf \$RPM_BUILD_ROOT
# install software into the Buildroot
make DESTDIR=\$RPM_BUILD_ROOT install


########################################################################
# 
# Files section 
#

%files

# set perms and ownerships of packaged files
# the - indicates that the current permissions on the files should be used
%defattr(-,root,root)

# package all files within the \$RPM_BUILD_ROOT/ directory
/*


########################################################################
# 
# define a clean-up script to run after the software in Buildroot is pkg'ed
#

%clean

# just delete all files within the Buildroot
rm -rf \$RPM_BUILD_ROOT


########################################################################
# 
# Scripts section begin here
#
# No Scripts in this RPM

########################################################################
# 
# Changelog 
#

%changelog

* Mon Sep 12 2011 Andre Merzky <andre@merzky.net>
- Initial RPM packaging

EOT
    #
    ###########################################################
 
 
    AC_SUBST([SAGA_RPM_DEPENDENCIES])
    AC_SUBST([SAGA_RPM_ARCH])
    AC_SUBST([SAGA_RPM_VERSION])
    AC_SUBST([SAGA_RPM_RELEASE])
    AC_SUBST([SAGA_RPM_LICENSE])
    AC_SUBST([SAGA_RPM_PACKAGE])
    AC_SUBST([SAGA_RPM_CONTROL])

  fi # SAGA_RPM != ""

])

