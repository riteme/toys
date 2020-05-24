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

/**
 * TESTBENCH
 */

WITH TRACE {
    dev->nop();
} AS("nop")
