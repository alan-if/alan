/*----------------------------------------------------------------------*\

  emit.c


\*---------------------------------------------------------------------*/

#include "sysdep.h"
#include "types.h"

#include "acode.h"
#include "version.h"


static FILE *acdfil;
static Aword buff[BLOCKLEN];

static int pc = 0;

static Boolean rev;


#ifdef _PROTOTYPES_
static void buffer(Aword w)
#else
static void buffer(w)
     Aword w;
#endif
{
  buff[pc++%BLOCKLEN] = w;
  if (pc%BLOCKLEN == 0)
    fwrite(buff, BLOCKSIZE, 1, acdfil);
}


#ifdef _PROTOTYPES_
static Aword swaplong(Aword w)
             			/* IN - The ACODE word to swap bytes in */
#else
static Aword swaplong(w)
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
  if (rev)
    buffer(swaplong(c));
  else
    buffer(c);
}


#ifdef _PROTOTYPES_
void emitstr(char *str)
#else
void emitstr(str)
     char str[];
#endif
{
  int i;
#ifdef WORDADDRESS
  Aword w;
  
  for (i = 0; i < strlen(str) + 1; i = i+4) {
    w =  (unsigned long)((unsigned char)str[i])<<24;
    w += (unsigned long)((unsigned char)str[i+1])<<16;
    w += (unsigned long)((unsigned char)str[i+2])<<8;
    w += (unsigned long)((unsigned char)str[i+3]);
#ifdef REVERSED
    buffer(swaplong(w));
#else
    buffer(w);
#endif
#else
  Aword *w;
  
  for (i = 0; i < strlen(str) + 1; i = i+4) {
    w =  (Aword *)&str[i];
    buffer(*w);
#endif
  }
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
     char *acdfnm,		/* IN - File name from ACODE instructions */
     Boolean revflg		/* IN - Reversed byte ordering */
)
#else
void eminit(acdfnm, revflg)
     char acdfnm[];		/* IN - File name from ACODE instructions */
     Boolean revflg;		/* IN - Reversed byte ordering */
#endif
{
  int i;

  rev = revflg;			/* Remember reversed bytes or not */

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
  int i;

  if (pc%BLOCKSIZE > 0)
    fwrite(buff, BLOCKSIZE, 1, acdfil);

#ifdef REVERSED
  hdr->rev = !rev;		/* Tell ARUN if reversed bytes */
#else
  hdr->rev = rev;		/* Tell ARUN if reversed bytes */
#endif

  (void) rewind(acdfil);
  pc = 0;

  /* Construct version marking */
  v = (Aword)product.version.version<<24;
  v+= (Aword)product.version.revision<<16;
  v+= (Aword)product.version.correction<<8;
  v+= (Aword)product.version.state[0];
  hdr->vers = v;

  hp = (Aword *) hdr;		/* Point to header */
  for (i=0; i<sizeof(AcdHdr)/sizeof(Aword); i++) /* Emit header */
    emit(*hp++);
  fwrite(buff, sizeof(AcdHdr), 1, acdfil); /* Flush first block out */
  fclose(acdfil);
}
