#include <cgreen/cgreen.h>

#include "unitList.h"
#include "lmList.mock"
#include "lmlog.mock"

#include "srcp.mock"
#include "id.mock"
#include "lst_x.h"
#include "sym.mock"
#include "wrd.mock"
#include "chk.mock"
#include "stm.mock"
#include "emit.mock"

#include "ext_x.h"


Describe(Exit);
BeforeEach(Exit) {}
AfterEach(Exit) {}


Ensure(Exit, can_be_generated) {
    Id *direction = newId(nulsrcp, "w");
    Symbol *direction_symbol = newSymbol(direction, DIRECTION_SYMBOL);
    Id *target_location = newId(nulsrcp, "aLocation");
    Symbol *location_symbol = newSymbol(target_location, INSTANCE_SYMBOL);
    ExitEntry expected_entry;

    direction->symbol = direction_symbol;
    target_location->symbol = location_symbol;

    expect(lookup, when(idString, is_equal_to_string("w")), will_return(direction_symbol));

    Exit *theExit = newExit(&nulsrcp, newList(direction, ID_LIST), target_location, NULL, NULL);
    List *exits = newList(theExit, EXIT_LIST);

    assert_that(theExit->directions->member.id->symbol, is_not_null);
    assert_that(theExit->directions->member.id->symbol->code, is_equal_to(1));

    expect(generateChecks);
    always_expect(nextEmitAddress, will_return(14));
    expect(emit0);

    expected_entry.code = direction_symbol->code;
    expected_entry.checks = 0;
    expected_entry.action = 0;
    expected_entry.target = location_symbol->code;

    expect(emitEntry,
           when(entry, is_equal_to_contents_of(&expected_entry, sizeof(expected_entry))),
           when(noOfBytes, is_equal_to(sizeof(expected_entry))));;
    expect(emit);
    generateExits(exits);
}
