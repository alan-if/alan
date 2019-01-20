#include <cgreen/cgreen.h>

/* Mocks: */
#include "id.mock"
#include "sym.mock"
#include "exp.mock"
#include "srcp.mock"
#include "cla.mock"
#include "ins.mock"
#include "encode.mock"
#include "adv.mock"
#include "context.mock"
#include "wht.mock"
#include "lmList.mock"
#include "smScSema.mock"
#include "emit.mock"
#include "atr.mock"
#include "cnt.mock"
#include "ext.mock"
#include "whr.mock"
#include "stm.mock"
#include "article.mock"
#include "vrb.mock"
#include "scr.mock"
#include "nam.mock"
#include "description.mock"
#include "initialize.mock"

/* Utility modules: */
#include "lst_x.h"

/* Ensure also private functions are available in the SUT */
#define UNITTESTING
#include "prop_x.h"


Describe(Properties);
BeforeEach(Properties) {}
AfterEach(Properties) {}

/* No tests yet */
Ensure(Properties, no_tests_yet) {
}
