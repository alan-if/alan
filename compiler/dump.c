/*----------------------------------------------------------------------*\

  Internal form DUMP routines for the ALAN compiler

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "lmList.h"
#include "dump.h"


static char strbuf[256] = "";
static int indent = 0;


void put(char *str)
{
  strcat(strbuf, str);
}



void nl(void)
{
  int i;

  lmLiPrint(strbuf);
  strbuf[0] = '\0';

  for (i=1 ; i<=indent; i++)
    put(".  ") ;
}


void in(void)
{
  indent++;
  nl();
}


void out(void)
{
  indent--;
}


void dustr(char *s)
{
  put("\"");
  put(s);
  put("\"");
}


void dumpInt(int i)
{
  char buf[20];

  sprintf(buf, "%d", i);
  put(buf);
}


void dumpAddress(int i)
{
  char buf[20];

  sprintf(buf, "%d (0x%x)", i, i);
  put(buf);
}


void dumpPointer(void *adr)
{
  char buf[20];

  sprintf(buf, "{0x%lx} ", (unsigned long)adr);
  put(buf);
}


void dumpBool(Bool b)
{
  if (b)
    put("TRUE");
  else
    put("FALSE");
}
