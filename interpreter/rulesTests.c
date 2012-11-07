#include "cgreen/cgreen.h"

#include "rules.c"


static Stack stack;


static void setUp() {
    stack = createStack(10);
    setInterpreterStack(stack);
    memory = allocate(100*sizeof(Aword));
    header->ruleTableAddress = 5;

    RuleEntry *rulEntry = (RuleEntry *)&memory[5];
    rulEntry->exp = 77;
    rulEntry->stms = 78;
    rulEntry->alreadyRun = false;

    rulEntry++;
    setEndOfArray(rulEntry);
}

Ensure (initRulesInitsRulesAdmin) {

    initRules();

    assert_that(rulesAdmin, is_non_null);
    assert_that(rulesAdmin[0].exp, is_equal_to(rules[0].exp));
    assert_that(rulesAdmin[0].stms, is_equal_to(rules[0].stms));
}


static void interpretAndReturnFalse(Aaddr adr) {
    push(stack, false);
}

Ensure (rulesEvaluatingToFalseSetsLastEvalToFalse) {
    initRules();
    setInterpreterMock(interpretAndReturnFalse);
    rulesAdmin[0].lastEval = true;

    evaluateRules();

    assert_that(rulesAdmin[0].lastEval, is_false);
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

    assert_that(rulesAdmin[0].lastEval, is_true);
    assert_that(interpreterExecuted);
}


Ensure (rulesEvaluatingToTrueWithLastEvalTrueDontExecute) {
    initRules();
    setInterpreterMock(interpretAndReturnTrueIfEval);
    interpreterExecuted = false;

    rulesAdmin[0].lastEval = true;

    evaluateRules();

    assert_that(rulesAdmin[0].lastEval, is_true);
    assert_that(interpreterExecuted, is_false);
}

Ensure(canClearRulesAdmin) {
    rulesAdmin[0].alreadyRun = TRUE;
    clearRulesAdmin();
    assert_that(rulesAdmin[0].alreadyRun, is_false);
}

TestSuite *rulesTests(void)
{
    TestSuite *suite = create_test_suite();

    set_setup(suite, setUp);

    add_test(suite, initRulesInitsRulesAdmin);
    add_test(suite, rulesEvaluatingToFalseSetsLastEvalToFalse);
    add_test(suite, rulesEvaluatingToTrueWithLastEvalFalseSetsLastEvalToTrueAndExecutes);
    add_test(suite, rulesEvaluatingToTrueWithLastEvalTrueDontExecute);

    return suite;
}
