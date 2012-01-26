#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_MK_INC_WARNING = yes

.PHONY: warning
warning::
ifdef SAGA_MAKE_WARNING
	$(warning  -- $(SAGA_MAKE_WARNING))
endif

