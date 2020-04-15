module DecodeStage(
    input logic [25:0] instr,  // opcode excluded
    input logic [31:0] pc,
    input logic [12:9] signal,

    output logic [4:0] r1, r2,
    input logic [31:0] v1, v2,

    output logic eq,
    output logic [4:0] rd, sht,
    output logic [31:0] imm, vs, vt
);
    logic imm_src, sign_ext;
    logic [1:0] reg_dest;
    assign imm_src = signal[9];
    assign sign_ext = signal[10];
    assign reg_dest = signal[12:11];

    assign {r1, r2} = instr[25:16];
    assign sht = instr[10:6];

    logic [31:0] ext_imm;
    Extension ext(
        .sign_ext(sign_ext),
        .in(instr[15:0]), .out(ext_imm)
    );
    assign imm = imm_src ? pc : ext_imm;

    always_comb begin
        case (reg_dest)
            2'b00: rd = 0;
            2'b01: rd = 31;  // $ra
            2'b10: rd = instr[15:11];
            2'b11: rd = instr[20:16];
        endcase
    end

    assign eq = v1 == v2;
    assign vs = v1;
    assign vt = v2;
endmodule