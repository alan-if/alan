/*----------------------------------------------------------------------*\

  EXT.C
  Exit Nodes

  \*----------------------------------------------------------------------*/

#include "ext_x.h"

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "chk_x.h"
#include "wrd_x.h"
#include "dump_x.h"

#include "elm.h"                /* ELM-nodes */

#include "emit.h"
#include "lmlog.h"
#include "acode.h"


/* PUBLIC: */

int dirmin, dirmax;
int dircount = 0;



/*======================================================================*/
Exit *newExit(Srcp *srcp, List *dirs, Id *target, List *chks, List *stms)
{
    Exit *new;			/* The newly created node */
    Symbol *sym;

    progressCounter();

    new = NEW(Exit);

    new->srcp = *srcp;
    new->directions = dirs;
    new->target = target;
    new->chks = chks;
    new->stms = stms;

    for (List *lst = dirs; lst != NULL; lst = lst->next) {
        sym = lookup(lst->member.id->string); /* Find any earlier definition */
        if (sym == NULL) {
            lst->member.id->symbol = newSymbol(lst->member.id, DIRECTION_SYMBOL);
            lst->member.id->code = lst->member.id->symbol->code;
            newDirectionWord(lst->member.id->string, lst->member.id->symbol->code);
        } else if (sym->kind == DIRECTION_SYMBOL) {
            lst->member.id->symbol = sym;
            lst->member.id->code = lst->member.id->symbol->code;
        } else
            idRedefined(lst->member.id, sym, sym->srcp);
    }

    return(new);
}



/*-----------------------------------------------------------------------*/
static void symbolizeExit(Exit *theExit)
{
    symbolizeId(theExit->target);
#ifdef FIXME // Why?
    symbolizeChecks(theExit->checks);
    symbolizeStatements(theExit->does);
#endif
}


/*======================================================================*/
void symbolizeExits(List *theExitList)
{
    for (List *lst = theExitList; lst != NULL; lst = lst->next)
        symbolizeExit(lst->member.ext);
}


/*======================================================================*/
void analyzeExit(Exit *ext, Context *context)
{
    instanceCheck(ext->target, "Target of an Exit", "location");

    analyzeChecks(ext->chks, context);
    analyzeStatements(ext->stms, context);
}



/*======================================================================*/
bool exitIdFound(Id *targetId, List *exits)
{
    for (List *theExit = exits; theExit != NULL; theExit = theExit->next) {
        for (List *theIdInList = theExit->member.ext->directions; theIdInList != NULL; theIdInList = theIdInList->next)
            if (findIdInList(targetId, theIdInList) != NULL)
                return true;
    }
    return false;
}


/*======================================================================*/
void analyzeExits(List *exts, Context *context)
{
    for (List *lst = exts; lst != NULL; lst = lst->next)
        analyzeExit(lst->member.ext, context);

    /* Check for multiple definitions of a direction */
    for (List *ext = exts; ext != NULL; ext = ext->next) {
        List *dir = ext->member.ext->directions;
        /* First check other directions in this EXIT */
        for (List *other = dir->next; other != NULL; other = other->next) {
            if (other->member.id->symbol != NULL && dir->member.id->symbol != NULL)
                if (other->member.id->symbol->code == dir->member.id->symbol->code) {
                    lmlog(&other->member.id->srcp, 202, sevWAR,
                          other->member.id->string);
                    break;
                }
        }
        /* Then the directions in the other EXITs */
        for (List *lst = ext->next; lst != NULL; lst = lst->next) {
            for (List *other = lst->member.ext->directions; other != NULL; other = other->next)
                if (other->member.id->symbol && dir->member.id->symbol)
                    /* Not error symbols... */
                    if (other->member.id->symbol->code == dir->member.id->symbol->code) {
                        lmlog(&other->member.id->srcp, 203, sevWAR,
                              other->member.id->string);
                        break;
                }
        }
    }
}


/*----------------------------------------------------------------------*/
static bool haveExit(List *ownExits, Id *direction) {
    List *exits;
    List *directions;

    ITERATE(exits, ownExits) {
        ITERATE(directions, exits->member.ext->directions) {
            if (equalId(directions->member.id, direction))
                return true;
        }
    }
    return false;
}

/*----------------------------------------------------------------------*/
static Exit *copyExitExcludingOwn(Exit *original, List *ownExits) {
    List *directionsToCopy = NULL;
    List *direction;

    ITERATE (direction, original->directions)
        if (!haveExit(ownExits, direction->member.id))
            directionsToCopy = concat(directionsToCopy, direction->member.id, ID_LIST);
    return newExit(&original->srcp, directionsToCopy, original->target,
                   original->chks, original->stms);
}


/*======================================================================*/
List *combineExits(List *ownExits, List *exitsToAdd)
{
    /* Insert all exits from the list to add that are not there
       already.
    */
    List *toAdd;
    List *direction;
    List *new = NULL;

    ITERATE(toAdd, exitsToAdd) {
        bool foundOneToAdd = false;
        /* Each exit may have multiple directions so we must traverse that
           list to see if we should copy this Exit node */
        ITERATE(direction, toAdd->member.ext->directions) {
            if (!haveExit(ownExits, direction->member.id)) {
                foundOneToAdd = true;
                break;
            }
        }
        if (foundOneToAdd)
            new = concat(new, copyExitExcludingOwn(toAdd->member.ext, ownExits), EXIT_LIST);
    }
    return combine(ownExits, new);
}


/*----------------------------------------------------------------------*/
static Aaddr generateExitStatements(Exit *ext)
{
    Aaddr stmadr = nextEmitAddress();

    if (ext->stms == NULL)
        return(0);

    generateStatements(ext->stms);
    emit0(I_RETURN);
    return(stmadr);
}



/*----------------------------------------------------------------------

  generateExitEntry()

  Generate one exit entry in the exit table.

*/
static void generateExitEntry(Exit *ext) /* IN - The exit to generate */
{
    ExitEntry entry;

    for (List *dir = ext->directions; dir != NULL; dir = dir->next) {
        entry.code = dir->member.id->symbol->code;
        entry.checks = ext->chks? ext->chkadr : 0;
        entry.action = ext->stms? ext->stmadr : 0;
        entry.target = ext->target->symbol->code;
        emitEntry(&entry, sizeof(entry));
    }
}



/*======================================================================*/
Aaddr generateExits(List *exits)
{
    Aaddr extadr;               /* The adress where the exits start */

    if (exits == NULL)
        return(0);

    for (List *l = exits; l != NULL; l = l->next) {
        l->member.ext->chkadr = generateChecks(l->member.ext->chks);
        l->member.ext->stmadr = generateExitStatements(l->member.ext);
        emit0(I_RETURN);
    }

    extadr = nextEmitAddress();
    for (List *l = exits; l != NULL; l = l->next)
        generateExitEntry(l->member.ext);
    emit(EOF);
    return(extadr);
}




/*======================================================================

  dumpExit()

  Dump an Exit node.

*/
void dumpExit(Exit *ext)
{
    if (ext == NULL) {
        put("NULL");
        return;
    }

    put("EXT: "); dumpSrcp(ext->srcp); indent();
    put("dirs: "); dumpList(ext->directions, ID_LIST); nl();
    put("target: "); dumpId(ext->target); nl();
    put("chks: "); dumpList(ext->chks, CHECK_LIST); nl();
    put("chkadr: "); dumpAddress(ext->chkadr); nl();
    put("stms: "); dumpList(ext->stms, STATEMENT_LIST); nl();
    put("stmadr: "); dumpAddress(ext->stmadr); out();
}


/*======================================================================*/
void xmlExit(Exit *exit, FILE* xmlFile)
{
    List *direction;
    ITERATE(direction, exit->directions) {
        fprintf(xmlFile, "            <exit DIRECTION=\"%s\" TARGET=\"%s\" />\n",
                direction->member.id->string, exit->target->string);
    }
}
