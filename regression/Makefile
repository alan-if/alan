# Makefile for the regression directory
#

all:
	resteam


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
	    echo $$ @$${case}/output=$${case}.output >> regression.com ; \
	    echo $$ alan $${case} > $${case}.com ; \
	    echo $$ arun $${case} >> $${case}.com ; \
	    cat $${case}.input >> $${case}.com ; \
	    echo $$ diff/output=$${case}.diff $${case}.output $${case}.tst.expected >> regression.com ; \
	done
	echo $$ pur >> regression.com
	echo $$ dir/size *.diff >> regression.com
	/usr/5bin/echo "thoni\ncd alan/regression\nprompt\nmput *.com *.alan *.expected\n" | ftp gollum
	echo Rlogin and run regression tests!
	rlogin gollum
	rm *.com

mac:
	for f in *.tst ; do \
	    case=`basename $${f} .tst` ; \
	    ramp iso2mac $${case}.alan $(HOME)/mac/Alan/Regression/$${case}.alan ; \
	done
	@echo Run each test manually!
	