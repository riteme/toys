#pragma once

#include <cstdio>

#include <vector>
#include <functional>

#include "unistd.h"

#include "utils.h"

using TestList = std::vector<class ITestbench*>;
using PretestHook = std::function<void(void)>;
using PosttestHook = std::function<void(void)>;

extern TestList *_p_test_list;
extern class ITestbench *_current_test;
extern PretestHook _pretest_hook;
extern PosttestHook _posttest_hook;

/**
 * example:
 * SETUP_TESTLIST
 * PRETEST_HOOK = [] {
 *     dev->reset();
 * };
 * POSTTEST_HOOK = [] {
 *     // source code here.
 * };
 */
#define SETUP_TESTLIST \
    static TestList __test_list; \
    TestList *_p_test_list = &__test_list; \
    ITestbench *_current_test = nullptr;
#define PRETEST_HOOK PretestHook _pretest_hook
#define POSTTEST_HOOK PosttestHook _posttest_hook

class ITestbench {
public:
    ITestbench(cstr _name) : name(_name) {
        _p_test_list->push_back(this);
    }

    void run() {
        _current_test = this;

        _pretest_hook();
        _run();
        _posttest_hook();

        auto fmt = isatty(STDOUT_FILENO) ?
            "\033[32m[OK]\033[0m %s\n" :
            "[OK] %s\n";
        printf(fmt, name);
        _current_test = nullptr;
    }

    cstr name;

private:
    virtual void _run() = 0;
};

// unique id magic: https://stackoverflow.com/a/2419720/7434327
#define _TESTBENCH_CAT_IMPL(x, y) x##y
#define _TESTBENCH_CAT(x, y) _TESTBENCH_CAT_IMPL(x, y)
#define _TESTBENCH_UNIQUE(x) _TESTBENCH_CAT(x, __LINE__)

#define _TESTBENCH_BEGIN(id) \
    static class id : public ITestbench { \
        using ITestbench::ITestbench; \
        void _run() {

#define _TESTBENCH_END(id, name) \
    } } id(name);

/**
 * usage:
 * WITH [TRACE] {
 *     // source code here.
 * } AS("test name")
 */
#define WITH _TESTBENCH_BEGIN(_TESTBENCH_UNIQUE(__Testbench))
#define AS(name) _TESTBENCH_END(_TESTBENCH_UNIQUE(__testbench), name)
#define TRACE { dev->enable_print(); }

inline void run_tests() {
    for (auto t : *_p_test_list)
        t->run();
}