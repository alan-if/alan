#! /bin/bash
#
# Tests for bin/jregr, the wrapper that runs the regression tool.
#
# The wrapper is what reports missing test tooling, so it has to work on
# a machine where the tooling is missing or wrong. That rules out testing
# it through jregr itself, and these are plain shell instead. They need
# nothing but bash, and no network.
#
# Each case runs a copy of the wrapper in a temporary directory, with a
# stub java first on PATH, so that nothing here touches the real bin/ or
# the jar that may or may not have been fetched into it.

wrapper="$(cd -P "$(dirname "$0")/.." && pwd)"/jregr

failures=0

fail() {
    echo "FAIL: ${FUNCNAME[1]}: $1" >&2
    failures=$((failures+1))
}

# A sandbox holding a copy of the wrapper, a jar beside it so that
# nothing is fetched, and a directory to put stub commands in.
sandbox() {
    dir=$(mktemp -d "${TMPDIR:-/tmp}/jregr-tests.XXXXXX")
    cp "$wrapper" "$dir"/jregr
    : > "$dir"/jregr.jar
    mkdir "$dir"/stubs
}

# A java of a version older than the jar was compiled for. This is what
# the JVM itself prints, with the numbers of a Java 11 asked to run a jar
# built for 21.
stub_too_old_java() {
    cat > "$dir"/stubs/java <<'STUB'
#! /bin/sh
echo "Error: LinkageError occurred while loading main class se.alanif.jregr.Main" >&2
echo "    java.lang.UnsupportedClassVersionError: se/alanif/jregr/Main has been compiled by a more recent version of the Java Runtime (class file version 65.0), this version of the Java Runtime only recognizes class file versions up to 55.0" >&2
exit 1
STUB
    chmod +x "$dir"/stubs/java
}

test_a_java_too_old_for_the_jar_is_reported() {
    sandbox
    stub_too_old_java

    output=$(PATH="$dir/stubs:$PATH" "$dir"/jregr -bin . -dir testing 2>&1)
    status=$?

    case "$output" in
        *"too old"*) ;;
        *) fail "should say the java is too old, said: $output" ;;
    esac
    case "$output" in
        *"make build"*) ;;
        *) fail "should point at 'make build', said: $output" ;;
    esac
    case "$output" in
        *"class file version"*) ;;
        *) fail "should pass on what java said, said: $output" ;;
    esac
    [ "$status" -eq 1 ] || fail "should exit 1, exited $status"

    rm -rf "$dir"
}

test_a_java_too_old_for_the_jar_is_reported

if [ "$failures" -gt 0 ]; then
    echo "$failures failure(s) in $(basename "$0")" >&2
    exit 1
fi
echo "$(basename "$0"): all tests passed"
