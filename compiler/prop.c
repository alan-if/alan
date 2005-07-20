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
#include "description_x.h"
#include "initialize_x.h"
#include "article_x.h"
#include "lst_x.h"
#include "nam_x.h"
#include "scr_x.h"
#include "stm_x.h"
#include "sym_x.h"
#include "vrb_x.h"
#include "whr_x.h"
#include "wrd_x.h"
#include "dump_x.h"


/*======================================================================*/
Properties *newEmptyProps(void)
{
  return NEW(Properties);
}



/*======================================================================*/
Properties *newProps(Where *whr, List *names,
		     Srcp pronounsSrcp, List *pronouns,
		     List *attributes,
		     Initialize *init,
		     Description *description,
		     Srcp mentionedSrcp, List *mentioned,
		     Srcp definiteSrcp, Article *definite, Bool definiteIsForm, Article *indefinite,
		     Container *container,
		     List *verbs,
		     Srcp enteredSrcp, List *enteredStatements,
		     List *exits,
		     List *scripts)
{
  Properties *new;                  /* The newly allocated area */

  showProgress();

  new = NEW(Properties);

  new->whr = whr;
  new->names = names;
  new->pronounsSrcp = pronounsSrcp;
  new->pronouns = pronouns;
  new->attributes = attributes;
  new->initialize = init;
  new->description = description;
  new->mentioned = mentioned;
  new->mentionedSrcp = mentionedSrcp;
  new->indefinite = indefinite;
  new->definite = definite;
  new->container = container;
  new->verbs = verbs;
  new->enteredStatements = enteredStatements;
  new->enteredSrcp = enteredSrcp;
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
}


/*======================================================================*/
void addOpaqueAttribute(Properties *props, Bool opaque)
{
  IdNode *opaqueId = newId(nulsrcp, "opaque");
  Attribute *attribute = newBooleanAttribute(nulsrcp, opaqueId, opaque);

  attribute->id->code = OPAQUEATTRIBUTE;	/* Pre-defined 'opaque' code */
  /* Make sure the opaque attribute is first, so combine the lists */
  props->attributes = combine(concat(NULL, attribute, ATTRIBUTE_EXPRESSION),
			      props->attributes);
}


/*======================================================================*/
void symbolizeProps(Properties *props, Bool inClassDeclaration)
{
  symbolizeContainer(props->container);
  symbolizeParent(props);
  symbolizeAttributes(props->attributes, inClassDeclaration);
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
    /* TODO Actually we should not construct a Mentioned, better
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
static void checkSubclassing(Properties *props)
{
  if (props->parentId) {
    if (props->id->symbol == theHero) {
      if (props->parentId->symbol != actorSymbol)
	lmLog(&props->parentId->srcp, 411, sevERR, "Inheritance from anything but 'actor'");
    } else if (props->parentId->symbol)
      if (props->parentId->symbol->fields.entity.prohibitedSubclassing &&
	  !props->predefined)
	lmLog(&props->parentId->srcp, 423, sevERR, props->parentId->string);
  } else if (props->id->symbol != entitySymbol && props->id->symbol != theHero)
    lmLog(&props->id->srcp, 429, sevERR, "");
}


/*======================================================================*/
void analyzeProps(Properties *props, Context *context)
{
  checkSubclassing(props);

  if (props->whr != NULL)
    verifyInitialLocation(props->whr);
  if (!inheritsFrom(props->id->symbol, thingSymbol) && props->whr != NULL)
    lmLog(&props->whr->srcp, 405, sevERR, "have initial locations");
  if (inheritsFrom(props->id->symbol, actorSymbol)
      && props->whr != NULL && props->whr->kind == WHERE_IN)
    lmLog(&props->whr->srcp, 402, sevERR, "An Actor");

  /* Don't analyze attributes since those are analyzed already */
  analyzeInitialize(props->initialize, context);
  analyzeDescription(props->description, context);
  analyzeStatements(props->enteredStatements, context);
  analyzeMentioned(props, context);
  analyzeStatements(props->mentioned, context);
  analyzeArticle(props->definite, context);
  analyzeArticle(props->indefinite, context);
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
  generateInitialize(props->initialize);
  generateDescription(props->description);

  if (props->enteredStatements != NULL) {
    props->enteredAddress = nextEmitAddress();
    generateStatements(props->enteredStatements);
    emit0(I_RETURN);
  }

  generateArticle(props->definite);
  generateArticle(props->indefinite);

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

  props->attributeAddress = generateAttributes(props->attributes, props->id->symbol->code);

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

  if (props->pronouns)
    entry->pronoun = props->pronouns->element.id->code;
  else
    entry->pronoun = 0;

  if (props->initialize)
    entry->initialize = props->initialize->stmsAddress;
  else
    entry->initialize = 0;

  entry->checks = checksAddressOf(props->description);
  entry->description = doesAddressOf(props->description);
  entry->entered = props->enteredAddress;

  if (props->container != NULL)
    entry->container = props->container->code;
  else
    entry->container = 0;

  entry->mentioned = props->mentionedAddress;

  if (props->definite) {
    entry->definite = props->definite->address;
    entry->definiteIsForm = props->definite->isForm;
  } else
    entry->definite = 0;

  if (props->indefinite) {
    entry->indefinite = props->indefinite->address;
    entry->indefiniteIsForm = props->indefinite->isForm;
  } else
    entry->indefinite = 0;

  entry->exits = props->exitsAddress;
  entry->verbs = props->verbsAddress;
}


/*======================================================================*/
void dumpProps(Properties *props)
{
  put("PROPS: "); dumpPointer(props); indent();
  put("id: "); dumpId(props->id); nl();
  put("parentId: "); dumpId(props->parentId); nl();
  put("whr: "); dumpWhere(props->whr); nl();
  put("names: "); dumpListOfLists(props->names, NAME_LIST); nl();
  put("pronoun: "); dumpList(props->pronouns, ID_LIST); nl();
  put("initialize: "); dumpInitialize(props->initialize); nl();
  put("container: "); dumpContainer(props->container); nl();
  put("attributes: "); dumpList(props->attributes, ATTRIBUTE_LIST); nl();
  put("attributeAddress: "); dumpAddress(props->attributeAddress); nl();
  put("description: "); dumpDescription(props->description); nl();
  put("definite: "); dumpArticle(props->definite); nl();
  put("indefinite: "); dumpArticle(props->indefinite); nl();
  put("mentioned: "); dumpList(props->mentioned, STATEMENT_LIST); nl();
  put("mentionedAddress: "); dumpAddress(props->mentionedAddress); nl();
  put("scripts: "); dumpList(props->scripts, SCRIPT_LIST); nl();
  put("scriptsAddress: "); dumpAddress(props->scriptsAddress); nl();
  put("verbs: "); dumpList(props->verbs, VERB_LIST); nl();
  put("verbsAddress: "); dumpAddress(props->verbsAddress); nl();
  put("exits: "); dumpList(props->exits, EXIT_LIST); nl();
  put("exitsAddress: "); dumpAddress(props->exitsAddress); out();
}
