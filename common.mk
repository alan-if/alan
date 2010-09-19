###################################################################
#
# Version number file generation
#
alan.version.c: ../alan.version.c $(VERSIONSRCS)
	cd ..; venum alan time
	cp ../alan.version.c .

alan.version.h : ../alan.version.h
	cp ../alan.version.h .

version.h : ../version.h
	cp ../version.h .

../alan.version.c ../alan.version.h ../version.h:
	cd ..; venum alan time

../alan.version:
	cd ..; cvs update alan.version


###################################################################
# Automatically generate dependency rules 
%.d : %.c 
	$(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<" 

# -MF  write the generated dependency rule to a file 
# -MG  assume missing headers will be generated and don't stop with an error 
# -MM  generate dependency rule for prerequisite, skipping system headers 
# -MP  add phony target for each header to prevent errors when header is missing 
# -MT  add a target to the generated dependency 
