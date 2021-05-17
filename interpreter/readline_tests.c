#include <cgreen/cgreen.h>

#include "readline.h"


/* Mocked modules */
#include "syserr.mock"
#include "output.mock"
#include "converter.mock"

/* Need this just because instance_tests.c uses real set, so other isolated tests must link it too */
#include "instance.mock"

Describe(Readline);
BeforeEach(Readline) {}
AfterEach(Readline) {}

Ensure(Readline, can_have_tests) {
    pass_test();
}
