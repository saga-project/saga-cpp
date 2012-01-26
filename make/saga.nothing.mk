#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_NOTHING
       SAGA_MK_NOTHING = yes

.DEFAULT_GOAL := all
DEFAULT_GOAL::   all


.PHONY: nothing
nothing:: 
	@$(TRUE)

all::            nothing
check::          nothing
configure::      nothing
depend::         nothing
deps::           nothing
forceinstall::   nothing
install::        nothing
makefiles::      nothing
apidoc::         nothing

subdirs:         nothing
subdirs_after:   nothing
subdirs_before:  nothing

error::          nothing
warning::        nothing

clean::          nothing
confclean::      nothing
depclean::       nothing
distclean::      nothing
libclean::       nothing

.PHONY: error
.PHONY: check

endif # guard

