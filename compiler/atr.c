/*----------------------------------------------------------------------*\

				ATR.C
			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "atr_x.h"


/* IMPORT: */
#include "srcp_x.h"
#include "id_x.h"
#include "adv_x.h"
#include "cla_x.h"
#include "ins_x.h"
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

int attributeAreaSize = 0;	/* # of Awords needed for attribute storage */


/*----------------------------------------------------------------------*/
static Attribute *newAttribute(Srcp *srcp, /* IN - Source Position */
			TypeKind type,	/* IN - Type of this atribute */
			IdNode *id,	/* IN - The id */
			int value,      /* IN - The initial value */
			long int fpos,	/* IN - File pos. for initial string */
			int len,	/* IN - D:o length */
			IdNode *instance, List *set) /* IN - Initial instance */
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
  new->instance = instance;
  new->set = set;

  return(new);
}


/*======================================================================*/
Attribute *newBooleanAttribute(Srcp srcp, IdNode *id, Bool value)
{
  Attribute *new;			/* The newly allocated area */

  new = newAttribute(&srcp, BOOLEAN_TYPE, id, value, 0, 0, NULL, NULL);

  return(new);
}


/*======================================================================*/
Attribute *newStringAttribute(Srcp srcp, IdNode *id, long fpos, int len)
{
  Attribute *new;			/* The newly allocated area */

  new = newAttribute(&srcp, STRING_TYPE, id, 0, fpos, len, NULL, NULL);

  return(new);
}


/*======================================================================*/
Attribute *newIntegerAttribute(Srcp srcp, IdNode *id, int value)
{
  Attribute *new;			/* The newly allocated area */

  new = newAttribute(&srcp, INTEGER_TYPE, id, value, 0, 0, NULL, NULL);

  return(new);
}


/*======================================================================*/
Attribute *newReferenceAttribute(Srcp srcp, IdNode *id, IdNode *instance)
{
  Attribute *new;			/* The newly allocated area */

  new = newAttribute(&srcp, INSTANCE_TYPE, id, 0, 0, 0, instance, NULL);

  return(new);
}


/*======================================================================*/
Attribute *newSetAttribute(Srcp srcp, IdNode *id, List *set)
{
  Attribute *new;			/* The newly allocated area */

  new = newAttribute(&srcp, SET_TYPE, id, 0, 0, 0, NULL, set);

  return(new);
}


/*----------------------------------------------------------------------*/
static void checkMultipleAttributes(List *atrs)
{
  List *al1;
  List *al2;

  TRAVERSE(al1, atrs) {
    Attribute *thisAttribute = al1->element.atr;
    /* Check multiple declaration */
    TRAVERSE(al2, al1->next) {
      Attribute *nextAttribute = al2->element.atr;
      if (equalId(thisAttribute->id, nextAttribute->id))
	  lmLog(&nextAttribute->id->srcp, 218, sevERR, nextAttribute->id->string);
    }
  }
}


/*======================================================================*/
void symbolizeAttributes(List *atrs)
{
  List *al;

  checkMultipleAttributes(atrs);

  TRAVERSE(al, atrs) {
    Attribute *thisAttribute = al->element.atr;
    if (thisAttribute->type == INSTANCE_TYPE) {
      symbolizeId(thisAttribute->instance);
      if (thisAttribute->instance->symbol)
	if (thisAttribute->instance->symbol->kind != INSTANCE_SYMBOL)
	  lmLogv(&thisAttribute->instance->srcp, 428, sevERR, "Attribute value in reference attribute declaration", "an instance", NULL);
    }
  }
}


/*======================================================================*/
Attribute *findAttribute(List *attributes, IdNode *id)
{
  List *this;

  TRAVERSE(this, attributes)
    if (equalId(this->element.atr->id, id))
      return this->element.atr;
  return NULL;
}


/*======================================================================*/
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
	  syserr("Sorting multiple attributes with same code.", NULL);
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



/*----------------------------------------------------------------------*/
static Attribute *copyAttribute(Attribute *theOriginal)
{
  Attribute *theCopy = NEW(Attribute);

  memcpy(theCopy, theOriginal, sizeof(Attribute));
  theCopy->inheritance = INHERITED;
  return theCopy;
}


/*----------------------------------------------------------------------*/
static List *copyAttributeList(List *theOriginal)
{
  List *theCopy = NULL;
  List *traversal;

  for (traversal = theOriginal; traversal != NULL; traversal = traversal->next)
    theCopy = concat(theCopy, copyAttribute(traversal->element.atr),
		     ATTRIBUTE_LIST);
  return theCopy;
}



/*======================================================================*/
List *combineAttributes(List *ownAttributes, List *attributesToAdd)
{
  /* Insert all attributes from the list to add that are not there
     already, then sort the list.

     NOTE! that we use the codes to combine, so this can't be used
     before attributes have been numbered!!
  */

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


/*----------------------------------------------------------------------*/
static void analyzeSetAttribute(Attribute *thisAttribute)
{
  List *elements;
  TypeKind inferedType = UNINITIALIZED_TYPE;
  Symbol *inferedClass = NULL;

  if (thisAttribute->set == NULL)
    lmLog(&thisAttribute->srcp, 413, sevERR, "");
  else {
    TRAVERSE(elements, thisAttribute->set) {
      Expression *exp = elements->element.exp;
      analyzeExpression(exp, NULL);
      if (inferedType == UNINITIALIZED_TYPE)
	inferedType = exp->type;
      if (!equalTypes(inferedType, exp->type))
	lmLogv(&exp->srcp, 408, sevERR, "Expressions", "Set attribute", "the same", NULL);
      else if (exp->type == ERROR_TYPE)
	inferedType = ERROR_TYPE;
      else
	switch (exp->type) {
	case INSTANCE_TYPE:
	  if (inferedClass == NULL)
	    inferedClass = exp->class;
	  else {
	    while (!inheritsFrom(inferedClass, exp->class) && !inheritsFrom(exp->class, inferedClass)) {
	      /* They are not of the same class so we need to find a common ancestor */
	      inferedClass = inferedClass->fields.entity.parent;
	      if (inferedClass == NULL)
		SYSERR("No common ancestor found for Set members");
	    }
	  }
	  break;
	case INTEGER_TYPE:
	  inferedClass = integerSymbol;
	  break;
	case STRING_TYPE:
	  lmLogv(&exp->srcp, 410, sevERR, "Set initialization", "integers or instance references", NULL);
	  break;
	default:
	  SYSERR("Unexpected type kind");
	  break;
	}
    }
    thisAttribute->setType = inferedType;
    if (inferedType == ERROR_TYPE)
      thisAttribute->type = ERROR_TYPE;
    thisAttribute->setClass = inferedClass;
  }
}


/*======================================================================*/
void analyzeAttributes(List *atrs)
{
  List *al1;

  TRAVERSE (al1, atrs) {
    Attribute *thisAttribute = al1->element.atr;
    if (thisAttribute->type == INSTANCE_TYPE) {
      if (thisAttribute->instance->symbol != NULL)
	thisAttribute->value = thisAttribute->instance->symbol->code;
    } else if (thisAttribute->type == SET_TYPE)
      analyzeSetAttribute(thisAttribute);
  }
}


/*======================================================================*/
void analyzeAllAttributes() {
  analyzeAllClassAttributes();
  analyzeAllInstanceAttributes();
}


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeOfId(IdNode *id, IdNode *attribute,
				       Context *context)
{
  Attribute *atr = NULL;
  Symbol *sym;

  sym = id->symbol;
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


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeOfActor(IdNode *attribute, Context *context)
{
  /* Resolve an attribute reference for reference to current Actor. */

  Attribute *atr = NULL;

  atr = findAttribute(actorSymbol->fields.entity.props->attributes, attribute);
  if (atr == NULL)
    lmLogv(&attribute->srcp, 314, sevERR, attribute->string, "Actor", "actor", NULL);
  return atr;
}


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeOfLocation(IdNode *attribute, Context *context)
{
  /* Resolve an attribute reference for reference to current Location. */

  Attribute *atr = NULL;

  atr = findAttribute(locationSymbol->fields.entity.props->attributes, attribute);
  if (atr == NULL)
    lmLogv(&attribute->srcp, 314, sevERR, attribute->string, "Location", "location", NULL);
  return atr;
}


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeOfThis(IdNode *attribute, Context *context)
{
  Attribute *atr = NULL;
  Context *thisContext = context;
  Bool contextFound = FALSE;

  while (!contextFound && thisContext != NULL) {
    switch (thisContext->kind) {
    case CLASS_CONTEXT:
      if (thisContext->class == NULL)
	SYSERR("Context->class == NULL");
      
      atr = findAttribute(thisContext->class->props->attributes, attribute);
      contextFound = TRUE;
      break;
      
    case INSTANCE_CONTEXT:
      if (thisContext->instance == NULL)
	SYSERR("context->instance == NULL");
      
      atr = findAttribute(thisContext->instance->props->attributes, attribute);
      contextFound = TRUE;
      break;
      
    default:
      thisContext = thisContext->previous;
    }
  }
  /* If no context found then THIS is not defined here which we should
     already have reported. Report of the attribute was not found. */
  if (contextFound && atr == NULL)
    lmLog(&attribute->srcp, 313, sevERR, attribute->string);
  return atr;
}


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeToClass(Symbol *class, IdNode *attribute, Context *context) {
  Attribute *atr = NULL;

  if (class != NULL) {
    atr = findAttribute(class->fields.entity.props->attributes, attribute);
    if (!atr)
      lmLogv(&attribute->srcp, 317, sevERR, attribute->string, "the expression",
	     class->string, NULL);
  }
  return atr;
}


/*----------------------------------------------------------------------*/
static Attribute *resolveAttributeToWhat(What *what, IdNode *attribute, Context *context)
{
  /* Analyze a reference to an attribute. Will handle static identifiers and
     parameters and return a reference to the attribute node, if all is well. */

  switch (what->kind) {
  case WHAT_ID: return resolveAttributeOfId(what->id, attribute, context); break;
  case WHAT_ACTOR: return resolveAttributeOfActor(attribute, context); break;
  case WHAT_LOCATION: return resolveAttributeOfLocation(attribute, context); break;
  case WHAT_THIS: return resolveAttributeOfThis(attribute, context); break;
  default: SYSERR("Unexpected switch");
  }
  return NULL;
}


/*======================================================================*/
Attribute *resolveAttribute(Expression *exp, IdNode *attributeId, Context *context) {
  switch (exp->kind) {
  case WHAT_EXPRESSION:
    return resolveAttributeToWhat(exp->fields.wht.wht, attributeId, context);
  case ATTRIBUTE_EXPRESSION:
    return resolveAttributeToClass(exp->class, attributeId, context);
  default:
    SYSERR("Unexpected expression kind");
  }
  return NULL;
}

/*----------------------------------------------------------------------*/
static void generateAttribute(Attribute *attribute)
{
  AttributeEntry entry;
  Attribute *new;

  if (attribute->type == STRING_TYPE || attribute->type == SET_TYPE) {
    if (attribute->type == STRING_TYPE) {
      if (!attribute->encoded) {
	encode(&attribute->fpos, &attribute->len);
	attribute->encoded = TRUE;
      }
      /* Now make a copy to use for initialisation if attribute is
	 inherited, else the address will be overwritten by generation
	 of other instances of the same attribute */
      new = newStringAttribute(attribute->srcp, NULL, attribute->fpos, attribute->len);
      adv.stringAttributes = concat(adv.stringAttributes, new, ATTRIBUTE_LIST);
    } else {			/* SET ATTRIBUTE */
      /* Make a copy to keep the address in */
      new = newSetAttribute(attribute->srcp, NULL, attribute->set);
      new->setType = attribute->setType;
      adv.setAttributes = concat(adv.setAttributes, new, ATTRIBUTE_LIST);
    }
    new->address = nextEmitAddress(); /* Record on which Aadress to put it */
  }

  entry.code = attribute->id->code;
  entry.value = attribute->value;
  entry.stringAddress = attribute->stringAddress;
  emitEntry(&entry, sizeof(entry));
}


/*======================================================================*/
Aword generateAttributes(List *atrs) /* IN - List of attribute nodes */
{
  Aaddr adr;
  List *lst;

  /* First generate the names of the attributes if needed */ 
  if ((Bool) opts[OPTDEBUG].value) {
    for (lst = atrs; lst != NULL; lst = lst->next) {
      lst->element.atr->stringAddress = nextEmitAddress();
      emitString(lst->element.atr->id->string);
    }
  }

  adr = nextEmitAddress();

  for (lst = atrs; lst != NULL; lst = lst->next) {
    generateAttribute(lst->element.atr);
    attributeAreaSize++;
  }
  emit(EOF);
  attributeAreaSize++;

  return(adr);
}



/*======================================================================*/
Aaddr generateStringInit(void)
{
  /* Generate initialisation value table for string attributes. */

  List *atrs;
  StringInitEntry entry;
  Aaddr adr = nextEmitAddress();

  for (atrs = adv.stringAttributes; atrs != NULL; atrs = atrs->next) {
    entry.fpos = atrs->element.atr->fpos;
    entry.len = atrs->element.atr->len;
    entry.adr = atrs->element.atr->address;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);
  return adr;
}


/*----------------------------------------------------------------------*/
static Aaddr generateSet(Attribute *atr)
{
  /* Generate initial set for one attribute */

  List *elements;
  Aaddr adr = nextEmitAddress();

  if (atr->setType == STRING_TYPE)
    SYSERR("Can't generate STRING sets yet,");

  TRAVERSE (elements, atr->set)
    switch (atr->setType) {
    case INSTANCE_TYPE: emit(symbolOfExpression(elements->element.exp, NULL)->code); break;
    case INTEGER_TYPE: emit(elements->element.exp->fields.val.val); break;
    default: SYSERR("Unexpected attribute type");
    }
  emit(EOF);

  return adr;
}


/*======================================================================*/
Aaddr generateSetInit(void)
{
  /* Generate initialisation value table for set attributes. */

  List *atrs;
  SetInitEntry entry;
  Aaddr adr;

  TRAVERSE (atrs, adv.setAttributes)
    atrs->element.atr->setAddress = generateSet(atrs->element.atr);

  adr = nextEmitAddress();
  TRAVERSE (atrs, adv.setAttributes) {
    entry.size = length(atrs->element.atr->set);
    entry.setAddress = atrs->element.atr->setAddress;
    entry.adr = atrs->element.atr->address;
    emitEntry(&entry, sizeof(entry));
  }
  emit(EOF);

  return adr;
}


/*----------------------------------------------------------------------*/
static void dumpInheritance(AttributeInheritance inheritance)
{
  switch (inheritance) {
  case UNKNOWN_INHERITANCE: put("UNKNOWN"); break;
  case LOCAL: put("LOCAL"); break;
  case INHERITED_REDEFINED: put("INHERITED/REDEFINED"); break;
  case INHERITED: put("INHERITED"); break;
  }
}


/*======================================================================*/
void dumpAttribute(Attribute *atr)
{
  put("ATR: "); dumpSrcp(&atr->srcp); indent();
  put("type: "); dumpType(atr->type);
  put(", inheritance: "); dumpInheritance(atr->inheritance); nl();
  put("id: "); dumpId(atr->id); nl();
  put("address: "); dumpAddress(atr->address);
  switch (atr->type) {
  case STRING_TYPE:
    put(", stringAddress: "); dumpAddress(atr->stringAddress);
    put("fpos: "); dumpInt(atr->fpos); nl();
    put(", len: "); dumpInt(atr->len);
    break;
  case INTEGER_TYPE:
    nl();
    put("value: "); dumpInt(atr->value);
    break;
  case BOOLEAN_TYPE:
    nl();
    put("value: "); dumpBool(atr->value);
    break;
  case INSTANCE_TYPE:
    nl();
    put("instance: "); dumpId(atr->instance);
    break;
  case SET_TYPE:
    nl();
    put("setType: "); dumpType(atr->setType); nl();
    if (atr->setType == INSTANCE_TYPE) {
      put("atr->setClass: "); dumpPointer(atr->setClass);
      if (atr->setClass != NULL) {
	put(" \""); put(atr->setClass->string); put("\"");
      }
      nl();
    }
    put("set: "); dumpList(atr->set, EXPRESSION_LIST);
    break;
  default:
    put(", stringAddress: "); dumpAddress(atr->stringAddress);
    put(", fpos: "); dumpInt(atr->fpos);
    put(", len: "); dumpInt(atr->len); nl();
    put("value: "); dumpInt(atr->value); nl();
    put("instance: "); dumpId(atr->instance); nl();
    put("setType: "); dumpType(atr->setType); nl();
    if (atr->setType == INSTANCE_TYPE) {
      put("atr->setClass: "); dumpPointer(atr->setClass);
      if (atr->setClass != NULL) {
	put(" \""); put(atr->setClass->string); put("\"");
      }
      nl();
    }
    put("set: "); dumpList(atr->set, EXPRESSION_LIST);
    break;
  }
  out();

}
