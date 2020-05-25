/**
 * pattern history table
 */
module PHT #(
    IWIDTH = 6,
    CWIDTH = 2,
    TRUST_FALLBACK = 0,
    _SIZE = 2**IWIDTH
) (
    input logic clk, reset, en,

    /**
     * do_update: is the instr in DECODE is branch?
     * last_taken: if so, the feedback (taken/not taken) is sent here.
     */
    input logic do_update, last_taken,

    /**
     * do_lookup: is the instr in FETCH is branch?
     * index: dependent on index policy.
     */
    input logic do_lookup,
    input logic [IWIDTH - 1:0] index,

    /**
     * fallback: default prediction for the instr in FETCH.
     * pred: prediction produced by PHT. Either from a matched entry or the fallback.
     */
    input logic fallback,
    output logic pred
);
    logic [_SIZE - 1:0] valid;
    logic [CWIDTH - 1:0] cnt[_SIZE];
    logic [IWIDTH - 1:0] last_index;

    logic hit;
    assign hit = valid[index];
    assign pred = hit ? cnt[index][CWIDTH - 1] : fallback;

    logic [CWIDTH - 1:0] next_cnt, fallback_cnt;
    if (TRUST_FALLBACK)
        assign fallback_cnt = fallback ? 2**CWIDTH - 1 : 0;
    else
        assign fallback_cnt = 2**(CWIDTH - 1) - (!fallback);

    SaturateCounter #(
        .WIDTH(CWIDTH)
    ) inc(
        .taken(last_taken),
        .in(cnt[last_index]), .out(next_cnt)
    );

    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            // for (int i = 0; i < _SIZE; i++) begin
            //     cnt[i] <= 0;
            // end
            cnt <= '{default: 0};
            valid <= 0;
            last_index <= 0;
        end else if (en) begin
            if (do_lookup && !hit) begin
                valid[index] <= 1'b1;
                cnt[index] <= fallback_cnt;
            end

            if (do_update) begin
                cnt[last_index] <= next_cnt;
            end

            last_index <= index;
        end
    end
endmodule