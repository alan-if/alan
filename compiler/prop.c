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
#include "chk_x.h"
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
		     Container *container,
		     Srcp *descriptionCheckSrcp,
		     List *descriptionChecks,
		     Srcp *descriptionSrcp,
		     List *description,
		     List *mentioned,
		     Srcp *mentionedSrcp,
		     List *article,
		     Srcp *articleSrcp,
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

  new->descriptionSrcp = *descriptionSrcp;
  new->descriptionChecks = descriptionChecks;
  new->descriptionStatements = description;
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
    len = analyzeNames(props->names, props->id,
		 inheritsFrom(props->id->symbol, locationSymbol) ||
		 inheritsFrom(props->id->symbol, actorSymbol));

    /* Then create a PRINT statement */
    stm = newstm(&nulsrcp, PRINT_STATEMENT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    props->mentioned = concat(NULL, stm, LIST_STATEMENT);
  } else {
    if ((props->names != NULL) & inheritsFrom(props->id->symbol, locationSymbol))
      lmLog(&props->mentionedSrcp, 425, sevWAR, "");
    analyzeStatements(props->mentioned, NULL);
  }
}

/*----------------------------------------------------------------------*/
static void checkProhibitedSubclassing(Properties *props)
{
  if (props->parentId && props->parentId->symbol)
    if (props->parentId->symbol->fields.entity.prohibitedSubclassing &&
	!props->predefined)
      lmLog(&props->parentId->srcp, 423, sevERR, props->parentId->string);
}


/*======================================================================*/
void analyzeProps(Properties *props, Context *context)
{
  checkProhibitedSubclassing(props);

  if (props->whr != NULL) verifyInitialLocation(props->whr);
  if (inheritsFrom(props->id->symbol, locationSymbol) && props->whr != NULL)
    lmLog(&props->whr->srcp, 405, sevERR, "");
  if (inheritsFrom(props->id->symbol, actorSymbol)
      && props->whr != NULL && props->whr->kind == WHR_IN)
    lmLog(&props->whr->srcp, 402, sevERR, "An Actor");

  analyzeName(props);
  analyzeChecks(props->descriptionChecks, context);
  analyzeStatements(props->descriptionStatements, context);
  analyzeStatements(props->mentioned, context);
  analyzeStatements(props->article, context);
  analyzeVerbs(props->verbs, context);

  /* Have container but is a location? */
  if (props->container && inheritsFrom(props->id->symbol, locationSymbol))
    lmLog(&props->id->srcp, 354, sevERR, props->id->string);
  analyzeContainer(props->container, context);

  /* Have exits but not a location? */
  if (props->exits && !inheritsFrom(props->id->symbol, locationSymbol))
    lmLog(&props->id->srcp, 352, sevERR, props->id->string);
  analyzeExits(props->exits, context);

  /* Have scripts but not an actor? */
  if (props->scripts && !inheritsFrom(props->id->symbol, actorSymbol))
    lmLog(&props->id->srcp, 353, sevERR, props->id->string);
  prepareScripts(props->scripts, props->id);
  analyzeScripts(props->scripts, context);
}


/*======================================================================*/
void generateClassPropertiesData(Properties *props)
{
  props->descriptionChecksAddress = generateChecks(props->descriptionChecks);

  if (props->descriptionStatements != NULL) {
    props->descriptionAddress = emadr();
    generateStatements(props->descriptionStatements);
    emit0(I_RETURN);
  }

  if (props->article != NULL) {
    props->articleAddress = emadr();
    generateStatements(props->article);
    emit0(I_RETURN);
  }

  if (props->mentioned != NULL) {
    props->mentionedAddress = emadr();
    generateStatements(props->mentioned);
    emit0(I_RETURN);
  }

  props->verbsAddress = generateVerbs(props->verbs);
  props->exitsAddress = generateExits(props->exits);
}


/*======================================================================*/
void generateInstancePropertiesData(Properties *props)
{
  props->idAddress = emadr();
  emitString(props->id->string);

  props->attributeAddress = generateAttributes(props->attributes);

  props->descriptionChecksAddress = generateChecks(props->descriptionChecks);

  if (props->descriptionStatements != NULL) {
    props->descriptionAddress = emadr();
    generateStatements(props->descriptionStatements);
    emit0(I_RETURN);
  }

  if (props->mentioned != NULL) {
    props->mentionedAddress = emadr();
    generateStatements(props->mentioned);
    emit0(I_RETURN);
  } else
    emit(0);

  if (props->article != NULL) {
    props->articleAddress = emadr();
    generateStatements(props->article);
    emit0(I_RETURN);
  } else
    emit(0);

  props->verbsAddress = generateVerbs(props->verbs);
  props->exitsAddress = generateExits(props->exits);
}


/*======================================================================*/
void generatePropertiesEntry(InstanceEntry *entry, Properties *props)
{
  entry->code = props->id->symbol->code; /* First own code */
  entry->idAddress = props->idAddress; /* Address to the id string */

  if (props->parentId == NULL)	/* Then parents */
    entry->parent = 0;
  else
    entry->parent = props->parentId->symbol->code;

  entry->location = generateInitialLocation(props->whr);
  entry->attributes = props->attributeAddress;
  entry->checks = props->descriptionChecksAddress;
  entry->description = props->descriptionAddress;
  if (props->container != NULL)
    entry->container = props->container->code;
  else
    entry->container = 0;
  entry->mentioned = props->mentionedAddress;
  entry->article = props->articleAddress;
  entry->exits = props->exitsAddress;
  entry->verbs = props->verbsAddress;
}


/*======================================================================*/
void dumpProps(Properties *props)
{
  put("PROPS: "); dumpPointer(props); in();
  put("id: "); dumpId(props->id); nl();
  put("parentId: "); dumpId(props->parentId); nl();
  put("names: "); dumpList(props->names, NAME_LIST); nl();
  put("whr: "); duwhr(props->whr); nl();
  put("container: "); dumpContainer(props->container); nl();
  put("attributes: "); dumpList(props->attributes, ATTRIBUTE_LIST); nl();
  put("attributeAddress: "); dumpAddress(props->attributeAddress); nl();
  put("descriptionChecks: "); dumpList(props->descriptionChecks, CHECK_LIST); nl();
  put("descriptionChecksAddress: "); dumpAddress(props->descriptionChecksAddress); nl();
  put("description: "); dumpList(props->descriptionStatements, STATEMENT_LIST); nl();
  put("descriptionAddress: "); dumpAddress(props->descriptionAddress); nl();
  put("article: "); dumpList(props->article, STATEMENT_LIST); nl();
  put("articleAddres: "); dumpAddress(props->articleAddress); nl();
  put("mentioned: "); dumpList(props->mentioned, STATEMENT_LIST); nl();
  put("mentionedAddress: "); dumpAddress(props->mentionedAddress); nl();
  put("scripts: "); dumpList(props->scripts, SCRIPT_LIST); nl();
  put("scriptsAddress: "); dumpAddress(props->scriptsAddress); nl();
  put("verbs: "); dumpList(props->verbs, VERB_LIST); nl();
  put("verbsAddress: "); dumpAddress(props->verbsAddress); nl();
  put("exits: "); dumpList(props->exits, EXIT_LIST); nl();
  put("exitsAddress: "); dumpAddress(props->exitsAddress); out();
}
