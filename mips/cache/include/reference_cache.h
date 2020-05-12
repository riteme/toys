#pragma once

#include <cstring>
#include <cassert>

#include <tuple>
#include <vector>

#include "utils.h"

class ReferenceCache {
public:
    std::vector<u32> mem;

    ReferenceCache() {
        reset();
    }

    void reset() {
        _now = 0;
        memset(_dirty, 0, sizeof(_dirty));
        memset(_tag, 0, sizeof(_tag));
        memset(_line, 0, sizeof(_line));
        memset(_tick, 0, sizeof(_tick));
    }

    auto resolve(u32 addr) -> std::tuple<u32, u32, u32> {
        return std::make_tuple(TAG(addr), IDX(addr), OFFSET(addr));
    }

    auto read(u32 addr) -> std::tuple<u32, bool> {
        u32 tag, i, k;
        std::tie(tag, i, k) = resolve(addr);
        int j = search(tag, i);

        bool hit = j >= 0;
        if (j < 0)
            j = load(tag, i);

        _tick[i][j] = ++_now;
        return std::make_tuple(_line[i][j][k], hit);
    }

    auto write(u32 addr, u32 data) -> bool {
        u32 tag, i, k;
        std::tie(tag, i, k) = resolve(addr);
        int j = search(tag, i);

        bool hit = j >= 0;
        if (j < 0)
            j = load(tag, i);

        _tick[i][j] = ++_now;
        _dirty[i][j] = true;
        _line[i][j][k] = data;
        return hit;
    }

    auto search(u32 tag, u32 idx) -> int {
        for (int i = 0; i < CACHE_E; i++) {
            if (_tick[idx][i] && _tag[idx][i] == tag)
                return i;
        }
        return -1;
    }

    auto load(u32 tag, u32 idx) -> int {
        int i = 0;
        for (int j = 1; j < CACHE_E; j++) {
            if (_tick[idx][j] < _tick[idx][i])
                i = j;
        }
        assert(!_tick[idx][i] || _tag[idx][i] != tag);

        if (_dirty[idx][i]) {
            u32 s = ADDR(_tag[idx][i], idx, 0) >> 2;
            assert(0 <= s && s + LINE_SIZE <= mem.size());
            memcpy(&mem[s], _line[idx][i], sizeof(u32) * LINE_SIZE);
            _dirty[idx][i] = false;
        }

        _tag[idx][i] = tag;
        u32 s = ADDR(tag, idx, 0) >> 2;
        assert(0 <= s && s + LINE_SIZE <= mem.size());
        memcpy(_line[idx][i], &mem[s], sizeof(u32) * LINE_SIZE);
        return i;
    }

private:
    friend class Device;

    int _now;
    int _tick[SET_NUM][CACHE_E];
    bool _dirty[SET_NUM][CACHE_E];
    u32 _tag[SET_NUM][CACHE_E];
    u32 _line[SET_NUM][CACHE_E][LINE_SIZE];
};