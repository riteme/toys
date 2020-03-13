#include <cstdio>
#include <cstring>

#include <fstream>

#include "verilated.h"
#include "VCore.h"

#define IMEM_SIZE 1024
#define DMEM_SIZE 4096

typedef uint32_t u32;

int tickcnt = 1;
u32 imem[IMEM_SIZE], dmem[DMEM_SIZE];
u32 regs[32];

u32 read(u32 addr, int pc = -1) {
    addr = addr >> 2;

    if (addr >= DMEM_SIZE) {
        /*
        if (pc >= 0)
            printf("[pc = %d] ", pc);
        printf("read: invalid access to %d\n", addr << 2);
        // exit(-1);
        */
        return 0xCCCCCCCC;
    }

    return dmem[addr];
}

void write(u32 addr, u32 data, int pc = -1) {
    addr = addr >> 2;

    if (addr >= DMEM_SIZE) {
        if (pc >= 0)
            printf("[pc = %d] ", pc + 1);
        printf("write: invalid access to %d [data = %u]\n", addr << 2, data);
        exit(-1);
    }

    dmem[addr] = data;
}

void load_memory(const char *filename, int n, u32 *mem) {
    std::ifstream fp(filename);
    std::string buf;

    for (int i = 0; fp; i++) {
        buf.clear();
        fp >> buf;
        if (buf.empty()) break;
        // printf("%s\n", buf.c_str());

        if (i < n)
            mem[i] = std::stoul(buf, 0, 16);
        else {
            printf("%s: \"%s\" not loaded due to limited memory size (= %d)\n",
                filename, buf.c_str(), n
            );
            break;
        }
    }
}

void tick(VCore *t) {
    int pc = t->PC >> 2;
    // printf("pc = %d\n", pc + 1);

    if (pc >= IMEM_SIZE)
        return;

    t->CLK = 0;
    t->INSTR = imem[pc];
    // printf("INSTR = 0x%X\n", t->INSTR);
    t->MEM_READ = read(t->MEM_ADDR, pc);
    t->eval();

    /* ALU
    printf("[%d/%d] ALU: %d [%d] %d -> RET = %d\n",
        tickcnt, pc + 1,
        t->Core__DOT___DP__DOT__SrcA,
        (t->Core__DOT__Signals >> 9) & 0x3f,
        t->Core__DOT___DP__DOT__SrcB,
        t->Core__DOT___DP__DOT__ALURet
    ); */

    t->CLK = 1;
    if (t->SIG_WRITE) {
        // if (t->MEM_ADDR == 0) {
            printf("[%d/%d] write@%d: %u\n",
                tickcnt, (t->PC >> 2) + 1, t->MEM_ADDR, t->MEM_DATA
            );
        // }
        write(t->MEM_ADDR, t->MEM_DATA, pc);
    }
    t->MEM_READ = read(t->MEM_ADDR, pc);
    t->eval();

    if (pc + 1 != (t->PC >> 2)) {
        printf("[%d/%d] jump: %d -> %d\n",
            tickcnt, pc + 1, pc + 1, (t->PC >> 2) + 1);

        if (pc == (t->PC >> 2)) {
            printf("[%d/%d] infinte loop!\n",
                tickcnt, pc + 1);
            exit(-1);
        }
    }

    auto r = t->Core__DOT___DP__DOT__Reg__DOT__Registers;
    for (int i = 0; i < 32; i++) {
        if (regs[i] != r[i])
            printf("[%d/%d] $%d <- %u\n", tickcnt, pc + 1, i, r[i]);
        regs[i] = r[i];
    }

    tickcnt++;
}

int main(int argc, char *argv[]) {
    load_memory("code.hex", IMEM_SIZE, imem);
    load_memory("data.hex", DMEM_SIZE, dmem);

    Verilated::commandArgs(argc, argv);
    VCore *t = new VCore;

    t->RESET = 1;

    t->CLK = 0;
    t->eval();
    t->CLK = 1;
    t->eval();
    t->CLK = 0;
    t->eval();

    t->RESET = 0;

    while ((t->PC >> 2) < IMEM_SIZE)
        tick(t);

    delete t;
    return 0;
}