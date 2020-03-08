module BTAExtension(
    input logic [31:0] OFFSET,
    input logic [31:0] PC,
    output logic [31:0] RET
);
    assign RET = PC + (OFFSET << 2);
endmodule