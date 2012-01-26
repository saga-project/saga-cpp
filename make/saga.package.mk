#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_INC_PACKAGE
       SAGA_MK_INC_PACKAGE = yes

-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need either SAGA_ROOT or SAGA_LOCATION)
endif


ifneq "x$(SAGA_FACADE_PACKAGE)" "xyes"
  ifndef SAGA_IS_CLEANING
    -include $(SAGA_MAKE_INCLUDE_ROOT)/saga.core.i.mk
  endif
endif

###########################################################
# 
# SAGA_PACKAGE_NAME must be defined
# 

ifndef  SAGA_PACKAGE_NAME
  $(info make: )
  $(info make:  -- SAGA_PACKAGE_NAME name not defined! -- )
  $(info make: )
  $(info make: SAGA_PACKAGE_NAME must be set in)
  $(info make: Makefile if saga.package.mk is used.)
  $(info make: )
  $(error SAGA make error ***))
endif


ifneq "$(SAGA_HAVE_PACKAGE_$(call saga_uc,$(SAGA_PACKAGE_NAME)))" "yes"
  SAGA_DISABLE_PACKAGE  =  yes
endif


# check if that package reqs are enabled types are enabled

check_pre = $(SAGA_HAVE_$(call saga_uc,$(1)))$(SAGA_HAVE_INTERNAL_$(call saga_uc,$(1)))
check_sub = $(subst no,,$(call check_pre,$(1))$)
check_req = $(if $(strip $(call check_sub,$(1))),, \
                 SAGA_DISABLE_PACKAGE:="$(SAGA_DISABLE_PACKAGE) $(1)")
dummy = $(foreach req,          \
          $(SAGA_PACKAGE_REQUIREMENTS), \
          $(call check_req,$(req)))
endif # guard

# this stupid construct prevents eval from beeing called 
# with an ifdef'ed area, which is broken for make 3.80, at least on MacOS X...

dummy2 := $(eval $(dummy))

ifeq "$(SAGA_IS_CLEANING)" "yes"
  SAGA_DISABLE_PACKAGE =
endif



ifndef SAGA_MK_INC_PACKAGE_2
       SAGA_MK_INC_PACKAGE_2 = yes

ifneq "$(SAGA_DISABLE_PACKAGE)" ""

include $(SAGA_MAKE_INCLUDE_ROOT)/saga.nothing.mk

nothing::
	  @$(ECHO) "      ======================================================"
	  @$(ECHO) "      package disabled : $(SAGA_PACKAGE_NAME)"
	  @$(ECHO) "      ======================================================"

else

 ifneq "x$(SAGA_FACADE_PACKAGE)" "xyes"
   SAGA_PKG_LD    =  -lsaga_package_$(SAGA_PACKAGE_NAME)
   SAGA_PKG_LIB   =    saga_package_$(SAGA_PACKAGE_NAME)
   SAGA_PKG_MAK   =    saga.package.$(SAGA_PACKAGE_NAME).mk
   SAGA_PKG_MAK_I =    saga.package.$(SAGA_PACKAGE_NAME).i.mk
   
   SAGA_LIB       = $(SAGA_PKG_LIB)
   SAGA_MAK       = $(SAGA_PKG_MAK)
   SAGA_MAK_I     = $(SAGA_PKG_MAK_I)
 endif

 include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk

 SAGA_TMP = $(foreach dep,$(SAGA_PACKAGE_DEPS), \
              $(SAGA_MAKE_INCLUDE_ROOT)/saga.package.$(dep).i.mk )

 ifneq "$(strip $(SAGA_TMP))" ""
   ifndef SAGA_IS_CLEANING
    include $(SAGA_TMP)
   endif
 endif


 ifdef SAGA_ROOT
  ifneq "x$(SAGA_FACADE_PACKAGE)" "xyes"
 
   SAGA_LITE_TYPE        = PACKAGE
   SAGA_LITE_NAME        = $(SAGA_PACKAGE_NAME)
   SAGA_LITE_DEPS        = $(SAGA_PACKAGE_DEPS)
   
   include $(SAGA_ROOT)/make/saga.mklite.mk
 
  endif
 endif
 
 SAGA_CPPDEFS  += -DSAGA_PACKAGE_NAME=$(SAGA_PACKAGE_NAME)

endif # package enabled


# always allow clean
clean::
	-@$(RM) $(SAGA_LIB)

distclean::
	-@$(RM) $(SAGA_MAK) $(SAGA_MAK_I)


endif # guard

