#VERSION	= `ls [0-9]*_[0-9]*_[0-9]*.* | tail -1 | sed s/\\\\.//g | sed s/_//g`
VERSION = `grep alanlib_versionString alanlib.version.imp | sed -e s/^\\.*\\(\\"// -e s/\\"\\)$$// | sed -e s/\\\\./_/g |sed -e s/\\ //g`

all: version
	@echo Building Alan Standard Library version $(VERSION)
	@-rm *.tgz *.zip *.tar.Z
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe index.txt
	@wzzip alanlib$(VERSION).zip *.i ReadMe index.txt

.PHONY: version
.PHONY: ../alan.version
version: ../alan.version
	cp ../alan.version alanlib.version
	venum alanlib time -output imp

alanlibversion:
	venum alanlib time -output imp
	echo version = \"$(VERSION)\"

newVersion:
	cvs commit -m 'New Version' alanlib.version
