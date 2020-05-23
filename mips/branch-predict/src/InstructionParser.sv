`include "bpb.vh"
`include "Opcode.vh"

module InstructionParser(
    input logic [31:0] pc, instr,
    output logic [`TMAX:0] traits,
    output logic [31:0] taddr
);
    logic [5:0] op, funct;
    InstructionTraits ext(
        .instr(instr),
        .op(op), .funct(funct),
        .traits(traits)
    );

    logic [31:0] next_pc;
    logic [25:0] addr;
    logic [15:0] imm;
    logic [31:0] sign_imm;
    assign next_pc = pc + 4;
    assign addr = instr[25:0];
    assign imm = instr[15:0];
    assign sign_imm = {{16{imm[15]}}, imm};

    logic [31:0] jta, bta;
    assign jta = {next_pc[31:28], addr, 2'b00};
    assign bta = next_pc + (sign_imm << 2);

    always_comb
    case (op)
        `JMP: taddr = jta;
        `JAL: taddr = jta;
        `BEQ: taddr = bta;
        `BNE: taddr = bta;
        default: taddr = next_pc;
    endcase

    logic __unused_ok = &{1'b0, funct, 1'b0};
endmodule