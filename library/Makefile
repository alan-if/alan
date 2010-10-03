######################################################################
# Makefile for the new library
# Runs the tests and builds a zipfile
VERSION = `cat VERSION`
all: tests zip

build: zip

.PHONY: test
test:
	cd testing; java -jar ../../bin/jregr.jar

.PHONY: zip
zip: changelog.txt classes.i COPYING.txt verbs.i locations.i messages.i library.i readme.txt testgame.alan
	-rm library*.zip
	zip library$(VERSION).zip $^
