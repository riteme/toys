module PipelineController(
    input logic [5:0] opcode, funct,
    output logic [12:0] signal
);
    logic [3:0] alu_op;
    assign alu_op = funct[3:0] ^ {{2{funct[5]}}, 2'b0};

    logic is_shift;
    assign is_shift = ~|funct[5:2];

    always_comb
    case (opcode)
        6'b000000: if (funct != 6'b001000)
            signal = {5'b10_0_0_0, is_shift, 1'b_0, alu_op, 2'b0_0};
        else  // jr
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        6'b001000:  // addi
            signal = 13'b11_1_0_0_0_1_1100_0_0;
        6'b001100:  // andi
            signal = 13'b11_0_0_0_0_1_1000_0_0;
        6'b001101:  // ori
            signal = 13'b11_0_0_0_0_1_1001_0_0;
        6'b001110:  // xori
            signal = 13'b11_0_0_0_0_1_1010_0_0;
        6'b001010:  // slti
            signal = 13'b11_1_0_0_0_1_0110_0_0;
        6'b101011:  // sw
            signal = 13'b00_1_0_0_0_1_1100_1_0;
        6'b100011:  // lw
            signal = 13'b11_1_0_1_0_1_1100_0_1;
        6'b000010:  // j
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        6'b000100:  // beq
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        6'b000101:  // bne
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        6'b000011:  // jal
            signal = 13'b01_0_1_0_0_1_1111_0_0;
        default: signal = 0;
    endcase
endmodule