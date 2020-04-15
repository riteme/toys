module ExecuteStage(
    input logic [4:0] rd, sht,
    input logic [31:0] imm, vs, vt,
    input logic [7:2] signal,
    output logic [4:0] _rd,
    output logic [31:0] out, _vt
);
    logic [5:2] alu_op;
    logic alu_src1, alu_src2;
    assign alu_op = signal[5:2];
    assign alu_src1 = signal[7];
    assign alu_src2 = signal[6];

    logic [31:0] v1, v2;
    assign v1 = alu_src1 ? {27'b0, sht} : vs;
    assign v2 = alu_src2 ? imm : vt;

    ALU alu(
        .op(alu_op),
        .v1(v1), .v2(v2),
        .out(out)
    );

    assign _rd = rd;
    assign _vt = vt;
endmodule