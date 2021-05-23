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

static void expect_a(void) {
    static char stubbed_a = 'a';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_a, sizeof(char)),
           will_return(1));
}

static void expect_b(void) {
    static char stubbed_b = 'b';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_b, sizeof(char)),
           will_return(1));
}

static void expect_c(void) {
    static char stubbed_c = 'c';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_c, sizeof(char)),
           will_return(1));
}


Ensure(Readline, can_read_some_ascii_characters) {
    char buffer[100];

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_c();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("abc")),
           will_return(strdup("abc"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("abc"));
}


static void expect_delBwd(void) {
    static char delBwd = '\x08';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &delBwd, sizeof(char)),
           will_return(1));
}

static void expect_delFwd(void) {
    static char delBwd = '\x04';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &delBwd, sizeof(char)),
           will_return(1));
}

Ensure(Readline, can_delete_last_ascii_character) {
    char buffer[100];

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_c();

    /* Now delete last */
    expect_delBwd();

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("ab")),
           will_return(strdup("ab"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("ab"));
}

static void expect_leftArrow(void) {
    static uchar leftArrow []= "\x1b\x5b\x44";

    for (int i=0; i<sizeof(leftArrow)-1; i++)
        expect(mocked_read,
           will_set_contents_of_parameter(buf, &leftArrow[i], sizeof(char)),
           will_return(1));
}

Ensure(Readline, can_delete_next_ascii_character) {
    char buffer[100];

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_c();

    /* Now move left and delete next */
    expect_leftArrow();
    expect_delFwd();

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("ab")),
           will_return(strdup("ab"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("ab"));
}



Ensure(Readline, can_delete_an_ascii_character_in_the_middle) {
    char buffer[100];

    /* Expect to read 'a', 'b', 'c' ... */
    expect_a();
    expect_b();
    expect_c();

    /* Move left */
    expect_leftArrow();

    /* Now delete the 'b' */
    expect_delBwd();

    /* ... and newline... */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("ac")),
           will_return(strdup("ac"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("ac"));
}

Ensure(Readline, can_insert_an_ascii_character_in_the_middle) {
    char buffer[100];

    /* Expect to read 'a', 'c' ... */
    expect_a();
    expect_c();

    /* Move left */
    expect_leftArrow();

    expect_b();

    /* ... and newline... */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("abc")),
           will_return(strdup("abc"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("abc"));
}

static void expect_aring(void) {
    static uchar stubbed_aring[2] = {0xC3, 0xA5}; // 'å';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_aring[1], sizeof(char)),
           will_return(1));
}

static void expect_adiaeresis(void) {
    static uchar stubbed_adiaeresis[2] = {0xC3, 0xA4}; // 'ä';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_adiaeresis[1], sizeof(char)),
           will_return(1));
}

static void expect_odiaeresis(void) {
    static uchar stubbed_odiaeresis[2] = {0xC3, 0xB6}; // 'ö';
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[0], sizeof(char)),
           will_return(1));
    expect(mocked_read,
           will_set_contents_of_parameter(buf, &stubbed_odiaeresis[1], sizeof(char)),
           will_return(1));
}


Ensure(Readline, can_read_some_utf8_characters) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect_aring();
    expect_adiaeresis();
    expect_odiaeresis();

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\xC3\xA4\xC3\xB6")),
           will_return(strdup("\xE5\xE4\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("\xE5\xE4\xF6"));
}


Ensure(Readline, can_delete_last_utf8_character) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect_aring();
    expect_adiaeresis();
    expect_odiaeresis();

    /* ... and delete the last character */
    expect_delBwd();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\xC3\xA4")), /* åä */
           will_return(strdup("\xE5\xE4"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("\xE5\xE4"));
}


Ensure(Readline, can_delete_next_utf8_character) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect_aring();
    expect_adiaeresis();
    expect_odiaeresis();

    expect_leftArrow();
    /* ... and delete the next character */
    expect_delFwd();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\xC3\xA4")), /* åä */
           will_return(strdup("\xE5\xE4"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("\xE5\xE4"));
}

Ensure(Readline, can_delete_an_utf8_character_in_the_middle) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Expect to read 3 UTF-chars, which is actually 6 bytes ... */
    expect_aring();
    expect_adiaeresis();
    expect_odiaeresis();

    /* backup one UTF-8 character */
    expect_leftArrow();

    /* ... and delete the last character */
    expect_delBwd();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\xC3\xB6")), /* åö */
           will_return(strdup("\xE5\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("\xE5\xF6"));
}

static void type_insert_toogle(void) {
    static uchar insertToggle[] = "\x1b\x5b\x32z";

    for (int i=0; i<sizeof(insertToggle)-1; i++)
        expect(mocked_read,
               will_set_contents_of_parameter(buf, &insertToggle[i], sizeof(char)),
               will_return(1));
}

Ensure(Readline, can_overwrite_singlebyte_character_with_singlebyte) {
    char buffer[100];

    type_insert_toogle();

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_c();

    expect_leftArrow();         /* Backup ... */
    expect_a();                 /* ... and overwrite last char with an 'a' */

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("aba")),
           will_return(strdup("aba"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("aba"));
}

Ensure(Readline, can_overwrite_multibyte_character_with_singlebyte) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    type_insert_toogle();

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_adiaeresis();

    expect_leftArrow();         /* Backup ... */
    expect_a();                 /* ... and overwrite last char with an 'a' */

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("aba")),
           will_return(strdup("aba"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("aba"));
}

Ensure(Readline, can_overwrite_singlebyte_character_with_multibyte) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    type_insert_toogle();

    /* Expect to read 'abc' ... */
    expect_a();
    expect_b();
    expect_c();

    expect_leftArrow();         /* Backup ... */
    expect_odiaeresis();

    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("ab\xC3\xB6")),
           will_return(strdup("ab\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);

    assert_that(buffer, is_equal_to_string("ab\xF6"));
}

Ensure(Readline, can_insert_an_utf8_character_in_the_middle) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Type two UTF-8 characters */
    expect_aring();
    expect_odiaeresis();

    /* backup one UTF-8 character */
    expect_leftArrow();

    /* ... and type another UTF-8 character */
    expect_adiaeresis();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\xC3\xA4\xC3\xB6")), /* åäö */
           will_return(strdup("\xE5\xE4\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("\xE5\xE4\xF6"));
}

Ensure(Readline, can_insert_a_singlebyte_character_in_the_middle_of_multibytes) {
    char buffer[100];

    encodingOption = ENCODING_UTF;

    /* Type two UTF-8 characters */
    expect_aring();
    expect_odiaeresis();

    /* backup one UTF-8 character */
    expect_leftArrow();

    /* ... and type another character */
    expect_a();

    /* Enter */
    expect_newline();

    expect(ensureInternalEncoding,
           when(string, is_equal_to_string("\xC3\xA5\x61\xC3\xB6")), /* åäö */
           will_return(strdup("\xE5\x61\xF6"))); /* Because it should be malloc'ed */

    readline(buffer);
    assert_that(buffer, is_equal_to_string("\xE5\x61\xF6"));
}

extern int character_length(uchar *utf_string);

Ensure(Readline, can_count_utf_chars) {

    encodingOption = ENCODING_ISO;

    uchar three_ascii[] = "abc";
    assert_that(character_length(three_ascii), is_equal_to(3));

    uchar three_ascii_with_utf_prefix[] = "abc\xC3";
    assert_that(character_length(three_ascii_with_utf_prefix), is_equal_to(4));

    encodingOption = ENCODING_UTF;

    uchar three_other_ascii[] = "abc";
    assert_that(character_length(three_other_ascii), is_equal_to(3));

    uchar four_ascii[] = "abcd";
    assert_that(character_length(four_ascii), is_equal_to(4));

    uchar one_utf[] = "ö";
    assert_that(character_length(one_utf), is_equal_to(1));

    uchar two_utf[] = "öñ";
    assert_that(character_length(two_utf), is_equal_to(2));

    uchar mixed[] = "Aöabñ4";
    assert_that(character_length(mixed), is_equal_to(6));
}
