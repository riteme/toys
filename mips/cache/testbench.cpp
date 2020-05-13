#include "testbench.h"
#include "device.h"

constexpr int TEST_LOAD = 500000;

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
    dev->check_mem();
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
    dev->read(ADDR(0x7a, 0, 0));
    dev->read(ADDR(0x99, 0, 0));
    dev->read(ADDR(0xcc, 0, 0));
    dev->read(ADDR(0x32, 0, 0));
    dev->read(ADDR(0x11, 0, 0));
    dev->read(ADDR(0x22, 0, 0));
    dev->read(ADDR(0x33, 0, 0));
    dev->read(ADDR(0x44, 0, 0));
} AS("read multiline")

WITH {
    dev->read(ADDR(0x7a, 0, 0));
    dev->read(ADDR(0x99, 1, 1));
    dev->read(ADDR(0xcc, 2, 0));
    dev->read(ADDR(0x32, 3, 0));
    dev->read(ADDR(0x11, 0, 0));
    dev->read(ADDR(0x11, 1, 0));
    dev->read(ADDR(0x11, 2, 0));
    dev->read(ADDR(0x11, 3, 0));
} AS("read multiset")

WITH {
    dev->write(0, 0xdeadbeef);
} AS("write M[0]")

WITH {
    dev->write(0xdead, 0xdeadbeef);
} AS("write M[0xdead]")

WITH {
    dev->write(0xdead, 0xdeadbeef);
    dev->write(0xdead, 0xdeadbeef);
} AS("double write M[0xdead]")

WITH {
    dev->write(ADDR(0x7a, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x99, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0xcc, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x32, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x11, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x22, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x33, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x44, 0, 0), 0xdeadbeef);
} AS("write multiline")

WITH {
    dev->write(ADDR(0x7a, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x99, 1, 1), 0xdeadbeef);
    dev->write(ADDR(0xcc, 2, 0), 0xdeadbeef);
    dev->write(ADDR(0x32, 3, 0), 0xdeadbeef);
    dev->write(ADDR(0x11, 0, 0), 0xdeadbeef);
    dev->write(ADDR(0x11, 1, 0), 0xdeadbeef);
    dev->write(ADDR(0x11, 2, 0), 0xdeadbeef);
    dev->write(ADDR(0x11, 3, 0), 0xdeadbeef);
} AS("write multiset")

WITH {
    for (int i = 0; i <= ADDR_MAX; i++) {
        dev->read(i);
    }
} AS("all read")

WITH {
    for (int i = 0; i <= ADDR_MAX; i++) {
        dev->write(i, 0xdeadbeef);
    }
} AS("all write")

WITH {
    for (int i = 0; i < TEST_LOAD; i++) {
        dev->read(randi(0, ADDR_MAX));
    }
} AS("random read")

WITH {
    for (int i = 0; i < TEST_LOAD; i++) {
        dev->write(randi(0, ADDR_MAX), randi());
    }
} AS("random write")

WITH {
    for (int i = 0; i < TEST_LOAD; i++) {
        int op = randi(0, 1);
        u32 addr = randi(0, ADDR_MAX);
        if (op == 0)  // read
            dev->read(addr);
        else  // write
            dev->write(addr, randi());
    }
} AS("random read/write")