# Main common targets: clean build unit test
# package must be defined in platform dependent makefiles

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

.PHONY: clean
clean:
	@echo "***************** Clean *****************"
	-cd interpreter; $(MAKE) clean
	-cd compiler; $(MAKE) clean
	-cd converter; $(MAKE) clean
	-cd library; $(MAKE) clean

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
	-cd interpreter; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd compiler; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd library; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd converter; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test

