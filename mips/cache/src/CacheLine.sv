`include "cache.vh"

/**
 * building blocks for a cache.
 */
module CacheLine #(
    TAG_WIDTH = `CACHE_T,
    LINE_WIDTH = `CACHE_B,
    KEY_WIDTH = $clog2(`CACHE_E),
    LINE_SIZE = 2**(LINE_WIDTH - 2)
) (
    input logic clk, reset, en,
    input logic [KEY_WIDTH - 1:0] key,

    /**
     * line searching.
     *
     * by_tag: match by tag or by key.
     * index: will be aligned to 4 bytes.
     *
     * NOTE: invalid lines are not allowed to be indexed by tag.
     */
    input logic by_tag,
    input logic [TAG_WIDTH - 1:0] target_tag,
    input logic [KEY_WIDTH - 1:0] target_key,
    input logic [LINE_WIDTH - 1:0] index,

    /**
     * data manipulations.
     *
     * ctrl: {tick_en, update_en, write_en}
     *   update: dirty <- 0, tag <- set_tag
     */
    input logic [2:0] ctrl,
    input logic [31:0] data, set_tick,
    input logic [TAG_WIDTH - 1:0] set_tag,

    /**
     * line information.
     */
    output logic dirty,
    output logic [TAG_WIDTH - 1:0] tag,
    output logic [31:0] tick,

    /**
     * cache line is enabled only if hit = 1.
     * if hit = 0, out = 0.
     */
    output logic hit,
    output logic [31:0] out
);
    logic valid;
    logic [31:0] line[LINE_SIZE];

    always_comb
    case (by_tag)
        0: hit = key == target_key;
        1: hit = valid && tag == target_tag;
    endcase

    logic [LINE_WIDTH - 3:0] aligned_index;
    assign aligned_index = index[LINE_WIDTH - 1:2];
    assign out = hit ? line[aligned_index] : 0;

    logic tick_en, update_en, write_en;
    assign {tick_en, update_en, write_en} = ctrl;
    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < LINE_SIZE; i++) begin
                line[i] <= 0;
            end
            {tag, valid, dirty, tick} <= 0;
        end else if (en && hit) begin
            if (write_en) begin
                dirty <= 1;
                line[aligned_index] <= data;
            end
            if (update_en) begin
                valid <= 1;
                dirty <= 0;
                tag <= set_tag;
            end
            if (tick_en)
                tick <= set_tick;
        end
    end

    logic __unused_ok = &{1'b0, index[1:0], 1'b0};
endmodule