#
#  Makefile for yasa/doc
#

.PHONY: clean distclean

.SUFFIXES: .pdf .tex

# user targets
all: yasa

# create yasa document
yasa: yasa.pdf

# create monthly reports
berichte: bericht1.pdf bericht2.pdf


clean:
	-rm -f *.aux *.log *.pdf

distclean: clean
	-rm -f *~


.tex.pdf:
	pdflatex $< && pdflatex $<
