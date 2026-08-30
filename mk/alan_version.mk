###################################################################
#
# The Alan version, for component directories that compile C
#
# Generates alan_version.h from the components in the top level
# VERSION file, and makes the shared version sources available
# locally, the same way and for the same reason as git_revision.h:
# compilation happens in the component directory.
#
# This replaces the old mk/venum.mk. venum generated the version
# *and* a build timestamp into files that were then committed, so
# every build on a machine that had venum dirtied the working tree,
# and every machine that did not have it silently used whatever the
# last committed timestamp happened to be. The version is now a plain
# checked-in fact and the timestamp comes from the commit, see
# mk/git-revision.sh.
#
# Generated at parse time so the header exists before the first
# compile, and rewritten only when the version changed.

ALAN_VERSION_HEADER := $(shell sh ../mk/alan-version.sh alan_version.h \
	$(strip $(ALAN_VERSION)) $(strip $(ALAN_REVISION)) \
	$(strip $(ALAN_CORRECTION)) "$(strip $(ALAN_STATE))" $(VERSION))

# The Product struct and its declaration are shared, but compilation
# happens here, so keep local copies. They are generated, see .gitignore.
#
# At parse time, and not as rules, for the same reason the headers above
# are generated at parse time: nothing lists these as prerequisites, so
# rules would not fire before the first compile of a fresh checkout, and
# the -MMD dependencies that would list them do not exist yet either.
# Copied only when the contents differ, so mtimes stay put and an
# unchanged file does not force a rebuild.
ALAN_VERSION_SOURCES := $(shell for f in alan.version.c alan.version.h version.h ; do \
	cmp -s ../$$f $$f || cp ../$$f $$f ; done)
