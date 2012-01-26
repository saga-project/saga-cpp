
#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_DELAYED
       SAGA_MK_DELAYED = yes

# subdirs_delayed should only be traversed if all other 
# targets are done.
.PHONY: delayed
.PHONY: subdirs_delayed
.PHONY: $(SAGA_SUBDIRS_DELAYED)

delayed::         delayed_subdirs
delayed_subdirs:: $(SAGA_SUBDIRS_DELAYED)

$(SAGA_SUBDIRS_DELAYED)::
	@$(ECHO) " -=>  $(SAGA_REL_PWD)$@/"
	@$(MAKE) SAGA_DEEP=1 -C $@ $(MAKECMDGOALS)
	@$(ECHO) " <=-  $(SAGA_REL_PWD)$@/"

all::            delayed
check::          delayed
configure::      delayed
depend::         delayed
deps::           delayed
forceinstall::   delayed
install::        delayed
makefiles::      delayed
apidoc::         delayed

error::          delayed
warning::        delayed

clean::          delayed
confclean::      delayed
depclean::       delayed
distclean::      delayed
libclean::       delayed

endif # guard

