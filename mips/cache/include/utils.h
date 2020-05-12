#pragma once

#include <random>

using u64 = unsigned long long;
using u32 = unsigned int;
using cstr = const char*;

#define CACHE_T 24
#define CACHE_S 2
#define CACHE_B 4
#define SET_NUM (1 << 2)
#define LINE_SIZE (1 << ((CACHE_B) - 2))
#define CACHE_E 4

#define MASK(len) ((1u << (len)) - 1)
#define TAG(x) (u32(x) >> ((CACHE_S) + (CACHE_B)))
#define IDX(x) ((u32(x) >> (CACHE_B)) & MASK(CACHE_S))
#define OFFSET(x) ((u32(x) & MASK(CACHE_B)) >> 2)
#define ADDR(tag, idx, offset) ( \
    (u32(tag) << ((CACHE_S) + (CACHE_B))) | \
    (u32(idx) << (CACHE_B)) | \
    (u32(offset) << 2))

#define MEMSIZE 16384  // in words, 65536 bytes
#define CLK_LIMIT 50  // maximum number of clocks in one operation (read/write/nop)
#define DEFAULT_DATA 0xcccccccc

#define DEFAULT_SEED 19260817

inline u64 randi(u64 l, u64 r) {
    static std::mt19937_64 gen(DEFAULT_SEED);
    return gen() % (r - l + 1) + l;
}