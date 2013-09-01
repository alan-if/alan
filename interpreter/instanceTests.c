#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "instance.c"

Describe(Instance);

BeforeEach(Instance) {}
AfterEach(Instance) {}

static ACodeHeader localHeader;

void setUp() {
    header = &localHeader;
    literals = NULL;
}


void tearDown() {
    free(literals);
}


static void given_a_literal_table_of(int size) {
    literals = allocate(sizeof(LiteralEntry)*(size+1));
}

static void given_that_literal_has_value(int literal_number, int value) {
    literals[literal_number].value = value;
}


Ensure(Instance, canAccessLiteralValue) {
    int value = 45;
    int literal_number = 1;
    
    int attribute_number_for_literal_value = 0;

    given_a_literal_table_of(1);
    given_that_literal_has_value(1, value);

    assert_equal(value, literalAttribute(instanceFromLiteral(literal_number), attribute_number_for_literal_value));
}

TestSuite *instanceTests(void)
{
    TestSuite *suite = create_test_suite();

    set_setup(suite, setUp);
    
    add_test_with_context(suite, Instance, canAccessLiteralValue);

    set_teardown(suite, tearDown);

    return suite;
}
