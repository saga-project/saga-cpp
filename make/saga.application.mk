#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_MK_INC_APPLICATION
       SAGA_MK_INC_APPLICATION = yes

###########################################################
#
# make configuration
#
-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need either SAGA_ROOT or SAGA_LOCATION)
endif


include  $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk


ifdef SAGA_USE_BOOST_TEST
  SAGA_LDFLAGS_SYS += $(SAGA_BOOST_TEST_LDFLAGS)
endif

ifdef SAGA_USE_PROFILER
  SAGA_CPPINCS_SYS += $(SAGA_CPPFLAGS_PROFILER)
  SAGA_LDFLAGS_SYS += $(SAGA_LDFLAGS_PROFILER)
endif

ifdef SAGA_USE_READLINE
  SAGA_LDFLAGS_SYS += $(SAGA_LDFLAGS_READLINE)
endif

ifdef SAGA_USE_BOOST_OPTIONS
  SAGA_LDFLAGS_SYS += $(SAGA_BOOST_OPTIONS_LDFLAGS)
endif


# the test suite uses aplication.mk, but needs test includes from SAGA_ROOT
ifdef SAGA_ROOT
 SAGA_CPPINCS  += -I$(SAGA_ROOT)/test/
endif


ifdef SAGA_ROOT
  SAGA_TMP_MINCS += $(wildcard $(SAGA_MAKE_INCLUDE_ROOT)/saga.core.i.mk)
  SAGA_TMP_MINCS += $(wildcard $(SAGA_MAKE_INCLUDE_ROOT)/saga.package.*.i.mk)
else
  SAGA_TMP_MINCS += $(wildcard $(SAGA_MAKE_INCLUDE_ROOT)/saga.core.mk)
  SAGA_TMP_MINCS += $(wildcard $(SAGA_MAKE_INCLUDE_ROOT)/saga.package.*.mk)
endif

ifdef SAGA_TMP_MINCS
  include $(SAGA_TMP_MINCS)
endif


ifeq "$(SAGA_USE_LITE)" "yes"

 -include  $(SAGA_MAKE_INCLUDE_ROOT)/saga.lite.mk

 ifneq "$(SAGA_HAVE_LITE_MK)" "yes"
   ifeq "$(filter clean distclean libclean depclean confclean, $(MAKECMDGOALS))" ""
    $(warning "sorry, saga_lite is not available.")
   endif
 else
  SAGA_USE_LDFLAGS_LIB = $(SAGA_LDFLAGS_LITE)
  SAGA_USE_LDFLAGS_BIN = $(SAGA_LDFLAGS_LITE)
 endif

endif

endif # guard

