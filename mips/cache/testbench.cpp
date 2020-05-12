#include "testbench.h"
#include "device.h"

/**
 * SETUP
 */

static ReferenceCache *ref = new ReferenceCache;
static Device *dev = new Device(ref);

SETUP_TESTLIST
PRETEST_HOOK = [] {
    dev->reset();
};
POSTTEST_HOOK = [] {
    // nothing.
};

/**
 * TESTBENCH
 */

WITH {
    dev->nop();
} AS("nop")

WITH {
    dev->read(0);
} AS("read M[0]")

WITH {
    dev->read(0xdead);
} AS("read M[0xdead]")

WITH {
    dev->read(0xdead);
    dev->read(0xdead);
} AS("double read M[0xabcd]")

WITH {
    dev->read(0x30);
    dev->read(0x34);
    dev->read(0x38);
    dev->read(0x3c);
} AS("sequential read M[0x30]")

WITH {

} AS("read multiline")

WITH TRACE {
    dev->read(ADDR(0x7a, 0, 0));
    dev->read(ADDR(0x99, 1, 1));
    dev->read(ADDR(0xcc, 2, 0));
    dev->read(ADDR(0x32, 3, 0));
} AS("read multiset")
