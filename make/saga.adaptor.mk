#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make


ifndef SAGA_MK_INC_ADAPTOR
       SAGA_MK_INC_ADAPTOR = yes

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

ifndef SAGA_ADAPTOR_NAME
  $(info make: )
  $(info make:  -- Adaptor name not defined! -- )
  $(info make: )
  $(info make: SAGA_ADAPTOR_NAME must be set in Makefile)
  $(info make: if saga.adaptor.mk is used.)
  $(info make: )
  $(error SAGA make error ***))
endif


# include the rquired make includes
ifndef SAGA_CONFDIR
  SAGA_CONFDIR = ../config
endif

TMP_ADAPTOR_REQS    = $(patsubst   package_%,package.%,$(SAGA_ADAPTOR_REQUIREMENTS))
TMP_PACKAGE_REQS    = $(filter     package.%,$(TMP_ADAPTOR_REQS))
TMP_EXTERNAL_REQS   = $(filter-out package.%,$(TMP_ADAPTOR_REQS))


ifdef SAGA_ROOT
  TMP_MAKE_INCLUDES   = $(foreach preq,$(TMP_PACKAGE_REQS),$(SAGA_MAKE_INCLUDE_ROOT)/saga.$(preq).i.mk)
else
  TMP_MAKE_INCLUDES   = $(foreach preq,$(TMP_PACKAGE_REQS),$(SAGA_MAKE_INCLUDE_ROOT)/saga.$(preq).mk)
endif

TMP_MAKE_INCLUDES  += $(foreach ereq,$(TMP_EXTERNAL_REQS),$(SAGA_CONFDIR)/saga.config.$(ereq).c.mk)

ifneq "x$(TMP_MAKE_INCLUDES)" "x"
  ifndef SAGA_IS_CLEANING
    include $(TMP_MAKE_INCLUDES)
  endif
endif


# SAGA_ADAPTOR_OPTIONS are just like SAGA_ADAPTOR_REQUIREMENTS, but they are,
# well, optional (-include)

TMP_ADAPTOR_OPTS    = $(patsubst   package_%,package.%,$(SAGA_ADAPTOR_OPTIONS))
TMP_PACKAGE_OPTS    = $(filter     package.%,$(TMP_ADAPTOR_OPTS))
TMP_EXTERNAL_OPTS   = $(filter-out package.%,$(TMP_ADAPTOR_OPTS))


ifdef SAGA_ROOT
  TMP_MAKE_INCLUDES   = $(foreach preq,$(TMP_PACKAGE_OPTS),$(SAGA_MAKE_INCLUDE_ROOT)/saga.$(preq).i.mk)
else
  TMP_MAKE_INCLUDES   = $(foreach preq,$(TMP_PACKAGE_OPTS),$(SAGA_MAKE_INCLUDE_ROOT)/saga.$(preq).mk)
endif

TMP_MAKE_INCLUDES  += $(foreach ereq,$(TMP_EXTERNAL_OPTS),$(SAGA_CONFDIR)/saga.config.$(ereq).c.mk)

ifneq "x$(TMP_MAKE_INCLUDES)" "x"
  ifndef SAGA_IS_CLEANING
    -include $(TMP_MAKE_INCLUDES)
  endif
endif



# check if that package types are enabled, and of pre-requisites are met
HAVE_PACKAGES=$(foreach p,$(SAGA_ADAPTOR_PACKAGES),$(SAGA_HAVE_PACKAGE_$(call saga_uc,$p)))
TMP_CHECK=$(strip $(patsubst yes,,$(HAVE_PACKAGES)))

ifeq "x$(TMP_CHECK)" "x"
 
# check if we have the pre-requisits
# check_pre = $(or $(SAGA_HAVE_$(call saga_uc,$(1))) \
#                  $(SAGA_HAVE_INTERNAL_$(call saga_uc,$(1))) )
  check_pre = $(SAGA_HAVE_$(call saga_uc,$(1)))$(SAGA_HAVE_INTERNAL_$(call saga_uc,$(1)))
  check_sub = $(subst no,,$(call check_pre,$(1)))
  check_req = $(if $(strip $(call check_sub,$(1))),, \
                   SAGA_DISABLE_ADAPTOR:="$(SAGA_DISABLE_ADAPTOR) $(1)")
  dummy = $(foreach req,          \
            $(SAGA_ADAPTOR_REQUIREMENTS), \
            $(call check_req,$(req)))
else
	SAGA_DISABLE_ADAPTOR="one of the SAGA packages '$(call saga_uc,$(SAGA_NEED_PACKAGES))'"
endif
endif # guard

# this stupid construct prevents eval from beeing called 
# with an ifdef'ed area, which is broken for make 3.80, at least on MacOS X...

dummy2 := $(eval $(dummy))

ifeq "$(SAGA_IS_CLEANING)" "yes"
  SAGA_DISABLE_ADAPTOR =
endif


ifndef SAGA_MK_INC_ADAPTOR_2
       SAGA_MK_INC_ADAPTOR_2 = yes



ifneq "$(SAGA_DISABLE_ADAPTOR)" ""
 
include $(SAGA_MAKE_INCLUDE_ROOT)/saga.nothing.mk
nothing::
	  @$(ECHO) "      ======================================================"
	  @$(ECHO) "      the $(SAGA_ADAPTOR_NAME) adaptor is disabled."
		@$(ECHO) "      $(SAGA_DISABLE_ADAPTOR) missing"
	  @$(ECHO) "      ======================================================"

else

SAGA_ADP         = libsaga_adaptor_$(SAGA_ADAPTOR_NAME).$(SAGA_LEXT)
SAGA_ADP_STAT    = libsaga_adaptor_$(SAGA_ADAPTOR_NAME).$(SAGA_AEXT)
SAGA_ADP_ROOT    = $(SAGA_LOCATION)/lib/
SAGA_ADP_ROOTQ   = "$$SAGA_LOCATION/lib/"
SAGA_ADP_INI     = saga_adaptor_$(SAGA_ADAPTOR_NAME).ini

SAGA_LIB         = saga_adaptor_$(SAGA_ADAPTOR_NAME)
SAGA_LIB_OBJ     = $(SAGA_ADAPTOR_OBJ)
SAGA_LIB_OBJC    = $(SAGA_ADAPTOR_OBJC)


include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk
include $(SAGA_MAKE_INCLUDE_ROOT)/saga.doc.mk


ifdef SAGA_ROOT
  SAGA_PACKAGE_INC = $(foreach p,$(SAGA_ADAPTOR_PACKAGES),$(SAGA_MAKE_INCLUDE_ROOT)/saga.package.$p.i.mk)
else
  SAGA_PACKAGE_INC = $(foreach p,$(SAGA_ADAPTOR_PACKAGES),$(SAGA_MAKE_INCLUDE_ROOT)/saga.package.$p.mk)
endif

ifneq "$(SAGA_PACKAGE_INC)" ""
  ifndef SAGA_IS_CLEANING
    include $(SAGA_PACKAGE_INC)
  endif
endif


all:: $(SAGA_ADP) $(SAGA_ADP_STAT)

install:: all forceinstall

.PHONY: forceinstall
forceinstall:: subdirs
 ifndef SAGA_DONT_INSTALL
  ifneq "$(SAGA_ADAPTOR_HAS_INI)" "FALSE"
	 @$(ECHO) "      installing   adaptor ini"
	 @$(INSTALL) -f -t $(SAGA_ADAPTOR_INI) $(SAGA_INI_ROOT)/$(SAGA_ADP_INI)

  endif
 endif


ifdef SAGA_ROOT
 ifneq "$(SAGA_ADAPTOR_LITE_ENABLE)" "FALSE"

  SAGA_LITE_TYPE        = ADAPTOR
  SAGA_LITE_NAME        = $(SAGA_ADAPTOR_NAME)
  SAGA_LITE_DEPS        = $(SAGA_ADAPTOR_PACKAGES)
  
  include $(SAGA_ROOT)/make/saga.mklite.mk

 endif
endif


clean::
	-@$(RM)    $(SAGA_ADP) $(SAGA_ADP_STAT)


SAGA_CPPDEFS  += -DSAGA_ADAPTOR_NAME=$(SAGA_ADAPTOR_NAME)

ifdef SAGA_ROOT
 SAGA_CPPINCS  += -I$(SAGA_ROOT)/test/
 SAGA_LDFLAGS  += -L$(SAGA_ROOT)/saga/impl/engine -lsaga_core
else
 SAGA_CPPINCS  += -I$(SAGA_LOCATION)/include/saga/
 SAGA_LDFLAGS  += -L$(SAGA_LOCATION)/lib -lsaga_core
endif

endif # package enabled

endif # guard

