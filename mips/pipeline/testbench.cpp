// #define NDEBUG

#include <random>
#include <algorithm>

#include "verilated.h"
#include "testbench.h"

static Device dev;
static std::vector<ITest*> test_list;

std::vector<ITest*> *p_test_list = &test_list;
ITest *current_test = nullptr;

using namespace std;

u32 randi() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen();
}


//
// TESTS
//

BEGIN(1)
    dev.reset();
    assert(dev.pc0() == 0);
    for (int i = 0; i < 32; i++)
        assert(dev[i] == 0);
END(1, "reset")

BEGIN(2)
    dev.resize_imem(9);
    dev.reset();

    // dev.enable_print();
    for (int i = 0; i < 4; i++) {
        dev.run();

        assert(dev.pc0() == 8 * i);
        assert(dev.instr0() == 0);
    }
END(2, "double fetch")

BEGIN(5)
    dev.resize_imem(8);

    auto add = RTYPE(ADD, $v0, $s0, $s1, 0);
    dev.imem[0] = dev.imem[2] = add;
    dev.reset();

    // dev.enable_print();
    dev.run();
    assert(dev.pc0() == 0);
    assert(dev.instr0() == add);

    dev.run();
    assert(dev.pc0() == 8);
    assert(dev.instr0() == add);
END(5, "nop1")

BEGIN(4)
    dev.resize_imem(8);

    auto add = RTYPE(ADD, $v0, $s0, $s1, 0);
    dev.imem[0] = dev.imem[1] = add;
    dev.reset();

    // dev.enable_print();
    dev.run();
    assert(dev.pc0() == 0);
    assert(dev.instr0() == add);

    dev.run();
    assert(dev.pc0() == 4);
    assert(dev.instr0() == add);
END(4, "nop2")

BEGIN(3)
    dev.resize_imem(32);

    auto add = RTYPE(ADD, $v0, $s0, $s1, 0);
    for (int i = 0; i < 8; i++)
        dev.imem[i] = add;

    dev.reset();

    // dev.enable_print();
    for (int i = 0; i < 8; i++) {
        dev.run();
        assert(dev.pc0() == i * 4);
        assert(dev.instr0() == add);
        // printf("can_swap: %d\n", dev.dp->Datapath__DOT__frontend__DOT__select__DOT__can_swap);
    }
END(3, "normal fetch")

BEGIN(6)
    dev.resize_imem(32);
    dev.resize_dmem(32);

    for (int i = 0; i < 8; i++)
        dev.dmem[i] = i + 1;
    for (int i = 0; i < 8; i++)
        dev.imem[i] = ITYPE(LW, $0, $t0 + i, i * 4);

    // dev.enable_print();
    dev.reset();

    dev.run(11);
    for (int i = 0; i < 8; i++)
        assert(dev[$t0 + i] == i + 1);
END(6, "lw sequence")

BEGIN(7)
    dev.resize_imem(32);
    dev.resize_dmem(32);

    for (int i = 0; i < 16; i++)
        dev.dmem[i] = i * 4;
    for (int i = 0; i < 8; i++)
        dev.imem[i] = ITYPE(LW, $t0 + i, $t0 + i + 1, 4);

    // dev.enable_print();
    dev.reset();
    dev.run(18);

    for (int i = 0; i < 9; i++)
        assert(dev[$t0 + i] == dev.dmem[i]);
END(7, "lw sequence 2")

BEGIN(8)
    dev.dmem = {233, 666};
    dev.imem = {
        NOP,
        JTYPE(JMP, 4),
        ITYPE(LW, $0, $v0, 0),
        JTYPE(JMP, 5),
        ITYPE(LW, $0, $v0, 4)
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(6);
    assert(dev[$v0] == 666);
END(8, "jmp in buf")

BEGIN(9)
    dev.dmem = {233, 666};
    dev.imem = {
        NOP,
        JTYPE(JAL, 4),
        ITYPE(LW, $0, $v0, 0),
        JTYPE(JMP, 5),
        ITYPE(LW, $0, $v0, 4)
    };
    dev.imem.resize(32);
    // dev.enable_print();
    dev.reset();
    dev.run(6);
    assert(dev[$ra] == 8);
    assert(dev[$v0] == 666);
END(9, "jal in buf")

BEGIN(10)
    int n = 500;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(ADD, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum += dev.dmem[i];
        assert(dev[$v0] == sum);
    }
END(10, "add")

BEGIN(11)
    int n = 500;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(SUB, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum -= dev.dmem[i];
        assert(dev[$v0] == sum);
    }
END(11, "sub")

BEGIN(18)
    int n = 32;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    dev.imem.push_back(ITYPE(ADDI, $0, $v0, -1));
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(AND, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(4);

    u32 sum = -1;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum &= dev.dmem[i];
        assert(dev[$v0] == sum);
    }
END(18, "and")

BEGIN(12)
    int n = 32;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(OR, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum |= dev.dmem[i];
        assert(dev[$v0] == sum);
    }
END(12, "or")

BEGIN(13)
    int n = 500;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(XOR, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum ^= dev.dmem[i];
        assert(dev[$v0] == sum);
    }
END(13, "xor")

BEGIN(14)
    int n = 500;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++)
        dev.dmem.push_back(randi());
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $v1, 4 * i));
        dev.imem.push_back(RTYPE(NOR, $v0, $v1, $v0, 0));
    }
    dev.imem.resize(2 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);

    int sum = 0;
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v1] == dev.dmem[i]);

        dev.run(1);
        sum = ~(sum | dev.dmem[i]);
        assert(dev[$v0] == sum);
    }
END(14, "nor")

BEGIN(15)
    int n = 500;

    dev.dmem.clear();
    dev.imem.clear();
    for (int i = 0; i < n; i++) {
        dev.dmem.push_back(randi());
        dev.dmem.push_back(randi());
    }
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $t0, 8 * i));
        dev.imem.push_back(ITYPE(LW, $0, $t1, 8 * i + 4));
        dev.imem.push_back(RTYPE(SLT, $t0, $t1, $v0, 0));
    }
    dev.imem.resize(3 * n + 16);

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        int a = dev.dmem[2 * i];
        int b = dev.dmem[2 * i + 1];

        dev.run(2);
        assert(dev[$t0] == a);
        assert(dev[$t1] == b);
        dev.run(2);
        assert(dev[$v0] == (a < b));
    }
END(15, "slt")

BEGIN(16)
    dev.resize_dmem(32);
    dev.resize_imem(64);
    for (int i = 0; i < 32; i++) {
        dev.dmem[i] = randi();
        dev.imem[i] = ITYPE(LW, $0, i, 4 * i);
    }

    dev.reset();
    dev.run(35);
    assert(dev[0] == 0);
    for (int i = 1; i < 32; i++)
        assert(dev[i] == dev.dmem[i]);
END(16, "write to registers")

BEGIN(17)
    int n = 500;
    dev.resize_imem(n + 16);

    int sum = 0;
    vector<int> arr;
    for (int i = 0; i < n; i++) {
        int rv = int(randi() << 16) >> 16;  // note: sign extension
        dev.imem[i] = ITYPE(ADDI, $v0, $v0, rv);
        sum += rv;
        arr.push_back(sum);
    }

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(1);
        assert(dev[$v0] == arr[i]);
    }
END(17, "addi")

BEGIN(19)
    int n = 32;
    dev.resize_imem(n + 32);

    u32 sum = -1;
    vector<u32> arr;
    dev.imem[0] = ITYPE(ADDI, $0, $v0, -1);
    for (int i = 0; i < n; i++) {
        int rv = randi() & 0xffff;
        dev.imem[i + 1] = ITYPE(ANDI, $v0, $v0, rv);
        sum &= rv;
        arr.push_back(sum);
    }

    // dev.enable_print();
    dev.reset();
    dev.run(4);
    for (int i = 0; i < n; i++) {
        dev.run(1);
        assert(dev[$v0] == arr[i]);
    }
END(19, "andi")

BEGIN(20)
    int n = 32;
    dev.resize_imem(n + 32);

    u32 sum = 0;
    vector<u32> arr;
    for (int i = 0; i < n; i++) {
        int rv = randi() & 0xffff;
        dev.imem[i] = ITYPE(ORI, $v0, $v0, rv);
        sum |= rv;
        arr.push_back(sum);
    }

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(1);
        assert(dev[$v0] == arr[i]);
    }
END(20, "ori")

BEGIN(21)
    int n = 500;
    dev.resize_imem(n + 16);

    int sum = 0;
    vector<int> arr;
    for (int i = 0; i < n; i++) {
        int rv = randi() & 0xffff;
        dev.imem[i] = ITYPE(XORI, $v0, $v0, rv);
        sum ^= rv;
        arr.push_back(sum);
    }

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(1);
        assert(dev[$v0] == arr[i]);
    }
END(21, "xori")

BEGIN(22)
    int n = 500;
    dev.imem.clear();

    vector<int> arr;
    for (int i = 0; i < n; i++) {
        int a = int(randi() << 16) >> 16;
        int b = int(randi() << 16) >> 16;
        dev.imem.push_back(ITYPE(ADDI, $0, $t0, a));
        dev.imem.push_back(ITYPE(SLTI, $t0, $v0, b));
        arr.push_back(a < b);
    }
    dev.imem.resize(2 * n + 32);

    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(2);
        assert(dev[$v0] == arr[i]);
    }
END(22, "slti")

BEGIN(23)
    int n = 500;

    dev.imem.clear();
    vector<u32> arr;
    for (int i = 0; i < n; i++) {
        u32 v = randi() & 0xffff;
        int k = randi() % 32;
        dev.imem.push_back(ITYPE(ORI, $0, $v0, v));
        dev.imem.push_back(RTYPE(SLL, 0, $v0, $v0, k));
        arr.push_back(v << k);
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(2);
        // printf("%d <-> %d\n", dev[$v0], arr[i]);
        assert(dev[$v0] == arr[i]);
    }
END(23, "sll")

BEGIN(24)
    int n = 500;

    dev.imem.clear();
    vector<int> arr;
    for (int i = 0; i < n; i++) {
        int v = int(randi() << 16) >> 16;
        int k = randi() % 32;
        dev.imem.push_back(ITYPE(ADDI, $0, $v0, v));
        dev.imem.push_back(RTYPE(SRA, 0, $v0, $v0, k));
        arr.push_back(v >> k);
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(2);
        // printf("%d <-> %d\n", dev[$v0], arr[i]);
        assert(dev[$v0] == arr[i]);
    }
END(24, "sra")

BEGIN(25)
    int n = 500;

    dev.imem.clear();
    vector<u32> arr;
    for (int i = 0; i < n; i++) {
        u32 v = int(randi() << 16) >> 16;
        int k = randi() % 32;
        dev.imem.push_back(ITYPE(ADDI, $0, $v0, v));
        dev.imem.push_back(RTYPE(SRL, 0, $v0, $v0, k));
        arr.push_back(v >> k);
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(3);
    for (int i = 0; i < n; i++) {
        dev.run(2);
        // printf("%d <-> %d\n", dev[$v0], arr[i]);
        assert(dev[$v0] == arr[i]);
    }
END(25, "srl")

BEGIN(26)
    dev.resize_dmem(32);
    dev.resize_imem(128);

    vector<int> arr;
    for (int i = 0; i < 32; i++) {
        int v = int(randi() << 16) >> 16;
        dev.imem[i] = ITYPE(ADDI, $0, i, v);
        arr.push_back(v);
    }
    for (int i = 0; i < 32; i++)
        dev.imem[32 + i] = ITYPE(SW, $0, i, 4 * i);

    // dev.enable_print();
    dev.reset();
    dev.run(66);
    assert(dev.dmem[0] == 0);  // $0 is hardcoded
    for (int i = 1; i < 32; i++)
        assert(dev.dmem[i] == arr[i]);
END(26, "sw")

BEGIN(27)
    int n = 128;

    dev.resize_dmem(2 * n);
    for (int i = 0; i < n; i++)
        dev.dmem[i] = randi();

    dev.imem.clear();
    for (int i = 0; i < n; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $t0, 4 * i));
        dev.imem.push_back(ITYPE(SW, $0, $t0, 4 * (n + i)));
    }
    dev.imem.resize(2 * n + 32);

    dev.reset();
    dev.run(3 * n + 2);  // due to stalls
    for (int i = 0; i < n; i++)
        assert(dev.dmem[i] == dev.dmem[n + i]);
END(27, "memcpy")

/**
 * NOTE:
 * Too aggressive swap heuristics will result in
 *  poor performance of memcpy2 & memcpy3.
 * Since pipeline frontend has little knowledge
 *  about programs. Compilers may make better
 *  decision in the order of loads & stores.
 *
 * principles:
 *  * DO NOT swap consecutive `lw` instructions.
 *  * DO NOT pre-emit non-branch & non-`lw` instructions.
 */

BEGIN(28)
    int n = 128;

    dev.resize_dmem(2 * n);
    for (int i = 0; i < n; i++)
        dev.dmem[i] = randi();

    dev.imem.clear();
    for (int i = 0; i < n / 2; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $t0, 8 * i));
        dev.imem.push_back(ITYPE(LW, $0, $t1, 8 * i + 4));
        dev.imem.push_back(ITYPE(SW, $0, $t0, 4 * n + 8 * i));
        dev.imem.push_back(ITYPE(SW, $0, $t1, 4 * n + 8 * i + 4));
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(2 * n + 2);
    for (int i = 0; i < n; i++)
        assert(dev.dmem[i] == dev.dmem[n + i]);
END(28, "memcpy2")

BEGIN(29)
    int n = 128;

    dev.resize_dmem(2 * n);
    for (int i = 0; i < n; i++)
        dev.dmem[i] = randi();

    dev.imem.clear();
    dev.imem.push_back(NOP);
    for (int i = 0; i < n / 2; i++) {
        dev.imem.push_back(ITYPE(LW, $0, $t0, 8 * i));
        dev.imem.push_back(ITYPE(LW, $0, $t1, 8 * i + 4));
        dev.imem.push_back(ITYPE(SW, $0, $t0, 4 * n + 8 * i));
        dev.imem.push_back(ITYPE(SW, $0, $t1, 4 * n + 8 * i + 4));
    }
    dev.imem.resize(2 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(2 * n + 3);
    for (int i = 0; i < n; i++)
        assert(dev.dmem[i] == dev.dmem[n + i]);
END(29, "memcpy3")

BEGIN(30)
    for (int _ = 0; _ < 500; _++) {
        int v = int(randi() << 16) >> 16;

        dev.imem = {
            ITYPE(SLTI, $0, $t0, v),
            ITYPE(BEQ, $0, $t0, 2),
            ITYPE(ADDI, $0, $v0, 233),
            JTYPE(JMP, 5),
            ITYPE(ADDI, $0, $v0, 666),
            NOP
        };
        dev.imem.resize(32);

        // dev.enable_print();
        dev.reset();
        dev.run(0 < v ? 8 : 6);
        assert(dev[$t0] == (0 < v));
        assert(dev[$v0] == (0 < v ? 233 : 666));
    }
END(30, "beq in buf")

BEGIN(31)
    for (int _ = 0; _ < 500; _++) {
        int v = int(randi() << 16) >> 16;

        dev.imem = {
            ITYPE(SLTI, $0, $t0, v),
            ITYPE(BNE, $0, $t0, 2),
            ITYPE(ADDI, $0, $v0, 233),
            JTYPE(JMP, 5),
            ITYPE(ADDI, $0, $v0, 666),
            NOP
        };
        dev.imem.resize(32);

        // dev.enable_print();
        dev.reset();
        dev.run(0 < v ? 6 : 8);
        assert(dev[$t0] == (0 < v));
        assert(dev[$v0] == (0 < v ? 666 : 233));
    }
END(31, "bne in buf")

BEGIN(32)
    dev.imem = {
        ITYPE(ADDI, $0, $t0, 12),
        RTYPE(JR, $t0, 0, 0, 0),
        ITYPE(ADDI, $0, $v0, 233),
        ITYPE(ADDI, $0, $v1, 666),
        ITYPE(ADDI, $0, $a0, 10086)
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(8);
    assert(dev[$t0] == 12);
    assert(dev[$v0] == 0);
    assert(dev[$v1] == 666);
    assert(dev[$a0] == 10086);
END(32, "jr in buf")

BEGIN(33)
    int n = 500;

    dev.resize_dmem(2 * n);
    for (int i = 0; i < n; i++)
        dev.dmem[i] = randi();
    dev.imem.clear();
    vector<u32> arr;
    for (int i = 0; i < n; i++) {
        u32 v = randi() & 0xffff;
        arr.push_back(v);
        dev.append(ITYPE(LW, $0, $v0, 4 * i));
        dev.append(ITYPE(ORI, $0, $v0, v));
        dev.append(ITYPE(SW, $0, $v0, 4 * (n + i)));
    }
    dev.imem.resize(3 * n + 32);

    // dev.enable_print();
    dev.reset();
    dev.run(3 * n + 2);
    for (int i = 0; i < n; i++)
        assert(arr[i] == dev.dmem[n + i]);
END(33, "forwarding priority")

BEGIN(34)
    dev.imem = {
        NOP,
        ITYPE(ADDI, $0, $t0, 1),
        JTYPE(JMP, 4),
        ITYPE(ADDI, $0, $t1, 2),
        ITYPE(ADDI, $0, $t2, 3)
    };
    dev.imem.resize(32);

    dev.reset();
    dev.run(2);
    assert(dev.pc0() == 8);
    assert(dev.instr0() == JTYPE(JMP, 4));
    dev.run(1);
    assert(dev.pc0() == 4);
    assert(dev.instr0() == ITYPE(ADDI, $0, $t0, 1));
    dev.run(4);
    assert(dev[$t0] == 1);
    assert(dev[$t1] == 0);
    assert(dev[$t2] == 3);
END(34, "swap jmp")

BEGIN(35)
    dev.imem = {
        NOP,
        ITYPE(ADDI, $0, $t0, 1),
        JTYPE(JAL, 4),
        ITYPE(ADDI, $0, $t1, 2),
        ITYPE(ADDI, $0, $t2, 3)
    };
    dev.imem.resize(32);

    dev.reset();
    dev.run(2);
    assert(dev.pc0() == 8);
    assert(dev.instr0() == JTYPE(JAL, 4));
    dev.run(1);
    assert(dev.pc0() == 4);
    assert(dev.instr0() == ITYPE(ADDI, $0, $t0, 1));
    dev.run(4);
    assert(dev[$t0] == 1);
    assert(dev[$t1] == 0);
    assert(dev[$t2] == 3);
    assert(dev[$ra] == 12);
END(35, "swap jal")

BEGIN(36)
    dev.imem = {
        NOP,
        ITYPE(ADDI, $0, $t0, 1),
        JTYPE(JAL, 4),
        ITYPE(ADDI, $0, $t1, 2),
        ITYPE(ADDI, $0, $t2, 3),
        RTYPE(JR, $ra, 0, 0, 0),
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(2);
    assert(dev.pc0() == 8);
    assert(dev.instr0() == JTYPE(JAL, 4));
    dev.run(1);
    assert(dev.pc0() == 4);
    assert(dev.instr0() == ITYPE(ADDI, $0, $t0, 1));
    dev.run(1);
    assert(dev.pc0() == 20);
    assert(dev.instr0() == RTYPE(JR, $ra, 0, 0, 0));
    dev.run(1);
    assert(dev.pc0() == 16);
    assert(dev.instr0() == ITYPE(ADDI, $0, $t2, 3));
    dev.run(1);
    assert(dev.pc0() == 12);
    assert(dev.instr0() == ITYPE(ADDI, $0, $t1, 2));
    dev.run(3);
    assert(dev[$t0] == 1);
    assert(dev[$t1] == 2);
    assert(dev[$t2] == 3);
    assert(dev[$ra] == 12);
END(36, "swap jr")

BEGIN(37)
    dev.imem = {
        ITYPE(ORI, $0, $t0, 0),
        ITYPE(ORI, $0, $v0, 123),
        ITYPE(BEQ, $t0, $0, 2),
        ITYPE(ORI, $0, $v1, 233),
        JTYPE(JMP, 6),
        ITYPE(ORI, $0, $v1, 666),
        NOP
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(2);
    assert(dev.pc0() == 8);
    assert(dev.instr0() == ITYPE(BEQ, $t0, $0, 2));
    dev.run(5);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 666);

    dev.imem[0] = ITYPE(ORI, $0, $t0, 1);
    dev.reset();
    dev.run(8);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 233);
    dev.run(8);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 233);
END(37, "swap beq")

BEGIN(38)
    dev.imem = {
        ITYPE(ORI, $0, $t0, 1),
        ITYPE(ORI, $0, $v0, 123),
        ITYPE(BNE, $t0, $0, 2),
        ITYPE(ORI, $0, $v1, 233),
        JTYPE(JMP, 6),
        ITYPE(ORI, $0, $v1, 666),
        NOP
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(2);
    assert(dev.pc0() == 8);
    assert(dev.instr0() == ITYPE(BNE, $t0, $0, 2));
    dev.run(5);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 666);

    dev.imem[0] = ITYPE(ORI, $0, $t0, 0);
    dev.reset();
    dev.run(8);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 233);
    dev.run(8);
    assert(dev[$v0] == 123);
    assert(dev[$v1] == 233);
END(38, "swap bne")

BEGIN(39)
    dev.dmem = {666};
    dev.imem = {
        NOP,
        ITYPE(ORI, $0, $v0, 233),
        ITYPE(LW, $0, $v1, 0),
        RTYPE(OR, $v1, $0, $t0, 0)
    };
    dev.imem.resize(32);

    dev.reset();
    dev.run(7);
    assert(dev[$v0] == 233);
    assert(dev[$v1] == 666);
    assert(dev[$t0] == 666);
END(39, "swap lw")

BEGIN(40)
    dev.imem = {
        NOP,
        JTYPE(JMP, 3),
        JTYPE(JMP, 4),
        ITYPE(ORI, $0, $t0, 233),
        ITYPE(ORI, $0, $t1, 666)
    };
    dev.imem.resize(32);

    // dev.enable_print();
    dev.reset();
    dev.run(7);
    assert(dev[$t0] == 233);
    assert(dev[$t1] == 666);
END(40, "double jmp")

BEGIN(41)
    dev.imem = {
        ITYPE(ORI, $0, $at, 100),
        ITYPE(ORI, $0, $a0, 1),
        RTYPE(ADD, $v0, $a0, $v0, 0),
        RTYPE(SLT, $a0, $at, $t0, 0),
        ITYPE(ADDI, $a0, $a0, 1),
        ITYPE(BNE, $t0, $0, -4),
        NOP
    };
    int fin = dev.imem.size() * 4 - 4;
    dev.imem.resize(64);

    // dev.enable_print();
    dev.reset();
    while (dev.pc0() < fin)
        dev.run();
    assert(dev[$a0] == 100);
    dev.run(4);
    assert(dev[$a0] == 101);
    assert(dev[$v0] == 5050);
END(41, "for loop")

BEGIN(42)
    int T = 1000, n = 2048;
    dev.resize_dmem(n + 1);
    dev.imem = {
        ITYPE(ORI, $0, $t0, 1),  // l
        ITYPE(ORI, $0, $t1, n + 1),  // r
        ITYPE(LW, $0, $at, 0),  // K
        RTYPE(ADD, $t0, $t1, $t2, 0),
        RTYPE(SRL, 0, $t2, $t2, 1),  // m = (l + r) / 2
        RTYPE(SLL, 0, $t2, $t3, 2),  // addr = 4 * m
        ITYPE(LW, $t3, $t4, 0),  // $t4 = arr[m]
        RTYPE(SLT, $t4, $at, $t5, 0),
        ITYPE(BNE, $t5, $0, 2),
        RTYPE(ADD, $0, $t2, $t1, 0),  // $t4 >= $at
        JTYPE(JMP, 13),
        RTYPE(ADD, $0, $t2, $t0, 0),  // $t4 < $at
        ITYPE(ADDI, $t0, $t0, 1),
        ITYPE(BNE, $t0, $t1, -11)  // if $t0 != $t1 ?
    };
    int fin = dev.imem.size() * 4;
    dev.imem.resize(128);

    // dev.enable_print();
    while (T--) {
        for (int i = 0; i <= n; i++)
            dev.dmem[i] = randi() >> 1;
        sort(dev.dmem.begin() + 1, dev.dmem.end());
        int ans = lower_bound(
            dev.dmem.begin() + 1, dev.dmem.end(), dev.dmem[0]
        ) - dev.dmem.begin();

        // printf("%u, %u\n", dev.dmem[0], dev.dmem[ans]);

        // printf("ans = %d\n", ans);
        // for (int i = 0; i <= n; i++)
        //     printf("%u\n", dev.dmem[i]);

        dev.reset();
        while (dev.pc0() <= fin)
            dev.run();
        dev.run(4);
        assert(dev[$t0] == ans);
    }
END(42, "bisection")

BEGIN(43)
    dev.resize_dmem(8);
    dev.imem = {
        ITYPE(ORI, $0, $sp, 32),
        JTYPE(JMP, 10),
        ITYPE(ADDI, $sp, $sp, -4),
        ITYPE(SW, $sp, $t0, 0),
        RTYPE(ADD, $a0, $a0, $t0, 0),
        RTYPE(ADD, $0, $t0, $v0, 0),
        ITYPE(LW, $sp, $t0, 0),
        ITYPE(ADDI, $sp, $sp, 4),
        RTYPE(JR, $ra, 0, 0, 0),
        ITYPE(ORI, $0, $t0, 233),  // useless

        ITYPE(ORI, $0, $t0, 666),
        ITYPE(ORI, $0, $a0, 123),
        JTYPE(JAL, 2),
        RTYPE(ADD, $0, $v0, $t1, 0)
    };
    dev.imem.resize(512);

    // dev.enable_print();
    dev.reset();
    dev.run(50);
    assert(dev[$ra] == 52);
    assert(dev[$a0] == 123);
    assert(dev[$t0] == 666);
    assert(dev[$t1] == 246);
    assert(dev.dmem[7] == 666);
END(43, "function call")

/**
 * NOTE:
 * Prevent swap lw and jump instructions.
 * Avoid implicit load stalls.
 */

BEGIN(44)
    dev.dmem = {233};
    dev.imem = {
        NOP,
        ITYPE(LW, $0, $t0, 0),
        JTYPE(JMP, 4),
        ITYPE(ADDI, $0, $t1, 666),
        RTYPE(ADD, $t0, $t0, $t1, 0)
    };
    dev.imem.resize(64);

    // dev.enable_print();
    dev.reset();
    dev.run(7);
    assert(dev[$t0] == 233);
    assert(dev[$t1] == 466);
END(44, "jmp after lw")

BEGIN(45)
    dev.imem = {
        ITYPE(ORI, $0, $a0, 7),
        ITYPE(ORI, $0, $v1, 12),
        ITYPE(ORI, $0, $a1, 233),
        RTYPE(SLT, $v1, $a0, $a0, 0),
        ITYPE(BEQ, $0, $a0, 1),
        ITYPE(ADDI, $0, $a1, 0),
        NOP
    };
    dev.imem.resize(64);

    dev.reset();
    dev.run(20);
    assert(dev[$a1] == 233);
END(45, "beq test")

BEGIN(46)
    dev.imem = {
        ITYPE(ORI, $0, $t0, 1),
        ITYPE(BEQ, $0, $t0, 2),
        ITYPE(BEQ, $0, $0, 1),
        ITYPE(ORI, $0, $a0, 233),
        NOP
    };
    dev.imem.resize(128);
    dev.reset();
    dev.run(16);
    assert(dev[$a0] != 233);
END(46, "double beq")

BEGIN(47)
    dev.resize_dmem(1);
    dev.imem = {
        ITYPE(ORI, $0, $t0, 233),
        ITYPE(SW, $0, $t0, 0),
        ITYPE(LW, $0, $t1, 0)
    };
    dev.imem.resize(64);

    dev.reset();
    dev.run(10);
    assert(dev[$t0] == 233);
    assert(dev[$t1] == 233);
END(47, "load after store")


//
// MAIN
//

typedef void handler_t(int);

void unix_error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

handler_t *Signal(int signum, handler_t *handler) {
    struct sigaction action, old_action;

    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
        unix_error("Signal error");
    return (old_action.sa_handler);
}

void abort_handler(int) {
    if (current_test)
        fprintf(stderr, "\033[31mERR!\033[0m abort in \"%s\"\n", current_test->name);
    fflush(stdout);
}

void exit_handler() {
    abort_handler(0);
}

int main(int argc, char *argv[]) {
    Verilated::commandArgs(argc, argv);

    Signal(SIGABRT, abort_handler);
    atexit(exit_handler);

    for (auto t : test_list)
        t->run();

    return 0;
}
