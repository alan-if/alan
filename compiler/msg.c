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
#include "lmlog.h"

#include "msg.h"
#include "opt.h"

#include "emit.h"
#include "encode.h"
#include "acode.h"



/* PRIVATE: */

static struct {int messageCode; char *id; char *english; char *swedish; char *german;} defmsg[] =
    {
        {M_UNKNOWN_WORD, "unknown_word",
         "I don't know the word '$1'.",
         "Jag k\xE4nner inte till ordet '$1'.",
         "Ich kenne das Wort '$1' nicht."},
        {M_WHAT, "what",
         "I don't understand.",
         "Jag f\xF6rst\xE5r inte.",
         "Ich ferste nicht."},
        {M_WHAT_WORD, "what_word",
         "I don't know what you mean by '$1'.",
         "Jag vet inte vad du menar med '$1'.",
         "Ich verstehe nicht was Du mit '$1' meinst."},
        {M_MULTIPLE, "multiple",
         "You can't refer to multiple objects with '$v'.",
         "Du kan inte ange flera objekt tillsammans med verbet '$v'.",
         "Du kannst Dich nicht auf mehrere Gegenst\xE4nde mit '$v' beziehen."},
        {M_NOUN, "noun",
         "You must supply a noun.",
         "Du m\xE5ste ange ett substantiv.",
         "Du musst ein Gegenstandswort angeben."},
        {M_AFTER_BUT, "after_but",
         "You must give at least one object after '$1'.",
         "Det borde vara minst ett objekt efter '$1'.",
         "Du musst zum mindesten einen Gegenstand nach '$1' angeben."},
        {M_BUT_ALL, "but_all",
         "You can only use '$1' after '$2'.",
         "Du kan bara anv\xE4nda '$1' efter '$2'.",
         "Du kannst '$1' nur nach '$2' verwenden."},	/* german: "Nimm alles ausser der Lampe." */
        {M_NOT_MUCH, "not_much",
         "That doesn't leave much to $v!",
         "Det blir inte s\xE5 mycket kvar att $v d\xE5!",
         "Das überl\xE4\xDFt viel nicht zu $v!"},
        {M_WHICH_ONE_START, "which_start",
         "I don't know if you mean $+1",
         "Jag vet inte om du menar $+1",
         "Ich wei\xDF nicht, falls Du $+1"},
        {M_WHICH_ONE_COMMA, "which_comma",
         ", $+1",
         ", $+1",
         ", $+1"},
        {M_WHICH_ONE_OR, "which_or",
         "or $+1.",
         "eller $+1.",
         "oder $+1 meinst."},
        {M_NO_SUCH, "no_such",
         "I can't see any $1 here.",
         "Jag ser ingen $1 h\xE4r.",
         "Ich kann hier keinen $1 sehen."},
        {M_NO_WAY, "no_way",
         "You can't go that way.",
         "Du kan inte g\xE5 \xE5t det h\xE5llet.",
         "Du kannst nicht in diese Richtung gehen."},
        {M_CANT0, "cant0",
         "You can't do that.",
         "Det g\xE5r inte att g\xF6ra.",
         "Du kannst das nicht tun."},
        {M_SEE_START, "see_start",
         "There is $01",
         "Det finns $01",
         "Es gibt $01"},
        {M_SEE_COMMA, "see_comma",
         ", $01",
         ", $01",
         ", $01"},
        {M_SEE_AND, "see_and",
         "and $01",
         "och $01",
         "und $01"},
        {M_SEE_END, "see_end",
         "here.",
         "h\xE4r.",
         "hier."},
        /* Two messages for default HEADER (objects and actors) */
        {M_CONTAINS, "contains",
         "$+1 contains",
         "$+1 inneh\xE5ller",
         "$+1 enth\xE4lt"},
        {M_CARRIES, "carries",
         "$+1 carries",
         "$+1 b\xE4r p\xE5",
         "$+1 tr\xE4gt"},
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
         "$+1 \xE4r tom.",
         "$+1 ist leer."},
        {M_EMPTYHANDED, "empty_handed",
         "$+1 is empty-handed.",
         "$+1 \xE4r tomh\xE4nt.",
         "$+1 tr\xE4gt nichts."},
        {M_CANNOTCONTAIN, "can_not_contain",
         "$+1 can not contain $+2.",
         "$+1 kan inte inneh\xE5lla $+2.",
         "$+1 kan $+2 nicht tr\xE4gen."},
        {M_SCORE, "have_scored",
         "You have scored $1 points out of $2 in $3 moves.",
         "Du har samlat $1 po\xE4ng av $2 p\xE5 $3 drag.",
         "Du hast $1 Punkte von $2 in $3 bewegt erzielt."},
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
         "Spara \xE4ventyret i vilken fil",
         "Gib den Dateinamen an, um zu speichern"},
        {M_SAVEOVERWRITE, "save_overwrite",
         "That file already exists, overwrite (RETURN confirms) ? ",
         "Filen fanns redan, skriva \xF6ver (RETURN f\xF6r ja) ? ",
         "Die Datei existiert bereits, überschreiben (ENTER, best\xE4tigt)? "},
        {M_SAVEFAILED, "save_failed",
         "Sorry, save failed.",
         "Tyv\xE4rr, det gick inte att spara \xE4ventyret.",
         "Das Spiel konnte nicht gespeichert werden."},
        {M_RESTOREFROM, "restore_from",
         "Enter file name to restore from",
         "H\xE4mta sparat \xE4ventyr fr\xE5n vilken fil",
         "Von welcher Datei soll das Spiel wieder hergestellt werden."},
        {M_SAVEMISSING, "save_missing",
         "Sorry, could not open that save file.",
         "Tyv\xE4rr, kunde inte hitta n\xE5got s\xE5""dant sparat \xE4ventyr.",
         "Konnte das gespeicherte Spiel nicht \xF6""ffnen."},
        {M_NOTASAVEFILE, "not_a_savefile",
         "That file does not seem to be an Alan game save file.",
         "Den filen verkar inte vara ett sparat Alan spel.",
         "Diese Datei scheint kein gespeichertes ALAN Spiel zu sein."},
        {M_SAVEVERS, "save_version",
         "Sorry, the save file was created by a different version.",
         "Tyv\xE4rr, det sparade \xE4ventyret har inte samma version.",
         "Diese Datei wurde mit einer anderen Version gespeichert."},
        {M_SAVENAME, "save_name",
         "Sorry, the save file did not contain a save for this adventure.",
         "Tyv\xE4rr, det var inte detta \xE4ventyr som var sparat i den filen.",
         "Diese gespeicherte Datei geh\xF6rt nicht zu diesem Spiel."},
        {M_REALLY, "really",
         "Are you sure (RETURN confirms) ? ",
         "\xC4r du s\xE4ker (RETURN f\xF6r ja) ? ",
         "Bist Du sicher (ENTER best\xE4tigt) ? "},
        {M_QUITACTION, "quit_action",
         "Do you want to UNDO, RESTART, RESTORE or QUIT ? ",
         "Vad vill du g\xF6ra, UNDO, RESTART, RESTORE or QUIT ? ",
         "M\xF6""chtest Du UNDO, RESTART, RESTORE oder QUIT ? "},	/* TODO: is this changeable to "NEUSTART, LADEN oder ENDE ?" ? */
        {M_UNDONE, "undone",
         "'$1' undone.",
         "'$1' ogjort.",
         "'$1' annuliert."},
        {M_NO_UNDO, "no_undo",
         "Nothing to undo.",
         "Det finns inget att g\xF6ra ogjort.",
         "Es gibt nichts zu annulieren."},
        {M_WHICH_PRONOUN_START, "which_pronoun_start",
         "I don't know what you mean by '$1',",
         "Jag vet inte vad du menar med '$1',",
         "Ich wei\xDF nicht, was Du mit '$1' meinst,"},
        {M_WHICH_PRONOUN_FIRST, "which_pronoun_first",
         "$+1",
         "$+1",
         "$+1"},
        {M_IMPOSSIBLE_WITH, "impossible_with",
         "That's impossible with $+1.",
         "Det g\xE5r inte med $+1.",
         "Es gibt nicht mit $+1."},
        {M_CONTAINMENT_LOOP, "containment_loop",
         "Putting $+1 in itself is impossible.",
         "Det \xE4r om\xF6jligt att stoppa $+1 i sig sj\xE4lv.",
         "Es ist unm\xF6glich, $+1 in sich selbst zu setzen."},
        {M_CONTAINMENT_LOOP2, "containment_loop2",
         "Putting $+1 in $+2 is impossible since $+2 already is inside $+1.",
         "Det \xE4r om\xF6jligt att stoppa $+1 i $+2 eftersom $+2 redan \xE4r inuti $+1.",
         "Es ist unm\xF6glich, $+1 in $+2 zu setzen, weil $+2 bereits in $+1 ist."},
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
    fprintf(txtfil, "%s", txt);
}


/*======================================================================*/
Message *newMessage(Srcp *srcp, Id *id, List *stms)
{
    Message *msg;

    progressCounter();

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
    List *stms = NULL;


    createMessageVerbs();

    if (sizeof(defmsg)/sizeof(defmsg[0]) != MSGMAX+1)
        SYSERR("Incorrect number of messages in message tables", nulsrcp);

    switch (opts[OPTLANG].value) {
    case L_ENGLISH:
    case L_SWEDISH:
    case L_GERMAN:
        break;
    default:
        SYSERR("Unrecognised switch", nulsrcp);
        return;
    }

    /* For all messages in the pointed to table */
    for (msgno = 0; defmsg[msgno].id; msgno++) {
        Message *msg;
        char *msgp = NULL;

        if (msgno != defmsg[msgno].messageCode)
            SYSERR("Wrong code in message table", nulsrcp);

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
        stms = newPrintStatementListFromString(msgp);
        msg = newMessage(NULL, NULL, stms);
        /* Save the message text */
#ifdef __mac__
        {
            char buf[256];
x
            toIso(buf, msgp);
            getxt(buf);
        }
#else
        generateText(msgp);
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
            if (compareStrings(defmsg[msgno].id, umsgs->member.msg->id->string) == 0)
                break;
        if (defmsg[msgno].id == NULL)
            lmlog(&umsgs->member.msg->id->srcp, 700, sevERR, NULL);
        else {
            /* Find that message in the system messages list */
            for (lst = smsgs; lst != NULL && lst->member.msg->msgno != msgno; lst = lst->next);
            if (lst == NULL)
                SYSERR("Reached end of system messages list", nulsrcp);

            /* Update the message statements */
            garb = lst->member.msg->stms;
            lst->member.msg->stms = umsgs->member.msg->stms;

            /* And system print statement and list and user message node */
            free(garb->member.stm);
            free(garb);
            free(umsgs->member.msg);
            umsgs->member.msg = NULL;
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
    case M_WHICH_PRONOUN_FIRST:
    case M_WHICH_ONE_OR:
    case M_NO_SUCH:
    case M_SEE_START:
    case M_SEE_COMMA:
    case M_SEE_AND:
    case M_CONTAINS:
    case M_CARRIES:
    case M_CONTAINS_COMMA:
    case M_CONTAINS_AND:
    case M_CONTAINS_END:
    case M_EMPTY:
    case M_EMPTYHANDED:
    case M_IMPOSSIBLE_WITH:
    case M_CONTAINMENT_LOOP:
        return newVerbContext(messageVerbSymbolForInstance);

    case M_WHAT_WORD:
    case M_AFTER_BUT:
    case M_UNKNOWN_WORD:
    case M_WHICH_PRONOUN_START:
        return newVerbContext(messageVerbSymbolForString);

    case M_BUT_ALL:
        return newVerbContext(messageVerbSymbolFor2Strings);

    case M_CANNOTCONTAIN:
    case M_CONTAINMENT_LOOP2:
        return newVerbContext(messageVerbSymbolFor2Instances);

    case M_SCORE:
        return newVerbContext(messageVerbSymbolFor3Integers);

    case M_UNDONE:
        return newVerbContext(messageVerbSymbolForString);

    case M_WHAT:
    case M_MULTIPLE:
    case M_NOUN:
    case M_NOT_MUCH:
    case M_NO_WAY:
    case M_CANT0:
    case M_SEE_END:
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
    case M_NO_UNDO:
        return NULL;
    case MSGMAX:
        SYSERR("Unexpected message number", nulsrcp);
    }
    return NULL;
}


/*======================================================================*/
void analyzeMessages(void)
{
    List *lst;

    /* Nothing to do except to analyze the statements */
    for (lst = adv.msgs; lst; lst = lst->next) {
        Message *msg = lst->member.msg;
        progressCounter();
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
        progressCounter();
        gemsgent(lst->member.msg);
    }

    adr = nextEmitAddress();		/* Save address to messages table */
    for (lst = adv.msgs; lst; lst = lst->next)
        emit(lst->member.msg->stmadr);
    emit(EOF);
    return(adr);
}
