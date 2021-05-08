#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

/* SUT: */
#include "lmlog.h"

/* Mocks: */
#include "lmList.mock"
#include "srcp.mock"
#include "converter.mock"


Describe(Lmlog);
BeforeEach(Lmlog) {}
AfterEach(Lmlog) {}


Ensure(Lmlog, converts_no_varargs_to_empty_insert_string) {
    expect(lmLog);
    lmlogv(&nulsrcp, 0, 0, NULL);
}

Ensure(Lmlog, converts_one_vararg_string_to_single_insert_string) {
    expect(ensureExternalEncoding,
           will_return(strdup("string")));
    expect(lmLog, when(istrs, is_equal_to_string("string")));

    lmlogv(&nulsrcp, 0, 0, "string", NULL);
}

Ensure(Lmlog, converts_two_vararg_string_to_insert_string) {
    expect(ensureExternalEncoding,
           when(input, is_equal_to_string("string1")),
           will_return(strdup("string1")));
    expect(ensureExternalEncoding,
           when(input, is_equal_to_string("string2")),
           will_return(strdup("string2")));
    expect(lmLog, when(istrs, is_equal_to_string("string1" "\xff" "string2")));

    lmlogv(&nulsrcp, 0, 0, "string1", "string2", NULL);
}

Ensure(Lmlog, converts_two_arguments_to_external_encoding) {
    expect(ensureExternalEncoding,
           when(input, is_equal_to_string("\340\341\342")),
           will_return(strdup("åäö")));
    expect(ensureExternalEncoding,
           when(input, is_equal_to_string("\361\211\231")),
           will_return(strdup("ñ‰™")));
    expect(lmLog, when(istrs, is_equal_to_string("åäö" "\xff" "ñ‰™")));

    /* ö ä å, n tilde, per mille, trademark */
    lmlogv(&nulsrcp, 0, 0, "\340\341\342", "\361\211\231", NULL);
}

Ensure(Lmlog, converts_the_argument_to_external_encoding) {
    expect(ensureExternalEncoding,
           when(input, is_equal_to_string("\340\341\342")),
           will_return(strdup("åäö")));
    expect(lmLog, when(istrs, is_equal_to_string("åäö")));

    lmlog(&nulsrcp, 0, 0, "\340\341\342");
}
