/*----------------------------------------------------------------------*\

				NAM.C
			    Names handling

\*----------------------------------------------------------------------*/

#include "nam_x.h"

#include "alan.h"
#include "lst.h"
#include "msg.h"

#include "../interpreter/acode.h"
#include "emit.h"


/*======================================================================*/
int analyzeNames(List *nams, IdNode *id)
{
  List *namlst;
  char buf[80];
  int len = 0;

  if (nams != NULL) {
    for (namlst = nams->element.lst; namlst != NULL; namlst = namlst->next) {
      if (strlen(namlst->element.id->string) > 79)
	namlst->element.id->string[79] = '\0';
	toIso(buf, namlst->element.id->string, charset);
      if (namlst->next)
	strcat(buf, " ");
      generateText(buf);
      len = len + strlen(buf);
    }
  } else {
    toIso(buf, id->string, charset);
    generateText(buf);
    len = strlen(buf);
  }
  return(len);
}


