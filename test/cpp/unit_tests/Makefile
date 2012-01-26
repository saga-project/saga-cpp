#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../

include $(SAGA_ROOT)/make/saga.util.mk

SAGA_SUBDIRS      = uuid

  ifeq "x$(SAGA_HAVE_PACKAGE_FILESYSTEM)" "xyes"
    SAGA_SUBDIRS   += url directory file
    SAGA_CHECKDEFS += -DSAGA_CHECK_PACKAGE_FILESYSTEM
  else
   ifdef SAGA_IS_CHECKING
    $(warning "filesystem tests disabled - package not found")
   endif
  endif

  ifeq "x$(SAGA_HAVE_PACKAGE_JOB)" "xyes"
    SAGA_SUBDIRS   += job
    SAGA_CHECKDEFS += -DSAGA_CHECK_PACKAGE_JOB
  else
   ifdef SAGA_IS_CHECKING
    $(warning "job tests disabled - package not found")
   endif
  endif

  ifeq "x$(SAGA_HAVE_PACKAGE_REPLICA)" "xyes"
   ifeq "x$(SAGA_HAVE_SQLITE3)" "xyes"
    SAGA_SUBDIRS   += logical_file logical_directory
    SAGA_CHECKDEFS += -DSAGA_CHECK_PACKAGE_REPLICA
   else
    ifdef SAGA_IS_CHECKING
     $(warning "replica tests disabled - no sqlite3 detected")
    endif
   endif
  else
   ifdef SAGA_IS_CHECKING
    $(warning "replica tests disabled - package not found")
   endif
  endif

  SAGA_SRC          = $(wildcard *.cpp)
  SAGA_ADD_BIN_OBJ += $(SAGA_SRC:%.cpp=%.o)
  SAGA_TEST_BIN     = test_saga
  SAGA_TEST_ARGS    = --catch_system_errors=no

  SAGA_ADD_BIN_SRC  = $(foreach subdir,$(SAGA_SUBDIRS),$(wildcard $(subdir)/*.cpp))
  SAGA_ADD_BIN_OBJ += $(SAGA_ADD_BIN_SRC:%.cpp=%.o)


  SAGA_USE_BOOST_TEST = "yes"

  SAGA_ENVIRON     += SAGA_ROOT=$(SAGA_ROOT)
  SAGA_ENVIRON     += BOOST_TEST_SHOW_PROGRESS=yes
  SAGA_ENVIRON     += BOOST_TEST_OUTPUT_FORMAT=HRF
  SAGA_ENVIRON     += BOOST_TEST_REPORT_LEVEL=detailed
  SAGA_ENVIRON     += BOOST_TEST_LOG_LEVEL=error
  SAGA_ENVIRON     += BOOST_TEST_BUILD_INFO=yes
  SAGA_ENVIRON     += BOOST_TEST_CATCH_SYSTEM_ERRORS=yes
  SAGA_ENVIRON     += BOOST_TEST_DETECT_MEMORY_LEAK=1

SAGA_DONT_INSTALL = yes

include $(SAGA_ROOT)/make/saga.test.mk

SAGA_CXXFLAGS += -DBOOST_TEST_DYN_LINK
SAGA_CXXFLAGS += $(SAGA_CHECKDEFS)


distclean::
	@$(RM) replica.db

