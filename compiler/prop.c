/*----------------------------------------------------------------------*\

			       PROP.C
			    Property Nodes

\*----------------------------------------------------------------------*/

#include "prop_x.h"

/* IMPORT */
#include <stdio.h>
#include "alan.h"
#include "util.h"
#include "dump.h"
#include "emit.h"
#include "lmList.h"


#include "atr_x.h"
#include "cla_x.h"
#include "cnt_x.h"
#include "ext_x.h"
#include "id_x.h"
#include "lst_x.h"
#include "nam_x.h"
#include "scr_x.h"
#include "stm_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"


/*======================================================================*/
Properties *newEmptyProps(void)
{
  return NEW(Properties);
}



/*======================================================================*/
Properties *newProps(List *names,
		Where *whr,
		List *attributes,
		CntNod *container,
		List *description,
		Srcp *descriptionSrcp,
		List *mentioned,
		Srcp *mentionedSrcp,
		List *article,
		Srcp *articleSrcp,
		List *does,
		List *exits,
		List *verbs,
		List *scripts)
{
  Properties *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(Properties);

  new->names = names;
  new->whr = whr;
  new->attributes = attributes;

  new->container = container;
  if (new->container != NULL)
    new->container->ownerProperties = new;

  new->description = description;
  new->descriptionSrcp = *descriptionSrcp;
  new->mentioned = mentioned;
  new->mentionedSrcp = *mentionedSrcp;
  new->article = article;
  new->articleSrcp = *articleSrcp;
  new->verbs = verbs;
  new->exits = exits;
  new->scripts = scripts;

  return(new);
}



/*----------------------------------------------------------------------

  symbolizeParent()

  Symbolize parent property node.

 */
static void symbolizeParent(Properties *props)
{
  Symbol *parent;

  if (props->parentId != NULL) {
    parent = lookup(props->parentId->string);
    if (parent == NULL)
      lmLog(&props->parentId->srcp, 310, sevERR, props->parentId->string);
    else if (parent->kind != CLASS_SYMBOL)
      lmLog(&props->parentId->srcp, 350, sevERR, "");
    else {
      props->parentId->symbol = parent;
      setParent(props->id->symbol, props->parentId->symbol);
    }
  }
}


/*======================================================================*/
void symbolizeProps(Properties *props)
{
  symbolizeParent(props);
  checkMultipleAttributes(props->attributes);
  symbolizeWhere(props->whr);
  symbolizeExits(props->exits);
}


/*----------------------------------------------------------------------*/
static void analyzeName(Properties *props)
{
  long fpos;
  int len = 0;
  StmNod *stm;

  if (props->mentioned == NULL) {
    /* Generate a mentioned from the first of the names */
    /* First output the formated name to the text file */
    fpos = ftell(txtfil);
    len = annams(props->names, props->id,
		 inheritsFrom(props->id->symbol, locationSymbol) ||
		 inheritsFrom(props->id->symbol, actorSymbol));

    /* Then create a PRINT statement */
    stm = newstm(&nulsrcp, PRINT_STATEMENT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    props->mentioned = concat(NULL, stm, LIST_STATEMENT);
  } else
    analyzeStatements(props->mentioned, NULL);
}


/*======================================================================*/
void analyzeProps(Properties *props, Context *context)
{
  if (props->whr != NULL) verifyInitialLocation(props->whr);
  if (inheritsFrom(props->id->symbol, locationSymbol) && props->whr != NULL)
    lmLog(&props->whr->srcp, 405, sevERR, "");
  if (inheritsFrom(props->id->symbol, actorSymbol)
      && props->whr != NULL && props->whr->kind == WHR_IN)
    lmLog(&props->whr->srcp, 402, sevERR, "An Actor");

  analyzeName(props);
  analyzeStatements(props->description, context);
  analyzeVerbs(props->verbs, context);
  analyzeContainer(props->container, context);

  if (props->exits && !inheritsFrom(props->id->symbol, locationSymbol))
    lmLog(&props->id->srcp, 352, sevERR, props->id->string);
  analyzeExits(props->exits, context);

  if (props->scripts && !inheritsFrom(props->id->symbol, actorSymbol))
    lmLog(&props->id->srcp, 353, sevERR, props->id->string);
  analyzeScripts(props->scripts, context);
}


/*======================================================================*/
void generateClassPropertiesData(Properties *props)
{
  if (props->description != NULL) {
    props->descriptionAddress = emadr();
    gestms(props->description, props->id->symbol->code);
    emit0(C_STMOP, I_RETURN);
  }

  if (props->mentioned != NULL) {
    props->mentionedAddress = emadr();
    gestms(props->mentioned, props->id->symbol->code);
    emit0(C_STMOP, I_RETURN);
  }

  props->scriptsAddress = generateScripts(props->scripts, props->id->symbol->code);
  props->verbsAddress = generateVerbs(props->verbs, props->id->symbol->code);
  props->exitsAddress = generateExits(props->exits, props->id->symbol->code);
}


/*======================================================================*/
void generateInstancePropertiesData(Properties *props)
{
  props->idAddress = emadr();
  emitString(props->id->string);

  props->attributeAddress = generateAttributes(props->attributes);

  if (props->description != NULL) {
    props->descriptionAddress = emadr();
    gestms(props->description, props->id->symbol->code);
    emit0(C_STMOP, I_RETURN);
  }

  if (props->mentioned != NULL) {
    props->mentionedAddress = emadr();
    gestms(props->mentioned, props->id->symbol->code);
    emit0(C_STMOP, I_RETURN);
  } else
    emit(0);

  props->scriptsAddress = generateScripts(props->scripts, props->id->symbol->code);
  props->verbsAddress = generateVerbs(props->verbs, props->id->symbol->code);
  props->exitsAddress = generateExits(props->exits, props->id->symbol->code);
}


/*======================================================================*/
void generatePropertiesEntry(InstanceEntry *entry, Properties *props)
{
  entry->code = props->id->symbol->code; /* First own code */
  entry->idAddress = props->idAddress; /* Address to the id string */

  if (props->parentId == NULL)	/* Then parents */
    entry->parentClass = 0;
  else
    entry->parentClass = props->parentId->symbol->code;

  entry->location = generateInitialLocation(props->whr);
  entry->attributes = props->attributeAddress;
  entry->description = props->descriptionAddress;
  if (props->container != NULL)
    entry->container = props->container->code;
  else
    entry->container = 0;
  entry->mentioned = props->mentionedAddress;
  entry->article = props->articleAddress;
  entry->exits = props->exitsAddress;
  entry->verbs = props->verbsAddress;
  entry->scripts = props->scriptsAddress;
}


/*======================================================================*/
void dumpProps(Properties *props)
{
  put("PROPS: "); dumpPointer(props); in();
  put("id: "); dumpId(props->id); nl();
  put("parentId: "); dumpId(props->parentId); nl();
  put("names: "); dumpList(props->names, LIST_NAM); nl();
  put("whr: "); duwhr(props->whr); nl();
  put("container: "); dumpContainer(props->container); nl();
  put("attributes: "); dumpList(props->attributes, ATTRIBUTE_LIST); nl();
  put("attributeAddress: "); dumpAddress(props->attributeAddress); nl();
  put("description: "); dumpList(props->description, LIST_STM); nl();
  put("descriptionAddress: "); dumpAddress(props->descriptionAddress); nl();
  put("article: "); dumpList(props->article, LIST_STM); nl();
  put("articleAddres: "); dumpAddress(props->articleAddress); nl();
  put("mentioned: "); dumpList(props->mentioned, LIST_STM); nl();
  put("mentionedAddress: "); dumpAddress(props->mentionedAddress); nl();
  put("scripts: "); dumpList(props->scripts, LIST_SCR); nl();
  put("scriptsAddress: "); dumpAddress(props->scriptsAddress); nl();
  put("verbs: "); dumpList(props->verbs, LIST_VRB); nl();
  put("verbsAddress: "); dumpAddress(props->verbsAddress); nl();
  put("exits: "); dumpList(props->exits, LIST_EXT); nl();
  put("exitsAddress: "); dumpAddress(props->exitsAddress); out();
}
