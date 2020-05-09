`include "cache.vh"

module CacheControllerFSM #(
    TAG_WIDTH = `CACHE_T,
    COUNT_MAX = 2**(`CACHE_B - 2)
) (
    input logic clk, reset, en,
    output logic [2:0] state,
    output logic [TAG_WIDTH - 1:0] saved_tag,

    input logic [31:0] count,
    input logic line_hit, line_dirty,
    input logic [TAG_WIDTH - 1:0] line_tag
);
    /**
     * 5 states DFA:
     * normal -> req (-> writeback) -> alloc -> fetch -> normal
     *
     * state = {tick_en, mode}
     *
     * normal: 1**
     * req: 000
     * writeback: 010, with count
     * alloc: 001
     * fetch: 011, with count
     */
    always_ff @(posedge clk) begin
        if (reset)
            state <= 3'b100;
        else if (en) begin
            case (state)
                3'b000: begin  // req
                    state <= line_dirty ? 3'b010 : 3'b001;
                    saved_tag <= line_tag;
                end
                3'b001:  // alloc
                    state <= 3'b011;
                3'b010:  // writeback
                    if (count == COUNT_MAX - 1)
                        state <= 3'b001;
                3'b011:  // fetch
                    if (count == COUNT_MAX - 1)
                        state <= 3'b100;
                default:  // normal
                    if (!line_hit)
                        state <= 3'b000;
            endcase
        end
    end
endmodule