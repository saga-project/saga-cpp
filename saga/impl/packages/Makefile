#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying 
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../../

SAGA_SUBDIRS     = namespace advert filesystem job replica rpc stream sd cpr isn

include $(SAGA_ROOT)/make/saga.mk

# package dependencies

advert:     namespace
filesystem: namespace
replica:    namespace
cpr:        namespace
cpr:        filesystem
cpr:        job

