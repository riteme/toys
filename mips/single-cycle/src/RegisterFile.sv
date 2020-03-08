module RegisterFile(
    input logic CLK, RESET, WE,
    input logic [4:0] A1, A2, A3,
    input logic [31:0] WD,
    output logic [31:0] RD1, RD2
);
    logic [31:0] Registers [32];

    assign RD1 = Registers[A1];
    assign RD2 = Registers[A2];

    always_ff @(posedge CLK) begin
        if (RESET) begin
            for (int i = 0; i < 32; i++) begin
                Registers[i] <= {32{1'b0}};
            end
        end
        else if (WE)
            Registers[A3] <= WD;
    end
endmodule