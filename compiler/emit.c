/*----------------------------------------------------------------------*\

  emit.c


\*---------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#include "alan.h"

#include "acode.h"
#include "version.h"

#include "emit.h"


static FILE *acdfil;
static Aword buff[BLOCKLEN];

static int pc = 0;
static Aword crc = 0;


#ifdef _PROTOTYPES_
static void buffer(Aword w)
#else
static void buffer(w)
     Aword w;
#endif
{
  crc += w&0xff;			/* Check sum calculation */
  crc += (w>>8)&0xff;
  crc += (w>>16)&0xff;
  crc += (w>>24)&0xff;

  buff[pc++%BLOCKLEN] = w;
  if (pc%BLOCKLEN == 0)
    fwrite(buff, BLOCKSIZE, 1, acdfil);
}


#ifdef REVERSED
#ifdef _PROTOTYPES_
static Aword reversed(Aword w)
             			/* IN - The ACODE word to swap bytes in */
#else
static Aword reversed(w)
     Aword w;			/* IN - The ACODE word to swap bytes in */
#endif
{
  Aword s;			/* The swapped ACODE word */
  char *wp, *sp;
  int i;
  
  wp = (char *) &w;
  sp = (char *) &s;

  for (i = 0; i < sizeof(Aword); i++)
    sp[sizeof(Aword)-1 - i] = wp[i];

  return (s);
}
#endif


#ifdef _PROTOTYPES_
Aword emadr(void)
#else
Aword emadr()
#endif
{
  return(pc);
}


#ifdef _PROTOTYPES_
void emit(Aword c)
             			/* IN - Constant to emit */
#else
void emit(c)
     Aword c;			/* IN - Constant to emit */
#endif
{
#ifdef REVERSED
      buffer(reversed(c));
#else
      buffer(c);
#endif
}


/*----------------------------------------------------------------------

  emitstr()

  Function to emit strings to the ACD file. Note that strings are
  *always* stored with their first character at the lowest address so
  we have to be careful how we do this.

  On all machines strings can be copied word by word, except if they are
  not aligned on words. In this case we have to read them byte by byte and
  create the words. And in this case of course on reversed architectures
  the word must be reversed before emitting it.

*/
#ifdef _PROTOTYPES_
void emitstr(char *str)
#else
void emitstr(str)
     char str[];
#endif
{
  int i;
  char *copy;

  copy = allocate(strlen(str)+1);
  toIso(copy, str);

#ifdef WORDADDRESS
  {
    Aword w;

    for (i = 0; i < strlen(copy) + 1; i = i+4) {
      w =  (unsigned long)((unsigned char)copy[i])<<24;
      w += (unsigned long)((unsigned char)copy[i+1])<<16;
      w += (unsigned long)((unsigned char)copy[i+2])<<8;
      w += (unsigned long)((unsigned char)copy[i+3]);
#ifdef REVERSED
      buffer(reversed(w));
#else
      buffer(w);
#endif
    }
  }
#else
  {
    Aword *w;
    int len = strlen(copy) + 1;

    for (i = 0; i < len; i = i+4) {
      w =  (Aword *)&copy[i];
      buffer(*w);
    }
  }
#endif
}


#ifdef _PROTOTYPES_
void emit0(
     OpClass class,		/* IN - Operation class */
     Aword op			/* IN - Operation, instr, const or var */
)
#else
void emit0(class, op)
     OpClass class;		/* IN - Operation class */
     Aword op;			/* IN - Operation, instr, const or var */
#endif
{
  emit(((Aword)class<<28)|((Aword)op&0x0fffffff));
}


#ifdef _PROTOTYPES_
void eminit(
     char *acdfnm		/* IN - File name from ACODE instructions */
)
#else
void eminit(acdfnm)
     char acdfnm[];		/* IN - File name from ACODE instructions */
#endif
{
  int i;

  acdfil = fopen(acdfnm, WRITE_MODE);

  /* Make space for ACODE header */
  for (i=0; i<sizeof(AcdHdr)/sizeof(Aword); i++)
    emit(0L);

}


#ifdef _PROTOTYPES_
void emterm(AcdHdr *hdr)
#else
void emterm(hdr)
     AcdHdr *hdr;
#endif
{
  Aword *hp;			/* Pointer to header as words */
  Aword v;
  char *vp = (char *)&v;
  int i;

  if (pc%BLOCKSIZE > 0)
    fwrite(buff, BLOCKSIZE, 1, acdfil);

  hdr->acdcrc = crc;		/* Save checksum */

  (void) rewind(acdfil);
  pc = 0;

  /* Construct version marking */
#ifdef REVERSED
  vp[3] = (Aword)product.version.version;
  vp[2] = (Aword)product.version.revision;
  vp[1] = (Aword)product.version.correction;
  vp[0] = (Aword)product.version.state[0];
#else
  vp[0] = (Aword)product.version.version;
  vp[1] = (Aword)product.version.revision;
  vp[2] = (Aword)product.version.correction;
  vp[3] = (Aword)product.version.state[0];
#endif
  hdr->vers = v;

  hp = (Aword *) hdr;		/* Point to header */
  for (i=0; i<sizeof(AcdHdr)/sizeof(Aword); i++) /* Emit header */
    emit(*hp++);
  fwrite(buff, sizeof(AcdHdr), 1, acdfil); /* Flush first block out */
  fclose(acdfil);
}
