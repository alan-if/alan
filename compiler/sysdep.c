/*----------------------------------------------------------------------*\
  sysdep.c

  This file contains necessary system dependent routines.

  Notes on string handling:

  - Native - means the "natural" character set/encoding for the
  platform, nowadays often UTF-8, but that was not the case in the
  beginning.

  - Internal - is always ISO8859-1 in which encoding everything
  internal should use (even dictionary entries, which is not certain
  it does currently. TODO!

  - Current - the compiler (currently) has options for different
  charsets, if that is used all input files are considered to be in
  that encoding, which might be different from the native encoding.
  It (will) also auto-detect an UTF BOM and enforce UTF-8 for that
  single file, which again might be different from native or the one
  given using the -charset option.

  \*----------------------------------------------------------------------*/

#include "sysdep.h"

#include <time.h>


/* The following work on Internal/ISO characters */

#ifdef USE_FOR_LOWCHRS
static char lowChrs[] =
{
    0x61, /* a */  0x62, /* b */  0x63, /* c */  0x64, /* d */
    0x65, /* e */  0x66, /* f */  0x67, /* g */  0x68, /* h */
    0x69, /* i */  0x6A, /* j */  0x6B, /* k */  0x6C, /* l */
    0x6D, /* m */  0x6E, /* n */  0x6F, /* o */  0x70, /* p */
    0x71, /* q */  0x72, /* r */  0x73, /* s */  0x74, /* t */
    0x75, /* u */  0x76, /* v */  0x77, /* w */  0x78, /* x */
    0x79, /* y */  0x7A, /* z */  0xDF, /* ss <small sharp s> */
    0xE0, /* a grave */           0xE1, /* a acute */
    0xE2, /* a circumflex */      0xE3, /* a tilde */
    0xE4, /* a diaeresis */       0xE5, /* a ring */
    0xE6, /* ae */                0xE7, /* c cedilla */
    0xE8, /* e grave */           0xE9, /* e acute */
    0xEA, /* e circumflex */      0xEB, /* e diaeresis */
    0xEC, /* i grave */           0xED, /* i acute */
    0xEE, /* i circumflex */      0xEF, /* i diaeresis */
    0xF0, /* <small eth> */       0xF1, /* n tilde */
    0xF2, /* o grave */           0xF3, /* o acute */
    0xF4, /* o circumflex */      0xF5, /* o tilde */
    0xF6, /* o diaeresis */       0xF8, /* o slash */
    0xF9, /* u grave */           0xFA, /* u acute */
    0xFB, /* u circumflex */      0xFC, /* u diaeresis */
    0xFD, /* y acute */           0xFE, /* <small thorn> */
    0xFF, /* y diaeresis */       0x00
};

/* FIXME: ss <small sharp s> and y diaeresis have no UC analogues
   Are they really considered LC?
*/
#endif

static char uppChrs[] =
{
    0x41, /* A */  0x42, /* B */  0x43, /* C */  0x44, /* D */
    0x45, /* E */  0x46, /* F */  0x47, /* G */  0x48, /* H */
    0x49, /* I */  0x4A, /* J */  0x4B, /* K */  0x4C, /* L */
    0x4D, /* M */  0x4E, /* N */  0x4F, /* O */  0x50, /* P */
    0x51, /* Q */  0x52, /* R */  0x53, /* S */  0x54, /* T */
    0x55, /* U */  0x56, /* V */  0x57, /* W */  0x58, /* X */
    0x59, /* Y */  0x5A, /* Z */
    0xC0, /* A grave */           0xC1, /* A acute */
    0xC2, /* A circumflex */      0xC3, /* A tilde */
    0xC4, /* A diaeresis */       0xC5, /* A ring */
    0xC6, /* AE */                0xC7, /* C cedilla */
    0xC8, /* E grave */           0xC9, /* E acute */
    0xCA, /* E circumflex */      0xCB, /* E diaeresis */
    0xCC, /* I grave */           0xCD, /* I acute */
    0xCE, /* I circumflex */      0xCF, /* I diaeresis */
    0xD0, /* <capital eth> */     0xD1, /* N tilde */
    0xD2, /* O grave */           0xD3, /* O acute */
    0xD4, /* O circumflex */      0xD5, /* O tilde */
    0xD6, /* O diaeresis */       0xD8, /* O slash */
    0xD9, /* U grave */           0xDA, /* U acute */
    0xDB, /* U circumflex */      0xDC, /* U diaeresis */
    0xDD, /* Y acute */           0xDE, /* <capital thorn> */
    0x00
};

static int isUpperCase(unsigned int c)          /* IN - ISO character to test */
{
    for (int i = 0; i < strlen(uppChrs); i++)
        if (uppChrs[i] == c) return 1;
    return 0;
}


static char toLowerCase(int c)         /* IN - ISO character to convert */
{
    return (isUpperCase(c)? c + ('a' - 'A'): c);
}


/*----------------------------------------------------------------------*/
/* Case insensitive string comparison on internal strings */
int compareStrings(char *str1, char *str2)
{
    char *s1 = str1, *s2 = str2;

    while (*s1 != '\0' && *s2 != '\0') {
        if (toLowerCase(*s1) < toLowerCase(*s2)) return -1;
        if (toLowerCase(*s1) > toLowerCase(*s2)) return 1;
        s1++;
        s2++;
    }
    return toLowerCase(*s2) - toLowerCase(*s1);
}

/*======================================================================*/
int littleEndian() {
    int x = 1;
    return (*(char *)&x == 1);
}


/*======================================================================*/
char *baseNameStart(char *fullPathName) {
    static char *delimiters = "\\>]/:";
    int i;

    for (i = strlen(fullPathName)-1; i > 0; i--)
        if (strchr(delimiters, fullPathName[i]) != NULL)
            return &fullPathName[i+1];
    return(fullPathName);
}
