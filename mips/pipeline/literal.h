#include <cstdint>

#include <string>

using std::string;

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
#define _take(data, hi, lo) (((data) >> (lo)) & _mask((hi) - (lo) + 1))

#define RTYPE(funct, rs, rt, rd, sht) ( \
    _slot(rs, 25, 21) | _slot(rt, 20, 16) | \
    _slot(rd, 15, 11) | _slot(sht, 10, 6) | \
    _slot(funct, 5, 0))
#define ITYPE(op, rs, rt, imm) ( \
    _slot(op, 31, 26) | _slot(rs, 25, 21) | \
    _slot(rt, 20, 16) | _slot(imm, 15, 0))
#define JTYPE(op, addr) ( \
    _slot(op, 31, 26) | _slot(addr, 25, 0))

inline auto _format(const char *fmt, ...) -> string {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    va_end(args);
    return buf;
}

inline auto translate_rtype(u32 instr) -> string {
    const char *rs = r[_take(instr, 25, 21)];
    const char *rt = r[_take(instr, 20, 16)];
    const char *rd = r[_take(instr, 15, 11)];
    u32 sht = _take(instr, 10, 6);
    u32 funct = _take(instr, 5, 0);

    switch (funct) {
        case ADD:
            return _format("%s %s, %s, %s", "add", rd, rs, rt);
        case SUB:
            return _format("%s %s, %s, %s", "sub", rd, rs, rt);
        case AND:
            return _format("%s %s, %s, %s", "and", rd, rs, rt);
        case OR:
            return _format("%s %s, %s, %s", "or", rd, rs, rt);
        case XOR:
            return _format("%s %s, %s, %s", "xor", rd, rs, rt);
        case NOR:
            return _format("%s %s, %s, %s", "nor", rd, rs, rt);
        case SLT:
            return _format("%s %s, %s, %s", "slt", rd, rs, rt);
        case JR:
            return _format("%s %s", "jr", rs);
        case SLL:
            return _format("%s %s, %s, %u", "sll", rd, rt, sht);
        case SRA:
            return _format("%s %s, %s, %u", "sra", rd, rt, sht);
        case SRL:
            return _format("%s %s, %s, %u", "srl", rd, rt, sht);
        default:
            return "(unknown)";
    }
}

inline auto translate_itype(u32 instr) -> string {
    u32 op = _take(instr, 31, 26);
    const char *rs = r[_take(instr, 25, 21)];
    const char *rt = r[_take(instr, 20, 16)];
    u32 imm = _take(instr, 15, 0);

    switch (op) {
        case LW:
            return _format("%s %s, %u(%s)", "lw", rt, imm, rs);
        case SW:
            return _format("%s %s, %u(%s)", "sw", rt, imm, rs);
        case ADDI:
            return _format("%s %s, %s, %u", "addi", rt, rs, imm);
        case ANDI:
            return _format("%s %s, %s, %u", "andi", rt, rs, imm);
        case ORI:
            return _format("%s %s, %s, %u", "ori", rt, rs, imm);
        case XORI:
            return _format("%s %s, %s, %u", "xori", rt, rs, imm);
        case SLTI:
            return _format("%s %s, %s, %u", "slti", rt, rs, imm);
        case BEQ:
            return _format("%s %s, %s, %u", "beq", rt, rs, imm);
        case BNE:
            return _format("%s %s, %s, %u", "bne", rt, rs, imm);
        default:
            return "(unknown)";
    }
}

inline auto translate_jtype(u32 instr) -> string {
    u32 op = _take(instr, 31, 26);
    u32 addr = _take(instr, 25, 0);

    switch (op) {
        case JMP:
            return _format("%s %u", "j", addr);
        case JAL:
            return _format("%s %u", "jal", addr);
        default:
            return "(unknown)";
    }
}

inline auto translate(u32 instr) -> string {
    if (instr == 0)
        return "nop";

    u32 op = _take(instr, 31, 26);

    switch (op) {
        case 0:
            return translate_rtype(instr);
        case LW:
        case SW:
        case ADDI:
        case ANDI:
        case ORI:
        case XORI:
        case SLTI:
        case BEQ:
        case BNE:
            return translate_itype(instr);
        case JMP:
        case JAL:
            return translate_jtype(instr);
        default:
            return "(unknown)";
    }
}