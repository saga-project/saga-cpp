#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../

SAGA_SHS        += saga-config
SAGA_SHS        += saga-install.sh
SAGA_SHS        += saga-install.pl
SAGA_SHS        += saga-mksrcdist.pl
SAGA_SHS        += saga-mkbindist.pl
# SAGA_SHS        += saga-profile.sh
# SAGA_SHS        += saga-run-test.sh
SAGA_SHS        += saga-run.sh

include $(SAGA_ROOT)/make/saga.application.mk

distclean::
	@$(RM) saga-profile.sh 
	@$(RM) saga-run-test.sh
	@$(RM) saga-run.sh
	@$(RM) saga-install.sh
	@$(RM) saga-install.pl
	@$(RM) saga-config

