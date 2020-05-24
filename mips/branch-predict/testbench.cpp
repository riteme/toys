#include <vector>
#include <numeric>
#include <algorithm>

#include "testbench.h"
#include "device.h"

/**
 * SETUP
 */

static auto ref = new ReferencePredictor;
static auto dev = new Device(ref);

SETUP_TESTLIST
PRETEST_HOOK = [] {
    dev->reset();
};
POSTTEST_HOOK = [] {
    // do nothing
};

#define IF(expr) if (dev->lookup((expr), __LINE__, randi(1, 100)))

/**
 * TESTBENCH
 */

WITH {
    dev->nop();
} AS("nop");

WITH {
    IF (1 == 1) {}
} AS("single branch");

WITH {
    IF (1 == 1) {}
    IF (1 == 1) {}
} AS("double branches");

WITH {
    IF (1 == 1) {

    }
    IF (1 == 1) {

    }
} AS("double branches 2");

WITH {
    for (int i = 0; i < 10; i++) {
        IF (1 == 1) {}
    }
} AS("repeat branch");