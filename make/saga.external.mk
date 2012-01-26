#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make


ifndef SAGA_MK_INC_ADAPTOR
       SAGA_MK_INC_ADAPTOR = yes

###########################################################
#
# make configuration
#
-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need either SAGA_ROOT or SAGA_LOCATION)
endif


###########################################################
# 
# make configuration for external dirs
# 

# include the rquired make includes
ifndef SAGA_CONFDIR
  SAGA_CONFDIR = ../config
endif


ifdef SAGA_ROOT



 include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk


 SAGA_LITE_TYPE  = EXTERNAL
 SAGA_LITE_NAME  = $(SAGA_EXTERNAL_NAME)
 
 include $(SAGA_ROOT)/make/saga.mklite.mk

 SAGA_LITE_OBJ   = $(call saga_abs, $(shell $(FIND) $(SAGA_PWD) -name \*.o))

endif


endif # guard

