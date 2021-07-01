# Makefile for alan compiler parser, scanner and lister
#
# This makefile is to ensure that all generated sources are
# up-to-date. It will generate parser, scanner and lister files only
# if ToolMaker (tmk, pmk, smk and lmk) are available. Which is tested
# by checking the variable TMHOME. If it's not available files are not
# generated. Instead the existing generated file will be used.
#
# NOTE that if the TMHOME variable is set, the executables are assumed
# to be available in the PATH.
#
# So any source distribution should include the resulting files and
# this Makefile will then print warnings but it should build anyway.
#
# REMEMBER: You have to set both the path to include the ToolMaker
# bin directory and the TMHOME environment variable to point there!

# TMHOME	= $(HOME)/Utveckling/ToolMaker
TMLIB	= $(TMHOME)/lib/ansi-c


# Make all LMK output files dependent on lmList.c
lmList.h: lmList.c

# Is TMHOME set?
ifneq ($(TMHOME),)
lmList.c: alan.lmk alan.tmk $(TMLIB)/List.imp $(TMLIB)/Common.imp
	lmk alan
else
lmList.c: alan.lmk alan.tmk
	@echo "WARNING! ToolMaker (lmk) not available, not re-generating, using current lmList.c"
endif


# Make all PMK output files dependent on pmParse.c
pmParse.h pmPaSema.c pmErr.c pmErr.h alan.voc alan.pml: pmParse.c

# Is TMHOME set?
ifneq ($(TMHOME),)
pmParse.c: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp $(TMLIB)/Common.imp
	pmk alan
	sed -f prod.sed alan.pml > alan.prod
else
pmParse.c: alan.pmk alan.tmk
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

# Is TMHOME set?
ifneq ($(TMHOME),)
# TODO: now with only ISO and UTF we don't need all this, remove it and be content with what smk generates...
smScan.c : alan.smk alan.tmk alan.voc $(TMLIB)/Scan.imp $(TMLIB)/Common.imp
	smk alan
else
smScan.c : alan.smk alan.tmk alan.voc
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
