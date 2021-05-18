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
BeforeEach(Readline) {
    never_expect(doBeep);
}
AfterEach(Readline) {}

Ensure(Readline, can_read_some_ascii_characters) {
    char stubbed_char = 'a';
    char newline = '\n';
    char buffer[100];

    cgreen_mocks_are(loose_mocks);

    /* Expect to read 3 'a's and a newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_char, sizeof(char)),
           will_return(1),
           times(3));
    /* ... and echo them */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &newline, sizeof(char)),
           will_return(1));

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("aaa")),
           will_return(strdup("aaa"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("aaa"));
}



Ensure(Readline, can_delete_last_ascii_character) {
    char stubbed_char = 'a';
    char delBwd = '\x08';
    char newline = '\n';
    char buffer[100];

    cgreen_mocks_are(loose_mocks);

    /* Expect to read 3 'a's, a delete and a newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_char, sizeof(char)),
           will_return(1),
           times(3));
    /* Now delete last */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &delBwd, sizeof(char)),
           will_return(1));
    /* ... and newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &newline, sizeof(char)),
           will_return(1));

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("aa")),
           will_return(strdup("aa"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("aa"));
}

Ensure(Readline, can_delete_a_ascii_character_in_the_middle) {
    char stubbed_a = 'a';
    char stubbed_b = 'b';
    char stubbed_c = 'c';
    char escHook = '\x1b';
    char arrowHook = '\x5b';
    char leftArrow = '\x44';
    char delBwd = '\x08';
    char newline = '\n';
    char buffer[100];

    cgreen_mocks_are(loose_mocks);

    /* Expect to read 'a', 'b', 'c', a leftArrow, a delete and a newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_a, sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_b, sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_c, sizeof(char)),
           will_return(1));
    /* Move left */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &escHook, sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &arrowHook, sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &leftArrow, sizeof(char)),
           will_return(1));
    /* Now delete the b */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &delBwd, sizeof(char)),
           will_return(1));
    /* ... and newline... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &newline, sizeof(char)),
           will_return(1));

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("ac")),
           will_return(strdup("ac"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("ac"));
}
