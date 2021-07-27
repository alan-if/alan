/*----------------------------------------------------------------------*\

  emit.c


\*---------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"
#include "util.h"

#include <sys/time.h>

#include "alan.h"


#include "acode.h"
#include "alan.version.h"

#include "emit.h"

#include "srcp_x.h"

/* PUBLIC DATA */
ACodeHeader acodeHeader;


/* Only visible in emit.h if UNITTESTING */
FILE *acdfil;
static Aword *emitBuffer;
static Aword eBuffer[BLOCKLEN];

static Aaddr pc;
static Aword crc;


/*----------------------------------------------------------------------*/
static void buffer(Aword w)
{
    crc += w&0xff;			/* Check sum calculation */
    crc += (w>>8)&((Aword)0xff);
    crc += (w>>16)&((Aword)0xff);
    crc += (w>>24)&((Aword)0xff);

    emitBuffer[pc++%BLOCKLEN] = w;
    if (pc%BLOCKLEN == 0)
        fwrite(emitBuffer, BLOCKSIZE, 1, acdfil);
}


/*======================================================================*/
Aword reversed(Aword w)		/* IN - The ACODE word to swap bytes in */
{
    Aword s;			/* The swapped ACODE word */
    char *wp, *sp;

    wp = (char *) &w;
    sp = (char *) &s;

    for (int i = 0; i < sizeof(Aword); i++)
        sp[sizeof(Aword)-1 - i] = wp[i];

    return (s);
}

/*======================================================================*/
Aword nextEmitAddress(void)
{
    return(pc);
}

/*======================================================================*/
void emit(Aword c)		/* IN - Constant to emit */
{
    if (littleEndian())
        buffer(reversed(c));
    else
        buffer(c);
}


/*======================================================================*/
void emitEntry(void *entry, int noOfBytes)
{
    Aword *words = entry;

    /* Should never start an entry with an EOF word since the reversal process
       depends on it for terminating. */
    if (words[0] == EOF) SYSERR("First word of an entry should never be EOF", nulsrcp);

    if (noOfBytes%sizeof(Aword) != 0) SYSERR("Emitting unaligned data", nulsrcp);

    for (int i = 0; i < noOfBytes/sizeof(Aword); i++)
        if (littleEndian())
            buffer(reversed(words[i]));
        else
            buffer(words[i]);
}


/*======================================================================*/
void emitN(void *address, int noOfWords) /* IN - Constant to emit */
{
    Aword *words = address;

    for (int i = 0; i < noOfWords; i++)
        if (littleEndian())
            buffer(reversed(words[i]));
        else
            buffer(words[i]);
}


/*======================================================================

  emitString()

  Function to emit strings to the ACD file. Note that strings are
  *always* stored with their first character at the lowest address so
  we have to be careful how we do this.

  On all machines strings can be copied word by word, except if they are
  not aligned on words. In this case we have to read them byte by byte and
  create the words. And in this case of course on reversed architectures
  the word must be reversed before emitting it.

*/
void emitString(char *str)
{
#ifdef WORDADDRESS
    Aword w;

    for (int i = 0; i < strlen(str) + 1; i = i+4) {
        w =  (unsigned long)((unsigned char)str[i])<<24;
        w += (unsigned long)((unsigned char)str[i+1])<<16;
        w += (unsigned long)((unsigned char)str[i+2])<<8;
        w += (unsigned long)((unsigned char)str[i+3]);
        if (littleEndian())
            buffer(reversed(w));
        else
            buffer(w);
    }
#else
    Aword *w;
    int len = strlen(str) + 1;

    for (int i = 0; i < len; i = i+4) {
        w =  (Aword *)&str[i];
        buffer(*w);
    }
#endif
}


/*======================================================================*/
void emitVariable(Aword var)
{
    emit(((Aword)C_CURVAR<<28)|((Aword)var&0x0fffffff));
}


/*======================================================================*/
void emitConstant(int arg)
{
    emit(((Aword)C_CONST<<28)|((Aword)arg&0x0fffffff));
}


/*======================================================================*/
void emit0(Aword op)
{
    emit(INSTRUCTION(op));
}


/*======================================================================*/
void emit1(Aword op, Aword arg1)
{
    emitConstant(arg1);
    emit0(op);
}

/*======================================================================*/
void emit2(Aword op, Aword arg1, Aword arg2)
{
    emitConstant(arg2);
    emitConstant(arg1);
    emit0(op);
}

/*======================================================================*/
void emit3(Aword op, Aword arg1, Aword arg2, Aword arg3)
{
    emitConstant(arg3);
    emitConstant(arg2);
    emitConstant(arg1);
    emit0(op);
}


/*======================================================================*/
void initEmitBuffer(Aword *bufferToUse) {
    pc = 0;
    crc = 0;

    emitBuffer = bufferToUse;
}



/*======================================================================*/
void initEmit(char *acdfnm)	/* IN - File name for ACODE instructions */
{
    initEmitBuffer(eBuffer);

    acdfil = fopen(acdfnm, WRITE_MODE);
    if (!acdfil) {
        char errorString[1000];
        sprintf(errorString, "Could not open output file '%s' for writing.", acdfnm);
        SYSERR(errorString, nulsrcp);
    }

    /* Make space for ACODE header */
    for (int i = 0; i < (sizeof(ACodeHeader)/sizeof(Aword)); i++)
        emit(0L);
}

/*----------------------------------------------------------------------*/
static void prepareHeader(ACodeHeader *acodeHeader) {

    struct timeval times;

    /* Generate header tag "ALAN" */
    if (littleEndian()) {
        /* Since we reverse these when emitting */
        acodeHeader->tag[3] = 'A';
        acodeHeader->tag[2] = 'L';
        acodeHeader->tag[1] = 'A';
        acodeHeader->tag[0] = 'N';
    } else {
        acodeHeader->tag[0] = 'A';
        acodeHeader->tag[1] = 'L';
        acodeHeader->tag[2] = 'A';
        acodeHeader->tag[3] = 'N';
    }

    /* Construct version marking */
    if (littleEndian()) {
        /* Since we reverse these when emitting */
        acodeHeader->version[3] = (Aword)alan.version.version;
        acodeHeader->version[2] = (Aword)alan.version.revision;
        acodeHeader->version[1] = (Aword)alan.version.correction;
        acodeHeader->version[0] = (Aword)alan.version.state[0];
    } else {
        acodeHeader->version[0] = (Aword)alan.version.version;
        acodeHeader->version[1] = (Aword)alan.version.revision;
        acodeHeader->version[2] = (Aword)alan.version.correction;
        acodeHeader->version[3] = (Aword)alan.version.state[0];
    }

    /* The timestamping isn't important, it is only used to give the
       compiled game a semi-unique id so that loading a saved game can
       not be done if it was not created with exactly the same compiled
       game. You can remove it or replace it by something else. It does
       not affect game compatibility. */
    gettimeofday(&times, NULL);
    acodeHeader->uid = times.tv_usec;
}


void finalizeEmit()
{
    if (pc%BLOCKSIZE > 0)
        fwrite(emitBuffer, BLOCKSIZE, 1, acdfil);

    acodeHeader.size = nextEmitAddress();	/* Save next address as size */

#ifdef EXTENDED_HEADER
    // I'll keep this for future reference if the header need to change size again
    /* From beta3 the header increased in length so to be somewhat
       backwards compatible we need to add the content of those words
       to the crc. We can do that by emitting them after we have
       registered memory size, since they will then be added to the
       crc, we just don't want to write them to the code file, not
       that it matters I think... */
    Aword *headerAsArray = (Aword *)&acodeHeader;
    for (int i = ASIZE(Pre3_0beta3Header); i < ASIZE(ACodeHeader); i++)
        emit(headerAsArray[i]);
#endif

    acodeHeader.acdcrc = crc;	/* Save checksum */
}


void copyTextDataToAcodeFile(char dataFileName[])
{
    int c;
    FILE *dataFile = fopen(dataFileName, READ_MODE);

    acodeHeader.stringOffset = ftell(acdfil);

    while ((c = fgetc(dataFile)) != EOF)
        fputc(c, acdfil);
    fclose(dataFile);
}


void writeHeader(ACodeHeader *acodeHeader)
{
    Aword *hp;			/* Pointer to header as words */

    prepareHeader(acodeHeader);

    pc = 0;

    hp = (Aword *) acodeHeader;		/* Point to header */
    for (int i = 0; i < (sizeof(ACodeHeader)/sizeof(Aword)); i++) /* Emit header */
        emit(*hp++);
    (void) rewind(acdfil);
    fwrite(emitBuffer, sizeof(ACodeHeader), 1, acdfil); /* Flush header out */
}


void terminateEmit(void) {
    fclose(acdfil);
}
