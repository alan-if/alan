/*
 ! Internal form DUMP routines for the ALAN compiler
 */


#include "alan.h"

#include "adv.h"		/* ADV-node */
#include "lst.h"		/* LST-nodes */
#include "nam.h"		/* NAM-nodes */
#include "atr.h"                /* ATR-nodes */
#include "whr.h"                /* WHR-nodes */
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


void duint(int i)
{
  char buf[20];

  sprintf(buf, "%d", i);
  put(buf);
}


void duadr(int i)
{
  char buf[20];

  sprintf(buf, "%d (0x%x)", i, i);
  put(buf);
}


void duptr(void *adr)
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
