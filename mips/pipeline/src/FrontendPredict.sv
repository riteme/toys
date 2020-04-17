module FrontendPredict(
    input logic [31:0] pc, instr,
    output logic [31:0] pred
);
    logic [5:0] opcode;
    logic [31:0] next_pc;
    logic [25:0] addr;
    logic [15:0] imm;
    logic [31:0] sign_imm;
    assign opcode = instr[31:26];
    assign next_pc = pc + 4;
    assign addr = instr[25:0];
    assign imm = instr[15:0];
    assign sign_imm = {{16{imm[15]}}, imm};

    logic [31:0] jta, bta;
    assign jta = {next_pc[31:28], addr, 2'b00};
    assign bta = next_pc + (sign_imm << 2);

    always_comb
    case (opcode)
        6'b000010: pred = jta;  // j
        6'b000011: pred = jta;  // jal
        6'b000100: pred = bta;  // beq
        6'b000101: pred = bta;  // bne
        default: pred = next_pc;
    endcase
endmodule