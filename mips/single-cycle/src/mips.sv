module mips(
    input logic clk, reset,
    input logic [31:0] instr,
    input logic [31:0] readdata,

    output logic [31:0] pc,
    output logic memwrite,
    output logic [31:0] aluout, writedata
);
    Core core(
        .CLK(clk), .RESET(reset),
        .INSTR(instr), .MEM_READ(readdata),
        .SIG_WRITE(memwrite),
        .PC(pc),
        .MEM_ADDR(aluout),
        .MEM_DATA(writedata)
    );
endmodule