# How to make a relase of the Alan System

This file attempts to describe the procedure for creating releases and
distributions of the Alan system.

The strategy requires a main development machine. Currently that is
ThoNi's machine, where also https://ci.alanif.se is running a release
job, "Alan Release".

## The Alan Release job

The release job will do

    make release

This should always be done on the main development machine (ThoNi's)
before any attempt to build release distributions on on secondary
platforms. It will check that the source files are checked in, and
some other stuff. You can't do a release otherwise.

It then goes on to build the primary binaries for Windows (alan, arun,
WinAlan and WinArun), currently using cross-compile from Cygwin, for
release and run all tests.

If that is successful, a tag (vXYZ) is set at the current HEAD in the
main branch, and a branch for this OS is created at the same commit.

Then all packages for the primary platform is uploaded to the Alan
website.

The file LASTRELEASE will be updated to contain the venum of the last
released version on the current branch.

Once this job has been run successfully the venum should be upgraded
(manually) and committed.

    vX.Y.(Z+1)

On next compile that upcoming version is written to the file
NEXTRELEASE so that other platforms can use that as version (versions
should only be updated on the main development machine).

## Secondary platforms

On seconday platforms (Linux, MacOS) you should be able to do

    make distribution

This should build all binaries, run all tests, and if succesful
package them, and upload them to the web site.

## Non-standard binaries

On some platforms there are some extra binaries, e.g gararun, the
Gargoyle slot-in interpreter.

On Windows that has to be built separately. In a Cygwin terminal
enable gararun and build it using

    make gararun

You will have to upload it manually.
