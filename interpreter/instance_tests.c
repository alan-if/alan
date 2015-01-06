#include <cgreen/cgreen.h>
#include <cgreen/mocks.h>

#include "instance.h"

#include "memory.h"


/* Mocked modules */
#include "literal.mock"
#include "class.mock"
#include "attribute.mock"
#include "syserr.mock"
#include "set.mock"
#include "compatibility.mock"
#include "current.mock"
#include "location.mock"
#include "inter.mock"
#include "output.mock"
#include "dictionary.mock"
#include "msg.mock"
#include "container.mock"
#include "checkentry.mock"
#include "actor.mock"
#include "debug.mock"
#include "params.mock"



#define MAX_NO_OF_INSTANCES

static Aword *allocate_memory() {
    return (Aword *)allocate(sizeof(ACodeHeader)+(MAX_NO_OF_INSTANCES+1)*sizeof(InstanceEntry));
}


Describe(Instance);

BeforeEach(Instance) {
    memory = allocate_memory();
}

AfterEach(Instance) {
    free(memory);
}


Ensure(Instance, can_handle_legacy_directly_containment) {
    fail_test("");
}
