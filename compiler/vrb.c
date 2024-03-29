/*----------------------------------------------------------------------

  VRB.C
  Verb Nodes

  ----------------------------------------------------------------------*/

#include "vrb_x.h"

/* USE: */
#include "srcp_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "id_x.h"
#include "adv_x.h"
#include "stx_x.h"
#include "context_x.h"
#include "dump_x.h"

#include "lmlog.h"
#include "util.h"
#include "alt.h"
#include "ins.h"

#include "acode.h"
#include "emit.h"


/* PUBLIC: */



/*======================================================================*/
Verb *newVerb(Srcp *srcp, List *ids, List *alts, bool meta)
{
    Verb *new;			/* The newly allocated area */
    Symbol *sym;

    progressCounter();

    new = NEW(Verb);

    new->srcp = *srcp;
    new->ids = ids;
    new->meta = meta;
    new->alternatives = alts;

    for (List *l = ids; l != NULL; l = l->next) {
        Id *id = l->member.id;
        sym = lookup(id->string); /* Find earlier definition */
        if (sym == NULL) {
            id->symbol = newVerbSymbol(id);
            id->symbol->fields.verb.meta = meta;
            id->code = id->symbol->code;
        } else if (sym->kind == VERB_SYMBOL) {
            id->symbol = sym;
            id->code = sym->code;
            id->symbol->fields.verb.meta |= new->meta;
        } else
            idRedefined(id, sym, sym->srcp);
    }

    /* Use first verb symbol as context symbol */
    new->symbol = ids->member.id->symbol;

    return new;
}


/*----------------------------------------------------------------------*/
static void analyzeVerb(Verb *theVerb, Context *previousContext)
{
    List *syntaxLists = NULL;
    Syntax *stx;
    Context *context = pushContext(previousContext);

    progressCounter();

    /* First find the syntax definitions for all verbs */
    for (List *ids = theVerb->ids; ids; ids = ids->next) {
        stx = NULL;
        for (List *lst = adv.stxs; lst; lst = lst->next) {
            if (lst->member.stx->id->symbol != NULL && ids->member.id->symbol != NULL)
                if (lst->member.stx->id->symbol->code == ids->member.id->symbol->code) {
                    stx = lst->member.stx;
                    break;
                }
        }
        if (stx == NULL) {
            /* Define a default syntax for the verb */
            if (!inEntityContext(context)) {
                /* A global, no parameter, verb */
                lmlog(&ids->member.id->srcp, 230, sevINF, ids->member.id->string);
                stx = defaultSyntax0(ids->member.id->string);
            } else {
                Id *className = classIdInContext(context);
                if (className == NULL)
                    className = newId(nulsrcp, "object");
                lmlogv(&ids->member.id->srcp, 231, sevINF, ids->member.id->string,
                       className->string, NULL);
                stx = defaultSyntax1(ids->member.id, context);
            }
        }
        syntaxLists = concat(syntaxLists, stx, SYNTAX_LIST);
    }
    stx = syntaxLists->member.stx;	/* Use first syntax */
    theVerb->stx = stx;

    /* Check compatible parameter lists for all the verbs? */
    List *ids = theVerb->ids->next;
    for (List *lst = syntaxLists->next; lst != NULL; lst = lst->next) {
        if (!equalParameterLists(stx, lst->member.stx))
            lmlog(&ids->member.id->srcp, 215, sevERR,
                  theVerb->ids->member.id->string);
        ids = ids->next;
    }

    if (!inEntityContext(context)) {
        /* No alternatives allowed in global verb definition */
        if (theVerb->alternatives->member.alt->id != NULL)
            lmlog(&theVerb->alternatives->member.alt->srcp, 213, sevERR, "");
        /* No parameters allowed in global verb definition */
        if (syntaxLists->member.stx->parameters != NULL)
            lmlog(&theVerb->srcp, 219, sevERR, "");
    }

    /* TODO - Warn if no ALT for every parameter in the defined syntax */

    context->kind = VERB_CONTEXT;
    if (stx != NULL) {
        context->verb = theVerb->symbol;
        analyzeAlternatives(theVerb->alternatives, context);
    } else
        analyzeAlternatives(theVerb->alternatives, context);
}


/*======================================================================*/
bool verbIdFound(Id *targetId, List *verbs)
{
    for (List *theVerb = verbs; theVerb != NULL; theVerb = theVerb->next) {
        for (List *theIdInList = theVerb->member.vrb->ids; theIdInList != NULL; theIdInList = theIdInList->next)
            if (findIdInList(targetId, theIdInList) != NULL)
                return true;
    }
    return false;
}


/*----------------------------------------------------------------------*/
static void checkMultipleVerbDeclarations(List *verbs)
{
    Id *foundId;

    for (List *thisVerbDeclaration = verbs; thisVerbDeclaration != NULL; thisVerbDeclaration = thisVerbDeclaration->next) {
        for (List *firstId = thisVerbDeclaration->member.vrb->ids; firstId != NULL; firstId = firstId->next) {
            if ((foundId = findIdInList(firstId->member.id, firstId->next)) != NULL)
                lmlogv(&foundId->srcp, 201, sevWAR, "verb", foundId->string, "in this VERB declaration", NULL);
            /* Then the names in the other VERBs */
            for (List *otherVerbs = thisVerbDeclaration->next; otherVerbs != NULL; otherVerbs = otherVerbs->next) {
                if ((foundId = findIdInList(firstId->member.id, otherVerbs->member.vrb->ids)) != NULL)
                    lmlogv(&foundId->srcp, 201, sevWAR, "verb", foundId->string, "in this class/instance. Duplicate in a previous VERB clause", NULL);
            }
        }
    }
}



/*======================================================================  */
void analyzeVerbs(List *verbs, Context *context)
{
    for (List *verb = verbs; verb != NULL; verb = verb->next)
        analyzeVerb(verb->member.vrb, context);

    checkMultipleVerbDeclarations(verbs);
}



/*----------------------------------------------------------------------*/
static void generateVerb(Verb *vrb)
{
    progressCounter();

    if (vrb->alternatives == NULL)
        vrb->altAddress = 0;
    else
        vrb->altAddress = generateAlternatives(vrb->alternatives);
}


/*----------------------------------------------------------------------*/
static int metaVerbCode(int code) {
    return -code-1;
}


/*----------------------------------------------------------------------*/
static void emitVerbCode(bool meta, int code) {
    if (meta)
        emit(metaVerbCode(code));
    else
        emit(code);
}

/*----------------------------------------------------------------------*/
static void generateVerbEntry(Verb *vrb)
{
    for (List *ids = vrb->ids; ids != NULL; ids = ids->next) {
        emitVerbCode(vrb->symbol->fields.verb.meta, ids->member.id->code);
        emit(vrb->altAddress);
    }
}


/*======================================================================*/
Aaddr generateVerbs(List *vrbs)
{
    Aaddr vrbadr;			/* Address to alt-table */

    if (vrbs == NULL)
        return 0;

    /* First generate action procedures for all verbs */
    for (List *lst = vrbs; lst != NULL; lst = lst->next)
        generateVerb(lst->member.vrb);

    /* and then the verb table */
    vrbadr = nextEmitAddress();
    for (List *lst = vrbs; lst != NULL; lst = lst->next)
        generateVerbEntry(lst->member.vrb);
    emit(EOF);

    return vrbadr;
}



/*======================================================================*/
void dumpVerb (Verb *vrb)
{
    if (vrb == NULL) {
        put("NULL");
        return;
    }

    put("VRB: "); dumpSrcp(vrb->srcp); indent();
    put("meta: "); dumpBool(vrb->meta); nl();
    put("ids: "); dumpList(vrb->ids, ID_LIST); nl();
    put("altadr: "); dumpAddress(vrb->altAddress); nl();
    put("alts: "); dumpList(vrb->alternatives, ALTERNATIVE_LIST); out();
}
