`include "Opcode.vh"

module PipelineController(
    input logic [5:0] opcode, funct,
    output logic [12:0] signal,
    output logic req
);
    logic [3:0] alu_op;
    assign alu_op = funct[3:0] ^ {{2{funct[5]}}, 2'b0};

    logic is_shift;
    assign is_shift = ~|funct[5:2];

    assign req = opcode == `SW || opcode == `LW;

    always_comb
    case (opcode)
        `RTYPE: if (funct != `JR)
            signal = {5'b10_0_0_0, is_shift, 1'b_0, alu_op, 2'b0_0};
        else  // jr
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        `ADDI:  // addi
            signal = 13'b11_1_0_0_0_1_1100_0_0;
        `ANDI:  // andi
            signal = 13'b11_0_0_0_0_1_1000_0_0;
        `ORI:  // ori
            signal = 13'b11_0_0_0_0_1_1001_0_0;
        `XORI:  // xori
            signal = 13'b11_0_0_0_0_1_1010_0_0;
        `SLTI:  // slti
            signal = 13'b11_1_0_0_0_1_0110_0_0;
        `SW:  // sw
            signal = 13'b00_1_0_0_0_1_1100_1_0;
        `LW:  // lw
            signal = 13'b11_1_0_1_0_1_1100_0_1;
        `JMP:  // j
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        `BEQ:  // beq
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        `BNE:  // bne
            signal = 13'b00_0_0_0_0_0_0000_0_0;
        `JAL:  // jal
            signal = 13'b01_0_1_0_0_1_1111_0_0;
        default: signal = 0;
    endcase
endmodule