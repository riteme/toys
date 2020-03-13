`timescale 1ps / 1ps

module imem #(
    parameter RAMSIZE = 64
) (
        input logic [5:0] a,
        output logic [31:0] rd
    );
    logic [31:0] RAM[64];
    assign rd = RAM[a];
endmodule
