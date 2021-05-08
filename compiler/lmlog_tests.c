#include <cgreen/cgreen.h>

/* SUT: */
#include "lmlog.h"

/* Mocks: */
#include "lmList.mock"
#include "srcp.mock"


Describe(Lmlog);
BeforeEach(Lmlog) {}
AfterEach(Lmlog) {}


Ensure(Lmlog, converts_no_varargs_to_empty_insert_string) {
    expect(lmLog, when(istrs, is_equal_to_string("")));

    lmlogv(&nulsrcp, 0, 0, NULL);
}

Ensure(Lmlog, converts_one_vararg_string_to_single_insert_string) {
    expect(lmLog, when(istrs, is_equal_to_string("string")));

    lmlogv(&nulsrcp, 0, 0, "string", NULL);
}

Ensure(Lmlog, converts_two_vararg_string_to_insert_string) {
    expect(lmLog, when(istrs, is_equal_to_string("string1" "\xff" "string2")));

    lmlogv(&nulsrcp, 0, 0, "string1", "string2", NULL);
}
