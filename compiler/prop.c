/*----------------------------------------------------------------------*\

			       PROP.C
			    Property Nodes

\*----------------------------------------------------------------------*/

#include "prop_x.h"

/* IMPORT */
#include <stdio.h>
#include "alan.h"
#include "util.h"
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
#include "dump_x.h"


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
		     Srcp descriptionCheckSrcp, List *descriptionChecks,
		     Srcp descriptionSrcp, List *description,
		     Srcp enteredSrcp, List *enteredStatements,
		     Srcp mentionedSrcp, List *mentioned,
		     Srcp definiteSrcp, List *definite, Bool definiteIsForm,
		     Srcp indefiniteSrcp, List *indefinite, Bool indefiniteIsForm,
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

  new->descriptionCheckSrcp = descriptionCheckSrcp;
  new->descriptionChecks = descriptionChecks;
  new->descriptionSrcp = descriptionSrcp;
  new->descriptionStatements = description;
  new->enteredStatements = enteredStatements;
  new->enteredSrcp = enteredSrcp;
  new->mentioned = mentioned;
  new->mentionedSrcp = mentionedSrcp;
  new->definite = definite;
  new->definiteIsForm = definiteIsForm;
  new->definiteSrcp = definiteSrcp;
  new->indefinite = indefinite;
  new->indefiniteIsForm = indefiniteIsForm;
  new->indefiniteSrcp = indefiniteSrcp;
  new->verbs = verbs;
  new->exits = exits;
  new->scripts = scripts;

  return(new);
}



/*----------------------------------------------------------------------*/
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
#ifdef DEFAULTENTITY
  else if (strcmp(props->id->string, "entity") != 0) {
    /* Unless it is "entity" itself, assume "entity" is its parent */
    lmLog(&props->id->srcp, 270, sevWAR, "");
    props->parentId = newId(&nulsrcp, "entity");
    props->parentId->symbol = entitySymbol;
    setParent(props->id->symbol, props->parentId->symbol);
  }
#endif
}


/*----------------------------------------------------------------------*/
static void addOpaqueAttribute(Properties *props, Bool opaque)
{
  IdNode *opaqueId = newId(&nulsrcp, "opaque");
  Attribute *attribute = newBooleanAttribute(nulsrcp, opaqueId, opaque);

  attribute->id->code = OPAQUEATTRIBUTE;	/* Pre-defined 'opaque' code */
  props->attributes = concat(props->attributes, attribute, ATTRIBUTE_LIST);
}


/*======================================================================*/
void symbolizeProps(Properties *props)
{
  symbolizeContainer(props->container);
  symbolizeParent(props);
  symbolizeAttributes(props->attributes);
  if (props->container)
    addOpaqueAttribute(props, props->container->body->opaque);
  symbolizeWhere(props->whr);
  symbolizeExits(props->exits);
}


/*----------------------------------------------------------------------*/
static void analyzeMentioned(Properties *props, Context *context)
{
  long fpos;
  int len = 0;
  Statement *stm;

  if (props->mentioned == NULL) {
    /* FIXME Actually we should not construct a Mentioned, better
       would be to generate it during runtime. However, to do this we
       would require that we store the names so that they can be
       printed, preferably exactly as the author wrote them. An entry
       called name which would be a list of strings would suffice. */
    /* Generate a mentioned from the first of the names */
    /* First output the formatted name to the text file */
    fpos = ftell(txtfil);
    len = analyzeNames(props->names, props->id);
    /* Then create a PRINT statement */
    stm = newStatement(&nulsrcp, PRINT_STATEMENT);
    stm->fields.print.fpos = fpos;
    stm->fields.print.len = len;
    props->mentioned = concat(NULL, stm, LIST_STATEMENT);
  } else {
    if ((props->names != NULL) & inheritsFrom(props->id->symbol, locationSymbol))
      lmLog(&props->mentionedSrcp, 425, sevWAR, "");
    analyzeStatements(props->mentioned, context);
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
    lmLog(&props->whr->srcp, 405, sevERR, "have initial locations");
  if (inheritsFrom(props->id->symbol, actorSymbol)
      && props->whr != NULL && props->whr->kind == WHERE_IN)
    lmLog(&props->whr->srcp, 402, sevERR, "An Actor");


  /* Don't analyze attributes since those are analyzed already */
  analyzeChecks(props->descriptionChecks, context);
  analyzeStatements(props->descriptionStatements, context);
  analyzeStatements(props->enteredStatements, context);
  analyzeMentioned(props, context);
  analyzeStatements(props->mentioned, context);
  analyzeStatements(props->definite, context);
  analyzeStatements(props->indefinite, context);
  analyzeVerbs(props->verbs, context);

  /* Have container ? */
  if (props->container) {
    /* But is a location? */
    if (inheritsFrom(props->id->symbol, locationSymbol))
      lmLogv(&props->id->srcp, 354, sevERR,
	     isClass(props->id->symbol)?"Class":"Instance",
	     props->id->string,
	     "location",
	     "Container properties, which is dubious in use",
	     NULL);
    analyzeContainer(props->container, context);
  }

  /* Have ENTERED or EXITs but not a location? */
  if (props->enteredStatements && !inheritsFrom(props->id->symbol, locationSymbol))
    lmLogv(&props->id->srcp, 352, sevERR,
	   isClass(props->id->symbol)?"Class":"Instance",
	   props->id->string,
	   "location",
	   "ENTERED statements which is not allowed",
	   NULL);
  if (props->exits && !inheritsFrom(props->id->symbol, locationSymbol))
    lmLogv(&props->id->srcp, 352, sevERR,
	   isClass(props->id->symbol)?"Class":"Instance",
	   props->id->string,
	   "location",
	   "EXITs, which can never be traversed",
	   NULL);
  analyzeExits(props->exits, context);

  /* Have scripts but not an actor? */
  if (props->scripts && !inheritsFrom(props->id->symbol, actorSymbol))
    lmLogv(&props->id->srcp, 352, sevERR,
	   isClass(props->id->symbol)?"Class":"Instance",
	   props->id->string,
	   "actor",
	   "SCRIPTs, which can never be executed",
	   NULL);
  prepareScripts(props->scripts, props->id);
  analyzeScripts(props->scripts, context);
}


/*======================================================================*/
void generateCommonPropertiesData(Properties *props)
{
  props->descriptionChecksAddress = generateChecks(props->descriptionChecks);

  if (props->descriptionStatements != NULL) {
    props->descriptionAddress = nextEmitAddress();
    generateStatements(props->descriptionStatements);
    emit0(I_RETURN);
  }

  if (props->enteredStatements != NULL) {
    props->enteredAddress = nextEmitAddress();
    generateStatements(props->enteredStatements);
    emit0(I_RETURN);
  }

  if (props->definite != NULL) {
    props->definiteAddress = nextEmitAddress();
    generateStatements(props->definite);
    emit0(I_RETURN);
  }

  if (props->indefinite != NULL) {
    props->indefiniteAddress = nextEmitAddress();
    generateStatements(props->indefinite);
    emit0(I_RETURN);
  }

  if (props->mentioned != NULL) {
    props->mentionedAddress = nextEmitAddress();
    generateStatements(props->mentioned);
    emit0(I_RETURN);
  }

  props->verbsAddress = generateVerbs(props->verbs);
  props->exitsAddress = generateExits(props->exits);
}


/*======================================================================*/
void generateInstancePropertiesData(Properties *props)
{
  props->idAddress = nextEmitAddress();
  emitString(props->id->string);

  props->attributeAddress = generateAttributes(props->attributes);

  /* Now generate all the things both instances and classes have */
  generateCommonPropertiesData(props);
}


/*======================================================================*/
void generatePropertiesEntry(InstanceEntry *entry, Properties *props)
{
  entry->code = props->id->symbol->code; /* First own code */
  entry->idAddress = props->idAddress; /* Address to the id string */

  if (props->parentId == NULL)	/* Then parents... */
    entry->parent = 0;
  else
    entry->parent = props->parentId->symbol->code;

  entry->initialLocation = generateInitialLocation(props->whr);
  entry->initialAttributes = props->attributeAddress;
  entry->checks = props->descriptionChecksAddress;
  entry->description = props->descriptionAddress;
  entry->entered = props->enteredAddress;
  if (props->container != NULL)
    entry->container = props->container->code;
  else
    entry->container = 0;
  entry->mentioned = props->mentionedAddress;
  entry->definite = props->definiteAddress;
  entry->definiteIsForm = props->definiteIsForm;
  entry->indefinite = props->indefiniteAddress;
  entry->indefiniteIsForm = props->indefiniteIsForm;
  entry->exits = props->exitsAddress;
  entry->verbs = props->verbsAddress;
}


/*======================================================================*/
void dumpProps(Properties *props)
{
  put("PROPS: "); dumpPointer(props); indent();
  put("id: "); dumpId(props->id); nl();
  put("parentId: "); dumpId(props->parentId); nl();
  put("names: "); dumpListOfLists(props->names, NAME_LIST); nl();
  put("whr: "); dumpWhere(props->whr); nl();
  put("container: "); dumpContainer(props->container); nl();
  put("attributes: "); dumpList(props->attributes, ATTRIBUTE_LIST); nl();
  put("attributeAddress: "); dumpAddress(props->attributeAddress); nl();
  put("descriptionChecks: "); dumpList(props->descriptionChecks, CHECK_LIST); nl();
  put("descriptionChecksAddress: "); dumpAddress(props->descriptionChecksAddress); nl();
  put("description: "); dumpList(props->descriptionStatements, STATEMENT_LIST); nl();
  put("descriptionAddress: "); dumpAddress(props->descriptionAddress); nl();
  put("definite: "); dumpList(props->definite, STATEMENT_LIST); nl();
  put("definiteAddress: "); dumpAddress(props->definiteAddress); nl();
  put("indefinite: "); dumpList(props->indefinite, STATEMENT_LIST); nl();
  put("indefiniteAddress: "); dumpAddress(props->indefiniteAddress); nl();
  put("mentioned: "); dumpList(props->mentioned, STATEMENT_LIST); nl();
  put("mentionedAddress: "); dumpAddress(props->mentionedAddress); nl();
  put("scripts: "); dumpList(props->scripts, SCRIPT_LIST); nl();
  put("scriptsAddress: "); dumpAddress(props->scriptsAddress); nl();
  put("verbs: "); dumpList(props->verbs, VERB_LIST); nl();
  put("verbsAddress: "); dumpAddress(props->verbsAddress); nl();
  put("exits: "); dumpList(props->exits, EXIT_LIST); nl();
  put("exitsAddress: "); dumpAddress(props->exitsAddress); out();
}
