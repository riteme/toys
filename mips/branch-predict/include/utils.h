#pragma once

#include <cassert>

#include <random>

using u64 = unsigned long long;
using u32 = unsigned int;
using cstr = const char*;

#define BPB_T 6
#define BPB_SIZE (1 << BPB_T)

#define JTYPE(op, addr) ( \
    (u32(op) << 26) | \
    (u32(addr) & 0x03ffffff) \
)
#define JMP(addr) JTYPE(0b000010, addr)
#define JAL(addr) JTYPE(0b000011, addr)

#define ITYPE(op, rs, rt, imm) ( \
    (u32(op) << 26) | \
    ((u32(rs) & 0x3f) << 20) | \
    ((u32(rt) & 0x3f) << 16) | \
    (u32(imm) & 0xffff) \
)
#define BEQ(rs, rt, offset) ITYPE(0b000100, rs, rt, offset)
#define BNE(rs, rt, offset) ITYPE(0b000101, rs, rt, offset)

#define DEFAULT_SEED 19260817

/**
 * generate a random 64-bit unsigned integer.
 */
inline u64 randi() {
    static std::mt19937_64 gen(DEFAULT_SEED);
    return gen();
}

/**
 * generate an unsigned integer in [l, r].
 */
inline u64 randi(u64 l, u64 r) {
    return randi() % (r - l + 1) + l;
}

inline auto bitcast(u32 data, int hi, int lo) -> std::string {
    std::string buf;
    buf.resize(hi - lo + 1);

    data >>= lo;
    for (int i = 0; i <= hi - lo; i++) {
        buf[i] = (data >> i) & 1 ? '1' : '0';
    }
    std::reverse(buf.begin(), buf.end());

    return buf;
}