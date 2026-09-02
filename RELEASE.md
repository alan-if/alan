# How to make a relase of the Alan System

This file attempts to describe the procedure for creating releases and
distributions of the Alan system.

The strategy requires a main development machine. Currently that is
ThoNi's machine, where also https://ci.alanif.se is running a release
job, "Alan Release".

Main development environment have for a long time been Cygwin (for its
Unix capabilities and easy cross-compilation to Windows), but that is
changing now that WSL (Windows Subsystem for Linux) exists.

In essence a release will have a semver version marking. Any
development build before the release carries that same marking with a
`-dev<count>` suffix, where the count is how many commits it is past
the previous release tag. Once the release is created all subsequent
builds will have the next release version as its marking.

## Two markings, and why both are needed

Every binary reports two things, and a release needs both to be right:

    $ alan -version
    3.0beta9                             <- what it calls itself
    Built from git v3.0beta8-98-g7142db50 <- what it was built from

The first comes from the `VERSION` file. It is also what names the
packages and what goes into the `.a3c` header.

Note that the two disagree above, and that this is correct: `VERSION`
names the release being worked *towards*, the tag names the last one
actually made. They coincide only on the release commit itself.

The second comes from `git describe`, anchored on the release tags.
It needs no maintenance, but it does need the tag to exist:

    Built from git v3.0beta8             a release build, from the tag
    Built from git v3.0beta8-95-gabc1234 95 commits past that release
    Built from git v3.0beta8-95-gabc1234-dirty  ...and uncommitted edits

So the bare form is the signature of an actual release, and it is only
obtainable by tagging the commit and pushing the tag. A bug report
quoting a suffixed form is someone's own build, not something shipped.

## Naming a development build

The commit count is also what names development packages, as
`BUILDVERSION` in `mk/common.mk`:

    alan-3.0beta9-dev98-linux-x86_64.tgz   98 commits into the beta9 cycle
    alan-3.0beta9-linux-x86_64.tgz         the beta9 release itself

The count is monotonic within a cycle because master is linear - one
commit, one increment - and it restarts at each release, being a
distance from a known point rather than a global serial. It replaces
the Jenkins `BUILD_NUMBER`, which no longer exists.

That restart is why `BUILDNAME`, which names the upload directory
(`snapshots/$(BUILDNAME)`), carries the version and not just the count:

    snapshots/3.0beta9-dev98        not  snapshots/dev98

`Build<n>` was a global serial and could never repeat. `dev98` can, and
would overwrite the previous cycle's. `BUILDNAME` is also defined
unconditionally, so a build made on a release tag is
`snapshots/3.0beta9` rather than the bare `snapshots/`.

Deriving it needs the release tags. **A CI job must therefore fetch
them** - Jenkins' git plugin does by default, but a shallow clone does
not, and `actions/checkout` needs `fetch-depth: 0`. Without them a
build cannot tell a release from a development build. Rather than
quietly naming such a build as if it were a release, it is named
`-devunknown`, matching the `unknown` that `mk/git-revision.sh` emits
in the same situation. If a CI job produces that, its checkout is
shallow and needs `fetch-depth: 0`.

## Order of operations

The two markings are bumped at different moments, and the order
matters, because `make tags` tags whatever `VERSION` currently says:

1. `VERSION` still holds the version being released, e.g. 3.0beta9.
   Build, test, package, upload. `BUILDVERSION` is empty at this point
   only if you are standing on the release commit with the tag already
   made; the packages uploaded by `distribution` refuse to be built
   otherwise.
2. `make tags`. Writes LASTRELEASE, tags `v3.0beta9`, pushes the tag,
   and creates a local per-OS branch.
3. *Then* bump `ALAN_CORRECTION` in `VERSION` and commit, opening the
   next cycle as 3.0beta10.

Bumping before tagging puts the new number on the old commit. Between
steps 3 and the next release the two markings deliberately disagree,
which reads correctly as "3.0beta10 in development, so many commits
past the v3.0beta9 tag".

Release tags are lightweight and use the dotted spelling, `v3.0beta8`.
Both matter: `mk/git-revision.sh` matches `v[0-9]*.[0-9]*`, so the older
underscored spelling and the rest of the legacy tags -- `v2_base`,
`v261`, `v3_start` -- are not candidates, and it passes `--tags`,
without which lightweight tags are invisible to `git describe`.

## The Alan Release job

The release job will do

    make release

This should always be done on the main development machine (ThoNi's)
before any attempt to build release distributions on any secondary
platform. It will check that the source files are checked in, and
some other stuff. You can't do a release otherwise.

It then goes on to build the primary binaries for Windows (alan, arun,
WinAlan and WinArun), currently using cross-compile from Cygwin (or
WSL/Linux), for release and run all tests.

If that is successful, `make tags` sets a tag (vX.Y.Z) at the current
HEAD in the main branch and pushes it, see "Order of operations"
above. Later a branch for this OS, and a bugfix branch, can be created
at the same commit.

Then all packages for the primary platform is uploaded to the Alan
website.

The file LASTRELEASE will be updated to contain the version of the last
released version on the current branch.

Once this job has been run successfully the version should be stepped,
at least to:

    vX.Y.(Z+1)

by editing the top level `VERSION` file, normally bumping
`ALAN_CORRECTION`, and committing it. `VERSION` is the only place the
version is written down; the makefiles, the C sources and the Windows
installer metadata all derive from it.

Note that `ALAN_STATE` is not cosmetic. Its first character is written
into the `.a3c` header and compared by the interpreter's compatibility
check, so changing it changes which story files this system will run.
It also picks the third field of the installer's four-number
VIProductVersion, see ALAN_STATE_NUMBER in Makefile.win32.

## Secondary platforms

On seconday platforms (Linux, MacOS) you should checkout the release
tag and then do

    make distribution

This should build all binaries, run all tests, and if succesful
package them, and upload them to the web site. The distribution build
should use LASTRELEASE as the version marking.

Normal builds on secondary platforms take their version from `VERSION`
like everywhere else.

## Non-standard binaries

On some platforms there are some extra binaries, e.g gararun, the
Gargoyle slot-in interpreter.

On Windows that has to be built separately. In a Cygwin terminal
enable gararun and build it using

    make gararun

You will have to upload it manually.
