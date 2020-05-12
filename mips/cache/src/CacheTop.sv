/**
 * for simulation only.
 */

`include "cache.vh"

module CacheTop #(
    KEY_WIDTH = $clog2(`CACHE_E)
) (
    input logic clk, reset, en,

    input logic ready, is_write,
    input logic [31:0] addr, data,
    output logic hit,
    output logic [31:0] out,

    output logic mwrite_en,
    output logic [31:0] maddr, mdata,
    input logic [31:0] mout,

    /**
     * inspecting internal signals
     */
    output logic enabled,
    output logic [31:0] now, count,
    output logic [1:0] state,
    output logic req_dirty[`SET_NUM],
    output logic [`CACHE_T - 1:0] req_tag[`SET_NUM],
    output logic [KEY_WIDTH - 1:0] swap_key[`SET_NUM],
    output logic set_en[`SET_NUM],
    output logic line_en[`SET_NUM][`CACHE_E],
    output logic set_hit[`SET_NUM],
    output logic line_hit[`SET_NUM][`CACHE_E],
    output logic tick_en[`SET_NUM][`CACHE_E],
    output logic valid[`SET_NUM][`CACHE_E],
    output logic dirty[`SET_NUM][`CACHE_E],
    output logic [`CACHE_T - 1:0] tag[`SET_NUM][`CACHE_E],
    output logic [31:0] tick[`SET_NUM][`CACHE_E],
    output logic [31:0] line[`SET_NUM][`CACHE_E][`LINE_SIZE]
);
    Cache cache(
        .clk(clk), .reset(reset), .en(en),
        .ready(ready), .write_en(is_write),
        .addr(addr), .data(data),
        .hit(hit), .out(out),
        .mwrite_en(mwrite_en),
        .maddr(maddr), .mdata(mdata),
        .mout(mout)
    );

    assign enabled = cache.enabled;
    assign now = cache.cctrl.now;
    assign count = cache.cctrl.count;
    assign state = cache.cctrl.state;

    for (genvar i = 0; i < `SET_NUM; i++) begin
        assign req_dirty[i] = cache.sets[i].inst.dirty;
        assign req_tag[i] = cache.sets[i].inst.tag;
        assign set_en[i] = cache.sets[i].inst.en;
        assign set_hit[i] = cache.sets[i].inst.hit;
        assign swap_key[i] = cache.sets[i].inst.key;

        for (genvar j = 0; j < `CACHE_E; j++) begin
            assign line_en[i][j] = cache.sets[i].inst.lines[j].inst.en;
            assign line_hit[i][j] = cache.sets[i].inst.lines[j].inst.hit;
            assign tick_en[i][j] = cache.sets[i].inst.lines[j].inst.tick_en;

            assign valid[i][j] = cache.sets[i].inst.lines[j].inst.valid;
            assign dirty[i][j] = cache.sets[i].inst.lines[j].inst.dirty;
            assign tag[i][j] = cache.sets[i].inst.lines[j].inst.tag;
            assign tick[i][j] = cache.sets[i].inst.lines[j].inst.tick;
            for (genvar k = 0; k < `LINE_SIZE; k++)
                assign line[i][j][k] =
                    cache.sets[i].inst.lines[j].inst.line[k];
        end
    end
endmodule