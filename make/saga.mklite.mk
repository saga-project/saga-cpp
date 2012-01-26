#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_INC_LITE
       SAGA_MK_INC_LITE = yes

ifeq "$(SAGA_HAVE_LITE)" "yes"

-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need SAGA_MAKE_INCLUDE_ROOT)
endif


ifndef SAGA_LITE_TYPE            
  $(error need SAGA_LITE_TYPE)
endif

ifndef SAGA_LITE_NAME
  $(error need SAGA_LITE_NAME)
endif


ifndef SAGA_LITE_LDFLAGS_SYS
  SAGA_LITE_LDFLAGS_SYS = $(SAGA_LDFLAGS_SYS)
endif

ifndef SAGA_LITE_LDFLAGS_EXT
  SAGA_LITE_LDFLAGS_EXT = $(SAGA_LDFLAGS_EXT)
endif

ifndef SAGA_LITE_OBJ
  SAGA_LITE_OBJ        = $(SAGA_LIB_OBJ) $(SAGA_ADD_LIB_OBJ)
endif

ifndef SAGA_LITE_OBJC
  SAGA_LITE_OBJC        = $(SAGA_LIB_OBJC) $(SAGA_ADD_LIB_OBJC)
endif

ifndef SAGA_LITE_LIBNAME
 ifdef SAGA_LIBNAME
  SAGA_LITE_LIBNAME     = $(SAGA_PWD)/$(SAGA_LIBNAME)
 endif
endif

ifndef SAGA_LITE_ARCNAME
 ifdef SAGA_ARCNAME
  SAGA_LITE_ARCNAME     = $(SAGA_PWD)/$(SAGA_ARCNAME)
 endif
endif

 
SAGA_LITE_TYPE_L    = $(call saga_lc, $(SAGA_LITE_TYPE))
SAGA_LITE_NAME_L    = $(call saga_lc, $(SAGA_LITE_NAME))

SAGA_LITE_CONF      = $(SAGA_ROOT)/lite/saga.lite.$(SAGA_LITE_TYPE_L).$(SAGA_LITE_NAME_L).c.mk

ifdef SAGA_LITE_OBJ
 SAGA_LITE_OBJ     := $(call saga_abs, $(SAGA_LITE_OBJ))
endif
ifdef SAGA_LITE_OBJC
 SAGA_LITE_OBJC    := $(call saga_abs, $(SAGA_LITE_OBJC))
endif
ifdef SAGA_LITE_LIBNAME
 SAGA_LITE_LIBNAME := $(call saga_abs, $(SAGA_LITE_LIBNAME))
endif
ifdef SAGA_LITE_ARCNAME
 SAGA_LITE_ARCNAME := $(call saga_abs, $(SAGA_LITE_ARCNAME))
endif

all:: $(SAGA_LITE_CONF)

$(SAGA_LITE_CONF):
	@$(ECHO) "      creating     saga lite config"
	@$(ECHO) "LITE_$(SAGA_LITE_TYPE)_NAME  = $(SAGA_LITE_NAME_L)"        > $@
	@$(ECHO) "LITE_$(SAGA_LITE_TYPE)_DEPS  = $(SAGA_LITE_DEPS)"         >> $@
	@$(ECHO) "LITE_OBJ                    += $(SAGA_LITE_OBJ)"          >> $@
	@$(ECHO) "LITE_OBJC                   += $(SAGA_LITE_OBJC)"         >> $@
	@$(ECHO) "LITE_LDFLAGS_SYS            += $(SAGA_LITE_LDFLAGS_SYS)"  >> $@         
	@$(ECHO) "LITE_LDFLAGS_EXT            += $(SAGA_LITE_LDFLAGS_EXT)"  >> $@         
	@$(ECHO) "LITE_LIB                    += $(SAGA_LITE_LIBNAME)"      >> $@
	@$(ECHO) "LITE_S_LIB                  += $(SAGA_LITE_ARCNAME)"      >> $@


clean::
	-@$(RM) $(SAGA_LITE_CONF)

endif # SAGA_HAVE_LITE

endif # guard

