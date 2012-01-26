#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../

SAGA_SUBDIRS      = ini

SAGA_LIB          = saga_core
SAGA_HDR          = $(wildcard *.hpp)
SAGA_SRC          = $(wildcard *.cpp)

# FIXME: really, stuff to be compiled from dir below and from 
# here should be merged into a single dir.
SAGA_SRC         += $(wildcard ../*.cpp)


SAGA_LIB_OBJ      = $(SAGA_SRC:%.cpp=%.o)

SAGA_MAK          = saga.core.mk
SAGA_MAK_I        = saga.core.i.mk

# add the base SAGA objects and adaptor utils
SAGA_TMP_LIB_OBJ += $(wildcard $(SAGA_ROOT)/saga/saga/*.o)
SAGA_TMP_LIB_OBJ += $(wildcard $(SAGA_ROOT)/saga/saga/adaptors/*.o)
SAGA_TMP_LIB_OBJ += $(wildcard $(SAGA_ROOT)/saga/saga/adaptors/utils/*.o)
SAGA_TMP_LIB_OBJ += $(wildcard $(SAGA_ROOT)/saga/saga/adaptors/utils/ini/*.o)
SAGA_TMP_LIB_OBJ += $(wildcard $(SAGA_ROOT)/saga/saga/adaptors/utils/process/*.o)

# add the core implementation and helpers
# SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/impl/exception.o
# SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/impl/exception_list.o
# SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/impl/runtime.o
SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/impl/engine/ini/ini.o
SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/saga/detail/safe_getenv.o
SAGA_TMP_LIB_OBJ += $(SAGA_ROOT)/saga/saga/detail/module_needs_logging.o

# remove ini_test (contains main() )
SAGA_LIB_OBJ     += $(filter-out $(SAGA_ROOT)/impl/engine/ini/ini_test.o,$(SAGA_TMP_LIB_OBJ))

include $(SAGA_ROOT)/make/saga.mk

SAGA_CXXFLAGS    += -DSAGA_ENGINE_EXPORTS=1 -DSAGA_PACKAGE_LIB_NAME=libsaga_core

SAGA_LITE_TYPE    = CORE
SAGA_LITE_NAME    = CORE
SAGA_LITE_OBJ     = $(filter-out engine.o,$(SAGA_LIB_OBJ))

include $(SAGA_ROOT)/make/saga.mklite.mk


distclean::
	-@rm -f $(SAGA_MAK) $(SAGA_MAK_I)

