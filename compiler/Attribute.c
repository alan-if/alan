/*----------------------------------------------------------------------*\

			     Attribute.c

			   Attribute Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "Adventure.h"
#include "Attribute.h"
#include "List.h"
#include "Option.h"

#include "acode.h"
#include "lmList.h"
#include "encode.h"
#include "dump.h"


/* Public: */
int attributeCount = 0;


/*======================================================================

  newAttribute()

  Allocate and initialise an attribute.

 */
#ifdef _PROTOTYPES_
Attribute *newAttribute(Srcp *srcp,	  /* IN - Source Position */
			TypeKind type,	  /* IN - Type of this attributeibute */
			Id *id,		  /* IN - The name */
			int value,	  /* IN - The initial value */
			long fpos,	  /* IN - File position for initial string */
			int len)	  /* IN - D:o length */
#else
Attribute *newAttribute(srcp, type, id, value, fpos, len)
     Srcp *srcp;
     TypeKind type;
     Id *id;
     int value;
     long fpos;
     int len;
#endif

{
  Attribute *new;			/* The newly allocated area */

  new = NEW(Attribute);

  new->srcp = *srcp;
  new->type = type;
  new->id = id;
  new->value = value;
  new->stradr = 0;
  new->encoded = FALSE;
  new->fpos = fpos;
  new->len = len;

  return(new);
}


/*======================================================================

  findAttribute()

  Find an attribute in a list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Attribute *findAttribute(Id *id, /* IN - The attribute id to find */
			 List *attributes) /* IN - List of attribute nodes */
#else
Attribute *findAttribute(id, attributes)
     Id *id;
     List *attributes;
#endif
{
  List *list;

  for (list = attributes; list; list = list->next)
    if (equalIds(id, list->element.attribute->id))
      return (list->element.attribute);
  return NULL;
}



/*======================================================================

  findAttributeInLists()

  Find an attribute in a list of attribute list and return a pointer to it.

 */
#ifdef _PROTOTYPES_
Attribute *findAttributeInLists(Srcp *srcp, /* IN - The source position to point to in case of error */
				Id *id, /* IN - The attribute id to find */
				List *lists) /* IN - The lists of attribute lists */
#else
Attribute *findAttributeInLists(srcp, id, lists)
     Srcp *srcp;
     Id *id;
     List *lists;
#endif
{
  List *list;
  Attribute *found1, *found2 = NULL;

  for (list = lists; list; list = list->next) {
    found1 = findAttribute(id, list->element.list);
    if (found2)
      if (found1 != found2) {
	lmLog(srcp, 229, sevERR, id->string);
	return (found1);
      }
    found2 = found1;
  }
  return (found1);
}



/*======================================================================

  inheritAttributes()

  Inherit attributes from a slot, and update the list 

 */
#ifdef _PROTOTYPES_
void inheritAttributes(Slot *slot, /* IN - The slot to inherit from */
		       List **attributeListsP) /* INOUT - Combine with this */
#else
void inheritAttributes(slot, attributeListsP)
     Slot *slot;
     List **attributeListsP;
#endif
{
  List *inheritedAttributeList;

  if (slot->attributes != NULL)
    inheritedAttributeList = prepend(slot->attributes,
				     copyList(slot->inheritedAttributeLists));
  else
    inheritedAttributeList = copyList(slot->inheritedAttributeLists);
  /* Now the inheritedAttributeList contains all lists of attributes
     inherited from this class */
  if (inheritedAttributeList != NULL)
    *attributeListsP = combine(*attributeListsP, inheritedAttributeList);
}


/*======================================================================

  analyseAttributes()

  Analyse all entries in an attribute list.

 */
#ifdef _PROTOTYPES_
void analyseAttributes(List *attributes) /* IN - List of attribute nodes */
#else
void analyseAttributes(attributes)
     List *attributes;
#endif
{
  /* 4f - analyseAttributes not implemented */
}



/*----------------------------------------------------------------------

  generateAttribute()

  */
#ifdef _PROTOTYPES_
static void generateAttribute(Attribute *attribute) /* IN - Attribute to generate for */
#else
static void generateAttribute(attribute)
     Attribute *attribute;
#endif
{
  Attribute *new;

  /* Format of attribute table: {code, value, string address} */
  emit(attribute->code);
  if (attribute->type == STRING_TYPE) {
    if (!attribute->encoded) {
      encode(&attribute->fpos, &attribute->len);
      attribute->encoded = TRUE;
    }
    attribute->adr = emitAddress();		/* Remember where to put it */
    /* Now make a copy to use for initialisation if attribute is default */
    /* 4f - shouldn't this apply for all inherited attributes ? */
    new = newAttribute(&attribute->srcp, STRING_TYPE, NULL, attribute->value, attribute->fpos, attribute->len);
    new->adr = attribute->adr;
    adventure.stringAttributes = append(adventure.stringAttributes, new);
  }
  emit(attribute->value);
  emit(attribute->stradr);
}


/*======================================================================

  generateAttributes()

  Generate all entries in an attribute list.

 */
#ifdef _PROTOTYPES_
Aword generateAttributes(List *attributes) /* IN - List of attribute nodes */
#else
Aword generateAttributes(attributes)
     List *attributes;
#endif
{
  Aaddr adr;
  List *lst;

  if (attributes == NULL)
    return(0);

  /* First generate the names of the attributes if needed */ 
  if ((Bool) options[DEBUG_OPTION].value) {
    for (lst = attributes; lst != NULL; lst = lst->next) {
      lst->element.attribute->stradr = emitAddress();
      emitString(lst->element.attribute->id->string);
    }
  }

  adr = emitAddress();

  for (lst = attributes; lst; lst = lst->next)
    generateAttribute(lst->element.attribute);
  emit(EOF);

  return(adr);
}



/*======================================================================

  generateStringInit()


  Generate initialisation info for all string attributes in the list.

 */
#ifdef _PROTOTYPES_
Aaddr generateStringInit(void)
#else
Aaddr generateStringInit()
#endif
{
  List *attributes;
  Aaddr adr = emitAddress();

  for (attributes = adventure.stringAttributes; attributes != NULL; attributes = attributes->next) {
    emit(attributes->element.attribute->fpos);
    emit(attributes->element.attribute->len);
    emit(attributes->element.attribute->adr);
  }
  emit(EOF);
  return adr;
}



/*======================================================================

  dumpAttribute()

  Dump an Attribute node.

 */
#ifdef _PROTOTYPES_
void dumpAttribute(Attribute *attribute)
#else
void dumpAttribute(attribute)
     Attribute *attribute;
#endif
{
  put("ATTRIBUTE: "); dumpSrcp(&attribute->srcp); in();
  put("type: "); dumpType(attribute->type); nl();
  put("id: "); dumpId(attribute->id); nl();
  put("code: "); dumpInteger(attribute->code); nl();
  put("stradr: "); dumpInteger(attribute->stradr); nl();
  put("value: "); dumpInteger(attribute->value); nl();
  put("fpos: "); dumpInteger(attribute->fpos); nl();
  put("len: "); dumpInteger(attribute->len); out();
}
