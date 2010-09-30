######################################################################
# Makefile for the new library
# Runs the tests and builds a zipfile
all: tests zip

build: zip

.PHONY: tests
tests:
	cd testing; java -jar ../../bin/jregr.jar

.PHONY: zip
zip: changelog.txt classes.i COPYING.txt verbs.i locations.i messages.i library.i readme.txt testgame.alan
	-rm library*.zip
	zip library0.95.zip $^
