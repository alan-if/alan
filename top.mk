.PHONY: build
build:
	-cd compiler; $(MAKE) -k build
	-cd interpreter; $(MAKE) -k build
	-cd converter; $(MAKE) -k build
	-cd library; $(MAKE) -k build

.PHONY: unit
unit:
	-cd compiler; $(MAKE) unit
	-cd interpreter; $(MAKE) unit

.PHONY: test
test:
	@java -jar bin/jregr.jar -bin bin -dir regression $(TESTOUTPUT)
	@java -jar bin/jregr.jar -dir regression/restore $(TESTOUTPUT)
	@java -jar bin/jregr.jar -dir regression/versions $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir regression/winalan $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir compiler/testing/positions $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir library/testing $(TESTOUTPUT)
	@java -jar bin/jregr.jar -bin bin -dir converter/testing $(TESTOUTPUT)

.PHONY: zip
zip: doc/manual/manual.pdf bin/alan.exe bin/arun.exe alan.readme CHANGES alan.readme.windows lib/alanlib*.zip games/adventv3/adventV3.a3c regression/saviour.alan COPYING
	-rm alan*.zip
	zip alan$(VERSION).win32.x86.zip $^

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
	cd games/adventv3; alan adventv3

# Checking for up-to-date files which can not be automated
doc/manual/manual.pdf: doc/manual/manual.odt
	@echo "PDF is not up-to-date"
	exit 1

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
