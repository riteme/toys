/**
 * branch history tracker
 */
module BHT #(
    IWIDTH = 6,
    HWIDTH = 6,
    _SIZE = 2**IWIDTH
) (
    input logic clk, reset, en,

    /**
     * refer to PHT
     */
    input logic do_update, last_taken,

    /**
     * index: index hashed from branch instr address.
     * out: the stored history record.
     */
    input logic [IWIDTH - 1:0] index,
    output logic [HWIDTH - 1:0] out
);
    logic [IWIDTH - 1:0] last_index;
    logic [HWIDTH - 1:0] tb[_SIZE];

    assign out = tb[index];

    logic [HWIDTH - 1:0] shifted;
    assign shifted = {tb[last_index][HWIDTH - 2:0], last_taken};

    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            for (int i = 0; i < _SIZE; i++)
                tb[i] <= 0;
            last_index <= 0;
        end else if (en) begin
            if (do_update) begin
                tb[last_index] <= shifted;
            end
            last_index <= index;
        end
    end
endmodule