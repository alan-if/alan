/*----------------------------------------------------------------------*\

				NAM.C
			    Names handling

\*----------------------------------------------------------------------*/

#include "nam_x.h"

#include "alan.h"
#include "lst.h"
#include "msg.h"

#include "acode.h"
#include "emit.h"


/*======================================================================

  annams()

  Analyse a list of names (output an appropriate text).

  */
int annams(List *nams,		/* IN - List of names to use */
	   IdNode *id,		/* IN - An identifier if nams == NULL */
	   Bool cap)		/* IN - Capitalize the words? */
{
  List *namlst;
  char buf[80];
  int len = 0;

  if (nams != NULL) {
    for (namlst = nams->element.lst; namlst != NULL; namlst = namlst->next) {
      if (strlen(namlst->element.id->string) > 79)
	namlst->element.id->string[79] = '\0';
      if (cap) {
	sprintf(buf, "%c%s", toUpperCase(namlst->element.id->string[0]),
		&namlst->element.id->string[1]);
	toIso(buf, buf, charset);
      } else
	toIso(buf, namlst->element.id->string, charset);
      if (namlst->next)
	strcat(buf, " ");
      getxt(buf);
      len = len + strlen(buf);
    }
  } else {
    if (cap) {
      sprintf(buf, "%c%s", toUpper(id->string[0]), &id->string[1]);
      toIso(buf, buf, charset);
    } else
      toIso(buf, id->string, charset);
    getxt(buf);
    len = strlen(buf);
  }
  return(len);
}


