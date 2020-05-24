#pragma once

#include <cstring>

#include <tuple>

#include "utils.h"

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

        ght = ((ght << 1) | real_taken) & 0x3f;
        bht[tag] = ((bht[tag] << 1) | real_taken) & 0x3f;
    }

private:
    friend class Device;

    auto _predict(u32 pc, u32 instr)
        -> std::tuple<bool, bool, u32, u32, u32> {
        u32 tag = (pc >> 2) & 0x3f;
        u32 track = bht[tag];
        u32 imm = (int(instr) << 16) >> 16;
        u32 addr = pc + 1 + imm;
        u32 gindex = tag ^ ght;
        u32 lindex = tag ^ track;

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