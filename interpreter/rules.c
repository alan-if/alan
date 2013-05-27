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
} RulesAdmin;

/* PRIVATE DATA: */
static int ruleCount;
static RulesAdmin *rulesAdmin; /* Table for administration of the rules */

/*----------------------------------------------------------------------*/
static void clearRulesAdmin(int ruleCount) {
    int r;
    for (r = 0; r < ruleCount; r++) {
        rulesAdmin[r].lastEval = FALSE;
        rulesAdmin[r].alreadyRun = FALSE;
    }
}


/*----------------------------------------------------------------------*/
static void initRulesAdmin(int ruleCount) {
    int r;

    rulesAdmin = allocate((ruleCount+1)*sizeof(RulesAdmin));
    for (r = 0; r < ruleCount; r++)
        ;
    setEndOfArray(&rulesAdmin[r]);
}


/*======================================================================*/
void initRules(Aaddr ruleTableAddress) {

    rules = (RuleEntry *) pointerTo(ruleTableAddress);

    if (ruleCount == 0) {       /* Not initiated */
        for (ruleCount = 0; !isEndOfArray(&rules[ruleCount]); ruleCount++)
            ;
        initRulesAdmin(ruleCount);
    }
    clearRulesAdmin(ruleCount);
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
        char *eval = evaluate(rules[i-1].exp)?"Evaluating to true":"Evaluating to false";
        if (!singleStepOption && !traceSourceOption) {
            traceRule(i, eval, "");
        } else {
            traceRule(i, eval, ":>\n");
        }
    }
}


/*----------------------------------------------------------------------*/
static void traceRuleExecution(int i) {
    if (sectionTraceOption) {
        if (!singleStepOption && !traceSourceOption)
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
static void evaluateRulesBeta2New(void) {
    int i;

    for (i = 1; !isEndOfArray(&rules[i-1]); i++)
        rules[i-1].alreadyRun = FALSE;

    current.location = NOWHERE;
    current.actor = 0;

    anyRuleRun = FALSE;

    for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
        bool evaluated_value = evaluate(rules[i-1].exp);
        traceRuleEvaluation(i);
        rules[i-1].alreadyRun = evaluated_value;
    }
    for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
        if (rules[i-1].alreadyRun) {
            traceRuleExecution(i);
            interpret(rules[i-1].stms);
            anyRuleRun = TRUE;
        }
    }
}


/*----------------------------------------------------------------------*/
/* This is how beta2 thought rules should be evaluated:
 */
static void evaluateRulesBeta2(void)
{
    bool change = TRUE;
    int i;

    for (i = 1; !isEndOfArray(&rules[i-1]); i++)
        rules[i-1].alreadyRun = FALSE;

    current.location = NOWHERE;
    current.actor = 0;

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
                    }
                    rulesAdmin[i-1].lastEval = triggered;
                } else {
                    rulesAdmin[i-1].lastEval = false;
                    if (sectionTraceOption && !singleStepOption)
                        printf(":>\n");
                }
	    }
    }
}


/*======================================================================*/
void resetRules() {
    int i;
    for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
        rulesAdmin[i-1].alreadyRun = FALSE;

    }
}


/*======================================================================*/
void evaluateRules(RuleEntry rules[]) {
    bool change = TRUE;
    int i;

    current.location = NOWHERE;
    current.actor = 0;

    while (change) {
        change = FALSE;
        for (i = 1; !isEndOfArray(&rules[i-1]); i++) {
            traceRuleEvaluation(i);
            bool evaluated_value = evaluate(rules[i-1].exp);
            if (evaluated_value == true && rulesAdmin[i-1].lastEval == false
                && !rulesAdmin[i-1].alreadyRun) {
                change = TRUE;
                traceRuleExecution(i);
                interpret(rules[i-1].stms);
                rulesAdmin[i-1].alreadyRun = TRUE;
                anyRuleRun = TRUE;
            } else {
                if (sectionTraceOption && !(singleStepOption || traceSourceOption))
                    printf(":>\n");
            }
            rulesAdmin[i-1].lastEval = evaluated_value;
        }
    }
}


/*=======================================================================*/
void resetAndEvaluateRules(RuleEntry rules[], char *version) {
    if (isPreBeta2(version))
        evaluateRulesPreBeta2();
    else if (isPreBeta3(version))
        evaluateRulesBeta2();
    else {
        resetRules();
        evaluateRules(rules);
    }
}
