`include "bpb.vh"

typedef logic [31:0] Word;
typedef logic [`BPB_T - 1:0] Index;
typedef logic [`BPB_H - 1:0] Track;

module TestTop #(SIZE = 2**`BPB_T) (
    input logic clk, reset, en,
    input Word cur_pc, cur_instr,
    input logic miss,
    input Word last_pc, last_instr,
    output Word pred_pc,

    output Track ght, bht, bht_tb[SIZE],
    output Index tag, htag, gindex, lindex,
    output Index bht_last_index,
    output logic fallback,
    output Index glast_index, llast_index, slast_index,
    output logic [1:0] gcnt[SIZE], lcnt[SIZE], scnt[SIZE],
    output logic gvalid[SIZE], lvalid[SIZE], svalid[SIZE],
    output logic pred, gpred, lpred, mux
);
    FrontendPredict dev(
        .clk(clk), .reset(reset), .en(en),
        .cur_pc(cur_pc), .cur_instr(cur_instr),
        .miss(miss),
        .last_pc(last_pc), .last_instr(last_instr),
        .pred_pc(pred_pc)
    );

    assign ght = dev.ght;
    assign bht = dev.bht;
    assign tag = dev.tag;
    assign htag = dev.hashed_tag;
    assign gindex = dev.gindex;
    assign lindex = dev.lindex;

    assign bht_last_index = dev._bht.last_index;
    for (genvar i = 0; i < SIZE; i++) begin
        assign bht_tb[i] = dev._bht.tb[i];
    end

    assign fallback = dev.fallback;
    assign glast_index = dev.gshare_predictor.last_index;
    assign llast_index = dev.lshare_predictor.last_index;
    assign slast_index = dev.selector.last_index;

    for (genvar i = 0; i < SIZE; i++) begin
        assign gcnt[i] = dev.gshare_predictor.cnt[i];
        assign lcnt[i] = dev.lshare_predictor.cnt[i];
        assign scnt[i] = dev.selector.cnt[i];
        assign gvalid[i] = dev.gshare_predictor.valid[i];
        assign lvalid[i] = dev.lshare_predictor.valid[i];
        assign svalid[i] = dev.selector.valid[i];
    end

    assign pred = dev.pred;
    assign gpred = dev.gpred;
    assign lpred = dev.lpred;
    assign mux = dev.mux;
endmodule