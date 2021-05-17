#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "readline.h"


/* Mocked modules */
#include "syserr.mock"
#include "output.mock"
#include "converter.mock"

/* Need this just because instance_tests.c uses real set, so other isolated tests must link it too */
#include "instance.mock"

ssize_t mocked_read (int fd, void *buf, size_t nbytes) {
    return mock(fd, buf, nbytes);
}

Describe(Readline);
BeforeEach(Readline) {}
AfterEach(Readline) {}

Ensure(Readline, can_have_tests_with_mocked_read) {
    char buffer[100];
    expect(mocked_read);
    readline(buffer);
}
