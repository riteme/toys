`include "Common.vh"

module Frontend(
    input logic clk, reset, eq,
    input logic [31:0] data, prev, prev_pc, vs,
    output logic [31:0] iaddr, pc, instr
);
    logic [31:0] cpc, bf, bpc;
    logic ok;

    assign iaddr = cpc;

    logic normal;
    logic [1:0] result;
    logic [31:0] next_pc, pred_pc, pred_instr;
    logic [31:0] pred, rpc;


    FrontendSelect select(
        .cpc(cpc), .data(data),
        .bpc(bpc), .bf(bf),
        .pc(pc), .instr(instr),
        .pred_pc(pred_pc), .pred_instr(pred_instr),
        .result(result)
    );

    FrontendPredict predict(
        .pc(pred_pc), .instr(pred_instr),
        .pred(pred)
    );

    FrontendLogic _logic(
        .eq(eq), .vs(vs),
        .prev(prev), .prev_pc(prev_pc),
        .normal(normal), .rpc(rpc)
    );

    assign next_pc = normal ? pred : rpc;

    always_ff @(posedge clk, negedge clk) begin
        if (reset) begin
            {cpc, bf, bpc, ok} <= 0;
        end else if (clk) begin
            ok <= normal;
            cpc <= next_pc;
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
            ok <= 1;
        end
    end
endmodule