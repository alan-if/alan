# Makefile for the regression directory
#

all:
	steam test


archives: CASES regression.dos
	lha c regression.lha *.alan *.input *.expected regression regression.amiga CASES
	zip -k regression.zip *.alan *.input *.expected regression regression.dos

CASES: *.alan
	ls -1 *.alan | sed -e s/.alan// > CASES

dos:
	-@mkdir dosregr
	@for f in *.tst ; do \
	    base=`basename $${f} .tst` ; \
	    case=`basename $${f} .tst | awk '{print substr($$1, 1, 8)}'` ; \
	    echo -n Doing $${base} as $${case} ; \
	    sed -e "s;$${base};$${case};g" -e "s;input;inp;g" $$f > dosregr/$${case}.tst~ ; \
	    echo -n . ; \
	    ramp iso2pc dosregr/$${case}.tst~  dosregr/$${case}.bat ; \
	    echo -n . ; \
	    \rm  dosregr/$${case}.tst~ ; \
	    echo -n . ; \
	    if test -f $${base}.input ; then \
		ramp iso2pc $${base}.input dosregr/$${case}.inp ; \
		echo -n . ; \
	    fi ; \
	    if test -f $${base}.alan ; then \
		ramp iso2pc $${base}.alan dosregr/$${case}.ala ; \
		echo -n . ; \
	    fi ; \
	    ramp iso2pc $${base}.tst.expected dosregr/$${case}.exp ; \
	    echo . ; \
	done
	zip -k -m dosregr.zip dosregr/*

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
	