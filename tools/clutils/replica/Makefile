#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../

SAGA_SRC          = $(wildcard *.cpp)
SAGA_ADD_BIN_OBJ  = $(SAGA_SRC:%.cpp=%.o)
SAGA_BIN          = saga-replica
SAGA_BIN_ALIASES += saga-replica-add-directory
SAGA_BIN_ALIASES += saga-replica-add-lfn
SAGA_BIN_ALIASES += saga-replica-add-pfn
SAGA_BIN_ALIASES += saga-replica-list-attributes
SAGA_BIN_ALIASES += saga-replica-list-directory
SAGA_BIN_ALIASES += saga-replica-list-pfns
SAGA_BIN_ALIASES += saga-replica-remove-attribute
SAGA_BIN_ALIASES += saga-replica-remove-directory
SAGA_BIN_ALIASES += saga-replica-remove-lfn
SAGA_BIN_ALIASES += saga-replica-remove-pfn
SAGA_BIN_ALIASES += saga-replica-set-attribute

include $(SAGA_ROOT)/make/saga.application.mk

SAGA_CPPINCS     += -I../common
SAGA_LDFLAGS     += -L../common -lsaga_tools_common

