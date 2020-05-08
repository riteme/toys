module Aggregate #(
    VAL_WIDTH = 32,
    ARR_SIZE = 8
) (
    input logic [VAL_WIDTH - 1:0] array_in[ARR_SIZE],
    output logic [VAL_WIDTH - 1:0] out
);
    always_comb begin
        out = 0;
        for (int i = 0; i < ARR_SIZE; i++)
            out |= array_in[i];
    end
endmodule