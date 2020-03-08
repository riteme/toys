module SignExtension(
    input logic [15:0] IMM,
    input logic SIGN,
    output logic [31:0] RET
);
    assign RET = SIGN ?
        {{16{IMM[15]}}, IMM} :
        {16'b0, IMM};
endmodule