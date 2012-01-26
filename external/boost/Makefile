#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT     = ../../

include $(SAGA_ROOT)/make/saga.util.mk

SAGA_SUBDIRS  = futures plugin spirit process logging

ifneq "$(SAGA_BOOST_HAVE_UUID)" "yes"
 SAGA_SUBDIRS += uuid
endif

ifneq "$(SAGA_BOOST_HAVE_IOSTREAMS)" "yes"
 SAGA_SUBDIRS += iostreams
endif

ifneq "$(SAGA_BOOST_HAVE_ASIO)" "yes"
 SAGA_SUBDIRS += asio
endif

ifneq "x$(SAGA_BOOST_HAVE_SYSTEM)" "xyes"
  SAGA_SUBDIRS += system
endif

# for 1.33, we use our own libboost_filesystem, but also if we could not find
# a system version
ifeq "$(SAGA_BOOST_HAVE_FILESYSTEM)" "no"
  SAGA_SUBDIRS += filesystem
endif


include $(SAGA_ROOT)/make/saga.mk

