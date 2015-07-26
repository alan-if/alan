# Main common targets: clean build unit test
# package must be defined in platform dependent makefiles

ifneq ($(EMACS),)
JREGROUTPUT = -noansi
else
UNITOUTPUT ?= -c
endif


BUILD_NUMBER_FILE = $(wildcard BUILD_NUMBER)
BUILD_FILE_CONTENT := $(shell cat $(BUILD_NUMBER_FILE))
ifeq ($(BUILD_NUMBER_FILE),)
  BUILD:=
  BUILDNUMBER:=
else
  BUILD:= -$(BUILD_FILE_CONTENT)
  BUILDNUMBER:= $(BUILD_FILE_CONTENT)
endif

# Create version and build numbers
VERSION = `cat LASTRELEASE`

.PHONY: clean
clean:
	@echo "***************** Clean *****************"
	-cd compiler; $(MAKE) clean
	-cd interpreter; $(MAKE) clean
	-cd converter; $(MAKE) clean
	-cd library; $(MAKE) clean

.PHONY: build
build:
	@echo "***************** Build *****************"
	cd compiler; $(MAKE) build
	cd interpreter; $(MAKE) build
	cd converter; $(MAKE) build
	cd library; $(MAKE) build

.PHONY: unit
unit:
	@echo "***************** Unit *****************"
	-cd compiler; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit
	-cd interpreter; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit
	-cd converter; $(MAKE) UNITOUTPUT="$(UNITOUTPUT)" unit

.PHONY: test
test:
ifeq ($(OS), Linux)
	@touch regression/isochars.suspended regression/unknownWordMessage.suspended
endif
	@echo "***************** Test *****************"
	-cd compiler; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd interpreter; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd library; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
	-cd converter; $(MAKE) JREGROUTPUT=$(JREGROUTPUT) test
