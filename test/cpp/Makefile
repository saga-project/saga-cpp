#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../

include $(SAGA_ROOT)/make/saga.util.mk

ifneq "$(SAGA_HAVE_PACKAGE_FILESYSTEM)" "yes"

  ifdef SAGA_IS_CLEANING
    SAGA_SUBDIRS = unit_tests 
  else
    $(warning "Skipping c++ unit tests, require filesystem package and adaptor")
  endif

  include $(SAGA_ROOT)/make/saga.nothing.mk

nothing::
	@$(ECHO) "      ======================================================"
	@$(ECHO) "      c++ unit tests disabled: no file package / adaptor "
	@$(ECHO) "      ======================================================"


else # have filesystem package

SAGA_SUBDIRS   = unit_tests 

ifeq "$(SAGA_HAVE_PACKAGE_JOB)" "yes"
  SAGA_SUBDIRS  += test_helper misc_tests
endif

include $(SAGA_ROOT)/make/saga.mk

.PHONY: tests
tests:
	  @$(MAKE) -C v2 tests

# dir dependencies
unit_tests: test_helper

endif

