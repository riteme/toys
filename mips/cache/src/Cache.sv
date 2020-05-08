`include "cache.vh"

module Cache #(
    TAG_WIDTH = `CACHE_T,
    SET_WIDTH = `CACHE_S,
    LINE_WIDTH = `CACHE_B,
    SET_SIZE = `CACHE_E
) (
    /**
     * stall: do nothing when stall = 1.
     */
    input logic clk, reset, stall,

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
     * Interface with imem/dmem.
     *
     * mwrite_en: issue a write to memory if mwrite_en = 1.
     * mdata: data for memory.
     * mout: output from memory.
     */
    output logic mwrite_en,
    output logic [31:0] maddr, mdata,
    input logic [31:0] mout
);

endmodule