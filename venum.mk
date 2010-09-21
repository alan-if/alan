###################################################################
#
# Version number file generation
#
alan.version.c: $(VERSIONSRCS)
	cd ..; venum alan time
	cp ../alan.version.c .

alan.version.h : ../alan.version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h:
	cd ..; venum alan time
