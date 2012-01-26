#
#   Copyright (c) 2008 João Abecasis
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

SAGA_ROOT               = ../../../

include $(SAGA_ROOT)/make/saga.util.mk

SAGA_SUBDIRS            = interactive

SAGA_TEST_SRC           = $(wildcard *.cpp)
#SAGA_TEST_RUN           = ./ssh_localhost.sh.run

ifdef SAGA_IS_CHECKING
 ifneq "x$(SAGA_HAVE_SQLIT3)" "xyes"
  $(warning "serialization tests disabled - no sqlite3 detected")
  SAGA_TEST_SRC         := $(filter-out test_session_serialization.cpp,$(SAGA_TEST_SRC))
 endif
endif

SAGA_DONT_INSTALL       = yes

include $(SAGA_ROOT)/make/saga.test.mk

distclean::
	@$(RM) thread_safety_test.src
	@$(RM) thread_safety_test.tgt
	@$(RM) session_test.db

