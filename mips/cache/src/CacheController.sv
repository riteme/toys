`include "cache.vh"

module CacheController #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B,
    _COUNT_MAX = 2**(LINE_WIDTH - 2)
) (
    input logic clk, reset, en,

    /**
     * interface with MIPS CPU.
     * addr has been parsed into {tag, idx, offset}.
     */
    input logic write_en,
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
    logic count_reset;
    logic [31:0] count;
    Counter counter(
        .clk(clk), .reset(count_reset), .en(1),
        .count(count)
    );

    logic [2:0] state;
    logic [TAG_WIDTH - 1:0] saved_tag;
    CacheControllerFSM #(
        .TAG_WIDTH(TAG_WIDTH),
        .COUNT_MAX(_COUNT_MAX)
    ) fsm(
        .clk(clk), .reset(reset), .en(en),
        .state(state), .saved_tag(saved_tag),

        .count(count),
        .line_hit(line_hit),
        .line_dirty(line_dirty),
        .line_tag(line_tag)
    );

    logic normal;
    assign normal = state[2];
    assign tick_en = normal;
    assign hit = normal & line_hit;
    assign out = line_out;

    logic [LINE_WIDTH - 1:0] pos;
    assign pos = count[LINE_WIDTH - 1:0] << 2;
    assign mdata = line_out;

    /**
     * dataflow & control signals for each state.
     */
    always_comb begin
        {mode, mwrite_en, count_reset} = 0;
        if (en) begin
            if (normal)
                mode = {1'b1, write_en};
            else
                mode = state[1:0];

            case (state)
                3'b000:  // req
                    count_reset = 1;

                3'b001: begin  // alloc
                    target = tag;
                    count_reset = 1;
                end

                3'b010: begin  // writeback: cache -> mem
                    {target, index} = {saved_tag, pos};
                    maddr = {saved_tag, idx, pos};
                    mwrite_en = 1;
                end

                3'b011: begin  // fetch: mem -> cache
                    {target, index} = {tag, pos};
                    maddr = {tag, idx, pos};
                    write_data = mout;
                end

                default: begin  // normal
                    {target, index} = {tag, offset};
                    write_data = data;
                end
            endcase
        end
    end
endmodule