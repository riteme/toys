`ifndef __COMMON_VH__
`define __COMMON_VH__

/* InstructionTraits */
`define MAX_TYPE_ID 2

`define RTYPE   0
`define IRTYPE  1
// `define ITYPE   1
// `define JTYPE   2
`define BRANCH  2

`define MAX_INSTR_ID 21

`define LW      0
`define SW      1
`define ADD     2
`define SUB     3
`define AND     4
`define OR      5
`define XOR     6
`define NOR     7
`define SLT     8
`define ADDI    9
`define ANDI    10
`define ORI     11
`define XORI    12
`define SLTI    13
`define JMP     14
`define BEQ     15
`define BNE     16
`define JR      17
`define SLL     18
`define SRL     19
`define SRA     20
`define JAL     21

/* FrontendSelect */
`define INSERT_NOP  2'b10
`define POP_BUF     2'b01
`define POP_DATA    2'b00

`endif