#ifndef _ATYPES_H_
#define _ATYPES_H_
/*----------------------------------------------------------------------*\

  types.h

  Header file for the Alan interpreter module.

\*----------------------------------------------------------------------*/


#include "sysdep.h"
#include "acode.h"


/* CONSTANTS */

#ifndef __mac__
#ifndef TRUE
#define TRUE (0==0)
#endif
#ifndef FALSE
#define FALSE (!TRUE)
#endif
#endif

#define LITMIN (header->locmax+1)
#define LITMAX (header->locmax+1+litCount)

#define HERO (header->theHero)
#define OBJECT (header->objectClassId)
#define LOCATION (header->locationClassId)
#define ACTOR (header->actorClassId)


#define addrTo(x) ((void *)&memory[x])

/* The word classes are represented as numbers but in the dictonary they are generated as bits */
#define isVerb(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_VRB))!=0)
#define isConj(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_CONJ))!=0)
#define isBut(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_BUT))!=0)
#define isThem(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_THEM))!=0)
#define isIt(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_IT))!=0)
#define isNoun(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_NOUN))!=0)
#define isAdj(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_ADJ))!=0)
#define isPrep(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_PREP))!=0)
#define isAll(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_ALL))!=0)
#define isDir(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_DIR))!=0)
#define isNoise(word) (word < dictsize && (dict[word].class&((Aword)1L<<WRD_NOISE))!=0)
#define isLiteral(word) (word >= dictsize)


/* TYPES */

#ifndef __mac__
typedef int Boolean;		/* Boolean values within interpreter */
#endif

/* Amachine variables */
typedef struct CurVars {
  int
    vrb,
    obj,
    loc,
    act,
    tick,
    score,
    visits;
} CurVars;

typedef struct AdminEntry {	/* Administrative data about instances */
  Abool alreadyDescribed;
  Aint visitsCount;
  Aint script;
  Aint step;
  Aint waitCount;
} AdminEntry;


/* The various tables */
typedef struct WrdEntry {	/* Dictionary */
  Aaddr wrd;			/* ACODE address to string */
  Aword class;			/* Word class */
  Aword code;
  Aaddr adjrefs;		/* Address to reference list */
  Aaddr nounrefs;		/* Address to reference list */
} WrdEntry;

typedef struct ActEntry {	/* ACTOR TABLE */
  Aword loc;			/* Location */
  Abool describe;		/* Description flag */
  Aaddr nam;			/* Address to name printing code */
  Aaddr atrs;			/* Address to attribute list */
  Aword cont;			/* Code for the container props if any */
  Aword script;			/* Which script is he using */
  Aaddr scradr;			/* Address to script table */
  Aword step;
  Aword count;
  Aaddr vrbs;
  Aaddr dscr;			/* Address of description code */
} ActEntry;

typedef struct StepEntry {	/* STEP TABLE */
  Aword after;			/* After how many ticks? */
  Aaddr exp;			/* Address to expression saying when */
  Aaddr stm;			/* Address to the actual code */
} StepEntry;

typedef struct LocEntry {	/* LOCATION TABLE */
  Aaddr nams;			/* Address of name printing code */
  Aaddr dscr;			/* Address of description code */
  Aaddr does;			/* Address of does code */
  Aword describe;		/* Description flag & counter */
  Aaddr atrs;			/* Address of attribute list */
  Aaddr exts;			/* Address of exit list */
  Aaddr vrbs;			/* Address of local verb list */
} LocEntry;

typedef struct ChkEntry {	/* CHECK TABLE */
  Aaddr exp;			/* ACODE address to expression code */
  Aaddr stms;			/* ACODE address to statement code */
} ChkEntry;

typedef struct VrbEntry {	/* VERB TABLE */
  Aword code;			/* Code for the verb */
  Aaddr alts;			/* Address to alternatives */
} VrbEntry;

typedef struct StxEntry {	/* SYNTAX TABLE */
  Aword code;			/* Code for verb word */
  Aaddr elms;			/* Address to element tables */
} StxEntry;

typedef struct AltEntry {	/* VERB ALTERNATIVE TABLE */
  Abool done;			/* Flag for patching (reverse/convert) process */
  Aword param;			/* Parameter number */
  Aword qual;			/* Verb execution qualifier */
  Aaddr checks;			/* Address of the check table */
  Aaddr action;			/* Address of the action code */
} AltEntry;


/* ClassEntry & InstanceEntry moved to acode.h */

typedef struct ObjEntry {	/* OBJECT TABLE */
  Aword loc;			/* Current location */
  Abool describe;		/* Describe flag */
  Aaddr atrs;			/* Address of attribute list */
  Aword cont;			/* Index to container properties if any */
  Aaddr vrbs;			/* Address to local verb table */
  Aaddr dscr1;			/* Address to Aword description code */
  Aaddr art;			/* Article printing code? Else use default */
  Aaddr dscr2;			/* Address to short description code */
} ObjEntry;

typedef struct LimEntry {	/* LIMIT Type */
  Aword atr;			/* Attribute that limits */
  Aword val;			/* And the limiting value */
  Aaddr stms;			/* Statements if fail */
} LimEntry;

typedef struct RulEntry {	/* RULE TABLE */
  Abool run;			/* Is rule already run? */
  Aaddr exp;			/* Address to expression code */
  Aaddr stms;			/* Address to run */
} RulEntry;

typedef struct EventQueueEntry { /* EVENT QUEUE ENTRIES */
  int time;
  int event;
  int where;
} EventQueueEntry;

typedef struct IniEntry {	/* STRING INITIALISATION TABLE */
  Aword fpos;			/* File position */
  Aword len;			/* Length */
  Aword adr;			/* Where to store the string */
} IniEntry;

typedef struct MsgEntry {	/* MESSAGE TABLE */
  Aaddr stms;			/* Address to statements*/
} MsgEntry;

typedef struct ParamEntry {	/* PARAMETER */
  Aword code;			/* Code for this parameter (0=multiple) */
  Aword firstWord;		/* Index to first word used by player */
  Aword lastWord;			/* d:o to last */
} ParamEntry;

typedef enum LiteralType {
  NO_LITERAL, NUMERIC_LITERAL, STRING_LITERAL
} LiteralType;

typedef struct LiteralEntry {	/* LITERAL */
  LiteralType type;
  Aword value;
} LiteralEntry;

#define MAXPARAMS 9
#define MAXENTITY (header->instanceMax)

#endif
