/* verilator lint_off UNOPTFLAT */

module Comparer #(
    KEY_WIDTH = 8,
    VAL_WIDTH = 32
) (
    input logic [KEY_WIDTH - 1:0] key1, key2,
    input logic [VAL_WIDTH - 1:0] val1, val2,
    output logic [KEY_WIDTH - 1:0] key,
    output logic [VAL_WIDTH - 1:0] val
);
    logic cmp;
    assign cmp = val1 <= val2;

    assign key = cmp ? key1 : key2;
    assign val = cmp ? val1 : val2;
endmodule