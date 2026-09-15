/*----------------------------------------------------------------------*\

  IFID.C

  IFID handling

  \*----------------------------------------------------------------------*/

#include "ifid_x.h"

#include <stdio.h>
#include <sys/time.h>

#include "srcp_x.h"
#include "str.h"
#include "dump_x.h"
#include "util.h"
#include "emit.h"
#include "adv_x.h"




/*======================================================================*/
IfidNode *newIfid(Srcp srcp, char *name, char *value)
{
    IfidNode *new;			/* The newly allocated area */

    progressCounter();

    new = NEW(IfidNode);

    new->srcp = srcp;
    new->name  = newString(name);
    new->value = value;

    return new;
}




/*======================================================================*/
static void fillRandomBytes(unsigned char buffer[], int nbytes)
{
    static int initted = 0;
    struct timeval times;
    long time_now;

    if (!initted) {
        gettimeofday(&times, NULL);
        time_now = times.tv_usec;
        srand(time_now);
        initted = 1;
    }

    for (int i = 0; i < nbytes; i++) {
        *buffer++ = rand() & 0xFF;
    }
}



/* An IFID is a UUID in this format, where each 'x' is a hex digit */
static const char uuidFormat[] = "UUID://xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx//";


/*======================================================================*/
static char randomHexDigit(void)
{
    unsigned char byte;

    fillRandomBytes(&byte, 1);
    return "0123456789abcdef"[byte & 0xF];
}


/*======================================================================*/
static char *randomUUID(void)
{
    /* The IFID goes into the acode string table, so a UUID of the wrong
       length moves every instruction address after it. That is what made
       the debug/trace regression fail at random before 65ae39f4, when bytes
       from 0x80 up were printed as eight hex digits instead of two. It
       looked fixed by re-running because readOrCreateIFID keeps the first
       *valid* UUID in the .ifid file and reuses it from then on. Filling in
       uuidFormat always gives the right length. */
    static char uuid[sizeof(uuidFormat)];

    for (int i = 0; uuidFormat[i] != '\0'; i++)
        uuid[i] = uuidFormat[i] == 'x' ? randomHexDigit() : uuidFormat[i];
    return uuid;
}


/*======================================================================*/
static bool isValidUUID(char *uuid)
{
    if (strlen(uuid) != strlen(uuidFormat))
        return false;
    for (int i = 0; uuidFormat[i] != '\0'; i++)
        if (uuidFormat[i] == 'x' ? !isxdigit((unsigned char)uuid[i]) : uuid[i] != uuidFormat[i])
            return false;
    return true;
}


/*======================================================================*/
static char *readOrCreateIFID(void)
{
    char ifidfnm[255] = "";
    FILE *ifidFile;
    static char buffer[sizeof(uuidFormat)];

    /* -- create IFID file name -- */
    if (adv.name)
        strcpy(ifidfnm, adv.name);
    strcat(ifidfnm, ".ifid");

    if ((ifidFile = fopen(ifidfnm, "r")) != NULL)
        (void)!fread(buffer, strlen(uuidFormat), 1, ifidFile);
    else
        buffer[0] = '\0';
    if (!isValidUUID(buffer)) {
        strcpy(buffer, randomUUID());
        if ((ifidFile = fopen(ifidfnm, "w")) != NULL)
            fwrite(buffer, strlen(uuidFormat), 1, ifidFile);
    }

    return buffer;
}


/*======================================================================*/
List *initIfids(void)
{
    return newList(newIfid(nulsrcp, newString("IFID"), readOrCreateIFID()), IFID_LIST);
}


/*======================================================================*/
static void generateIfid(IfidNode *ifid)
{
    ifid->nameAddress = nextEmitAddress();
    emitString(ifid->name);
    ifid->valueAddress = nextEmitAddress();
    emitString(ifid->value);
}


/*======================================================================*/
Aaddr generateIfids(List *ifids)
{
    List *lst;
    Aaddr ifidAddress;
    IfidEntry entry;

    ITERATE(lst, ifids)
        generateIfid(lst->member.ifid);

    ifidAddress = nextEmitAddress();
    ITERATE(lst, ifids) {
        entry.nameAddress = lst->member.ifid->nameAddress;
        entry.valueAddress = lst->member.ifid->valueAddress;
        emitEntry(&entry, sizeof(entry));
    }
    emit(EOF);
    return ifidAddress;
}


/*======================================================================*/
void dumpIfid(IfidNode *ifid)
{
    if (ifid == NULL) {
        put("NULL");
        return;
    }

    put("IFID: "); dumpSrcp(ifid->srcp);
    dumpString(ifid->name); put("="); dumpString(ifid->value);
}
