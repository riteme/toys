`include "Opcode.vh"

module FrontendLogic(
    input logic eq,
    input logic [31:0] prev_instr, prev_pc, vs,
    output logic [31:0] rpc
);
    logic [31:0] addr, next_pc;
    AddressExtract parser(
        .pc(prev_pc), .instr(prev_instr),
        .out(addr)
    );
    assign next_pc = prev_pc + 4;

    logic [5:0] opcode, funct;
    assign opcode = prev_instr[31:26];
    assign funct = prev_instr[5:0];

    always_comb begin
    case (opcode)
        `RTYPE:
        if (funct == `JR)  // jr
            rpc = vs;
        else
            rpc = next_pc;

        `BEQ:
        if (!eq)// beq
            rpc = next_pc;
        else
            rpc = addr;

        `BNE:
        if (eq)  // bne
            rpc = next_pc;
        else
            rpc = addr;

        `JMP:
            rpc = addr;
        `JAL:
            rpc = addr;

        default:
            rpc = next_pc;
    endcase
    end
endmodule