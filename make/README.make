#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
#
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 ----------------------------------------------------------------------
|                                                                      |
| This README referes to Unix type systems which use make for building |
| SAGA.  Windows user (i.e. non-cygwin Windows user) can use the MS    |
| Developers Studio build environment.                                 |
|                                                                      |
 ----------------------------------------------------------------------


SAGA uses a gnu-make based build system.  It includes a number of
makefiles throughout the source tree, and in the $SAGA_ROOT/make/
directory.

All these make includes get installed into
$SAGA_LOCATION/share/saga/make/.

If SAGA_ROOT is set in a Makefile, the make includes from the thus
referenced saga source tree are used.  If it is not set, the make
includes are used from the installation tree in SAGA_LCOATION, which
defaults to /usr/local.

The following Makefile stub should provide a good starting point for
building SAGA applications.

 ----------------------------------------------------------------------
|                                                                      |
| SAGA_SRC  = $(wildcard *.cpp)                                        |
| SAGA_BIN  = $(SRC:%.cpp=%)                                           |
|                                                                      |
| include $(SAGA_LOCATION)/share/saga/make/saga.application.mk         |
|                                                                      |
 ----------------------------------------------------------------------


This stub loads the make rules etc. needed to build the application.
If the application needs additional include directories or libraries,
use the following syntax _after_ the make includes:

 ----------------------------------------------------------------------
|                                                                      |
| SAGA_CPPFLAGS += -I/opt/super/include                                |
| SAGA_LDFLAGS  += -L/opt/super/lib -lsuper                            |
|                                                                      |
 ----------------------------------------------------------------------


Of course it is possible to build SAGA applications with custom
Makefiles.  The includes can, however, be used to obtain the SAGA
specific compiler and linker flags:

 ----------------------------------------------------------------------
|                                                                      |
| SRC      = $(wildcard *.cpp)                                         |
| OBJ      = $(SRC:%.cpp=%.o)                                          |
| BIN      = $(SRC:%.cpp=%)                                            |
|                                                                      |
| CXX      = gcc                                                       |
| CXXFLAGS = -c -O3 -pthreads -I/opt/mpi/include                       |
|                                                                      |
| LD       = $(CXX)                                                    |
| LDFLAGS  = -L/opt/mpi/lib/ -lmpi                                     |
|                                                                      |
| include $(SAGA_LOCATION)/make/saga.core.mk                         |
| include $(SAGA_LOCATION)/make/saga.package.file.mk                   |
|                                                                      |
|                                                                      |
| .default: $(BIN)                                                     |
|                                                                      |
| $(BIN): % : %.cpp                                                    |
|                                                                      |
| $(OBJ): %.o : %.cpp                                                  |
|   $(CXX) $(CXXFLAGS) $(SAGA_CXXFLAGS) -o $@ $<                       |
|                                                                      |
| $(BIN): % : %.o                                                      |
|   $(LD)  $(LDFLAGS)  $(SAGA_LDFLAGS)  -o $@ $<                       |
|                                                                      |
 ----------------------------------------------------------------------


SAGA_CXXFLAGS and SAGA_LDFLAGS contain only those options and settings
which are absolutely required to use SAGA.  You may want to use
'make -n' to print what the resulting make commands are, in order to
debug eventual incompatibilities between SAGA compiler and linker
flags, and your own ones.

Yet another option is to use the output of saga-config for makefiles:

 ----------------------------------------------------------------------
|                                                                      |
| SRC       = $(wildcard *.cpp)                                        |
| OBJ       = $(SRC:%.cpp=%.o)                                         |
| BIN       = $(SRC:%.cpp=%)                                           |
|                                                                      |
| CXX       = gcc                                                      |
| CPPFLAGS  = -I/opt/mpi/include                                       |
| CXXFLAGS  = -c -O3 -pthreads                                         |
|                                                                      |
| LD        = $(CXX)                                                   |
| LDFLAGS   = -L/opt/mpi/lib/ -lmpi                                    |
|                                                                      |
| CPPFLAGS += $(shell $(SAGA_LOCATION)/bin/saga-config --cppflags)     |
| CXXFLAGS += $(shell $(SAGA_LOCATION)/bin/saga-config --cxxflags)     |
| LDFLAGSS += $(shell $(SAGA_LOCATION)/bin/saga-config --ldflags)      |
|                                                                      |
| .default: $(BIN)                                                     |
|                                                                      |
| $(BIN): % : %.cpp                                                    |
|                                                                      |
| $(OBJ): %.o : %.cpp                                                  |
|   $(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $<                            |
|                                                                      |
| $(BIN): % : %.o                                                      |
|   $(LD)  $(LDFLAGS) -o $@ $<                                         |
|                                                                      |
 ----------------------------------------------------------------------


The SAGA make system provides a large number of make targets.  The
most important ones are listed below:


default targets:
----------------
  all:
    the default make target, which will build all default targets.

  install:
    install the built components (implies 'make all')

special targets:
----------------
  check:
    run the test suite (implies 'make all')

  apidoc:
    run doxygen and pydoc to generate the API documentation

  depend:
    runs the dependency checks, but does not attempt to build anything

  configure:
    re-create the configure script from configure.in and the m4 macros

  lib:
    create library targets only

  bin:
    create binary executable targets only


cleaning targets:
-----------------
  clean:
    clean all temporary compile files

  depclean:
    clean temporary dependency information

  distclean:
    clean all created files, thus resetting the source tree to its 
    initial, virgin state

  libclean:
    remove build libraries

  binclean:
    clean binary executables


packagin targets:
-----------------
  dist:
    run srcdist and bindist make targets

  srcdist:
    create a tarball of the source tree

  bindist:
    create a tarball of the install tree (implies 'make all')

  deb: (beta)
    create a binary debian package distribution

  rpm: (not implemented)
    create a binary rehead package distribution


HOWTO create/install/test a debian package:
-------------------------------------------

  Note that the debian packages will get installed under the
  same user-id as used when creating the packages - packages
  should thus be created as root. 

  Please remember to generate all documentations before running 
  'make deb' - otherwise the deb package will not contain the
  documentation, obviously.


   export PREFIX=/usr/local/packages/saga-1.6/

   ./configure --prefix=$PREFIX                                   # configure the sources

   make deb                                                       # build     the package
   sudo dpkg -c saga-core-1.5.4.deb                               # inspects  the package
   sudo dpkg -i saga-core-1.5.4.deb                               # installs  the package

   dpkg -l | grep saga                                            # check     the installation
   $PREFIX/bin/saga-run.sh saga-file list_dir /                   # test      the installation
   LD_LIBRARY_PATH=$PREFIX/lib $PREFIX/bin/saga-file list_dir /   # test      the installation

   sudo dpkg -r saga-core-1.5.4.deb                               # removes   the package
   sudo dpkg -p saga-core-1.5.4.deb                               # purges    the package



  When install fails due to architecture mismatch (e.g., if
  x86_64 seems to be incompatible with amd64, although it
  is not), then try '--force-architecture'.

