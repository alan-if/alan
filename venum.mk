###################################################################
#
# Version number file generation
#
# We create NEXTRELEASE to be able to generate version marking on
# non-Windows platforms where 'venum' isn't available

NEXTRELEASEFORMAT='$$v.$$r{s$$s|}$$c'

alan.version.c: $(VERSIONSRCS)
	cd ..; venum alan time
	cd ..; venum alan -print $(NEXTRELEASEFORMAT) > NEXTRELEASE
	cp ../alan.version.c .

alan.version.h : ../alan.version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h:
	cd ..; venum alan time

v:
	@echo NEXTRELEASEFORMAT=$(NEXTRELEASEFORMAT)
	@which venum
	if [ -f "alan.version" ] ; then \
		venum alan -print $(NEXTRELEASEFORMAT) ; \
	elif [ -f "../alan.version" ] ; then \
		cd ..; venum alan -print $(NEXTRELEASEFORMAT) ; \
	fi
	@echo BUILDNUMBER=$(BUILDNUMBER)
	@echo BUILDVERSION=$(BUILDVERSION)
	@echo BUILDNAME=$(BUILDNAME)
	@echo VERSION=$(VERSION)
