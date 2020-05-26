`include "bpb.vh"
`include "Opcode.vh"

module FrontendPredict #(
    IWIDTH = `BPB_T,
    HWIDTH = `BPB_H
) (
    input logic clk, reset, en,
    input logic [31:0] cur_pc, cur_instr,
    input logic miss,
    input logic [31:0] last_pc, last_instr,
    output logic [31:0] pred_pc
);
    typedef logic [31:0] Word;
    typedef logic [`TMAX:0] Traits;
    typedef logic [IWIDTH - 1:0] Index;
    typedef logic [HWIDTH - 1:0] Track;

    logic last_pred, last_mux, last_not_same;

    logic pred, last_taken;
    assign last_taken = last_pred ^ miss;

    Word taddr, next_pc;
    Traits cur_traits, last_traits;
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

    Word hashed_pc;
    Index tag, hashed_tag;
    Track ght, bht;
    assign hashed_pc[31:18] = cur_pc[31:18];
    assign hashed_pc[1:0] = 0;
    HashFunction hash(.in(cur_pc[17:2]), .out(hashed_pc[17:2]));

    assign tag = cur_pc[IWIDTH + 1:2];  // ignore aligned bits
    assign hashed_tag = hashed_pc[IWIDTH + 1:2];

    GHT #(
        .HWIDTH(HWIDTH)
    ) _ght(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_taken),
        .out(ght)
    );
    BHT #(
        .IWIDTH(IWIDTH),
        .HWIDTH(HWIDTH)
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

    Index gindex, lindex;
    /* verilator lint_save */
    /* verilator lint_off WIDTH */
    assign gindex = hashed_tag ^ ght;
    assign lindex = hashed_tag ^ bht;
    /* verilator lint_restore */

    PHT #(
        .IWIDTH(IWIDTH)
    ) gshare_predictor(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_taken),
        .do_lookup(do_lookup),
        .index(gindex),
        .fallback(fallback),
        .pred(gpred)
    );
    PHT #(
        .IWIDTH(IWIDTH)
    ) lshare_predictor(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update),
        .last_taken(last_taken),
        .do_lookup(do_lookup),
        .index(lindex),
        .fallback(fallback),
        .pred(lpred)
    );
    PHT #(
        .IWIDTH(IWIDTH),
        .TRUST_FALLBACK(1)
    ) selector(
        .clk(clk), .reset(reset), .en(en),
        .do_update(do_update && last_not_same),
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
     * save prediction states for later updates.
     */
    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            {last_pred, last_mux, last_not_same} <= 0;
        end else if (en) begin
            last_pred <= pred;
            last_mux <= mux;
            last_not_same <= gpred != lpred;
        end
    end

    logic __unused_ok = &{1'b0,
        // last_op, last_funct,
        last_pc, hashed_pc, auto_mux,
        cur_traits, last_traits,
    1'b0};
endmodule