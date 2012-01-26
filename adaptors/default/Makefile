#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)


SAGA_ROOT = ../../

-include $(SAGA_ROOT)/make/saga.config.check.c.mk


SAGA_SUBDIRS    = url context

ifeq "x$(SAGA_BUILD_ADAPTOR_ADVERT)" "xyes"
  SAGA_SUBDIRS += advert
endif

ifeq "x$(SAGA_BUILD_ADAPTOR_REPLICA)" "xyes"
  SAGA_SUBDIRS += replica
endif

ifeq "x$(SAGA_BUILD_ADAPTOR_FILESYSTEM)" "xyes"
  SAGA_SUBDIRS += file
endif

ifeq "x$(SAGA_BUILD_ADAPTOR_JOB)" "xyes"
  SAGA_SUBDIRS += job
endif

ifeq "x$(SAGA_BUILD_ADAPTOR_RPC)" "xyes"
  SAGA_SUBDIRS += rpc
endif

ifeq "x$(SAGA_BUILD_ADAPTOR_SD)" "xyes"
  SAGA_SUBDIRS += sd
endif

 ifeq "x$(SAGA_BUILD_ADAPTOR_STREAM)" "xyes"
   SAGA_SUBDIRS += stream
 endif

ifeq "x$(SAGA_BUILD_ADAPTOR_CPR)" "xyes"
  SAGA_SUBDIRS += cpr
endif


ifdef SAGA_IS_CLEANING
  SAGA_SUBDIRS += advert replica file job rpc sd stream cpr
endif

-include $(SAGA_ROOT)/make/saga.mk

