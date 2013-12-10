#include <cgreen/cgreen.h>

#include "atr_x.h"
#include "lst_x.h"

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

List *fileNames = NULL;

Symbol *theVerbSymbol;

Describe(Attribute);
BeforeEach(Attribute) {entitySymbol = newSymbol(newId(nulsrcp, "entity"), CLASS_SYMBOL);}
AfterEach(Attribute) {}

static IdNode *given_a_parameter_id_of_class(char *parameterName, Symbol *classOfParameter) {
    IdNode *id = newId(nulsrcp, parameterName);
    id->symbol = newSymbol(id, PARAMETER_SYMBOL);
    id->symbol->fields.parameter.class = classOfParameter;
    return id;
}
static IdNode *given_an_attribute_id(char *attributeName) {
    return newId(nulsrcp, attributeName);
}
static IdNode *given_a_class(char *className) {
    IdNode *classId = newId(nulsrcp, className);
    Symbol *classSymbol = newSymbol(classId, CLASS_SYMBOL);
    classId->symbol = classSymbol;
    return classId;
}
static Attribute *given_an_attribute(IdNode *id) {
    return newBooleanAttribute(nulsrcp, id, TRUE);
}
static Expression *given_a_what_expression_of_class(IdNode *parameterName, Symbol *classSymbol) {
    What *what = newWhatId(nulsrcp, parameterName);
    Expression *whatExp = newWhatExpression(nulsrcp, what);
    whatExp->class = classSymbol;
    return whatExp;
}
static void given_that_the_class_has_the_attribute(IdNode *classId, Attribute *attribute) {
    Symbol *symbol = classId->symbol;
    symbol->fields.entity.props = NEW(Properties);
    symbol->fields.entity.props->attributes = concat(NULL, attribute, ATTRIBUTE_LIST);
}


Ensure(Attribute, finds_restriction_for_parameter_from_context) {
    IdNode *parameterId = given_a_parameter_id_of_class("parameter", entitySymbol);
    IdNode *attributeId = given_an_attribute_id("atr");
    Attribute *atr = given_an_attribute(attributeId);
    IdNode *classId = given_a_class("someClass");
    Expression *exp = given_a_what_expression_of_class(parameterId, classId->symbol);
    Context *context = NULL;    /* Any will do since we have mocked it away */

    given_that_the_class_has_the_attribute(classId, atr);

    expect(contextRestrictsIdTo, will_return(classId->symbol));

    assert_that(resolveAttribute(exp, attributeId, context), is_equal_to(atr));
}
