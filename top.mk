ifneq ($(EMACS),)
JREGROUTPUT = -noansi
endif


RELEASE = `cat LASTRELEASE`
BUILD_NUMBER_FILE = $(wildcard BUILD_NUMBER)
ifeq ($(BUILD_NUMBER_FILE),)
  BUILD=
else
  BUILD= -`cat $(BUILD_NUMBER_FILE)`
endif

# Main common targets: clean build unit test
# package must be defined in platform dependent makefiles
.PHONY: clean
clean:
	@echo "***************** Clean *****************"
	-cd interpreter; make clean
	-cd compiler; make clean
	-cd converter; make clean
	-cd library; make clean

.PHONY: build
build:
	@echo "***************** Build *****************"
	-cd compiler; $(MAKE) build
	-cd interpreter; $(MAKE) build
	-cd converter; $(MAKE) build
	-cd library; $(MAKE) build

.PHONY: unit
unit:
	@echo "***************** Unit *****************"
	-cd compiler; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit
	-cd interpreter; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit
	-cd converter; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit

.PHONY: test
test:
	@echo "***************** Test *****************"
	@java -jar bin/jregr.jar -bin bin -dir regression $(JREGROUTPUT)
#	@java -jar bin/jregr.jar -bin bin -dir regression/versions/compiler $(JREGROUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir regression/versions/interpreter $(JREGROUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir regression/tracing $(JREGROUTPUT)
	@java -jar bin/jregr.jar -dir regression/restore $(JREGROUTPUT)		# Uses sh not the executables
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing $(JREGROUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing/positions $(JREGROUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir library/testing $(JREGROUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir converter/testing $(JREGROUTPUT)

