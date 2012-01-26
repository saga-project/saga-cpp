#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../

include $(SAGA_ROOT)/make/saga.util.mk


SAGA_SUBDIRS += common
SAGA_SUBDIRS += context


ifeq "x$(SAGA_HAVE_PACKAGE_ADVERT)" "xyes"
  SAGA_SUBDIRS += advert
endif

ifeq "x$(SAGA_HAVE_PACKAGE_FILESYSTEM)" "xyes"
  SAGA_SUBDIRS += filesystem
endif

ifeq "x$(SAGA_HAVE_PACKAGE_JOB)" "xyes"
  SAGA_SUBDIRS += job
endif

ifeq "x$(SAGA_HAVE_PACKAGE_REPLICA)" "xyes"
  SAGA_SUBDIRS += replica
endif

ifeq "x$(SAGA_HAVE_PACKAGE_SD)" "xyes"
  SAGA_SUBDIRS += sd
endif

ifeq "x$(SAGA_HAVE_PACKAGE_ISN)" "xyes"
  SAGA_SUBDIRS += isn
endif

# ifeq "x$(SAGA_HAVE_PACKAGE_STREAM)" "xyes"
#   SAGA_SUBDIRS += stream
# endif


include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk

context:  common
advert:   common
file:     common
job:      common
replica:  common
sd:       common
isn:      common

