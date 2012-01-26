#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT = ../../../

-include $(SAGA_ROOT)/make/saga.util.mk

ifeq "$(SAGA_HAVE_INTERNAL_SOCI)-$(SAGA_HAVE_SQLITE3)" "yes-yes"

  SAGA_HDR        = soci-sqlite3.h
  SAGA_HDR_DIR    = .

  SAGA_ARC        = soci_sqlite3
  SAGA_SRC        = $(wildcard *.cpp)
  SAGA_LIB_OBJ    = $(SAGA_SRC:%.cpp=%.o)

  SAGA_DONT_INSTALL = 1

endif # build soci with sqlite3

-include $(SAGA_ROOT)/make/saga.mk
-include $(SAGA_ROOT)/config/saga.config.sqlite3.c.mk

SAGA_CPPINCS    += -I../core/
SAGA_LDFLAGS    += -L../core/ -lsoci_core

