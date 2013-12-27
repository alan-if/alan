######################################################################
# Makefile for the new library
# Runs the tests and builds a zipfile
VERSION = `cat VERSION`
DISTDIR	= ~/Utveckling/Alan/alanweb/downloads

all: test package

build: zip

.PHONY: test
test:
	@cd ..;bin/jregr -bin bin -dir library/testing $(JREGROUTPUT)

.PHONY: package
package: zip

.PHONY: clean
clean:
	rm library*.zip

# Release is only possible on Thoni's machines
.PHONY: release
release: test zip
	cp library$(VERSION).zip $(DISTDIR)/Library
	echo "Please update VERSION file to indicate next version number"

#####################################################################

.PHONY: zip
zip: Changelog.txt classes.i COPYING.txt verbs.i locations.i messages.i library.i QuickRef.txt readme.txt testgame.alan
	-rm library*.zip
	zip library$(VERSION).zip $^
