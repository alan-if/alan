/*----------------------------------------------------------------------*\

				NAM.C
			 Name Nodes handling

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"

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
#ifdef _PROTOTYPES_
NamNod *newnam(Srcp *srcp, char *str)
                		/* IN - Source Position */
                		/* IN - The string */
#else
NamNod *newnam(srcp, str)
     Srcp *srcp;		/* IN - Source Position */
     char str[];		/* IN - The string */
#endif
{
  NamNod *new;			/* The newly allocated area */

  new = NEW(NamNod);
  
  new->srcp = *srcp;
  new->str  = newstr(str);
  new->kind = NAMUNK;		/* Unknown yet... */
  new->code = -1;

  return(new);
}



/*======================================================================

  eqnams()

  Check if two names matches.

  */
#ifdef _PROTOTYPES_
Bool eqnams(NamNod *nam1, NamNod *nam2)
                  		/* IN - The first name */
                  		/* IN - Second name */
#else
Bool eqnams(nam1, nam2)
     NamNod *nam1;		/* IN - The first name */
     NamNod *nam2;		/* IN - Second name */
#endif
{
  return (strcmp(nam1->str, nam2->str) == 0);
}


/*======================================================================

  annams()

  Analyse a list of names (output an appropriate text).

  */
#ifdef _PROTOTYPES_
int annams(List *nams, NamNod *nam, Bool cap)
                		/* IN - List of names to use */
                 		/* IN - A single name if nams == NULL */
                 		/* IN - Capitalize the words? */
#else
int annams(nams, nam, cap)
     List *nams;		/* IN - List of names to use */
     NamNod *nam;		/* IN - A single name if nams == NULL */
     Bool cap;		/* IN - Capitalize the words? */
#endif
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
	toIso(buf, buf);
      } else
	toIso(buf, namlst->element.nam->str);
      if (namlst->next)
	strcat(buf, " ");
      getxt(buf);
      len = len + strlen(buf);
    }
  } else {
    if (cap) {
      sprintf(buf, "%c%s", toUpper(nam->str[0]), &nam->str[1]);
      toIso(buf, buf);
    } else
      toIso(buf, nam->str);
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
#ifdef _PROTOTYPES_
void genam(NamNod *nam)
                 		/* IN - The name to generate for */
#else
void genam(nam)
     NamNod *nam;		/* IN - The name to generate for */
#endif
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
    unimpl(&nam->srcp, "Code Generator");
    break;
  }
}


/*======================================================================

  dunam()

  Dump a NAM-node.

  */
#ifdef _PROTOTYPES_
void dunam(NamNod *nam)
#else
void dunam(nam)
     NamNod *nam;
#endif
{
  if (nam == NULL) {
    put("NULL");
    return;
  }

  put("NAM: "); dusrcp(&nam->srcp); in();
  put("string: "); dustr(nam->str); nl();
  put("kind: ");
  switch (nam->kind) {
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
  } nl();
  put("code: "); duint(nam->code); out();
}
