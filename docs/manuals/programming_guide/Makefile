#  Copyright (c) 2010 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../../

include $(SAGA_ROOT)/make/saga.mk

install::
	@if test -f tex/saga-programming-guide.pdf ; \
	$(MV) tex pdf; \
	then $(ECHO) "      installing programming guide (pdf)"; \
	$(INSTALL) pdf/saga-programming-guide.pdf	$(SAGA_DAT_ROOT)/doc/; \
	$(MV) pdf tex; \
	else $(ECHO) "      cannot find programming guide (pdf) - call 'make doc' to generate it"; fi
	
	@if test -d html ; \
	then $(ECHO) "      installing programming guide (html)"; \
	$(INSTALL) -d  html/ 	$(SAGA_DAT_ROOT)/doc/saga-programming-guide; \
	else $(ECHO) "      cannot find programming guide (html) - call 'make doc' to generate it"; fi

.PHONY: doc
doc:
	@$(MAKE) -C tex doc

clean::
	@$(MAKE) -C tex clean

distclean::
	@$(RM) -rf ./html/ 
	@$(MAKE) -C tex distclean
