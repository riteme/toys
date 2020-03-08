module Datapath(
    input logic CLK, RESET,
    input logic [14:0] SIG,  // WriteMemory excluded
    input logic [31:0] INSTR,
    input logic [31:0] MEM_READ,

    output logic [31:0] PC,
    output logic [5:0] OP, FUNC,
    output logic [31:0] MEM_ADDR, MEM_DATA
);
    /* Branch & PC */
    logic [31:0] NextPC, Branch[4];
    logic [1:0] BranchID;

    always_ff @(posedge CLK)
        PC <= RESET ? 0 : Branch[BranchID];

    assign NextPC = PC + 4;
    assign Branch[0] = NextPC;  // normal advance

    /* Instruction Decode */
    logic [4:0] rs, rt, rd, shamt;
    logic [15:0] imm;
    logic [25:0] addr;
    logic [31:0] ExtImm;

    assign {  // R-type
        OP, rs, rt, rd, shamt, FUNC
    } = INSTR;
    assign imm = INSTR[15:0];
    assign addr = INSTR[25:0];

    SignExtension _SignExt(
        .IMM(imm), .SIGN(SIG[5]), .RET(ExtImm)
    );
    JTAExtension _JTA(  // direct jump
        .ADDR(addr), .PC_HEAD(NextPC[31:28]), .RET(Branch[1])
    );
    BTAExtension _BTA(  // conditional branch
        .OFFSET(ExtImm), .PC(NextPC), .RET(Branch[2])
    );

    /* Register File */
    logic [4:0] RegDst;
    logic [31:0] RegData, RegRead[2];

    RegisterFile Reg(
        .CLK(CLK), .RESET(RESET), .WE(SIG[4]),
        .A1(rs), .A2(rt), .A3(RegDst),
        .WD(SIG[3] ? NextPC : RegData),  // SavePC
        .RD1(RegRead[0]), .RD2(RegRead[1])
    );

    assign RegDst = SIG[3] ?
        5'b11111 :  // $ra = $31
        (SIG[2] ? rt : rd);
    assign Branch[3] = RegRead[0];  // indirect jump

    /* ALU */
    logic [31:0] SrcA, SrcB, ALURet;
    logic ALUZero;

    ALU alu(
        .OP(SIG[14:9]),
        .A(SrcA), .B(SrcB), .RET(ALURet),
        .Z(ALUZero)
    );

    assign SrcA = SIG[6] ? {27'b0, shamt} : RegRead[0];
    assign SrcB = SIG[8] ? RegRead[1] : ExtImm;
    assign RegData = SIG[7] ? ALURet : MEM_READ;
    assign BranchID = SIG[1:0] & {2{ALUZero}};

    /* Memory */
    assign MEM_ADDR = ALURet;
    assign MEM_DATA = RegRead[1];
endmodule