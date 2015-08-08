# Main common targets: clean build unit test
# package must be defined in platform dependent makefiles

ifneq ($(EMACS),)
JREGROUTPUT = -noansi
else
UNITOUTPUT ?= -c
endif


# Build designations:
#   BUILD includes a dash, if no-empty, so can be used with $(VERSION)$(BUILD)
#   BUILDNUMBER is just the number
#   BUILDNAME is "Build"$(BUILDNUMBER), e.g. "Build1667"
BUILD_NUMBER_FILE = $(wildcard BUILD_NUMBER)
ifeq ($(BUILD_NUMBER_FILE),)
  BUILD=
  BUILDNUMBER=
  BUILDNAME=
else
  BUILD= -`cat $(BUILD_NUMBER_FILE)`
  BUILDNUMBER= `cat $(BUILD_NUMBER_FILE)`
  BUILDNAME= Build$(BUILDNUMBER)
endif

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

buildnumber:
	-wget -q 'ci.alanif.se:8080/job/Alan/api/xml?xpath=*/lastSuccessfulBuild/number' -O- | xpath -q -e 'number/text()' > BUILD_NUMBER

snapshot-check:
	@if [ ! -f BUILD_NUMBER ] ; then \
	  echo Should not upload a snapshot without BUILD_NUMBER, create it first ; \
	  exit 1 ; \
	fi
