ifneq ($(EMACS),)
TESTOUTPUT = -noansi
endif


VERSION = `cat LASTRELEASE`
BUILD_NUMBER = $(wildcard BUILD_NUMBER)
ifeq ($(BUILD_NUMBER),)
  BUILD=
else
  BUILD= -$(BUILD_NUMBER)
endif

.PHONY: clean
clean:
	cd interpreter; make clean
	cd compiler; make clean

.PHONY: build
build:
	-cd compiler; $(MAKE) build
	-cd interpreter; $(MAKE) build
	-cd converter; $(MAKE) build
	-cd library; $(MAKE) build

.PHONY: unit
unit:
	-cd compiler; $(MAKE) unit
	-cd interpreter; $(MAKE) TESTOUTPUT=$(TESTOUTPUT) OUT="TEST-interpreter-unit.xml" unit

.PHONY: test
test:
	@java -jar bin/jregr.jar -bin bin -dir regression $(TESTOUTPUT)
#	@java -jar bin/jregr.jar -bin bin -dir regression/versions/compiler $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir regression/versions/interpreter $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir regression/tracing $(TESTOUTPUT)
	@java -jar bin/jregr.jar -dir regression/restore $(TESTOUTPUT)		# Uses sh not the executables
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing/positions $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir library/testing $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir converter/testing $(TESTOUTPUT)

.PHONY: zip
zip: doc/manual/manual.pdf bin/alan.exe bin/arun.exe alan.readme.txt CHANGES.txt alan.readme.windows.txt games/adventv3/adventV3.a3c regression/saviour.alan regression/logo.png COPYING 
	-rm alan*.zip
	zip -j alan$(VERSION).win32.x86.zip $^

.PHONY: setup
setup: COPYING.txt CHANGES.txt alan.readme.txt alan.readme.windows.txt converter/a2a3.readme.txt
	-rm winarun*setup.exe
	sed -e s/VERSION/$(VERSION)/ winarun.iss > winarun_tmp.iss
	/cygdrive/c/Program\ Files/Inno\ Setup\ 5/iscc winarun_tmp.iss
	-rm winarun_tmp.iss
	-rm alan*setup.exe
	sed -e s/VERSION/$(VERSION)/ alan.iss > alan_tmp.iss
	/cygdrive/c/Program\ Files/Inno\ Setup\ 5/iscc alan_tmp.iss
	-rm alan_tmp.iss

.PHONY: sync
sync:
	/cygdrive/c/Program\ Files/Allway\ Sync/Bin/syncappw.exe -s AlanIFDownloadSync -m -e

games/adventv3/adventV3.a3c: games/adventv3/adventV3.alan bin/alan
	cd games/adventv3; ../../bin/alan adventv3

COPYING.txt: COPYING
	unix2dos >$@ <$<

CHANGES.txt: CHANGES
	unix2dos >$@ <$<

alan.readme.txt: alan.readme
	unix2dos >$@ <$<

alan.readme.windows.txt: alan.readme.windows
	unix2dos >$@ <$<

converter/a2a3.readme.txt: converter/a2a3.readme
	unix2dos >$@ <$<
