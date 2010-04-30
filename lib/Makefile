# Alan standard library Makefile
#

VERSION = `venum alanlib -print \\$$v_\\$$r_\\$$c\\$$s`

all: build

build: version
	@echo Building Alan Standard Library version $(VERSION)
	@-rm *.tgz *.zip *.tar.Z
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe index.txt
	@zip alanlib$(VERSION).zip *.i ReadMe index.txt

.PHONY: version

build: alanlib

alanlib:
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe index.txt

version:
	venum alanlib time -output imp

alanlibversion:
	venum alanlib time -output imp
	echo version = \"$(VERSION)\"

newVersion:
	cvs commit -m 'New Version' alanlib.version

test:
#	cd testing ; ../../bin/regr
	java -jar ../bin/jregr.jar -dir testing -bin ../../bin