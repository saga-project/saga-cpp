#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying 
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT         = ../../../
SAGA_SUBDIRS      = test

SAGA_SRC          = $(wildcard *.cpp)
SAGA_OBJ          = $(SAGA_SRC:%.cpp=%.o)

SAGA_ADAPTOR_NAME = default_advert
SAGA_ADAPTOR_INI  = default_advert_adaptor.ini
SAGA_ADAPTOR_OBJ  = $(SAGA_OBJ)

SAGA_ADAPTOR_PACKAGES     = advert namespace
SAGA_ADAPTOR_REQUIREMENTS = soci package_namespace

-include $(SAGA_ROOT)/make/saga.adaptor.mk
-include $(SAGA_ROOT)/config/saga.config.soci.c.mk

distclean::
	@$(RM) default_advert_adaptor.ini

