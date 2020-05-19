`ifndef __OPCODE_VH__
`define __OPCODE_VH__

`define RTYPE 6'b000000
`define JR 6'b001000

`define ADDI 6'b001000
`define ANDI 6'b001100
`define ORI 6'b001101
`define XORI 6'b001110
`define SLTI 6'b001010
`define SW 6'b101011
`define LW 6'b100011
`define JMP 6'b000010
`define BEQ 6'b000100
`define BNE 6'b000101
`define JAL 6'b000011

`endif