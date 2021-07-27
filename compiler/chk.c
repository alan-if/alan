/*----------------------------------------------------------------------*\

  CHK.C
  Check Nodes

\*----------------------------------------------------------------------*/

#include "chk_x.h"

/* USE: */

#include "lst_x.h"
#include "exp_x.h"
#include "stm_x.h"
#include "dump_x.h"

#include "util.h"
#include "options.h"
#include "emit.h"
#include "acode.h"




/*======================================================================*/
CheckNode *newCheck(Expression *exp, /* IN - Expression for this CHECK */
                    List *stms)	/* IN - Statements for a false CHECK */
{
    CheckNode *new;                     /* The newly allocated area */

    progressCounter();

    new = NEW(CheckNode);

    new->exp = exp;
    new->stms = stms;

    return(new);
}


/*----------------------------------------------------------------------*/
static void anchk(CheckNode *chk,
                  Context *context)
{
    analyzeExpression(chk->exp, context);
    analyzeStatements(chk->stms, context);
}



/*======================================================================*/
void analyzeChecks(List *chks,
                   Context *context)
{
    while (chks != NULL) {
        anchk(chks->member.chk, context);
        chks = chks->next;
    }
}




/*======================================================================*/
Aword generateChecks(List *chks)
{
    Aword tbladr;			/* Save ACODE address to check table */

    if (chks == NULL) return 0;

    /* First checks */
    if (chks->member.chk->exp == NULL) { /* An unconditional CHECK */
        chks->member.chk->expadr = 0;
        chks->member.chk->stmadr = nextEmitAddress();
        generateStatements(chks->member.chk->stms);
        emit0(I_RETURN);
    } else
        for (List *l = chks; l != NULL; l = l->next) {
            l->member.chk->expadr = nextEmitAddress();
            generateExpression(l->member.chk->exp);
            emit0(I_RETURN);
            l->member.chk->stmadr = nextEmitAddress();
            generateStatements(l->member.chk->stms);
            emit0(I_RETURN);
        }

    /* Then generate a check table */
    tbladr = nextEmitAddress();
    for (List *l = chks; l != NULL; l = l->next) {
        emit(l->member.chk->expadr);
        emit(l->member.chk->stmadr);
    }
    emit(EOF);

    return(tbladr);
}



/*======================================================================*/
void dumpCheck(CheckNode *chk)
{
    if (chk == NULL) {
        put("NULL");
        return;
    }

    put("CHECK: "); indent();
    put("exp: "); dumpExpression(chk->exp); nl();
    put("expadr: "); dumpAddress(chk->expadr); nl();
    put("stms: "); dumpList(chk->stms, STATEMENT_LIST); nl();
    put("stmadr: "); dumpAddress(chk->stmadr); out();
}
