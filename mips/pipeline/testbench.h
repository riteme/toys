#pragma once

#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdarg>

#include <vector>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "utils.h"

#include "VDatapath.h"

class Device {
public:
    std::vector<u32> imem;
    std::vector<u32> dmem;
    VDatapath *dp;

    Device() {
        dp = new VDatapath;
        _dp_v = dp->Datapath__DOT__regfile__DOT__v;

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
        bool old_en = _enable_print;
        enable_print(false);
        dp->reset = 1;
        dp->clk = 0;
        tick();
        tick();
        dp->reset = 0;
        dp->eval();
        enable_print(old_en);

        load_memory();
        dp->eval();

        memset(_dp_cpy, 0, sizeof(_dp_cpy));
    }

    void load_memory() {
        if (dp->mem_en)
            dwrite(dp->addr, dp->mem_data);

        // _print("cpc = %d\n", dp->Datapath->frontend->__PVT__cpc);
        dp->imem_out = iread(dp->iaddr);
        dp->mem_out = dread(dp->addr);
    }

    void tick() {
        dp->clk ^= 1;
        dp->eval();

        if (dp->clk) {
            _print(
                "# clk ← %d %s\n",
                dp->clk,
                dp->Datapath__DOT__stall ? "[stalled]" : ""
            );

            checkout_register();
            _print(
                "  > new: \"%s\" [pc = %d]\n",
                translate(instr0()).c_str(), pc0()
            );
        }

        load_memory();
        dp->eval();
    }

    void checkout_register() {
        for (int i = 0; i < 32; i++)
        if (_dp_cpy[i] != _dp_v[i])
            _print("    R[%s]: %d → %d\n", r[i], _dp_cpy[i], _dp_v[i]);
        memcpy(_dp_cpy, _dp_v, sizeof(_dp_cpy));
    }

    void run(int n = 1) {
        assert(dp->clk == 0);
        while (n--) {
            for (int i = 0; i < 2; i++)
                tick();

            print_forwarding();
            print_frontend();
            _print("\n");
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
        _print("    \"%s\" @imem[%d]\n", translate(data).c_str(), addr);

        last_imem_read = data;
        return data;
    }

    void iwrite(int addr, u32 data) {
        if (!dp->clk)
            return;

        _check_addr(addr, imem.size(), "imem/write");
        imem[addr >> 2] = data;
        _print("    imem[%d] ← %d\n", addr, data);
    }

    u32 dread(int addr) {
        if (addr > 4 * (dmem.size() - 1))
            return 0xcccccccc;

        // _check_addr(addr, dmem.size(), "dmem/read");
        u32 data = dmem[addr >> 2];
        _print("    %d @dmem[%d]\n", data, addr);
        return data;
    }

    void dwrite(int addr, u32 data) {
        if (!dp->clk)
            return;

        _check_addr(addr, dmem.size(), "dmem/write");
        dmem[addr >> 2] = data;
        _print("    dmem[%d] ← %d\n", addr, data);
    }

    void append(u32 instr) {
        imem.push_back(instr);
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

    void print_forwarding() {
        _print("    signal2 = %d\n", dp->Datapath__DOT__signal2);
        _print("    mem_out = %d\n", dp->mem_out);
        _print("    v1 = %d\n", dp->Datapath__DOT__v1);
        _print("    v2 = %d\n", dp->Datapath__DOT__v2);
    }

    void print_frontend() {
        // _print(
        //     "  > no_deps = %d\n",
        //     dp->Datapath__DOT__frontend__DOT__select__DOT__no_deps
        // );
        // _print(
        //     "  > allow_swap = %d\n",
        //     dp->Datapath__DOT__frontend__DOT__select__DOT__allow_swap
        // );
        // _print(
        //     "  > jmp_after_lw = %d\n",
        //     dp->Datapath__DOT__frontend__DOT__select__DOT__jmp_after_lw
        // );
        // _print(
        //     "  > buf: \"%s\" [pc = %d]\n",
        //     translate(dp->Datapath__DOT__frontend__DOT__bf).c_str(),
        //     dp->Datapath__DOT__frontend__DOT__bpc
        // );
        _print(
            "  > cur: \"%s\" [pc = %d]\n",
            translate(last_imem_read).c_str(),
            dp->Datapath__DOT__frontend__DOT__cpc
        );
        // _print(
        //     "  > emit \"%s\" [pc = %d]\n",
        //     translate(dp->Datapath__DOT__frontend__DOT__select__DOT__emit & 0xffffffff).c_str(),
        //     dp->Datapath__DOT__frontend__DOT__select__DOT__emit >> 32
        // );
        // _print(
        //     "  > pred \"%s\" [pc = %d]\n",
        //     translate(dp->Datapath__DOT__frontend__DOT__select__DOT__pred & 0xffffffff).c_str(),
        //     dp->Datapath__DOT__frontend__DOT__select__DOT__pred >> 32
        // );
        // _print("  > result = %d\n", dp->Datapath__DOT__frontend__DOT__result);
        // _print("  > prev = %s\n", translate(dp->Datapath__DOT__instr0).c_str());
        _print("  > miss = %d\n", dp->Datapath__DOT__frontend__DOT__miss);
        // _print("  > pred pc = %d\n", dp->Datapath__DOT__frontend__DOT__select__DOT__pred);
    }

    u32 &operator[](int i) {
        return _dp_v[i];
    }

private:
    bool _enable_print = false;
    u32 *_dp_v;
    u32 _dp_cpy[32];
    u32 last_imem_read;

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
