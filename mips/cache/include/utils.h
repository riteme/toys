#pragma once

#include <random>

using u64 = unsigned long long;
using u32 = unsigned int;
using cstr = char*;

#define CACHE_T 24
#define CACHE_E 2
#define CACHE_B 6
#define SET_NUM (1 << 2)
#define LINE_SIZE (1 << ((CACHE_B) - 2))

#define MEMSIZE 16384  // in words, 65536 bytes
#define CLK_LIMIT 64  // maximum number of clocks in one operation (read/write/nop)
#define DEFAULT_DATA 0xcccccccc

#define DEFAULT_SEED 19260817

u64 randi(u64 l, u64 r) {
    static std::mt19937_64 gen(DEFAULT_SEED);
    return gen() % (r - l + 1) + l;
}