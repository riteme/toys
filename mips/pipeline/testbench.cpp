#include "verilated.h"
#include "testbench.h"

static Device dev;
static std::vector<ITest*> test_list;

std::vector<ITest*> *p_test_list = &test_list;
ITest *current_test = nullptr;


BEGIN(1)

END(1, "(nothing)")

BEGIN(2)
    assert(false);
END(2, "failed")


void abort_handler(int) {
    if (current_test)
        printf("\033[31mERR!\033[0m abort in \"%s\"\n", current_test->name);
}

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);
    Signal(SIGABRT, abort_handler);

    for (auto t : test_list)
        t->run();

    return 0;
}
