#ifndef _ACODE_H_
#define _ACODE_H_

/* Basic types */
typedef unsigned long Aword;		/* Type for an ACODE word */
typedef unsigned long Aaddr;		/* Type for an ACODE address */
typedef unsigned long Abool;		/* Type for an ACODE Boolean value */
typedef signed long Aint;		/* Type for an ACODE Integer value */

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


/* AMACHINE Word Classes, bit positions */
typedef int WrdKind;
#define  SYNONYM_WORD 0		/* 1 - Synonym */
#define  ADJECTIVE_WORD 1		/* 2 - Adjective */
#define  ALL_WORD 2		/* 4 - All */
#define  BUT_WORD 3		/* 8 - But */
#define  CONJUNCTION_WORD 4		/* 16 - Conjunction */
#define  PREPOSITION_WORD 5		/* 32 - Preposition */
#define  DIRECTION_WORD 6		/* 64 - Direction */
#define  IT_WORD 7		/* 128 - It */
#define  NOISE_WORD 8		/* 256 - Noise word */
#define  NOUN_WORD 9		/* 512 - Noun */
#define  ACTOR_WORD 10		/* 1024 - Actor */
#define  THEM_WORD 11		/* 2048 - Them */
#define  VERB_WORD 12		/* 4096 - Verb */
#define  WRD_CLASSES 13



/* Syntax element classifications */
#define EOS (-2)		/* End Of Syntax */

/* Syntax element flag bits */
#define MULTIPLEBIT 0x1
#define OMNIBIT 0x2


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

#define INSTRUCTION(op) ((((Aword)C_STMOP)<<28)|((Aword)op))
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
  I_STOP,
  I_IN,
  I_DESCRIBE,
  I_SAY,
  I_SAYINT,
  I_SAYSTR,
  I_IF,
  I_ELSE,
  I_ENDIF,
  I_ATTRIBUTE,			/* Push the value of an attribute */
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
  I_MULT,
  I_DIV,
  I_NOT,
  I_UMINUS,
  I_RND,
  I_AGRSTART,			/* Aggregate looping instructions */
  I_AGRCHECK,
  I_ENDAGR,
  I_SUM,			/* Aggregates */
  I_MAX,
  I_MIN,
  I_COUNT,			/* COUNT aggregate & limit meta-attribute */
  I_RETURN,
  I_SYSTEM,
  I_RESTART,
  I_BTW,
  I_CONTAINS,
  I_DUP,
  I_DEPEND,
  I_DEPCASE,
  I_DEPEXEC,
  I_DEPELSE,
  I_ENDDEP,
  I_ISA,
  I_FRAME,
  I_SETLOCAL,
  I_GETLOCAL,
  I_ENDFRAME,
  I_EACH,
  I_NEXTEACH,
  I_ENDEACH,
  I_SHOW
} InstClass;


typedef enum VarClass {
  V_PARAM,
  V_CURLOC,
  V_CURACT,
  V_CURVRB,
  V_SCORE,
  V_CURRENT_INSTANCE
} VarClass;


#define I_CLASS(x) ((x)>>28)
#define I_OP(x)    ((x&0x8000000)?(x)|0x0f0000000:(x)&0x0fffffff)


/* AMACHINE Table entry types */

#define ACDsizeOf(x) (sizeof(x)/sizeof(Aword))

typedef struct ClassEntry {	/* CLASS TABLE */
  Aword code;			/* Own code */
  Aaddr idAddress;		/* Address to identifier string */
  Aword parent;			/* Code for the parent class, 0 if none */
  Aaddr checks;			/* Address of description checks */
  Aaddr description;		/* Address of description code */
  Aaddr verbs;			/* Address of verb table */
} ClassEntry;

typedef struct InstanceEntry {	/* INSTANCE TABLE */
  Aword code;			/* Own code */
  Aaddr idAddress;		/* Address to identifier string */
  Aword parent;			/* Code for the parent class, 0 if none */
  Aword location;		/* DYNAMIC - Code for current location */
  Aword container;		/* Code for a possible container property */
  Aaddr attributes;		/* DYNAMIC CONTENT - Address of attribute list */
  Aaddr checks;			/* Address of description checks */
  Aaddr description;		/* Address of description code */
  Aaddr mentioned;		/* Address to short description code */
  Aaddr article;		/* Address of article code */
  Aaddr exits;			/* Address of exit list */
  Aaddr verbs;			/* Address of local verb list */
} InstanceEntry;

typedef struct AttributeEntry {	/* ATTRIBUTE LIST */
  Aint code;			/* Its code */
  Aword value;			/* Its value */
  Aaddr stringAddress;		/* Address to the name */
} AttributeEntry;

typedef struct ExitEntry {	/* EXIT TABLE structure */
  Abool duplicate;		/* Is this a duplicate (exit e,v) used
				   in reverse/convert process */
  Aword code;			/* Direction code */
  Aaddr checks;			/* Address of check table */
  Aaddr action;			/* Address of action code */
  Aword target;			/* Id for the target location */
} ExitEntry;


#define RESTRICTIONCLASS_CONTAINER (-2)
#define RESTRICTIONCLASS_INTEGER (-3)
#define RESTRICTIONCLASS_STRING (-4)

typedef struct RestrictionEntry { /* PARAMETER RESTRICTION TABLE */
  Aword parameter;		/* Parameter number */
  Aword class;			/* Parameter class code */
  Aaddr stms;			/* Exception statements */
} RestrictionEntry;

typedef struct ContainerEntry {	/* CONTAINER TABLE */
  Aword owner;			/* Owner instance index */
  Aaddr limits;			/* Address to limit check code */
  Aaddr header;			/* Address to header code */
  Aaddr empty;			/* Address to code for header when empty */
  Aaddr extractChecks;		/* Address to check before extracting */
  Aaddr extractStatements;	/* Address to execute when extracting */
} ContainerEntry;


typedef struct ElementEntry {	/* SYNTAX ELEMENT TABLES */
  Aword code;			/* Code for this element, 0 -> parameter */
  Aword flags;			/* Flags for multiple/omni (if parameter) */
  Aaddr next;			/* Address to next element table ... */
				/* ... or restrictions if code == EOS */
} ElementEntry;

typedef struct ParseEntry {	/* PARSE TABLE */
  Aword code;			/* Code for verb word */
  Aaddr elms;			/* Address to element tables */
} ParseEntry;

typedef struct SyntaxEntry {	/* SYNTAX MAPPING TABLE */
  Aint syntaxNumber;
  Aaddr parameterMapping;
  Aint verbCode;
} SyntaxEntry;

typedef struct EventEntry {	/* EVENT TABLE */
  Aaddr stringAddress;		/* Address to name string */
  Aaddr code;		
} EventEntry;

typedef struct ScriptEntry {	/* SCRIPT TABLE */
  Aaddr stringAddress;		/* Address to name string */
  Aword code;			/* Script number */
  Aaddr description;		/* Optional description statements */
  Aaddr steps;			/* Address to steps */
} ScriptEntry;

typedef struct AltEntry {	/* VERB ALTERNATIVE TABLE */
  Aword qual;			/* Verb execution qualifier */
  Aword param;			/* Parameter number */
  Aaddr checks;			/* Address of the check table */
  Aaddr action;			/* Address of the action code */
} AltEntry;



/* AMACHINE Header */

typedef struct AcdHdr {
/* Important info */
  char tag[4];			/* "ALAN" */
  char vers[4];			/* Version of compiler */
  Aword size;			/* Size of ACD-file in Awords */
/* Options */
  Abool pack;			/* Is the text packed ? */
  Aword stringOffset;		/* Offset to string data in game file */
  Aword pageLength;			/* Length of a page */
  Aword pageWidth;		/* and width */
  Aword debug;			/* Option debug */
/* Data structures */
  Aaddr classTableAddress;	/* Class table */
  Aword classMax;		/* Number of classes */
  Aword entityClassId;
  Aword thingClassId;
  Aword objectClassId;
  Aword locationClassId;
  Aword actorClassId;
  Aword literalClassId;
  Aword integerClassId;
  Aword stringClassId;
  Aaddr instanceTableAddress;	/* Instance table */
  Aword instanceMax;		/* Highest number of an instance */
  Aword theHero;		/* The hero instance code (id) */
  Aaddr containerTableAddress;
  Aword containerMax;
  Aaddr scriptTableAddress;
  Aword scriptMax;
  Aaddr eventTableAddress;
  Aword eventMax;
  Aaddr parseTableAddress;
  Aaddr syntaxTableAddress;
  Aword syntaxMax;
  Aaddr dictionary;
  Aaddr verbTableAddress;
  Aaddr ruleTableAddress;
  Aaddr messageTableAddress;
/* Miscellaneous */
  Aint attributesAreaSize;	/* Size of attribute data area in Awords */
  Aaddr init;			/* String init table */
  Aaddr start;			/* Address to Start code */
  Aword maxscore;		/* Maximum score */
  Aaddr scores;			/* Score table */
  Aint scoresMax;		/* Max index into scores table */
  Aaddr freq;			/* Address to Char freq's for coding */
  Aword acdcrc;			/* Checksum for acd code (excl. hdr) */
  Aword txtcrc;			/* Checksum for text data file */
} AcdHdr;

/* Error message numbers */
typedef enum MsgKind {
  M_WHAT,
  M_WHAT_ALL,
  M_WHAT_IT,
  M_WHAT_THEM,
  M_MULTIPLE,
  M_WANT,
  M_NOUN,
  M_AFTER_BUT,
  M_BUT_ALL,
  M_NOT_MUCH,
  M_WHICH_ONE,
  M_NO_SUCH,
  M_NO_WAY,
  M_CANT0,
  M_CANT,
  M_SEEOBJ1,
  M_SEEOBJ2,
  M_SEEOBJ3,
  M_SEEOBJ4,
  M_SEEACT,
  M_CONTAINS0,
  M_CONTAINS,
  M_CARRIES,
  M_CONTAINSCOMMA,
  M_CONTAINSAND,
  M_CONTAINSEND,
  M_EMPTY,
  M_EMPTYHANDED,
  M_SCORE1,
  M_SCORE2,
  M_UNKNOWN_WORD,
  M_MORE,
  M_AGAIN,
  M_SAVEWHERE,
  M_SAVEOVERWRITE,
  M_SAVEFAILED,
  M_SAVEMISSING,
  M_NOTASAVEFILE,
  M_SAVEVERS,
  M_SAVENAME,
  M_RESTOREFROM,
  M_REALLY,			/* CHANGED: v2.7 from M_RESTART */
  M_QUITACTION,			/* INTRODUCED: v2.7, so M_ARTICLE moved */
  M_ARTICLE,			/* INTRODUCED: v2.6 but replaced the M_NOMSG*/
  MSGMAX
} MsgKind;

#define M_ARTICLE26 M_QUITACTION
#define M_MSGMAX26 M_ARTICLE

#endif
