/*----------------------------------------------------------------------*\

				SYM.C
			  Symbol Table Nodes

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "sym.h"		/* SYM-nodes */
#include "nam.h"		/* NAM-nodes */
#include "vrb.h"		/* VRB-nodes */
#include "ext.h"		/* EXT-nodes */
#include "elm.h"		/* ELM-nodes */
#include "loc.h"		/* LOC-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "act.h"		/* ACT-nodes */
#include "evt.h"		/* EVT-nodes */



static SymNod *symtree = NULL;



/*======================================================================

  redefined()

  Calls lmLog() with correct code according to the symnod sent.

  */
#ifdef _PROTOTYPES_
void redefined(Srcp *srcp, SymNod *sym, char *str)
                		/* IN - Source position */
                 		/* IN - The previous definition */
                		/* IN - The symbol name */
#else
void redefined(srcp, sym, str)
     Srcp *srcp;		/* IN - Source position */
     SymNod *sym;		/* IN - The previous definition */
     char str[];		/* IN - The symbol name */
#endif
{
  int code;			/* Error code */

  switch (sym->class) {
  case NAMDIR: code = 301; break;
  case NAMLOC: code = 302; break;
  case NAMVRB: code = 303; break;
  case NAMOBJ: code = 304; break;
  case NAMCNT: code = 305; break;
  case NAMACT: code = 306; break;
  default: syserr("Unrecognized switch in redefined()"); break;
  }

  lmLog(srcp, code, sevERR, str);
}



/*======================================================================

  newsym()

  Creates a new symnod and links it in the symtree.

  */
#ifdef _PROTOTYPES_
int newsym(char *str, NamKind class, void *ref)
                		/* IN - Name of the new symbol */
                   		/* IN - and its class */
               			/* IN - Reference to the symbols node */
#else
int newsym(str, class, ref)
     char str[];		/* IN - Name of the new symbol */
     NamKind class;		/* IN - and its class */
     void *ref;			/* IN - Reference to the symbols node */
#endif
{
  SymNod *new;			/* The newly created symnod */
  SymNod *s1,*s2;		/* Traversal pointers */
  int comp;			/* Result of comparison */
  
  if (str == NULL)
    return (0);
  
  new = NEW(SymNod);
  
  new->class = class;
  new->str = str;
  
  new->low = NULL;
  new->high = NULL;
  
  s1 = symtree;
  s2 = NULL;
  
  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(str, s1->str);
    if (comp < 0)
      s1 = s1->low;
    else
      s1 = s1->high;
  }
  
  if (s2 == NULL)
    symtree = new;
  else if(comp < 0)
    s2->low = new;
  else
    s2->high = new;
  
  switch (class) {
  case NAMDIR: new->code = ++dircount; break;
  case NAMLOC: new->code = ++loccount; break;
  case NAMVRB: new->code = ++vrbcount; break;
  case NAMOBJ: new->code = ++objcount; break;
  case NAMEVT: new->code = ++evtcount; break;
  case NAMCNT: new->code = ++cntcount; break;
  case NAMACT: new->code = ++actcount; break;
  default: break;
  }
  
  new->ref = ref;
  return(new->code);
}



/*======================================================================

  lookup()

  Look for a symbol. If found return a pointer to its symnod, else NULL.

  */
#ifdef _PROTOTYPES_
SymNod *lookup(char *str)
                		/* IN - The name to look up */
#else
SymNod *lookup(str)
     char str[];		/* IN - The name to look up */
#endif
{
  SymNod *s1,*s2;		/* Traversal pointers */
  int comp;			/* Result of comparison */

  if (str == NULL) return(NULL);

  s1 = symtree;
  s2 = NULL;

  while (s1 != NULL) {
    s2 = s1;
    comp = strcmp(str, s1->str);
    if (comp == 0)
      return(s1);
    else if (comp < 0)
      s1 = s1->low;
    else
      s1 = s1->high;
  }

  return(NULL);
}



/*----------------------------------------------------------------------

  symstr()

  Create a string indicating the types in the set.

  */
#ifdef _PROTOTYPES_
static char *symstr(NamKind syms) 
                  		/* IN - A set of symbol types */
#else
static char *symstr(syms) 
     NamKind syms;		/* IN - A set of symbol types */
#endif
{
  static char *sym[] = {
    "an Object",
    "a Container",
    "an Actor",
    "an Integer",
    "a String",
    "an Object",
    "an Actor",
    "a Direction",
    "a Location",
    "a Verb",
    "an Attribute",
    "an Event",
    "a Parameter",
    "a Word",
    "a Rule"
    };
  static char str[255];		/* To hold the string */
  Boolean found = FALSE;
  int i;

  /* 4f_Hack to not have repetitions */
  if (syms & NAMACT) syms &= ~NAMCACT;
  if (syms & NAMOBJ) syms &= ~NAMCOBJ;

  str[0] = '\0';		/* Clear the string */
  for (i = 0; i <= 15; i++) {
    if ((syms & (1<<i)) != 0) {
      if (found)
	strcat(str, " or ");
      strcat(str, sym[i]);
      found = TRUE;
    }
  }
  return(str);
}


/*======================================================================

  symcheck()

  Check if a name is known, if so return its kind else give an error
  message indicating what it should have been. Also check the possible
  parameters, if any of those match use it instead. Will set elm resp.
  sym depending on which one was used if no error.

  */
#ifdef _PROTOTYPES_
void symcheck(
    SymNod **sym,		/* OUT - Found symbol */
    ElmNod **elm,		/* OUT - Found parameter  */
    NamNod *nam,		/* IN - The name to check */
    NamKind classes,		/* IN - A set of symbol classes */
    NamKind props,		/* IN - A set of symbol properties */
    List *pars			/* IN - Possible parameters */
)
#else
void symcheck(sym, elm, nam, classes, props, pars)
     SymNod **sym;		/* OUT - Found symbol */
     ElmNod **elm;		/* OUT - Found parameter  */
     NamNod *nam;		/* IN - The name to check */
     NamKind classes;		/* IN - A set of symbol classes */
     NamKind props;		/* IN - A set of symbol properties */
     List *pars;		/* IN - Possible parameters */
#endif
{
  List *lst;			/* Parameter traversal pointer*/
  NamKind elmclasses;		/* Classes defined for the parameter */

  *sym = NULL;
  *elm = NULL;
  if (nam == NULL)
    return;

  /* Look it up, as a symbol and in the parameter list */
  *sym = lookup(nam->str);
  *elm = NULL;			/* Clear it first */
  for (lst = pars; lst; lst = lst->next) /* Then search for parameter */
    if (eqnams(lst->element.elm->nam, nam)) {
      *elm = lst->element.elm;
      break;
    }

  /* Not found? */
  if (*sym == NULL && *elm == NULL) {
    lmLog(&nam->srcp, 310, sevERR, nam->str);
    newsym(nam->str, NAMUNK, NULL);
  } else if (*elm) {
    if (*sym && ((*sym)->class != NAMUNK)) {
      /* Parameter overrides symbol */
      lmLog(&nam->srcp, 212, sevINF, nam->str);
      *sym = NULL;
    }
    nam->kind = NAMPAR;		/* Remember it is a parameter */
    nam->code = (*elm)->no;
    /* Check if classes match */
    if ((*elm)->cla == NULL) {
      /* No definition, assume OBJECT */
      if ((classes & NAMOBJ) == 0) {
	lmLog(&nam->srcp, 312, sevERR, symstr(classes));
	*elm = NULL;
      }
    } else {
      /* Check its defined classes */
      elmclasses = (*elm)->cla->classes & (~NAMCNT); /* Ignore container prop */
      if ((classes & elmclasses) != elmclasses ) {
	lmLog(&nam->srcp, 312, sevERR, symstr(classes));
	*elm = NULL;
      } else {
	/* If it was uniquely defined as any literal say so */
	if (elmclasses & NAMNUM)
	  nam->kind = NAMNUM;
	else if (elmclasses & NAMSTR)
	  nam->kind = NAMSTR;
	else {
	  /* Check properties */
	  if (props != NAMANY) {
	    if ((elmclasses & NAMCACT) == 0 && (elmclasses & NAMCOBJ) == 0 &&
		(*elm)->cla->classes & props != props)
	    lmLog(&nam->srcp, 312, sevERR, symstr(props));
	    *elm = NULL;
	  }
	}
      }
    }
  } else if ((*sym)->class != NAMUNK) {
    if ((classes & (*sym)->class) == 0) {
      lmLog(&nam->srcp, 311, sevERR, symstr(classes));
      *sym = NULL;
    } else {
      nam->code = (*sym)->code;
      nam->kind = (*sym)->class;
    }
  }
}
