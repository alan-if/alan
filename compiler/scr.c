/*----------------------------------------------------------------------*\

                                SCR.C
                             Script Nodes

\*----------------------------------------------------------------------*/

#include "scr_x.h"

/* IMPORT: */
#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "stp_x.h"
#include "sym_x.h"

#include "lmList.h"
#include "../interpreter/acode.h"
#include "emit.h"
#include "dump.h"


static int scriptCode = 0;		/* Numbering code */
static List *allScripts;


/*======================================================================

  newScript()

  */
Script *newScript(Srcp *srcp,
		  IdNode *id,
		  List *description,
		  List *steps
)
{
  Script *new;          /* The newly allocated node */

  showProgress();

  new = NEW(Script);

  new->srcp = *srcp;
  new->id = id;
  new->id->code = ++scriptCode;
  new->description = description;
  new->steps = steps;

  allScripts = concat(allScripts, new, SCRIPT_LIST);

  return(new);
}



/*======================================================================

  prepareScripts()

  Prepare scripts for this actor (i.e. number them)

  */
void prepareScripts(List *scrs, IdNode *id)
{
  List *lst;
  List *scrlst;

  if (scrs == NULL) return;

  /* Look for redefinition of script names */
  for (lst = scrs; lst != NULL; lst = lst->next) {
    /* Any multiple of this name ? */
    for (scrlst = lst->next; scrlst != NULL; scrlst = scrlst->next) {
      if (equalId(lst->element.scr->id, scrlst->element.scr->id))
	lmLog(&scrlst->element.scr->srcp, 403, sevERR, id->string);
    }
  }
}


/*======================================================================

  analyzeScripts()

  Analyze the scripts for one actor.

  */
void analyzeScripts(List *scripts, Context *context)
{
  List *lst;

  if (scripts == NULL) return;

  /* Error if defined for HERO */
  if (context->instance != 0) {
    if (scripts != NULL && context->instance->props->id->symbol == theHero)
      lmLog(&scripts->element.scr->srcp, 411, sevWAR, "Script");
  }

  for (lst = scripts; lst != NULL; lst = lst->next) {
    /* Analyze the statements */
    analyzeStatements(lst->element.scr->description, context);

    /* Finally, analyse the steps inside the script */
    analyzeSteps(lst->element.scr->steps, context);
  }
}


/*----------------------------------------------------------------------*/
static Aaddr generateScriptDescription(Script *script)
{
  Aaddr address = 0;

  if (script->description != NULL) {
    address = nextEmitAddress();
    generateStatements(script->description);
    emit0(I_RETURN);
  }
  return address;
}


/*======================================================================*/
Aaddr generateScripts(AcdHdr *header)
{
  List *lst;
  Aaddr scriptTableAddress;
  ScriptEntry entry;

  if (allScripts == NULL) return 0;

  for (lst = allScripts; lst != NULL; lst = lst->next) {
    lst->element.scr->stepAddress = generateSteps(lst->element.scr->steps);
    lst->element.scr->descriptionAddress = generateScriptDescription(lst->element.scr);
    lst->element.scr->stringAddress = nextEmitAddress();
    emitString(lst->element.scr->id->string);
  }

  /* Script table */
  scriptTableAddress = nextEmitAddress();
  for (lst = allScripts; lst != NULL; lst = lst->next) {
    entry.stringAddress = lst->element.scr->stringAddress;
    entry.code = lst->element.scr->id->code;
    entry.description = lst->element.scr->descriptionAddress;
    entry.steps = lst->element.scr->stepAddress;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);
  return(scriptTableAddress);
}


/*======================================================================

  dumpScript()

  */
void dumpScript(Script *scr)
{
  put("SCRIPT: "); dumpSrcp(&scr->srcp); in();
  put("id: "); dumpId(scr->id); nl();
  put("steps: "); dumpList(scr->steps, STEP_LIST); nl();
  put("stepAddress: "); dumpAddress(scr->stepAddress); out();
}



