#include "cgreen/cgreen.h"

#include "rules.c"


static Stack stack;

Describe(Rules);
BeforeEach(Rules) {
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
AfterEach(Rules) {}

Ensure(Rules, canInitRulesAdmin) {

    initRules();

    assert_that(rulesAdmin, is_non_null);
    assert_that(rulesAdmin[0].exp, is_equal_to(rules[0].exp));
    assert_that(rulesAdmin[0].stms, is_equal_to(rules[0].stms));
}


static void interpretAndReturnFalse(Aaddr adr) {
    push(stack, false);
}

Ensure(Rules, setsLastEvalToFalseForRulesEvaluatingToFalse) {
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

Ensure(Rules, setsLastEvalToTrueAndExecutesRulesEvaluatingToTrueWithLastEvalFalse) {
    initRules();
    setInterpreterMock(interpretAndReturnTrueIfEval);
    interpreterExecuted = false;

    evaluateRules();

    assert_that(rulesAdmin[0].lastEval, is_true);
    assert_that(interpreterExecuted);
}


Ensure(Rules, dontExecuteRulesEvaluatingToTrueWithLastEvalTrue) {
    initRules();
    setInterpreterMock(interpretAndReturnTrueIfEval);
    interpreterExecuted = false;

    rulesAdmin[0].lastEval = true;

    evaluateRules();

    assert_that(rulesAdmin[0].lastEval, is_true);
    assert_that(interpreterExecuted, is_false);
}

Ensure(Rules, canClearRulesAdmin) {
    initRules();
    rulesAdmin[0].alreadyRun = TRUE;
    clearRulesAdmin();
    assert_that(rulesAdmin[0].alreadyRun, is_false);
}

TestSuite *rulesTests(void)
{
    TestSuite *suite = create_test_suite();

    add_test_with_context(suite, Rules, canInitRulesAdmin);
    add_test_with_context(suite, Rules, setsLastEvalToFalseForRulesEvaluatingToFalse);
    add_test_with_context(suite, Rules, setsLastEvalToTrueAndExecutesRulesEvaluatingToTrueWithLastEvalFalse);
    add_test_with_context(suite, Rules, dontExecuteRulesEvaluatingToTrueWithLastEvalTrue);

    return suite;
}
