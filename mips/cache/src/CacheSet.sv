`include "cache.vh"

module CacheSet #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B,
    SET_SIZE = `CACHE_E,
    KEY_WIDTH = $clog2(SET_SIZE)
) (
    /**
     * tick_en: reserved for LFU strategy. LRU does not need it.
     */
    input logic clk, reset, en, tick_en,

    /**
     * mode [1:0]:
     *  10: read
     *  11: write
     *  00: req
     *  01: alloc
     *
     * mode[0]: write_en?
     * mode[1]: no tick?
     *
     * NOTE: alloc only resets tag & dirty flag, and does not
     * clear cache line, which is expected to be filled by
     * subsequent writes.
     */
    input logic [1:0] mode,
    input logic [31:0] addr, data,
    output logic hit,
    output logic [31:0] out,

    /**
     * For `req` mode.
     *
     * dirty: is the line swapped out dirty?
     * tag: if dirty = 1, tag contains the line tag.
     */
    output logic dirty,
    output logic [TAG_WIDTH - 1:0] tag
);
    logic [TAG_WIDTH - 1:0] target;
    logic [SET_WIDTH - 1:0] idx;
    logic [LINE_WIDTH - 1:0] index;
    assign {target, idx, index} = addr;

    logic [SET_SIZE - 1:0] hits;
    logic [31:0] outs[SET_SIZE];
    logic dirty_array[SET_SIZE];
    logic [31:0] tick_array[SET_SIZE];
    logic [TAG_WIDTH - 1:0] tag_array[SET_SIZE];

    logic need_write, need_tick, is_write;
    assign {need_write, need_tick} = mode;
    assign is_write = mode == 2'b11;

    for (genvar i = 0; i < SET_SIZE; i++) begin
        CacheLine #(
            .TAG_WIDTH(TAG_WIDTH),
            .LINE_WIDTH(LINE_WIDTH)
        ) lines(
            .clk(clk), .reset(reset), .en(en),
            .target(target), .index(index),

            .tick_en(tick_en & need_tick),
            .set_tick(now),

            .write_en(need_write),
            .data(data),
            .set_dirty(is_write),
            .set_tag(is_write ? tag_array[i] : target),

            .hit(hits[i]), .dirty(dirty_array[i]),
            .tag(tag_array[i]), .tick(tick_array[i]),
            .out(outs[i])
        );
    end
    assign hit = |hits;
    Aggregate #(
        .VAL_WIDTH(32),
        .ARR_SIZE(SET_SIZE)
    ) sum_out(.array_in(outs), .out(out));

    logic [KEY_WIDTH - 1:0] select;
    LRUStrategy #(
        .SET_SIZE(SET_SIZE)
    ) selector(
        .tick(tick_array),
        .out(select)
    );
    assign dirty = dirty_array[select];
    assign tag = tag_array[select];

    logic [31:0] now;
    Counter #(
        .START_COUNT(1)
    ) counter(
        .clk(clk), .reset(reset), .en(en),
        .count(now)
    );

    logic __unused_ok = &{1'b0, idx, 1'b0};
endmodule