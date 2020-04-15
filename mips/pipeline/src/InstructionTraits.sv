`include "Common.vh"

module InstructionTraits(
    input logic [5:0] opcode, funct,
    output logic [`MAX_INSTR_ID:0] instr_tb,
    output logic [`MAX_TYPE_ID:0] type_tb
);
    logic is_rtype = opcode == 0;
    assign instr_tb[`ADD] = is_rtype & funct == 6'b100000;
    assign instr_tb[`SUB] = is_rtype & funct == 6'b100010;
    assign instr_tb[`AND] = is_rtype & funct == 6'b100100;
    assign instr_tb[`OR] = is_rtype & funct == 6'b100101;
    assign instr_tb[`XOR] = is_rtype & funct == 6'b100110;
    assign instr_tb[`NOR] = is_rtype & funct == 6'b100111;
    assign instr_tb[`SLT] = is_rtype & funct == 6'b101010;
    assign instr_tb[`JR] = is_rtype & funct == 6'b001000;
    assign instr_tb[`SLL] = is_rtype & funct == 6'b000000;
    assign instr_tb[`SRL] = is_rtype & funct == 6'b000010;
    assign instr_tb[`SRA] = is_rtype & funct == 6'b000011;

    logic is_irtype = instr_tb[`ADDI] | instr_tb[`ANDI] |
        instr_tb[`ORI] | instr_tb[`XORI] | instr_tb[`SLTI];
    // logic is_itype = is_irtype | instr_tb[`SW] |
    //     instr_tb[`LW] | instr_tb[`BEQ] | instr_tb[`BNE];
    assign instr_tb[`SW] = opcode == 6'b101011;
    assign instr_tb[`LW] = opcode == 6'b100011;
    assign instr_tb[`ADDI] = opcode == 6'b001000;
    assign instr_tb[`ANDI] = opcode == 6'b001100;
    assign instr_tb[`ORI] = opcode == 6'b001101;
    assign instr_tb[`XORI] = opcode == 6'b001110;
    assign instr_tb[`SLTI] = opcode == 6'b001010;
    assign instr_tb[`BEQ] = opcode == 6'b000100;
    assign instr_tb[`BNE] = opcode == 6'b000101;

    logic is_jtype = instr_tb[`JMP] | instr_tb[`JAL];
    assign instr_tb[`JMP] = opcode == 6'b000010;
    assign instr_tb[`JAL] = opcode == 6'b000011;

    assign type_tb[`RTYPE] = is_rtype;
    assign type_tb[`IRTYPE] = is_irtype;
    // assign type_tb[`ITYPE] = is_itype;
    // assign type_tb[`JTYPE] = is_jtype;
    assign type_tb[`BRANCH] = is_jtype |
        instr_tb[`BEQ] | instr_tb[`BNE] | instr_tb[`JR];
endmodule