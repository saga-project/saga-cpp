#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT    = ../

SAGA_MAK_C   = saga.config.c.mk
SAGA_INI     = saga.ini

SAGA_M4     += saga-config.cache install.sh config.guess config.sub

install::
	@$(TOUCH) saga-config.cache


include $(SAGA_ROOT)/make/saga.mk


distclean::
	@$(RM) saga.*.mk saga.config.sh saga.deb.ctrl
	@$(RM) saga-config.cache saga.ini saga.pc saga_boost.pc

install:: saga.pc
	@$(ECHO)  "      installing   saga package config"
	@$(INSTALL)  saga.pc          $(SAGA_LIB_ROOT)/pkgconfig/
	@$(INSTALL)  saga_boost.pc    $(SAGA_LIB_ROOT)/pkgconfig/
	@$(ECHO)  "      installing   configure tools"
	@$(INSTALL)  config.guess     $(SAGA_DAT_ROOT)/config/
	@$(INSTALL)  config.sub       $(SAGA_DAT_ROOT)/config/
	@$(INSTALL)  saga.config.sh   $(SAGA_DAT_ROOT)/config/

# overwrite an unwanted built-in rule which messes up 'make install'
%: %.sh
	@$(TRUE)

