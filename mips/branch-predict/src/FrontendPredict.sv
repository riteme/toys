`include "bpb.vh"
`include "Opcode.vh"

module FrontendPredict #(
    IWIDTH = `BPB_T
) (
    input logic clk, reset, en,
    input logic [31:0] cur_pc, cur_instr,
    input logic miss,
    input logic [31:0] last_pc, last_instr,
    output logic [31:0] pred_pc
);
    logic last_pred, last_mux;

    logic pred, last_taken;
    assign last_taken = last_pred ^ miss;

    logic [31:0] taddr, next_pc;
    logic [`TMAX:0] cur_traits, last_traits;
    // logic [5:0] last_op, last_funct;

    InstructionParser cur_parser(
        .pc(cur_pc), .instr(cur_instr),
        .next_pc(next_pc), .traits(cur_traits),
        .taddr(taddr)
    );
    InstructionTraits last_parser(
        .instr(last_instr),
        // .op(last_op), .funct(last_funct),
        .traits(last_traits)
    );

    logic do_update, do_lookup;
    assign do_update = last_traits[`T_BR];
    assign do_lookup = cur_traits[`T_BR];

    logic [IWIDTH - 1:0] ght, bht, tag;
    assign tag = cur_pc[IWIDTH + 1:2];  // ignore aligned bits
    GHT #(
        .HWIDTH(IWIDTH)
    ) _ght(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_taken),
        .out(ght)
    );
    BHT #(
        .IWIDTH(IWIDTH),
        .HWIDTH(IWIDTH)
    ) _bht(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_taken),
        .index(tag), .out(bht)
    );

    /**
     * mux: 0: gshare, 1: lshare [default = 0]
     *
     * NOTE: only implemented a partial-update scheme.
     */
    `define GSHARE 0
    `define LSHARE 1
    logic fallback, gpred, lpred, mux, auto_mux;
    BTFNT fallback_predictor(
        .next_pc(next_pc),
        .cur_addr(taddr),
        .pred(fallback)
    );
    PHT #(
        .IWIDTH(IWIDTH)
    ) gshare_predictor(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update && last_mux == `GSHARE),
        .last_taken(last_taken),
        .do_lookup(do_lookup && mux == `GSHARE),
        .index(tag ^ ght),
        .fallback(fallback),
        .pred(gpred)
    );
    PHT #(
        .IWIDTH(IWIDTH)
    ) lshare_predictor(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update && last_mux == `LSHARE),
        .last_taken(last_taken),
        .do_lookup(do_lookup && mux == `LSHARE),
        .index(tag ^ bht),
        .fallback(fallback),
        .pred(lpred)
    );
    PHT #(
        .IWIDTH(IWIDTH)
    ) selector(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_mux ^ miss),
        .do_lookup(do_lookup),
        .index(tag),
        .fallback(`GSHARE),
        .pred(auto_mux)
    );

    /**
     * generate prediction
     */
    always_comb begin
    `ifdef USE_BTFNT
        mux = 0;
        pred = fallback;
    `elsif USE_GSHARE
        mux = `GSHARE;
        pred = gpred;
    `elsif USE_LSHARE
        mux = `LSHARE;
        pred = lpred;
    `elsif USE_ALL
        mux = auto_mux;
        pred = mux == `GSHARE ? gpred : lpred;
    `endif
    end

    /**
     * translate predictions into pc addresses.
     */
    always_comb begin
        if (cur_traits[`T_JMP] ||
            cur_traits[`T_JAL] ||
            (cur_traits[`T_BR] && pred))
            pred_pc = taddr;
        else
            pred_pc = next_pc;
    end

    /**
     * save last prediction states for later updates.
     */
    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            {last_pred, last_mux} <= 0;
        end else if (en) begin
            last_pred <= pred;
            last_mux <= mux;
        end
    end

    logic __unused_ok = &{1'b0,
        // last_op, last_funct,
        last_pc,
        cur_traits, last_traits,
    1'b0};
endmodule