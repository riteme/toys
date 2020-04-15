module Extension(
    input logic sign_ext,
    input logic [15:0] in,
    output logic [31:0] out
);
    assign out = sign_ext ? {{16{in[15]}}, in} : {16'b0, in};
endmodule