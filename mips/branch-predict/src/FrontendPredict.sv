`include "bpb.vh"
`include "Opcode.vh"

module FrontendPredict(
    input logic [31:0] cur_pc, cur_instr,
    input logic miss,
    input logic [31:0] prev_pc, prev_instr,
    output logic [31:0] pred_pc
);
    logic [31:0] addr;
    AddressExtract parser(
        .pc(cur_pc), .instr(cur_instr),
        .out(addr)
    );

    assign pred_pc = addr;

    logic __unused_ok = &{1'b0, miss, prev_pc, prev_instr,1'b0};
endmodule