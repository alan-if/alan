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


#ifdef __amiga__
#include <intuition/intuition.h>
#include <workbench/workbench.h>
#include <functions.h>
extern struct Library *IconBase;
static UWORD iconData[200] = {
  0x3fff, 0xfff0, 0x3fff, 0xfe00, 0x0000, 0x3f75, 0xba76, 0xef77,
  0x7700, 0x0000, 0x37bd, 0xedbb, 0x7db5, 0xdf80, 0x0000, 0x33df,
  0x7e97, 0xbeff, 0x56c0, 0x0000, 0x31ef, 0x57ee, 0xdd5d, 0xbbe0,
  0x0000, 0x30f7, 0xbeb5, 0x6dbe, 0xeef0, 0x0000, 0x3c7b, 0x556e,
  0xb71f, 0xfff8, 0x0000, 0x333d, 0x6dab, 0x5b8f, 0xfffc, 0x0000,
  0xb0fe, 0xffff, 0xedff, 0xfffe, 0x0000, 0xf01e, 0x0000, 0x0200,
  0x0001, 0x0000, 0x0e07, 0xffff, 0xfcff, 0xffff, 0x0000, 0x0003,
  0xffff, 0xffff, 0xffff, 0x8000, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0000, 0x0000, 0x4fff, 0xffff, 0xffff, 0xe000, 0x0000, 0x77ff,
  0xffff, 0xffff, 0xfc00, 0x0000, 0x7a66, 0x6666, 0x6667, 0x4f00,
  0x0000, 0x1e99, 0x9999, 0x9999, 0xb3c0, 0x0000, 0x03be, 0x6666,
  0x6667, 0xecf0, 0x0000, 0x0077, 0xf800, 0x0000, 0x7ffc, 0x0000,
  0x000f, 0xffff, 0xffff, 0xfffe,
  
  0x3f00, 0x0000, 0x0000, 0x0000, 0x0000, 0x3c0b, 0x6108, 0x1480,
  0x0000, 0x0000, 0x3e07, 0x5044, 0x13c8, 0x0000, 0x0000, 0x3f02,
  0xd148, 0x0520, 0x8900, 0x0000, 0x3d82, 0xa801, 0x02f0, 0x4000,
  0x0000, 0x3cc0, 0x014a, 0x8241, 0x1100, 0x0000, 0x3f60, 0x0010,
  0x40e0, 0x0000, 0x0000, 0x3ff0, 0x9254, 0x8070, 0x0000, 0x0000,
  0xbcf8, 0x0000, 0x1000, 0x0000, 0x0000, 0xfc1d, 0x0000, 0x0000,
  0x0000, 0x2920, 0x0f06, 0x0000, 0x0000, 0x0000, 0xffe0, 0x0003,
  0xffff, 0xffff, 0xffff, 0x80f8, 0x0000, 0x0000, 0x0000, 0x0000,
  0x0068, 0x0000, 0x0000, 0x0000, 0x0000, 0x03a0, 0x0000, 0x0000,
  0x0000, 0x0000, 0x03e0, 0x0000, 0x0099, 0x9999, 0x9998, 0xb000,
  0x0000, 0x0066, 0x6666, 0x6666, 0x4c00, 0x0000, 0x0001, 0x9999,
  0x9998, 0x1300, 0x0000, 0x0000, 0x07ff, 0xffff, 0x8000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000
  };
static struct Image iconImage = {
  0,				/* LeftEdge */
  0,				/* TopEdge */
  79,				/* Width */
  20,				/* Height */
  2,				/* Depth */
  &iconData,			/* ImageData */
  3,				/* PlanePick */
  0,				/* PlaneOnOff */
  NULL				/* NextImage */
  };
static struct DiskObject iconObject = {
  WB_DISKMAGIC, WB_DISKVERSION,
  {
    NULL,			/* NextGadget */
    215,			/* LeftEdge */
    20,				/* TopEdge */
    79,				/* Width */
    21,				/* Height */
    GADGIMAGE|GADGBACKFILL,	/* Flags */
    RELVERIFY|GADGIMMEDIATE,	/* Activation */
    BOOLGADGET,			/* GadgetType */
    &iconImage,			/* GadgetRender */
    NULL,			/* SelectRender */
    NULL,			/* GadgetText */
    0,				/* MutualExclude */
    NULL,			/* SpecialInfo */
    0,				/* GadgetId */
    NULL			/* UserData */
    },
  WBPROJECT,			/* Type */
  "AlanDir:arun",		/* DefaultTool */
  NULL,				/* ToolTypes */
  NO_ICON_POSITION,		/* CurrentX */
  NO_ICON_POSITION,		/* CurrentY */
  NULL,				/* DrawerData */
  NULL,				/* ToolWindow */
  NULL				/* StackSize */
  };
#endif

#ifdef _PROTOTYPES_
void eminit(
     char *acdfnm		/* IN - File name for ACODE instructions */
)
#else
void eminit(acdfnm)
     char acdfnm[];		/* IN - File name for ACODE instructions */
#endif
{
  int i;

#ifdef __amiga__
  struct DiskObject *existingIcon;

  if ((IconBase = OpenLibrary("icon.library", 0)) == NULL)
      syserr("Could not open 'icon.library'");
  if ((existingIcon = GetDiskObject(advnam)) == 0)
      PutDiskObject(advnam, &iconObject);
  else
      FreeDiskObject(existingIcon);
#endif

  acdfil = fopen(acdfnm, WRITE_MODE);

  /* Make space for ACODE header */
  for (i = 0; i < sizeof(AcdHdr)/sizeof(Aword); i++)
      emit(0L);

#ifdef __mac__
  /* Add FinderInfo to point to Arun */
  {
    char fnm[256];
    short vRefNum = 0;
    FInfo finfo;
    OSErr oe;

    fnm[0] = (char)strlen(acdfnm);
    strncpy(&fnm[1], acdfnm, fnm[0]);
    oe = GetFInfo((ConstStr255Param *)fnm, 0, &finfo);
    printf("GetFInfo oe = %d\n", oe);

    strncpy((char *)&finfo.fdType, "TEXT", 4);
    strncpy((char *)&finfo.fdCreator, "Arun", 4);
    oe = SetFInfo((unsigned char *)fnm, 0, &finfo);
    printf("SetFInfo oe = %d\n", oe);
  }
#endif
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
