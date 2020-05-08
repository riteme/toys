`include "cache.vh"

/**
 * NOTE: The sum of TAG_WIDTH, SET_WIDTH and OFFSET_WIDTH should be 32
 *
 * TAG_WIDTH    : (t) tag bits
 * SET_WIDTH    : (s) set index bits, the number of sets is 2**SET_WIDTH
 * OFFSET_WIDTH : (b) block offset bits
 * LINES        : number of lines per set
 *
 * stall        : inorder to synchronize instruction memroy cache and data memroy cache, you may need this so that two caches will write data at most once per instruction respectively.
 *
 * input_ready  : whether input data from processor are ready
 * addr         : cache read/write address from processor
 * write_data   : cache write data from processor
 * w_en         : cache write enable
 * hit          : whether cache hits
 * read_data    : data read from cache
 *
 * maddr        : memory address
 * mwrite_data  : data written to memory
 * m_wen        : memory write enable
 * mread_data   : data read from memory
 */
module cache #(
	parameter TAG_WIDTH    = `CACHE_T,
		      SET_WIDTH    = `CACHE_S,
		      OFFSET_WIDTH = `CACHE_B,
		      LINES        = `CACHE_E
)(
	input         clk, reset, stall,

	// interface with CPU
	input input_ready,
	input [31:0]  addr, write_data,
	input         w_en,
	output        hit,
	output [31:0] read_data,

	// interface with memory
	output [31:0] maddr, mwrite_data,
	output        m_wen,
	input [31:0]  mread_data
	/* input         mready // memory ready ? */
);

	Cache #(
		.TAG_WIDTH(TAG_WIDTH),
		.SET_WIDTH(SET_WIDTH),
		.LINE_WIDTH(OFFSET_WIDTH),
		.SET_SIZE(LINES)
	) inst(
		.clk(clk), .reset(reset), .en(~stall),
		.ready(input_ready), .write_en(w_en),
		.hit(hit),
		.addr(addr), .data(write_data), .out(read_data),
		.mwrite_en(m_wen),
		.maddr(maddr), .mdata(mwrite_data),
		.mout(mread_data)
	);

endmodule
