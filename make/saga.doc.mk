
-include $(SAGA_ROOT)/make/saga.util.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

ifndef SAGA_DOC
  SAGA_DOC = $(shell $(EGREP) -le '^ *::SAGA  *DOCUMENTATION  *FILE:: *$$' *)
endif
	SAGA_CLEAN_TGT     += $(SAGA_DOC:%=%.aux)
	SAGA_CLEAN_TGT     += $(SAGA_DOC:%=%.log)
	SAGA_CLEAN_TGT     += $(SAGA_DOC:%=%.out)
	SAGA_CLEAN_TGT     += $(SAGA_DOC:%=%.toc)

	SAGA_DISTCLEAN_TGT += $(SAGA_DOC:%=%.pdf)
	SAGA_DISTCLEAN_TGT += $(SAGA_DOC:%=%.tex)


doc:: $(SAGA_DOC)
 ifeq "$(PDFLATEX)" ""
	@$(ECHO) "Cannot create documentation w/o pdflatex"
 else
  ifneq "$(SAGA_DOC)" ""
	 @for doc in $(SAGA_DOC); do \
	     $(CAT) $(SAGA_MAKE_INCLUDE_ROOT)/misc/doc.adaptor.header.tex  >  $$doc.tex; \
	     $(CAT) $$doc                                 \
	       | $(EGREP) -v '::SAGA DOCUMENTATION FILE::' \
	       | $(SED) -e 's|:\(.*\):|\\section{\1}|g'                    >> $$doc.tex; \
	     $(CAT) $(SAGA_MAKE_INCLUDE_ROOT)/misc/doc.adaptor.footer.tex  >> $$doc.tex; \
	     $(ENV) TEXINPUTS=$(SAGA_MAKE_INCLUDE_ROOT)/misc:$(SAGA_PWD):$$TEXINPUTS $(PDFLATEX) $$doc.tex; \
	   done
  endif
 endif

