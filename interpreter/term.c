/*----------------------------------------------------------------------*\

  term.c

  System dependent terminal oriented functions for ALAN interpreter ARUN

\*----------------------------------------------------------------------*/

#include "main.h"

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
#ifdef GLK
  paglen = 0;
  pagwidth = 0;

#else
#ifdef HAVE_TERMIO

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
#include <graphics/text.h>
#include <clib/exec_protos.h>

  struct Process * proc;
  struct InfoData *id;
  struct Window *win; 
  struct TextFont *textFont;
  struct StandardPacket *packet;

  proc = (struct Process *) FindTask(0L);

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

    /* Calculate number of characters and lines w.r.t font size and borders */
    textFont = win->IFont;
    paglen = win->Height/textFont->tf_YSize-2;
    pagwidth = win->Width/textFont->tf_XSize-3;
  } else {
    paglen = header->paglen;
    pagwidth = header->pagwidth;
  }

#else

  paglen = header->paglen;
  pagwidth = header->pagwidth;

#endif
#endif
#endif
}
