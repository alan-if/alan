/*
 ! Internal form DUMP routines for the ALAN compiler
 */


#include "srcp.h"

#include "alan.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "exp.h"                /* EXP-nodes */
#include "atr.h"                /* ATR-nodes */
#include "whr.h"                /* WHR-nodes */
#include "wht.h"                /* WHT-nodes */
#include "stm.h"		/* STM-nodes */
#include "chk.h"                /* CHK-nodes */
#include "vrb.h"                /* VRB-nodes */
#include "lim.h"		/* LIM-nodes */
#include "cnt.h"		/* CNT-nodes */
#include "obj.h"		/* OBJ-nodes */
#include "ext.h"                /* EXT-nodes */
#include "loc.h"                /* LOC-nodes */
#include "scr.h"                /* SCR-nodes */
#include "stp.h"                /* STP-nodes */
#include "act.h"                /* ACT-nodes */
#include "evt.h"                /* EVT-nodes */
#include "rul.h"                /* RUL-nodes */

#include "lmList.h"


static char strbuf[256] = "";
static int indent;


#ifdef _PROTOTYPES_
void put(char *str)
#else
void put(str)
     char str[];
#endif
{
  strcat(strbuf, str);
}



#ifdef _PROTOTYPES_
void nl(void)
#else
void nl()
#endif
{
  int i;

  lmLiPrint(strbuf);
  strbuf[0] = '\0';

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
void dustr(char *s)
#else
void dustr(s)
     char s[];
#endif
{
  put("\"");
  put(s);
  put("\"");
}


#ifdef _PROTOTYPES_
void duint(int i)
#else
void duint(i)
     int i;
#endif
{
  char buf[20];

  sprintf(buf, "%d", i);
  put(buf);
}


#ifdef _PROTOTYPES_
void duadr(void *adr)
#else
void duadr(adr)
     char *adr;
#endif
{
  char buf[20];

  sprintf(buf, "{0x%lx} ", (unsigned long)adr);
  put(buf);
}


#ifdef _PROTOTYPES_
void duBoolean(Boolean b)
#else
void duBoolean(b)
     Boolean b;
#endif
{
  if (b)
    put("TRUE");
  else
    put("FALSE");
}
