`include "cache.vh"

/**
 * simply consists of multiple lines.
 */
module CacheSet #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B,
    SET_SIZE = `CACHE_E,
    KEY_WIDTH = $clog2(SET_SIZE)
) (
    input logic clk, reset, en,

    /**
     * the following ports are directly passed to
     * cache lines.
     *
     * see CacheLine.sv for more details.
     */
    input logic by_tag,
    input logic [TAG_WIDTH - 1:0] target_tag,
    input logic [KEY_WIDTH - 1:0] target_key,
    input logic [LINE_WIDTH - 1:0] index,
    input logic [2:0] ctrl,
    input logic [31:0] data, set_tick,
    input logic [TAG_WIDTH - 1:0] set_tag,

    /**
     * hit: if any line matches?
     * out: the output of the matched line.
     */
    output logic hit,
    output logic [31:0] out,

    /**
     * replacement strategy outputs.
     *
     * dirty: is the line swapped out dirty?
     */
    output logic dirty,
    output logic [TAG_WIDTH - 1:0] tag,
    output logic [KEY_WIDTH - 1:0] key
);
    logic [SET_SIZE - 1:0] hits;
    logic [31:0] outs[SET_SIZE];
    logic dirty_array[SET_SIZE];
    logic [31:0] tick_array[SET_SIZE];
    logic [TAG_WIDTH - 1:0] tag_array[SET_SIZE];

    for (genvar i = 0; i < SET_SIZE; i++)
    begin: lines
        CacheLine #(
            .TAG_WIDTH(TAG_WIDTH),
            .LINE_WIDTH(LINE_WIDTH),
            .KEY_WIDTH(KEY_WIDTH)
        ) inst(
            .clk(clk), .reset(reset), .en(en),
            .key(i), .by_tag(by_tag),
            .target_tag(target_tag), .target_key(target_key),
            .index(index), .ctrl(ctrl), .data(data),
            .set_tick(set_tick), .set_tag(set_tag),
            .dirty(dirty_array[i]),
            .tag(tag_array[i]),
            .tick(tick_array[i]),
            .hit(hits[i]), .out(outs[i])
        );
    end

    assign hit = |hits;
    Aggregate #(
        .VAL_WIDTH(32),
        .ARR_SIZE(SET_SIZE)
    ) sum_out(.array_in(outs), .out(out));

    LRUStrategy #(
        .SET_SIZE(SET_SIZE)
    ) selector(
        .tick(tick_array),
        .out(key)
    );
    assign dirty = dirty_array[key];
    assign tag = tag_array[key];
endmodule