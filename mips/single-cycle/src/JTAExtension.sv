module JTAExtension(
    input logic [25:0] ADDR,
    input logic [3:0] PC_HEAD,
    output logic [31:0] RET
);
    assign RET = {PC_HEAD, ADDR, 2'b0};
endmodule