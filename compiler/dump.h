#ifndef _DUMP_H_
#define _DUMP_H_
/*----------------------------------------------------------------------*\

				dump.h

			Dump Support Functions

\*----------------------------------------------------------------------*/

/* USE other definitions */

#include "sysdep.h"
#include "types.h"


/* Types: */

/* Dump kinds */
typedef enum DumpKind {
    DUMP_SYNONYM = 1L,
    DUMP_SYNTAX = (long)DUMP_SYNONYM<<1,
    DUMP_VERB = (long)DUMP_SYNTAX<<1,
    DUMP_CLASS = (long)DUMP_VERB<<1,
    DUMP_INSTANCE = (long)DUMP_CLASS<<1,
    DUMP_EVENT = (long)DUMP_INSTANCE<<1,
    DUMP_RULE = (long)DUMP_EVENT<<1,
    DUMP_ALL = (long)DUMP_RULE<<1,
    DUMP_1 = (long)DUMP_ALL<<1,
    DUMP_2 = (long)DUMP_1<<1,
    DUMP_3 = (long)DUMP_2<<1
} DumpKind;


typedef enum NodeKind {
  ADDRESS_NODE,
  ADDRESSLIST_NODE,
  ADDRESSLISTLIST_NODE,
  ALTERNATIVE_NODE,
  ATTRIBUTE_NODE,
  CHECK_NODE,
  CLASS_NODE,
  ELEMENT_NODE,
  EVENT_NODE,
  EXIT_NODE,
  EXPRESSION_NODE,
  ID_NODE,
  INSTANCE_NODE,
  LIMIT_NODE,
  RESTRICTION_NODE,
  RULE_NODE,
  SCRIPT_NODE,
  STATEMENT_NODE,
  STEP_NODE,
  SYNONYM_NODE,
  SYNTAX_NODE,
  VERB_NODE
} NodeKind;


/* Data: */

extern Bool dumpOnStdout;


/* Functions: */
#ifdef _PROTOTYPES_

extern void put(char str[]);
extern void nl(void);
extern void in(void);
extern void out(void);
extern void dumpString(char s[]);
extern void dumpAddress(void *adr);
extern void dumpInteger(int i);
extern void dumpBoolean(Bool b);

#else
extern void put();
extern void nl();
extern void in();
extern void out();
extern void dumpAddress();
extern void dumpString();
extern void dumpInteger();
extern void dumpBoolean();
#endif

#endif
