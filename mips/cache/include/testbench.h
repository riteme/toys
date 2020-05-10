#pragma once

#include <cstdio>

#include <vector>

#include "utils.h"

extern std::vector<class ITest*> *p_test_list;
extern class ITest *current_test;

class ITest {
public:
    ITest(cstr _name) : name(_name) {
        p_test_list->push_back(this);
    }

    void run() {
        current_test = this;
        _run();
        printf("\033[32m[OK]\033[0m %s\n", name);
        current_test = nullptr;
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
