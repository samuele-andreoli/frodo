### Makefile ###

## Configuration ##
# Project name, make sure your main tex file is $(NAME).tex
NAME = master_thesis
# Tex command
TEX = latex
BIBTEX = bibtex
# Spellcheck command
SPELLCHECK = aspell check

## Subdirs ##
STYDIR = styles
SRCDIR = src

## File lists ##
SPELLFILES = ${NAME}.bak src/*.bak
DISTFILES = ${NAME}.aux ${NAME}.out  ${NAME}.bbl ${NAME}.blg ${NAME}.dvi ${NAME}.idx ${NAME}.log ${NAME}.ps ${NAME}.toc *~
CLEANFILES = ${NAME}.pdf

## Build rules ##
.PHONY: all

all: ${NAME}.pdf 

$(NAME).pdf: ${NAME}.tex ${NAME}.bib
	$(TEX) ${NAME}
	$(BIBTEX) ${NAME}
	$(TEX) ${NAME}
	$(TEX) ${NAME}
	dvips -Ppdf -G0 $(NAME).dvi
	ps2pdf $(NAME).ps

## Clean-up rules ##
.PHONY: clean distclean spellclean

clean: distclean spellclean
	-rm -f ${CLEANFILES}

distclean:
	-rm -f ${DISTFILES}

spellclean:
	-rm -f ${SPELLFILES}

## Spelling checks rules ##
.PHONY:spell,spell_src

spell: spell_src
	@$(SPELLCHECK) ${NAME}.tex

spell_src:
	@for file in $(shell ls ${SRCDIR} | grep .tex);\
	do\
		$(SPELLCHECK) src/$${file};\
	done
