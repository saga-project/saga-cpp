#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying 
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../../

SAGA_HDR          = $(wildcard *.hpp)
SAGA_SRC          = $(wildcard *.cpp)
SAGA_LIB_OBJ      = $(SAGA_SRC:%.cpp=%.o)

SAGA_LIB_OBJ     += $(wildcard $(SAGA_ROOT)/saga/saga/packages/cpr/*.o)
SAGA_LIB_OBJ     += $(wildcard $(SAGA_ROOT)/saga/saga/packages/cpr/adaptors/*.o)

SAGA_PACKAGE_NAME = cpr
SAGA_PACKAGE_DEPS = job namespace filesystem

include $(SAGA_ROOT)/make/saga.package.mk

