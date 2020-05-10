`include "cache.vh"

/**
 * cache sets & cache controller
 */
module Cache #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B,
    SET_SIZE = `CACHE_E,
    KEY_WIDTH = $clog2(SET_SIZE),
    _NUM_SET = 2**SET_WIDTH
) (
    /**
     * en: do nothing when en = 0.
     */
    input logic clk, reset, en,

    /**
     * Interface with MIPS CPU.
     *
     * ready: issue a read/write when ready = 1.
     * write_en: if ready = 1, a write is issued iff. write_en = 1.
     * data: input data.
     * out: output data.
     * hit: hit = 1 indicates successful one-cycle read/write.
     */
    input logic ready, write_en,
    input logic [31:0] addr, data,
    output logic hit,
    output logic [31:0] out,

    /**
     * Interface with memory.
     *
     * mwrite_en: issue a write to memory if mwrite_en = 1.
     * mdata: data for memory.
     * mout: output from memory.
     */
    output logic mwrite_en,
    output logic [31:0] maddr, mdata,
    input logic [31:0] mout
);
    logic enabled;
    assign enabled = en & ready;

    logic [TAG_WIDTH - 1:0] tag;
    logic [SET_WIDTH - 1:0] idx;
    logic [LINE_WIDTH - 1:0] offset;
    assign {tag, idx, offset} = addr;

    logic [_NUM_SET - 1:0] set_en;
    Decoder #(
        .WIDTH(SET_WIDTH)
    ) decoder(.idx(idx), .out(set_en));

    /**
     * request interface
     */
    logic by_tag;
    logic [TAG_WIDTH - 1:0] target_tag;
    logic [KEY_WIDTH - 1:0] target_key;
    logic [LINE_WIDTH - 1:0] index;
    logic [2:0] ctrl;
    logic [31:0] write_data, set_tick;
    logic [TAG_WIDTH - 1:0] set_tag;

    /**
     * response interface
     */
    logic line_hit, line_dirty;
    logic [31:0] line_out;
    logic [TAG_WIDTH - 1:0] line_tag;
    logic [KEY_WIDTH - 1:0] line_key;

    CacheController #(
        .TAG_WIDTH(TAG_WIDTH),
        .SET_WIDTH(SET_WIDTH),
        .LINE_WIDTH(LINE_WIDTH),
        .KEY_WIDTH(KEY_WIDTH)
    ) cctrl(
        .clk(clk), .reset(reset), .en(enabled),

        .write_en(write_en),
        .tag(tag), .idx(idx), .offset(offset),
        .data(data), .hit(hit), .out(out),

        .by_tag(by_tag),
        .target_tag(target_tag),
        .target_key(target_key),
        .index(index), .ctrl(ctrl),
        .write_data(write_data),
        .set_tick(set_tick),
        .set_tag(set_tag),
        .line_hit(line_hit),
        .line_dirty(line_dirty),
        .line_out(line_out),
        .line_tag(line_tag),
        .line_key(line_key),

        .mwrite_en(mwrite_en),
        .maddr(maddr), .mdata(mdata),
        .mout(mout)
    );

    /**
     * multiplexers
     */
    logic hit_array[_NUM_SET];
    logic dirty_array[_NUM_SET];
    logic [31:0] out_array[_NUM_SET];
    logic [TAG_WIDTH - 1:0] tag_array[_NUM_SET];
    logic [KEY_WIDTH - 1:0] key_array[_NUM_SET];
    assign line_hit = hit_array[idx];
    assign line_dirty = dirty_array[idx];
    assign line_out = out_array[idx];
    assign line_tag = tag_array[idx];
    assign line_key = key_array[idx];

    for (genvar i = 0; i < _NUM_SET; i++)
    begin: sets
        CacheSet #(
            .TAG_WIDTH(TAG_WIDTH),
            .SET_WIDTH(SET_WIDTH),
            .LINE_WIDTH(LINE_WIDTH),
            .SET_SIZE(SET_SIZE),
            .KEY_WIDTH(KEY_WIDTH)
        ) inst(
            .clk(clk), .reset(reset),
            .en(enabled & set_en[i]),
            .by_tag(by_tag),
            .target_tag(target_tag),
            .target_key(target_key),
            .index(index), .ctrl(ctrl),
            .data(write_data),
            .set_tick(set_tick),
            .set_tag(set_tag),

            .hit(hit_array[i]),
            .out(out_array[i]),
            .dirty(dirty_array[i]),
            .tag(tag_array[i]),
            .key(key_array[i])
        );
    end
endmodule