# Makefile for alan compiler
# This makefile is to ensure that all sources are up-to-date
# It will generate parser, scanner, lister and version files
# using tools only available on ThoNi's machines
# Any source distribution should be complete with respect to these
# files and this Makefile can be ignored.
#
# REMEMBER: You have to set both the Path to include the ToolMaker
# directory and the TMHOME environment variable to point there!


TMLIB	= $(TMHOME)/lib/ansi-c

EXTRAS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk

TMSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

all : tm smScanx.c sysdep.h sysdep.c acode.h version.h

tm: .pmkstamp .smkstamp .lmkstamp
	touch .tmstamp

.lmkstamp : alan.lmk alan.tmk $(TMLIB)/List.imp
	lmk $(LMKQ) alan
	touch .lmkstamp

.pmkstamp: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp
	pmk $(PMKQ) alan
	sed -e "1,/P R O D/d" -e "/Summary/,$$ d" alan.pml > alan.prod
	touch .pmkstamp

.smkstamp : alan.smk alan.tmk alan.voc $(TMLIB)/Scan.imp
	smk alan
	sed -e "1,/START of scanning tables/d" -e "/END of scanning tables/,$$ d" -e "/static UByte1 smMap/,/;/d" -e "/static UByte1 smDFAcolVal/,/;/d" -e "/static UByte1 smDFAerrCol/,/;/d" smScan.c > smScan.tbl
	echo "/* ISO scanner tables */" > smScan.iso.new
	echo "UByte1 smIsoMap[256]={" >> smScan.iso.new
	sed -e "1,/static UByte1 smMap/d" -e "/;/,$$ d" smScan.c >> smScan.iso.new
	echo ";" >> smScan.iso.new
	echo "" >> smScan.iso.new
	echo "UByte1 smIsoDFAcolVal[256]={" >> smScan.iso.new
	sed -e "1,/static UByte1 smDFAcolVal/d" -e "/;/,$$ d" smScan.c >> smScan.iso.new
	echo ";" >> smScan.iso.new
	echo "" >> smScan.iso.new
	echo "UByte1 smIsoDFAerrCol[256]={" >> smScan.iso.new
	sed -e "1,/static UByte1 smDFAerrCol/d" -e "/;/,$$ d" smScan.c >> smScan.iso.new
	echo ";" >> smScan.iso.new
	echo "" >> smScan.iso.new
	if test -f smScan.iso ; then \
		if cmp smScan.iso smScan.iso.new ; then \
			rm smScan.iso.new ; \
		else \
			mv smScan.iso.new smScan.iso ; \
		fi ; \
	else \
		mv smScan.iso.new smScan.iso ; \
	fi ;
	smk -set MAC alan
	echo "/* MAC scanner tables */" > smScan.mac.new
	echo "UByte1 smMacMap[256]={" >> smScan.mac.new
	sed -e "1,/static UByte1 smMap/d" -e "/;/,$$ d" smScan.c >> smScan.mac.new
	echo ";" >> smScan.mac.new
	echo "" >> smScan.mac.new
	echo "UByte1 smMacDFAcolVal[256]={" >> smScan.mac.new
	sed -e "1,/static UByte1 smDFAcolVal/d" -e "/;/,$$ d" smScan.c >> smScan.mac.new
	echo ";" >> smScan.mac.new
	echo "" >> smScan.mac.new
	echo "UByte1 smMacDFAerrCol[256]={" >> smScan.mac.new
	sed -e "1,/static UByte1 smDFAerrCol/d" -e "/;/,$$ d" smScan.c >> smScan.mac.new
	echo ";" >> smScan.mac.new
	echo "" >> smScan.mac.new
	if test -f smScan.mac ; then \
		if cmp smScan.mac smScan.mac.new ; then \
			rm smScan.mac.new ; \
		else \
			mv smScan.mac.new smScan.mac ; \
		fi ; \
	else \
		mv smScan.mac.new smScan.mac ; \
	fi ;
	smk -set PC alan
	echo "/* DOS scanner tables */" > smScan.dos.new
	echo "UByte1 smDosMap[256]={" >> smScan.dos.new
	sed -e "1,/static UByte1 smMap/d" -e "/;/,$$ d" smScan.c >> smScan.dos.new
	echo ";" >> smScan.dos.new
	echo "" >> smScan.dos.new
	echo "UByte1 smDosDFAcolVal[256]={" >> smScan.dos.new
	sed -e "1,/static UByte1 smDFAcolVal/d" -e "/;/,$$ d" smScan.c >> smScan.dos.new
	echo ";" >> smScan.dos.new
	echo "" >> smScan.dos.new
	echo "UByte1 smDosDFAerrCol[256]={" >> smScan.dos.new
	sed -e "1,/static UByte1 smDFAerrCol/d" -e "/;/,$$ d" smScan.c >> smScan.dos.new
	echo ";" >> smScan.dos.new
	echo "" >> smScan.dos.new
	if test -f smScan.dos ; then \
		if cmp smScan.dos smScan.dos.new ; then \
			rm smScan.dos.new ; \
		else \
			mv smScan.dos.new smScan.dos ; \
		fi ; \
	else \
		mv smScan.dos.new smScan.dos ; \
	fi
	sed -e "1,/START of scanning tables/w smScan.head" -e "/END of scanning tables/,$$ w smScan.tail" smScan.c > /dev/null
	cat smScan.head > smScanx.c
	echo "UByte1 *smMap;" >> smScanx.c
	echo "UByte1 *smDFAcolVal;" >> smScanx.c
	echo "UByte1 *smDFAerrCol;" >> smScanx.c
	echo "" >> smScanx.c
	cat smScan.iso >> smScanx.c
	cat smScan.mac >> smScanx.c
	cat smScan.dos >> smScanx.c
	cat smScan.tbl >> smScanx.c
	cat smScan.tail >> smScanx.c
	touch .smkstamp

smScanx.c : .smkstamp

pmParse.h pmParse.c pmPaSema.c pmErr.c alan.voc alan.pml:
	-rm .pmkstamp
	make .pmkstamp

smScan.h smScSema.c:
	-rm .smkstamp
	make .smkstamp

lmList.h lmList.c:
	-rm .lmkstamp
	make .lmkstamp

