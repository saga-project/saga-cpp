#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../


include $(SAGA_ROOT)/make/saga.util.mk

SAGA_SUBDIRS     = scripts

ifeq "x$(SAGA_HAVE_PACKAGE_ADVERT)"     "xyes"
 ifeq "x$(SAGA_HAVE_PACKAGE_FILESYSTEM)" "xyes"
  ifeq "x$(SAGA_HAVE_PACKAGE_JOB)"        "xyes"
   ifeq "x$(SAGA_HAVE_PACKAGE_REPLICA)"    "xyes"
    ifeq "x$(SAGA_HAVE_PACKAGE_NAMESPACE)"  "xyes"

     SAGA_SUBDIRS    += clutils shell # fuse 

    endif
   endif
  endif
 endif
endif


include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk


