# Makefile for alan compiler on Windows using Cygwin32
#

CPP	= gcc -MM -ansi

TMLIB	= $(TMHOME)/lib/ansi-c

COMPILER= -DCOMPILER=\"[CygWinB20]\"

CFLAGS = -g -Wall -funsigned-char -D__win__ $(INCLUDE) -mno-cygwin $(COMPILER)

LDFLAGS = -g -mno-cygwin

EXTRAS = \
	alan.tmk \
	alan.lmk \
	alan.smk \
	alan.pmk \
	Makefile

TMSRCS = \
	pmParse.c pmPaSema.c \
	pmErr.c \
	smScanx.c smScSema.c\
	lmList.c

TMOBJS = ${TMSRCS:.c=.o}

OBJECTS = \
	alan.o \
	alan.version.o \
	main.o \
	options.o \
	act.o \
	adv.o \
	alt.o \
	atr.o \
	chk.o \
	cnt.o \
	dump.o \
	encode.o \
	emit.o \
	elm.o \
	evt.o \
	exp.o \
	ext.o \
	lim.o \
	loc.o \
	lst.o \
	msg.o \
	nam.o \
	obj.o \
	opt.o \
	res.o \
	rul.o \
	sco.o \
	scr.o \
	srcp.o \
	stm.o \
	stp.o \
	stx.o \
	str.o \
	sym.o \
	syn.o \
	vrb.o \
	whr.o \
	wht.o \
	wrd.o \
	timing.o \
	spa.o \
	sysdep.o \
	$(TMOBJS)

all alan alan.exe : tm $(OBJECTS) Makefile
	gcc -o alan $(OBJECTS) $(LDFLAGS)
	cp alan.exe ../bin

spa.o: spa.c spa.h
	gcc $(CFLAGS) -DSTDIONONCONST -c spa.c -o spa.o 

smScanx.o: smScanx.c

alan.version.c : ../alan.version.c alan.version.h
	cp ../alan.version.c .

alan.version.h : ../alan.version.h version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

src: tm smScanx.c sysdep.h sysdep.c acode.h version.h

fromsrc: $(OBJECTS) version.o
	$(LD) -o alan version.o $(OBJECTS) $(LDFLAGS)
	-[ ! -d $(ALANDIR)/bin/`arch` ] && mkdir $(ALANDIR)/bin/`arch`
	cp alan $(ALANDIR)/bin/`arch`

tm: .pmkstamp .smkstamp .lmkstamp

.lmkstamp : alan.lmk alan.tmk $(TMLIB)/List.imp
	lmk $(LMKQ) alan
	touch .lmkstamp

.pmkstamp: alan.pmk alan.tmk $(TMLIB)/Parse.imp $(TMLIB)/Err.imp
	pmk $(PMKQ) alan
	sed -e "1,/P R O D/d" -e "/Summary/,$$ d" alan.pml > alan.prod
	touch .pmkstamp

.smkstamp : alan.smk alan.tmk alan.voc $(TMLIB)/Scan.imp Makefile
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

acode.h: ../interpreter/acode.h
	-ln -s ../interpreter/acode.h

sysdep.h: ../interpreter/sysdep.h
	-ln -s ../interpreter/sysdep.h

sysdep.c: ../interpreter/sysdep.c
	-ln -s ../interpreter/sysdep.c


#- Other machines

savetm:
	@if test -f alan.tmk.orig; then \
	    echo "There is already an original!"; \
	    exit 1; \
	fi
	@cp -p alan.tmk alan.tmk.orig

restoretm:
	@cp -p alan.tmk.orig alan.tmk
	@-rm alan.tmk.orig

NONANSI:
	make savetm
	sed -e s/ansi-c/c/ alan.tmk.orig > alan.tmk
	make src
	make restoretm

DOSSUBS = -e s/alanCommon/alanComm/
MSDOS:
	make src	
	for f in `grep -l alanCommon *.[ch]` ; do \
		sed $(DOSSUBS) $$f > `toupper $$f` ; \
	done
	ramp iso2pc msg.c MSG.C
	ramp iso2pc sysdep.c SYSDEP.C

mac:
	@echo "Can't do that yet!"

amiga:
	make src
	lha u compiler.lha *.c *.h Makefile Makefile.amiga dependencies.mk

pc:
	make MSDOS
	zip -k -u compiler.zip *.[ch] Makefile.dos makefile.gpp
	zip -k -m compiler.zip *.[CH]

#--
CLEAN: clean
	-rm alanCommon.h pmParse* pmPaSema* pmErr* smScan* smScSema* lmList* alan.voc *.o

clean:
	-rm core *~ *.zoo *.bak *.lzh

#--
#FILETER = | sed -e "\,/usr/, d" -e "s,./,," | sort -u 
FILETER =
depend: dependencies Makefile.amiga Makefile.mgw makefile.gpp

dependencies:
	@echo "Updating dependencies.mk!"
	@echo > dependencies.mk
	@for f in *.c; \
	  do \
	  $(CPP) $(CFLAGS) $$f $(FILETER) >> dependencies.mk; \
	done

Makefile.amiga: dependencies.mk Makefile
	@echo Updating Makefile.amiga!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.amiga > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cat "#Makefile.tmp#" dependencies.mk > Makefile.amiga

makefile.gpp: dependencies.mk Makefile
	@echo Updating makefile.gpp!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' makefile.gpp > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cp "#Makefile.tmp#" makefile.gpp
	@sed $(DOSSUBS) -e s/\\.o/\\.obj/ dependencies.mk >> makefile.gpp

Makefile.mgw: dependencies.mk Makefile
	@echo Updating Makefile.mgw!
	@sed -e '/^#---<< GENERATED DEPENDENCIES FOLLOWS >>---$$/,$$ d' Makefile.mgw > "#Makefile.tmp#"
	@echo "#---<< GENERATED DEPENDENCIES FOLLOWS >>---" >> "#Makefile.tmp#"
	@cp "#Makefile.tmp#" Makefile.mgw
	@sed $(DOSSUBS) -e s/\\.o/\\.obj/ dependencies.mk >> Makefile.mgw

include dependencies.mk
