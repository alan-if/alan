#include "cgreen/cgreen.h"
#include "cgreen/mocks.h"

#include "rules.c"


static Stack stack;


static void setUp() {
    stack = createStack(10);
    setInterpreterStack(stack);
    memory = allocate(100*sizeof(Aword));
    header->ruleTableAddress = 5;

    RuleEntry *rulPtr = (RuleEntry *)&memory[5];
    rulPtr->exp = 77;
    rulPtr->stms = 78;
    rulPtr->run = false;

    rulPtr++;
    setEndOfArray(rulPtr);
}

Ensure (initRulesAllocatesLastEvalArray) {

    initRules();

    assert_not_equal(rulesLastEval, NULL);
}


static void interpretAndReturnFalse(Aaddr adr) {
    push(stack, false);
}

Ensure (rulesEvaluatingToFalseSetsLastEvalToFalse) {
    initRules();
    setInterpreterMock(interpretAndReturnFalse);
    rulesLastEval[0] = true;

    evaluateRules();

    assert_equal(rulesLastEval[0], false);
}


static bool interpreterExecuted = false;
static void interpretAndReturnTrueIfEval(Aaddr adr) {
    if (adr == rules[0].exp)
        push(stack, true);
    else
        interpreterExecuted = true;
}

Ensure (rulesEvaluatingToTrueWithLastEvalFalseSetsLastEvalToTrueAndExecutes) {
    initRules();
    setInterpreterMock(interpretAndReturnTrueIfEval);
    interpreterExecuted = false;

    evaluateRules();

    assert_equal(rulesLastEval[0], true);
    assert_true(interpreterExecuted);
}


Ensure (rulesEvaluatingToTrueWithLastEvalTrueDontExecute) {
    initRules();
    setInterpreterMock(interpretAndReturnTrueIfEval);
    interpreterExecuted = false;

    rulesLastEval[0] = true;

    evaluateRules();

    assert_equal(rulesLastEval[0], true);
    assert_false(interpreterExecuted);
}


TestSuite *rulesTests(void)
{
    TestSuite *suite = create_test_suite();

    set_setup(suite, setUp);

    add_test(suite, initRulesAllocatesLastEvalArray);
    add_test(suite, rulesEvaluatingToFalseSetsLastEvalToFalse);
    add_test(suite, rulesEvaluatingToTrueWithLastEvalFalseSetsLastEvalToTrueAndExecutes);
    add_test(suite, rulesEvaluatingToTrueWithLastEvalTrueDontExecute);

    return suite;
}
