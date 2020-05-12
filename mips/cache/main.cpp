#include <cstdio>
#include <cassert>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "testbench.h"
#include "verilated.h"

// signal handling from CS:APP
using handler_t = void(int);

void hook_signal(int sig, handler_t *handler) {
    struct sigaction action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    assert(sigaction(sig, &action, NULL) >= 0);
}

void abort_handler(int) {
    auto fmt = isatty(STDERR_FILENO) ?
        "\033[31mERR!\033[0m abort in \"%s\"\n":
        "ERR! abort in \"%s\"\n";

    if (_current_test)
        fprintf(stderr, fmt, _current_test->name);
    fflush(stdout);
}

void exit_handler() {
    abort_handler(0);
}

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);

    hook_signal(SIGABRT, abort_handler);
    atexit(exit_handler);

    run_tests();
    return 0;
}