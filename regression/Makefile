# Makefile for the regression directory
#

all:
	regression


archives: CASES regression.dos
	lha c regression.lha *.alan *.input *.expected regression regression.amiga CASES
	zip -k regression.zip *.alan *.input *.expected regression regression.dos

CASES: *.alan
	ls -1 *.alan | sed -e s/.alan// > CASES

regression.dos: *.alan Makefile
	-rm regression.dos
	for f in *.alan ; do \
	    case=`basename $${f} .alan | awk '{print substr($$1, 1, 8)}'` ; \
	    echo alan $${case} >> regression.dos ; \
	    echo "arun $${case} <$${case}.inp >$${case}.out" >> regression.dos ; \
	done

vms:
	-rm regression.com
	for f in *.alan ; do \
	    case=`basename $${f} .alan` ; \
	    echo $$ @`basename $${f} .alan`/output=`basename $${f} .alan`.output >> regression.com ; \
	    echo $$ alan $${f} > `basename $${f} .alan`.com ; \
	    echo $$ arun `basename $${f} .alan` >> `basename $${f} .alan`.com ; \
	    cat `basename $${f} .alan`.input >> `basename $${f} .alan`.com ; \
	    echo $$ diff/output=$${case}.diff $${case}.output $${case}.expected >> regression.com ; \
	done
	echo $$ pur >> regression.com
	echo $$ dir/size *.diff >> regression.com
	/usr/5bin/echo "thoni\ncd alan/regression\nprompt\nmput *.com *.alan *.expected\n" | ftp gollum
	echo Rlogin and run regression tests!
	rlogin gollum
	rm *.com

mac:
	for f in *.alan ; do \
	    case=`basename $${f} .alan` ; \
	    ramp iso2mac $${f} $(HOME)/mac/Alan/Regression/$${f} ; \
	done
	@echo Run each test manually!
	