/*----------------------------------------------------------------------*\

				MSG.C
			       Messages

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "stm_x.h"
#include "lmList.h"

#include "adv.h"
#include "msg.h"
#include "opt.h"

#include "emit.h"
#include "encode.h"
#include "../interpreter/acode.h"



/* PRIVATE: */

static struct {int messageCode; char *id; char *english; char *swedish;} defmsg[] =
{
  {M_WHAT, "what",
     "I don't understand.",
     "Jag f�rst�r inte."},
  {M_WHAT_ALL, "whatall",
     "I don't know what you mean by 'all'.",
     "Jag vet inte vad du menar med 'allt'."},
  {M_WHAT_IT, "whatit",
     "I don't know what you mean by 'it'.",
     "Jag vet inte vad du menar med 'den'."},
  {M_WHAT_THEM, "whatthem",
     "I don't know what you mean by 'them'.",
     "Jag vet inte vad du menar med 'dem'."},
  {M_MULTIPLE, "multiple",
     "You can't refer to multiple objects with '$v'.",
     "Du kan inte ge flera objekt tillsammans med '$v'."},
  {M_WANT, "want",
     "I can't guess what you want to $v.",
     "Jag kan inte begripa vad du vill $v."},
  {M_NOUN, "noun",
     "You must supply a noun.",
     "Du m�ste ge ett substantiv."},
  {M_AFTER_BUT, "afterbut",
     "You must give an object after 'but'.",
     "Det borde vara ett objekt efter 'utom'."},
  {M_BUT_ALL, "butall",
     "You can only use 'but' after 'all'.",
     "Du kan bara anv�nda 'utom' efter 'allt'."},
  {M_NOT_MUCH, "notmuch",
     "That doesn't leave much to $v!",
     "Det blir inte s� mycket kvar att $v d�!"},
  {M_WHICH_ONE, "whichone",
     "I don't know which $1 you mean.",
     "Jag vet inte vilken $1 du menar."},
  {M_NO_SUCH, "nosuch",
     "I can't see any $1 here.",
     "Jag ser ingen $1 h�r."},
  {M_NO_WAY, "noway",
     "You can't go that way.",
     "Du kan inte g� �t det h�llet."},
  {M_CANT0, "cant0",
     "You can't do that.",
     "Det g�r inte att g�ra."},
  {M_CANT, "cant",
     "You can't $v the $1.",
     "Du kan inte $v $1."},
  {M_SEEOBJ1, "seeobject1",
     "There is",
     "Det finns"},
  {M_SEEOBJ2, "seeobjectcomma",
     "$$, ",
     "$$, "},
  {M_SEEOBJ3, "seeobjectand",
     "and",
     "och"},
  {M_SEEOBJ4, "seeobjectend",
     "here.",
     "h�r."},
  {M_SEEACT, "seeactor",
     "is here.",
     "�r h�r."},
  {M_CONTAINS0, "contains0",
     "The",
     ""},
  {M_CONTAINS, "contains",
     "contains",
     "inneh�ller"},
  {M_CARRIES, "carries",
     "carries",
     "b�r p�"},
  {M_CONTAINSCOMMA, "containscomma",
     "$$, ",
     "$$, "},
  {M_CONTAINSAND, "containsand",
     "and",
     "och"},
  {M_CONTAINSEND, "containsend",
     "$$.",
     "$$."},
  {M_EMPTY, "isempty",
     "is empty.",
     "�r tom."},
  {M_EMPTYHANDED, "emptyhanded",
     "is empty-handed.",
     "�r tomh�nt."},
  {M_SCORE1, "havescored",
     "You have scored",
     "Du har samlat"},
  {M_SCORE2, "scoreoutof",
     "points out of",
     "po�ng av"},
  {M_UNKNOWN_WORD, "unknownword",
     "I don't know that word.",
     "Jag k�nner inte till det ordet."},
  {M_MORE, "more",
     "<More>",
     "<Mera>"},
  {M_AGAIN, "again",
     "(again)",
     "(igen)"},
  {M_SAVEWHERE, "savewhere",
     "Enter file name to save in",
     "Spara �ventyret i vilken fil"},
  {M_SAVEOVERWRITE, "saveoverwrite",
     "That file already exists, overwrite (RETURN confirms) ? ",
     "Filen fanns redan, skriva �ver (RETURN f�r ja) ? "},
  {M_SAVEFAILED, "savefailed",
     "Sorry, save failed.",
     "Tyv�rr, det gick inte att spara �ventyret."},
  {M_SAVEMISSING, "savemissing",
     "Sorry, could not open that save file.",
     "Tyv�rr, kunde inte hitta n�got s�dant sparat �ventyr."},
  {M_SAVEVERS, "saveversion",
     "Sorry, the save file was created by a different version.",
     "Tyv�rr, det sparade �ventyret har inte samma version."},
  {M_SAVENAME, "savename",
     "Sorry, the save file did not contain a save for this adventure.",
     "Tyv�rr, det var inte detta �ventyr som var sparat i den filen."},
  {M_RESTOREFROM, "restorefrom",
     "Enter file name to restore from",
     "H�mta sparat �ventyr fr�n vilken fil"},
  {M_REALLY, "really",
     "Are you sure (RETURN confirms) ? ",
     "�r du s�ker (RETURN f�r ja) ? "},
  {M_QUITACTION, "quitaction",
     "Do you want to RESTART, RESTORE or QUIT ? ",
     "Vad vill du g�ra, RESTART, RESTORE or QUIT ? "},
  {M_ARTICLE, "defarticle",
     "a",
     "en"},
  {0, NULL, NULL, NULL}		/* MUST end with NULL */
};


/*======================================================================

  getxt()

  Generate a text string as text data. This function takes a string
  and outputs it in the text file, updating the character frequencies
  for the characters encountered. This will complement the text
  directly found in the adventure program strings, such as for example
  standard messages and default object, actor and location names.
  NOTE that any newlines at end of texts to make the .txt file
  prettier is not inserted.

 */
void getxt(char *txt)		/* IN - The text to output */
{
  int i;

  for (i = 0; txt[i]; i++)
    incFreq(txt[i]);
  fprintf(txtfil, txt);
}




/*======================================================================

  newmsg()

  Create a new message node.

 */
MsgNod *newmsg(Srcp *srcp, IdNode *id, List *stms)
{
  MsgNod *msg;

  showProgress();

  msg = NEW(MsgNod);

  if (srcp) msg->srcp = *srcp;
  msg->id = id;
  msg->stms = stms;

  return(msg);
}


/*======================================================================

  prepmsgs()

  Prepare all system messages by writing them out to the text file and
  noting their position.

 */
void prepmsgs(void)
{
  int msgno;
  List *smsgs = NULL;		/* The constructed list */
  List *umsgs;			/* Iteration pointer into user messages list */
  List *lst, *rest;
  /* The dummy list of statements */
  StmNod *stm;
  List *stms = NULL;

  if (sizeof(defmsg)/sizeof(defmsg[0]) != MSGMAX+1)
    syserr("Incorrect number of messages in message tables");

  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
  case L_SWEDISH:
    break;
  default:
    syserr("unrecognised switch in prepmsgs()");
    return;
  }

  
  /* For all messages in the pointed to table */
  for (msgno = 0; defmsg[msgno].id; msgno++) {
    MsgNod *msg;
    char *msgp = NULL;

    if (msgno != defmsg[msgno].messageCode)
      syserr("Wrong code in message table.");

    /* Create a message node */
    switch (opts[OPTLANG].value) {
    case L_ENGLISH:
      msgp = defmsg[msgno].english;
      break;
    case L_SWEDISH:
      msgp = defmsg[msgno].swedish;
      break;
    }
    /* Create a PRINT statement and enter the print info */
    stm = newstm(&nulsrcp, PRINT_STATEMENT);
    stm->fields.print.fpos = ftell(txtfil);
    stm->fields.print.len = strlen(msgp);
    /* Make a list of it */
    stms = concat(NULL, stm, LIST_STM);
    msg = newmsg(NULL, NULL, stms);
    /* Save the message text */
#ifdef __mac__
    { 
      char buf[256];

      toIso(buf, msgp);
      getxt(buf);
    }
#else
#ifdef __dos__
    { 
      char buf[256];

      toIso(buf, msgp);
      getxt(buf);
    }
#else
    getxt(msgp);
#endif
#endif
    /* Finally enter it in the list */
    msg->msgno = msgno;
    smsgs = concat(smsgs, msg, LIST_MSG);
  }

  /* Merge user messages to the list */
  for (umsgs = adv.msgs; umsgs; umsgs = umsgs->next) {
    List *garb;			/* The standard message stm list is garbage */
    /* Find what number the user defined message has */
    for (msgno = 0; defmsg[msgno].id != NULL; msgno++)
      if (strcmp(defmsg[msgno].id, umsgs->element.msg->id->string) == 0)
	break;
    if (defmsg[msgno].id == NULL)
      lmLog(&umsgs->element.msg->id->srcp, 223, sevWAR, NULL);
    else {
      /* Find that message in the system messages list */
      for (lst = smsgs; lst != NULL && lst->element.msg->msgno != msgno; lst = lst->next);
      if (lst == NULL)
	syserr("Reached end of system messages list.");

      /* Update the message statements */
      garb = lst->element.msg->stms;
      lst->element.msg->stms = umsgs->element.msg->stms;

      /* And system print statement and list and user message node */
      free(garb->element.stm);
      free(garb);
      free(umsgs->element.msg);
      umsgs->element.msg = NULL;
    }
  }

  /* Free the user defined messages */
  for (lst = adv.msgs; lst; lst = rest) {
    rest = lst->next;
    free(lst);
  }
  adv.msgs = smsgs;
}



/*======================================================================

  anmsgs()

  Analyze the statements in the system messages

  */
void anmsgs(void)
{
  List *lst;

  /* Nothing to do except to analyze the statements */
  for (lst = adv.msgs; lst; lst = lst->next) {
    showProgress();
    analyzeStatements(lst->element.msg->stms, NULL);
  }

}


/*----------------------------------------------------------------------

  gemsgent()

  Generate statements for the message construct

  */
static void gemsgent(MsgNod *msg)
{
  msg->stmadr = emadr();	/* Save address to messages statements */
  gestms(msg->stms, 0);
  emit0(C_STMOP, I_RETURN);
}

/*======================================================================

  gemsgs()

  Generate error and other messages depending on the selected language.

  */
Aaddr gemsgs(void)
{
  Aaddr adr;
  List *lst;

  /* First generate the statements for each message */
  for (lst = adv.msgs; lst; lst = lst->next) {
    showProgress();
    gemsgent(lst->element.msg);
  }

  adr = emadr();		/* Save address to messages table */
  for (lst = adv.msgs; lst; lst = lst->next)
    emit(lst->element.msg->stmadr);
  emit(EOF);
  return(adr);
}

