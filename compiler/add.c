/*----------------------------------------------------------------------*\

				ADD.C
			     Add To Nodes

\*----------------------------------------------------------------------*/

#include "add_x.h"

/* IMPORT: */
#include "prop_x.h"
#include "sym_x.h"
#include "id_x.h"
#include "srcp_x.h"
#include "atr_x.h"
#include "lst_x.h"

#include "vrb.h"
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
static void addAttributes(AddNode *add, Symbol *originalSymbol)
{
  List *addedAttributes = add->props->attributes;
  Properties *originalProps = originalSymbol->fields.entity.props;
  List *originalAttributes = originalProps->attributes;
  List *l;

  if (addedAttributes == NULL) return;

  for (l = addedAttributes; l != NULL; l = l->next) {
    Attribute *originalAttribute = findAttribute(originalAttributes, l->element.atr->id);
    if (originalAttribute != NULL) /* It was found in the original */
      lmLog(&l->element.atr->id->srcp, 336, sevERR, "an existing attribute");
  }
  originalProps->attributes = combine(originalProps->attributes,
				      addedAttributes);
}


/*----------------------------------------------------------------------*/
static void addDescriptionCheck(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->descriptionChecks != NULL)
    lmLogv(&props->descriptionSrcp, 341, sevERR, "description checks", "(yet!)", NULL);

}


/*----------------------------------------------------------------------*/
static void addDescription(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->description != NULL)
    lmLogv(&props->descriptionSrcp, 341, sevERR, "description", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addArticle(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->article != NULL)
    lmLogv(&props->articleSrcp, 341, sevERR, "article", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addMentioned(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->mentioned != NULL)
    lmLogv(&props->mentionedSrcp, 341, sevERR, "mentioned", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addContainer(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->container != NULL)
    lmLogv(&props->container->srcp, 341, sevERR, "container", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addVerbs(AddNode *add, Symbol *originalSymbol)
{
  Properties *originalProps = originalSymbol->fields.entity.props;

  if (add->props->verbs != NULL)
    originalProps->verbs = combine(originalProps->verbs, add->props->verbs);
}


/*----------------------------------------------------------------------*/
static void addScripts(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->scripts != NULL)
    lmLogv(&props->scripts->element.scr->srcp, 341, sevERR, "scripts", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addExits(AddNode *add, Symbol *original)
{
  Properties *props = add->props;

  if (props->exits != NULL)
    lmLogv(&props->exits->element.ext->srcp, 341, sevERR, "exits", "(yet)", NULL);

}


/*----------------------------------------------------------------------*/
static void addAddition(AddNode *add)
{
  Symbol *originalClass = symcheck(add->toId, CLASS_SYMBOL, NULL);

  if (originalClass != NULL) {
    addInitialLocation(add, originalClass);
    addNames(add, originalClass);
    addAttributes(add, originalClass);
    addDescriptionCheck(add, originalClass);
    addDescription(add, originalClass);
    addArticle(add, originalClass);
    addMentioned(add, originalClass);
    addContainer(add, originalClass);
    addVerbs(add, originalClass);
    addScripts(add, originalClass);
    addExits(add, originalClass);
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
  put("ADD: "); dumpSrcp(&add->srcp); in();
  put("toId: "); dumpId(add->toId); nl();
  put("props: "); dumpProps(add->props); out();
}
