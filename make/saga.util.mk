#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_INC_UTIL
       SAGA_MK_INC_UTIL = yes

###########################################################
#
# cleaning often gives trouble, as it removes config files needed for, oops,
# cleaning...  Flag it, so makefiles can handle it...  We also flag other build
# modes
#
ifneq "$(filter clean distclean configure confclean libclean depclean checkclean,$(MAKECMDGOALS))" ""
  SAGA_IS_CLEANING = yes
endif
ifneq "$(filter check,$(MAKECMDGOALS))" ""
  SAGA_IS_CHECKING = yes
endif
ifneq "$(filter srcdist,$(MAKECMDGOALS))" ""
  SAGA_IS_PACKAGING = yes
endif
#
###########################################################

ifdef SAGA_ROOT
# ifdef SAGA_LOCATION
#   $(warning both SAGA_ROOT and SAGA_LOCATION are set - picking SAGA_ROOT)
# endif
  SAGA_SCRIPT_ROOT       = $(SAGA_ROOT)/tools/scripts
  SAGA_MAKE_INCLUDE_ROOT = $(SAGA_ROOT)/make
  SAGA_INCLUDE_ROOT      = $(SAGA_ROOT)
else
  ifdef SAGA_LOCATION
    SAGA_SCRIPT_ROOT       = $(SAGA_LOCATION)/bin
    SAGA_MAKE_INCLUDE_ROOT = $(SAGA_LOCATION)/share/saga/make
    SAGA_INCLUDE_ROOT      = $(SAGA_LOCATION)/include
  else
    $(error neither SAGA_ROOT or SAGA_LOCATION are set)
  endif
endif


ifdef SAGA_CONFIGURE_OK
 ifneq "$(SAGA_CONFIGURE_OK)" "yes"
  $(warning configure failed or did not run: make may fail)
 endif
endif

INSTALL    = $(SAGA_SCRIPT_ROOT)/saga-install.sh

 include $(SAGA_MAKE_INCLUDE_ROOT)/saga.config.mk
-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.config.check.c.mk

###########################################################
#
# setup pwd
#
SAGA_PWD      := $(shell $(PWD))
#
###########################################################

###########################################################
#
# convert SAGA_ROOT into an absolute path
#
ifdef SAGA_ROOT
 SAGA_TMP_ROOT  = $(realpath $(SAGA_ROOT))/
 ifeq "$(SAGA_TMP_ROOT)" "/"
  SAGA_TMP_ROOT = $(shell cd $(SAGA_ROOT) && $(PWD))/
 endif
 SAGA_ROOT := $(SAGA_TMP_ROOT)
endif
#
###########################################################

###########################################################
#
# two helper functions, returning the upper and lower 
# case versions of their input....
#
saga_uc = $(shell echo $1 | tr '[a-z]' '[A-Z]')
saga_lc = $(shell echo $1 | tr '[A-Z]' '[a-z]')
#
###########################################################

###########################################################
#
# Another helper which replaces make's abspath function, 
# which is not available for earlier make versions.
#
saga_abs = $(shell echo " $1" | $(SED) -e 's| \([^ /]\)| $(SAGA_PWD)/\1|g')
#
###########################################################

###########################################################
#
# some hack: we need white space now and then
#
SAGA_NULL  :=
SAGA_SPACE := $(SAGA_NULL) # end of the line
#
###########################################################



endif # guard

