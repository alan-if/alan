###################################################################
#
# Version number file generation
#
# We create next release to be able to generate version marking on
# non-Windows platforms where 'venum' isn't available
alan.version.c: $(VERSIONSRCS)
	cd ..; venum alan time
	cd ..; venum alan -print '$$v_$$r{s$$s|}$$c' > NEXTRELEASE
	cp ../alan.version.c .

alan.version.h : ../alan.version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h:
	cd ..; venum alan time
