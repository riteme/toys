/**
 * for simulation only.
 */

`include "cache.vh"

module CacheTop(
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
    output logic [2:0] state,
    output logic [`CACHE_T - 1:0] saved_tag,
    output logic req_dirty[`SET_NUM],
    output logic [`CACHE_T - 1:0] req_tag[`SET_NUM],
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
    assign now = cache.now;
    assign count = cache.cctrl.count;
    assign state = cache.cctrl.state;
    assign saved_tag = cache.cctrl.saved_tag;

    for (genvar i = 0; i < `SET_NUM; i++) begin
        assign req_dirty[i] = cache.sets[i].inst.dirty;
        assign req_tag[i] = cache.sets[i].inst.tag;

        for (genvar j = 0; j < `CACHE_E; j++) begin
            assign dirty[i][j] = cache.sets[i].inst.lines[j].inst.dirty;
            assign tag[i][j] = cache.sets[i].inst.lines[j].inst.tag;
            assign tick[i][j] = cache.sets[i].inst.lines[j].inst.tick;
            for (genvar k = 0; k < `LINE_SIZE; k++)
                assign line[i][j][k] =
                    cache.sets[i].inst.lines[j].inst.line[k];
        end
    end
endmodule