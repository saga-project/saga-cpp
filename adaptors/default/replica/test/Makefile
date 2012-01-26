#
#   Copyright (c) 2008 João Abecasis
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

SAGA_ROOT               = ../../../../

include $(SAGA_ROOT)/make/saga.util.mk

ifeq "x$(SAGA_HAVE_SQLITE3)" "xyes"
 SAGA_TEST_SRC           = $(wildcard *.cpp)
 SAGA_TEST_RUN           = ./run_adaptor_tests.sh

 SAGA_DONT_INSTALL       = yes
 SAGA_TEST_CACHE         = no
else
 ifdef SAGA_IS_CHECKING
  $(warning replica tests disabled - no sqlite3 detected)
 endif
endif


include $(SAGA_ROOT)/make/saga.test.mk

