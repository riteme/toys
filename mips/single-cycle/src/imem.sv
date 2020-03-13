`timescale 1ps / 1ps

module imem #(
    parameter RAMSIZE = 64
) (
        input logic [29:0] a,
        output logic [31:0] rd
    );
    logic [31:0] RAM[RAMSIZE];
    assign rd = RAM[a];
endmodule
