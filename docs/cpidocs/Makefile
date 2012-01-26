#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
#  Copyright (c) 2005-2011 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../
SAGA_VERSION     = $(shell grep 'VERSION:' $(SAGA_ROOT)/PACKAGE | cut -f 2 -d ' ')

DOXYGEN          = $(shell which doxygen)

include $(SAGA_ROOT)/make/saga.mk

cpidoc::
ifeq "x$(DOXYGEN)" "x"
	@$(ECHO) "      cannot generate Adaptor CPI docs - doxygen is not installed"
else
	@$(ECHO) "      generating Adaptor CPI docs"
	@if ! test -d html/ ; \
	then $(MKDIR) 	html ; fi
	@$(CP) 		 saga-cpi.png html
	@sed "s/###PROJECT_NUMBER###/$(SAGA_VERSION)/g" Doxyfile.in > Doxyfile
	@$(DOXYGEN)  Doxyfile 
endif

install::
	@if test -d html/ ; \
	then $(ECHO) "      installing Adaptor CPI documentation"; \
	$(MV) html cpp; \
	$(INSTALL) -d cpp  $(SAGA_DAT_ROOT)/doc/html/; \
	$(MV) cpp html; \
	else $(ECHO) "      cannot find Adaptor CPI documentation - run 'make cpidoc' to generate it"; fi


# FIXME: we should actually delete the svn traces
#	@$(FIND)              $(SAGA_LOCATION)/share/saga/doc/ -name .svn -exec rm -rf {} \;

distclean::
	@$(RM)	-rf	Doxyfile
