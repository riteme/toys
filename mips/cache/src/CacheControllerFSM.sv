`include "cache.vh"
`include "CacheController.vh"

module CacheControllerFSM #(
    KEY_WIDTH = $clog2(`CACHE_E),
    COUNT_MAX = 2**(`CACHE_B - 2)
) (
    input logic clk, reset, en,
    output logic [1:0] state,
    input logic [31:0] count,
    input logic line_hit, line_dirty
);
    /**
     * 3 states DFA:
     * normal (-> write) -> (fetch -> alloc) -> normal
     *
     * alloc actually updates cache line meta information,
     * not "alloc" a new line. alloc happens at the last step
     * of fetch process.
     *
     * state = {is_normal, write_en}
     * normal: 1* (10, 2)
     * write: 00 (0), with count
     * fetch: 01 (1), with count
     */
    always_ff @(posedge clk) begin
        if (reset)
            state <= `NORMAL;
        else if (en) begin
            case (state)
                `WRITE:
                    if (count == COUNT_MAX - 1)
                        state <= `FETCH;
                `FETCH:
                    if (count == COUNT_MAX - 1)
                        state <= `NORMAL;
                default:
                    if (!line_hit)
                        state <= line_dirty ? `WRITE : `FETCH;
            endcase
        end
    end
endmodule