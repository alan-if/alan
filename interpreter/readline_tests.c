#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "readline.h"

#include "options.h"


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
    always_expect(mocked_write, will_return(1));
}
AfterEach(Readline) {}


static void expect_newline(void) {
    static char newline = '\n';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &newline, sizeof(char)),
           will_return(1));
}


Ensure(Readline, can_read_some_ascii_characters) {
    char stubbed_char = 'a';
    char buffer[100];

    cgreen_mocks_are(loose_mocks);

    /* Expect to read 3 'a's ... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_char, sizeof(char)),
           will_return(1),
           times(3));

    expect_newline();

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


static void expect_delBwd(void) {
    static char delBwd = '\x08';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &delBwd, sizeof(char)),
           will_return(1));
}


Ensure(Readline, can_delete_an_ascii_character_in_the_middle) {
    char stubbed_a = 'a';
    char stubbed_b = 'b';
    char stubbed_c = 'c';
    char escHook = '\x1b';
    char arrowHook = '\x5b';
    char leftArrow = '\x44';
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
    expect_delBwd();

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


Ensure(Readline, can_read_some_utf8_characters) {
    uchar stubbed_aring[2] = {0xC3, 0xA5}; // 'å';
    uchar stubbed_adiaeresis[2] = {0xC3, 0xA4}; // 'ä';
    uchar stubbed_odiaeresis[2] = {0xC3, 0xB6}; // 'ö';
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[1], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[1], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[1], sizeof(char)),
           will_return(1));

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("åäö")),
           will_return(strdup("\xE5\xE4\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("\xE5\xE4\xF6"));
}


Ensure(Readline, can_delete_last_utf8_character) {
    uchar stubbed_aring[2] = {0xC3, 0xA5}; // 'å';
    uchar stubbed_adiaeresis[2] = {0xC3, 0xA4}; // 'ä';
    uchar stubbed_odiaeresis[2] = {0xC3, 0xB6}; // 'ö';
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[1], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[1], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[1], sizeof(char)),
           will_return(1));

    /* ... and delete the last character */
    expect_delBwd();

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("åä")),
           will_return(strdup("\xE5\xE4"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("\xE5\xE4"));
}
