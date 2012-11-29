/*----------------------------------------------------------------------*\

  rules.c

  Rule handling unit of Alan interpreter module, ARUN.

\*----------------------------------------------------------------------*/

#include "types.h"
#include "rules.h"


/* IMPORTS */
#include "lists.h"
#include "inter.h"
#include "debug.h"
#include "current.h"
#include "options.h"
#include "compatibility.h"

#ifdef HAVE_GLK
#include "glkio.h"
#endif


/* PUBLIC DATA: */
RuleEntry *rules;         /* Rule table pointer */
bool anyRuleRun;


/* PRIVATE TYPES: */
typedef struct RulesAdmin {
    bool lastEval;
    bool alreadyRun;
    Aaddr exp;
    Aaddr stms;
} RulesAdmin;

/* PRIVATE DATA: */
static int ruleCount;
static RulesAdmin *rulesAdmin; /* Table for administration of the rules */

/*----------------------------------------------------------------------*/
static void clearRulesAdmin() {
    int r;
    for (r = 0; r < ruleCount; r++) {
        rulesAdmin[r].lastEval = FALSE;
        rulesAdmin[r].alreadyRun = FALSE;
        rulesAdmin[r].exp = rules[r].exp;
        rulesAdmin[r].stms = rules[r].stms;
    }
}


/*======================================================================*/
void initRules() {

    rules = (RuleEntry *) pointerTo(header->ruleTableAddress);

    if (ruleCount == 0) {       /* Not initiated */
        for (ruleCount = 0; !isEndOfArray(&rules[ruleCount]); ruleCount++);
        rulesAdmin = allocate(ruleCount*sizeof(RulesAdmin));
	clearRulesAdmin();
    }
}


/*----------------------------------------------------------------------*/
static void traceRule(int i, char *what, char *tail) {
    printf("\n<RULE %d", i);
    if (current.location != 0) {
	printf(" (at ");
	traceSay(current.location);
    } else
	printf(" (nowhere");
    printf("[%d]), %s%s", current.location, what, tail);
}


/*----------------------------------------------------------------------*/
static void traceRuleEvaluation(int i) {
    if (sectionTraceOption) {
	if (!singleStepOption) {
	    traceRule(i, "Evaluating", "");
	} else {
	    traceRule(i, "Evaluating", ":>\n");
	}
    }
}


/*----------------------------------------------------------------------*/
static void traceRuleExecution(int i) {
    if (sectionTraceOption) {
	if (!singleStepOption)
	    printf(", Executing:>\n");
	else {
	    traceRule(i, "Executing:>\n", "");
	}
    }
}



/*----------------------------------------------------------------------*/
static void evaluateRulesPreBeta2(void)
{
    bool change = TRUE;
    int i;

    for (i = 1; !isEndOfArray(&rules[i-1]); i++)
	rules[i-1].alreadyRun = FALSE;

    while (change) {
	change = FALSE;
	for (i = 1; !isEndOfArray(&rules[i-1]); i++)
	    if (!rules[i-1].alreadyRun) {
		traceRuleEvaluation(i);
		if (evaluate(rules[i-1].exp)) {
		    change = TRUE;
		    rules[i-1].alreadyRun = TRUE;
		    traceRuleExecution(i);
		    interpret(rules[i-1].stms);
		} else if (sectionTraceOption && !singleStepOption)
		    printf(":>\n");
	    }
    }
}


/*----------------------------------------------------------------------*/
static void evaluateRulesBeta2Onwards(void)
{
    bool change = TRUE;
    int i;

    for (i = 1; !isEndOfArray(&rules[i-1]); i++)
	rules[i-1].alreadyRun = FALSE;

    current.location = NOWHERE;
    current.actor = 0;

    anyRuleRun = FALSE;

#ifndef OLD
    while (change) {
	change = FALSE;
	for (i = 1; !isEndOfArray(&rules[i-1]); i++)
	    if (!rules[i-1].alreadyRun) {
                bool triggered = evaluate(rules[i-1].exp);
		traceRuleEvaluation(i);
		if (triggered) {
                    if (rulesAdmin[i-1].lastEval == false) {
                        change = TRUE;
                        rules[i-1].alreadyRun = TRUE;
                        traceRuleExecution(i);
                        interpret(rules[i-1].stms);
						anyRuleRun = TRUE;
                    }
                    rulesAdmin[i-1].lastEval = triggered;
		} else {
                    rulesAdmin[i-1].lastEval = false;
                    if (sectionTraceOption && !singleStepOption)
                        printf(":>\n");
                }
			}
    }
#else
	for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
		bool triggered = evaluate(rules[i-1].exp);
		traceRuleEvaluation(i);
		rules[i-1].run = triggered;
	}
	for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
		if (rules[i-1].run) {
			traceRuleExecution(i);
			interpret(rules[i-1].stms);
			anyRuleRun = TRUE;
		}
	}
#endif
}


/*=======================================================================*/
void evaluateRules(void) {
    if (isPreBeta2(header->version))
        evaluateRulesPreBeta2();
    else
        evaluateRulesBeta2Onwards();
}
