# Makefile for the alanguide by Mike Arnoud written in 2006 and converted to AsciiDOC

all: alanguide.pdf

alanguide.pdf: alanguide.adoc

%.pdf : %.adoc
	asciidoctor-pdf $^
