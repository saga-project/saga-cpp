#
#   Copyright (c) 2008 João Abecasis
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

ifndef SAGA_MK_INC_COMPLIANCE
       SAGA_MK_INC_COMPLIANCE = yes

-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MK_INC_UTIL
	$(error Could not find the SAGA make system)
endif

include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk


.PHONY: compliance_check
compliance_check:: 
	@(test -e INSTALL && echo "INSTALL... - found") || (echo "INSTALL... - missing" && $(FALSE))



endif # guard

