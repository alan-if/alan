/*----------------------------------------------------------------------*\

  term.c

  System dependent terminal oriented functions for ALAN interpreter ARUN

\*----------------------------------------------------------------------*/

#include "arun.h"

#include "term.h"


/*======================================================================

  getPageSize()

  Try to get the current page size from the system, else use the ones
  from the header.

 */
#ifdef _PROTOTYPES_
void getPageSize(void)
#else
void getPageSize()
#endif
{
#ifdef __sun__

#include <sys/termios.h>

  extern int ioctl();

  struct winsize win;
  int ecode;

  ecode = ioctl(1, TIOCGWINSZ, &win);

  if (ecode != 0 || win.ws_row == 0)
    paglen = header->paglen;
  else
    paglen = win.ws_row;

  if (ecode != 0 || win.ws_col == 0)
    pagwidth = header->pagwidth;
  else
    pagwidth = win.ws_col;

#else
#ifdef __amiga__
#include <libraries/dosextens.h>
#include <intuition/intuition.h>
#include <functions.h>
  struct Process * proc;
  struct InfoData *id;
  struct Window *win; 
  struct StandardPacket *packet;

  proc = (struct Process *) FindTask(0);

  id = (struct InfoData *) allocate(sizeof(struct InfoData));

  if (proc->pr_ConsoleTask) {
    packet = (struct StandardPacket *) allocate(sizeof(struct StandardPacket));
    packet->sp_Msg.mn_Node.ln_Name	= (char *)&(packet->sp_Pkt);
    packet->sp_Pkt.dp_Link		= & packet->sp_Msg;
    packet->sp_Pkt.dp_Port		= & proc->pr_MsgPort;
    packet->sp_Pkt.dp_Type		= ACTION_DISK_INFO;

    packet->sp_Pkt.dp_Arg1 = ((LONG) id) >> 2;

    PutMsg ((struct MsgPort *) proc->pr_ConsoleTask, & packet->sp_Msg);
    WaitPort(&proc->pr_MsgPort);
    GetMsg(&proc->pr_MsgPort);
    free((char *)packet);

    win = (struct Window *) id->id_VolumeNode;
    free(id);

    /* 4f_ti - Must be calculated w.r.t font size and borders */
    paglen = win->Height/8-2;
    pagwidth = win->Width/8-3;
  } else {
    paglen = header->paglen;
    pagwidth = header->pagwidth;
  }

#else

  paglen = header->paglen;
  pagwidth = header->pagwidth;

#endif
#endif
}
