#ifndef _ACODE_H_
#define _ACODE_H_

typedef long Aword;		/* Type for an ACODE word */
typedef long Aaddr;		/* Type for an ACODE address */
typedef long Abool;		/* Type for an ACODE Boolean value */

/* Constants for the Acode file, words/block & bytes/block */
#define BLOCKLEN 256L
#define BLOCKSIZE (BLOCKLEN*sizeof(Aword))


/* Definitions for the packing process */
typedef long CodeValue;
#define VALUEBITS 16

#define EOFChar 256
#define TOPVALUE (((CodeValue)1<<VALUEBITS) - 1) /* Highest value possible */

/* Half and quarter points in the code value range */
#define ONEQUARTER (TOPVALUE/4+1)	/* Point after first quarter */
#define HALF (2*ONEQUARTER)		/* Point after first half */
#define THREEQUARTER (3*ONEQUARTER)	/* Point after third quarter */


/* AMACHINE Word Classes */
typedef int WrdKind;
#define  WRD_SYN 0		/* 1 - Synonym */
#define  WRD_ADJ 1		/* 2 - Adjective */
#define  WRD_ALL 2		/* 4 - All */
#define  WRD_BUT 3		/* 8 - But */
#define  WRD_CONJ 4		/* 16 - Conjunction */
#define  WRD_PREP 5		/* 32 - Preposition */
#define  WRD_DIR 6		/* 64 - Direction */
#define  WRD_IT 7		/* 128 - It */
#define  WRD_NOISE 8		/* 256 - Noise word */
#define  WRD_NOUN 9		/* 512 - Noun */
#define  WRD_ACT 10		/* 1024 - Actor */
#define  WRD_THEM 11		/* 2048 - Them */
#define  WRD_VRB 12		/* 4096 - Verb */
#define  WRD_CLASSES 13



/* Syntax element classifications */
#define EOS (-2)		/* End Of Syntax */

/* Parameter Classes */
typedef enum ClaKind {		/* NOTE! These must have the same order as */
  CLA_OBJ = 1,			/* the name classes in NAM.H */
  CLA_CNT = (int)CLA_OBJ<<1,
  CLA_ACT = (int)CLA_CNT<<1,
  CLA_NUM = (int)CLA_ACT<<1,
  CLA_STR = (int)CLA_NUM<<1,
  CLA_COBJ = (int)CLA_STR<<1,
  CLA_CACT = (int)CLA_COBJ<<1
} ClaKind;
  

/* Verb Qualifiers */
typedef enum QualClass {
  Q_DEFAULT,
  Q_AFTER,
  Q_BEFORE,
  Q_ONLY
} QualClass;


/* The AMACHINE Operations */
typedef enum OpClass {
  C_CONST,
  C_STMOP,
  C_CURVAR
} OpClass;

typedef enum InstClass {
  I_PRINT,			/* Print a string from the text file */
  I_QUIT,
  I_LOOK,
  I_SAVE,
  I_RESTORE,
  I_LIST,			/* List contents of a container */
  I_EMPTY,
  I_SCORE,
  I_VISITS,
  I_SCHEDULE,
  I_CANCEL,
  I_LOCATE,
  I_MAKE,
  I_SET,			/* Set a numeric attribute to the */
				/* value on top of stack */
  I_STRSET,			/* Set a string valued attribute to a */
				/* copy of the string on top of stack, */
				/* deallocate current contents first */
  I_GETSTR,			/* Get a string contents from text */
				/* file, create a copy and push it */
				/* on top of stack */
  I_INCR,			/* Increment an attribute */
  I_DECR,			/* Decrement a numeric attribute */
  I_USE,
  I_IN,
  I_DESCRIBE,
  I_SAY,
  I_SAYINT,
  I_SAYSTR,
  I_IF,
  I_ELSE,
  I_ENDIF,
  I_ATTRIBUTE,
  I_STRATTR,			/* Push a copy of a string attribute */
  I_HERE,
  I_NEAR,
  I_WHERE,
  I_AND,
  I_OR,
  I_NE,
  I_EQ,
  I_STREQ,			/* String compare */
  I_STREXACT,
  I_LE,
  I_GE,
  I_LT,
  I_GT,
  I_PLUS,
  I_MINUS,
  I_NOT,
  I_UMINUS,
  I_RND,
  I_SUM,			/* SUM-aggregate */
  I_MAX,			/* MAX-aggregate */
  I_COUNT,			/* COUNT-aggregate */
  I_RETURN,
  I_SYSTEM
} InstClass;


typedef enum VarClass {
  V_PARAM,
  V_CURLOC,
  V_CURACT,
  V_CURVRB,
  V_SCORE
} VarClass;


#define I_CLASS(x) ((x)>>28)
#define I_OP(x)    ((x&0x8000000)?(x)|0x0f0000000:(x)&0x0fffffff)


typedef struct AcdHdr {
/* Important info */
  Aword vers;			/* 01 - Version of compiler */
  Aword size;			/* 02 - Size of ACD-file in Awords */
/* Options */
  Abool pack;			/* 03 - Is the text packed ? */
  Aword paglen;			/* 04 - Length of a page */
  Aword pagwidth;		/* 05 - and width */
  Aword debug;			/* 06 - Option debug */
/* Data structures */
  Aaddr dict;			/* 07 - Dictionary */
  Aaddr oatrs;			/* 08 - Object default attributes */
  Aaddr latrs;			/* 09 - Location default attributes */
  Aaddr aatrs;			/* 0a - Actor default attributes */
  Aaddr acts;			/* 0b - Actor table */
  Aaddr objs;			/* 0c - Object table */
  Aaddr locs;			/* 0d - Location table */
  Aaddr stxs;			/* 0e - Syntax table */
  Aaddr vrbs;			/* 0f - Verb table */
  Aaddr evts;			/* 10 - Event table */
  Aaddr cnts;			/* 11 - Container table */
  Aaddr ruls;			/* 12 - Rule table */
  Aaddr init;			/* 13 - String init table */
  Aaddr start;			/* 14 - Start code */
  Aword msgs;			/* 15 - Messages table */
/* Miscellaneous */
  Aword objmin, objmax;		/* 16 - Interval for object codes */
  Aword actmin, actmax;		/* 18 - Interval for actor codes */
  Aword cntmin, cntmax;		/* 1a - Interval for container codes */
  Aword locmin, locmax;		/* 1c - Interval for location codes */
  Aword dirmin, dirmax;		/* 1e - Interval for direction codes */
  Aword evtmin, evtmax;		/* 20 - Interval for event codes */
  Aword rulmin, rulmax;		/* 22 - Interval for rule codes */
  Aword maxscore;		/* 24 - Maximum score */
  Aaddr scores;			/* 25 - Score table */
  Aaddr freq;			/* 26 - Address to Char freq's for coding */
  Aword acdcrc;			/* 27 - Checksum for acd code (excl. hdr) */
  Aword txtcrc;			/* 28 - Checksum for text data file */
} AcdHdr;

#endif
