###################################################################
#
# Version number file generation
#
# This should only be included in subdirectories with VERSIONSRCS set
# to a list of files that should trigger a new timestamp.
#
# If venum is not available will try to do something reasonable.
#
# It also creates NEXTRELEASE to be able to generate version marking
# on platforms where 'venum' isn't available

NEXTRELEASEFORMAT='$$v.$$r{s$$s|}$$c'
VERSION = `cat ../NEXTRELEASE`

alan.version.c: ../alan.version.c
	cp ../alan.version.c .

alan.version.h : ../alan.version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .


# Disable implicit rule for files without known extensions from .c file
# Make considers alan.version to be one of those obviously
%.version :

../alan.version.c ../alan.version : $(VERSIONSRCS)
ifneq ($(shell which venum 2>/dev/null), )
	# Use venum to generate timestamps
	cd ..; venum alan time
	cd ..; venum alan -print $(NEXTRELEASEFORMAT) > NEXTRELEASE
	cp ../alan.version.c .
else
	@echo "WARNING! Venum is not available, using current timestamps"
	cp ../alan.version.c .
endif

# For testing:
venum:
	@echo NEXTRELEASEFORMAT=$(NEXTRELEASEFORMAT)
	@echo BUILDNUMBER=$(BUILDNUMBER)
	@echo BUILDVERSION=$(BUILDVERSION)
	@echo BUILDNAME=$(BUILDNAME)
	@echo VERSION=$(VERSION)
	@echo venum=`which venum`
	@if [ -f "alan.version" ] ; then \
		venum alan -print $(NEXTRELEASEFORMAT) ; \
	elif [ -f "../alan.version" ] ; then \
		cd ..; venum alan -print $(NEXTRELEASEFORMAT) ; \
	fi
