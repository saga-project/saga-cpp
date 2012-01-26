#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../
SAGA_SUBDIRS      = test


ifeq "$(SAGA_OSTYPE)" "SAGA_WINDOWS"
  SAGA_SRC        = $(sort $(wildcard *adaptor.cpp default_job*.cpp execution_environment.cpp))
else
  SAGA_SRC        = $(sort $(wildcard *adaptor.cpp posix_job*.cpp   impl_posix*.cpp))
endif

SAGA_OBJ          = $(SAGA_SRC:%.cpp=%.o)

SAGA_ADAPTOR_NAME = default_job
SAGA_ADAPTOR_INI  = default_job_adaptor.ini
SAGA_ADAPTOR_OBJ  = $(SAGA_OBJ)

SAGA_ADAPTOR_PACKAGES = job

-include $(SAGA_ROOT)/make/saga.adaptor.mk

# the following flags allow to debug client io and other things for the POSIX
# part.  Have a look at the log file at /tmp/log
#
# SAGA_CXXFLAGS += -DUSE_LOGGER
# SAGA_CXXFLAGS += -DMIGRATE_KILLS


