module Counter #(
    START_COUNT = 0
) (
    input logic clk, reset, en,
    output logic [31:0] count
);
    always_ff @(posedge clk) begin
        if (reset)
            count <= START_COUNT;
        else if (en)
            count <= count + 1;
    end
endmodule