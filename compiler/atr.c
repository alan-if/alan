/*----------------------------------------------------------------------*\

				ATR.C
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "atr_x.h"


/* IMPORT: */
#include "srcp_x.h"
#include "id_x.h"
#include "adv_x.h"
#include "sym_x.h"
#include "lst_x.h"
#include "exp_x.h"

#include "util.h"
#include "emit.h"

#include "ins.h"		/* INS-nodes */
#include "opt.h"		/* OPT-nodes */

#include "../interpreter/acode.h"

#include "lmList.h"
#include "encode.h"
#include "dump.h"


/* Exported data: */

int atrmax;



/*======================================================================

  newAttribute()

  Allocates and initialises an atrnod.

 */
Attribute *newAttribute(Srcp *srcp,	/* IN - Source Position */
		     TypeKind type,	/* IN - Type of this atribute */
		     IdNode *id,	/* IN - The id */
		     int value,		/* IN - The initial value */
		     long int fpos,	/* IN - File position for initial string */
		     int len)		/* IN - D:o length */
{
  Attribute *new;			/* The newly allocated area */

  showProgress();

  new = NEW(Attribute);

  new->srcp = *srcp;
  new->type = type;
  new->id = id;
  new->inheritance = UNKNOWN_INHERITANCE;
  new->value = value;
  new->stringAddress = 0;
  new->encoded = FALSE;
  new->fpos = fpos;
  new->len = len;

  return(new);
}


/*======================================================================

  checkMutipleAttributes()

  Check for multiple declarations of attributes in a list.

 */
void checkMultipleAttributes(List *atrs)
{
  List *al;

  while (atrs) {
    for (al = atrs->next; al; al = al->next) {
      if (equalId(atrs->element.atr->id, al->element.atr->id))
	  lmLog(&al->element.atr->id->srcp, 218, sevERR, al->element.atr->id->string);
    }
    atrs = atrs->next;
  }
}


/*======================================================================

  findAttribute()

*/
Attribute *findAttribute(List *attributes, IdNode *id)
{
  List *this;

  for (this = attributes; this != NULL; this = this->next)
    if (equalId(this->element.atr->id, id))
      return this->element.atr;
  return NULL;
}


/*======================================================================

  sortAttributes()

  Sort a list of attributes.

 */
List *sortAttributes(List *attributes)
{
  List *sortedList = attributes;
  Bool change;			/* Change during sorting */
  List **lstp;			/* Pointer to a list pointer */
  List *tmp1, *tmp2;		/* Temporary pointers */

  if (attributes != NULL) {
    change = TRUE;
    while (change) {
      change = FALSE;
      for (lstp = &sortedList; (*lstp)->next != NULL; lstp = &(*lstp)->next) {
	tmp1 = *lstp;
	tmp2 = tmp1->next;
	if (tmp1->element.atr->id->code != 0 &&
	    tmp1->element.atr->id->code == tmp2->element.atr->id->code) 
	  syserr("Sorting multiple attributes with same code.");
	if (tmp1->element.atr->id->code > tmp2->element.atr->id->code) {
	  change = TRUE;
	  tmp1->next = tmp2->next;
	  tmp2->next = tmp1;
	  *lstp = tmp2;
	}
      }
    }
  }
  return sortedList;
}



/*----------------------------------------------------------------------

  copyAttribute()

  Make a copy of an attribute

*/
static Attribute *copyAttribute(Attribute *theOriginal)
{
  Attribute *theCopy = NEW(Attribute);

  memcpy(theCopy, theOriginal, sizeof(Attribute));
  theCopy->inheritance = INHERITED;
  return theCopy;
}


/*----------------------------------------------------------------------

  copyAttributeList()

  Make a copy of a complete attribute list

*/
static List *copyAttributeList(List *theOriginal)
{
  List *theCopy = NULL;
  List *traversal;

  for (traversal = theOriginal; traversal != NULL; traversal = traversal->next)
    theCopy = concat(theCopy, copyAttribute(traversal->element.atr),
		     ATTRIBUTE_LIST);
  return theCopy;
}



/*======================================================================

  combineAttributes()

  Insert all attributes from the list to add that are not there
  already, then sort the list.

  NOTE! that we use the codes to combine, so this can't be used before
  attributes have been numbered!!

*/
List *combineAttributes(List *ownAttributes, List *attributesToAdd)
{
  List *own = ownAttributes;
  List *toAdd = attributesToAdd;
  List *new;

  while (own != NULL) {
    if (toAdd == NULL)
      break;
    else if (own->element.atr->id->code == toAdd->element.atr->id->code) {
      own = own->next;
      toAdd = toAdd->next;
    } else if (own->element.atr->id->code < toAdd->element.atr->id->code) {
      own = own->next;
    } else if (own->element.atr->id->code > toAdd->element.atr->id->code) {
      insert(own, copyAttribute(toAdd->element.atr), ATTRIBUTE_LIST);
      toAdd = toAdd->next;
    }
  }
  if (toAdd != NULL)
    new = combine(ownAttributes, copyAttributeList(toAdd));
  else
    new = ownAttributes;

  return sortAttributes(new);
}



/*======================================================================*/
void analyzeAttributes(List *atrs)
{
  List *al;

  while (atrs) {
    for (al = atrs->next; al; al = al->next) {
      if (equalId(atrs->element.atr->id, al->element.atr->id))
	  lmLog(&al->element.atr->id->srcp, 218, sevERR, al->element.atr->id->string);
    }
    atrs = atrs->next;
  }
}


/*----------------------------------------------------------------------

  resolveIdAttribute()

  Resolve an attribute reference for an identifier.

*/
static Attribute *resolveIdAttribute(IdNode *id, IdNode *attribute, Context *context)
{
  Attribute *atr = NULL;
  Symbol *sym;

  sym = symcheck(id, INSTANCE_SYMBOL, context);
  if (sym) {
    if (sym->kind == INSTANCE_SYMBOL) {
      id->code = sym->code;
      atr = findAttribute(sym->fields.entity.props->attributes, attribute);
      if (atr == NULL)
	lmLog(&attribute->srcp, 315, sevERR, id->string);
    } else if (sym->kind == PARAMETER_SYMBOL) {
      if (sym->fields.parameter.class != NULL) {
	if (inheritsFrom(sym->fields.parameter.class, literalSymbol)) {
	  lmLog(&attribute->srcp, 406, sevERR, "");
	} else {
	  Symbol *classOfParameter = sym->fields.parameter.class;
	  atr = findAttribute(classOfParameter->fields.entity.props->attributes, attribute);
	  if (atr == NULL)
	    lmLogv(&attribute->srcp, 316, sevERR, attribute->string, "parameter",
		   id->string, classOfParameter->string, NULL);
	}
      }
    } if (sym->kind == LOCAL_SYMBOL) {
      if (sym->fields.local.class) {
	Symbol *classOfLocal = sym->fields.local.class;
	atr = findAttribute(classOfLocal->fields.entity.props->attributes, attribute);
	if (atr == NULL)
	  lmLogv(&attribute->srcp, 316, sevERR, attribute->string, "variable",
		 id->string, classOfLocal->string, NULL);
      }
    }
    return atr;
  } else /* no symbol found */
    return NULL;
}


/*----------------------------------------------------------------------

  resolveActorAttribute()

  Resolve an attribute reference for reference to current Actor.

*/
static Attribute *resolveActorAttribute(IdNode *attribute, Context *context)
{
  Attribute *atr = NULL;

  atr = findAttribute(actorSymbol->fields.entity.props->attributes, attribute);
  if (atr == NULL)
    lmLogv(&attribute->srcp, 314, sevERR, attribute->string, "actor", NULL);
  return atr;
}


/*----------------------------------------------------------------------

  resolveLocationAttribute()

  Resolve an attribute reference for reference to current Location.

*/
static Attribute *resolveLocationAttribute(IdNode *attribute, Context *context)
{
  Attribute *atr = NULL;

  atr = findAttribute(locationSymbol->fields.entity.props->attributes, attribute);
  if (atr == NULL)
    lmLogv(&attribute->srcp, 314, sevERR, attribute->string, "location", NULL);
  return atr;
}


/*----------------------------------------------------------------------

  resolveThisAttribute()

  Resolve an attribute reference for reference to THIS instance.

*/
static Attribute *resolveThisAttribute(IdNode *attribute, Context *context)
{
  Attribute *atr = NULL;
  Context *thisContext = context;
  Bool contextFound = FALSE;

  while (!contextFound && thisContext != NULL) {
    switch (thisContext->kind) {
    case CLASS_CONTEXT:
      if (thisContext->class == NULL) syserr("Context->class == NULL in resolveThisAttribute()");
      
      atr = findAttribute(thisContext->class->props->attributes, attribute);
      contextFound = TRUE;
      break;
      
    case INSTANCE_CONTEXT:
      if (thisContext->instance == NULL) syserr("context->instance == NULL in resolveThisAttribute()");
      
      atr = findAttribute(thisContext->instance->props->attributes, attribute);
      contextFound = TRUE;
      break;
      
    default:
      thisContext = thisContext->previous;
    }
  }
  if (!contextFound)
    lmLog(&attribute->srcp, 421, sevERR, "");
  else if (atr == NULL)
    lmLog(&attribute->srcp, 313, sevERR, attribute->string);
  return atr;
}


/*======================================================================

  resolveAttributeReference()

  Analyze a reference to an attribute. Will handle static identifiers and
  parameters and return a reference to the attribute node, if all is well.

 */
Attribute *resolveAttributeReference(What *what, IdNode *attribute, Context *context)
{
  switch (what->kind) {
  case WHAT_ID: return resolveIdAttribute(what->id, attribute, context); break;
  case WHAT_ACTOR: return resolveActorAttribute(attribute, context); break;
  case WHAT_LOCATION: return resolveLocationAttribute(attribute, context); break;
  case WHAT_THIS: return resolveThisAttribute(attribute, context); break;
  default: syserr("Unexpected switch in resolveAttrbuteReference()");
  }
  return NULL;
}


/*----------------------------------------------------------------------

  generateAttribute()

  */
static void generateAttribute(Attribute *attribute)
{
  AttributeEntry entry;
  Attribute *new;

  if (attribute->type == STRING_TYPE) {
    if (!attribute->encoded)
      encode(&attribute->fpos, &attribute->len);
    attribute->encoded = TRUE;
    attribute->address = emadr(); /* Record on which Aadress to put it */

    /* Now make a copy to use for initialisation if attribute is
       inherited, else the address will be overwritten by generation
       of other instances of the same attribute */
    new = newAttribute(&attribute->srcp, STRING_TYPE, NULL, attribute->value,
		 attribute->fpos, attribute->len);
    new->address = attribute->address;
    adv.stratrs = concat(adv.stratrs, new, ATTRIBUTE_LIST);
  }

  entry.code = attribute->id->code;
  entry.value = attribute->value;
  entry.stringAddress = attribute->stringAddress;
  emitEntry(&entry, sizeof(entry));
}


/*======================================================================

  generateAttributes()

  Generate all entries in an attribute list.

 */
Aword generateAttributes(List *atrs) /* IN - List of attribute nodes */
{
  Aaddr adr;
  List *lst;

  /* First generate the names of the attributes if needed */ 
  if ((Bool) opts[OPTDEBUG].value) {
    for (lst = atrs; lst != NULL; lst = lst->next) {
      lst->element.atr->stringAddress = emadr();
      emitString(lst->element.atr->id->string);
    }
  }

  adr = emadr();

  for (lst = atrs; lst != NULL; lst = lst->next)
    generateAttribute(lst->element.atr);
  emit(EOF);

  return(adr);
}



/*======================================================================

  generateStringInit()

  Generate initialisation value table for string attributes.

 */
Aaddr generateStringInit(void)
{
  List *atrs;
  Aaddr adr = emadr();

  for (atrs = adv.stratrs; atrs != NULL; atrs = atrs->next) {
    emit(atrs->element.atr->fpos);
    emit(atrs->element.atr->len);
    emit(atrs->element.atr->address);
  }
  emit(EOF);
  return adr;
}


/*----------------------------------------------------------------------

  dumpInheritance()

*/
static void dumpInheritance(AttributeInheritance inheritance)
{
  switch (inheritance) {
  case UNKNOWN_INHERITANCE: put("UNKNOWN"); break;
  case LOCAL: put("LOCAL"); break;
  case INHERITED_REDEFINED: put("INHERITED/REDEFINED"); break;
  case INHERITED: put("INHERITED"); break;
  }
}


/*======================================================================

  dumpAttribute()

  Dump an Attribute node.

 */
void dumpAttribute(Attribute *atr)
{
  put("ATR: "); dumpSrcp(&atr->srcp); in();
  put("type: "); dumpType(atr->type);
  put(", inheritance: "); dumpInheritance(atr->inheritance); nl();
  put("id: "); dumpId(atr->id); nl();
  put("address: "); dumpAddress(atr->address);
  put(", stringAddress: "); dumpAddress(atr->stringAddress); nl();
  put("value: "); dumpInt(atr->value);
  put(", fpos: "); dumpInt(atr->fpos);
  put(", len: "); dumpInt(atr->len); out();
}
