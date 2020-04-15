module MemoryStage(
    input logic [4:0] rd,
    input logic [31:0] out, vt,
    input logic data_src,

    output logic [31:0] addr, mem_data,
    input logic [31:0] mem_out,

    output logic [4:0] r3,
    output logic [31:0] data
);
    assign addr = out;
    assign mem_data = vt;
    assign data = data_src ? mem_out : out;
    assign r3 = rd;
endmodule