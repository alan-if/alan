# Makefile for alan compiler parser, scanner and lister
#
# This makefile is to ensure that all generated sources are
# up-to-date. It will generate parser, scanner and lister files only
# if ToolMaker (tmk, pmk, smk and lmk) are available.  Any source
# distribution should include the resulting files and this Makefile
# will then print warnings but it should build anyway.
#
# REMEMBER: You have to set both the path to include the ToolMaker
# bin directory and the TMHOME environment variable to point there!

TMHOME	= $(HOME)/Utveckling/ToolMaker
TMLIB	= $(TMHOME)/lib/ansi-c


# Make all LMK output files dependent on lmList.c
lmList.h: lmList.c

ifneq ($(shell which lmk 2>/dev/null), )
lmList.c: alan.lmk alan.tmk $(TMLIB)/List.imp $(TMLIB)/Common.imp
	lmk alan
else
lmList.c: alan.lmk alan.tmk $(TMLIB)/List.imp $(TMLIB)/Common.imp
	@echo "WARNING! ToolMaker (lmk) not available, not re-generating, using current lmList.c"
endif


# Make all PMK output files dependent on pmParse.c
pmParse.h pmPaSema.c pmErr.c pmErr.h alan.voc alan.pml: pmParse.c

ifneq ($(shell which pmk 2>/dev/null), )
pmParse.c: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp $(TMLIB)/Common.imp
	pmk alan
	sed -f prod.sed alan.pml > alan.prod
else
pmParse.c: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp $(TMLIB)/Common.imp
	@echo "WARNING! ToolMaker (pmk) not available, not re-generating, using current pmParse.c, pmPaSema.c pmErr.c"
endif

# Here we try to create CoCo and ANTLR grammars from the ToolMaker output
# in case we need to port or there is a editor that might need it for
# syntax highlighting or something...
alan.atg : coco.sed coco.header alan.prod
	cp coco.header alan.atg
	sed -f coco.sed alan.prod >> alan.atg

alan.g : antlr.sed antlr.header alan.prod
	cp antlr.header alan.g
	sed -f antlr.sed alan.prod >> alan.g

######################################################################
#
# Scanner - complex scripting to create a scanner that can use different
#           character sets
#

# Make all SMK output files dependent on smScanx.c
smScSema.c smScan.h: smScanx.c

ifneq ($(shell which smk 2>/dev/null), )
smScanx.c : alan.smk alan.tmk alan.voc $(TMLIB)/Scan.imp $(TMLIB)/Common.imp
	smk alan -generate tables
	imp $(IMPQ) alan.smt
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
	smk -set PC alan -generate tables
	imp $(IMPQ) alan.smt
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
	cat smScan.dos >> smScanx.c
	cat smScan.tbl >> smScanx.c
	cat smScan.tail >> smScanx.c
	dos2unix smScanx.c
	dos2unix smScSema.c
else
smScanx.c : alan.smk alan.tmk alan.voc $(TMLIB)/Scan.imp $(TMLIB)/Common.imp
	@echo "WARNING! ToolMaker (smk) not available, not re-generating, using current smScan.c and smScSema.c"
endif


######################################################################
#
# Debugging
#
.PHONY: x
x :
	@echo TMLIB=$(TMLIB)
	@echo TMHOME=$(TMHOME)
