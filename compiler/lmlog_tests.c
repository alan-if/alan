#include <cgreen/cgreen.h>

/* SUT: */
#include "lmlog.h"

/* Mocks: */
#include "lmList.mock"
#include "srcp.mock"


/* Ensure also private functions are available in the SUT */

Describe(Lmlog);
BeforeEach(Lmlog) {}
AfterEach(Lmlog) {}

/* No tests yet */
Ensure(Lmlog, should_convert_no_varargs_to_empty_insert_string) {
    expect(lmLog, when(istrs, is_equal_to_string("")));

    lmlogv(&nulsrcp, 0, 0, "", NULL);
}
