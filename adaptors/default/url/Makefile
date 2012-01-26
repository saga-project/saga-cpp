#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../

SAGA_SRC          = $(wildcard *.cpp)
SAGA_OBJ          = $(SAGA_SRC:%.cpp=%.o)

SAGA_ADAPTOR_NAME = local_url
SAGA_ADAPTOR_INI  = local_url_adaptor.ini
SAGA_ADAPTOR_OBJ  = $(SAGA_OBJ)

-include $(SAGA_ROOT)/make/saga.adaptor.mk

