module ALU(
    input logic [3:0] op,
    input logic [31:0] v1, v2,
    output logic [31:0] out
);
    // from MIPS: op ^ {{2{op[5]}}, 2'b00}
    always_comb begin
        case (op)
            4'b0000: out = v2 << v1;
            4'b0001: out = v2 <<< v1;
            4'b0010: out = v2 >> v1;
            4'b0011: out = v2 >>> v1;

            4'b1100: out = v1 + v2;
            4'b1110: out = v1 - v2;
            4'b1000: out = v1 & v2;
            4'b1001: out = v1 | v2;
            4'b1010: out = v1 ^ v2;
            4'b1011: out = ~(v1 | v2);
            4'b0110: out = {31'b0, $signed(v1) < $signed(v2)};  // signed comparison

            4'b1111: out = v2;
            default: out = 0;
        endcase
    end
endmodule