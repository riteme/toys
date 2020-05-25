#pragma once

#include <cstring>

#include <tuple>

#include "utils.h"

constexpr u32 M_hash[16] = {
    0xaaaa, 0x5555, 0x33b3,
    51295, 13543, 54947, 49066, 64334, 32201,
    17399, 35255, 41252, 30113, 4540, 56082, 14591
};

inline u32 hash_multiply(u32 x) {
    u32 val = 0;
    for (int i = 0; i < 16; i++) {
        if ((x >> i) & 1)
            val ^= M_hash[i];
    }
    return val;
}

inline u32 evaluate_hash(u32 x) {
    assert((x & 3) == 0);
    return (x & 0xfffc0000) | (hash_multiply((x & 0x0003fffc) >> 2) << 2);
}

class PHT {
public:
    int tb[BPB_SIZE];

    void reset() {
        memset(tb, -1, sizeof(tb));
    }

    bool get(u32 index, int fallback) {
        int &v = tb[index];
        if (v < 0)
            v = fallback;
        return v & 2;
    }

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

    bool predict(u32 pc, u32 instr) {
        return std::get<0>(_predict(pc, instr));
    }

    void update(u32 pc, u32 instr, bool miss) {
        bool pred, mux;
        u32 tag, gindex, lindex;
        std::tie(pred, mux, tag, gindex, lindex) =
            _predict(pc, instr);

        bool real_taken = pred ^ miss;
        selector.update(tag, mux ^ miss);
        if (mux)
            lshare.update(lindex, real_taken);
        else
            gshare.update(gindex, real_taken);

        ght = ((ght << 1) | real_taken) & BPB_HMASK;
        bht[tag] = ((bht[tag] << 1) | real_taken) & BPB_HMASK;
    }

private:
    friend class Device;

    auto _predict(u32 pc, u32 instr)
        -> std::tuple<bool, bool, u32, u32, u32> {
        u32 hashed_pc = evaluate_hash(pc);
        u32 tag = (pc >> 2) & BPB_TMASK;
        u32 hashed_tag = (hashed_pc >> 2) & BPB_TMASK;
        u32 track = bht[tag];
        u32 imm = (int(instr) << 16) >> 16;
        u32 addr = pc + 1 + imm;
        u32 gindex = hashed_tag ^ ght;
        u32 lindex = hashed_tag ^ track;
        // printf("hashed_pc=%x, hashed_tag=%x\n", hashed_pc, hashed_tag);

        bool btfnt = addr <= pc;
        int fallback = 1 + btfnt;
        bool mux = selector.get(tag, 0b01);
        bool pred = mux ?
            lshare.get(lindex, fallback) :
            gshare.get(gindex, fallback);

        return std::make_tuple(
            pred, mux, tag, gindex, lindex
        );
    }

    u32 ght, bht[BPB_SIZE];
    PHT gshare, lshare, selector;
};