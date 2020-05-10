module Counter #(
    START_COUNT = 0
) (
    input logic clk, reset, en,
    output logic [31:0] out
);
    always_ff @(posedge clk) begin
        if (reset)
            out <= START_COUNT;
        else if (en)
            out <= out + 1;
    end
endmodule