/*----------------------------------------------------------------------*\

                OPT.C
               Option Handling

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "sysdep.h"

#include "srcp_x.h"
#include "lmList.h"
#include "lmlog.h"

#include "acode.h"

#include "opt.h"		/* OPTIONS */
#include "emit.h"



/* Exports: */

OptDef opts[NOPT] = {
  {ENUMOPT, false, "language", L_ENGLISH}, /* OPTION Language */
  {INTOPT, false, "width", 75},            /* OPTION Width */
  {INTOPT, false, "length", 24},           /* OPTION Length */
  {BOOLOPT, false, "pack", false},         /* OPTION Pack */
  {BOOLOPT, false, "debug", false}         /* OPTION Debug */
};



/* Private: */

/* Enumerated values for Language-option */
static char *enumlang[] = {
  "english",
  "swedish",
  "german",
  NULL
};

/* Option bounds for numeric options */
static struct {
  int min,max;
} optbounds[NOPT] = {
  {  0,   0},
  { 24, 255},
  {  5, 255},
  {  0,   0},
  {  0, 255}
};

/* Table of pointers to enumerated values for enum options */
static char **enumtbl[NOPT] = {
  enumlang,
  NULL,
  NULL,
  NULL,
  NULL
};


/*----------------------------------------------------------------------

  optcode()

  Convert an option name to a code.

  */
static int optcode(char *id)
{
  for (int opt = 0; opt <= NOPT-1; opt++)
    if (compareStrings(opts[opt].name, id) == 0)
      return opt;

  return EOF;
}


/*----------------------------------------------------------------------

  enum2code()

  Convert an option enum name to a code.

  */
static int enum2code(int opt, char *id)
{
  char **names;

  if ((names = enumtbl[opt]) == NULL)
    return EOF;

  for (int i = 0; names[i] != NULL; i++)
    if (compareStrings(names[i], id) == 0)
      return i;

  return EOF;
}


/*======================================================================

  optint()

  Handle an INTEGER option.

  */
void optint(char *id, Srcp *srcp, int val)
{
  int opt;

  if ((opt = optcode(id)) == EOF) {
    lmlog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != INTOPT
      ||  val < optbounds[opt].min
      ||  val > optbounds[opt].max) {
        lmlog(srcp, 602, sevWAR, id);
        return;
      }

  if (opts[opt].used) {
    lmlog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = true;
  opts[opt].value = val;
}


/*======================================================================

  optenum()

  Handle an ENUM option.

  */
void optenum(char *id, Srcp *srcp, char *val)
{
  int opt, code;

  if ((opt = optcode(id)) == EOF) {
    if (compareStrings(id, "no") == 0) {
      /* This was actually a NO optBool */
      optBool(val, srcp, false);
    } else
      /* Unrecognized option */
      lmlog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != ENUMOPT
      || (code = enum2code(opt, val)) == EOF) {
    lmlog(srcp, 602, sevWAR, id);
    return;
    }

  if (opts[opt].used) {
    lmlog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = true;
  opts[opt].value = code;
}


/*======================================================================

  optbool()

  Handle a BOOLEAN option.

  */
void optBool(char *id, Srcp *srcp, int val)
{
  int opt;

  if ((opt = optcode(id)) == EOF) {
    lmlog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != BOOLOPT) {
    lmlog(srcp, 602, sevWAR, id);
    return;
  }

  if (opts[opt].used) {
    lmlog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = true;
  opts[opt].value = val;
}


/*======================================================================*/
void generateOptions(ACodeHeader *header)
{
  header->pageLength = opts[OPTLEN].value;
  header->pageWidth = opts[OPTWIDTH].value;
  header->pack = opts[OPTPACK].value;
  header->debug = opts[OPTDEBUG].value;
}
