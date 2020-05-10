#include <cstdio>

#include "testbench.h"
#include "device.h"

SETUP_TEST

static ReferenceCache *ref = new ReferenceCache;
static Device *dev = new Device(ref);

BEGIN(1)
    dev->reset();
    dev->init();
    // dev->enable_print();
    dev->nop();
END(1, "nop")

BEGIN(2)
    dev->reset();
    dev->init();
    dev->enable_print();
    dev->read(0);
END(2, "read")

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);
    run_test();
    return 0;
}