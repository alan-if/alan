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
development build before the release will have the same version
marking but with a build number (added in the Jenkins jobs). Once the
release is created all subsequent builds will have the next release
version as its marking.

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

If that is successful, a tag (vX.Y.Z) is set at the current HEAD in
the main branch. Later a branch for this OS, and a bugfix branch, can
be created at the same commit.

Then all packages for the primary platform is uploaded to the Alan
website.

The file LASTRELEASE will be updated to contain the version of the last
released version on the current branch.

Once this job has been run successfully the version should be stepped,
at least to:

    vX.Y.(Z+1)

by editing the top level `VERSION` file, normally bumping
`ALAN_CORRECTION`, and committing it. `VERSION` is the only place the
version is written down; both the makefiles and the C sources derive
everything from it.

Note that `ALAN_STATE` is not cosmetic. Its first character is written
into the `.a3c` header and compared by the interpreter's compatibility
check, so changing it changes which story files this system will run.

Builds also carry the commit they were made from, from `git describe`,
which needs no maintenance -- see `mk/git-revision.sh`.

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
