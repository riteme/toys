#pragma once

#include <vector>
#include <numeric>

#include "top.h"
#include "utils.h"
#include "reference_cache.h"

class Device {
public:
    std::vector<u32> mem;
    VCacheTop *top;
    ReferenceCache *ref;

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
        update_memory();

        tick();

        top->reset = 0;
        top->eval();
        enable_print(old_en);

        update_memory();

        if (ref)
            ref->reset();
    }

    void init() {
        std::iota(mem.begin(), mem.end(), 0);

        if (ref)
            ref->mem = mem;
    }

    void resize(int n) {
        mem.resize(n);
        init();
    }

    /**
     * interact with Cache's memory interface.
     */
    void update_memory() {
        if (top->mwrite_en)
            _write(top->maddr, top->mdata);

        top->mout = _read(top->maddr);
        top->eval();
    }

    void tick() {
        top->clk ^= 1;
        top->eval();

        update_memory();
    }

    /**
     * keeps running until cache is finished, or due to clock limit.
     */
    auto invoke() -> int {
        assert(top->clk == 1);
        top->eval();
        update_memory();

        for (int cnt = 0; cnt < CLK_LIMIT; cnt++) {
            tick();

            assert(top->clk == 0);
            bool ok = !top->ready || top->hit;

            tick();
            print_all();
            _print("\n");

            if (ok) {
                _print("invoke: count=%d\n", cnt + 1);
                return cnt + 1;
            }
        }

        _fatal("operation runs for too long! [LIMIT = %d]", CLK_LIMIT);
    }

    /**
     * no operation, i.e. ready = 0.
     */
    void nop() {
        top->ready = 0;
        top->is_write = randi(0, 1);
        top->addr = randi(0, mem.size() - 1);
        top->data = DEFAULT_DATA;
        invoke();

        if (ref)
            _check_ref();
    }

    /**
     * issue a read operation at address `addr`.
     */
    u32 read(u32 addr) {
        top->ready = 1;
        top->is_write = 0;
        top->addr = addr;
        top->data = DEFAULT_DATA;
        int cnt = invoke();

        if (ref) {
            bool hit;
            u32 data;
            std::tie(data, hit) = ref->read(addr);

            assert(top->out == data);
            _check_hit(hit, cnt);
            _check_ref();
        }

        return out();
    }

    /**
     * issue a write operation at address `addr` with `data`.
     */
    void write(u32 addr, u32 data) {
        top->ready = 1;
        top->is_write = 1;
        top->addr = addr;
        top->data = data;
        int cnt = invoke();

        if (ref) {
            bool hit = ref->write(addr, data);

            _check_hit(hit, cnt);
            _check_ref();
        }

        assert(top->out == data);
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

    void _check_hit(bool hit, int cnt) {
        if (hit && cnt != 1)
            _fatal("expected hit.");
        if (!hit && cnt == 1)
            _fatal("expected miss.");
    }

    /**
     * compare top's internal memory with reference implementation.
     *
     * NOTE: `ref` variable must not be `nullptr`.
     */
    void _check_ref() {
        assert(ref != nullptr);

        for (int i = 0; i < SET_NUM; i++)
        for (int j = 0; j < CACHE_E; j++) {
            if (top->tag[i][j] != ref->_tag[i][j])
                _fatal("@tag[%d][%d], expected: %x, got: %x\n",
                    i, j, ref->_tag[i][j], top->tag[i][j]);

            if (top->dirty[i][j] != ref->_dirty[i][j])
                _fatal("@dirty[%d][%d], expected: %d, got: %d\n",
                    i, j, ref->_dirty[i][j], top->dirty[i][j]);

            for (int k = 0; k < LINE_SIZE; k++) {
                if (top->line[i][j][k] != ref->_line[i][j][k])
                    _fatal("@line[%d][%d][%d], expected = %08x, got: %08x\n",
                        i, j, k, ref->_line[i][j][k], top->line[i][j][k]);
            }
        }
    }

    void _check_addr(u32 addr, u32 size, const char *category) {
        if (addr < 0 || addr > 4 * (std::max(1u, size) - 1))
            _fatal("%s: out of range: addr = %d\n", category, addr);
        if (addr & 3)
            _fatal("%s: addr not aligned: addr = %d\n", category, addr);
    }

    u32 _read(u32 addr) {
        if (addr > 4 * (std::max(1ul, mem.size()) - 1))
            return DEFAULT_DATA;

        // _check_addr(addr, mem.size(), "mem/read");
        u32 data = mem[addr >> 2];
        _print("  %08x @M[%08x]\n", data, addr);
        return data;
    }

    void _write(int addr, u32 data) {
        if (!top->clk)
            return;

        _check_addr(addr, mem.size(), "mem/write");
        mem[addr >> 2] = data;
        _print("  M[%08x] ← %08x\n", addr, data);
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

    template <typename ... TArgs>
    void _fatal(const char *fmt, const TArgs & ... args) {
        _error(fmt, args...);
        abort();
    }
};