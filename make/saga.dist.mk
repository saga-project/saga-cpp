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

ifndef SAGA_UPLOAD_HOST
	UPLOAD = no
endif
ifndef SAGA_UPLOAD_USER
	UPLOAD = no
endif
ifndef SAGA_UPLOAD_PATH
	UPLOAD = no
endif

ifndef SAGA_RPM_ARCH
 SAGA_RPM_ARCH=$(SAGA_ARCH)
endif
ifndef SAGA_DEB_ARCH
 SAGA_DEB_ARCH=$(SAGA_ARCH)
endif

HAVE_PACKAGE=$(shell test -e PACKAGE && grep "VERSION:" PACKAGE >/dev/null && echo yes)

ifeq "$(HAVE_PACKAGE)" "yes"
 PACKAGE = $(shell $(GREP) '^PACKAGE:' PACKAGE | $(SED) -e 's/^PACKAGE: *//')
 VERSION = $(shell $(GREP) '^VERSION:' PACKAGE | $(SED) -e 's/^VERSION: *//')
 
 ifneq "$(PACKAGE)" ""
  STAGE            = $(PACKAGE)-$(VERSION)
  SRCDIST          = $(PACKAGE)-$(VERSION).tgz
  SRCLATE          = $(PACKAGE)-latest.tgz
  BINDIST          = $(PACKAGE)-bin-$(VERSION).tgz
  BINLATE          = $(PACKAGE)-bin-latest.tgz
  DEBFILE          = $(PACKAGE)-$(VERSION).deb

  RPMFILES         = $(PACKAGE)-$(VERSION).src.rpm
  RPMFILES        += $(PACKAGE)-$(VERSION).$(SAGA_ARCH).rpm

  SAGA_DEB_CONTROL = $(PACKAGE).deb.ctrl
  SAGA_RPM_CONTROL = $(PACKAGE).rpm.ctrl
 endif
endif

.PHONY: dist srcdist bindist upload srcupload binupload

dist:: srcdist bindist

srcdist:: $(SRCDIST)
$(SRCDIST): PACKAGE
	+@$(SAGA_SCRIPT_ROOT)/saga-mksrcdist.pl
	
bindist:: $(BINDIST)
$(BINDIST): $(SRCDIST)
	+@$(SAGA_SCRIPT_ROOT)/saga-mkbindist.pl

upload:: srcupload binupload

srcupload:: srcdist
ifeq "$(UPLOAD)" "no"
	@$(ECHO) "'make upload' requires SAGA_UPLOAD_HOST/USER/PATH to be set"
else
	@scp $(SRCDIST) $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST):$(SAGA_UPLOAD_PATH)/$(SRCDIST)
	@ssh $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST) rm -f $(SAGA_UPLOAD_PATH)/$(SRCLATE)
	@ssh $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST) "cd $(SAGA_UPLOAD_PATH)/ && ln -s $(SRCDIST) $(SRCLATE)"
endif

binupload:: bindist
ifeq "$(UPLOAD)" "no"
	@$(ECHO) "'make upload' requires SAGA_UPLOAD_HOST/USER/PATH to be set"
else
	@scp $(BINDIST) $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST):$(SAGA_UPLOAD_PATH)/$(BINDIST)
	@ssh $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST) rm -f $(SAGA_UPLOAD_PATH)/$(BINLATE)
	@ssh $(SAGA_UPLOAD_USER)@$(SAGA_UPLOAD_HOST) "cd $(SAGA_UPLOAD_PATH)/ && ln -s $(BINDIST) $(BINLATE)"
endif

.PHONY: rpm
rpm:: $(SRCDIST)
	@tar zxf $(SRCDIST)
	@cp $(SRCDIST) $(PACKAGE)-$(VERSION)/
	@cd $(PACKAGE)-$(VERSION)/ && ./configure
	@make -C $(PACKAGE)-$(VERSION)/ rpm_package
	@find  $(PACKAGE)-$(VERSION)/rpmbuild -name \*.rpm -exec mv {} $(SAGA_PWD) \;
	@rm -rf $(PACKAGE)-$(VERSION)/


.PHONY: rpm_package
rpm_package::
ifdef RPMBUILD
	@$(RM)       -rf rpmbuild
	@$(MKDIR)    -p  rpmbuild/BUILD
	@$(MKDIR)    -p  rpmbuild/RPMS
	@$(MKDIR)    -p  rpmbuild/SOURCES
	@$(MKDIR)    -p  rpmbuild/SPECS
	@$(MKDIR)    -p  rpmbuild/SRPMS 
	@$(CP)       -f  $(SRCDIST) rpmbuild/SOURCES/
	@$(RPMBUILD) --define "_topdir $(SAGA_PWD)/rpmbuild/" -ba "$(PACKAGE).rpm.ctrl"
else
	$(error "no rpmbuild utility found - not a debian system?")
endif

.PHONY: deb
deb:: $(SRCDIST)
	@tar zxf $(SRCDIST)
	@cp $(SRCDIST) $(PACKAGE)-$(VERSION)/
	@cd $(PACKAGE)-$(VERSION)/ && ./configure --prefix=/usr/
	@make -C $(PACKAGE)-$(VERSION)/ deb_package
	@mv  $(PACKAGE)-$(VERSION)/*.deb $(SAGA_PWD)
	@rm -rf $(PACKAGE)-$(VERSION)/

.PHONY: deb_package
deb_package:: stage
ifdef DPKG
	@$(ECHO) "      creating $(DEBFILE)"
	@test -d $(SAGA_PWD)/stage/DEBIAN || mkdir $(SAGA_PWD)/stage/DEBIAN
	@cp $(SAGA_DEB_CONTROL) $(SAGA_PWD)/stage/DEBIAN/control
	@export SIZE=`du -bs $(SAGA_PWD)/stage/ | cut -f 1` && sed -i $(SAGA_PWD)/stage/DEBIAN/control -e "s/###SIZE###/$$SIZE/g"
	@$(DPKG) -D73 -b $(SAGA_PWD)/stage/ $(DEBFILE)
else
	$(error "no dpkg utility found - not a debian system?")
endif

.PHONY: stage
stage::
	@$(ECHO) "      staging install for packaging"
	-@mkdir $(SAGA_PWD)/stage 
	@$(MAKE) DESTDIR=$(SAGA_PWD)/stage/ install


install::
ifeq "$(HAVE_PACKAGE)" "yes"
	@$(ECHO) "      install PACKAGE file"
	@$(INSTALL) -f PACKAGE $(SAGA_DAT_ROOT)/PACKAGE.$(PACKAGE)
endif

distclean::
ifeq "$(HAVE_PACKAGE)" "yes"
	@$(RM) $(SRCDIST) $(BINDIST) $(DEBFILE) $(RPMFILES) $(SAGA_DEB_CONTROL) $(SAGA_RPM_CONTROL)
	@$(RM) -r $(SAGA_PWD)/stage/ $(STAGE)/
endif



endif # guard

