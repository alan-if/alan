VERSION	= `ls [0-9]*_[0-9]*_[0-9]*.* | tail -1 | sed s/\\\\.//g | sed s/_//g`

all: version
	@echo Building Alan Standard Library version $(VERSION)
	@-rm *.tgz *.zip *.tar.Z
	@tar zcvf alanlib$(VERSION).tar.Z *.i ReadMe
	@zip alanlib$(VERSION).zip *.i ReadMe
	@cp *.zip *.tar.Z ../../www/download
	@cp ReadMe ../../www/download/library.txt
	@venum +c
	@make newVersion

version: version.sh
	-rm [0-9]*_[0-9]*_[0-9]*
	sh version.sh

newVersion:
	echo "cat > `ls [0-9]*_[0-9]*_[0-9]*.* ` <<EOF" > version.sh
	echo "AlanLib" >> version.sh
	echo "Alan Standard Library" >> version.sh
	echo "EOF" >> version.sh
	cvs commit -m 'New Version' version.sh
