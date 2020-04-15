#include "verilated.h"
#include "testbench.h"

static Device dev;
static std::vector<ITest*> test_list;
std::vector<ITest*> *p_test_list = &test_list;

BEGIN

END("(nothing)")

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);

    for (auto t : test_list)
        t->run();

    return 0;
}