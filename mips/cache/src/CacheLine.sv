`include "cache.vh"

module CacheLine #(
    TAG_WIDTH = `CACHE_T,
    LINE_WIDTH = `CACHE_B,
    LINE_SIZE = 2**(LINE_WIDTH - 2)
) (
    input logic clk, reset, en,

    /**
     * index: will be aligned to 4 bytes.
     */
    input logic [TAG_WIDTH - 1:0] target,
    input logic [LINE_WIDTH - 1:0] index,

    /**
     * cache line is enabled only if hit = 1.
     */
    output logic hit,

    input logic write_en,
    input logic set_dirty,
    input logic [TAG_WIDTH - 1:0] set_tag,
    input logic [31:0] data,

    input logic tick_en,
    input logic [31:0] set_tick,

    output logic dirty,
    output logic [TAG_WIDTH - 1:0] tag,
    output logic [31:0] tick,

    /**
     * if hit = 0, out = 0.
     * if hit = 1, corresponding data will be sent to out.
     */
    output logic [31:0] out
);
    logic [31:0] line[LINE_SIZE];

    assign hit = tag == target;

    logic [LINE_WIDTH - 3:0] aligned_index;
    assign aligned_index = index[LINE_WIDTH - 1:2];
    assign out = hit ? line[aligned_index] : 0;

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < LINE_SIZE; i++)
                line[i] <= 0;
            {tag, dirty, tick} <= 0;
        end else if (en && hit) begin
            if (write_en) begin
                tag <= set_tag;
                dirty <= set_dirty;
                line[aligned_index] <= data;
            end
            if (tick_en) begin
                tick <= set_tick;
            end
        end
    end

    logic __unused_ok = &{1'b0, index[1:0], 1'b0};
endmodule