#include <cgreen/cgreen.h>

#include "stm_x.h"

#include "lmList.mock"
#include "context.mock"
#include "exp.mock"
#include "emit.mock"
#include "sym.mock"
#include "adv.mock"
#include "wht.mock"
#include "whr.mock"
#include "srcp.mock"
#include "id.mock"
#include "encode.mock"
#include "smScSema.mock"
#include "set.mock"
#include "atr.mock"
#include "resource.mock"


/* Global data */
FILE *txtfil;
int totalScore;


Describe(Statement);
BeforeEach(Statement) {}
AfterEach(Statement) {}


Ensure(Statement, analyzeIf_creates_restricting_context_for_Id_Isa_expression) {
    What *what = newWhatId(nulsrcp, newId(nulsrcp, "id"));
    Expression *what_expression = newWhatExpression(nulsrcp, what);
    Expression *isa = newIsaExpression(nulsrcp, what_expression, FALSE, NULL);
    expect(addRestrictionInContext);
}
