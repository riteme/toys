`include "bpb.vh"
`include "Opcode.vh"

module InstructionTraits(
    input logic [31:0] instr,
    output logic [5:0] op, funct,
    output logic [`TMAX:0] traits
);
    assign op = instr[31:26];
    assign funct = instr[5:0];

    assign traits = {
        op == `RTYPE && funct == `JR,
        op == `BEQ || op == `BNE,
        op == `JAL,
        op == `JMP
    };

    logic __unused_ok = &{1'b0, instr[25:6], 1'b0};
endmodule