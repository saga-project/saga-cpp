#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_MK_INC_ERROR

SAGA_MK_INC_ERROR = yes

.PHONY: error
error::
ifdef SAGA_MAKE_ERROR
	$(error $(SAGA_MAKE_ERROR))
endif

endif # guard

