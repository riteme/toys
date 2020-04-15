#pragma once

#include <cstdio>
#include <cassert>
#include <cstring>

#include <vector>

#include "VDatapath.h"

typedef uint32_t u32;
typedef const char *cstr;

extern std::vector<class ITest*> *p_test_list;

const char *r[] = {
    "$0", "at", "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3",
    "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3",
    "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1",
    "gp", "sp", "fp", "ra"
};

#define $0 0
#define $at 1
#define $v0 2
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $k0 26
#define $k1 27
#define $gp 28
#define $sp 29
#define $fp 30
#define $ra 31

class Device {
public:
    std::vector<u32> imem;
    std::vector<u32> dmem;

    Device() {
        _dp = new VDatapath;
        imem.resize(8);
        dmem.resize(8);

        reset();
    }

    ~Device() {
        delete _dp;
    }

    void reset() {
        _dp->reset = 1;
        _dp->clk = 0;
        tick();
        tick();
        _dp->reset = 0;
        _dp->eval();

        memset(_dp_cpy, 0, sizeof(_dp_cpy));
    }

    void tick() {
        _dp->clk ^= 1;
        _dp->eval();

        if (_dp->mem_en) {
            dwrite(_dp->addr, _dp->mem_data);
        }

        _dp->imem_out = iread(_dp->iaddr);
        _dp->mem_out = dread(_dp->addr);
        _dp->eval();
    }

    void run(int n = 1) {
        assert(_dp->clk == 0);
        while (n--) {
            for (int i = 0; i < 2; i++)
                tick();

            for (int i = 0; i < 32; i++)
            if (_dp_cpy[i] != _dp_v[i])
                printf("R[%s]: %u → %u\n", r[i], _dp_cpy[i], _dp_v[i]);
            memcpy(_dp_cpy, _dp_v, sizeof(_dp_cpy));
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
        printf("%u ← imem[%d]\n", data, addr);
        return data;
    }

    void iwrite(int addr, u32 data) {
        _check_addr(addr, imem.size(), "imem/write");
        imem[addr] = data;
        printf("imem[addr] ← %u\n", addr, data);
    }

    u32 dread(int addr) {
        _check_addr(addr, dmem.size(), "dmem/read");
        u32 data = dmem[addr >> 2];
        printf("%u ← dmem[%d]\n", data, addr);
        return data;
    }

    void dwrite(int addr, u32 data) {
        _check_addr(addr, dmem.size(), "dmem/write");
        dmem[addr] = data;
        printf("dmem[addr] ← %u\n", addr, data);
    }

    u32 &operator[](int i) {
        return _dp_v[i];
    }

private:
    VDatapath *_dp;
    u32 *_dp_v = _dp->Datapath__DOT__register__DOT__v;
    u32 _dp_cpy[32];

    void _check_addr(int addr, int size, const char *category) {
        if (addr < 0 || addr >= size) {
            fprintf(stderr, "ERR! %s: out of range: addr = %d\n", category, addr);
            exit(-1);
        }
        if (addr & 3) {
            fprintf(stderr, "ERR! %s: addr not aligned: addr = %d\n", category, addr);
            exit(-1);
        }
    }
};

class ITest {
public:
    ITest(cstr _name) : name(_name) {
        p_test_list->push_back(this);
    }

    void run() {
        _run();
        printf("[OK] %s\n", name);
    }

private:
    cstr name;
    virtual void _run() = 0;
};

#define BEGIN \
    static class Test##__LINE__ : public ITest { \
        using ITest::ITest; \
        void _run() {

#define END(name) \
        } \
    } test##__LINE__(name);
