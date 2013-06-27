
#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "rules.c"

Describe(Rules);

#define VERSION "\3\0\3b" // 3.0beta3

static Stack stack;

static void interpret_and_return_false(Aaddr adr) {
    push(stack, false);
}

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

    initRules(header->ruleTableAddress);

    setInterpreterMock(interpret_and_return_false);
    resetRules();
}

AfterEach(Rules) {
}


Ensure(Rules, canClearRulesAdmin) {
    rulesAdmin[0].alreadyRun = TRUE;
    clearRulesAdmin(ruleCount);
    assert_that(rulesAdmin[0].alreadyRun, is_false);
}


static void interpreter_mock(Aaddr adr) {
    push(stack, (Aword)mock(adr));
}

Ensure(Rules, sets_last_eval_to_false_for_rules_evaluating_to_false) {
    setInterpreterMock(interpreter_mock);

    always_expect(interpreter_mock, will_return(false));
    rulesAdmin[0].lastEval = true;

    resetAndEvaluateRules(rules, VERSION);

    assert_that(rulesAdmin[0].lastEval, is_false);
}

Ensure(Rules, setsLastEvalToTrueForRulesEvaluatingToTrue) {
    setInterpreterMock(interpreter_mock);

    expect(interpreter_mock, when(adr, is_equal_to(rules[0].exp)), will_return(true));
    expect(interpreter_mock, when(adr, is_equal_to(rules[0].stms)));
    always_expect(interpreter_mock);

    rulesAdmin[0].lastEval = false;

    resetAndEvaluateRules(rules, VERSION);

    assert_that(rulesAdmin[0].lastEval, is_true);
}


static bool rule0_statements_have_been_executed = false;
static void interpret_and_return_true_if_evaluated(Aaddr adr) {
    if (adr == rules[0].exp)
        push(stack, true);
    else if (adr == rules[0].stms)
        rule0_statements_have_been_executed = true;
}

Ensure(Rules, sets_last_eval_to_true_and_executes) {
    setInterpreterMock(interpreter_mock);

    expect(interpreter_mock, when(adr, is_equal_to(rules[0].exp)), will_return(true));
    expect(interpreter_mock, when(adr, is_equal_to(rules[0].stms)), will_return(0));
    expect(interpreter_mock);

    evaluateRules(rules);

    assert_that(rulesAdmin[0].lastEval, is_true);
}


Ensure(Rules, dontExecuteStatementsForRulesWithLastEvalTrue) {
    setInterpreterMock(interpret_and_return_true_if_evaluated);

    rule0_statements_have_been_executed = false;

    rulesAdmin[0].lastEval = true;

    evaluateRules(rules);
    assert_that(rule0_statements_have_been_executed, is_false);
}

Ensure(Rules, executes_statements_for_a_rule_triggered_again_after_reset) {

    setInterpreterMock(interpret_and_return_true_if_evaluated);

    rule0_statements_have_been_executed = false;
    evaluateRules(rules);
    assert_that(rule0_statements_have_been_executed);

    resetRules();

    rule0_statements_have_been_executed = false;
    setInterpreterMock(interpret_and_return_true_if_evaluated);
    evaluateRules(rules);
    assert_that(rule0_statements_have_been_executed);
}

Ensure(Rules, dont_execute_statements_for_a_rule_triggered_a_second_time_without_being_reset) {
    setInterpreterMock(interpret_and_return_true_if_evaluated);

    rule0_statements_have_been_executed = false;
    evaluateRules(rules);
    assert_that(rule0_statements_have_been_executed);

    rulesAdmin[0].lastEval = false; /* Simulate another evaluation returning false */

    rule0_statements_have_been_executed = false;
    evaluateRules(rules);
    assert_that(rule0_statements_have_been_executed, is_false);
}

