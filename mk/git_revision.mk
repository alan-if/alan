###################################################################
#
# GIT_REVISION - marking a build with the source it was built from
#
# Generates git_revision.h, defining GIT_REVISION as the output of
# 'git describe', for the modules that print version information.
#
# This happens at parse time rather than through a rule so that the
# header is guaranteed to exist before the first compile, in every
# object directory and for every variant, without each of them having
# to declare a dependency on it. From then on the -MMD dependencies
# pick it up like any other header.
#
# The generator rewrites the file only when the revision has actually
# changed, so this does not cause a rebuild on every make. Note that
# the revision changes as soon as the working tree is modified, since
# 'git describe --dirty' is what makes a build identifiable at all.
#
# The generated header is never committed, see .gitignore.

GIT_REVISION_HEADER := $(shell sh ../mk/git-revision.sh git_revision.h)
