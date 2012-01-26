#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_MK_INC_CONFIG
       SAGA_MK_INC_CONFIG = yes

# always include default configuration
# also check for real configuration, and config checks
ifdef SAGA_ROOT
  SAGA_VERSION        = $(shell $(GREP) '^VERSION:' $(SAGA_ROOT)/PACKAGE | $(SED) -e 's/^VERSION: *//')

  SAGA_CONFIG_MK_MAN  =             $(SAGA_ROOT)/make/saga.config.default.mk
  SAGA_CONFIG_MK_OPT += $(wildcard  $(SAGA_ROOT)/make/saga.config.c.mk)
  SAGA_CONFIG_MK_OPT += $(wildcard  $(SAGA_ROOT)/make/saga.config.check.c.mk)

else
  SAGA_VERSION        = $(shell $(SAGA_LOCATION)/bin/saga-config --version)

  SAGA_CONFIG_MK_MAN  =             $(SAGA_LOCATION)/share/saga/make/saga.config.default.mk
  SAGA_CONFIG_MK_OPT += $(wildcard  $(SAGA_LOCATION)/share/saga/make/saga.config.c.mk)
  SAGA_CONFIG_MK_OPT += $(wildcard  $(SAGA_LOCATION)/share/saga/make/saga.config.check.c.mk)
endif

include $(SAGA_CONFIG_MK_MAN) $(SAGA_CONFIG_MK_OPT)

ifeq "$(SAGA_CONFIG_MK_OPT)" ""
  ifeq "$(filter srcdist clean distclean confclean configure makefiles, $(MAKECMDGOALS))" ""
    $(error "you need to run configure first")
  endif
endif 




endif # guard

