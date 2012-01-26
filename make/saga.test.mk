#
#   Copyright (c) 2008 João Abecasis
#
#   Distributed under the Boost Software License, Version 1.0. (See accompanying
#   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#

ifndef SAGA_MK_INC_TEST
       SAGA_MK_INC_TEST = yes

-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MK_INC_UTIL
	$(error Could not find the SAGA make system)
endif

  SAGA_TEST_OBJ  += $(SAGA_TEST_SRC:%.cpp=%.o)
  SAGA_TEST_BIN  += $(SAGA_TEST_SRC:%.cpp=%)
  
  SAGA_TEST_OK   += $(SAGA_TEST_BIN:%=%.ok)
  SAGA_TEST_OK   += $(SAGA_TEST_RUN:%=%.ok)
  SAGA_TEST_NOK  += $(SAGA_TEST_BIN:%=%.nok)
  SAGA_TEST_NOK  += $(SAGA_TEST_RUN:%=%.nok)
  SAGA_TEST_OUT   = $(SAGA_TEST_OK:%.ok=%.out)
  SAGA_TEST_ERR   = $(SAGA_TEST_OK:%.ok=%.err)
  
ifdef SAGA_IS_CHECKING
  SAGA_OBJ       += $(SAGA_TEST_OBJ)
  SAGA_OBJ       += $(SAGA_ADD_TEST_BIN_OBJ) $(SAGA_ADD_TEST_BIN_OBJC)
  
  SAGA_BIN       += $(SAGA_TEST_BIN)
endif

ifdef SAGA_IS_CLEANING
  SAGA_CLEAN_TGT += $(SAGA_TEST_OBJ)
  SAGA_CLEAN_TGT += $(SAGA_TEST_BIN)
  SAGA_CLEAN_TGT += $(SAGA_TEST_OK)
  SAGA_CLEAN_TGT += $(SAGA_TEST_OUT)
  SAGA_CLEAN_TGT += $(SAGA_TEST_ERR)
endif

ifdef SAGA_IS_CLEANING
  SAGA_CHECKCLEAN_TGT += $(SAGA_TEST_NOK)
  SAGA_CHECKCLEAN_TGT += $(SAGA_TEST_OK)
  SAGA_CHECKCLEAN_TGT += $(SAGA_TEST_OUT)
  SAGA_CHECKCLEAN_TGT += $(SAGA_TEST_ERR)
endif

ifdef SAGA_ROOT
 include $(SAGA_ROOT)/make/saga.application.mk
else
 include $(SAGA_LOCATION)/share/saga/make/saga.application.mk
endif

# We need to run things directly from the source tree
ifneq "$(SAGA_LD_LIBRARY_PATH)" ""
 SAGA_LD_LIBRARY_PATH       += :
endif

ifeq "$(SAGA_OSTYPE)" "SAGA_APPLE"
 SAGA_LD_LIBRARY_PATH       += $(DYLD_LIBRARY_PATH)
else
 SAGA_LD_LIBRARY_PATH       += $(LD_LIBRARY_PATH)
endif

SAGA_LD_LIBRARY_PATH        += :$(SAGA_BOOST_LOCATION)/lib

ifdef SAGA_ROOT
 SAGA_LD_LIBRARY_PATH       += $(shell echo $(SAGA_LDFLAGS) | $(SED) -e 's/-l[^ ]*//g' | $(SED) -e 's/ *-L/:/g')
 SAGA_LD_LIBRARY_PATH       += :$(SAGA_ROOT)/saga/impl/engine
 
 PACKAGE_DIRS_TMP            = $(wildcard   $(SAGA_ROOT)/saga/impl/packages/*)
 PACKAGE_DIRS                = $(filter-out $(SAGA_ROOT)/saga/impl/packages/Makefile,$(PACKAGE_DIRS_TMP))
 SAGA_LD_LIBRARY_PATH       += $(foreach PACKAGE_DIR,$(PACKAGE_DIRS),:$(PACKAGE_DIR))
endif


ifneq "x$(SAGA_BOOST_HAVE_SYSTEM)" "xyes"
  SAGA_LD_LIBRARY_PATH     += :$(SAGA_ROOT)/external/boost/system/libs/system/src
endif

# remove spaces from SAGA_LD_LIBRARY_PATH
SAGA_LD_LIBRARY_PATH       := $(shell $(ECHO) "$(SAGA_LD_LIBRARY_PATH)" | $(SED) -e "s/ //g")


-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.test.env.mk


SAGA_ENVIRON               +=  SAGA_LOCATION=$(SAGA_LOCATION)
ifeq "$(SAGA_OSTYPE)" "SAGA_APPLE"
  SAGA_ENVIRON             +=  DYLD_LIBRARY_PATH=$(subst $(SAGA_SPACE):,:,$(SAGA_LD_LIBRARY_PATH))
else
  SAGA_ENVIRON             +=    LD_LIBRARY_PATH=$(subst $(SAGA_SPACE):,:,$(SAGA_LD_LIBRARY_PATH))
endif


.PHONY: check
check:: $(SAGA_TEST_OK)

ifdef SAGA_ROOT
RUN    = $(ENV) SAGA_ROOT=$(SAGA_ROOT) $(SAGA_SCRIPT_ROOT)/saga-run-test.sh
else
RUN    = $(ENV) $(SAGA_SCRIPT_ROOT)/saga-run-test.sh
endif

# alow to skip previously successful tests
ifeq "$(SAGA_TEST_CACHE)" "yes"
	RUN += -c
endif

.PHONY: $(SAGA_TEST_OK)
$(SAGA_TEST_OK):: subdirs
$(SAGA_TEST_OK):: %.ok : %
	@$(RUN) -e "$(SAGA_ENVIRON)" ./$< $(SAGA_TEST_ARGS)
ifdef SAGA_TEST_FAIL_ON_ERROR
	@$(TEST) -e $@ || $(FALSE)
endif

distclean:: checkclean

.PHONY: checkclean
checkclean:: subdirs
	@$(RM) $(SAGA_CHECKCLEAN_TGT)



endif # guard

