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

    Device(ReferenceCache *_ref = nullptr) : ref(_ref) {
        top = new VCacheTop;
        mem.resize(MEM_SIZE);
        reset();
    }

    ~Device() {
        delete top;
    }

    void reset(bool _init = true) {
        memset(&_stat, 0, sizeof(_stat));
        _last_alloc = 0;

        enable_print(false);

        top->reset = 1;
        top->clk = 0;
        top->en = 1;
        top->eval();
        update_memory();

        tick();

        top->reset = 0;
        top->eval();

        update_memory();

        if (ref)
            ref->reset();

        if (_init)
            init();
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
                _print("invoke: count=%d\n\n", cnt + 1);
                return cnt + 1;
            }
        }

        _fatal("operation runs for too long! [LIMIT = %d]\n", CLK_LIMIT);
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

    u32 alloc() {
        u32 addr = _last_alloc;
        assert(addr < mem.size() * 4);
        _last_alloc += 4;
        return addr;
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
        _stat.cycles += cnt;
        _stat.read_cnt++;
        if (cnt == 1)
            _stat.read_hit++;

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
        _stat.cycles += cnt;
        _stat.write_cnt++;
        if (cnt == 1)
            _stat.write_hit++;

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
        _print("hit: %d, out: %08x", hit(), out());
        _print(" [addr=%08x, data=%08x, ready=%d, write=%d]\n",
            top->addr, top->data, top->ready, top->is_write);
        _print("cache: {enabled=%d, ", top->enabled);
        _print("now=%d, count=%d, ", top->now, top->count);
        _print("state=%d}\n", top->state);

        for (int i = 0; i < SET_NUM; i++) {
            _print("### set %d, req: {", i);
            _print("tag=%x, dirty=%d, en=%d, hit=%d",
                top->req_tag[i], top->req_dirty[i],
                top->set_en[i], top->set_hit[i]);
            _print(", swap_key=%d}\n", top->swap_key[i]);

            for (int j = 0; j < CACHE_E; j++) {
                _print("> tag=%x, valid=%d, dirty=%d, tick=%d, tick_en=%d, hit=%d\n",
                    top->tag[i][j], top->valid[i][j],
                    top->dirty[i][j], top->tick[i][j],
                    top->tick_en[i][j],/*top->line_en[i][j],*/
                    top->line_hit[i][j]);

                int step = std::min(8, LINE_SIZE);
                for (int k = 0; k < LINE_SIZE; k += step) {
                    _print("\t");
                    for (int l = 0; l < step; l++) {
                        _print(
                            l == 3 ? "%08x  " : "%08x ",
                            top->line[i][j][k + l]);
                    }
                    _print("\n");
                }
            }
        }
    }

    /**
     * print_statistics ignores _enable_print flag.
     */
    void print_statistics() {
        double n_read = _stat.read_cnt;
        double n_write = _stat.write_cnt;
        double n_op = n_read + n_write;
        double read_hit = _stat.read_hit / n_read * 100;
        double write_hit = _stat.write_hit / n_write * 100;
        double hit = (_stat.read_hit + _stat.write_hit) / n_op * 100;
        printf("cycles = %u\n", _stat.cycles);
        printf("hit = %.2lf%%, read_hit = %.2lf%%, write_hit = %.2lf%%\n",
            hit, read_hit, write_hit);
        printf("mem_write = %u\n", _stat.mem_write);
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

    void check_memory() {
        _check_mem();
    }

private:
    bool _enable_print = false;
    struct {
        u32 cycles;
        u32 read_cnt, read_hit;
        u32 write_cnt, write_hit;
        u32 mem_write;  // see _write
    } _stat;
    u32 _last_alloc;

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
            _expect(
                ref->_tag[i][j], top->tag[i][j],
                "@tag[%d][%d]", i, j);
            _expect(
                ref->_dirty[i][j], bool(top->dirty[i][j]),
                "@dirty[%d][%d]", i, j);

            for (int k = 0; k < LINE_SIZE; k++) {
                _expect(
                    ref->_line[i][j][k], top->line[i][j][k],
                    "@line[%d][%d][%d]", i, j, k);
            }
        }
    }

    void _check_mem() {
        assert(ref != nullptr);
        assert(mem.size() == ref->mem.size());

        for (int i = 0; i < mem.size(); i++) {
            _expect(ref->mem[i], mem[i], "@mem[%d]", i);
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

        _stat.mem_write++;
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

        auto err_fmt = isatty(STDERR_FILENO) ?
            "\033[33mERR!\033[0m " :
            "ERR! ";
        fprintf(stderr, "%s", err_fmt);

        vfprintf(stderr, fmt, args);
        va_end(args);
    }

    template <typename ... TArgs>
    [[noreturn]]
    void _fatal(const char *fmt, const TArgs & ... args) {
        _error(fmt, args...);
        abort();
    }

    template <typename T, typename ... TArgs>
    void _expect(
        const T &expected, const T &got,
        const char *fmt, const TArgs & ... args
    ) {
        if (got != expected) {
            _error(fmt, args...);
            fprintf(stderr, ", expected: %x, got: %x\n",
                expected, got);
            abort();
        }
    }
};