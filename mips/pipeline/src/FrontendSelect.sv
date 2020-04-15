`include "Common.vh"

module FrontendSelect(
    input logic [31:0] cpc, data, bf, bpc,
    output logic [31:0] pc, instr, pred_pc, pred_instr,
    output logic [1:0] result
);
    logic [14:0] B_fields = bf[25:11];
    logic [14:0] C_fields = data[25:11];

    logic [`MAX_INSTR_ID:0] B_instr;
    logic [`MAX_TYPE_ID:0] B_type;
    InstructionTraits _B(
        .opcode(bf[31:26]), .funct(bf[5:0]),
        .instr_tb(B_instr), .type_tb(B_type)
    );

    logic [`MAX_INSTR_ID:0] C_instr;
    logic [`MAX_TYPE_ID:0] C_type;
    InstructionTraits _C(
        .opcode(data[31:26]), .funct(data[5:0]),
        .instr_tb(C_instr), .type_tb(C_type)
    );

    logic [30:0] B_rmask, B_wmask;
    RWMask _B_mask(
        .instr_tb(B_instr), .type_tb(B_type),
        .fields(B_fields),
        .rmask(B_rmask), .wmask(B_wmask)
    );

    logic [30:0] C_rmask, C_wmask;
    RWMask _C_mask(
        .instr_tb(C_instr), .type_tb(C_type),
        .fields(C_fields),
        .rmask(C_rmask), .wmask(C_wmask)
    );

    logic can_swap = &(
        (B_wmask & (C_rmask | C_wmask)) |
        (C_wmask & (B_rmask | B_wmask)));

    always_comb begin
        if (B_type[`BRANCH]) begin
            pc = bpc;
            instr = bf;
            pred_pc = bpc;
            pred_instr = bf;
            result = `INSERT_NOP;
        end else begin
            pred_pc = cpc;
            pred_instr = data;

            if (can_swap & (C_type[`BRANCH] | C_instr[`LW])) begin
                pc = cpc;
                instr = data;
                result = `POP_DATA;
            end else begin
                pc = bpc;
                instr = bf;
                result = `POP_BUF;
            end
        end
    end
endmodule