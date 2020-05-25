typedef logic [15:0] HashWord;

/**
 * 16-bit linear hash.
 */
module HashFunction(
    input HashWord in,
    output HashWord out
);
    localparam HashWord M[16] = '{
        16'hAAAA, 16'h5555, 16'h33B3, 16'hC85F,
        16'h34E7, 16'hD6A3, 16'hBFAA, 16'hFB4E,
        16'h7DC9, 16'h43F7, 16'h89B7, 16'hA124,
        16'h75A1, 16'h11BC, 16'hDB12, 16'h38FF
    };

    HashWord lev1[8];
    for (genvar i = 0; i < 8; i++) begin
        assign lev1[i] =
            (in[2 * i] ? M[2 * i] : 0) ^
            (in[2 * i + 1] ? M[2 * i + 1] : 0);
    end

    HashWord lev2[4];
    for (genvar i = 0; i < 4; i++) begin
        assign lev2[i] = lev1[2 * i] ^ lev1[2 * i + 1];
    end

    assign out = lev2[0] ^ lev2[1] ^ lev2[2] ^ lev2[3];

    /**
     * elaboration gives a serial design.
     */
    // always_comb begin
    //     out = 0;
    //     for (int i = 0; i < 16; i++) begin
    //         out ^= in[i] ? M[i] : 0;
    //     end
    // end
endmodule