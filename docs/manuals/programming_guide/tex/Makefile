#  Copyright (c) 2010 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

SAGA_ROOT        = ../../../../

include $(SAGA_ROOT)/make/saga.mk

PDF = saga-programming-guide.pdf
SRC = $(wildcard *.tex)

doc:: $(PDF)

$(PDF): $(SRC)
ifeq "x$(PDFLATEX)" "x"
	@$(ECHO) "      cannot generate programming_guide pdf - pdflatex is not installed"
else
	@$(ECHO) "      generating programming_guide pdf"
	@$(MAKE)	    clean
	@$(PDFLATEX)    saga-programming-guide.tex
#	@$(BIBTEX)      saga-programming-guide.tex
	@$(PDFLATEX)    saga-programming-guide.tex
	@$(PDFLATEX)    saga-programming-guide.tex
endif

ifeq "x$(HTLATEX)" "x"
	@$(ECHO) "      cannot generate programming_guide html pages - htlatex is not installed"
else
	@$(ECHO) "      generating programming_guide html pages"
	@$(MAKE)		clean
	@$(MKDIR)       ./../html/
	@$(HTLATEX)     saga-programming-guide.tex "html,2,info"
	@$(MV)          *.html *.png *.css ./../html/
endif

clean::
	@$(RM) *.4tc *.4ct *.aux  *.log *.tmp *.out *.lg *.idv *.xref *.toc *.blg *.bbl

distclean::
	@$(MAKE)	clean
	@$(RM)    *.ps *.dvi

