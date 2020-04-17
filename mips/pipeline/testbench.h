#pragma once

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdarg>

#include <vector>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "VDatapath.h"
#include "literal.h"

class Device {
public:
    std::vector<u32> imem;
    std::vector<u32> dmem;
    VDatapath *dp;

    Device() {
        dp = new VDatapath;
        _dp_v = dp->Datapath__DOT__register__DOT__v;

        imem.resize(8);
        dmem.resize(8);

        reset();
    }

    ~Device() {
        delete dp;
    }

    int pc0() const {
        return dp->Datapath__DOT__pc0;
    }

    u32 instr0() const {
        return dp->Datapath__DOT__instr0;
    }

    void reset() {
        dp->reset = 1;
        dp->clk = 0;
        tick();
        tick();
        dp->reset = 0;
        dp->eval();
        load_memory();
        dp->eval();

        memset(_dp_cpy, 0, sizeof(_dp_cpy));
    }

    void load_memory() {
        if (dp->mem_en)
            dwrite(dp->addr, dp->mem_data);

        dp->imem_out = iread(dp->iaddr);
        dp->mem_out = dread(dp->addr);
    }

    void tick() {
        dp->clk ^= 1;
        dp->eval();

        load_memory();
        dp->eval();
    }

    void checkout_register() {
        for (int i = 0; i < 32; i++)
        if (_dp_cpy[i] != _dp_v[i])
            _print("R[%s]: %u → %u\n", r[i], _dp_cpy[i], _dp_v[i]);
        memcpy(_dp_cpy, _dp_v, sizeof(_dp_cpy));
    }

    void run(int n = 1) {
        assert(dp->clk == 0);
        while (n--) {
            for (int i = 0; i < 2; i++)
                tick();
            checkout_register();
            _print("> emit %u [pc = %d]\n", instr0(), pc0());
        }
    }

    void resize_imem(int n) {
        imem.resize(n);
        memset(&imem[0], 0, sizeof(u32) * n);
    }

    void resize_dmem(int n) {
        dmem.resize(n);
        memset(&dmem[0], 0, sizeof(u32) * n);
    }

    u32 iread(int addr) {
        _check_addr(addr, imem.size(), "imem/read");
        u32 data = imem[addr >> 2];
        _print("%u ← imem[%d]\n", data, addr);
        return data;
    }

    void iwrite(int addr, u32 data) {
        _check_addr(addr, imem.size(), "imem/write");
        imem[addr >> 2] = data;
        _print("imem[addr] ← %u\n", addr, data);
    }

    u32 dread(int addr) {
        _check_addr(addr, dmem.size(), "dmem/read");
        u32 data = dmem[addr >> 2];
        _print("%u ← dmem[%d]\n", data, addr);
        return data;
    }

    void dwrite(int addr, u32 data) {
        _check_addr(addr, dmem.size(), "dmem/write");
        dmem[addr >> 2] = data;
        _print("dmem[addr] ← %u\n", addr, data);
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

    u32 &operator[](int i) {
        return _dp_v[i];
    }

private:
    bool _enable_print = false;
    u32 *_dp_v;
    u32 _dp_cpy[32];

    void _check_addr(int addr, int size, const char *category) {
        if (addr < 0 || addr > 4 * (size - 1)) {
            _error("%s: out of range: addr = %d\n", category, addr);
            exit(-1);
        }
        if (addr & 3) {
            _error("%s: addr not aligned: addr = %d\n", category, addr);
            exit(-1);
        }
    }

    void _print(const char *fmt, ...) {
        if (!_enable_print)
            return;

        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }

    void _error(const char *fmt, ...) {
        va_list args;
        va_start(args, fmt);
        fprintf(stderr, "\033[33mERR!\033[0m ");
        vfprintf(stderr, fmt, args);
        va_end(args);
    }
};

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
