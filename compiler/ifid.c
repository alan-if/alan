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

  return(new);
}




/*======================================================================*/
static void fillRandomBytes(char buffer[], int nbytes)
{
  static int initted = 0;
  struct timeval times;
  long time_now;
  int i;

  if (!initted) {
    gettimeofday(&times, NULL);
    time_now = times.tv_usec;
    srand(time_now);
    initted = 1;
  }

  for (i = 0; i < nbytes; i++) {
    *buffer++ = rand() & 0xFF;
  }
}



/*======================================================================*/
static char *randomUUID(void)
{
  char buffer[16];
  int b, s;
  static char string[100];	/* 32 hexdigits, 4 dashes, 9 "UUID:////"
                   00112233-4455-6677-8899-aabbccddeeff */
  int i, j;

  fillRandomBytes(buffer, 16); b = 0;
  sprintf(string, "UUID://"); s = 7;
  for (i = 0; i < 4; i++, s+=2, b++)
    sprintf(&string[s], "%2.2x", buffer[b]);
  strcat(string, "-"); s++;
  for (j = 0; j < 3; j++) {
    for (i = 0; i < 2; i++, s+=2, b++)
      sprintf(&string[s], "%2.2x", buffer[b]);
    strcat(string, "-"); s++;
  }
  for (i = 0; i < 6; i++, s+=2, b++)
    sprintf(&string[s], "%2.2x", buffer[b]);
  strcat(string, "//");
  return string;
}


/*======================================================================*/
static bool isValidUUID(char *uuid)
{
  int i;

  if (strlen(uuid) != 45) return false;
  if (strncmp(uuid, "UUID://", 7) != 0) return false;
  for (i = 7; i <= 14; i++) if (!isxdigit((uint)uuid[i])) return false;
  if (uuid[15] != '-') return false;
  for (i = 16; i <= 19; i++) if (!isxdigit((uint)uuid[i])) return false;
  if (uuid[20] != '-') return false;
  for (i = 21; i <= 24; i++) if (!isxdigit((uint)uuid[i])) return false;
  if (uuid[25] != '-') return false;
  for (i = 26; i <= 29; i++) if (!isxdigit((uint)uuid[i])) return false;
  if (uuid[30] != '-') return false;
  for (i = 31; i <= 42; i++) if (!isxdigit((uint)uuid[i])) return false;
  if (strcmp(&uuid[43], "//") != 0) return false;
  return true;
}


/*======================================================================*/
static char *readOrCreateIFID(void)
{
  char ifidfnm[255] = "";
  FILE *ifidFile;
  static char buffer[100];	/* 32 hexdigits, 4 dashes, 9 "UUID:////" */

  /* -- create IFID file name -- */
  if (adv.name)
    strcpy(ifidfnm, adv.name);
  strcat(ifidfnm, ".ifid");

  if ((ifidFile = fopen(ifidfnm, "r")) != NULL)
      (void)!fread(buffer, 45, 1, ifidFile);
  else
    buffer[0] = '\0';
  if (!isValidUUID(buffer)) {
    strcpy(buffer, randomUUID());
    if ((ifidFile = fopen(ifidfnm, "w")) != NULL)
      fwrite(buffer, 45, 1, ifidFile);
  }

  return buffer;
}


/*======================================================================*/
List *initIfids(void)
{
  return(newList(newIfid(nulsrcp, newString("IFID"), readOrCreateIFID()), IFID_LIST));
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
