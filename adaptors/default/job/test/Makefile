#
#   Copyright (c) 2008 João Abecasis
#   Copyright (c) 2009 Andre Merzky <andre@merzky.net>
#
#   Distributed under the Boost Software License, Version 1.0. 
#   (See accompanying file LICENSE or copy at 
#   http://www.boost.org/LICENSE_1_0.txt)
#

SAGA_ROOT               = ../../../../

include $(SAGA_ROOT)/make/saga.config.c.mk

ifndef BC
 $(warning job tests require bc to be installed)
else
 SAGA_TEST_SRC           = $(wildcard *.cpp)
 SAGA_TEST_RUN           = ./fork_job_run.sh
endif

SAGA_DONT_INSTALL       = yes
SAGA_TEST_CACHE         = yes

include $(SAGA_ROOT)/make/saga.test.mk

