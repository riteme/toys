module Controller(
    input logic [5:0] OP, FUNC,
    output logic [15:0] SIG
);
    always_comb
    case (OP)
        //  SIG =     15 14:9  8 7 6 5 4 3 2 1:0
        6'b100011:  // lw $rt, imm($rs)
            SIG = 16'b0_100000_0_0_0_1_1_0_1_00;

        6'b101011:  // sw $rt, imm($rs)
            SIG = 16'b1_100000_0_0_0_1_0_0_0_00;

        6'b0: case (FUNC)  // R-type
            6'b1000:  // jr $rs
                SIG = 16'b0_000000_0_0_0_0_0_0_0_11;
            default:  // ordinary arithmetics
                SIG = {
                    1'b0, FUNC, 2'b1_1,
                    (FUNC & 3) == FUNC,  // shifts, !note on FUNC=01
                    6'b0_1_0_0_00
                };
        endcase

        6'b1000:  // addi $rt, $rs, imm
            SIG = 16'b0_100000_0_1_0_1_1_0_1_00;
        6'b1100:  // andi $rt, $rs, imm
            SIG = 16'b0_100100_0_1_0_0_1_0_1_00;
        6'b1101:  // ori $rt, $rs, imm
            SIG = 16'b0_100101_0_1_0_0_1_0_1_00;
        6'b1110:  // xori $rt, $rs, imm
            SIG = 16'b0_100110_0_1_0_0_1_0_1_00;
        6'b1010:  // slti $rt, $rs, imm
            SIG = 16'b0_101010_0_1_0_1_1_0_1_00;

        6'b10:  // j addr
            SIG = 16'b0_000000_0_0_0_0_0_0_0_01;
        6'b11:  // jal addr
            SIG = 16'b0_000000_0_0_0_0_1_1_0_01;

        6'b100:  // beq $rs, $rt, imm
            SIG = 16'b0_000000_1_0_0_1_0_0_0_10;
        6'b101:  // bne $rs, $rt, imm
            SIG = 16'b0_000000_1_0_0_1_0_0_0_10;

        default: SIG = 0;
    endcase

endmodule