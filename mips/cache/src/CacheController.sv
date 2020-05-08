`include "cache.vh"

module CacheController #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B
) (
    input logic clk, reset, en,

    /**
     * interface with MIPS CPU.
     * addr has been parsed into {tag, idx, offset}.
     */
    input logic ready, write_en,
    input logic [TAG_WIDTH - 1:0] tag,
    input logic [SET_WIDTH - 1:0] idx,
    input logic [LINE_WIDTH - 1:0] offset,
    input logic [31:0] data,
    output logic hit,
    output logic [31:0] out,

    /**
     * interface with cache set.
     */
    output logic tick_en,
    output logic [1:0] mode,
    output logic [TAG_WIDTH - 1:0] target,
    output logic [LINE_WIDTH - 1:0] index,
    output logic [31:0] write_data,
    input logic line_hit, line_dirty,
    input logic [31:0] line_out,
    input logic [TAG_WIDTH - 1:0] line_tag,

    /**
     * interface with memory.
     */
    output logic mwrite_en,
    output logic [31:0] maddr, mdata,
    input logic [31:0] mout
);
    // TODO: implement it.
endmodule