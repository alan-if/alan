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
#include "opt.h"		/* OPTIONS */

#include "emit.h"
#include "encode.h"

#include "acode.h"



/* PRIVATE: */

static char *engmsg[] =
{
  "Huh?",
  "I don't understand.",
  "I don't know what you mean by 'all'.",
  "I don't know what you mean by 'it'.",
  "I don't know what you mean by 'them'.",
  "You can't refer to multiple objects with '$v'.",
  "I can't guess what you want to $v.",
  "You must supply a noun.",
  "You must give an object after 'but'.",
  "You can only use 'but' after 'all'.",
  "That doesn't leave much to $v!",
  "I don't know which $1 you mean.",
  "I can't see any $1 here.",
  "You can't go that way.",
  "You can't do that.",
  "You can't $v the $1.",
  "There is nothing here that you can $v.",
  "There is a",
  ", a",
  "and a",
  "here.",
  "is here.",
  "The",
  "contains a",
  ", a",
  "and a",
  "$$.",
  "The",
  "is empty.",
  "You have scored",
  "points out of",
  "I don't know that word.",
  "<More>",
  "(again)",
  "Sorry, save failed.",
  "Sorry, could not open any save file.",
  "Sorry, the save file was created by a different version.",
  "Sorry, the save file did not contain a save for this adventure.",
  "Do you want to restart (y) ? ",
  NULL
};

static char *swemsg[] = {
  "Va?",
  "Jag förstår inte.",
  "Jag vet inte vad du menar med 'allt'.",
  "Jag vet inte vad du menar med 'den'.",
  "Jag vet inte vad du menar med 'dem'.",
  "Du kan inte ge flera objekt tillsammans med '$v'.",
  "Jag kan inte begripa vad du vill $v.",
  "Du måste ge ett substantiv.",
  "Det borde vara ett objekt efter 'utom'.",
  "Du kan bara använda 'utom' efter 'allt'.",
  "Det blir inte så mycket kvar att $v då!",
  "Jag vet inte vilken $1 du menar.",
  "Jag ser ingen $1 här.",
  "Du kan inte gå åt det hållet.",
  "Det går inte att göra.",
  "Du kan inte $v $1.",
  "Det finns inget du kan $v här.",
  "Det finns en",
  ", en",
  "och en",
  "här.",
  "är här.",
  "",
  "innehåller",
  ", en",
  "och en",
  "$$.",
  "",
  "är tom.",
  "Du har samlat",
  "poäng av",
  "Jag känner inte till det ordet.",
  "<Mera>",
  "(igen)",
  "Tyvärr, det gick inte att spara äventyret.",
  "Tyvärr, kunde inte hitta något sparat äventyr.",
  "Tyvärr, det sparade äventyret har inte samma version.",
  "Tyvärr, det var inte detta äventyr som var sparat i den filen.",
  "Vill du försöka igen (j) ? ",
  NULL
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




/*----------------------------------------------------------------------

  newmsg()

  Create a new message node;

 */
#ifdef _PROTOTYPES_
static MsgNod *newmsg(long int fpos, int len)
               			/* IN - The file position */
             			/* IN - Length of the text */
#else
static MsgNod *newmsg(fpos, len)
     long fpos;			/* IN - The file position */
     int len;			/* IN - Length of the text */
#endif
{
  MsgNod *msg;

  msg = NEW(MsgNod);

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
  char **msgp;
  List *lst = NULL;	/* The constructed list */


  if (sizeof(swemsg)/sizeof(swemsg[0]) != MSGMAX)
    syserr("Incorrect number of messages in swedish message tables");
  if (sizeof(engmsg)/sizeof(swemsg[0]) != MSGMAX)
    syserr("Incorrect number of messages in english message tables");

  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
    msgp = &engmsg[0];
    break;
  case L_SWEDISH:
    msgp = &swemsg[0];
    break;
  default:
    syserr("unrecognised switch in prepmsgs()");
    return;
  }

  
  /* For all messages in the pointed to table */
  while (*msgp) {
    /* Create a message node */
    lst = concat(lst, newmsg(ftell(txtfil), strlen(*msgp)));
    getxt(*msgp);
    msgp++;
  }

  adv.msgs = lst;
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
