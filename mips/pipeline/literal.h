#include <cstdint>

typedef uint32_t u32;
typedef const char *cstr;

const char *r[] = {
    "$0", "at", "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3",
    "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3",
    "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1",
    "gp", "sp", "fp", "ra"
};

#define $0 0
#define $at 1
#define $v0 2
#define $v1 3
#define $a0 4
#define $a1 5
#define $a2 6
#define $a3 7
#define $t0 8
#define $t1 9
#define $t2 10
#define $t3 11
#define $t4 12
#define $t5 13
#define $t6 14
#define $t7 15
#define $s0 16
#define $s1 17
#define $s2 18
#define $s3 19
#define $s4 20
#define $s5 21
#define $s6 22
#define $s7 23
#define $t8 24
#define $t9 25
#define $k0 26
#define $k1 27
#define $gp 28
#define $sp 29
#define $fp 30
#define $ra 31

#define NOP 0
#define ADD 0x20
#define SUB 0x22
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define NOR 0x27
#define SLT 0x2a
#define ADDI 0x8
#define ANDI 0xc
#define ORI 0xd
#define XORI 0xe
#define SLTI 0xa
#define SW 0x2b
#define LW 0x23
#define JMP 0x2
#define BEQ 0x4
#define BNE 0x5
#define JAL 0x3
#define JR 0x8
#define SLL 0
#define SRL 0x2
#define SRA 0x3

#define _mask(len) ((1u << (len)) - 1u)
#define _slot(data, hi, lo) (((data) & _mask((hi) - (lo) + 1)) << (lo))

#define RTYPE(rs, rt, rd, sht, funct) ( \
    _slot(rs, 25, 21) | _slot(rt, 20, 16) | \
    _slot(rd, 15, 11) | _slot(sht, 10, 6) | \
    _slot(funct, 5, 0))
#define ITYPE(op, rs, rt, imm) ( \
    _slot(op, 31, 26) | _slot(rs, 25, 21) | \
    _slot(rt, 20, 16) | _slot(imm, 15, 0))
#define JTYPE(op, addr) ( \
    _slot(op, 31, 26) | _slot(addr, 25, 0))
