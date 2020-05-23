/**
 * global history tracker
 */
module GHT #(
    HWIDTH = 6
) (
    input logic clk, reset, en,
    input logic do_update, last_taken,
    output logic [HWIDTH - 1:0] out
);
    always_ff @(posedge clk, posedge reset) begin
        if (reset) begin
            out <= 0;
        end else if (en && do_update) begin
            out <= {out[HWIDTH - 2:0], last_taken};
        end
    end
endmodule