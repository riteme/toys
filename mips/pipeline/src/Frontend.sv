`include "Common.vh"

module Frontend(
    input logic clk, reset, eq,
    input logic [31:0] data, prev, prev_pc, vs,
    output logic [31:0] iaddr, pc, instr
);
    logic [31:0] cpc, bf, bpc;
    logic ok;

    assign iaddr = cpc;

    logic normal, miss, req;
    logic [1:0] result;
    logic [31:0] next_pc, pred_pc, pred_instr;
    logic [31:0] pred, rpc;

    logic [31:0] _pc, _instr;
    FrontendSelect select(
        .cpc(cpc), .data(data),
        .bpc(bpc), .bf(bf),
        .pc(_pc), .instr(_instr),
        .pred_pc(pred_pc), .pred_instr(pred_instr),
        .result(result), .req(req)
    );

    FrontendPredict predict(
        .pc(pred_pc), .instr(pred_instr),
        .pred(pred)
    );

    FrontendLogic _logic(
        .eq(eq), .vs(vs),
        .prev(prev), .prev_pc(prev_pc),
        .miss(miss), .rpc(rpc)
    );

    assign normal = ~(miss | req);
    assign next_pc = miss ? rpc : pred;
    assign {pc, instr} = miss ? {bpc, bf} : {_pc, _instr};

    // logic first_time;
    always_ff @(posedge clk, negedge clk) begin
        if (reset) begin
            {cpc, bf, bpc, ok} <= 0;
            // first_time <= 1;
        end else if (clk) begin
            ok <= normal;
            cpc <= next_pc;
            // cpc <= first_time ? 0 : next_pc;
            // first_time <= 0;
            if (result[1]) begin  // insert `nop`
                bf <= 0;
            end else if (result[0]) begin  // normal propagation
                bf <= data;
                bpc <= cpc;
            end
        end else if (!ok) begin
            bf <= data;
            bpc <= cpc;
            cpc <= cpc + 4;
        end
    end
endmodule