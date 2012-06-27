#  Copyright (c) 2005-2007 Andre Merzky (andre@merzky.net)
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# vim: ft=make

ifndef SAGA_MK_INC_INC
       SAGA_MK_INC_INC = yes

###########################################################
#
# make configuration
#
-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_MAKE_INCLUDE_ROOT
  $(error need either SAGA_ROOT or SAGA_LOCATION)
endif

include $(SAGA_MAKE_INCLUDE_ROOT)/saga.nothing.mk
include $(SAGA_MAKE_INCLUDE_ROOT)/saga.dist.mk


# include the required make includes
ifndef SAGA_CONFDIR
  SAGA_CONFDIR = ../config
endif

TMP_REQS   = $(patsubst   package_%,package.%,$(SAGA_REQUIREMENTS))
TMP_PACKAGE_REQS   = $(filter     package.%,$(TMP_REQS))
TMP_EXTERNAL_REQS  = $(filter-out package.%,$(TMP_REQS))

TMP_MAKE_INCLUDES  = $(foreach preq,$(TMP_PACKAGE_REQS),$(SAGA_ROOT)/make/saga.$(preq).i.mk)
TMP_MAKE_INCLUDES += $(foreach ereq,$(TMP_EXTERNAL_REQS),$(SAGA_CONFDIR)/saga.config.$(ereq).c.mk)

ifneq "x$(TMP_MAKE_INCLUDES)" "x"
  ifndef SAGA_IS_CLEANING
    include $(TMP_MAKE_INCLUDES)
  endif
endif


ifndef SAGA_PACKAGE
 SAGA_PACKAGE = saga
endif


###########################################################
#
# saga related compiler flags etc
#
SAGA_CPPINCS    += -I$(SAGA_INCLUDE_ROOT)
SAGA_MKDEP       = $(SAGA_CPP) -MM
SAGA_CPPINCS_ALL = $(SAGA_CPPINCS_EXT) $(SAGA_CPPINCS) $(SAGA_CPPINCS_SYS)

# $(warning -------------------------------------- )
# $(warning SAGA_CXXFLAGS    : $(SAGA_CXXFLAGS)    )
# $(warning SAGA_CPPDEFS     : $(SAGA_CPPDEFS)     )
# $(warning SAGA_CPPINCS_EXT : $(SAGA_CPPINCS_EXT) )
# $(warning SAGA_CPPINCS     : $(SAGA_CPPINCS)     )
# $(warning SAGA_CPPINCS_SYS : $(SAGA_CPPINCS_SYS) )
# $(warning -------------------------------------- )
# $(warning SAGA_CPPINCS_ALL : $(SAGA_CPPINCS_ALL) )
# $(warning -------------------------------------- )
#
###########################################################

###########################################################
#
# headers get installed under SAGA_LOCATION/include/, 
# but beneath that directory the original directory 
# hierarchy is maintained.  For example, 
#                          saga/impl/engine.hpp 
# goes into 
#   $SAGA_LOCATION/include/saga/impl/engine.hpp
#
# REL = PWD - ROOT
#

# fix for old make versions which don't know realpath
SAGA_TMP_PWD    = $(realpath $(SAGA_PWD))/
SAGA_TMP_ROOT   = $(realpath $(SAGA_ROOT))

ifeq "$(SAGA_TMP_PWD)$(SAGA_TMP_ROOT)" "/"
 SAGA_TMP_PWD  := $(shell cd $(SAGA_PWD)  && $(PWD))/
 SAGA_TMP_ROOT := $(shell cd $(SAGA_ROOT) && $(PWD))
endif

ifeq "$(SAGA_TMP_PWD)$(SAGA_TMP_ROOT)" ""
 # SAGA_TMP_PWD   = $(subst //,/,$(SAGA_PWD)/)
 # SAGA_TMP_ROOT  = $(subst //,/,$(SAGA_ROOT)/)
endif

ifneq "$(SAGA_TMP_ROOT)" ""
 SAGA_REL_PWD    := $(shell $(ECHO) $(SAGA_TMP_PWD) \
                          | $(SED) -e 's|$(SAGA_TMP_ROOT)/||g' \
                          | $(SED) -e 's|/$$||g' \
                     )
else
 SAGA_REL_PWD    := $(shell $(ECHO) $(SAGA_TMP_PWD) \
                          | $(SED) -e 's|/$$||g' \
                     )
endif

# FIXME: relation to above??
ifeq "$(realpath $(SAGA_PWD))" ""
  SAGA_REAL_PWD  = $(SAGA_PWD)
  SAGA_REAL_ROOT = $(SAGA_ROOT)
else
  SAGA_REAL_PWD  = $(realpath $(SAGA_PWD))
  SAGA_REAL_ROOT = $(realpath $(SAGA_ROOT))
endif


ifneq "$(SAGA_REL_PWD)" ""
  SAGA_REL_PWD := $(SAGA_REL_PWD)/
endif

ifndef SAGA_HDR_DIR
  SAGA_HDR_DIR := $(shell $(ECHO) $(SAGA_REL_PWD) \
								 			 | $(SED) -e 's/^external\/boost\//boost\//')
endif
#
###########################################################

###########################################################
#
# main targets: all, install, subdirs
#
ifndef SAGA_BIN_OBJ
 SAGA_BIN_OBJ  = $(SAGA_BIN:%=%.o)
endif

ifndef SAGA_LIB_OBJ
 SAGA_LIB_OBJ  = $(SAGA_OBJ)
endif

SAGA_ALL_OBJ  += $(sort $(SAGA_LIB_OBJ)  $(SAGA_OBJ)  $(SAGA_ADD_LIB_OBJ) $(SAGA_ADD_BIN_OBJ) $(SAGA_BIN_OBJ))
SAGA_ALL_OBJC += $(sort $(SAGA_LIB_OBJC) $(SAGA_OBJC) $(SAGA_ADD_LIB_OBJC) $(SAGA_ADD_BIN_OBJC))

SAGA_ALL_PRE  += $(SAGA_ALL_OBJ:%.o=%.ii) 
SAGA_ALL_PREC += $(SAGA_ALL_OBJC:%.o=%.i) 


ifndef SAGA_LEXT
  SAGA_LEXT = so
endif

ifndef SAGA_AEXT
  SAGA_AEXT = a
endif


# build both dynamic and static libs
ifdef SAGA_LIB
  ifndef SAGA_IS_CLEANING
    SAGA_LIBNAME = lib$(SAGA_LIB).$(SAGA_LEXT)
    SAGA_ARCNAME = lib$(SAGA_LIB).$(SAGA_AEXT)
  else
    SAGA_LIBNAME_CLEAN = lib$(SAGA_LIB).$(SAGA_LEXT)
    SAGA_ARCNAME_CLEAN = lib$(SAGA_LIB).$(SAGA_AEXT)
  endif
endif

# sometimes, we need _only_ the static lib
ifdef SAGA_ARC
  ifndef SAGA_IS_CLEANING
    SAGA_ARCNAME = lib$(SAGA_ARC).$(SAGA_AEXT)
  else
    SAGA_ARCNAME_CLEAN = lib$(SAGA_ARC).$(SAGA_AEXT)
  endif
endif

# sometimes, we need _only_ the dynamic lib
ifdef SAGA_DYLIB
  ifndef SAGA_IS_CLEANING
    SAGA_LIBNAME = lib$(SAGA_DYLIB).$(SAGA_LEXT)
  endif
endif

# decide if we use static or dynamic libs (default: dynamic libs)
ifeq "$(SAGA_USE_STATIC_LIBS)" "yes"
  # we add dynamic libs below, for those libs which are only available as such
  SAGA_LDFLAGS_EXT := $(SAGA_S_LIBS) $(SAGA_LDFLAGS_EXT)
  SAGA_LDFLAGS_EXT := $(SAGA_S_LIBS) $(SAGA_LDFLAGS_EXT)
endif

SAGA_USE_LDFLAGS_BIN  += $(SAGA_LDFLAGS) $(SAGA_LDFLAGS_EXT) $(SAGA_LDFLAGS_SYS)
SAGA_USE_LDFLAGS_LIB  += $(SAGA_LDFLAGS) $(SAGA_LDFLAGS_EXT) $(SAGA_LDFLAGS_SYS)


# external applications using our make system do *not* need CXXFLAGS_INT
ifdef SAGA_ROOT
  SAGA_CXXFLAGS += $(SAGA_CXXFLAGS_INT)
endif


# for cleaning targets, make sure to clean make and config dirs last, so as to
# not bar make paths due to missing prerequsites etc
ifdef SAGA_IS_CLEANING
  SAGA_SUBDIRS_ORIG   := $(SAGA_SUBDIRS)
  SAGA_SUBDIRS_BEFORE :=
  SAGA_SUBDIRS        := $(filter-out config make, $(SAGA_SUBDIRS_ORIG))
  SAGA_SUBDIRS_AFTER  := $(filter     make config, $(SAGA_SUBDIRS_ORIG))
 ifdef SAGA_MAKE_DEBUG
  $(warning $(SAGA_PWD) - before  : $(SAGA_SUBDIRS_BEFORE))
  $(warning $(SAGA_PWD) - subdirs : $(SAGA_SUBDIRS))
  $(warning $(SAGA_PWD) - after   : $(SAGA_SUBDIRS_AFTER))
 endif
endif


SAGA_CONF_IN  = $(wildcard configure.in)
SAGA_CONF     = $(wildcard configure)
SAGA_META    += $(wildcard CHANGES)
SAGA_META    += $(wildcard LICENSE)
SAGA_META    += $(wildcard config.summary)
SAGA_M4      += $(wildcard *.m4)
SAGA_M4_DEP  += $(wildcard $(SAGA_ROOT)/config/*.m4) 
SAGA_M4_DEP  += $(wildcard config/*.m4) 

SAGA_CONF_IN := $(strip $(SAGA_CONF_IN))
SAGA_CONF    := $(strip $(SAGA_CONF))
SAGA_META    := $(strip $(SAGA_META))
SAGA_M4      := $(strip $(SAGA_M4))
SAGA_M4_DEP  := $(strip $(SAGA_M4_DEP))



#########################################################################################################
#
# make targets
#
.DEFAULT_GOAL := all
DEFAULT_GOAL::   all


all:: error makefiles depend subdirs $(SAGA_ALL_OBJ) $(SAGA_ALL_OBJC) $(SAGA_LIBNAME) $(SAGA_ARCNAME) 

# SHS: SHellScript
all:: $(SAGA_BIN) $(SAGA_SHS) $(SAGA_MAK) 

apidoc::            subdirs
compliance_check::  subdirs
check::         all subdirs
checkclean::        subdirs

# subdirs_before and subdirs_after allow to order subdir traversal.
.PHONY: $(SAGA_SUBDIRS_BEFORE) $(SAGA_SUBDIRS) $(SAGA_SUBDIRS_AFTER) $(SAGA_SUBDIRS_DELAYED)
.PHONY:        subdirs_before         subdirs         subdirs_after         subdirs_delayed

subdirs_after: subdirs
subdirs:       subdirs_before

subdirs_before:  $(SAGA_SUBDIRS_BEFORE)
subdirs:         $(SAGA_SUBDIRS) $(SAGA_SUBDIRS_BEFORE) $(SAGA_SUBDIRS_AFTER)
subdirs_after:   $(SAGA_SUBDIRS_AFTER)
subdirs_delayed: $(SAGA_SUBDIRS_DELAYED)

# $(warning subdirs_before:  $(SAGA_SUBDIRS_BEFORE))
# $(warning subdirs:         $(SAGA_SUBDIRS) -- $(SAGA_SUBDIRS_BEFORE) $(SAGA_SUBDIRS_AFTER))
# $(warning subdirs_after:   $(SAGA_SUBDIRS_AFTER))
# $(warning subdirs_delayed: $(SAGA_SUBDIRS_DELAYED))

$(SAGA_SUBDIRS_DELAYED): subdirs $(SAGA_LIBNAME) $(SAGA_ARCNAME) $(SAGA_BIN)
	@$(ECHO) " ==>  $(SAGA_REL_PWD)$@/"
	@$(MAKE) SAGA_DEEP=1 -C $@ $(MAKECMDGOALS)
	@$(ECHO) " <==  $(SAGA_REL_PWD)$@/"



# the following 'target' simply finishes the previous set of targets.  Stupid
# make parser...
no_target: ;

ifdef SAGA_IS_CLEANING
  $(SAGA_SUBDIRS):       $(SAGA_SUBDIRS_BEFORE)
  $(SAGA_SUBDIRS_AFTER): $(SAGA_SUBDIRS)
else
	$(SAGA_SUBDIRS_BEFORE): $(SAGA_SUBDIRS_AFTER)
endif

$(SAGA_SUBDIRS_BEFORE) $(SAGA_SUBDIRS) $(SAGA_SUBDIRS_AFTER):
	@$(ECHO) " -->  $(SAGA_REL_PWD)$@/"
	@$(MAKE) SAGA_DEEP=1 -C $@ $(MAKECMDGOALS)
	@$(ECHO) " <--  $(SAGA_REL_PWD)$@/"


configure:: $(SAGA_CONF_IN) $(SAGA_M4) $(SAGA_M4_DEP)
ifneq "$(SAGA_CONF_IN)" ""
	@$(ECHO) "      creating   configure"
 ifdef SAGA_ROOT
	@autoconf -B $(SAGA_PWD)/config -B $(SAGA_REAL_ROOT)/config
 else
	@autoconf -B $(SAGA_PWD)/config -B $(SAGA_LOCATION)/share/saga/config
 endif
	@$(RM) -r autom4te.cache
endif

.PHONY: makefiles
makefiles:: makeconfigs

makeconfigs:: $(SAGA_MAK_C) subdirs
ifdef SAGA_MAK_C
	@$(CP) $(SAGA_MAK_C) $(SAGA_ROOT)/make
endif

$(SAGA_MAK_C):
	@$(ECHO) ""
	@$(ECHO) " ================================= "
	@$(ECHO) "  you need to run configure first  "
	@$(ECHO) " ================================= "
	@$(ECHO) ""
	@$(FALSE)

ifdef SAGA_VERSION
  INSTVER = -v $(SAGA_VERSION)
endif

# allow makefiles ot overwrite SAGA_VERSION for specific libs
ifdef SAGA_MODULE_VERSION
  INSTVER = -v $(SAGA_MODULE_VERSION)
else
 ifdef SAGA_LIB_VERSION
	# DEPRECATED!
	$(warning SAGA_LIB_VERSION is DEPRECATED - use SAGA_MODULE_VERSION)
  INSTVER = -v $(SAGA_LIB_VERSION)
 endif
endif

install:: all
 ifndef SAGA_DONT_INSTALL
  ifneq "$(SAGA_HDR)" ""
	 @$(ECHO) "      installing   headers"
	 @$(INSTALL)     $(SAGA_HDR)     $(SAGA_HDR_ROOT)/$(SAGA_HDR_DIR)
  endif
  ifneq "$(SAGA_LIBNAME)" ""
	 @$(ECHO) "      installing   lib" 
	 @$(INSTALL)     $(INSTVER) $(SAGA_LIBNAME) $(SAGA_LIB_ROOT)/$(SAGA_LIB_DIR)
  endif
  ifneq "$(SAGA_ARCNAME)" ""
	 @$(ECHO) "      installing   lib (static)" 
	 @$(INSTALL)     $(INSTVER) $(SAGA_ARCNAME) $(SAGA_LIB_ROOT)/$(SAGA_LIB_DIR)
  endif
  ifneq "$(SAGA_BIN)" ""
	 @$(ECHO) "      installing   bin" 
	 @$(INSTALL)     $(SAGA_BIN)     $(SAGA_BIN_ROOT)/$(SAGA_BIN_DIR)
   ifneq "$(word 2,$(SAGA_BIN))x" "x"
    @$(ECHO) "      WARNING: Cannot install aliases for multible bins."
   else
	  @$(foreach VAR,$(SAGA_BIN_ALIASES),$(INSTALL) -a $(VAR) $(SAGA_BIN) $(SAGA_BIN_ROOT)/$(SAGA_BIN_DIR);)
   endif
  endif
  ifneq "$(SAGA_SHS)" ""
	 @$(ECHO) "      installing   scripts"
	 @$(INSTALL)     $(SAGA_SHS)     $(SAGA_SHS_ROOT)/$(SAGA_SHS_DIR)
  endif
  ifneq "$(SAGA_INI)" ""
	 @$(ECHO) "      installing   inis"
	 @$(INSTALL)     $(SAGA_INI)     $(SAGA_INI_ROOT)
  endif
  ifneq "$(SAGA_META)" ""
	 @$(ECHO) "      installing   meta data"
	 @for f in $(SAGA_META); do \
	   $(INSTALL)   -f $$f $(SAGA_DAT_ROOT)/$$f.$(SAGA_MODULE_NAME); \
	   done
  endif
  ifneq "$(SAGA_MAK)" ""
	 @$(ECHO) "      installing   makefiles"
	 @$(INSTALL)     $(SAGA_MAK)     $(SAGA_MAK_ROOT)
  endif        
  ifneq "$(SAGA_MAK_D)" ""
	 @$(ECHO) "      installing   make includes"
	 @$(INSTALL)     $(SAGA_MAK_D)   $(SAGA_MAK_ROOT)
  endif
  ifneq "$(SAGA_M4)" ""
	 @$(ECHO) "      installing   configure macros"
	 @$(INSTALL)     $(SAGA_M4) $(SAGA_M4_ROOT)
  endif
  ifneq "$(SAGA_MAN1)" ""
	 @$(ECHO) "      installing   man(1) pages"
	 @$(INSTALL)  -c $(SAGA_MAN1) $(SAGA_MAN_ROOT)/man1/
  endif
  ifneq "$(SAGA_MAN2)" ""
	 @$(ECHO) "      installing   man(2) pages"
	 @$(INSTALL)  -c $(SAGA_MAN2) $(SAGA_MAN_ROOT)/man2/
  endif
  ifneq "$(SAGA_MAN3)" ""
	 @$(ECHO) "      installing   man(3) pages"
	 @$(INSTALL)  -c $(SAGA_MAN3) $(SAGA_MAN_ROOT)/man3/
  endif
  ifneq "$(SAGA_MAN4)" ""
	 @$(ECHO) "      installing   man(4) pages"
	 @$(INSTALL)  -c $(SAGA_MAN4) $(SAGA_MAN_ROOT)/man4/
  endif
  ifneq "$(SAGA_MAN5)" ""
	 @$(ECHO) "      installing   man(5) pages"
	 @$(INSTALL)  -c $(SAGA_MAN5) $(SAGA_MAN_ROOT)/man5/
  endif
  ifneq "$(SAGA_MAN6)" ""
	 @$(ECHO) "      installing   man(6) pages"
	 @$(INSTALL)  -c $(SAGA_MAN6) $(SAGA_MAN_ROOT)/man6/
  endif
  ifneq "$(SAGA_MAN7)" ""
	 @$(ECHO) "      installing   man(7) pages"
	 @$(INSTALL)  -c $(SAGA_MAN7) $(SAGA_MAN_ROOT)/man7/
  endif
  ifneq "$(SAGA_MAN8)" ""
	 @$(ECHO) "      installing   man(8) pages"
	 @$(INSTALL)  -c $(SAGA_MAN8) $(SAGA_MAN_ROOT)/man8/
  endif
  ifneq "$(SAGA_MAN9)" ""
	 @$(ECHO) "      installing   man(9) pages"
	 @$(INSTALL)  -c $(SAGA_MAN9) $(SAGA_MAN_ROOT)/man9/
  endif
 endif # SAGA_DONT_INSTALL
#
###########################################################

###########################################################
#
# Handle dependencies for all cpp sources, if we did run configure before
#
ifeq "$(SAGA_CONFIGURED)" "yes"
 SAGA_DEPEND  = $(addprefix .depend/,$(SAGA_SRC:.cpp=.d))
 SAGA_DEPEND += $(addprefix .depend/,$(SAGA_CSRC:.c=.d))
endif

# include dependencies if we have any
ifneq "$(SAGA_DEPEND)" ""
 ifeq "$(filter depend clean distclean binclean libclean depclean confclean configure makefiles, $(MAKECMDGOALS))" ""
  -include $(SAGA_DEPEND)
 endif
endif

# 'make depend' re-creates dependencies if needed
.PHONY: depend 
depend:: error subdirs $(SAGA_DEPEND)

# dependencies depend on source file, and get 
# re-created automatically
.depend/%.d:: %.cpp
	@$(ECHO) "      check        $<"
	@$(MKDIR)    .depend
	@$(SAGA_MKDEP) $(SAGA_CPPFLAGS) $(SAGA_CXXFLAGS) $(SAGA_CPPINCS_ALL) $< > $@
                                                  
.depend/%.d:: %.c                                 
	@$(ECHO) "      check        $<"                
	@$(MKDIR)    .depend                            
	@$(SAGA_MKDEP) $(SAGA_CPPFLAGS) $(SAGA_CCFLAGS) $(SAGA_CPPINCS_ALL) $< > $@
#
###########################################################

###########################################################
#
# main rule set
#

ifeq "$(SAGA_DEBUG_MAKE)" "yes"
$(SAGA_ALL_PRE):  %.ii : %.cpp
	@$(ECHO) "      precompiling $@"
	@$(SAGA_CCACHE) $(SAGA_CXX)  $(SAGA_CPPFLAGS) $(SAGA_CPPDEFS) $(SAGA_CPPINCS_ALL) -c -o $@ $<
                                                                   
$(SAGA_ALL_PREC): %.i : %.c                                        
	@$(ECHO) "      precompiling $@"                                 
	@$(SAGA_CCACHE) $(SAGA_CC)   $(SAGA_CPPFLAGS) $(SAGA_CPPDEFS) $(SAGA_CPPINCS_SYS) -c -o $@ $<

$(SAGA_ALL_OBJ):  %.o : %.ii
	@$(ECHO) "      compiling    $@"
	@$(SAGA_CCACHE) $(SAGA_CXX)  $(SAGA_CXXFLAGS) -c -o $@ $<

$(SAGA_ALL_OBJC): %.o : %.i
	@$(ECHO) "      compiling    $@"
	@$(SAGA_CCACHE) $(SAGA_CC)   $(SAGA_CCFLAGS) -c -o $@ $<
else
$(SAGA_ALL_OBJ):  %.o : %.cpp
	@$(ECHO) "      compiling    $@"
	@$(SAGA_CCACHE) $(SAGA_CXX)  $(SAGA_CXXFLAGS) $(SAGA_CPPDEFS) $(SAGA_CPPINCS_ALL) -c -o $@ $<
                                                                       
$(SAGA_ALL_OBJC): %.o : %.c                                            
	@$(ECHO) "      compiling    $@"                                     
	@$(SAGA_CCACHE) $(SAGA_CC)   $(SAGA_CCFLAGS)  $(SAGA_CPPDEFS) $(SAGA_CPPINCS_ALL) -c -o $@ $<
endif

$(SAGA_BIN):      % : %.o $(SAGA_ADD_BIN_OBJ) $(SAGA_ADD_BIN_OBJC)
	@$(ECHO) "      binlinking   $@"
	@$(SAGA_LD_BIN) -o $@ $^ $(SAGA_USE_LDFLAGS_BIN)

$(SAGA_ARCNAME):  $(SAGA_LIB_OBJ) $(SAGA_ADD_LIB_OBJ) $(SAGA_LIB_OBJC) $(SAGA_ADD_LIB_OBJC)
ifneq "$(SAGA_BUILD_LIB_STATIC)" "yes"
	@$(ECHO) "      liblinking   $@ - static libs disabled ($(SAGA_BUILD_LIB_STATIC))"
else
	@$(ECHO) "      liblinking   $@ (static)"
	@$(SAGA_AR)     $(SAGA_ARFLAGS) $@ $^
endif

ifdef SAGA_USE_SONAME
 ifdef SAGA_MODULE_VERSION
  saga_soname_flag = -Wl,-soname,$1.$(SAGA_MODULE_VERSION)
 else
  saga_soname_flag = -Wl,-soname,$1.$(SAGA_VERSION_MAJOR).$(SAGA_VERSION_MINOR)
 endif
else
 saga_soname_flag = 
endif

$(SAGA_LIBNAME):  $(SAGA_LIB_OBJ) $(SAGA_ADD_LIB_OBJ) $(SAGA_LIB_OBJC) $(SAGA_ADD_LIB_OBJC)
ifneq "$(SAGA_BUILD_LIB_DYNAMIC)" "yes"
	@$(ECHO) "      liblinking   $@ - dynamic libs disabled ($(SAGA_BUILD_LIB_DYNAMIC))"
else
	@$(ECHO) "      liblinking   $@"
	@$(SAGA_LD_LIB) -o $@ $^ $(SAGA_USE_LDFLAGS_LIB) $(call saga_soname_flag,$(SAGA_LIBNAME))
endif
#
###########################################################

###########################################################
#
# cleanup rules
#
SAGA_CLEAN_TGT     += $(wildcard autom4te.cache)
SAGA_CLEAN_TGT     += $(wildcard confdefs.h)
SAGA_CLEAN_TGT     += $(SAGA_LIBNAME_CLEAN)
SAGA_CLEAN_TGT     += $(SAGA_ARCNAME_CLEAN)
SAGA_CLEAN_TGT     += $(SAGA_LIBNAME)
SAGA_CLEAN_TGT     += $(SAGA_ARCNAME)
SAGA_CLEAN_TGT     += $(SAGA_BIN) 
SAGA_CLEAN_TGT     += $(SAGA_ALL_OBJ)
SAGA_CLEAN_TGT     += $(SAGA_ALL_OBJC)
SAGA_CLEAN_TGT     += $(SAGA_ALL_PRE)
SAGA_CLEAN_TGT     += $(SAGA_ALL_PREC)

SAGA_DISTCLEAN_TGT += $(wildcard config.log)
SAGA_DISTCLEAN_TGT += $(wildcard config.summary)
SAGA_DISTCLEAN_TGT += $(wildcard config.status)
SAGA_DISTCLEAN_TGT += $(SAGA_MAK)
SAGA_DISTCLEAN_TGT += $(SAGA_MAK_I)
SAGA_DISTCLEAN_TGT += $(SAGA_MAK_C)

SAGA_CONFCLEAN_TGT += $(SAGA_CONF)

SAGA_DEPCLEAN_TGT  += $(wildcard .depend)

clean:: subdirs
ifneq "$(SAGA_CLEAN_TGT)" ""
	@$(ECHO) "      clean"
	@$(RM) -r  $(SAGA_CLEAN_TGT)
endif
	
libclean:: subdirs
	@$(ECHO) "      clean libs"
ifneq "$(SAGA_LIBNAME_CLEAN)" ""
	@$(RM)          $(SAGA_LIBNAME_CLEAN)
endif
ifneq "$(SAGA_ARCNAME_CLEAN)" ""
	@$(RM)          $(SAGA_ARCNAME_CLEAN)
endif

binclean:: subdirs
	@$(ECHO) "      clean bins"
ifneq "$(SAGA_BIN)" ""
	@$(RM)          $(SAGA_BIN)
endif

distclean:: depclean clean
ifneq "$(SAGA_DISTCLEAN_TGT)" ""
	@$(ECHO) "      clean all"
	@$(RM) -r  $(SAGA_DISTCLEAN_TGT)
endif

confclean:: distclean
ifneq "$(SAGA_CONFCLEAN_TGT)" ""
	@$(ECHO) "      clean confs"
	@$(RM) -r  $(SAGA_CONFCLEAN_TGT)
endif

depclean:: subdirs
ifneq "$(SAGA_DEPCLEAN_TGT)" ""
	@$(ECHO) "      clean deps"
	-@$(RM) -r $(SAGA_DEPCLEAN_TGT)
endif
#
###########################################################


endif # guard

