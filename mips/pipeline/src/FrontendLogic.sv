module FrontendLogic(
    input logic eq,
    input logic [31:0] prev, prev_pc, vs,
    output logic normal,
    output logic [31:0] rpc
);
    logic [5:0] opcode = prev[31:26];
    logic [5:0] funct = prev[5:0];

    always_comb
    case (opcode)
        6'b000000: if (funct == 6'b001000) begin  // jr, always retake
            normal = 0;
            rpc = vs;
        end
        6'b000100: if (!eq) begin
            normal = 0;
            rpc = prev_pc + 4;
        end
        6'b000101: if (eq) begin
            normal = 0;
            rpc = prev_pc + 4;
        end
        default: normal = 1;
    endcase

    logic _unused_ok = &{1'b0, prev, 1'b0};
endmodule