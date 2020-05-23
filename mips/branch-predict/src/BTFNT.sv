/**
 * backwards-taken & forwards-not-taken
 */
module BTFNT(
    input logic [31:0] next_pc, cur_addr,
    output logic pred
);
    assign pred = cur_addr < next_pc;
endmodule