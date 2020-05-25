#pragma once

#include <cstring>

#include <tuple>

#include "utils.h"

constexpr u32 M_hash[16] = {
    0xaaaa, 0x5555, 0x33b3,
    51295, 13543, 54947, 49066, 64334, 32201,
    17399, 35255, 41252, 30113, 4540, 56082, 14591
};

/**
 * calculate M*x.
 */
inline u32 hash_multiply(u32 x) {
    u32 val = 0;
    for (int i = 0; i < 16; i++) {
        if ((x >> i) & 1)
            val ^= M_hash[i];
    }
    return val;
}

/**
 * hash pc address
 */
inline u32 evaluate_hash(u32 x) {
    assert((x & 3) == 0);
    return (x & 0xfffc0000) | (hash_multiply((x & 0x0003fffc) >> 2) << 2);
}

/**
 * pattern history table
 */
class PHT {
public:
    int tb[BPB_SIZE];

    void reset() {
        memset(tb, -1, sizeof(tb));
    }

    /**
     * get table entry with `index`.
     * if not found, return fallback and
     * initialize that entry.
     */
    bool get(u32 index, int fallback) {
        int &v = tb[index];
        if (v < 0)
            v = fallback;
        return v & 2;
    }

    /**
     * update table entry.
     * acting as a 2-bit saturating counter.
     */
    void update(u32 index, bool taken) {
        int delta = taken ? 1 : -1;
        int &v = tb[index];
        assert(v >= 0);
        v = std::min(3, std::max(0, v + delta));
    }
};

class ReferencePredictor {
public:
    void reset() {
        ght = 0;
        memset(bht, 0, sizeof(bht));
        gshare.reset();
        lshare.reset();
        selector.reset();
    }

    /**
     * give prediction based on pc & instr.
     *
     * see _predict.
     */
    bool predict(u32 pc, u32 instr) {
        return std::get<0>(_predict(pc, instr));
    }

    void update(u32 pc, u32 instr, bool miss) {
        // get original predictions & basic instr information.
        bool pred, mux, same;
        u32 tag, gindex, lindex;
        std::tie(pred, mux, same, tag, gindex, lindex) =
            _predict(pc, instr);

        // if miss, the branch has been taken is `~pred`.
        bool real_taken = pred ^ miss;

        // only updates when two predictor gives different
        // predictions. Choose the correct one `mux ^ miss`.
        if (!same)
            selector.update(tag, mux ^ miss);

        // simultaneous updates.
        lshare.update(lindex, real_taken);
        gshare.update(gindex, real_taken);

        // update history tables.
        ght = ((ght << 1) | real_taken) & BPB_HMASK;
        bht[tag] = ((bht[tag] << 1) | real_taken) & BPB_HMASK;
    }

private:
    // for Device::_check_ref.
    friend class Device;

    auto _predict(u32 pc, u32 instr)
        -> std::tuple<bool, bool, bool, u32, u32, u32> {
        // instruction decoding.
        u32 hashed_pc = evaluate_hash(pc);
        u32 tag = (pc >> 2) & BPB_TMASK;
        u32 hashed_tag = (hashed_pc >> 2) & BPB_TMASK;
        u32 track = bht[tag];
        u32 imm = (int(instr) << 16) >> 16;
        u32 addr = pc + 1 + imm;

        // index for two predictors.
        u32 gindex = hashed_tag ^ ght;
        u32 lindex = hashed_tag ^ track;

        // fallback prediction.
        bool btfnt = addr <= pc;
        int fallback = 1 + btfnt;

        // select one predictor.
        // bool mux = selector.get(tag, 0b01);
        bool mux = selector.get(tag, 0);  // one more chance

        // get predictions.
        bool lpred = lshare.get(lindex, fallback);
        bool gpred = gshare.get(gindex, fallback);

        // final result.
        bool pred = mux ? lpred : gpred;
        bool same = gpred == lpred;

        return std::make_tuple(
            pred, mux, same, tag, gindex, lindex
        );
    }

    u32 ght, bht[BPB_SIZE];
    PHT gshare, lshare, selector;
};