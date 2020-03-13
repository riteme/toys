`timescale 1ps / 1ps

module dmem #(
    parameter RAMSIZE=128
) (
        input logic clk, we,
        input logic [31:0] a, wd,
        output logic [31:0] rd
    );
    logic [31:0] RAM[RAMSIZE];
    assign rd = RAM[a[31:2]];
    
    always_ff @(posedge clk)
    if (we)
        RAM[a[31:2]] <= wd;
endmodule
