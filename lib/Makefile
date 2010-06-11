# Alan standard library Makefile
#

VERSION = `venum alanlib -print \\$$v_\\$$r_\\$$c\\$$s`

.PHONY: all
all: build

.PHONY: build
build: version
	@echo Building Alan Standard Library version $(VERSION)
	@-rm *.tgz *.zip *.tar.Z
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe index.txt
	@zip alanlib$(VERSION).zip *.i ReadMe index.txt

.PHONY: version

build: alanlib

.PHONY:
alanlib:
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe index.txt

.PHONY: version
version:
	venum alanlib time -output imp

.PHONY: alanlibversion
alanlibversion:
	venum alanlib time -output imp
	echo version = \"$(VERSION)\"

.PHONY: newVersion
newVersion:
	cvs commit -m 'New Version' alanlib.version

test:
#	cd testing ; ../../bin/regr
	java -jar ../bin/jregr.jar -dir testing -bin ../../bin