`include "Opcode.vh"

module HazardController(
    input logic E_is_load,
    input logic [5:0] D_opcode,
    input logic [4:0] rs, rt,
    input logic [4:0] E_rd, M_rd,
    input logic [31:0] E_out, M_out,
    output logic stall, v1_mux, v2_mux,
    output logic [31:0] v1_fw, v2_fw
);
    logic read_rt;
    // assign read_rt = type_tb[`RTYPE] | instr_tb[`SW] |
    //     instr_tb[`BEQ] | instr_tb[`BNE];  // from `RWMask`
    assign read_rt = D_opcode == `RTYPE || D_opcode == `SW ||
        D_opcode == `BEQ || D_opcode == `BNE;

    assign stall = E_is_load && E_rd != 0 &&
        (E_rd == rs || (read_rt && E_rd == rt));
    assign v1_mux = rs != 0 && (rs == E_rd || rs == M_rd);
    assign v2_mux = rt != 0 && (rt == E_rd || rt == M_rd);
    assign v1_fw = rs == E_rd ? E_out : M_out;
    assign v2_fw = rt == E_rd ? E_out : M_out;
endmodule