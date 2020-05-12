`include "cache.vh"

module LRUStrategy #(
    SET_SIZE = `CACHE_E,
    _KEY_WIDTH = $clog2(SET_SIZE)
) (
    input logic [31:0] tick[SET_SIZE],
    output logic [_KEY_WIDTH - 1:0] out
);
    /**
     * some strange bug.
     * this one does not select the first available line.
     */
    // logic [_KEY_WIDTH:0] pos;
    // logic [_KEY_WIDTH:0] index;
    // assign index = pos - 1;
    // assign out = index[_KEY_WIDTH - 1:0];

    // RecursiveNode #(
    //     .LEFT(1),
    //     .RIGHT(SET_SIZE),
    //     .KEY_WIDTH(_KEY_WIDTH + 1)
    // ) root(
    //     .tick(tick), .out(pos)
    // );

    // logic __unused_ok = &{1'b0, index[_KEY_WIDTH], 1'b0};

    /**
     * heap-style tournament tree.
     *
     * warning: verilator UNOPTFLAT. See Comparer.sv.
     */
    logic [_KEY_WIDTH - 1:0] keys[1:2 * SET_SIZE - 1];
    logic [31:0] vals[1:2 * SET_SIZE - 1];

    assign out = keys[1];

    for (genvar i = 0; i < SET_SIZE; i++) begin
        assign keys[SET_SIZE + i] = i;
        assign vals[SET_SIZE + i] = tick[i];
    end

    for (genvar i = 1; i < SET_SIZE; i++) begin
        Comparer #(
            .KEY_WIDTH(_KEY_WIDTH),
            .VAL_WIDTH(32)
        ) node(
            .key1(keys[2 * i]), .val1(vals[2 * i]),
            .key2(keys[2 * i + 1]), .val2(vals[2 * i + 1]),
            .key(keys[i]), .val(vals[i])
        );
    end
endmodule