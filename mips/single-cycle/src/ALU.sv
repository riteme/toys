module ALU(
    input logic [31:0] A, B,
    input logic [5:0] OP,
    output logic Z,
    output logic [31:0] RET
);
    always_comb
    case (OP)
        6'b111111: Z = A == B;
        6'b111110: Z = A != B;
        default: Z = 1;
    endcase

    always_comb
    case (OP)
        6'b100000: RET = A + B;
        6'b100010: RET = A - B;
        6'b100100: RET = A & B;
        6'b100101: RET = A | B;
        6'b100110: RET = A ^ B;
        6'b100111: RET = ~(A | B);
        6'b101010: RET = {31'b0, A < B};  // slt
        6'b0: RET = B << A;
        6'b10: RET = B >> A;
        6'b11: RET = B >>> A;
        default: RET = 32'b1;
    endcase
endmodule