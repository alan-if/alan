/*----------------------------------------------------------------------*\

				ADD.C
			     Add To Nodes

\*----------------------------------------------------------------------*/

#include "add_x.h"

/* IMPORT: */
#include "adv_x.h"
#include "prop_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "srcp_x.h"
#include "atr_x.h"
#include "lst_x.h"

#include "vrb_x.h"
#include "scr.h"
#include "ext.h"


#include "adv.h"

#include "types.h"
#include "dump.h"
#include "util.h"
#include "lmList.h"


/*======================================================================*/
AddNode *newAdd(Srcp *srcp,
		IdNode *id,
		IdNode *parent,
		Properties *props)
{
  AddNode *new;

  showProgress();

  new = NEW(AddNode);

  new->srcp = *srcp;
  if (props)
    new->props = props;
  else
    new->props = newEmptyProps();
  new->toId = id;

  if (parent != NULL)
    lmLogv(&parent->srcp, 341, sevERR, "heritage", "", NULL);

  return(new);
}

/*----------------------------------------------------------------------*/
static void addAttributes(AddNode *add, Symbol *originalSymbol)
{
  List *addedAttributes = add->props->attributes;
  Properties *originalProps = originalSymbol->fields.entity.props;
  List *originalAttributes = originalProps->attributes;
  List *l;

  if (addedAttributes == NULL) return;

  TRAVERSE(l, addedAttributes) {
    Attribute *originalAttribute = findAttribute(originalAttributes, l->element.atr->id);
    if (originalAttribute != NULL) /* It was found in the original */
      lmLog(&l->element.atr->id->srcp, 336, sevERR, "an attribute which already exists");
  }
  originalProps->attributes = combine(originalProps->attributes,
				      addedAttributes);
}


/*----------------------------------------------------------------------*/
static void addContainer(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->container == NULL) return;

  if (original->fields.entity.props->container != NULL)
    lmLog(&add->props->container->body->srcp, 336, sevERR,
	  "container properties when the class already have it");
  else
    original->fields.entity.props->container = add->props->container;
  
}


/*----------------------------------------------------------------------*/
static void addVerbs(AddNode *add, Symbol *originalSymbol)
{
  Properties *originalProps = originalSymbol->fields.entity.props;
  List *verbList;
  List *verbIdList;
  Bool multipleVerb = FALSE;

  if (add->props->verbs != NULL) {
    if (originalSymbol == entitySymbol)
      lmLog(&add->props->verbs->element.vrb->srcp, 426, sevWAR, "");
    TRAVERSE(verbList, add->props->verbs) {
      TRAVERSE(verbIdList, verbList->element.vrb->ids)
	if (foundVerb(verbIdList->element.id, originalProps->verbs)) {
	  multipleVerb = TRUE;
	  lmLogv(&verbList->element.id->srcp, 240, sevERR, "Verb", verbIdList->element.id->string, originalSymbol->string, NULL);
	}
    }
    if (!multipleVerb)
      originalProps->verbs = combine(originalProps->verbs, add->props->verbs);
  }
}



/*----------------------------------------------------------------------*/
static void addInitialLocation(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->whr != NULL)
    lmLogv(&props->whr->srcp, 341, sevERR, "initial location", "(yet)", NULL);
}


/*----------------------------------------------------------------------*/
static void addNames(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->names != NULL)
    lmLogv(&props->names->element.id->srcp, 341, sevERR, "names", "(yet)", NULL);
}


/*----------------------------------------------------------------------*/
static void addDescriptionCheck(AddNode *add, Symbol *originalSymbol)
{
  Properties *props = add->props;

  if (props->descriptionChecks != NULL) {
    if (originalSymbol->fields.entity.props->descriptionChecks != NULL)
      lmLogv(&props->descriptionCheckSrcp, 241, sevERR, "Description Check",
	     originalSymbol->string, NULL);
    else
      originalSymbol->fields.entity.props->descriptionChecks = props->descriptionChecks;
  }
}


/*----------------------------------------------------------------------*/
static void addDescription(AddNode *add, Symbol *originalSymbol)
{
  Properties *props = add->props;

  if (props->descriptionStatements != NULL) {
    if (originalSymbol->fields.entity.props->descriptionStatements != NULL)
      lmLogv(&props->descriptionSrcp, 241, sevERR, "Description",
	     originalSymbol->string, NULL);
    else
      originalSymbol->fields.entity.props->descriptionStatements = props->descriptionStatements;

  }
}


/*----------------------------------------------------------------------*/
static void addEntered(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->enteredStatements != NULL)
    lmLogv(&props->enteredSrcp, 341, sevERR, "Entered clause", "(yet)", NULL);
}


/*----------------------------------------------------------------------*/
static void addArticles(AddNode *add, Symbol *original)
{
  if (add->props->definite != NULL) {
    if (original->fields.entity.props->definite != NULL)
      lmLog(&add->props->definiteSrcp, 336, sevERR,
	    "Definite Article when the class already have it");
    else
      original->fields.entity.props->definite = add->props->definite;
  }
  
  if (add->props->indefinite != NULL) {
    if (original->fields.entity.props->indefinite != NULL)
      lmLog(&add->props->indefiniteSrcp, 336, sevERR,
	    "Indefinite Article when the class already have it");
    else
      original->fields.entity.props->indefinite = add->props->indefinite;
  }
}


/*----------------------------------------------------------------------*/
static void addMentioned(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->mentioned != NULL)
    lmLogv(&props->mentionedSrcp, 341, sevERR, "mentioned", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addScripts(AddNode *add, Symbol *original)
{
  Properties *props = add->props;
  Properties *originalProps = original->fields.entity.props;
  List *addedScripts;
  List *originalScripts;
  List *scriptsToAdd = NULL;
  Bool doNotAdd = FALSE;

  if (props->scripts == NULL) return;

  if (!inheritsFrom(original, actorSymbol)) {
    lmLog(&add->toId->srcp, 336, sevERR, "scripts to a class which is not a subclass of actor");
    doNotAdd = TRUE;
  }
  TRAVERSE(addedScripts, props->scripts) {
    Script *addedScript = addedScripts->element.script;
    Bool duplicate = FALSE;
    TRAVERSE(originalScripts, originalProps->scripts) {
      Script *originalScript = originalScripts->element.script;
      if (equalId(addedScript->id, originalScript->id)) {
	lmLogv(&addedScript->srcp, 240, sevERR,
	       "Script", addedScript->id->string, add->toId->string, NULL);
	duplicate = TRUE;
	break;
      }
    }
    if (!duplicate && !doNotAdd)
      scriptsToAdd = concat(scriptsToAdd, addedScript, SCRIPT_LIST);
  }
  originalProps->scripts = combine(originalProps->scripts, scriptsToAdd);
}


/*----------------------------------------------------------------------*/
static void addExits(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->exits != NULL)
    lmLogv(&props->exits->element.ext->srcp, 341, sevERR, "exits", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void verifyAdd(AddNode *add, Symbol *originalSymbol)
{
  /* Can't add anything except verbs to non-instantiable classes */
  if (originalSymbol->fields.entity.prohibitedSubclassing) {
    int propsCount = 1;		/* Verbs-slot is not counted so start at 1 */

    if (add->props->names)
      lmLogv(&add->srcp, 424, sevERR, "names", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->whr)
      lmLogv(&add->props->whr->srcp, 424, sevERR, "initial location", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->attributes)
      lmLogv(&add->props->attributes->element.atr->srcp, 424, sevERR, "attributes", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->descriptionChecks || add->props->descriptionStatements)
      lmLogv(&add->props->descriptionSrcp, 424, sevERR, "description", originalSymbol->string, NULL);
    propsCount+=2;

    if (add->props->enteredStatements)
      lmLogv(&add->props->enteredSrcp, 424, sevERR, "entered", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->definite)
      lmLogv(&add->props->definiteSrcp, 424, sevERR, "article", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->indefinite)
      lmLogv(&add->props->indefiniteSrcp, 424, sevERR, "article", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->mentioned)
      lmLogv(&add->props->mentionedSrcp, 424, sevERR, "mentioned", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->container)
      lmLogv(&add->props->container->body->srcp, 424, sevERR, "container", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->scripts)
      lmLogv(&add->props->scripts->element.script->srcp, 424, sevERR, "scripts", originalSymbol->string, NULL);
    propsCount++;

    if (add->props->exits)
      lmLogv(&add->props->exits->element.ext->srcp, 424, sevERR, "exits", originalSymbol->string, NULL);
    propsCount++;

    if (propsCount != NOOFPROPS)
      syserr("Wrong number of property checks in '%s()'", __FUNCTION__);
  }
}


/*----------------------------------------------------------------------*/
static void addAddition(AddNode *add)
{
  Symbol *originalClass = symcheck(add->toId, CLASS_SYMBOL, NULL);

  if (originalClass != NULL) {
    int propCount = 0;
    verifyAdd(add, originalClass);
    addInitialLocation(add, originalClass); propCount++;
    addNames(add, originalClass); propCount++;
    addAttributes(add, originalClass); propCount++;
    addDescriptionCheck(add, originalClass); propCount++;
    addDescription(add, originalClass); propCount++;
    addEntered(add, originalClass);  propCount++;
    addArticles(add, originalClass); propCount+=2;
    addMentioned(add, originalClass); propCount++;
    addContainer(add, originalClass); propCount++;
    addVerbs(add, originalClass); propCount++;
    addScripts(add, originalClass); propCount++;
    addExits(add, originalClass); propCount++;
    if (propCount != NOOFPROPS)
      syserr("Wrong property count in '%s()'", __FUNCTION__);
  }
}


/*======================================================================*/
void addAdditions(void)
{
  List *l;

  for (l = adv.adds; l != NULL; l = l->next)
    addAddition(l->element.add);
}


/*======================================================================*/
void dumpAdd(AddNode *add)
{
  put("ADD: "); dumpSrcp(&add->srcp); indent();
  put("toId: "); dumpId(add->toId); nl();
  put("props: "); dumpProps(add->props); out();
}
