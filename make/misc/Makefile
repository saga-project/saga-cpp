#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT    = ../../

SAGA_MISC    = $(wildcard *.tex)
SAGA_MAK_DIR = misc/

include $(SAGA_ROOT)/make/saga.mk

install::
	@echo '      installing misc'
	@$(INSTALL)     $(SAGA_MISC)    $(SAGA_MAK_ROOT)/$(SAGA_MAK_DIR)
	
