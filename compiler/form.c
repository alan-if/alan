/*----------------------------------------------------------------------*\

				FORM.C
				Forms

\*----------------------------------------------------------------------*/

#include "form_x.h"

#include "dump.h"


/* PUBLIC: */



/*======================================================================*/
void dumpForm(FormType form)
{
  switch (form) {
  case SIMPLE: put("SIMPLE"); break;
  case DEFINITE: put("DEFINITE"); break;
  case INDEFINITE: put("INDEFINITE"); break;
  }
}


