VERSION	= `ls [0-9]*_[0-9]*_[0-9]*.* | tail -1 | sed s/\\\\.//g | sed s/_//g`

all: version
	rm *.tgz *.zip
	tar zcvf alanlib$(VERSION).tgz *.i ReadMe
	zip alanlib$(VERSION).zip *.i ReadMe
	cp *.zip *.tgz ../../www/download

version: version.sh
	-rm [0-9]*_[0-9]*_[0-9]*
	sh version.sh

newVersion:
	echo "cat > `ls [0-9]*_[0-9]*_[0-9]*.* ` <<EOF" > version.sh
	echo "AlanLib" >> version.sh
	echo "Alan Standard Library" >> version.sh
	echo "EOF" >> version.sh
	cvs commit -m 'New Version' version.sh
