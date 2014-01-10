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

Ensure(Symbol, can_return_class_of_symbol_for_instance) {
    Symbol *parent = NEW(Symbol);
    Id *id = newId(nulsrcp, "classname");
    Symbol *symbol = newInstanceSymbol(id, NULL, parent);

    assert_that(classOfSymbol(symbol), is_equal_to(parent));
}

Ensure(Symbol, can_return_class_of_symbol_for_parameter) {
    Symbol *parent = NEW(Symbol);
    Id *id = newId(nulsrcp, "classname");
    Element *element = newParameterElement(nulsrcp, id, 0);
    Symbol *symbol = newParameterSymbol(element);
    symbol->fields.parameter.class = parent;

    assert_that(classOfSymbol(symbol), is_equal_to(parent));
}

Ensure(Symbol, can_return_class_of_symbol_for_local) {
    Symbol *parent = NEW(Symbol);
    Id *id = newId(nulsrcp, "classname");
    Element *element = newParameterElement(nulsrcp, id, 0);
    Symbol *symbol = newParameterSymbol(element);
    symbol->fields.parameter.class = parent;

    assert_that(classOfSymbol(symbol), is_equal_to(parent));
}


static void mocked_syserr(char *string) {
    mock(string);
}

Ensure(Symbol, cannot_return_class_of_symbol_for_verb) {
    Id *id = newId(nulsrcp, "verbname");
    Symbol *symbol = newVerbSymbol(id);

    setSyserrHandler(mocked_syserr);
    expect(mocked_syserr);

    classOfSymbol(symbol);
}

Ensure(Symbol, can_get_type_according_to_symbol) {
    Symbol *integer = newSymbol(newId(nulsrcp, "integer"), INSTANCE_SYMBOL);

    integerSymbol = integer;

    assert_that(basicTypeFromSymbol(integer), is_equal_to(INTEGER_TYPE));
}

Ensure(Symbol, can_get_type_of_a_local_or_parameter_symbol) {
    Symbol *local_symbol = NEW(Symbol);
    Symbol *parameter_symbol = NEW(Symbol);

    local_symbol->kind = LOCAL_SYMBOL;
    parameter_symbol->kind = PARAMETER_SYMBOL;

    local_symbol->fields.local.type = STRING_TYPE;
    parameter_symbol->fields.parameter.type = INTEGER_TYPE;

    assert_that(typeOfSymbol(local_symbol), is_equal_to(STRING_TYPE));
    assert_that(typeOfSymbol(parameter_symbol), is_equal_to(INTEGER_TYPE));
}
