/*----------------------------------------------------------------------*\

				OPT.C
			   Option Handling

\*----------------------------------------------------------------------*/

#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "acode.h"

#include "opt.h"		/* OPTIONS */
#include "emit.h"



/* Exports: */

OptDef opts[NOPT] = {
  {ENUMOPT, FALSE, "language", L_ENGLISH}, /* OPTION Language */
  {INTOPT, FALSE, "width", 75},            /* OPTION Width */
  {INTOPT, FALSE, "length", 24},           /* OPTION Length */
  {BOOLOPT, FALSE, "pack", FALSE},	   /* OPTION Pack */
  {INTOPT, FALSE, "visits", 0},		   /* OPTION Visits */
  {BOOLOPT, FALSE, "debug", FALSE}         /* OPTION Debug */
};



/* Private: */

/* Enumerated values for Language-option */
static char *enumlang[] = {
  "english",
  "swedish7",
  "swedish",
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
#ifdef _PROTOTYPES_
static int optcode(char *id)
#else
static int optcode(id)
     char id[];
#endif
{
  int opt;

  for (opt = 0; opt <= NOPT-1; opt++)
    if (strcmp(opts[opt].name, id) == 0)
      return(opt);

  return(EOF);
}


/*----------------------------------------------------------------------

  enumcode()

  Convert an option enum name to a code.

  */
#ifdef _PROTOTYPES_
static int enumcode(int opt, char *id)
#else
static int enumcode(opt, id)
     int opt;
     char id[];
#endif
{
  char **names;
  int i;

  if ((names = enumtbl[opt]) == NULL)
    return(EOF);

  for (i = 0; names[i] != NULL; i++)
    if (strcmp(names[i], id) == 0)
      return(i);

  return(EOF);
}


/*======================================================================

  optint()

  Handle an INTEGER option.

  */
#ifdef _PROTOTYPES_
void optint(char *id, Srcp *srcp, int val)
#else
void optint(id, srcp, val)
     char id[];
     Srcp *srcp;
     int val;
#endif
{
  int opt;

  if ((opt = optcode(id)) == EOF) {
    lmLog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != INTOPT
      ||  val < optbounds[opt].min
      ||  val > optbounds[opt].max) {
        lmLog(srcp, 602, sevWAR, id);
        return;
      }

  if (opts[opt].used) {
    lmLog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = TRUE;
  opts[opt].value = val;
}


/*======================================================================

  optenum()

  Handle an ENUM option.

  */
#ifdef _PROTOTYPES_
void optenum(char *id, Srcp *srcp, char *val)
#else
void optenum(id, srcp, val)
     char id[];
     Srcp *srcp;
     char val[];
#endif
{
  int opt, code;

  if ((opt = optcode(id)) == EOF) {
    lmLog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != ENUMOPT
      || (code = enumcode(opt, val)) == EOF) {
    lmLog(srcp, 602, sevWAR, id);
    return;
    }

  if (opts[opt].used) {
    lmLog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = TRUE;
  opts[opt].value = code;
}


/*======================================================================

  optbool()

  Handle a BOOLEAN option.

  */
#ifdef _PROTOTYPES_
void optBoolean(char *id, Srcp *srcp)
#else
void optBoolean(id, srcp)
     char id[];
     Srcp *srcp;
#endif
{
  int opt;

  if ((opt = optcode(id)) == EOF) {
    lmLog(srcp, 601, sevWAR, id);
    return;
  }

  if (opts[opt].type != BOOLOPT) {
    lmLog(srcp, 602, sevWAR, id);
    return;
  }

  if (opts[opt].used) {
    lmLog(srcp, 600, sevWAR, id);
    return;
  }

  opts[opt].used = TRUE;
  opts[opt].value = TRUE;
}


/*======================================================================

  geopt()

  Generate all options, i.e. copy the values into the header.

  */
#ifdef _PROTOTYPES_
void geopt(AcdHdr *header)
                    	/* OUT - the header struct to fill */
#else
void geopt(header)
     AcdHdr *header;	/* OUT - the header struct to fill */
#endif
{
  header->paglen = opts[OPTLEN].value;
  header->pagwidth = opts[OPTWIDTH].value;
  header->pack = opts[OPTPACK].value;
  header->visits = opts[OPTVISITS].value;
  header->debug = opts[OPTDEBUG].value;
}
