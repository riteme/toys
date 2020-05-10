`include "cache.vh"
`include "CacheController.vh"

module CacheControllerFSM #(
    KEY_WIDTH = $clog2(`CACHE_E),
    COUNT_MAX = 2**(`CACHE_B - 2)
) (
    input logic clk, reset, en,
    output logic [3:0] state,
    input logic [31:0] count,
    input logic line_hit, line_dirty
);
    /**
     * 5 states DFA:
     * normal -> check (-> write) -> alloc -> fetch -> normal
     *
     * state = {is_normal, is_check, ctrl[1:0]}
     * normal: 1*** (1000, 8)
     * check: 0100 (4)
     * write: 0000 (0), with count
     * alloc: 0010 (2)
     * fetch: 0001 (1), with count
     */
    always_ff @(posedge clk) begin
        if (reset)
            state <= `NORMAL;
        else if (en) begin
            case (state)
                `CHECK:
                    state <= line_dirty ? `WRITE : `ALLOC;
                `ALLOC:
                    state <= `FETCH;
                `WRITE:
                    if (count == COUNT_MAX - 1)
                        state <= `ALLOC;
                `FETCH:
                    if (count == COUNT_MAX - 1)
                        state <= `NORMAL;
                default:
                    if (!line_hit)
                        state <= `CHECK;
            endcase
        end
    end
endmodule