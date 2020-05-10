#pragma once

#include <vector>
#include <numeric>

#include "utils.h"
#include "top.h"

class Device {
public:
    std::vector<u32> mem;
    VCacheTop *top;

    Device() {
        top = new VCacheTop;

        mem.resize(MEMSIZE);

        reset();
    }

    ~Device() {
        delete top;
    }

    void reset() {
        bool old_en = _enable_print;
        enable_print(false);

        top->reset = 1;
        top->clk = 0;
        top->en = 1;
        top->eval();
        load_memory();

        tick();

        top->reset = 0;
        top->eval();
        enable_print(old_en);

        load_memory();
    }

    void init() {
        std::iota(mem.begin(), mem.end(), 0);
    }

    void resize(int n) {
        mem.resize(n);
        init();
    }

    void load_memory() {
        if (top->mwrite_en)
            _write(top->maddr, top->mdata);

        top->mout = _read(top->maddr);
        top->eval();
    }

    void tick() {
        top->clk ^= 1;
        top->eval();

        load_memory();
    }

    void run() {
        assert(top->clk == 1);
        top->eval();
        load_memory();

        for (int cnt = 0; cnt < CLK_LIMIT; cnt++) {
            tick();

            assert(top->clk == 0);
            bool ok = !top->ready || top->hit;

            tick();
            print_all();
            _print("\n");

            if (ok) {
                _print("run: count=%d\n", cnt + 1);
                return;
            }
        }

        _error("operation runs for too long! [LIMIT = %d]", CLK_LIMIT);
        abort();
    }

    void nop() {
        top->ready = 0;
        top->is_write = randi(0, 1);
        top->addr = randi(0, mem.size() - 1);
        top->data = DEFAULT_DATA;
        run();
    }

    u32 read(int addr) {
        top->ready = 1;
        top->is_write = 0;
        top->addr = addr;
        top->data = DEFAULT_DATA;
        run();
        return out();
    }

    void write(int addr, u32 data) {
        top->ready = 1;
        top->is_write = 1;
        top->addr = addr;
        top->data = data;
        run();
        assert(out() == data);
    }

    bool hit() const {
        return top->hit;
    }

    u32 out() const {
        return top->out;
    }

    void print_all() {
        printf("cache.enabled=%d\n", top->enabled);
        printf("cache.now=%d, cache.count=%d\n", top->now, top->count);
        printf("cache.state=%d\n", top->state);
        printf("cache.saved_tag=%x\n", top->saved_tag);

        for (int i = 0; i < SET_NUM; i++) {
            printf("### set %d\n", i);
            printf("req_tag[%d]=%d, dirty=%d\n", i, top->req_tag[i], top->req_dirty[i]);
            for (int j = 0; j < CACHE_E; j++) {
                printf("tag=%x, dirty=%d, tick=%d\n",
                    top->tag[i][j], top->dirty[i][j], top->tick[i][j]);
                for (int k = 0; k < LINE_SIZE; k += 8) {
                    printf("\t");
                    for (int l = 0; l < 8; l++)
                        printf(l == 3 ? "%08x  " : "%08x " , top->line[i][j][k + l]);
                    puts("");
                }
            }
        }
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

private:
    bool _enable_print = false;

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

    u32 _read(int addr) {
        if (addr > 4 * (mem.size() - 1))
            return 0xcccccccc;

        // _check_addr(addr, mem.size(), "mem/read");
        u32 data = mem[addr >> 2];
        _print("    %08x @M[%08x]\n", data, addr);
        return data;
    }

    void _write(int addr, u32 data) {
        if (!top->clk)
            return;

        _check_addr(addr, mem.size(), "mem/write");
        mem[addr >> 2] = data;
        _print("    M[%08x] ← %08x\n", addr, data);
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