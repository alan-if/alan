#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "whr_x.h"

#include "lmList.mock"
#include "smScSema.mock"
#include "srcp.mock"
#include "exp.mock"
#include "sym.mock"
#include "cnt.mock"
#include "emit.mock"


Describe(Where);

BeforeEach(Where) {}

AfterEach(Where) {}

Ensure(Where, generates_transitivity) {
    expect(emitConstant, when(word, is_equal_to(DIRECTLY)));
    generateTransitivity(DIRECTLY);
}
