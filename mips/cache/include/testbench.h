#pragma once

#include <cstdio>

#include <vector>

#include "utils.h"

using TestList = std::vector<class ITest*>;

extern TestList *__p_test_list;
extern class ITest *__current_test;

#define SETUP_TEST \
    static TestList __test_list; \
    TestList *__p_test_list = &__test_list; \
    ITest *__current_test = nullptr;

class ITest {
public:
    ITest(cstr _name) : name(_name) {
        __p_test_list->push_back(this);
    }

    void run() {
        __current_test = this;
        _run();
        printf("\033[32m[OK]\033[0m %s\n", name);
        __current_test = nullptr;
    }

    cstr name;

private:
    virtual void _run() = 0;
};

#define BEGIN(id) \
    static class Test##id : public ITest { \
        using ITest::ITest; \
        void _run() {

#define END(id, name) \
        } \
    } test##id(name);

void run_test() {
    for (auto t : *__p_test_list)
        t->run();
}