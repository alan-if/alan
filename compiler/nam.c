/*----------------------------------------------------------------------*\

				NAM.C
			 Name Nodes handling

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lmList.h"

#include "lst.h"
#include "nam.h"
#include "str.h"

#include "obj.h"
#include "act.h"
#include "cnt.h"
#include "ext.h"
#include "loc.h"
#include "vrb.h"
#include "msg.h"

#include "dump.h"
#include "acode.h"
#include "emit.h"




/*======================================================================

  newnam()

  Allocates and initialises a namnod.

  */
NamNod *newnam(Srcp *srcp,	/* IN - Source Position */
	       char *str)	/* IN - The string */
{
  NamNod *new;			/* The newly allocated area */

  if (verbose) { printf("%8ld\b\b\b\b\b\b\b\b", counter++); fflush(stdout); }

  new = NEW(NamNod);
  
  new->srcp = *srcp;
  new->str  = newstr(str);
  new->kind = NAMUNK;		/* Unknown yet... */
  new->code = -1;

  return(new);
}



/*============================================================================

  namstr()

  Create a string indicating the types of names in the set.

  */
extern char *namstr(NamKind syms) /* IN - A set of name types */
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
  static char str[255];         /* To hold the string */
  Bool found = FALSE;
  int i;

  /* 4f_Hack to not have repetitions */
  if (syms & NAMACT) syms &= ~NAMCACT;
  if (syms & NAMOBJ) syms &= ~NAMCOBJ;

  str[0] = '\0';                /* Clear the string */
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

  namcheck()

  Check if a name is known, if so return its kind else give an error
  message indicating what it should have been. Also check the possible
  parameters, if any of those match use it instead. Will set elm resp.
  sym depending on which one was used if no error.

  */
void namcheck(
    SymNod **sym,               /* OUT - Found symbol */
    ElmNod **elm,               /* OUT - Found parameter  */
    IdNod *id,			/* IN - The name to check */
    NamKind classes,            /* IN - A set of allowed symbol classes */
    NamKind props,              /* IN - Set to NAMCNT if container properties required, NAMANY otherwise */
    List *pars                  /* IN - Possible parameters valid in this context */
)
{
  List *lst;                    /* Parameter traversal pointer*/
  NamKind elmclasses;           /* Classes defined for the parameter */

  *sym = NULL;
  *elm = NULL;
  if (id == NULL)
    return;

  /* Look it up, as a symbol and in the parameter list */
  *sym = lookup(id->string);
  *elm = NULL;                  /* Clear it first */
  for (lst = pars; lst; lst = lst->next) /* Then search for parameter */
    if (eqids(lst->element.elm->id, id)) {
      *elm = lst->element.elm;
      break;
    }

  /* Not found? */
  if (*sym == NULL && *elm == NULL) {
    /* Ids generated during error recovery start with '$' */
    if (id->string[0] != '$')
      lmLog(&id->srcp, 310, sevERR, id->string);
    newsym(id->string, NAMUNK, NULL);
  } else if (*elm) {
    if (*sym && ((*sym)->class != NAMUNK)) {
      /* Parameter overrides symbol */
      lmLog(&id->srcp, 212, sevINF, id->str);
      *sym = NULL;
    }
    nam->kind = NAMPAR;         /* Remember it is a parameter */
    nam->code = (*elm)->no;
    /* Check if classes match */
    if ((*elm)->res == NULL) {
      /* No restrictions defined, can only be an OBJECT */
      if ((classes & NAMOBJ) == 0) {
        lmLog(&nam->srcp, 312, sevERR, namstr(classes));
        *elm = NULL;
      } else
	/* Check properties */
	if (props != NAMANY) {
	  lmLog(&nam->srcp, 312, sevERR, namstr(props));
	  *elm = NULL;
	}
    } else {
      /* Find its defined classes and check against them */
      elmclasses = (*elm)->res->classbits & (~NAMCNT); /* Ignore container prop */

      if ((classes & elmclasses) != elmclasses ) {
	/* "Parameter not uniquely defined as a" */
        lmLog(&nam->srcp, 312, sevERR, namstr(classes));
        *elm = NULL;
      } else {
	/* If it was uniquely defined as any type of literal say so */
	if (elmclasses & NAMNUM)
	  nam->kind = NAMNUM;
	else if (elmclasses & NAMSTR)
	  nam->kind = NAMSTR;
	else {
	  /* Check properties */
	  if (props != NAMANY) {
	    if ((elmclasses & NAMCACT) == 0 && (elmclasses & NAMCOBJ) == 0 &&
		((*elm)->res->classbits & props) != props)
	      lmLog(&nam->srcp, 312, sevERR, namstr(props));
	    *elm = NULL;
	  }
	}
      }
    }
  } else if ((*sym)->class != NAMUNK) {
    if ((classes & (*sym)->class) == 0) {
      lmLog(&nam->srcp, 311, sevERR, namstr(classes));
      *sym = NULL;
    } else {
      id->code = (*sym)->code;
      id->kind = (*sym)->class;
    }
  }
}


/*======================================================================

  eqnams()

  Check if two names matches.

  */
Bool eqnams(NamNod *nam1,	/* IN - The first name */
	    NamNod *nam2)	/* IN - Second name */
{
  return (strcmp(nam1->str, nam2->str) == 0);
}


/*======================================================================

  annams()

  Analyse a list of names (output an appropriate text).

  */
int annams(List *nams,		/* IN - List of names to use */
	   NamNod *nam,		/* IN - A single name if nams == NULL */
	   Bool cap)		/* IN - Capitalize the words? */
{
  List *namlst;
  char buf[80];
  int len = 0;

  if (nams != NULL) {
    for (namlst = nams; namlst != NULL; namlst = namlst->next) {
      if (strlen(namlst->element.nam->str) > 79)
	namlst->element.nam->str[79] = '\0';
      if (cap) {
	sprintf(buf, "%c%s", toUpperCase(namlst->element.nam->str[0]),
		&namlst->element.nam->str[1]);
	toIso(buf, buf, charset);
      } else
	toIso(buf, namlst->element.nam->str, charset);
      if (namlst->next)
	strcat(buf, " ");
      getxt(buf);
      len = len + strlen(buf);
    }
  } else {
    if (cap) {
      sprintf(buf, "%c%s", toUpper(nam->str[0]), &nam->str[1]);
      toIso(buf, buf, charset);
    } else
      toIso(buf, nam->str, charset);
    getxt(buf);
    len = strlen(buf);
  }
  return(len);
}


/*======================================================================

  genam()

  Generate code for a NAM. Normally just emit the code, but if it is a
  parameter, get that parameter.

  */
void genam(NamNod *nam)		/* IN - The name to generate for */
{
  switch (nam->kind) {
  case NAMPAR:
    emit0(C_CONST, nam->code);
    emit0(C_CURVAR, V_PARAM);
    break;
  case NAMNUM:			/* For a literal parameter, get its value */
    emit0(C_CONST, 1);		/* First attribute is the value */
    emit0(C_CONST, nam->code);
    emit0(C_CURVAR, V_PARAM);
    emit0(C_STMOP, I_ATTRIBUTE);
    break;
  case NAMSTR:			/* For a string parameter, get its value */
    emit0(C_CONST, 1);		/* First attribute is the value */
    emit0(C_CONST, nam->code);
    emit0(C_CURVAR, V_PARAM);
    emit0(C_STMOP, I_STRATTR);
    break;
  case NAMOBJ:
    emit0(C_CONST, nam->code-1+objmin);
    break;
  case NAMACT:
    emit0(C_CONST, nam->code-1+actmin);
    break;
  case NAMCNT:
    emit0(C_CONST, nam->code-1+cntmin);
    break;
  case NAMDIR:
    emit0(C_CONST, nam->code-1+dirmin);
    break;
  case NAMLOC:
    emit0(C_CONST, nam->code-1+locmin);
    break;
  case NAMVRB:
    emit0(C_CONST, nam->code-1+vrbmin);
    break;
  case NAMATR:
  case NAMEVT:
  case NAMRUL:
    emit0(C_CONST, nam->code);
    break;
  default:
    emit0(C_CONST, 0);
    unimpl(&nam->srcp, "Code Generator");
    break;
  }
}



/*======================================================================

  dumpNamKind()

  Dump a NAM kind.

  */
void dumpNamKind(NamKind kind)
{
  switch (kind) {
  case NAMUNK: put("UNKNOWN"); break;
  case NAMOBJ: put("OBJECT"); break;
  case NAMACT: put("ACTOR"); break;
  case NAMCNT: put("CONTAINER"); break;
  case NAMDIR: put("DIRECTION"); break;
  case NAMLOC: put("LOCATION"); break;
  case NAMVRB: put("VERB"); break;
  case NAMATR: put("ATTRIBUTE"); break;
  case NAMEVT: put("EVENT"); break;
  case NAMPAR: put("PARAMETER"); break;
  case NAMWRD: put("WORD"); break;
  case NAMNUM: put("NUMBER"); break;
  case NAMSTR: put("STRING"); break;
  default: put("*** ERROR ***"); break;
  }
}


/*======================================================================

  dunam()

  Dump a NAM-node.

  */
void dunam(NamNod *nam)
{
  if (nam == NULL) {
    put("NULL");
    return;
  }

  put("NAM: "); dusrcp(&nam->srcp); in();
  put("string: "); dustr(nam->str); nl();
  put("kind: "); dumpNamKind(nam->kind); nl();
  put("code: "); duint(nam->code); out();
}
