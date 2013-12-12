#include <cgreen/cgreen.h>

#include "sym_x.h"

#include "util.h"

#include "lmList.mock"
#include "atr.mock"
#include "str.mock"
#include "prop.mock"
#include "ext.mock"
#include "id.mock"
#include "cnt.mock"
#include "srcp.mock"
#include "elm.mock"

List *fileNames = NULL;

Describe(Symbol);
BeforeEach(Symbol) {}
AfterEach(Symbol) {}

Ensure(Symbol, can_return_class_of_symbol) {
    Symbol *parent = NEW(Symbol);
    IdNode *id = newId(nulsrcp, "classname");
    Symbol *symbol = newInstanceSymbol(id, NULL, parent);

    assert_that(classOfSymbol(symbol), is_equal_to(parent));
}
