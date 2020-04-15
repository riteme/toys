module HazardController(
    input logic E_is_load,
    input logic [4:0] rs, rt,
    input logic [4:0] E_rd, M_rd,
    input logic [31:0] E_out, M_out,
    output logic stall, v1_mux, v2_mux,
    output logic [31:0] v1_fw, v2_fw
);
    assign stall = E_is_load & (E_rd == rs | E_rd == rt);
    assign v1_mux = &rs & (rs == E_rd | rs == M_rd);
    assign v2_mux = &rt & (rt == E_rd | rt == M_rd);
    assign v1_fw = rs == E_rd ? E_out : M_out;
    assign v2_fw = rt == E_rd ? E_out : M_out;
endmodule