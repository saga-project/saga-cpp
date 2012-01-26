#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT     = ../../../

include $(SAGA_ROOT)/make/saga.util.mk

# onli install asio headers if boost does not bring them along
ifneq "$(SAGA_BOOST_HAVE_ASIO)" "yes"
  SAGA_SUBDIRS  = boost
endif


include $(SAGA_ROOT)/make/saga.mk

