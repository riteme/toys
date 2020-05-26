#pragma once

#include <cstdio>

#include <set>
#include <vector>
#include <numeric>

#include "top.h"
#include "utils.h"
#include "reference_predictor.h"

#define REPORT_TO(filename) { dev->open_report(filename); }

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
        open_report(nullptr);
        reset_statistics();
        enable_reference();
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
        _print("\n# NOP\n");

        top->cur_pc = top->cur_instr = 0;
        top->miss = randu(0, 1);
        top->last_pc = top->last_instr = 0;
        top->eval();
        print_lookup();

        invoke();
        print_update();

        if (ref)
            _check_ref();
    }

    void update(bool miss) {
        _print("\n# UPDATE: miss = %d\n", miss);

        top->miss = miss;
        top->last_pc = top->cur_pc;
        top->last_instr = top->cur_instr;
        top->cur_pc = top->cur_instr = 0;

        invoke();
        print_update();

        if (ref) {
            ref->update(top->last_pc, top->last_instr, miss);
            _check_ref();
        }
    }

    bool lookup(bool expr, u32 id, int offset) {
        _print("\n# BRANCH: expr = %d, id = 0x%x, offset = %d\n",
            expr, id, offset);
        _idx.insert(id & 0x3f);
        id <<= 2;

        u32 instr = ITYPE(
            (randu(0, 1) ? BEQ : BNE),
            randu(0, 31), randu(0, 31), offset
        );
        top->cur_pc = id;
        top->cur_instr = instr;
        top->miss = randu(0, 1);
        top->last_pc = top->last_instr = 0;
        top->eval();

        print_lookup();
        if (ref) {
            bool ref_pred = ref->predict(id, instr);
            assert(top->pred == ref_pred);
        }

        invoke();
        print_update();
        if (ref)
            _check_ref();

        bool miss = expr ^ top->pred;
        update(miss);
        _update_stat(miss);

        return expr;
    }

    void print_lookup() {
        auto ght_s = bitcast(top->ght, BPB_H - 1, 0);
        auto bht_s = bitcast(top->bht, BPB_H - 1, 0);
        auto tag_s = bitcast(top->tag, BPB_T - 1, 0);
        auto htag_s = bitcast(top->htag, BPB_T - 1, 0);
        _print("GHT: %s, BHT: %s, TAG: %s, HTAG: %s\n",
            ght_s.data(), bht_s.data(), tag_s.data(), htag_s.data());
        _print("pred = %d\n", top->pred);
        _print("{gpred = %d, lpred = %d, mux = %d, fallback = %d}\n",
            top->gpred, top->lpred, top->mux, top->fallback);

        ght_s = bitcast(top->glast_index, BPB_T - 1, 0);
        bht_s = bitcast(top->llast_index, BPB_T - 1, 0);
        tag_s = bitcast(top->slast_index, BPB_T - 1, 0);
        _print("glast: %s, llast: %s, slast: %s\n",
            ght_s.data(), bht_s.data(), tag_s.data());
    }

    void print_update() {
        _print_pht(top->gcnt, top->gvalid, "gshare");
        _print_pht(top->lcnt, top->lvalid, "lshare");
        _print_pht(top->scnt, top->svalid, "mux");

        for (auto i : _idx) {
            auto index = bitcast(i, BPB_T - 1, 0);
            auto value = bitcast(top->bht_tb[i], BPB_H - 1, 0);
            _print("BHT@%s: %s\n", index.data(), value.data());
        }
    }

    void reset_statistics() {
        memset(&_stat, 0, sizeof(_stat));
    }

    /**
     * NOTE: print_statistics ignores _enable_print flag.
     */
    void print_statistics(bool simple_print = false, FILE *fd = stdout) {
        double ratio = static_cast<double>(_stat.miss) / _stat.count;
        if (simple_print)
            fprintf(fd, "%.4lf\n", ratio);
        else {
            fprintf(fd, "miss rate: %.2lf%% [miss = %u, count = %u]\n",
                ratio * 100, _stat.miss, _stat.count);

            int cnt[4] = {0};
            for (int i = 0; i < BPB_SIZE; i++) {
                if (!top->svalid[i])
                    continue;
                cnt[top->scnt[i]]++;
            }

            fprintf(fd, "cnt = {%d, %d, %d, %d}\n",
                cnt[0], cnt[1], cnt[2], cnt[3]);
        }
    }

    void enable_print(bool en = true) {
        _enable_print = en;
    }

    void open_report(const char *filename) {
        close_report();
        _report_fd = fopen(filename, "w");
    }

    void close_report() {
        if (_report_fd)
            fclose(_report_fd);
        _report_fd = nullptr;
    }

    void disable_reference() {
        assert(!_ref_cpy);
        _ref_cpy = ref;
        ref = nullptr;
    }

    void enable_reference() {
        if (_ref_cpy) {
            ref = _ref_cpy;
            _ref_cpy = nullptr;
        }
    }

private:
    bool _enable_print = false;
    FILE *_report_fd = nullptr;
    TReference *_ref_cpy = nullptr;
    struct {
        u32 count;
        u32 miss;
    } _stat;
    std::set<u32> _idx;

    void _update_stat(bool miss) {
        _stat.count++;
        _stat.miss += miss ? 1 : 0;
        if (_report_fd)
            print_statistics(true, _report_fd);
    }

    /**
     * hw: hardware result.
     * valid: is valid (in PHT.sv)
     * sw: software (referenece) result.
     */
    void _check_pht_entry(u32 hw, bool valid, int sw) {
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
            auto index = bitcast(i, BPB_T - 1, 0);
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