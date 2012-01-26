#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make


ifndef SAGA_MK_INC_ADAPTOR_UTIL
       SAGA_MK_INC_ADAPTOR_UTIL = yes

###########################################################
#
# make configuration
#
-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need either SAGA_ROOT or SAGA_LOCATION)
endif


###########################################################
# 
# make configuration for adaptor builds
# 

# include the rquired make includes
ifndef SAGA_CONFDIR
  SAGA_CONFDIR = ../config
endif

# SAGA_ADAPTOR_UTIL_OPTIONS are like SAGA_ADAPTOR_REQUIREMENTS, but optional

TMP_ADAPTOR_UTIL_OPTS = $(patsubst   package_%,package.%,$(SAGA_ADAPTOR_UTIL_OPTIONS))
TMP_PACKAGE_OPTS      = $(filter     package.%,$(TMP_ADAPTOR_UTIL_OPTS))
TMP_EXTERNAL_OPTS     = $(filter-out package.%,$(TMP_ADAPTOR_UTIL_OPTS))


TMP_MAKE_INCLUDES   = $(foreach preq,$(TMP_PACKAGE_OPTS),$(SAGA_MAKE_INCLUDE_ROOT)/saga.$(preq).i.mk)
TMP_MAKE_INCLUDES  += $(foreach ereq,$(TMP_EXTERNAL_OPTS),$(SAGA_CONFDIR)/saga.config.$(ereq).c.mk)

ifneq "x$(TMP_MAKE_INCLUDES)" "x"
  ifndef SAGA_IS_CLEANING
    -include $(TMP_MAKE_INCLUDES)
  endif
endif

ifeq "$(SAGA_ADAPTOR_SUITE)" "yes"
 SAGA_LIB        = saga_adaptor_$(SAGA_ADAPTOR_UTIL_NAME)_suite
else
 SAGA_LIB        = saga_adaptor_$(SAGA_ADAPTOR_UTIL_NAME)_util
endif

SAGA_LIB_OBJ     = $(SAGA_ADAPTOR_UTIL_OBJ)
SAGA_LIB_OBJC    = $(SAGA_ADAPTOR_UTIL_OBJC)


include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk
include $(SAGA_MAKE_INCLUDE_ROOT)/saga.doc.mk


all:: $(SAGA_UTIL_LIB)

install:: all forceinstall


ifdef SAGA_ROOT
 ifneq "$(SAGA_ADAPTOR_LITE_ENABLE)" "FALSE"

  SAGA_LITE_TYPE        = UTIL
  SAGA_LITE_NAME        = $(SAGA_ADAPTOR_UTIL_NAME)
  
  include $(SAGA_ROOT)/make/saga.mklite.mk

 endif
endif


ifdef SAGA_ROOT
 SAGA_CPPINCS  += -I$(SAGA_ROOT)
 SAGA_LDFLAGS  += -L$(SAGA_ROOT)/saga/impl/engine -lsaga_core
else
 SAGA_CPPINCS  += -I$(SAGA_LOCATION)/include/saga/
 SAGA_LDFLAGS  += -L$(SAGA_LOCATION)/lib -lsaga_core
endif

endif # guard

