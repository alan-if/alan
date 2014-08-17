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

BeforeEach(Symbol) {
    entitySymbol = newSymbol(newId(nulsrcp, "entity"), CLASS_SYMBOL);
    entitySymbol->fields.entity.parent = NULL;
    objectSymbol = newSymbol(newId(nulsrcp, "object"), CLASS_SYMBOL);
    objectSymbol->fields.entity.parent = entitySymbol;
    locationSymbol = newSymbol(newId(nulsrcp, "location"), CLASS_SYMBOL);
    locationSymbol->fields.entity.parent = entitySymbol;
    thingSymbol = newSymbol(newId(nulsrcp, "thing"), CLASS_SYMBOL);
    thingSymbol->fields.entity.parent = entitySymbol;

    literalSymbol = newSymbol(newId(nulsrcp, "literal"), CLASS_SYMBOL);
    thingSymbol->fields.entity.parent = entitySymbol;
    integerSymbol = newSymbol(newId(nulsrcp, "integer"), CLASS_SYMBOL);
    integerSymbol->fields.entity.parent = literalSymbol;
    stringSymbol = newSymbol(newId(nulsrcp, "string"), CLASS_SYMBOL);
    stringSymbol->fields.entity.parent = literalSymbol;
}

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
    assert_that(basicTypeFromClassSymbol(integerSymbol), is_equal_to(INTEGER_TYPE));
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

Ensure(Symbol, should_return_the_class_as_common_parent_for_same_class) {
    Symbol *parent = newClassSymbol(newId(nulsrcp, "class"), NULL, NULL);

    assert_that(commonParent(parent, parent), is_equal_to(parent));
}

Ensure(Symbol, should_return_the_parent_as_common_parent_if_one_is_child) {
    Symbol *parent = newClassSymbol(newId(nulsrcp, "parent"), NULL, NULL);
    Symbol *child = newClassSymbol(newId(nulsrcp, "child"), NULL, parent);

    assert_that(commonParent(parent, child), is_equal_to(parent));
    assert_that(commonParent(child, parent), is_equal_to(parent));
}

Ensure(Symbol, should_return_the_parent_as_common_parent_if_one_is_instance) {
    Symbol *parent = newClassSymbol(newId(nulsrcp, "class"), NULL, NULL);
    Symbol *instance = newInstanceSymbol(newId(nulsrcp, "instance"), NULL, parent);

    assert_that(commonParent(parent, instance), is_equal_to(parent));
    assert_that(commonParent(instance, parent), is_equal_to(parent));
}

Ensure(Symbol, should_return_the_common_parent_as_common_parent_if_both_have_same_parent) {
    Symbol *parent = newClassSymbol(newId(nulsrcp, "parent"), NULL, NULL);
    Symbol *child1 = newClassSymbol(newId(nulsrcp, "child1"), NULL, parent);
    Symbol *child2 = newClassSymbol(newId(nulsrcp, "child2"), NULL, parent);

    assert_that(commonParent(child1, child2), is_equal_to(parent));
    assert_that(commonParent(child2, child1), is_equal_to(parent));
}


static Properties *createContainerPropertiesTaking(Symbol *taken_class) {
    ContainerBody *containerBody = NEW(ContainerBody);
    containerBody->taking = newId(nulsrcp, "");
    containerBody->taking->symbol = taken_class;

    Container *container = NEW(Container);
    container->body = containerBody;

    return newProps(NULL, NULL, nulsrcp, NULL, NULL,
                    NULL, NULL, nulsrcp, NULL, NULL,
                    NULL, NULL, container, NULL, nulsrcp,
                    NULL, NULL, NULL);
}


static void createInstanceWithContainerTaking(char *name, Symbol *taken_class) {
    Properties *properties = createContainerPropertiesTaking(taken_class);
    newInstanceSymbol(newId(nulsrcp, name), properties, NULL);
}


static void createClassWithContainerTaking(char *name, Symbol *taken_class) {
    Properties *properties = createContainerPropertiesTaking(taken_class);
    newClassSymbol(newId(nulsrcp, name), properties, NULL);
}


Ensure(Symbol, can_figure_out_most_general_class_taken_by_any_container_instance) {
    /* If there are no containers it should return null */
    assert_that(find_contained_class(), is_null);

    /* If we add one instance that is a container it should return the class it takes */
    Symbol *taken_class = objectSymbol;
	createInstanceWithContainerTaking("o", taken_class);
    assert_that(find_contained_class(), is_equal_to(taken_class));

    /* If we add another taking a more abstract class it should return that */
    taken_class = thingSymbol;
    createInstanceWithContainerTaking("t", taken_class);
    assert_that(find_contained_class(), is_equal_to(taken_class));
}


Ensure(Symbol, can_figure_out_most_general_class_taken_by_any_container_class) {
    /* If there are no containers it should return null */
    assert_that(find_contained_class(), is_null);

    /* If we add one instance that is a container it should return the class it takes */
    Symbol *taken_class = objectSymbol;
	createClassWithContainerTaking("o", taken_class);
    assert_that(find_contained_class(), is_equal_to(taken_class));
}
