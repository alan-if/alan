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
typedef enum WrdKind {
  WRD_SYN,			/* Synonym */
  WRD_ADJ,			/* Adjective */
  WRD_ALL,			/* All */
  WRD_BUT,			/* But */
  WRD_CONJ,			/* Conjunction */
  WRD_PREP,			/* Preposition */
  WRD_DIR,			/* Direction */
  WRD_IT,			/* It */
  WRD_NOISE,			/* Noise word */
  WRD_NOUN,			/* Noun */
  WRD_ACT,			/* Actor */
  WRD_THEM,			/* Them */
  WRD_VRB,			/* Verb */
  WRD_CLASSES
} WrdKind;


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
  Abool rev;			/* 00 - Reversed bytes ? */
  Aword vers;			/* 01 - Version of compiler */
  Aword size;			/* 02 - Size of ACD-file in Awords */
/* Options */
  Abool pack;			/* 03 - Is the text packed ? */
  Aword paglen;			/* 04 - Length of a page */
  Aword pagwidth;		/* 05 - and width */
  Aword visits;			/* 06 - Number of visits between long descr */
  Aword debug;			/* 07 - Option debug */
/* Data structures */
  Aaddr dict;			/* 08 - Dictionary */
  Aaddr oatrs;			/* 09 - Object default attributes */
  Aaddr latrs;			/* 0a - Location default attributes */
  Aaddr aatrs;			/* 0b - Actor default attributes */
  Aaddr acts;			/* 0c - Actor table */
  Aaddr objs;			/* 0d - Object table */
  Aaddr locs;			/* 0e - Location table */
  Aaddr stxs;			/* 0f - Syntax table */
  Aaddr vrbs;			/* 10 - Verb table */
  Aaddr evts;			/* 11 - Event table */
  Aaddr cnts;			/* 12 - Container table */
  Aaddr ruls;			/* 13 - Rule table */
  Aaddr init;			/* 14 - String init table */
  Aaddr start;			/* 15 - Start code */
  Aword msgs;			/* 16 - Messages table */
/* Miscellaneous */
  Aword objmin, objmax;		/* 17 - Interval for object codes */
  Aword actmin, actmax;		/* 19 - Interval for actor codes */
  Aword cntmin, cntmax;		/* 1b - Interval for container codes */
  Aword locmin, locmax;		/* 1d - Interval for location codes */
  Aword dirmin, dirmax;		/* 1f - Interval for direction codes */
  Aword evtmin, evtmax;		/* 21 - Interval for event codes */
  Aword rulmin, rulmax;		/* 23 - Interval for rule codes */
  Aword maxscore;		/* 25 - Maximum score */
  Aaddr scores;			/* 26 - Score table */
  Aaddr freq;			/* 27 - Address to Char freq's for coding */
} AcdHdr;

#endif
