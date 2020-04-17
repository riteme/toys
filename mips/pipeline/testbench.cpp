#include <algorithm>

#include "verilated.h"
#include "testbench.h"

static Device dev;
static std::vector<ITest*> test_list;

std::vector<ITest*> *p_test_list = &test_list;
ITest *current_test = nullptr;


//
// TESTS
//

using namespace std;

BEGIN(1)

END(1, "empty")

BEGIN(2)
    dev.resize_imem(9);
    dev.reset();

    // dev.enable_print();
    for (int i = 0; i < 4; i++) {
        dev.run();

        assert(dev.pc0() == 8 * i);
        assert(dev.instr0() == 0);
    }
END(2, "double fetch")

BEGIN(5)
    dev.resize_imem(8);

    auto add = RTYPE($v0, $s0, $s1, 0, ADD);
    dev.imem[0] = dev.imem[2] = add;
    dev.reset();

    // dev.enable_print();
    dev.run();
    assert(dev.pc0() == 0);
    assert(dev.instr0() == add);

    dev.run();
    assert(dev.pc0() == 8);
    assert(dev.instr0() == add);
END(5, "nop1")

BEGIN(4)
    dev.resize_imem(8);

    auto add = RTYPE($v0, $s0, $s1, 0, ADD);
    dev.imem[0] = dev.imem[1] = add;
    dev.reset();

    // dev.enable_print();
    dev.run();
    assert(dev.pc0() == 0);
    assert(dev.instr0() == add);

    dev.run();
    assert(dev.pc0() == 4);
    assert(dev.instr0() == add);
END(4, "nop2")

BEGIN(3)
    dev.resize_imem(32);

    auto add = RTYPE($v0, $s0, $s1, 0, ADD);
    for (int i = 0; i < 8; i++)
        dev.imem[i] = add;

    dev.reset();

    // dev.enable_print();
    for (int i = 0; i < 8; i++) {
        dev.run();
        assert(dev.pc0() == i * 4);
        assert(dev.instr0() == add);
        // printf("can_swap: %d\n", dev.dp->Datapath__DOT__frontend__DOT__select__DOT__can_swap);
    }
END(3, "normal fetch")


//
// MAIN
//

typedef void handler_t(int);

void unix_error(const char *msg) {
    fprintf(stdout, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

void abort_handler(int) {
    if (current_test)
        printf("\033[31mERR!\033[0m abort in \"%s\"\n", current_test->name);
}

void exit_handler() {
    abort_handler(0);
}

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);

    Signal(SIGABRT, abort_handler);
    atexit(exit_handler);

    for (auto t : test_list)
        t->run();

    return 0;
}
