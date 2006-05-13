/*----------------------------------------------------------------------*\

				SRCP.C
			 Source Position Type

\*----------------------------------------------------------------------*/

#include "alan.h"
#include "dump_x.h"
#include "srcp_x.h"
#include "opt.h"
#include "emit.h"
#include "util.h"
#include "lst_x.h"


/* Private: */
static List *srcps = NULL;


/*----------------------------------------------------------------------*/
static Srcp *newSrcp(int file, int line) {
  Srcp *new = NEW(Srcp);
  new->line = line;
  new->file = file;
  return new;
}

/*======================================================================*/
Bool equalSrcp(Srcp srcp1, Srcp srcp2)
{
  return (srcp1.col == srcp2.col && srcp1.file == srcp2.file && srcp1.line == srcp2.line);
}


/*----------------------------------------------------------------------*/
static void addSrcp(Srcp srcp) {
  if (srcp.line != 0)
    srcps = concat(srcps, newSrcp(srcp.file, srcp.line), SRCP_LIST);
}


/*----------------------------------------------------------------------*/
Bool inSrcps(Srcp srcp) {
  List *list;

  if (srcp.line != 0)
    TRAVERSE(list, srcps) {
      if (list->element.srcp->file == srcp.file
	  && list->element.srcp->line == srcp.line)
	return TRUE;
    }
  return FALSE;
}


/*======================================================================*/
void generateSrcp(Srcp srcp) {
  static int previousFile = 0;
  static int previousLine = 0;

  if (!(Bool)opts[OPTDEBUG].value) return;

  if (srcp.file == previousFile && srcp.line == previousLine) return;
  previousFile = srcp.file;
  previousLine = srcp.line;

  emitConstant(srcp.file);
  emitConstant(srcp.line);
  emit0(I_LINE);

  if (!inSrcps(srcp))
    addSrcp(srcp);
}


/*----------------------------------------------------------------------*/
static int compareSrcps(List *srcp1, List *srcp2) {
  if (srcp1->element.srcp->file != srcp2->element.srcp->file)
    return srcp1->element.srcp->file > srcp2->element.srcp->file ? 1 : -1;
  return srcp1->element.srcp->line > srcp2->element.srcp->line ? 1 : -1;
}


/*======================================================================*/
Aaddr generateSrcps(void) {
  List *srcp;
  SourceLineEntry entry;
  Aaddr adr = nextEmitAddress();

  if (opts[OPTDEBUG].value) {
    srcps = sortList(srcps, &compareSrcps);
    TRAVERSE(srcp, srcps) {
      entry.file = srcp->element.srcp->file;
      entry.line = srcp->element.srcp->line;
      emitEntry(&entry, sizeof(entry));
    }
    emit(EOF);
    return adr;
  } else
    return 0;
}


/*======================================================================*/
void dumpSrcp(Srcp srcp)
{
  char str[20];

  sprintf(str, "(%d,%d,%d)", srcp.file, srcp.line, srcp.col);
  put(str);
}

