/*======================================================================*\
 *
 * advTest.c
 *
 * Unit tests for ADV node in the Alan compiler
 *
 *======================================================================*/

#include "adv.c"

#include <cgreen/cgreen.h>

#include "unitList.h"

#include "wht_x.h"
#include "exp_x.h"
#include "id_x.h"


Ensure(testInitAdv) {
    Srcp srcp = {2,3,4};

    IdNode *atUnknownId = newId(srcp, "atUnknownId");
    IdNode *atInsId = newId(srcp, "atInsId");
    IdNode *atInsLocId = newId(srcp, "atInsLocId");
    IdNode *atClaId = newId(srcp, "atClaId");

    Instance *ins;
    Instance *locationIns;
    Class *cla;


    initAdventure();

    cla = newClass(&srcp, atClaId, NULL, NULL);
    ins = newInstance(&srcp, atInsId, NULL, NULL);
    locationIns = newInstance(&srcp, atInsLocId, newId(srcp, "location"), NULL);

    adv.whr = newWhere(&srcp, FALSE, WHERE_HERE, NULL);
    symbolizeAdventure();
    analyzeStartAt();		/* Can not Start At Here */
    assert_true(readEcode() == 211 && readSev() == sevERR);

    adv.whr = newWhere(&srcp, FALSE, WHERE_AT,
                       newWhatExpression(srcp, newWhatId(srcp, atUnknownId)));
    symbolizeAdventure();
    assert_true(readSev() == sevERR && readEcode() == 310);

    adv.whr->what->fields.wht.wht->id = atClaId;
    symbolizeAdventure();
    analyzeStartAt();		/* Can not Start At Id not an instance */
    assert_true(readSev() == sevERR && readEcode() == 351);
 
    adv.whr->what->fields.wht.wht->id = atInsId;
    symbolizeAdventure();
    analyzeStartAt();		/* Can not Start At Id not inheriting from location */
    assert_true(readSev() == sevERR && readEcode() == 351);

    adv.whr->what->fields.wht.wht->id = atInsLocId;
    symbolizeAdventure();
    analyzeStartAt();		/* Can not Start At Id not a instance */
    assert_true(readSev() == sevNONE && readEcode() == 0);
}


TestSuite *advTests()
{
    TestSuite *suite = create_test_suite();
    add_test(suite, testInitAdv);
    return suite;
}

