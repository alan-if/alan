/*----------------------------------------------------------------------*\

				dump.c

	  Internal form DUMP routines for the ALAN compiler

\*----------------------------------------------------------------------*/

#include "types.h"

#include "Srcp.h"

#include "lmList.h"


/* PUBLIC */
Bool dumpOnStdout;


/* PRIVATE */
static char stringBuffer[256] = "";
static int indent;


#ifdef _PROTOTYPES_
void put(char *str)
#else
void put(str)
     char str[];
#endif
{
  strcat(stringBuffer, str);
}



#ifdef _PROTOTYPES_
void nl(void)
#else
void nl()
#endif
{
  int i;

  if (dumpOnStdout)
    printf("%s\n", stringBuffer);
  else
    lmLiPrint(stringBuffer);
  stringBuffer[0] = '\0';

  for (i=1 ; i<=indent; i++)
    put(".  ") ;
}


#ifdef _PROTOTYPES_
void in(void)
#else
void in()
#endif
{
  indent++;
  nl();
}


#ifdef _PROTOTYPES_
void out(void)
#else
void out()
#endif
{
  indent--;
}


#ifdef _PROTOTYPES_
void dumpString(char *s)
#else
void dumpString(s)
     char s[];
#endif
{
  put("\"");
  put(s);
  put("\"");
}


#ifdef _PROTOTYPES_
void dumpInteger(int i)
#else
void dumpInteger(i)
     int i;
#endif
{
  char buf[20];

  sprintf(buf, "%d", i);
  put(buf);
}


#ifdef _PROTOTYPES_
void dumpAddress(void *adr)
#else
void dumpAddress(adr)
     char *adr;
#endif
{
  char buf[20];

  sprintf(buf, "{0x%lx} ", (unsigned long)adr);
  put(buf);
}


#ifdef _PROTOTYPES_
void dumpBoolean(Bool b)
#else
void dumpBoolean(b)
     Bool b;
#endif
{
  if (b)
    put("TRUE");
  else
    put("FALSE");
}
