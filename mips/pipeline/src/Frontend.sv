`include "Common.vh"

module Frontend(
    input logic clk, stall, reset, eq,
    input logic [31:0] data, prev, prev_pc, vs,
    output logic [31:0] iaddr, pc, instr
);
    logic [31:0] cpc;

    assign iaddr = cpc;

    logic miss;
    logic [31:0] next_pc;
    logic [31:0] pred, rpc;

    FrontendPredict predict(
        .pc(cpc), .instr(data),
        .pred(pred)
    );

    FrontendLogic _logic(
        .eq(eq), .vs(vs),
        .prev(prev), .prev_pc(prev_pc),
        .miss(miss), .rpc(rpc)
    );

    assign next_pc = miss ? rpc : pred;
    assign {pc, instr} = miss ? 0 : {cpc, data};

    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            cpc <= 0;
        end else if (!stall) begin
            cpc <= next_pc;
        end
    end
endmodule