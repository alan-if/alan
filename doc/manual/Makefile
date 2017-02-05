# Location of XXXOffice for conversion from .odt to .pdf
OFFICEDIR = /cygdrive/c/Program\ Files\ \(x86\)/LibreOffice\ 5

all: manual.pdf conversion.pdf writing.pdf

manual.pdf: manual.odt

conversion.pdf: conversion.odt

writing.pdf: writing.odt

%.pdf : %.odt
	$(OFFICEDIR)/program/swriter --convert-to pdf:writer_pdf_Export --headless $^
