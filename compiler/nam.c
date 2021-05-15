/*----------------------------------------------------------------------*\

  NAM.C
  Names handling

  \*----------------------------------------------------------------------*/

#include "nam_x.h"

#include "alan.h"
#include "msg.h"
#include "stm_x.h"
#include "srcp_x.h"
#include "lst_x.h"
#include "emit.h"
#include "util.h"


/*----------------------------------------------------------------------*/
static int saveName(List *names, Id *id)
{
    int len = 0;

    if (names != NULL) {
        for (List *nameList = names->member.lst; nameList != NULL; nameList = nameList->next) {
            char *buf = allocate(strlen(nameList->member.id->string)+2);
            strcpy(buf, nameList->member.id->string);
            if (nameList->next)
                strcat(buf, " ");
            generateText(buf);
            len = len + strlen(buf);
            free(buf);
        }
    } else {
        generateText(id->string);
        len = strlen(id->string);
    }
    return len;
}


/*======================================================================*/
void analyzeNames(Properties *props) {
    Statement *stm;

    /* Create a PRINT statement for the first name */
    stm = newStatement(&nulsrcp, PRINT_STATEMENT);
    stm->fields.print.fpos = ftell(txtfil);
    stm->fields.print.len = saveName(props->names, props->id);
    props->nameStatement = newList(stm, STATEMENT_LIST);
}
