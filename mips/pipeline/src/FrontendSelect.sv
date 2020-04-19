`include "Common.vh"

module FrontendSelect(
    input logic [31:0] cpc, data, bpc, bf,
    output logic req,
    output logic [1:0] result,
    output logic [31:0] pc, instr, pred_pc, pred_instr
);
    logic [`MAX_INSTR_ID:0] B_instr;
    logic [`MAX_TYPE_ID:0] B_type;
    logic [`MAX_INSTR_ID:0] C_instr;
    logic [`MAX_TYPE_ID:0] C_type;
    InstructionTraits _B_traits(
        .opcode(bf[31:26]), .funct(bf[5:0]),
        .instr_tb(B_instr), .type_tb(B_type)
    );
    InstructionTraits _C_traits(
        .opcode(data[31:26]), .funct(data[5:0]),
        .instr_tb(C_instr), .type_tb(C_type)
    );

    logic [14:0] B_fields, C_fields;
    assign B_fields = bf[25:11];
    assign C_fields = data[25:11];

    logic [30:0] B_rmask, B_wmask;
    logic [30:0] C_rmask, C_wmask;
    RWMask _B_mask(
        .instr_tb(B_instr), .type_tb(B_type),
        .fields(B_fields),
        .rmask(B_rmask), .wmask(B_wmask)
    );
    RWMask _C_mask(
        .instr_tb(C_instr), .type_tb(C_type),
        .fields(C_fields),
        .rmask(C_rmask), .wmask(C_wmask)
    );

    logic [63:0] emit, pred;
    logic [63:0] B, C;
    assign B = {bpc, bf};
    assign C = {cpc, data};
    assign {pc, instr} = emit;
    assign {pred_pc, pred_instr} = pred;

    logic can_swap;
    assign can_swap = ~|(
        (B_wmask & (C_rmask | C_wmask)) |
        (C_wmask & (B_rmask | B_wmask)));

    logic allow_swap;  // avoid too aggressive heuristics
    assign allow_swap = C_instr[`LW] && !B_instr[`LW];

    logic jmp_after_lw;  // prevent swapping lw & jump
    assign jmp_after_lw = B_instr[`LW] && C_type[`JTYPE];

    always_comb begin
        if (B_type[`BRANCH]) begin
            emit = B;
            pred = B;
            result = `INSERT_NOP;
            req = 0;
        end else if (can_swap && C_type[`BRANCH] && !jmp_after_lw) begin
            emit = C;
            pred = C;
            result = `POP_DATA;
            req = 0;
        end else if (bf == 0)  begin  // skip `nop`
            emit = C;
            pred = C;
            result = `POP_DATA;
            req = 1;
        end else if (can_swap && allow_swap) begin
            emit = C;
            pred = C;
            result = `POP_DATA;
            req = 0;
        end else begin
            emit = B;
            pred = C;
            result = `POP_BUF;
            req = 0;
        end
    end
endmodule