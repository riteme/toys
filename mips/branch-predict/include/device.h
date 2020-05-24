#pragma once

#include <set>
#include <vector>
#include <numeric>

#include "top.h"
#include "utils.h"
#include "reference_predictor.h"

using TDeviceTop = VTestTop;
using TReference = ReferencePredictor;

class Device {
public:
    TDeviceTop *top;
    TReference *ref;

    Device(TReference *_ref = nullptr) : ref(_ref) {
        top = new TDeviceTop;
        reset();
    }

    ~Device() {
        delete top;
    }

    void reset(bool _init = true) {
        enable_print(false);
        memset(&_stat, 0, sizeof(_stat));
        _idx.clear();

        top->reset = 1;
        top->clk = 0;
        top->en = 1;
        top->eval();

        tick();

        top->reset = 0;
        top->eval();

        if (ref)
            ref->reset();

        if (_init)
            init();
    }

    void init() {
        // do nothing
    }

    void tick() {
        top->clk ^= 1;
        top->eval();
    }

    /**
     * invoke updates
     */
    void invoke() {
        assert(top->clk == 1);
        tick();  // negedge
        tick();  // posedge
    }

    /**
     * no operation, i.e. ready = 0.
     */
    void nop() {
        top->cur_pc = top->cur_instr = 0;
        top->miss = randi(0, 1);
        top->last_pc = top->last_instr = 0;
        top->eval();
        print_lookup();

        invoke();
        print_update();

        _check_ref();
    }

    bool beq(bool expr, u32 id, int offset) {
        _print("#BEQ: expr = %d, id = %u, offset = %d\n",
            expr, id, offset);
    }

    bool bne(bool expr, u32 id, int offset) {
        _print("#BNE: expr = %d, id = %u, offset = %d\n",
            expr, id, offset);
    }

    void print_lookup() {
        auto ght_s = bitcast(top->ght, 5, 0);
        auto bht_s = bitcast(top->bht, 5, 0);
        auto tag_s = bitcast(top->tag, 5, 0);
        _print("GHT: %s, BHT: %s, TAG: %s\n",
            ght_s.data(), bht_s.data(), tag_s.data());
        _print("pred = %d\n", top->pred);
        _print("{gpred = %d, lpred = %d, mux = %d, fallback = %d}\n",
            top->gpred, top->lpred, top->mux, top->fallback);

        ght_s = bitcast(top->glast_index, 5, 0);
        bht_s = bitcast(top->llast_index, 5, 0);
        tag_s = bitcast(top->slast_index, 5, 0);
        _print("glast: %s, llast: %s, slast: %s\n",
            ght_s.data(), bht_s.data(), tag_s.data());
    }

    void print_update() {
        _print_pht(top->gcnt, top->gvalid, "gshare");
        _print_pht(top->lcnt, top->lvalid, "lshare");
        _print_pht(top->scnt, top->svalid, "mux");

        for (auto i : _idx) {
            auto index = bitcast(i, 5, 0);
            auto value = bitcast(top->bht_tb[i], 5, 0);
            _print("BHT@%s: %s\n", index.data(), value.data());
        }
    }

    /**
     * NOTE: print_statistics ignores _enable_print flag.
     */
    void print_statistics() {
        double ratio = static_cast<double>(_stat.miss) / _stat.count * 100;
        printf("miss rate: %.2lf%% [miss = %u, count = %u]\n",
            ratio, _stat.miss, _stat.count);
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

private:
    bool _enable_print = false;
    struct {
        u32 count;
        u32 miss;
    } _stat;
    std::set<u32> _idx;

    /**
     * hw: hardware result.
     * valid: is valid (in PHT.sv)
     * sw: software (referenece) result.
     */
    void _check_pht_entry(u32 hw, bool valid, char sw) {
        if (sw < 0)
            assert(!valid);
        else {
            assert(valid);
            assert(hw == sw);
        }
    }

    /**
     * compare top's internal memory with reference implementation.
     *
     * NOTE: `ref` variable must not be `nullptr`.
     */
    void _check_ref() {
        assert(ref != nullptr);

        assert(top->ght == ref->ght);
        for (int i = 0; i < BPB_SIZE; i++) {
            assert(top->bht_tb[i] == ref->bht[i]);
            _check_pht_entry(top->gcnt[i], top->gvalid[i], ref->gshare.tb[i]);
            _check_pht_entry(top->lcnt[i], top->lvalid[i], ref->lshare.tb[i]);
            _check_pht_entry(top->scnt[i], top->svalid[i], ref->selector.tb[i]);
        }
    }

    void _print_pht(CData *cnt, CData *valid, const char *name) {
        for (int i = 0; i < BPB_SIZE; i++)
        if (valid[i]) {
            auto index = bitcast(i, 5, 0);
            _print("%s@%s: %d\n", name, index.data(), cnt[i]);
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