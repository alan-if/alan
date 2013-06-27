#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "rules.h"

#include "lists.h"

/* Mocked modules */
#include "options.mock"
#include "inter.mock"
#include "output.mock"
#include "compatibility.mock"
#include "debug.mock"
#include "utils.mock"
#include "current.mock"
#include "syserr.mock"

static Aaddr address_to_rules;

#define MAX_NO_OF_RULES 100

static Aword *allocate_memory() {
    return (Aword *)allocate(sizeof(ACodeHeader)+(MAX_NO_OF_RULES+1)*sizeof(RuleEntry));
}

static RuleEntry *setup_rules(int count) {
    RuleEntry *rules = pointerTo(memory[sizeof(ACodeHeader)]);
    int i;
    for (i = 0; i < count; i++) {
        int rule_no = 10*i;
        rules[i].exp = rule_no+1;
        rules[i].stms = rule_no+2;
    }
    setEndOfArray(&memory[address_to_rules + count*sizeof(RuleEntry)]);
    return (RuleEntry *)pointerTo(sizeof(ACodeHeader));
}

Describe(Rules);

BeforeEach(Rules) {
    memory = allocate_memory();
}

AfterEach(Rules) {}


Ensure(Rules, dont_run_if_no_rules) {
    never_expect(evaluate);
    rules = setup_rules(0);

    initRules(address_to_rules);
    evaluateRules(rules);
}

Ensure(Rules, dont_run_if_one_rule_evaluate_to_false) {
    never_expect(interpret);
    expect(evaluate, when(adr, is_equal_to(1)), will_return(false));

    rules = setup_rules(1);

    initRules(address_to_rules);
    evaluateRules(rules);
}
