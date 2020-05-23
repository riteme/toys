module SaturateCounter #(
    WIDTH = 2,
    _MAX = 2**WIDTH - 1,
    _MIN = 0
) (
    input logic taken,
    input logic [WIDTH - 1:0] in,
    output logic [WIDTH - 1:0] out
);
    always_comb
    case (taken)
        0: out = in > _MIN ? in - 1 : in;
        1: out = in < _MAX ? in + 1 : in;
    endcase
endmodule