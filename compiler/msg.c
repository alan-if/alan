/*----------------------------------------------------------------------*\

				MSG.C
			       Messages

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "alan.h"

#include "srcp.h"
#include "lmList.h"

#include "adv.h"
#include "msg.h"
#include "opt.h"

#include "emit.h"
#include "encode.h"

#include "acode.h"



/* PRIVATE: */

static struct {char *id; char *english; char *swedish;} defmsg[] =
{
  {"huh",
     "Huh?",
     "Va?"},
  {"what",
     "I don't understand.",
     "Jag förstår inte."},
  {"whatall",
     "I don't know what you mean by 'all'.",
     "Jag vet inte vad du menar med 'allt'."},
  {"whatit",
     "I don't know what you mean by 'it'.",
     "Jag vet inte vad du menar med 'den'."},
  {"whatthem",
     "I don't know what you mean by 'them'.",
     "Jag vet inte vad du menar med 'dem'."},
  {"multiple",
     "You can't refer to multiple objects with '$v'.",
     "Du kan inte ge flera objekt tillsammans med '$v'."},
  {"want",
     "I can't guess what you want to $v.",
     "Jag kan inte begripa vad du vill $v."},
  {"noun",
     "You must supply a noun.",
     "Du måste ge ett substantiv."},
  {"afterbut",
     "You must give an object after 'but'.",
     "Det borde vara ett objekt efter 'utom'."},
  {"butall",
     "You can only use 'but' after 'all'.",
     "Du kan bara använda 'utom' efter 'allt'."},
  {"notmuch",
     "That doesn't leave much to $v!",
     "Det blir inte så mycket kvar att $v då!"},
  {"whichone",
     "I don't know which $1 you mean.",
     "Jag vet inte vilken $1 du menar."},
  {"nosuch",
     "I can't see any $1 here.",
     "Jag ser ingen $1 här."},
  {"noway",
     "You can't go that way.",
     "Du kan inte gå åt det hållet."},
  {"cant0",
     "You can't do that.",
     "Det går inte att göra."},
  {"cant",
     "You can't $v the $1.",
     "Du kan inte $v $1."},
  {"nothing",
     "There is nothing here that you can $v.",
     "Det finns inget du kan $v här."},
  {"seeobject1",
     "There is",
     "Det finns "},
  {"seeobjectcomma",
     "$$, ",
     ", "},
  {"seeobjectand",
     "and ",
     "och "},
  {"seeobjectend",
     "here.",
     "här."},
  {"seeactor",
     "is here.",
     "är här."},
  {"contains1",
     "The",
     ""},
  {"contains",
     "contains",
     "innehåller"},
  {"containscomma",
     ", ",
     ", "},
  {"containsand",
     "and ",
     "och "},
  {"containsend",
     "$$.",
     "$$."},
  {"empty1",
     "The",
     ""},
  {"isempty",
     "is empty.",
     "är tom."},
  {"havescored",
     "You have scored",
     "Du har samlat"},
  {"scoreoutof",
     "points out of",
     "poäng av"},
  {"unknownword",
     "I don't know that word.",
     "Jag känner inte till det ordet."},
  {"more",
     "<More>",
     "<Mera>"},
  {"again",
     "(again)",
     "(igen)"},
  {"savewhere",
     "Enter file name to save in",
     "Spara äventyret i vilken fil"},
  {"saveoverwrite",
     "That file already exists, overwrite (y) ? ",
     "Filen fanns redan, skriva över (j) ? "},
  {"savefailed",
     "Sorry, save failed.",
     "Tyvärr, det gick inte att spara äventyret."},
  {"savemissing",
     "Sorry, could not open the save file.",
     "Tyvärr, kunde inte hitta något sparat äventyr."},
  {"saveversion",
     "Sorry, the save file was created by a different version.",
     "Tyvärr, det sparade äventyret har inte samma version."},
  {"savename",
     "Sorry, the save file did not contain a save for this adventure.",
     "Tyvärr, det var inte detta äventyr som var sparat i den filen."},
  {"restorefrom",
     "Enter file name to restore from",
     "Hämta sparat äventyr från vilken fil"},
  {"retry",
     "Do you want to restart (y) ? ",
     "Vill du försöka igen (j) ? "},
  {"defarticle",
     "a",
     "en"},
  {NULL, NULL, NULL}		/* MUST end with NULL */
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
#ifdef _PROTOTYPES_
void getxt(char *txt)
                		/* IN - The text to output */
#else
void getxt(txt)
     char txt[];		/* IN - The text to output */
#endif
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
#ifdef _PROTOTYPES_
MsgNod *newmsg(Srcp *srcp, NamNod *nam, long int fpos, int len)
#else
MsgNod *newmsg(srcp, nam, fpos, len)
     Srcp *srcp;		/* IN - Source position of possible user declaration */
     NamNod *nam;		/* IN - If user declared the name used */
     long fpos;			/* IN - The file position */
     int len;			/* IN - Length of the text */
#endif
{
  MsgNod *msg;

  msg = NEW(MsgNod);

  if (srcp) msg->srcp = *srcp;
  msg->nam = nam;
  msg->fpos = fpos;
  msg->len = len;

  return(msg);
}


/*======================================================================

  prepmsgs()

  Prepare all system messages by writing them out to the text file and
  noting their position.

 */
#ifdef _PROTOTYPES_
void prepmsgs(void)
#else
void prepmsgs()
#endif
{
  int msgno;
  List *smsgs = NULL;		/* The constructed list */
  List *umsgs;			/* Iteration pointer into user messages list */
  List *lst;

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
    char *msgp;

    /* Create a message node */
    switch (opts[OPTLANG].value) {
    case L_ENGLISH:
      msgp = defmsg[msgno].english;
      break;
    case L_SWEDISH:
      msgp = defmsg[msgno].swedish;
      break;
    }
    msg = newmsg(NULL, NULL, ftell(txtfil), strlen(msgp));
    msg->msgno = msgno;
    smsgs = concat(smsgs, msg);
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
  }

  /* Merge user messages to the list */
  for (umsgs = adv.msgs; umsgs; umsgs = umsgs->next) {
    /* Find what number the user defined message has */
    for (msgno = 0; defmsg[msgno].id != NULL; msgno++)
      if (strcmp(defmsg[msgno].id, umsgs->element.msg->nam->str) == 0)
	break;
    if (defmsg[msgno].id == NULL)
      lmLog(&umsgs->element.msg->nam->srcp, 223, sevWAR, NULL);
    else {
      /* Find that message in the system messages list */
      for (lst = smsgs; lst != NULL && lst->element.msg->msgno != msgno; lst = lst->next);
      if (lst == NULL)
	syserr("Reached end of system messages list.");

      /* Update the message info */
      lst->element.msg->fpos = umsgs->element.msg->fpos;
      lst->element.msg->len = umsgs->element.msg->len;

      /* And free user message node */
      free(umsgs->element.msg);
      umsgs->element.msg = NULL;
    }
  }

  /* Free the user defined messages */
  for (lst = adv.msgs; lst; lst = lst->next) {
    free(adv.msgs);
    adv.msgs = lst;
  }
  if (adv.msgs != NULL) free(adv.msgs);

  adv.msgs = smsgs;
}



/*======================================================================

  gemsgs()

  Generate error and other messages depending on the selected language.

  */
#ifdef _PROTOTYPES_
Aaddr gemsgs(void)
#else
Aaddr gemsgs()
#endif
{
  Aaddr adr;
  List *lst;

  adr = emadr();		/* Save address to messages table */
  for (lst = adv.msgs; lst; lst = lst->next) {
    encode(&lst->element.msg->fpos, &lst->element.msg->len);
    emit(lst->element.msg->fpos);
    emit(lst->element.msg->len);
  }
  emit(EOF);
  return(adr);
}
