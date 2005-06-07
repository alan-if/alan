/*----------------------------------------------------------------------*\

				FORM.C
				Forms

\*----------------------------------------------------------------------*/

#include "form_x.h"

#include "dump_x.h"


/* PUBLIC: */



/*======================================================================*/
void dumpForm(SayForm form)
{
  switch (form) {
  case SAY_SIMPLE: put("SIMPLE"); break;
  case SAY_DEFINITE: put("DEFINITE"); break;
  case SAY_INDEFINITE: put("INDEFINITE"); break;
  case SAY_PRONOUN: put("PRONOUN"); break;
  }
}


