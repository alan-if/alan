/*----------------------------------------------------------------------*\

			       Option.c

			   Option Handling

\*----------------------------------------------------------------------*/

#include "Option.h"

#include "Srcp.h"
#include "lmList.h"

#include "acode.h"
#include "emit.h"



/* Exports: */

OptionDef options[NOPTIONS] = {
  {ENUMOPTION, FALSE, "language", LANGUAGE_ENGLISH},
  {INTOPTION, FALSE, "width", 75},
  {INTOPTION, FALSE, "length", 24},
  {BOOLOPTION, FALSE, "pack", FALSE},
  {BOOLOPTION, FALSE, "debug", FALSE}
};



/* Private: */

/* Enumerated values for Language-option */
static char *enumeratedLanguages[] = {
  "english",
  "swedish",
  NULL
};

/* Option bounds for numeric options */
static struct {
  int min,max;
} optionbounds[NOPTIONS] = {
  {  0,   0},
  { 24, 255},
  {  5, 255},
  {  0,   0},
  {  0, 255}
};

/* Table of pointers to enumerated values for enum options */
static char **enumeratedTables[NOPTIONS] = {
  enumeratedLanguages,
  NULL,
  NULL,
  NULL,
  NULL
};


/*----------------------------------------------------------------------

  optionCode()

  Convert an option name to a code.

  */
#ifdef _PROTOTYPES_
static int optionCode(char *id)
#else
static int optionCode(id)
     char id[];
#endif
{
  int option;

  for (option = 0; option <= NOPTIONS-1; option++)
    if (strcmp(options[option].name, id) == 0)
      return(option);

  return(EOF);
}


/*----------------------------------------------------------------------

  enumeratedCode()

  Convert an option enum name to a code.

  */
#ifdef _PROTOTYPES_
static int enumeratedCode(int option, char *id)
#else
static int enumeratedCode(option, id)
     int option;
     char id[];
#endif
{
  char **names;
  int i;

  if ((names = enumeratedTables[option]) == NULL)
    return(EOF);

  for (i = 0; names[i] != NULL; i++)
    if (strcmp(names[i], id) == 0)
      return(i);

  return(EOF);
}


/*======================================================================

  integerOption()

  Handle an INTEGER option.

  */
#ifdef _PROTOTYPES_
void integerOption(Id *id, int value)
#else
void integerOption(id, value)
     Id *id;
     int value;
#endif
{
  int option;

  if ((option = optionCode(id->string)) == EOF) {
    lmLog(&id->srcp, 601, sevWAR, id->string);
    return;
  }

  if (options[option].type != INTOPTION
      ||  value < optionbounds[option].min
      ||  value > optionbounds[option].max) {
        lmLog(&id->srcp, 602, sevWAR, id->string);
        return;
      }

  if (options[option].used) {
    lmLog(&id->srcp, 600, sevWAR, id->string);
    return;
  }

  options[option].used = TRUE;
  options[option].value = value;
}


/*======================================================================

  enumeratedOption()

  Handle an ENUM option.

  */
#ifdef _PROTOTYPES_
void enumeratedOption(Id *id, Id *value)
#else
void enumeratedOption(id, value)
     Id *id;
     Id *value;
#endif
{
  int option, code;

  if ((option = optionCode(id->string)) == EOF) {
    lmLog(&id->srcp, 601, sevWAR, id->string);
    return;
  }

  if (options[option].type != ENUMOPTION
      || (code = enumeratedCode(option, value->string)) == EOF) {
    lmLog(&id->srcp, 602, sevWAR, id->string);
    return;
    }

  if (options[option].used) {
    lmLog(&id->srcp, 600, sevWAR, id->string);
    return;
  }

  options[option].used = TRUE;
  options[option].value = code;
}


/*======================================================================

  booleanOption()

  Handle a BOOLEAN option.

  */
#ifdef _PROTOTYPES_
void booleanOption(Id *id)
#else
void booleanOption(id)
     Id *id;
#endif
{
  int option;

  if ((option = optionCode(id->string)) == EOF) {
    lmLog(&id->srcp, 601, sevWAR, id->string);
    return;
  }

  if (options[option].type != BOOLOPTION) {
    lmLog(&id->srcp, 602, sevWAR, id->string);
    return;
  }

  if (options[option].used) {
    lmLog(&id->srcp, 600, sevWAR, id->string);
    return;
  }

  options[option].used = TRUE;
  options[option].value = TRUE;
}


/*======================================================================

  geoption()

  Generate all options, i.e. copy the values into the header.

  */
#ifdef _PROTOTYPES_
void geoption(AcdHdr *header)
                    	/* OUT - the header struct to fill */
#else
void geoption(header)
     AcdHdr *header;	/* OUT - the header struct to fill */
#endif
{
  header->paglen = options[LENGTH_OPTION].value;
  header->pagwidth = options[WIDTH_OPTION].value;
  header->pack = options[PACK_OPTION].value;
  header->debug = options[DEBUG_OPTION].value;
}
