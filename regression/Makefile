# Makefile for the regression directory
#

all:
	regression


archive:
	lha c regression.lha regression *.alan *.input *.ok

vms:
	-rm regression.com
	for f in *.alan ; do \
	    case=`basename $${f} .alan` ; \
	    echo $$ @`basename $${f} .alan`/output=`basename $${f} .alan`.output >> regression.com ; \
	    echo $$ alan $${f} > `basename $${f} .alan`.com ; \
	    echo $$ arun `basename $${f} .alan` >> `basename $${f} .alan`.com ; \
	    cat `basename $${f} .alan`.input >> `basename $${f} .alan`.com ; \
	    echo $$ diff/output=$${case}.diff $${case}.output $${case}.ok >> regression.com ; \
	done
	echo $$ pur >> regression.com
	echo $$ dir/size *.diff >> regression.com
	/usr/5bin/echo "thoni\ncd alan/regression\nprompt\nmput *.com *.alan *.ok\n" | ftp gollum
	rm *.com

