module Core(
    input logic CLK, RESET,
    input logic [31:0] INSTR, MEM_READ,
    output logic SIG_WRITE,
    output logic [31:0] PC, MEM_ADDR, MEM_DATA
);
    logic [15:0] Signals;
    logic [5:0] Opcode, Function;

    assign SIG_WRITE = Signals[15];

    Datapath _DP(
        .CLK(CLK), .RESET(RESET),
        .SIG(Signals[14:0]), .INSTR(INSTR),
        .MEM_READ(MEM_READ),

        .PC(PC),
        .OP(Opcode), .FUNC(Function),
        .MEM_ADDR(MEM_ADDR), .MEM_DATA(MEM_DATA)
    );
    Controller _CTRL(
        .OP(Opcode), .FUNC(Function),
        .SIG(Signals)
    );
endmodule