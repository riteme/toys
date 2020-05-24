module TestTop #(SIZE = 2**6) (
    input logic clk, reset, en,
    input logic [31:0] cur_pc, cur_instr,
    input logic miss,
    input logic [31:0] last_pc, last_instr,
    output logic [31:0] pred_pc,

    output logic [3:0] ght, bht, bht_tb[SIZE],
    output logic [5:0] tag, gindex, lindex,
    output logic [5:0] bht_last_index,
    output logic fallback,
    output logic [5:0] glast_index, llast_index, slast_index,
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