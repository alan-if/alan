/*----------------------------------------------------------------------*\

				MSG.C
			       Messages

\*----------------------------------------------------------------------*/

#include "sysdep.h"
#include "alan.h"
#include "util.h"

#include "srcp_x.h"
#include "lst_x.h"
#include "adv_x.h"
#include "stm_x.h"
#include "sym_x.h"
#include "context_x.h"
#include "lmList.h"

#include "msg.h"
#include "opt.h"

#include "emit.h"
#include "encode.h"
#include "../interpreter/acode.h"



/* PRIVATE: */

static struct {int messageCode; char *id; char *english; char *swedish; char *german;} defmsg[] =
{
  {M_UNKNOWN_WORD, "unknown_word",
   "I don't know the word '$1'.",
   "Jag k�nner inte till ordet '$1'.",
   "Ich kenne das Wort '$1' nicht."},
  {M_WHAT, "what",
   "I don't understand.",
   "Jag f�rst�r inte.",
   "Ich ferste nicht."},
  {M_WHAT_WORD, "what_word",
   "I don't know what you mean by '$1'.",
   "Jag vet inte vad du menar med '$1'.",
   "Ich verstehe nicht was Du mit '$1' meinst."},
  {M_MULTIPLE, "multiple",
   "You can't refer to multiple objects with '$v'.",
   "Du kan inte ange flera objekt tillsammans med verbet '$v'.",
   "Du kannst Dich nicht auf mehrere Gegenst�nde mit '$v' beziehen."},
  {M_NOUN, "noun",
   "You must supply a noun.",
   "Du m�ste ange ett substantiv.",
   "Du musst ein Gegenstandswort angeben."},
  {M_AFTER_BUT, "after_but",
   "You must give at least one object after '$1'.",
   "Det borde vara minst ett objekt efter '$1'.",
   "Du musst zum mindesten einen Gegenstand nach '$1' angeben."},
  {M_BUT_ALL, "but_all",
   "You can only use '$1' after '$2'.",
   "Du kan bara anv�nda '$1' efter '$2'.",
   "Du kannst '$1' nur nach '$2' verwenden."},	/* german: "Nimm alles ausser der Lampe." */
  {M_NOT_MUCH, "not_much",
   "That doesn't leave much to $v!",
   "Det blir inte s� mycket kvar att $v d�!",
   "Das �berl��t viel nicht zu $v!"},
  {M_WHICH_ONE_START, "which_start",
   "I don't know if you mean $+1",
   "Jag vet inte om du menar $+1",
   "Ich wei� nicht, falls Du $+1"},
  {M_WHICH_ONE_COMMA, "which_comma",
   ", $+1",
   ", $+1",
   ", $+1"},
  {M_WHICH_ONE_OR, "which_or",
   "or $+1.",
   "eller $+1.",
   "oder $+1."},
  {M_NO_SUCH, "no_such",
   "I can't see any $1 here.",
   "Jag ser ingen $1 h�r.",
   "Ich kann hier keinen $1 sehen."},
  {M_NO_WAY, "no_way",
   "You can't go that way.",
   "Du kan inte g� �t det h�llet.",
   "Du kannst nicht in diese Richtung gehen."},
  {M_CANT0, "cant0",
   "You can't do that.",
   "Det g�r inte att g�ra.",
   "Du kannst das nicht tun."},
  {M_CANT, "cant",
   "You can't $v $+1.",
   "Du kan inte $v $+1.",
   "Du kannst $+1 nicht $v."},
  {M_SEE_OBJ_START, "see_object_start",
   "There is $01",
   "Det finns $01",
   "Es gibt $01"},
  {M_SEE_OBJ_COMMA, "see_object_comma",
   ", $01",
   ", $01",
   ", $01"},
  {M_SEE_OBJ_AND, "see_object_and",
   "and $01",
   "och $01",
   "und $01"},
  {M_SEE_OBJ_END, "see_object_end",
   "here.",
   "h�r.",
   "hier."},
  {M_SEE_ACT, "see_actor",
   "$+1 is here.",
   "$+1 �r h�r.",
   "$+1 ist hier."},
  /* Two messages for default HEADER (objects and actors) */
  {M_CONTAINS, "contains",
   "$+1 contains",
   "$+1 inneh�ller",
   "$+1 enth�lt"},
  {M_CARRIES, "carries",
   "$+1 carries",
   "$+1 b�r p�",
   "$+1 tr�gt"},
  {M_CONTAINS_COMMA, "contains_comma",
   "$01,",
   "$01,",
   "$01,"},
  {M_CONTAINS_AND, "contains_and",
   "$01 and",
   "$01 och",
   "$01 und"},
  {M_CONTAINS_END, "contains_end",
   "$01.",
   "$01.",
   "$01."},
  {M_EMPTY, "is_empty",
   "$+1 is empty.",
   "$+1 �r tom.",
   "$+1 ist leer."},
  {M_EMPTYHANDED, "empty_handed",
   "$+1 is empty-handed.",
   "$+1 �r tomh�nt.",
   "$+1 tr�gt nichts."},
  {M_CANNOTCONTAIN, "can_not_contain",
   "$+1 can not contain $+2.",
   "$+1 kan inte inneh�lla $+2.",
   "$+1 kan $+2 nicht tr�gen."},
  {M_SCORE, "have_scored",
   "You have scored $1 points out of $2.",
   "Du har samlat $1 po�ng av $2.",
   "Du hast $1 Punkte von $2."},
  {M_MORE, "more",
   "<More>",
   "<Mera>",
   "<Mehr>"},
  {M_AGAIN, "again",
   "(again)",
   "(igen)",
   "(erneut)"},
  {M_SAVEWHERE, "save_where",
   "Enter file name to save in",
   "Spara �ventyret i vilken fil",
   "Gib den Dateinamen an, um zu speichern"},
  {M_SAVEOVERWRITE, "save_overwrite",
   "That file already exists, overwrite (RETURN confirms) ? ",
   "Filen fanns redan, skriva �ver (RETURN f�r ja) ? ",
   "Die Datei existiert bereits, �berschreiben (ENTER, best�tigt)? "},
  {M_SAVEFAILED, "save_failed",
   "Sorry, save failed.",
   "Tyv�rr, det gick inte att spara �ventyret.",
   "Das Spiel konnte nicht gespeichert werden."},
  {M_RESTOREFROM, "restore_from",
   "Enter file name to restore from",
   "H�mta sparat �ventyr fr�n vilken fil",
   "Von welcher Datei soll das Spiel wieder hergestellt werden."},
  {M_SAVEMISSING, "save_missing",
   "Sorry, could not open that save file.",
   "Tyv�rr, kunde inte hitta n�got s�dant sparat �ventyr.",
   "Konnte das gespeicherte Spiel nicht �ffnen."},
  {M_NOTASAVEFILE, "not_a_savefile",
   "That file does not seem to be an Alan game save file.",
   "Den filen verkar inte vara ett sparat Alan spel.",
   "Diese Datei scheint kein gespeichertes ALAN Spiel zu sein."},
  {M_SAVEVERS, "save_version",
   "Sorry, the save file was created by a different version.",
   "Tyv�rr, det sparade �ventyret har inte samma version.",
   "Diese Datei wurde mit einer anderen Version gespeichert."},
  {M_SAVENAME, "save_name",
   "Sorry, the save file did not contain a save for this adventure.",
   "Tyv�rr, det var inte detta �ventyr som var sparat i den filen.",
   "Diese gespeicherte Datei geh�rt nicht zu diesem Spiel."},
  {M_REALLY, "really",
   "Are you sure (RETURN confirms) ? ",
   "�r du s�ker (RETURN f�r ja) ? ",
   "Bist Du sicher (ENTER best�tigt) ? "},
  {M_QUITACTION, "quit_action",
   "Do you want to UNDO, RESTART, RESTORE or QUIT ? ",
   "Vad vill du g�ra, UNDO, RESTART, RESTORE or QUIT ? ",
   "M�chtest Du UNDO, RESTART, RESTORE oder QUIT ? "},	/* TODO: is this changeable to "NEUSTART, LADEN oder ENDE ?" ? */
  {M_INDEFINITE, "indefinite_article",
   "a",
   "en",
   "ein"},
  {M_DEFINITE, "definite_article",
   "the",
   "",
   "der"},
  {M_PRONOUN, "default_pronoun",
   "it",
   "den",
   "das"},
  {M_UNDONE, "undone",
   "'$1' undone.",
   "'$1' ogjort.",
   "'$1' annuliert."},
  {M_NO_UNDO, "no_undo",
   "Nothing more to undo.",
   "Det finns inte mer att g�ra ogjort.",
   "Es gibt nichts mehr zu annulieren."},
  {0, NULL, NULL, NULL, NULL}        /* MUST end with NULL */

};


/*======================================================================

  generateText()

  Generate a text string as text data. This function takes a string
  and outputs it in the text file, updating the character frequencies
  for the characters encountered. This will complement the text
  directly found in the adventure program strings, such as for example
  standard messages and default object, actor and location names.
  NOTE that any newlines at end of texts to make the .txt file
  prettier is not inserted.

 */
void generateText(char *txt)		/* IN - The text to output */
{
  int i;

  for (i = 0; txt[i]; i++)
    incFreq(txt[i]);
  fprintf(txtfil, txt);
}




/*======================================================================*/
Message *newMessage(Srcp *srcp, IdNode *id, List *stms)
{
  Message *msg;

  showProgress();

  msg = NEW(Message);

  if (srcp) msg->srcp = *srcp;
  msg->id = id;
  msg->stms = stms;

  return(msg);
}


/*======================================================================*/
void prepareMessages(void)
{
  int msgno;
  List *smsgs = NULL;		/* The constructed list */
  List *umsgs;			/* Iteration pointer into user messages list */
  List *lst, *rest;
  /* The dummy list of statements */
  Statement *stm;
  List *stms = NULL;


  createMessageVerbs();

  if (sizeof(defmsg)/sizeof(defmsg[0]) != MSGMAX+1)
    SYSERR("Incorrect number of messages in message tables");

  switch (opts[OPTLANG].value) {
  case L_ENGLISH:
  case L_SWEDISH:
  case L_GERMAN:
    break;
  default:
    SYSERR("Unrecognised switch");
    return;
  }

  /* For all messages in the pointed to table */
  for (msgno = 0; defmsg[msgno].id; msgno++) {
    Message *msg;
    char *msgp = NULL;

    if (msgno != defmsg[msgno].messageCode)
      SYSERR("Wrong code in message table");

    /* Create a message node */
    switch (opts[OPTLANG].value) {
    case L_ENGLISH:
      msgp = defmsg[msgno].english;
      break;
    case L_SWEDISH:
      msgp = defmsg[msgno].swedish;
      break;
    case L_GERMAN:
      msgp = defmsg[msgno].german;
      break;
    }
    stm = newPrintStatementFromString(msgp);
    /* Make a list of it */
    stms = concat(NULL, stm, STATEMENT_LIST);
    msg = newMessage(NULL, NULL, stms);
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
    generateText(msgp);
#endif
#endif
    /* Finally enter it in the list */
    msg->msgno = msgno;
    smsgs = concat(smsgs, msg, MESSAGE_LIST);
  }

  /* Merge user messages to the list */
  for (umsgs = adv.msgs; umsgs; umsgs = umsgs->next) {
    List *garb;			/* The standard message stm list is garbage */
    /* Find what number the user defined message has */
    for (msgno = 0; defmsg[msgno].id != NULL; msgno++)
      if (compareStrings(defmsg[msgno].id, umsgs->element.msg->id->string) == 0)
	break;
    if (defmsg[msgno].id == NULL)
      lmLog(&umsgs->element.msg->id->srcp, 700, sevERR, NULL);
    else {
      /* Find that message in the system messages list */
      for (lst = smsgs; lst != NULL && lst->element.msg->msgno != msgno; lst = lst->next);
      if (lst == NULL)
	SYSERR("Reached end of system messages list");

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


/*----------------------------------------------------------------------*/
Context *contextFor(MsgKind messageNo) {
  switch (messageNo) {
  case M_WHICH_ONE_START:
  case M_WHICH_ONE_COMMA:
  case M_WHICH_ONE_OR:
  case M_NO_SUCH:
  case M_CANT:
  case M_SEE_OBJ_START:
  case M_SEE_OBJ_COMMA:
  case M_SEE_OBJ_AND:
  case M_SEE_ACT:
  case M_CONTAINS:
  case M_CARRIES:
  case M_EMPTY:
  case M_EMPTYHANDED:
    return newVerbContext(messageVerbSymbolForInstance);

  case M_WHAT_WORD:
  case M_AFTER_BUT:
  case M_UNKNOWN_WORD:
    return newVerbContext(messageVerbSymbolForString);

  case M_BUT_ALL:
    return newVerbContext(messageVerbSymbolFor2Strings);

  case M_CANNOTCONTAIN:
    return newVerbContext(messageVerbSymbolFor2Instances);

  case M_SCORE:
    return newVerbContext(messageVerbSymbolFor2Integers);

  case M_UNDONE:
    return newVerbContext(messageVerbSymbolForString);

  case M_WHAT:
  case M_MULTIPLE:
  case M_NOUN:
  case M_NOT_MUCH:
  case M_NO_WAY:
  case M_CANT0:
  case M_SEE_OBJ_END:
  case M_CONTAINS_COMMA:
  case M_CONTAINS_AND:
  case M_CONTAINS_END:
  case M_MORE:
  case M_AGAIN:
  case M_SAVEWHERE:
  case M_SAVEOVERWRITE:
  case M_SAVEFAILED:
  case M_SAVEMISSING:
  case M_NOTASAVEFILE:
  case M_SAVEVERS:
  case M_SAVENAME:
  case M_RESTOREFROM:
  case M_REALLY:
  case M_QUITACTION:
  case M_INDEFINITE:
  case M_DEFINITE:
  case M_PRONOUN:
  case M_NO_UNDO:
    return NULL;
  case MSGMAX:
    SYSERR("Unexpected message number");
  }
  return NULL;
}


/*======================================================================*/
void analyzeMessages(void)
{
  List *lst;

  /* Nothing to do except to analyze the statements */
  for (lst = adv.msgs; lst; lst = lst->next) {
    Message *msg = lst->element.msg;
    showProgress();
    analyzeStatements(msg->stms, contextFor(msg->msgno));
  }

}


/*----------------------------------------------------------------------

  gemsgent()

  Generate statements for the message construct

  */
static void gemsgent(Message *msg)
{
  msg->stmadr = nextEmitAddress();	/* Save address to messages statements */
  generateStatements(msg->stms);
  emit0(I_RETURN);
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

  adr = nextEmitAddress();		/* Save address to messages table */
  for (lst = adv.msgs; lst; lst = lst->next)
    emit(lst->element.msg->stmadr);
  emit(EOF);
  return(adr);
}

