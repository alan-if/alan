#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "readline.h"


/* Mocked modules */
#include "syserr.mock"
#include "output.mock"
#include "converter.mock"

/* Need this just because instance_tests.c uses real set, so other isolated tests must link it too */
#include "instance.mock"

ssize_t mocked_read(int fd, char *buf, size_t nbytes) {
    return mock(fd, buf, nbytes);
}

ssize_t mocked_write(int fd, char *buf, size_t nbytes) {
    return mock(fd, buf, nbytes);
}

Describe(Readline);
BeforeEach(Readline) {}
AfterEach(Readline) {}

Ensure(Readline, can_read_some_ascii_characters) {
    char stubbed_char = 'a';
    char newline = '\n';
    char buffer[100];

    /* Expect to read 3 'a's and a newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_char, sizeof(char)),
           will_return(1),
           times(3));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &newline, sizeof(char)),
           will_return(1));
    /* ... and echo them */
    expect(mocked_write,
           will_return(1),
           times(4));

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("aaa")),
           will_return(strdup("aaa"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("aaa"));
}
