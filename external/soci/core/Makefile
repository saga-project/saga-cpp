#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT = ../../../

-include $(SAGA_ROOT)/make/saga.util.mk

ifeq "$(SAGA_HAVE_INTERNAL_SOCI)" "yes"

  SAGA_HDR_DIR    = /soci/

# SAGA_LIB        = soci_core
  SAGA_ARC        = soci_core
  SAGA_HDR        = $(wildcard *.h)
  SAGA_SRC        = $(wildcard *.cpp)
  SAGA_LIB_OBJ    = $(SAGA_SRC:%.cpp=%.o)

  SAGA_DONT_INSTALL = 1

endif

-include $(SAGA_ROOT)/make/saga.mk

SAGA_CPPINCS  += -I.

