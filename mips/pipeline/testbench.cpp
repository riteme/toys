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
    assert(dev[$ra] == 4);
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
 * Since pipeline frontend have little knowledge
 *  about programs. Compilers may have better
 *  decision in the order of loads & stores.
 *
 * baseline:
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

/**
 * TODO:
 *  * beq, bne, jr in buf
 *  * data forwarding
 *  * j, jal, beq, bne, jr, lw pre-emit
 *  * consecutive jump/branch
 *  * for loop
 *  * function call, factorial
 */


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
