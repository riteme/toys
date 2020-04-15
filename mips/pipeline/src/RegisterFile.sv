module RegisterFile(
    input logic clk, reset,
    input logic [4:0] r1, r2, r3,
    input logic [31:0] data,
    output logic [31:0] v1, v2
);
    logic [31:0] v[31:0];

    assign v[0] = 0;  // $0 is hardcoded as 0
    assign v1 = v[r1];
    assign v2 = v[r2];

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 1; i < 32; i++)  // do not reset $0
                v[i] <= 0;
        end else if (r3 != 0) begin  // r3 > 0 to enable write
            v[r3] <= data;
        end
    end
endmodule