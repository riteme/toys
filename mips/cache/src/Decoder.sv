module Decoder #(
    WIDTH = 4,
    _OUT_SIZE = 2**WIDTH
) (
    input [WIDTH - 1:0] idx,
    output [_OUT_SIZE - 1:0] out
);
    assign out = 1 << idx;
endmodule